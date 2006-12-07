// $Id$

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "WinShare.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/defs.h"
#endif

#include "WinShare.h"
#include <wx/utils.h>
#include <wx/dynlib.h>
#include <wx/arrimpl.cpp>

#ifdef __WXMSW__
#include <lm.h>

struct share_info_1 {
    char		shi1_netname[LM20_NNLEN+1];
    char		shi1_pad1;
    unsigned short	shi1_type;
    char FAR *		shi1_remark;
};  /* share_info_1 */

typedef NET_API_STATUS (__stdcall *NT_NetShareEnum)(LPWSTR svname, DWORD level, LPBYTE* bufptr, DWORD prefmaxlen,
        LPDWORD entriesread, LPDWORD totalentries, LPDWORD resume_handle);
typedef NET_API_STATUS (__stdcall *NT_NetApiBufferFree)(LPVOID buffer);
typedef NET_API_STATUS (__stdcall *W9X_NetShareEnum)(const char *svname, short sLevel, char *pBuffer,
        unsigned short cbBuffer, unsigned short *pcEntriesRead,
        unsigned short *pcTotalAvail);
// dummy typedefs for libsmbclient
typedef void (*SMBC_init)(void);
typedef void (*SMBC_opendir)(void);
typedef void* (*SMBC_readdir)(void);
typedef void (*SMBC_closedir)(void);
// dummy typedefs for cups
typedef void cups_dest_t;
typedef int (*FP_cupsGetDests)(cups_dest_t ** dests);
typedef const char * (*FP_cupsServer)();
#else
// Unix
#include <cups/cups.h>
#include <libsmbclient.h>

typedef int (*SMBC_init)(smbc_get_auth_data_fn fn, int debug);
typedef int (*SMBC_opendir)(const char *url);
typedef struct smbc_dirent* (*SMBC_readdir)(unsigned int dh);
typedef int (*SMBC_closedir)(int dh);

// dummy typedefs for windows funcptrs
typedef void (*NT_NetShareEnum)(void);
typedef void (*NT_NetApiBufferFree)(void);
typedef void (*W9X_NetShareEnum)(void);

typedef int (*FP_cupsGetDests)(cups_dest_t ** dests);
typedef const char * (*FP_cupsServer)();

// authentication-callback, used by libsmbclient
// we always return empty username and password which means "do anonymous logon"
void smbc_auth_fn(const char *, const char *, char *, int,
        char *username, int unmaxlen, char *password, int pwmaxlen)
{
    *username = '\0'; *password = '\0';
}
#endif

WX_DEFINE_OBJARRAY(ArrayOfShares);

class DllData
{
    public:
        typedef enum {
            SmbClientWinNT,
            SmbClientWin95,
            SmbClientUnix,
            CupsClientUnix,
        } ClientType;

        DllData(ClientType);
        ~DllData();

        ArrayOfShares GetShares();
        bool IsAvailable();

    private:
        bool isNT;
        bool isSMBC;
        wxDllType handle;
        NT_NetShareEnum NT_enum;
        NT_NetApiBufferFree NT_free;
        W9X_NetShareEnum W9X_enum;
        SMBC_init C_init;
        SMBC_opendir C_opendir;
        SMBC_readdir C_readdir;
        SMBC_closedir C_closedir;
        FP_cupsGetDests cupsGetDests;
        FP_cupsServer cupsServer;
};

DllData::DllData(ClientType ct)
{
    isNT = false;
    isSMBC = false;
    switch (ct) {
        case SmbClientUnix:
            {
                wxDynamicLibrary dll(_T("libsmbclient"));
                C_init = (SMBC_init)dll.GetSymbol(_T("smbc_init"));
                C_opendir = (SMBC_opendir)dll.GetSymbol(_T("smbc_opendir"));
                C_readdir = (SMBC_readdir)dll.GetSymbol(_T("smbc_readdir"));
                C_closedir = (SMBC_closedir)dll.GetSymbol(_T("smbc_closedir"));
                handle = dll.Detach();
                isSMBC = true;
            }
            break;
        case SmbClientWinNT:
            {
                wxDynamicLibrary dll(_T("netapi32"));
                NT_enum = (NT_NetShareEnum)dll.GetSymbol(_T("NetShareEnum"));
                NT_free = (NT_NetApiBufferFree)dll.GetSymbol(_T("NetApiBufferFree"));
                handle = dll.Detach();
                isNT = true;
            }
            break;
        case SmbClientWin95:
            {
                wxDynamicLibrary dll(_T("svrapi32"));
                W9X_enum = (W9X_NetShareEnum)dll.GetSymbol(_T("NetShareEnum"));
                handle = dll.Detach();
            }
            break;
        case CupsClientUnix:
            {
                wxDynamicLibrary dll(_T("libcups"));
                cupsGetDests = (FP_cupsGetDests)dll.GetSymbol(_T("cupsGetDests"));
                cupsServer = (FP_cupsServer)dll.GetSymbol(_T("cupsServer"));
                handle = dll.Detach();
            }
            break;
    }
}

DllData::~DllData()
{
    wxDynamicLibrary::Unload(handle);
}

ArrayOfShares DllData::GetShares()
{
    ArrayOfShares sa;
#ifdef __WXMSW__
    if (isNT) {
        // NT, Win2K, XP
        PSHARE_INFO_1 BufPtr, p;
        NET_API_STATUS res;
        DWORD er = 0, tr = 0, resume = 0, i;
        do {
            res = NT_enum(NULL, 1, (LPBYTE *)&BufPtr, (DWORD)-1, &er, &tr, &resume);
            if (res == ERROR_SUCCESS || res == ERROR_MORE_DATA) {
                p = BufPtr;
                for (i = 1; i <= er; i++) {
                    SharedResource r;
                    switch (p->shi1_type) {
                        case STYPE_DISKTREE:
                        case STYPE_PRINTQ:
                            r.name = wxConvLocal.cWC2WX((const wchar_t*)p->shi1_netname);
                            r.description = wxConvLocal.cWC2WX((const wchar_t*)p->shi1_remark);
                            r.sharetype = (p->shi1_type == STYPE_DISKTREE) ?
                                SharedResource::SHARE_SMB_DISK : SharedResource::SHARE_SMB_PRINTER;
                            if (r.name != _T("print$"))
                                sa.Add(r);
                            break;
                    }
                    p++;
                }
                NT_free(BufPtr);
            }
        } while (res == ERROR_MORE_DATA);
    } else {
        // Win 95, 98, Me
        char *buf = (char *)malloc(sizeof(share_info_1));
        unsigned short cbBuffer = 0;
        unsigned short nEntries = 0;
        unsigned short nTotal = 0;

        if (buf) {
            // First, get number of total entries
            NET_API_STATUS nStatus = W9X_enum(NULL, 1, buf, cbBuffer, &nEntries, &nTotal);
            if (nTotal) {
                free(buf);
                cbBuffer = nTotal * sizeof(share_info_1);
                buf = (char *)malloc(cbBuffer);
                if (buf) {
                    nStatus = W9X_enum(NULL, 1, buf, cbBuffer, &nEntries, &nTotal);
                    if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)) {
                        struct share_info_1 *p = (struct share_info_1 *)buf;
                        for (int i = 0; (i < nEntries); i++) {
                            if (p) {
                                SharedResource r;
                                switch (p->shi1_type) {
                                    case STYPE_DISKTREE:
                                    case STYPE_PRINTQ:
                                        r.name = wxConvUTF8.cMB2WX(p->shi1_netname);
                                        r.description = wxConvUTF8.cMB2WX(p->shi1_remark);
                                        r.sharetype = (p->shi1_type == STYPE_DISKTREE) ?
                                            SharedResource::SHARE_SMB_DISK : SharedResource::SHARE_SMB_PRINTER;
                                        if (r.name != _T("print$"))
                                            sa.Add(r);
                                        break;
                                }
                                p++;
                            }
                        }
                    }
                    free(buf);
                }
            }
        }
    }
#else
    if (isSMBC) {
        // Unix, use libsmbclient
        if (C_init(smbc_auth_fn, 0) == 0) {
            int d = C_opendir("smb://127.0.0.1/");
            if (d >= 0) {
                struct smbc_dirent *e;
                while (e = C_readdir(d)) {
                    SharedResource r;
                    switch (e->smbc_type) {
                        case SMBC_FILE_SHARE:
                        case SMBC_PRINTER_SHARE:
                            r.name = wxConvLocal.cMB2WX(e->name);
                            r.description = wxConvLocal.cMB2WX(e->comment);
                            r.sharetype = (e->smbc_type == SMBC_FILE_SHARE) ?
                                SharedResource::SHARE_SMB_DISK : SharedResource::SHARE_SMB_PRINTER;
                            sa.Add(r);
                            break;
                    }
                }
                C_closedir(d);
            }
        }
    } else {
        // Unix, use libcups
        cups_dest_t *dests = NULL;
        int ndests = cupsGetDests(&dests);
        for (int i = 0; i < ndests; i++) {
            SharedResource r;
            r.name = wxConvLocal.cMB2WX(dests[i].name);
            r.description = wxConvLocal.cMB2WX(dests[i].instance);
            r.sharetype = SharedResource::SHARE_CUPS_PRINTER;
        }
    }
#endif
    return sa;
}

bool DllData::IsAvailable()
{
    bool ret = false;
#ifdef __UNIX__
    if (isSMBC) {
        // Probe Samba
        if (C_init(smbc_auth_fn, 0) == 0) {
            int d = C_opendir("smb://127.0.0.1/");
            if (d >= 0) {
                ret = true;
                C_closedir(d);
            }
        }
    } else {
        // Probe Cups
        return (cupsServer() != NULL);
    }
#endif
    return ret;
}

    WinShare::WinShare()
: dllPrivate(NULL)
{
    int vMajor, vMinor;

    switch (::wxGetOsVersion(&vMajor, &vMinor)) {
        case wxWINDOWS_NT:
            dllPrivate = new DllData(DllData::SmbClientWinNT);
            break;
        case wxWIN95:
            dllPrivate = new DllData(DllData::SmbClientWin95);
            break;
        case wxGTK:
            dllPrivate = new DllData(DllData::SmbClientUnix);
            break;
    }
}

WinShare::~WinShare()
{
    if (dllPrivate)
        delete dllPrivate;
}

ArrayOfShares WinShare::GetShares()
{
    if (dllPrivate)
        m_shares = dllPrivate->GetShares();
    return m_shares;
}

bool WinShare::IsAvailable()
{
#ifdef __WXWIN__
    return true;
#else
    return dllPrivate && dllPrivate->IsAvailable();
#endif
}

    CupsShare::CupsShare()
: dllPrivate(NULL)
{
#ifdef __UNIX__
    dllPrivate = new DllData(DllData::CupsClientUnix);
#endif
}

CupsShare::~CupsShare()
{
    if (dllPrivate)
        delete dllPrivate;
}

ArrayOfShares CupsShare::GetShares()
{
#ifdef __UNIX__
    if (dllPrivate)
        m_shares = dllPrivate->GetShares();
#endif
    return m_shares;
}

bool CupsShare::IsAvailable()
{
    return dllPrivate && dllPrivate->IsAvailable();
}


// MyXmlConfig.h: interface for the MyXmlConfig class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MXXMLCONFIG_H_
#define _MXXMLCONFIG_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "MyXmlConfig.cpp"
#endif

#include <wx/dynarray.h>
#include <wx/string.h>
#include <wx/object.h>

class wxXmlNode;

class ShareGroup : public wxObject
{
public:
    ShareGroup() : wxObject() {}
    virtual ~ShareGroup() {}

    bool operator ==(const ShareGroup &);
    bool operator !=(const ShareGroup &);

    bool m_bMountNow;
    int m_iType;
    wxString m_sGroupName;
    wxString m_sMountPoint;
    wxString m_sName;
    wxString m_sUsername;
    wxString m_sPassword;
};

WX_DECLARE_OBJARRAY(ShareGroup, ArrayOfShareGroups);

class MyXmlConfig
{
public:
    typedef enum {
        SPEED_MODEM = 0,
        SPEED_ISDN,
        SPEED_ADSL,
        SPEED_WAN,
        SPEED_LAN,
    } ConnectionSpeed;

    typedef enum {
        STYPE_UNIX = 0,
        STYPE_WINDOWS,
        STYPE_VNC,
    } SessionType;

    typedef enum {
        DTYPE_RDP = -2,
        DTYPE_RFB = -1,
        DTYPE_KDE = 0,
        DTYPE_GNOME,
        DTYPE_CDE,
        DTYPE_XFCE,
        DTYPE_CUSTOM
    } DesktopType;

    typedef enum {
        DPTYPE_640x480 = 0,
        DPTYPE_800x600,
        DPTYPE_1024x768,
        DPTYPE_AVAILABLE,
        DPTYPE_FULLSCREEN,
        DPTYPE_CUSTOM
    } DisplayType;

    typedef enum {
        CACHEMEM_0MB = 0,
        CACHEMEM_1MB,
        CACHEMEM_2MB,
        CACHEMEM_4MB,
        CACHEMEM_8MB,
        CACHEMEM_16MB,
        CACHEMEM_32MB,
    } CacheMemory;

    typedef enum CacheDisk {
        CACHEDISK_0MB = 0,
        CACHEDISK_4MB,
        CACHEDISK_8MB,
        CACHEDISK_16MB,
        CACHEDISK_32MB,
        CACHEDISK_64MB,
        CACHEDISK_128MB,
    } CacheDisk;

    MyXmlConfig();
    MyXmlConfig(const wxString &);
    virtual ~MyXmlConfig();
    
    MyXmlConfig &operator =(const MyXmlConfig &);
    bool operator ==(const MyXmlConfig &);

    void saveState();
    bool checkChanged();
    bool SaveToFile();

    wxString sGetSessionParams(const wxString &);

    bool IsValid() { return m_bValid; }

    bool bGetDisableBackingstore() { return m_bDisableBackingstore; }
    bool bGetDisableComposite() { return m_bDisableComposite; }
    bool bGetDisableImageCompression() { return m_bDisableImageCompression; }
    bool bGetDisableJpeg() { return m_bDisableJpeg; }
    bool bGetDisableRender() { return m_bDisableRender; }
    bool bGetDisableStreaming() { return m_bDisableStreaming; }
    bool bGetDisableTaint() { return m_bDisableTaint; }
    bool bGetDisableTcpNoDelay() { return m_bDisableTcpNoDelay; }
    bool bGetDisableXagent() { return m_bDisableXagent; }
    bool bGetDisableZlibCompression() { return m_bDisableZlibCompression; }
    bool bGetEnableMultimedia() { return m_bEnableMultimedia; }
    bool bGetEnableSmbSharing() { return m_bEnableSmbSharing; }
    bool bGetEnableSSL() { return m_bEnableSSL; }
    bool bGetKbdLayoutOther() { return m_bKbdLayoutOther; }
    bool bGetRdpRememberPassword() { return m_bRdpRememberPassword; }
    bool bGetRdpRunApplication() { return m_bRdpRunApplication; }
    bool bGetRememberPassword() { return m_bRememberPassword; }
    bool bGetRemoveOldSessionFiles() { return m_bRemoveOldSessionFiles; }
    bool bGetRunConsole() { return m_bRunConsole; }
    bool bGetRunXclients() { return m_bRunXclients; }
    bool bGetUseCups() { return m_bUseCups; }
    bool bGetUseCustomImageEncoding() { return m_bUseCustomImageEncoding; }
    bool bGetImageEncodingJpeg() { return m_bImageEncodingJpeg; }
    bool bGetUseJpegQuality() { return m_bUseJpegQuality; }
    bool bGetImageEncodingPlainX() { return m_bImageEncodingPlainX; }
    bool bGetImageEncodingPNG() { return m_bImageEncodingPNG; }
    bool bGetUseProxy() { return m_bUseProxy; }
    bool bGetUseSmartCard() { return m_bUseSmartCard; }
    bool bGetUseTightJpeg() { return m_bUseTightJpeg; }
    bool bGetVirtualDesktop() { return m_bVirtualDesktop; }
    bool bGetVncRememberPassword() { return m_bVncRememberPassword; }
    bool bGetVncUseMxAuth() { return m_bVncUseMxAuth; }
    
    MyXmlConfig::ConnectionSpeed eGetConnectionSpeed() { return m_eConnectionSpeed; }
    MyXmlConfig::DesktopType eGetDesktopType() { return m_eDesktopType; }
    MyXmlConfig::DisplayType eGetDisplayType() { return m_eDisplayType; }
    MyXmlConfig::SessionType eGetSessionType() { return m_eSessionType; }
    MyXmlConfig::CacheMemory eGetCacheMemory() { return m_eCacheMemory; }
    MyXmlConfig::CacheDisk eGetCacheDisk() { return m_eCacheDisk; }

    int iGetCupsPort() { return m_iCupsPort; }
    int iGetDisplayHeight() { return m_iDisplayHeight; }
    int iGetDisplayWidth() { return m_iDisplayWidth; }
    int iGetJpegQuality() { return m_iJpegQuality; }
    int iGetKbdLayoutLanguage() { return m_iKbdLayoutLanguage; }
    int iGetProxyPort() { return m_iProxyPort; }
    int iGetRdpAuthType() { return m_iRdpAuthType; }
    int iGetRdpImageCompression() { return m_iRdpImageCompression; }
    int iGetServerPort() { return m_iServerPort; }
    int iGetUsedShareGroups() { return m_iUsedShareGroups; }
    int iGetVncDisplayNumber() { return m_iVncDisplayNumber; }
    int iGetVncImageEncoding() { return m_iVncImageEncoding; }

    wxString sGetCommandLine() { return m_sCommandLine; }
    wxString sGetCupsPath() { return m_sCupsPath; }
    wxString sGetFileName() { return m_sFileName; }
    wxString sGetName() { return m_sName; }
    wxString sGetPassword() { return m_sPassword; }
    wxString sGetProxyHost() { return m_sProxyHost; }
    wxString sGetRdpApplication() { return m_sRdpApplication; }
    wxString sGetRdpHostName() { return m_sRdpHostName; }
    wxString sGetRdpPassword() { return m_sRdpPassword; }
    wxString sGetRdpUsername() { return m_sRdpUsername; }
    wxString sGetServerHost() { return m_sServerHost; }
    wxString sGetUsername() { return m_sUsername; }
    wxString sGetVncHostName() { return m_sVncHostName; }
    wxString sGetVncPassword() { return m_sVncPassword; }

    wxArrayString aGetUsedShareGroups() { return m_aUsedShareGroups; }
    ArrayOfShareGroups aGetShareGroups() { return m_aShareGroups; }

    void bSetDisableBackingstore(const bool b) { m_bDisableBackingstore = b; }
    void bSetDisableComposite(const bool b) { m_bDisableComposite = b; }
    void bSetDisableImageCompression(const bool b) { m_bDisableImageCompression = b; }
    void bSetDisableJpeg(const bool b) { m_bDisableJpeg = b; }
    void bSetDisableRender(bool b) { m_bDisableRender = b; }
    void bSetDisableStreaming(const bool b) { m_bDisableStreaming = b; }
    void bSetDisableTaint(bool b) { m_bDisableTaint = b; }
    void bSetDisableTcpNoDelay(bool b) { m_bDisableTcpNoDelay = b; }
    void bSetDisableXagent(const bool b) { m_bDisableXagent = b; }
    void bSetDisableZlibCompression(bool b) { m_bDisableZlibCompression = b; }
    void bSetEnableMultimedia(bool b) { m_bEnableMultimedia = b; }
    void bSetEnableSmbSharing(bool b) { m_bEnableSmbSharing = b; }
    void bSetEnableSSL(bool b) { m_bEnableSSL = b; }
    void bSetKbdLayoutOther(bool b) { m_bKbdLayoutOther = b; }
    void bSetRdpRememberPassword(bool b) { m_bRdpRememberPassword = b; }
    void bSetRdpRunApplication(bool b) { m_bRdpRunApplication = b; }
    void bSetRememberPassword(bool b) { m_bRememberPassword = b; }
    void bSetRemoveOldSessionFiles(bool b) { m_bRemoveOldSessionFiles = b; }
    void bSetRunConsole(bool b) { m_bRunConsole = b; }
    void bSetRunXclients(bool b) { m_bRunXclients = b; }
    void bSetUseCups(bool b) { m_bUseCups = b; }
    void bSetUseCustomImageEncoding(bool b) { m_bUseCustomImageEncoding = b; }
    void bSetImageEncodingJpeg(bool b) { m_bImageEncodingJpeg = b; }
    void bSetUseJpegQuality(bool b) { m_bUseJpegQuality = b; }
    void bSetImageEncodingPlainX(bool b) { m_bImageEncodingPlainX = b; }
    void bSetImageEncodingPNG(bool b) { m_bImageEncodingPNG = b; }
    void bSetUseProxy(bool b) { m_bUseProxy = b; }
    void bSetUseSmartCard(bool b) { m_bUseSmartCard = b; }
    void bSetUseTightJpeg(bool b) { m_bUseTightJpeg = b; }
    void bSetVirtualDesktop(bool b) { m_bVirtualDesktop = b; }
    void bSetVncRememberPassword(bool b) { m_bVncRememberPassword = b; }
    void bSetVncUseMxAuth(bool b) { m_bVncUseMxAuth = b; }
    
    void eSetCacheDisk(MyXmlConfig::CacheDisk e) { m_eCacheDisk = e; }
    void eSetCacheMemory(MyXmlConfig::CacheMemory e) { m_eCacheMemory = e; }
    void eSetConnectionSpeed(MyXmlConfig::ConnectionSpeed e) { m_eConnectionSpeed = e; }
    void eSetDesktopType(MyXmlConfig::DesktopType e) { m_eDesktopType = e; }
    void eSetDisplayType(MyXmlConfig::DisplayType e) { m_eDisplayType = e; }
    void eSetSessionType(MyXmlConfig::SessionType e) { m_eSessionType = e; }

    void iSetCupsPort(int i) { m_iCupsPort = i; }
    void iSetDisplayHeight(int i) { m_iDisplayHeight = i; }
    void iSetDisplayWidth(int i) { m_iDisplayWidth = i; }
    void iSetJpegQuality(int i) { m_iJpegQuality = i; }
    void iSetKbdLayoutLanguage(int i) { m_iKbdLayoutLanguage = i; }
    void iSetProxyPort(int i) { m_iProxyPort = i; }
    void iSetRdpAuthType(int i) { m_iRdpAuthType = i; }
    void iSetRdpImageCompression(int i) { m_iRdpImageCompression = i; }
    void iSetServerPort(int i) { m_iServerPort = i; }
    void iSetUsedShareGroups(int i) { m_iUsedShareGroups = i; }
    void iSetVncDisplayNumber(int i) { m_iVncDisplayNumber = i; }
    void iSetVncImageEncoding(int i) { m_iVncImageEncoding = i; }

    void sSetCommandLine(const wxString &s) { m_sCommandLine = s; }
    void sSetCupsPath(const wxString &s) { m_sCupsPath = s; }
    void sSetFileName(const wxString &s) { m_sFileName = s; }
    void sSetName(const wxString &s) { m_sName = s; }
    void sSetPassword(const wxString &s) { m_sPassword = s; }
    void sSetProxyHost(const wxString &s) { m_sProxyHost = s; }
    void sSetRdpApplication(const wxString &s) { m_sRdpApplication = s; }
    void sSetRdpHostName(const wxString &s) { m_sRdpHostName = s; }
    void sSetRdpPassword(const wxString &s) { m_sRdpPassword = s; }
    void sSetRdpUsername(const wxString &s) { m_sRdpUsername = s; }
    void sSetServerHost(const wxString &s) { m_sServerHost = s; }
    void sSetUsername(const wxString &s) { m_sUsername = s; }
    void sSetVncHostName(const wxString &s) { m_sVncHostName = s; }
    void sSetVncPassword(const wxString &s) { m_sVncPassword = s; }

    void aSetShareGroups(const ArrayOfShareGroups &a) { m_aShareGroups = a; }
    void aSetUsedShareGroups(const wxArrayString &a) { m_aUsedShareGroups = a; }

private:

    bool getBool(wxXmlNode *, const wxString &, bool defval = false);
    long getLong(wxXmlNode *, const wxString &, long defval = 0);
    wxString getString(wxXmlNode *, const wxString &, const wxString &defval = _T(""));
    wxString getPassword(wxXmlNode *, const wxString &, const wxString &defval = _T(""));
    wxString *getStringNew(wxXmlNode *, const wxString &, wxString *defval = NULL);
    int mapMxLanguage(const int);
    int mapNxLanguage(const int);
    int mapLocaleToKeyboard();
    wxString isoKbd(const int);
    bool cmpShareGroups(const ArrayOfShareGroups, const ArrayOfShareGroups);
    bool cmpUsedShareGroups(const wxArrayString, const wxArrayString);
    void bAddOption(wxXmlNode *, const wxString &, const bool);
    void iAddOption(wxXmlNode *, const wxString &, const long);
    void iAddOptionBool(wxXmlNode *, const wxString &, const bool);
    void sAddOption(wxXmlNode *, const wxString &, const wxString &);
    wxXmlNode *AddGroup(wxXmlNode *, const wxString &);

    bool m_bDisableBackingstore;
    bool m_bDisableComposite;
    bool m_bDisableImageCompression;
    bool m_bDisableJpeg;
    bool m_bDisableRender;
    bool m_bDisableStreaming;
    bool m_bDisableTaint;
    bool m_bDisableTcpNoDelay;
    bool m_bDisableXagent;
    bool m_bDisableZlibCompression;
    bool m_bEnableSmbSharing;
    bool m_bEnableMultimedia;
    bool m_bEnableSSL;
    bool m_bKbdLayoutOther;
    bool m_bRdpRememberPassword;
    bool m_bRdpRunApplication;
    bool m_bRememberPassword;
    bool m_bRemoveOldSessionFiles;
    bool m_bRunConsole;
    bool m_bRunXclients;
    bool m_bUseCups;
    bool m_bUseCustomImageEncoding;
    bool m_bImageEncodingJpeg;
    bool m_bUseJpegQuality;
    bool m_bImageEncodingPlainX;
    bool m_bImageEncodingPNG;
    bool m_bUseProxy;
    bool m_bUseSmartCard;
    bool m_bUseTightJpeg;
    bool m_bValid;
    bool m_bVirtualDesktop;
    bool m_bVncRememberPassword;
    bool m_bVncUseMxAuth;

    int m_iCupsPort;
    int m_iDisplayWidth;
    int m_iDisplayHeight;
    int m_iJpegQuality;
    int m_iKbdLayoutLanguage;
    int m_iProxyPort;
    int m_iRdpAuthType;
    int m_iRdpImageCompression;
    int m_iServerPort;
    int m_iUsedShareGroups;
    int m_iVncDisplayNumber;
    int m_iVncImageEncoding;

    wxString m_sCommandLine;
    wxString m_sCupsPath;
    wxString m_sFileName;
    wxString m_sName;
    wxString m_sPassword;
    wxString m_sProxyHost;
    wxString m_sRdpHostName;
    wxString m_sRdpUsername;
    wxString m_sRdpPassword;
    wxString m_sRdpApplication;
    wxString m_sServerHost;
    wxString m_sUsername;
    wxString m_sVncHostName;
    wxString m_sVncPassword;

    ConnectionSpeed m_eConnectionSpeed;
    DesktopType m_eDesktopType;
    DisplayType m_eDisplayType;
    SessionType m_eSessionType;
    CacheMemory m_eCacheMemory;
    CacheDisk m_eCacheDisk;

    ArrayOfShareGroups m_aShareGroups;
    wxArrayString m_aUsedShareGroups;

    MyXmlConfig *saved;
    wxString *m_pMd5Password;
    wxString *m_pClrPassword;
};

#endif
// _MXXMLCONFIG_H_
/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// Author:      Fritz Elfert
// Created:     06/17/04 03:34:48
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


#include <wx/mimetype.h>
#include <wx/filename.h>
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/filesys.h>

#include "mxclientApp.h"

static char * broken_xpm[] = {
"14 16 17 1",
" 	c None",
".	c #000000",
"+	c #800000",
"@	c #008000",
"#	c #808000",
"$	c #000080",
"%	c #800080",
"&	c #008080",
"*	c #C0C0C0",
"=	c #808080",
"-	c #FF0000",
";	c #00FF00",
">	c #FFFF00",
",	c #0000FF",
"'	c #FF00FF",
")	c #00FFFF",
"!	c #FFFFFF",
"..........=   ",
".!!!!!!!!!==  ",
".!********=!= ",
".!***@@***=!!=",
".!**@;@.**....",
".!**@@@.****!.",
".!***..*****!.",
".!******,,,.! ",
".!*-****,),.! ",
".!*'-***,,$   ",
".!*''-**..  !.",
".!*'''-*   *!.",
".!*...     *!.",
".!**     ***!.",
".!!     !!!!!.",
"..      ......"};

wxBitmap CreateBitmapFromFile(const wxString &filename, int width, int height)
{
    wxFileName fn(filename);
    wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(fn.GetExt());

    wxBitmap bm(broken_xpm);
    if (ft) {
        wxString mt;
        if (ft->GetMimeType(&mt)) {
            int t = -1;

            if (mt == _T("image/png"))
                t = wxBITMAP_TYPE_PNG;
            if (mt == _T("image/bmp"))
                t = wxBITMAP_TYPE_BMP;
            if (mt == _T("image/gif"))
                t = wxBITMAP_TYPE_GIF;
            if (mt == _T("image/x-bitmap"))
                t = wxBITMAP_TYPE_XBM;
            if (mt == _T("image/jpeg"))
                t = wxBITMAP_TYPE_JPEG;
            if (mt == _T("image/tiff"))
                t = wxBITMAP_TYPE_TIF;
            if (mt == _T("image/pcx"))
                t = wxBITMAP_TYPE_PCX;
            if (mt == _T("image/pnm"))
                t = wxBITMAP_TYPE_PNM;

            if (t != -1) {
                wxBitmap rbm;
                wxFileSystem fs;
                wxFSFile *f = fs.OpenFile(::wxGetApp().GetResourcePrefix() + filename);
                if (f) {
                    wxInputStream *s = f->GetStream();
                    if (s && s->IsOk()) {
                        wxImage wi(*s, t);
                        rbm = wxBitmap(wi);
                    }
                    delete f;
                }
                if (rbm.Ok())
                    bm = rbm;
            }
        }
        delete ft;
    }
    if (width == -1)
        width = bm.GetWidth();
    if (height == -1)
        height = bm.GetHeight();
    if ((width != bm.GetWidth()) || (height != bm.GetHeight()))
        bm = wxBitmap(bm.ConvertToImage().Scale(width, height));
    return bm;
}

wxIcon CreateIconFromFile(const wxString &filename, int width, int height)
{
    wxIcon icon = wxNullIcon;
    wxBitmap bm = CreateBitmapFromFile(filename, width, height);
    if (bm.Ok())
      icon.CopyFromBitmap(bm);
    return icon;
}

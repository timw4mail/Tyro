/**
 * Common header for widget classes
 */

#ifndef WX_COMMON_H
#define	WX_COMMON_H

#include "common.h"

// Disable annoying warning
#ifndef __WXMAC__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
#endif

#ifdef WX_PRECOMP
	#include "wx_pch.h"
#endif
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

// Common helpers/functionality
#include <wx/debug.h>
#include <wx/utils.h>
#include <wx/version.h>
#include <wx/cmdline.h>
#include <wx/config.h>
#include <wx/font.h>
#include <wx/stdpaths.h>
#include <wx/platinfo.h>
#include <wx/vidmode.h>
#include <wx/filename.h>
#include <wx/artprov.h>

#ifndef __WXMAC__
#pragma clang diagnostic pop
#endif

// Define a check for font-control bug 
// in wxWidgets < 3.0.3 and OS X >= 10.10
static inline bool HAS_FONT_BUG()
{
#ifndef __WXMAC__
	return false;
#endif
		
#if	wxCHECK_VERSION(3,0,3)
	return false;
#endif
	
	wxPlatformInfo info;
	return (info.GetOSMajorVersion() == 10 && info.GetOSMinorVersion() > 9);
}

// Tyro-specific variables
#include "definitions.h"

#endif	/* WX_COMMON_H */


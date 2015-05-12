/**
 * Common header for widget classes
 */

#ifndef WX_COMMON_H
#define	WX_COMMON_H

#include "common.h"

#ifdef WX_PRECOMP
	#include "wx_pch.h"
#endif
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

// Common helpers/functionality
#include <wx/debug.h>
#include <wx/cmdline.h>
#include <wx/config.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/artprov.h>

// Tyro-specific variables
#include "definitions.h"

#endif	/* WX_COMMON_H */


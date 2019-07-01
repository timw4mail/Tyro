/**
 * Common header for widget classes
 */
#pragma once

// Import the basename c function
#include <libgen.h>

#include "common.h"

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
#include <wx/display.h>
#include <wx/font.h>
#include <wx/stdpaths.h>
#include <wx/platinfo.h>
#include <wx/filename.h>
#include <wx/artprov.h>

/**
 * Calculate original window size based on size of the current monitor
 */
wxSize static CalculateWindowSize()
{
    wxDisplay display;
    wxVideoMode mode = display.GetCurrentMode();

    wxLogDebug("Current display: %ix%i", mode.w, mode.h);

    auto width = (int)((float)mode.w * 0.9);
    auto height = (int)((float)mode.h * 0.9);

    wxSize base(width, height);

    return base;
}

wxString static BaseName(const wxString &path)
{
	auto fullPath = path.char_str();
	auto base = basename(fullPath);

	return (wxString) base;
}

// Tyro-specific variables
#include "definitions.h"

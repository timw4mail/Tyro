/**
 * Common header for widgets
 */
#ifndef TYRO_WIDGET_H
#define	TYRO_WIDGET_H

// Common wxWidgets stuff
#include "../wx_common.h"

// Base widgets
#include <wx/aboutdlg.h>
#include <wx/fdrepdlg.h>
#include <wx/aui/aui.h>
#include <wx/stc/stc.h>
#include <wx/treectrl.h>
#include <wx/dir.h>
#include <wx/dirctrl.h>
#include <wx/fontpicker.h>

#ifndef TRAVIS
#include <wx/preferences.h>
#endif

// Tyro includes
#include "TyroMenu.h"
#include "EditPane.h"
#include "TabContainer.h"
#include "MainFrame.h"
#ifndef TRAVIS
#include "PrefPane.h"
#endif
#include "FilePane.h"

#endif	/* TYRO_WIDGET_H */


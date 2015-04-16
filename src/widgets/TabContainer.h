/**
 * Wrapper around wxAuiNotebook
 */

#ifndef TABCONTAINER_H
#define	TABCONTAINER_H

#include "../wx_common.h"

#include <wx/aui/aui.h>
#include <wx/filename.h>
#include "EditPane.h"


static long tab_style = wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS 
	| wxAUI_NB_WINDOWLIST_BUTTON | wxAUI_NB_CLOSE_ON_ALL_TABS
	| wxAUI_NB_MIDDLE_CLICK_CLOSE | wxAUI_NB_TOP;

class TabContainer: public wxAuiNotebook
{
public:
	TabContainer(
		wxWindow *parent,
		wxWindowID id=wxID_ANY,
		const wxPoint &pos=wxDefaultPosition,
		const wxSize &size=wxDefaultSize,
		long style=tab_style
	);
	~TabContainer();
	void AddTab();
	void AddTab(wxString filePath);
	EditPane *GetCurrentEditor();
private:
};

#endif	/* TABCONTAINER_H */


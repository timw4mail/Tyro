/**
 * Wrapper around wxAuiNotebook
 */
#pragma once

#include "src/widgets/TyroMenu.h"
#include "src/widgets/EditorPane.h"

static long tab_style = wxBORDER_NONE | wxAUI_NB_TAB_SPLIT |wxAUI_NB_TAB_MOVE
	| wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_WINDOWLIST_BUTTON
	| wxAUI_NB_CLOSE_ON_ACTIVE_TAB | wxAUI_NB_MIDDLE_CLICK_CLOSE | wxAUI_NB_TOP;

class TabContainer: public wxAuiNotebook
{
public:
	explicit TabContainer(
		wxWindow *parent,
		wxWindowID id=wxID_ANY,
		const wxPoint &pos=wxDefaultPosition,
		const wxSize &size=wxDefaultSize,
		long style=tab_style
	);
	~TabContainer() override;
	void AddTab();
	void AddTab(const wxString &filePath);
	EditorPane *GetCurrentEditor();
	EditorPane *GetEditor(size_t page_idx);
	void OnCloseAll(wxCommandEvent &event);
	void OnCloseAllButThis(wxCommandEvent &event);
private:
	unsigned long untitled_document_count = 0;
	EditorPane *NewEditor();
	void OnTabSwitch(wxAuiNotebookEvent &event);
	void OnClose(wxAuiNotebookEvent &event);
	void OnClosed(wxAuiNotebookEvent &event);
	void OnTabContextMenu(wxAuiNotebookEvent &event);
};


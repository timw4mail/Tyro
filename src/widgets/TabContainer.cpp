/**
 * Wrapper around wxAuiNotebook
 */

#include "src/widgets/TabContainer.h"
#include "src/widgets/MainFrame.h"

extern TyroMenu *Glob_menu_bar;
extern wxStatusBar *Glob_status_bar;

/**
 * Constructor
 * 
 * @param wxWindow* parent
 * @param wxWindowID id
 * @param const wxPoint& pos
 * @param const wxSize& size
 * @param long style
 */
TabContainer::TabContainer(
		wxWindow* parent,
		wxWindowID id,
		const wxPoint& pos,
		const wxSize& size,
		long style
) : wxAuiNotebook(parent, id, pos, size, style)
{
	this->Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, &TabContainer::OnClose, this, wxID_ANY);
	this->Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSED, &TabContainer::OnClosed, this, wxID_ANY);
	this->Bind(wxEVT_AUINOTEBOOK_TAB_RIGHT_DOWN, &TabContainer::OnTabContextMenu, this, wxID_ANY);
	this->Bind(wxEVT_AUINOTEBOOK_PAGE_CHANGED, &TabContainer::OnTabSwitch, this, wxID_ANY);
}

/**
 * Destructor
 */
TabContainer::~TabContainer() 
{
	wxLogDebug("TabContainer Destructor Called.");
}

/**
 * Create a new editor instance for a new tab
 * 
 * @return EditPane*
 */
EditorPane* TabContainer::NewEditor()
{
	return new EditorPane(this);
}

/**
 * Add a new blank document
 * 
 * @return void
 */
void TabContainer::AddTab()
{
	this->untitled_document_count++;
	
	wxString caption;

	caption.Printf("Untitled %lu", this->untitled_document_count);

	this->AddPage(this->NewEditor(), caption, true);
}

/**
 * Open an existing document
 * 
 * @param wxString filePath
 * @return void
 */
void TabContainer::AddTab(const wxString &filePath)
{	
	wxFileName fileName(filePath);
	
	if ( ! (fileName.IsOk() && fileName.FileExists()))
	{
		wxLogDebug("Invalid file name.");
		return;
	}
	
	wxString caption = fileName.GetFullName();
	EditorPane *editor = this->NewEditor();
	
	if (editor->Load(filePath))
	{
		this->AddPage(editor, caption, true);
		
		this->SetPageToolTip(this->GetPageIndex(this->GetCurrentPage()), fileName.GetFullPath());

		// Select the appropriate language in the language menu
		Glob_menu_bar->SetCurrentLanguage(editor->GetCurrentLang());
		
		return;
	}
	
	wxLogDebug("Failed to load file!?");
}

/**
 * Get the EditPane control in the current tab
 * 
 * @return *EditPane 
 */
EditorPane* TabContainer::GetCurrentEditor()
{
	return (EditorPane *) this->GetCurrentPage();
}

/**
 * Get the EditPane control in the specified tab
 * 
 * @param size_t page_idx
 * @return *EditPane
 */
EditorPane* TabContainer::GetEditor(size_t page_idx)
{
	return (EditorPane *) this->GetPage(page_idx);
}

/**
 * Event handler for file saving checks on tab close
 *
 * @param wxAuiNotebookEvent& event
 * @return void
 */ 
void TabContainer::OnClose(wxAuiNotebookEvent &event)
{
	int current_tab = this->GetSelection();
	EditorPane *editor = this->GetCurrentEditor();
	
	// Sanity check
	if (current_tab == -1) return;
	
	if (editor->IsModified())
	{
		int Msgbox_Choice = wxMessageBox(
			"File has not been saved, save file before closing?", 
			"Modified File",
			wxYES_NO | wxCANCEL | wxICON_QUESTION,
			this
		);
		
		if (Msgbox_Choice == wxCANCEL)
		{
			return event.Veto();
		}
		else if (Msgbox_Choice == wxYES)
		{
			editor->SaveFile();
			if (editor->IsModified())
			{
				wxMessageBox(TYRO_SAVE_ERROR, TYRO_SAVE_ERROR_CAPTION, wxOK | wxICON_EXCLAMATION);
				return event.Veto();
			}
		}
	}
}

/**
 * Event handler triggered after a tab is closed
 * 
 * @param WXUNUSED
 * @return void
 */
void TabContainer::OnClosed(wxAuiNotebookEvent &WXUNUSED(event))
{
	if (this->GetPageCount() == 0)
	{
		auto parentFrame = (MainFrame *)this->GetParent();
		parentFrame->EnableEditControls(false);
		Glob_status_bar->SetStatusText("", STATUS_CURSOR_LOCATION);
		Glob_status_bar->SetStatusText("", STATUS_CURRENT_LANGUAGE);
	}
}

/**
 * Displays a context menu on the current tab
 * 
 * @return void
 */ 
void TabContainer::OnTabContextMenu(wxAuiNotebookEvent &WXUNUSED(event))
{
	// Create Menu
	auto *contextMenu = new wxMenu();
	contextMenu->Append(wxID_CLOSE, "&Close\tCtrl+W", "Close the current tab");
	contextMenu->Append(myID_CLOSE_ALL, "C&lose All\tShift+Ctrl+W", "Close all open documents.");
	contextMenu->Append(myID_CLOSE_ALL_BUT_THIS, "Close All but this\tCtrl+Shift+Alt+W", "Close all open documents, except the one selected");

	this->PopupMenu(contextMenu);
}

/**
 * Close all the open tabs
 *
 * @return void
 */
void TabContainer::OnCloseAll(wxCommandEvent &WXUNUSED(event))
{
	this->DeleteAllPages();
	auto parentFrame = (MainFrame *)this->GetParent();
	parentFrame->EnableEditControls(false);
}

/**
 * Close all tabs but the one selected
 * 
 * @param wxCommandEvent& event
 * @return void
 */
void TabContainer::OnCloseAllButThis(wxCommandEvent &WXUNUSED(event))
{
	auto ind = this->GetSelection();
	wxString curr_tooltip = this->GetPageToolTip(ind);
	wxString curr_caption = this->GetPageText(ind);
	size_t page_count = this->GetPageCount();
	
	this->Freeze();
	
	// Do the loop in this order, as the indices change
	// when tabs are removed
	size_t i = page_count;
	
	while(i > 0)
	{	
		i--;
		// Identify the current tab by the caption and tooltip
		// If they don't match, remove the tab
		if (
			curr_tooltip == this->GetPageToolTip(i) &&
			curr_caption == this->GetPageText(i)
		) continue;
		
		this->RemovePage(i);
	}
	this->Thaw();
}

/**
 * Update menu state when switching tabs
 * 
 * @param wxAuiNotebookEvent& event
 * @return void
 */
void TabContainer::OnTabSwitch(wxAuiNotebookEvent &event)
{
	EditorPane *editor = this->GetEditor(event.GetSelection());
	
	// Update view menu options
	Glob_menu_bar->SetIdChecked(myID_VIEW_WHITESPACE, (editor->GetViewWhiteSpace() == wxSTC_WS_VISIBLEALWAYS));
	Glob_menu_bar->SetIdChecked(myID_VIEW_LINE_ENDINGS, editor->GetViewEOL());
	Glob_menu_bar->SetIdChecked(myID_LINE_WRAP, (editor->GetWrapMode() == wxSTC_WRAP_WORD));
	
	// Update language menu selection
	Glob_menu_bar->SetCurrentLanguage(editor->GetCurrentLang());
	
	// Update status bar 
	Glob_status_bar->SetStatusText(editor->GetCurrentLang(), STATUS_CURRENT_LANGUAGE);
}

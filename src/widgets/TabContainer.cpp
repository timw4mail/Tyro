/**
 * Wrapper around wxAuiNotebook
 */

#include "widget.h"

extern MainFrame *main_frame;
static unsigned long untitled_document_count = 0;

TabContainer::TabContainer(
		wxWindow* parent,
		wxWindowID id,
		const wxPoint& pos,
		const wxSize& size,
		long style
) : wxAuiNotebook(parent, id, pos, size, style)
{
	Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, &TabContainer::OnClose, this, wxID_ANY);
	Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSED, &TabContainer::OnClosed, this, wxID_ANY);
	Bind(wxEVT_AUINOTEBOOK_TAB_RIGHT_DOWN, &TabContainer::OnTabContextMenu, this, wxID_ANY);
	Bind(wxEVT_AUINOTEBOOK_PAGE_CHANGED, &TabContainer::OnTabSwitch, this, wxID_ANY);
}

TabContainer::~TabContainer() 
{
	wxLogDebug("TabContainer destructor called");
}

/**
 * Add a new blank document
 * 
 * @return void
 */
void TabContainer::AddTab()
{
	untitled_document_count++;
	
	wxString caption;

	caption.Printf("Untitled %lu", untitled_document_count);

	EditPane *editor = new EditPane(this, wxID_ANY);

	this->AddPage(editor, caption, true);
}

/**
 * Open an existing document
 * 
 * @param wxString filePath
 * @return void
 */
void TabContainer::AddTab(wxString filePath)
{
	wxFileName fileName(filePath);
	
	wxString caption= fileName.GetFullName();
	EditPane *editor = new EditPane(this, wxID_ANY);
	
	if (editor->Load(filePath))
	{
		this->AddPage(editor, caption, true);
		
		this->SetPageToolTip(this->GetPageIndex(this->GetCurrentPage()), fileName.GetFullPath());

		return;
	}
	
	wxLogDebug("Failed to load file!?");
}

/**
 * Get the EditPane control in the current tab
 * 
 * @return *EditPane 
 */
EditPane *TabContainer::GetCurrentEditor()
{
	return (EditPane *) this->GetCurrentPage();
}

/**
 * Get the EditPane control in the specified tab
 * 
 * @param size_t page_idx
 * @return *EditPane
 */
EditPane *TabContainer::GetEditor(size_t page_idx)
{
	return (EditPane *) this->GetPage(page_idx);
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
	EditPane *editor = this->GetCurrentEditor();
	
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
	};
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
		main_frame->EnableEditControls(false);
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
	wxMenu *contextMenu = new wxMenu();
	contextMenu->Append(wxID_CLOSE, "&Close\tCtrl+W", "Close the current tab");
	contextMenu->Append(myID_CLOSE_ALL, "C&lose All\tShift+Ctrl+W", "Close all open documents.");

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
	main_frame->EnableEditControls(false);
}

/**
 * Update menu state when switching tabs
 * 
 * @param wxAuiNotebookEvent& event
 * @return void
 */
void TabContainer::OnTabSwitch(wxAuiNotebookEvent &WXUNUSED(event))
{
	//EditPane *editor = this->GetEditor(event.GetSelection());
	
	// @TODO: Update menu item checkboxes based on the state of the current editor
}
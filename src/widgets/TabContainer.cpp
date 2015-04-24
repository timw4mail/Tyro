/**
 * Wrapper around wxAuiNotebook
 */

#include "TabContainer.h"

static unsigned long untitled_document_count = 0;

TabContainer::TabContainer(
		wxWindow* parent,
		wxWindowID id,
		const wxPoint& pos,
		const wxSize& size,
		long style
) : wxAuiNotebook(parent, id, pos, size, style)
{
}

TabContainer::~TabContainer() {}

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
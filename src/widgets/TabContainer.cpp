/**
 * Wrapper around wxAuiNotebook
 */

#include "TabContainer.h"

BEGIN_EVENT_TABLE(TabContainer, wxAuiNotebook)
END_EVENT_TABLE()

static unsigned long untitled_document_count = 0;

TabContainer::TabContainer(
		wxWindow* parent,
		wxWindowID id,
		const wxPoint& pos,
		const wxSize& size,
		long style
) : wxAuiNotebook(parent, id, pos, size, style)
{	
	this->AddTab();
}

TabContainer::~TabContainer() {}

EditPane *TabContainer::CreateEditor()
{
	return new EditPane(this, wxID_ANY);
}

void TabContainer::AddTab()
{
	untitled_document_count++;
	wxString caption;
	
	caption.Printf("Untitled %lu", untitled_document_count);
	
	this->AddPage(CreateEditor(), caption);
}

void TabContainer::AddTab(wxString filePath)
{
	
}
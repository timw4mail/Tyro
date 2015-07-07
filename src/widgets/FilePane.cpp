#include "src/widgets/FilePane.h"

FilePane::FilePane(
	wxWindow* parent, 
	wxWindowID id, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString &name
) : wxTreeListCtrl(parent, id, pos, size, style, name)
{
	this->dir = new wxDir();
	this->InitImageList();
	this->SetImageList(this->img_list);
	
	this->dir->Open(".");
	
	this->AppendColumn("", 
		wxCOL_WIDTH_AUTOSIZE, 
		wxALIGN_LEFT, 
		wxCOL_RESIZABLE | wxCOL_SORTABLE);
	
	wxTreeListItem root = this->GetRootItem();
}

FilePane::~FilePane()
{
	
}

void FilePane::InitImageList()
{
	wxSize iconSize = wxArtProvider::GetSizeHint(wxART_LIST);
	
	if (iconSize == wxDefaultSize)
	{
		iconSize = wxSize(16, 16);
	}
	
	this->img_list = new wxImageList(iconSize.x, iconSize.y);
	
	static const char* icons[] = 
	{
		wxART_NORMAL_FILE,
		wxART_FOLDER,
		wxART_FOLDER_OPEN
	};
	
	for (unsigned n = 0; n < WXSIZEOF(icons); n++)
	{
		this->img_list->Add(
			wxArtProvider::GetIcon(icons[n], wxART_LIST, iconSize)
		);
	}
}
#include "src/widgets/FilePane.h"

enum
{
	Icon_File,
	Icon_FolderClosed,
	Icon_FolderOpened
};

FilePane::FilePane(
	wxWindow* parent, 
	wxWindowID id, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString &name
) : wxTreeListCtrl(parent, id, pos, size, style, name)
{
	this->InitImageList();
	this->SetImageList(this->img_list);
	
	wxString defaultPath(".");
	wxFileName fname(defaultPath);
	fname.MakeAbsolute(defaultPath);
	wxTreeListItem root = this->GetRootItem();
	this->CreateTree(defaultPath, root);
	
	this->AppendColumn("", 
		wxCOL_WIDTH_AUTOSIZE, 
		wxALIGN_LEFT, 
		wxCOL_RESIZABLE | wxCOL_SORTABLE);
	
	
}

FilePane::~FilePane()
{
	delete this->img_list;
}

void FilePane::BindEvents()
{
	this->Bind(wxEVT_TREELIST_ITEM_EXPANDING, &FilePane::OpenFolder, this, wxID_ANY);
}

void FilePane::OpenFolder(wxTreeListEvent& event)
{
	
	wxTreeListItem item = event.GetItem();
	const wxString path = this->GetItemText(item, 0);
	
	wxLogDebug(path);
}

/**
 * Iterates through the specified folder and creates the tree view
 * 
 * @access private
 * @param wxString &path
 * @param wxTreeListItem &root
 */
void FilePane::CreateTree(const wxString &path, wxTreeListItem &root)
{
	wxArrayString *files = new wxArrayString();
	wxDir::GetAllFiles(path, files);
	
	vector<wxString> examined;
	vector<wxString>::iterator it;
	
	for (wxString filename : *files)
	{
		wxFileName fname(filename);
		
		/*if (fname.IsRelative())
		{
			fname.MakeAbsolute();
		}*/

		// For loose files, just add directly to the tree
		if (fname.GetDirCount() == 1)
		{
			this->AppendItem(root, fname.GetFullName(), Icon_File, Icon_File);
		}
		else
		{
			fname.RemoveDir(0);
			wxArrayString folders = fname.GetDirs();
			wxString curr_folder = folders[0];
			
			it = find(examined.begin(), examined.end(), curr_folder);
			
			// If the directory already exists, continue;
			if (it != examined.end()) continue;
			
			wxTreeListItem current = this->AppendItem(root, curr_folder, Icon_FolderClosed, Icon_FolderOpened);
			examined.push_back(curr_folder);
			
			// Recurse to create sub dirs
			this->CreateTree(fname.GetPath(), current);
		}
	}
}

/*void FilePane::CreateFolderTree(StringVector dirList, wxTreeListItem &root)
{
	
}*/

/**
 * Create the image list object for the file pane widget
 */
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
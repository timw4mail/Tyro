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
	wxFileName filename(defaultPath);
	filename.MakeAbsolute(defaultPath);
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
	this->Bind(wxEVT_TREELIST_ITEM_ACTIVATED, &FilePane::OpenFileInEditor, this, wxID_ANY);
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
void FilePane::CreateTree(const wxString &path, wxTreeListItem &root, int level)
{
	// So yeah, this doesn't really work right.
	// It seems I need to create a tree from the list of file paths,
	// after which this should be much simpler.
	// @TODO Fix

	auto *files = new wxArrayString();
	wxDir::GetAllFiles(path, files);
	
	vector<wxString> examined;
	vector<wxString>::iterator it;
	
	for (const wxString &item : *files)
	{
		wxFileName filename(item);

		// For loose files, just add directly to the tree
		if (filename.GetDirCount() == 1)
		{
			auto fullFileName = filename.GetFullPath();

			auto fileData = new wxStringClientData();
			fileData->SetData(fullFileName);

			examined.push_back(fullFileName);

			this->AppendItem(root, filename.GetFullName(), Icon_File, Icon_File, fileData);
			continue;
		}

		// Remove the directory component closest to the root
		filename.RemoveDir(0);

		wxArrayString folders = filename.GetDirs();

		wxLogDebug(item);

		wxTreeListItem newRootNode = root;

		for (const wxString &curr_folder: folders)
		{
			// Check if directory has already been created
			it = find(examined.begin(), examined.end(), curr_folder);

			if (it != examined.end()) continue;

			// Create the directory node if it doesn't exist
			auto fileData = new wxStringClientData();
			fileData->SetData(curr_folder);

			wxTreeListItem current = this->AppendItem(
					newRootNode,
					curr_folder,
					Icon_FolderClosed,
					Icon_FolderOpened,
					fileData);
			examined.push_back(curr_folder);

			newRootNode = current;

			this->CreateTree(curr_folder, root);
		}
	}
}

/*void FilePane::CreateFolderTree(StringVector dirList, wxTreeListItem &root)
{
	
}*/

void FilePane::OpenFileInEditor(wxTreeListEvent& event)
{
	wxTreeListItem item = event.GetItem();
	auto path = this->GetItemText(item, 0);

	wxLogMessage(path);
}

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
	
	for (auto icon : icons)
	{
		this->img_list->Add(
			wxArtProvider::GetIcon(icon, wxART_LIST, iconSize)
		);
	}
}
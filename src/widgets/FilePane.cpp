#include <unordered_set>

#include "src/widgets/FilePane.h"
#include "src/widgets/MainFrame.h"

extern MainFrame *Glob_main_frame;

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
	this->BindEvents();
	this->InitImageList();
	this->SetImageList(this->img_list);
	
	wxString defaultPath(".");
	wxFileName filename(defaultPath);
	filename.MakeAbsolute(defaultPath);

	this->CreateTree(defaultPath);
	
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
	
	wxLogDebug("Opening sidebar dir: %s", path);
}

/**
 * Iterates through the specified folder and creates the tree view
 * 
 * @access private
 */
void FilePane::CreateTree(const wxString &path)
{
	wxTreeListItem root = this->GetRootItem();

	auto *files = new wxArrayString();
	wxFileName rootPath(path);
	rootPath.MakeAbsolute();

	this->base_path = rootPath.GetPath();

	wxLogDebug("Base Path for Sidebar: %s", this->base_path);

	wxDir::GetAllFiles(this->base_path, files);

	for (const wxString &file: *files)
	{
		wxFileName fileName(file);

		// Make the dir relative to the base path,
		// then only use the first dir segment
		fileName.MakeRelativeTo(this->base_path);
		auto dir = std::string(fileName.GetPath());

		if (dir.empty())
		{
			continue;
		}

		wxArrayString dirs = fileName.GetDirs();

		this->AddDirToTree(root, dirs[0], wxString(""));
	}

	delete files;

	// Add files that are in the root path
	this->AddDirFiles(root, this->base_path);
}

/**
 * Recursively create directory tree from list of files
 * 
 * @access private
 */
void FilePane::AddDirToTree(wxTreeListItem &root, const wxString &path, const wxString &parent)
{
	wxLogInfo("AddDirToTree path: %s, parent: %s", path, parent);
	auto fullPath = this->base_path;

	if ( ! parent.empty())
	{
		fullPath += "/";

		auto par = parent.Clone();
		par.Replace((wxString)this->base_path + "/", "");

		fullPath += par.ToStdString();
	}

	if ( ! fullPath.Contains(path))
	{
		fullPath += "/";
		fullPath += path;
	}

	wxString wFullPath(fullPath);

	// Stop early if folder exists
	auto it = this->dir_set.find(std::string(fullPath));
	if (it != this->dir_set.end())
	{
		wxLogInfo("Redundant call to AddDirToTree for: %s, %s", path, parent);
		return;
	}

	auto shortDir = BaseName(wFullPath);

	auto fileData = new wxStringClientData();
	fileData->SetData(fullPath);
	auto dir_node = this->AppendItem(root, shortDir, Icon_FolderClosed, Icon_FolderOpened, fileData);
	this->dir_set.insert(string(fullPath));

	// ------------------------------------------------------------------------------------
	// Find folder(s) to recurse
	// ------------------------------------------------------------------------------------

	auto *files = new wxArrayString();
	wxDir::GetAllFiles(fullPath, files);

	wxFileName currentPath((wxString)fullPath);
	currentPath.MakeAbsolute();
	currentPath.MakeRelativeTo(this->base_path);

	for (const wxString &file: *files)
	{
		auto parentDir = currentPath.GetPath();
		wxLogInfo("- Parent dir: %s, full dir: %s", parentDir, fullPath);

		wxFileName fileName(file);

		// Make the dir relative to the search path,
		// then only use the first dir segment
		fileName.MakeRelativeTo(fullPath);
		auto dir = std::string(fileName.GetPath());

		if (dir.empty())
		{
			continue;
		}

		auto newParent = parentDir;
		if (parentDir.empty())
		{
			newParent = BaseName(fullPath);
		}

		if ( ! newParent.Contains(BaseName(fullPath)))
		{
			newParent += "/" + BaseName(fullPath);
		}

		wxArrayString dirs = fileName.GetDirs();

		wxLogInfo("-- Recursing to deeper folder: %s(%s), parent: %s(%s)", dirs[0], dir, newParent, parentDir);

		this->AddDirToTree(dir_node, dirs[0], newParent);
	}

	delete files;

	// Add the files, if they exist
	// Defer until after recursion so that files follow folders
	this->AddDirFiles(dir_node, fullPath);
}

void FilePane::AddDirFiles(wxTreeListItem &root, const wxString &path)
{
	wxLogInfo("Adding files for dir: %s", path);

	auto *files = new wxArrayString();
	wxDir::GetAllFiles(path, files, wxEmptyString, wxDIR_FILES);

	wxFileName rootPath(path);
	rootPath.MakeAbsolute();

	for (const wxString &item: *files)
	{
		wxFileName fileName(item);
		fileName.MakeAbsolute();

		auto it = this->file_set.find(std::string(fileName.GetFullPath()));
		if (it != this->file_set.end())
		{
			continue;
		}

		auto fileData = new wxStringClientData();
		fileData->SetData(fileName.GetFullPath());

		auto fileLabel = BaseName(fileName.GetFullName());

		this->AppendItem(root, fileLabel, Icon_File, Icon_File, fileData);
		this->file_set.insert(std::string(fileName.GetFullPath()));
	}

	delete files;
}

/**
 * Open a file you double-click on the file list
 */
void FilePane::OpenFileInEditor(wxTreeListEvent& event)
{
	wxTreeListItem item = event.GetItem();
	auto data = (wxStringClientData*)this->GetItemData(item);
	const wxString& path = data->GetData();

	wxLogDebug("Opening file from sidebar: %s", path);

	wxString path_arr [1] = { path };
	auto files = wxArrayString(1, *path_arr);
	Glob_main_frame->OpenFiles(files);
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

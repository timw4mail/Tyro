#include <libgen.h>
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
	
	wxLogDebug("Opening sidebar dir: %s", path);
}

/**
 * Iterates through the specified folder and creates the tree view
 * 
 * @access private
 */
void FilePane::CreateTree(const wxString &path, wxTreeListItem &root)
{
	auto *files = new wxArrayString();
	wxFileName rootPath(path);
	rootPath.MakeAbsolute();

	this->base_path = rootPath.GetPath();

	wxLogDebug("Base Path for Sidebar: %s", this->base_path);

	wxDir::GetAllFiles(this->base_path, files);

	for (const wxString &file: *files)
	{
		wxFileName fileName(file);

		// If the file is at the root, add it to the tree
		if (fileName.GetDirCount() == rootPath.GetDirCount())
		{
			this->AddDirFiles(path, root);
		}

		auto dir = std::string(fileName.GetPath());

		if (dir.empty())
		{
			continue;
		}

		// Append the folder to the tree
		wxString wdir = wxString(dir);
		// wxLogDebug("Creating Dir Tree: %s", wdir);
		this->AddDirToTree(this->BaseName(wdir), root, wxString(""));
	}
}

/**
 * Recursively create directory tree from list of files
 * 
 * @access private
 */
void FilePane::AddDirToTree(const wxString &path, wxTreeListItem &root, const wxString &parent)
{
	auto pathBase = this->BaseName(path);
	auto fullPath = this->base_path;

	if ( ! parent.empty())
	{
		fullPath += "/";

		wxString par = parent.Clone();
		par.Replace((wxString)this->base_path, "");

		fullPath += par.ToStdString();
	}

	if ( ! fullPath.Contains(pathBase))
	{
		fullPath += "/";
		fullPath += pathBase;
	}

	wxFileName parentDir(fullPath);
	parentDir.MakeAbsolute();

	wxLogDebug("Rendering Dir Tree for %s, full path: %s", path, fullPath);

	auto *files = new wxArrayString();
	wxDir::GetAllFiles(fullPath, files);

	for (const wxString &item: *files)
	{
		wxFileName fileName(item);

		auto dir = std::string(fileName.GetPath());
		wxString wdir(dir);
		wxFileName dirName(dir);

		if ( ! (wdir.Contains(pathBase) || dirName.GetDirCount() != (parentDir.GetDirCount() + 1)))
		{
			continue;
		}

		// Stop early if folder exists
		auto it = this->dir_set.find(std::string(dir));
		if (it != this->dir_set.end())
		{
			continue;
		}

		auto fileData = new wxStringClientData();
		fileData->SetData(dir);

		auto dir_label = this->BaseName(dir);
		auto dir_node = this->AppendItem(root, dir_label, Icon_FolderClosed, Icon_FolderOpened, fileData);

		this->dir_set.insert(std::string(dir));

		wxLogDebug("Recursing for dir %s, from parent %s", dir_label, fullPath);

		this->AddDirFiles(fullPath, dir_node);
		this->AddDirToTree(dir_label, dir_node, fullPath);
	}
}

void FilePane::AddDirFiles(const wxString &path, wxTreeListItem &root)
{
	auto *files = new wxArrayString();
	wxDir::GetAllFiles(path, files);

	wxFileName rootPath(path);
	rootPath.MakeAbsolute();

	for (const wxString &item: *files)
	{
		wxFileName fileName(item);
		fileName.MakeAbsolute();

		// If the file is in another folder, don't add it here!
		if (fileName.GetDirCount() != rootPath.GetDirCount())
		{
			continue;
		}

		auto it = this->file_set.find(std::string(fileName.GetFullPath()));
		if (it != this->file_set.end())
		{
			continue;
		}

		auto fileData = new wxStringClientData();
		fileData->SetData(fileName.GetFullPath());

		auto fileLabel = this->BaseName(fileName.GetFullName());

		this->AppendItem(root, fileLabel, Icon_File, Icon_File, fileData);
		this->file_set.insert(std::string(fileName.GetFullPath()));
	}
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

wxString FilePane::BaseName(wxString path)
{
	auto fullPath = path.char_str();
	auto base = basename(fullPath);

	return (wxString) base;
}

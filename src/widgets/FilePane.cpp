#include "widget.h"

extern MainFrame *Glob_main_frame;

enum CustomFilePaneEventIds {
	myFP_ID_OPEN = (wxID_HIGHEST * 2),
	myFP_ID_RENAME,
	myFP_ID_DELETE
};

/**
 * Constructor
 * @param parent
 * @param id
 * @param dir
 * @param pos
 * @param size
 * @param style
 * @param filter
 */
FilePane::FilePane(
	wxWindow* parent, 
	const wxWindowID id, 
	const wxString &dir,
	const wxPoint &pos,
	const wxSize &size,
	long style,
	const wxString &filter
) : wxGenericDirCtrl(parent, id, dir, pos, size, style, filter)
{
	this->CreateContextMenu();
	this->BindEvents();
	
	this->SetDefaultPath(wxStandardPaths::Get().GetDocumentsDir());
	this->SetPath(this->GetDefaultPath());
}

/**
 * Destructor
 */
FilePane::~FilePane()
{
	delete this->contextMenu;
}

/**
 * Bind event handlers
 * 
 * @return void
 */
void FilePane::BindEvents()
{
	// Open files on double-click or enter
	this->Bind(wxEVT_DIRCTRL_FILEACTIVATED, [=](wxTreeEvent&) {
		wxLogDebug("File activated event fired");
		this->OpenSelectedFiles();
	}, wxID_ANY);
	
	// Context Menu
	this->Bind(wxEVT_TREE_ITEM_RIGHT_CLICK, [=](wxTreeEvent&) {
		this->PopupMenu(this->contextMenu);
	}, wxID_ANY);
	
	// Open file(s)
	this->Bind(wxEVT_MENU, [=](wxCommandEvent&) {
		this->OpenSelectedFiles();
	}, myFP_ID_OPEN);
}

/**
 * Create the filePane Context Menu
 * 
 * @return void
 */
void FilePane::CreateContextMenu()
{
	this->contextMenu = new wxMenu();
	this->contextMenu->Append(myFP_ID_OPEN, "&Open	Ctrl+Shift+O", "Open the selected file(s)");
	//this->contextMenu->Append(myFP_ID_RENAME, "&Rename Ctrl+Shift+R", "Rename the selected file");
	//this->contextMenu->Append(myFP_ID_DELETE, "Delete", "Delete the selected file");
}

/**
 * Open the files that are currently selected in the file pane in the current
 * editor window
 * 
 * @return void
 */
void FilePane::OpenSelectedFiles()
{
	wxArrayString paths;
	this->GetPaths(paths);
	Glob_main_frame->OpenFiles(paths);
}
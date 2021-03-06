#pragma once

#include <unordered_set>
#include "src/widgets/widget.h"

class FileTreePane : public wxTreeListCtrl {
public:
	explicit FileTreePane(
		wxWindow *parent, 
		wxWindowID id=wxID_ANY, 
		const wxPoint &pos=wxDefaultPosition, 
		const wxSize &size=wxDefaultSize, 
		long style=wxTL_DEFAULT_STYLE, 
		const wxString &name=wxTreeListCtrlNameStr
	);
	~FileTreePane() override;
	void CreateTree(const wxString &path);
private:
	wxString base_path = "";
	wxImageList *img_list = nullptr;
	unordered_set<string> file_set;
	unordered_set<string> dir_set;
	void BindEvents();
	void OpenFolder(wxTreeListEvent& event);
	void OpenFileInEditor(wxTreeListEvent& event);
	void InitImageList();
	void AddDirToTree(wxTreeListItem &root, const wxString &path, const wxString &parent, bool recurse = false);
	void AddDirFiles(wxTreeListItem &root, const wxString &path, wxArrayString *files);
};


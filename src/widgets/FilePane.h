#pragma once

#include <unordered_set>
#include "src/widgets/widget.h"

class FilePane : public wxTreeListCtrl {
public:
	explicit FilePane(
		wxWindow *parent, 
		wxWindowID id=wxID_ANY, 
		const wxPoint &pos=wxDefaultPosition, 
		const wxSize &size=wxDefaultSize, 
		long style=wxTL_DEFAULT_STYLE, 
		const wxString &name=wxTreeListCtrlNameStr
	);
	~FilePane() override;
	void CreateTree(const wxString &path);
private:
	wxString base_path = "";
	wxImageList *img_list = nullptr;
	unordered_set<std::string> file_set;
	unordered_set<std::string> dir_set;
	void BindEvents();
	void OpenFolder(wxTreeListEvent& event);
	void OpenFileInEditor(wxTreeListEvent& event);
	void InitImageList();
	void AddDirToTree(wxTreeListItem &root, const wxString &path, const wxString &parent, bool recurse = false);
	void AddDirFiles(wxTreeListItem &root, const wxString &path);
};


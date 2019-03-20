#pragma once

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
private:
	wxString curr_path = "";
	wxImageList *img_list = nullptr;
	void BindEvents();
	void OpenFolder(wxTreeListEvent& event);
	void OpenFileInEditor(wxTreeListEvent& event);
	void InitImageList();
	void CreateTree(const wxString &path, wxTreeListItem &root, int level = 0);
};


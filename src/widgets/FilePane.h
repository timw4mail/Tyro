#ifndef TYRO_FILE_PANE_H
#define	TYRO_FILE_PANE_H

#include "src/widgets/widget.h"

class FilePane : public wxTreeListCtrl {
public:
	FilePane(
		wxWindow *parent, 
		wxWindowID id=wxID_ANY, 
		const wxPoint &pos=wxDefaultPosition, 
		const wxSize &size=wxDefaultSize, 
		long style=wxTL_DEFAULT_STYLE, 
		const wxString &name=wxTreeListCtrlNameStr
	);
	~FilePane();
private:
	wxString curr_path = "";
	wxImageList *img_list = nullptr;
	void BindEvents();
	void OpenFolder(wxTreeListEvent& event);
	void InitImageList();
	void CreateTree(const wxString &path, wxTreeListItem &root);
};

#endif	/* TYRO_FILE_PANE_H */


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
	wxImageList *img_list = nullptr;
	wxDir *dir = nullptr;
	void InitImageList();
};

#endif	/* TYRO_FILE_PANE_H */


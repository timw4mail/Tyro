#ifndef TYRO_FILE_PANE_H
#define	TYRO_FILE_PANE_H

class FilePane : public wxGenericDirCtrl {
public:
	FilePane(
		wxWindow *parent,
		const wxWindowID id = wxID_ANY,
		const wxString &dir = wxDirDialogDefaultFolderStr,
		const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,
		long style = wxDIRCTRL_3D_INTERNAL | wxDIRCTRL_EDIT_LABELS |
			wxDIRCTRL_SHOW_FILTERS | wxDIRCTRL_MULTIPLE,
		const wxString &filter = wxEmptyString
	);
	~FilePane();
private:
	wxMenu *contextMenu = nullptr;
	void BindEvents();
	void CreateContextMenu();
	void OpenSelectedFiles();
};

#endif	/* TYRO_FILE_PANE_H */


#ifndef TYROEDIT_PANE_H
#define TYROEDIT_PANE_H

#include "../wx_common.h"
#include "../settings/Config.h"

#include <wx/stc/stc.h>

class EditPane: public wxStyledTextCtrl
{
public:
	EditPane(
		wxWindow *parent, 
		wxWindowID id = wxID_ANY,
		const wxPoint &post = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,
		long style =
#ifndef __WXMAC__
		wxSUNKEN_BORDER |
#endif
		wxVSCROLL
	);
	~EditPane();
	wxFileName fileName;
	string GetLangByFile();
	bool Load(wxString filePath);
	void Highlight(wxString filePath);
	bool SaveFile();
	bool SaveFile(const wxString &filename);
	bool IsModified();
private:
	StringConstMap lexerMap;
	StringConstMap::iterator lexerMapIt;
	TyroConfig *config;
	enum
	{
		MARGIN_FOLD,
		MARGIN_LINE_NUMBERS
	};
	bool FileReadable();
	bool FileWritable();
	void BindEvents();
	void OnMarginClick(wxStyledTextEvent &event);
};

#endif // TYRODOC_FRAME_H

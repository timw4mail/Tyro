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
	wxString fileName;
	bool LoadAndHighlight(wxString filePath);
private:
	TyroConfig *config;
	enum
	{
		MARGIN_LINE_NUMBERS,
		MARGIN_FOLD
	};
};

#endif // TYRODOC_FRAME_H

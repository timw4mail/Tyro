#ifndef TYROEDIT_PANE_H
#define TYROEDIT_PANE_H

#include "common.h"
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
private:
	DECLARE_EVENT_TABLE()
};

#endif // TYRODOC_FRAME_H

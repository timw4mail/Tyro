#ifndef TYRODOC_FRAME_H
#define TYRODOC_FRAME_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/stc/stc.h>

class DocFrame: public wxWindow
{
public:
	DocFrame(wxWindow *parent, wxWindowID id);
private:
	wxStyledTextCtrl *editor;
	DECLARE_EVENT_TABLE()
};

#endif // TYRODOC_FRAME_H

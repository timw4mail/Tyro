#ifndef TYRODOC_FRAME_H
#define TYRODOC_FRAME_H

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/stc/stc.h>

class DocFrame: public wxFrame
{
public:
private:
	void OnFileChanged(wxStyledTextEvent &event);
	DECLARE_EVENT_TABLE()
};

#endif // TYRODOC_FRAM_H
#include "DocFrame.h"

BEGIN_EVENT_TABLE(DocFrame, wxWindow)
END_EVENT_TABLE()

DocFrame::DocFrame(wxWindow *parent, wxWindowID id)
{
	editor = new wxStyledTextCtrl(parent);
}

DocFrame::~DocFrame()
{
}

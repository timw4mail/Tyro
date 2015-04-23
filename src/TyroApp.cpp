/**
 * Main application file
 */

#include "wx_common.h"

#include <wx/app.h>
#include <wx/debug.h>

class TyroApp : public wxApp
{
	friend class MainFrame;
public:
	virtual bool OnInit();
	virtual int OnExit();
private:
};

//**************************************************************

#include "widgets/MainFrame.h"

IMPLEMENT_APP(TyroApp);

/**
 * Start the event loop and create the main window
 * 
 * @return bool 
 */
bool TyroApp::OnInit()
{
	this->SetAppName(APP_NAME);
	this->SetVendorName(APP_VENDOR);
	
	MainFrame* frame = new MainFrame(0L, APP_NAME);

	SetTopWindow(frame);
	
	frame->Layout();
	frame->CenterOnScreen();
	frame->Show(true);

	return true;
}

/**
 * Exit handler
 * 
 * @return int 
 */
int TyroApp::OnExit()
{
	return close(true);
}

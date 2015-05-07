/**
 * Main application file
 */

#include "wx_common.h"

#include <wx/app.h>
#include <wx/config.h>
#include <wx/debug.h>

class TyroApp : public wxApp
{
	friend class MainFrame;
public:
	virtual bool OnInit();
	virtual int OnExit();
private:
};

wxConfigBase *Config;

//**************************************************************
#include "widgets/widget.h"

IMPLEMENT_APP(TyroApp);

// Some global stuff
TyroMenu *mbar;
MainFrame *main_frame;

/**
 * Start the event loop and create the main window
 * 
 * @return bool 
 */
bool TyroApp::OnInit()
{
	this->SetAppName(APP_NAME);
	this->SetVendorName(APP_VENDOR);
	
	Config = wxConfigBase::Get();
	mbar = new TyroMenu();
	main_frame = new MainFrame(0L, APP_NAME);

	SetTopWindow(main_frame);
	
	main_frame->Layout();
	main_frame->CenterOnScreen();
	main_frame->Show(true);

	return true;
}

/**
 * Exit handler
 * 
 * @return int 
 */
int TyroApp::OnExit()
{
	// Deallocate config object
	delete wxConfigBase::Set((wxConfigBase *) NULL);
	
	return close(true);
}

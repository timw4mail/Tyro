/**
 * Main application file
 */

#include "wx_common.h"
#include "widgets/widget.h"

#include <wx/app.h>
#include <wx/debug.h>

// Some global stuff
wxConfigBase *Config;
TyroMenu *mbar;
MainFrame *main_frame;

/**
 * Class with main method
 */
class TyroApp : public wxApp
{
public:
	/**
	 * Start the event loop and create the main window
	 * 
	 * @return bool 
	 */
	bool OnInit()
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
	int OnExit()
	{
	   // Deallocate config object
	   delete wxConfigBase::Set((wxConfigBase *) NULL);

	   return close(true);
	}
};

IMPLEMENT_APP(TyroApp);

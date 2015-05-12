/**
 * Main application file
 */

#include "wx_common.h"
#include "widgets/widget.h"

#include <wx/app.h>


// Some global stuff
wxConfigBase *Glob_config;
TyroMenu *Glob_menu_bar;
MainFrame *Glob_main_frame;

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

	   // Initialize globals
	   Glob_config = wxConfigBase::Get();
	   Glob_menu_bar = new TyroMenu();
	   Glob_main_frame = new MainFrame(0L, APP_NAME);

	   SetTopWindow(Glob_main_frame);

	   // Setup Main Window
	   Glob_main_frame->Layout();
	   Glob_main_frame->CenterOnScreen();
	   Glob_main_frame->Show(true);

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

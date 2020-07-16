/**
 * Main application file
 */

#include "src/wx_common.h"
#include "src/widgets/widget.h"

#include <wx/app.h>
#include <wx/sysopt.h>
#include <wx/vidmode.h>

#include "src/widgets/MainFrame.h"

// All the ugly globals
wxConfigBase *Glob_config = nullptr;
LangConfig *Glob_lang_config = nullptr;
ThemeConfig *Glob_theme_config = nullptr;
MainFrame *Glob_main_frame = nullptr;
TyroMenu *Glob_menu_bar = nullptr;
wxStatusBar *Glob_status_bar = nullptr;

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
	bool OnInit() final
	{
		TyroApp::SetSystemOptions();
		this->SetAppName(APP_NAME);
		this->SetVendorName(APP_VENDOR);

		// Initialize globals
		Glob_config = wxConfigBase::Get();
		Glob_lang_config = new LangConfig();
		Glob_theme_config = new ThemeConfig();
		Glob_menu_bar = new TyroMenu();
		Glob_main_frame = new MainFrame(nullptr, APP_NAME, CalculateWindowSize());

		// Setup Main Window
		Glob_main_frame->Layout();
		Glob_main_frame->CenterOnScreen();
		Glob_main_frame->Show(true);
		
		// Open passed files
		if (this->param_count > 0)
		{
			Glob_main_frame->OpenFiles(files);
		}

		SetTopWindow(Glob_main_frame);

		return true;
	}
	
	/**
	 * Exit handler
	 *
	 * @return int 
	 */
	int OnExit() final
	{
		wxLogDebug("Closing App...");

		// Deallocate config object
		wxLogDebug("Deleting wxConfigBase");
		delete wxConfigBase::Set((wxConfigBase *) nullptr);
		
		return close(true);
	}
	
	/**
	 * Set up Command Line options
	 * 
	 * @param wxCmdLineParser& parser
	 * @return void
	 */
	void OnInitCmdLine(wxCmdLineParser &parser) final
	{
		parser.SetDesc(Glob_cmdLineDesc);

		// Set - as parameter delimeter, so raw file paths can be used
		parser.SetSwitchChars("-");
	}
	
	/**
	 * Handler for command line options
	 * 
	 * @param wxCmdLineParser& parser
	 * @return bool
	 */
	bool OnCmdLineParsed(wxCmdLineParser &parser) final
	{
		// Get un-named parameters
		this->param_count = parser.GetParamCount();

		wxLogDebug("%i Parameters", this->param_count);

		for (auto i = 0; i < this->param_count; i++)
		{
			this->files.Add(parser.GetParam(i));
		}

		return true;
	}
private:
	// app loading variables
	wxArrayString files;
	size_t param_count = 0;
	
	/**
	 * Toolkit-specific settings 
	 */
	void static SetSystemOptions()
	{
	#ifdef __WXMAC__
		wxSystemOptions::SetOption("osx.openfiledialog.always-show-types", 1);
	#endif

	#ifdef __WXMSW__
		wxSystemOptions::SetOption("msw.remap", 0);
		wxSystemOptions::SetOption("msw.display.directdraw", 1);
	#endif
	}
};

// Set up the main method and event loop
wxIMPLEMENT_APP(TyroApp);

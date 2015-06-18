/**
 * Main application file
 */

#include "wx_common.h"
#include "widgets/widget.h"

#include <wx/app.h>
#include <wx/sysopt.h>
#include <wx/vidmode.h>
#include <wx/display.h>


// Some global stuff
wxConfigBase *Glob_config = nullptr;
TyroMenu *Glob_menu_bar = nullptr;
wxStatusBar *Glob_status_bar = nullptr;
MainFrame *Glob_main_frame = nullptr;
StringConstMap Glob_lexer_map;

#ifndef TRAVIS
PrefPane *Glob_pref_pane = nullptr;
#endif

// Static app loading variables
static wxArrayString files;
static int param_count;

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
		if ( ! wxApp::OnInit()) return false;

		this->SetSystemOptions();
		this->SetAppName(APP_NAME);
		this->SetVendorName(APP_VENDOR);

		// Initialize globals
		this->InitLexerMap();
		Glob_config = wxConfigBase::Get();
		Glob_menu_bar = new TyroMenu();
		Glob_main_frame = new MainFrame(0L, APP_NAME, this->CalculateWindowSize());
#ifndef TRAVIS
		Glob_pref_pane = new PrefPane();
#endif

		// Setup Main Window
		Glob_main_frame->Layout();
		Glob_main_frame->CenterOnScreen();
		Glob_main_frame->Show(true);
		
		// Open passed files
		if (param_count > 0)
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
	int OnExit()
	{
		// Deallocate config object
		delete wxConfigBase::Set((wxConfigBase *) NULL);
		
		return close(true);
	}
	
	/**
	 * Set up Command Line options
	 * 
	 * @param wxCmdLineParser& parser
	 * @return void
	 */
	void OnInitCmdLine(wxCmdLineParser &parser)
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
	bool OnCmdLineParsed(wxCmdLineParser &parser)
	{
		// Get un-named parameters
		int i;
		param_count = parser.GetParamCount();

		wxLogDebug("%i Parameters", param_count);

		for (i = 0; i < param_count; i++)
		{
			files.Add(parser.GetParam(i));
		}

		return true;
	}
private:
	/**
	 * Set up mapping for lexers
	 */
	void InitLexerMap()
	{
		Glob_lexer_map[""] = wxSTC_LEX_NULL;
		Glob_lexer_map["batch"] = wxSTC_LEX_BATCH;
		Glob_lexer_map["caml"] = wxSTC_LEX_CAML;
		Glob_lexer_map["cmake"] = wxSTC_LEX_CMAKE;
		Glob_lexer_map["coffeescript"] = wxSTC_LEX_COFFEESCRIPT;
		Glob_lexer_map["cpp"] = wxSTC_LEX_CPP;
		Glob_lexer_map["css"] = wxSTC_LEX_CSS;
		Glob_lexer_map["fortran"] = wxSTC_LEX_FORTRAN;
		Glob_lexer_map["haskell"] = wxSTC_LEX_HASKELL;
		Glob_lexer_map["html"] = wxSTC_LEX_HTML;
		Glob_lexer_map["java"] = wxSTC_LEX_CPP;
		Glob_lexer_map["js"] = wxSTC_LEX_CPP;
		Glob_lexer_map["lisp"] = wxSTC_LEX_LISP;
		Glob_lexer_map["lua"] = wxSTC_LEX_LUA;
		Glob_lexer_map["makefile"] = wxSTC_LEX_MAKEFILE;
		Glob_lexer_map["markdown"] = wxSTC_LEX_MARKDOWN;
		Glob_lexer_map["php"] = wxSTC_LEX_HTML;
		Glob_lexer_map["perl"] = wxSTC_LEX_PERL;
		Glob_lexer_map["python"] = wxSTC_LEX_PYTHON;
		Glob_lexer_map["ruby"] = wxSTC_LEX_RUBY;
		Glob_lexer_map["rust"] = wxSTC_LEX_CPP;
		Glob_lexer_map["shell"] = wxSTC_LEX_BASH;
		Glob_lexer_map["sql"] = wxSTC_LEX_SQL;
		Glob_lexer_map["xml"] = wxSTC_LEX_XML;
		Glob_lexer_map["yaml"] = wxSTC_LEX_YAML;
	}
	
	/**
	 * Calculate original window size based on size of the current monitor
     */
	wxSize CalculateWindowSize()
	{
		wxDisplay display;
		wxVideoMode mode = display.GetCurrentMode();
		
		wxLogDebug("Current display: %ix%i", mode.w, mode.h);
		
		wxSize base((int)((float)mode.w * 0.8), (int)((float)mode.h * 0.8));
		
		return base;
	}
	
	/**
	 * Toolkit-specific settings 
     */
	void SetSystemOptions()
	{
	#ifdef __WXMAC__
		wxSystemOptions::SetOption("osx.openfiledialog.always-show-types", 1);
	#endif

	#ifdef __WXMSW__
		wxSystemOptions::SetOption("msw.display.directdraw", 1);
	#endif
	}
};

// Set up the main method and event loop
IMPLEMENT_APP(TyroApp);

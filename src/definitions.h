/**
 * Miscellaneous Program-specific definitions
 */
#pragma once

// Application config
const wxString APP_NAME = "Tyro";
const wxString APP_VENDOR = "Aviat Ion";
const wxString APP_VERSION = "0.10.0";
const wxString APP_VERSION_MORE = "Pre-release";

// Command-line arguments
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#endif
const wxCmdLineEntryDesc Glob_cmdLineDesc[] = {
	{
		wxCMD_LINE_PARAM,
		nullptr,
		nullptr,
		"input file(s)",
		wxCMD_LINE_VAL_STRING,
		wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE 
	},
	{wxCMD_LINE_NONE}
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

// Some boilerplate text
const wxString TYRO_SAVE_ERROR = "Failed to save the file. Maybe you lack the permissions.";
const wxString TYRO_SAVE_ERROR_CAPTION = "Saving Failed";
const wxString TYRO_OPEN_ERROR = "Failed to open the file. Check that it exists, and that you have read permissions.";
const wxString TYRO_OPEN_ERROR_CAPTION = "Open Failed";

// Font defaults
const int TYRO_DEFAULT_FONT_FAMILY = (int) wxFONTFAMILY_MODERN;
#ifdef __WXMAC__
const int TYRO_DEFAULT_FONT_SIZE = 14;
#else
const int TYRO_DEFAULT_FONT_SIZE = 10;
#endif

// Editor margins
enum myMargins
{
	MARGIN_FOLD,
	MARGIN_LINE_NUMBERS
};

// Status bar sections
enum myStatusBarSections {
	STATUS_MESSAGES,
	STATUS_CURSOR_LOCATION,
	STATUS_CURRENT_LANGUAGE,
	STATUS_LINE_ENDINGS,
	STATUS_CODE_PAGE,
	STATUS_INDENT
};

// Top level menus
enum myMenuIds {
	myFILE_MENU,
	myEDIT_MENU,
	myVIEW_MENU,
	myLANG_MENU,
	myHELP_MENU
};

// General Menu ids
enum myMenuItemIds {
	// Menu options for sidebar
	myID_OPEN_DIR = wxID_HIGHEST + 1,

	// Menu options for immediate file
	myID_VIEW_WHITESPACE,
	myID_VIEW_LINE_ENDINGS,
	myID_CLOSE_ALL,
	myID_CLOSE_ALL_BUT_THIS,
	myID_LINE_WRAP,
	
	// Preferences, to apply to all files
	myID_PREFS_LINE_NUMBERS,
	myID_PREFS_CODE_FOLDING,
	myID_PREFS_IDENT_GUIDES,
	myID_PREFS_FONT
};

const wxString TYRO_FILE_OPEN_WILDCARDS =
	"All files (*.*)|*|"
	"Batch (*.bat, *.cmd, *.nt)|*.bat;*.cmd,*.nt|"
	"Caml (*.ml,*.mli,*.sml,*.thy)|*.ml;*.mli;*.sml;*.thy|"
	"Cmake (*.cmake) |*.cmake|"
	"C/C++ (*.c,*.cpp,*.h)|*.c;*.cc;*.cpp;*.cxx;*.cs;*.h;*.hh;*.hpp;*.hxx;*.sma;*.cp|"
	"CSS (*.css)|*.css|"
	"Fortran (*.f9*, *.f, *.for)|*.f9*;*.f,*.for|"
	"Haskell (*.hs,*.lhs,*.las)|*.hs;*.lhs;*.las|"
	"HTML/XHTML (*.html, *.htm)|*.htm*|"
	"Java (*.java)|*.java|"
	"JavaScript (*.js,*.json,*.ts)|*.js;*.json;*.ts|"
	"Lisp (*.lsp,*.lisp)|*.lsp;*.lisp|"
	"Lua (*.lua)|*.lua|"
	"Makefile |Makefile;makefile.*;MAKEFILE;configure.*;*.mak|"
	"Markdown (*.md,*.markdown)|*.md;*.markdown|"
	"Perl (*.pl, *.cgi)|*.pl;*.pm;*.cgi;*.pod|"
	"PHP (*.php)|*.php|"
	"Python (*.py,*.pyw)|*.py;*.pyw|"
	"Ruby (*.rb)|*.rb|"
	"Rust (*.rs)|*.rs|"
 	"Scheme (*.scm)|*.scm|"
	"Shell (*.sh, *.bsh)|*.sh;*.bsh|"
	"SQL (*.sql)|*.sql|"
	"Text (*.txt)|*.txt|"
	"Yaml (.yml,*.yaml)| *.yml;*.yaml";

const wxString TYRO_FILE_SAVE_WILDCARDS = TYRO_FILE_OPEN_WILDCARDS;

/**
 * Miscellaneous Program-specific definitions
 */

#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

enum {
	myFILE_MENU,
	myEDIT_MENU,
	myVIEW_MENU,
	myHELP_MENU
};

// Application config
const wxString APP_NAME = "Tyro";
const wxString APP_VENDOR = "Aviat Ion";
const wxString APP_VERSION = "0.0.1";

// Some boilerplate text
const wxString TYRO_SAVE_ERROR = "Failed to save the file. Maybe you lack the permissions.";
const wxString TYRO_SAVE_ERROR_CAPTION = "Saving Failed";
const wxString TYRO_OPEN_ERROR = "Failed to open the file. Check that it exists, and that you have read permissions.";
const wxString TYRO_OPEN_ERROR_CAPTION = "Open Failed";

// EditPane file extension to lexer mapping
typedef map<string, int> StringConstMap;

const wxString TYRO_FILE_OPEN_WILDCARDS =
	"All files (*.*)|*|"
	"Bash (*.sh, *.bsh) |*.sh;*.bsh|"
	"Batch (*.bat, *.cmd, *.nt)|*.bat;*.cmd,*.nt|"
	"C/C++ (*.c,*.cpp,*.h)| *.c;*.cc;*.cpp;*.cxx;*.cs;*.h;*.hh;*.hpp;*.hxx;*.sma;*.cp |"
	"CSS (*.css)|*.css|"
	"Fortran (*.f9*, *.f, *.for)|*.f9*;*.f,*.for|"
	"HTML/XHTML (*.html, *.htm)|*.htm*|"
	"Java (*.java)|*.java|"
	"JavaScript(*.js)|*.js|"
	"Makefile |Makefile;makefile.*;MAKEFILE;configure.*;*.mak|"
	"Pascal (*.pas, *.inc, *.pp)|*.pas;*.inc;*.pp|"
	"Perl (*.pl, *.cgi)|*.pl;*.pm;*.cgi;*.pod|"
	"PHP (*.php)|*.php|"
	"Ruby (*.rb)|*.rb|"
	"SQL (*.sql)|*.sql|"
	"TCL (*.tcl)|*.tcl|"
	"Text (*.txt)|*.txt";

const wxString TYRO_FILE_SAVE_WILDCARDS = 
	"All files (*.*)|*.*|"
	"Bash (*.sh, *.bsh) |*.sh;*.bsh|"
	"Batch (*.bat, *.cmd, *.nt)|*.bat;*.cmd,*.nt|"
	"C/C++ (*.c,*.cpp,*.h)| *.c;*.cc;*.cpp;*.cxx;*.cs;*.h;*.hh;*.hpp;*.hxx;*.sma;*.cp |"
	"CSS (*.css)|*.css|"
	"Fortran (*.f9*, *.f, *.for)|*.f9*;*.f,*.for|"
	"HTML/XHTML (*.html, *.htm)|*.htm*|"
	"Java (*.java)|*.java|"
	"JavaScript(*.js)|*.js|"
	"Makefile |Makefile;makefile.*;MAKEFILE;configure.*;*.mak|"
	"Pascal (*.pas, *.inc, *.pp)|*.pas;*.inc;*.pp|"
	"Perl (*.pl, *.cgi)|*.pl;*.pm;*.cgi;*.pod|"
	"PHP (*.php)|*.php|"
	"Ruby (*.rb)|*.rb|"
	"SQL (*.sql)|*.sql|"
	"TCL (*.tcl)|*.tcl|"
	"Text (*.txt)|*.txt";

#endif	/* DEFINITIONS_H */


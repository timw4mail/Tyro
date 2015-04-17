/**
 * Miscellaneous Program-specific definitions
 */

#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

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
typedef pair<string, int> StringConstMapData;
typedef map<string, int> StringConstMap;

const wxString TYRO_FILE_OPEN_WILDCARDS =
	_T("All files (*.*)|*.*|")
	_T("Bash (*.sh, *.bsh) |*.sh;*.bsh|")
	_T("Batch (*.bat, *.cmd, *.nt)|*.bat;*.cmd,*.nt|")
	_T("C/C++ (*.c,*.cpp,*.h)| *.c;*.cc;*.cpp;*.cxx;*.cs;*.h;*.hh;*.hpp;*.hxx;*.sma;*.cp |")
	_T("CSS (*.css)|*.css|")
	_T("Fortran (*.f9*, *.f, *.for)|*.f9*;*.f,*.for|")
	_T("HTML/XHTML (*.html, *.htm)|*.htm*|")
	_T("Java (*.java)|*.java|")
	_T("JavaScript(*.js)|*.js|")
	_T("Makefile |Makefile;makefile.*;MAKEFILE;configure.*;*.mak|")
	_T("Pascal (*.pas, *.inc, *.pp)|*.pas;*.inc;*.pp|")
	_T("Perl (*.pl, *.cgi)|*.pl;*.pm;*.cgi;*.pod|")
	_T("PHP (*.php)|*.php|")
	_T("Ruby (*.rb)|*.rb|")
	_T("SQL (*.sql)|*.sql|")
	_T("TCL (*.tcl)|*.tcl|")
	_T("Text (*.txt)|*.txt");

const wxString TYRO_FILE_SAVE_WILDCARDS = 
	_T("All files (*.*)|*.*|")
	_T("Bash (*.sh, *.bsh) |*.sh;*.bsh|")
	_T("Batch (*.bat, *.cmd, *.nt)|*.bat;*.cmd,*.nt|")
	_T("C/C++ (*.c,*.cpp,*.h)| *.c;*.cc;*.cpp;*.cxx;*.cs;*.h;*.hh;*.hpp;*.hxx;*.sma;*.cp |")
	_T("CSS (*.css)|*.css|")
	_T("Fortran (*.f9*, *.f, *.for)|*.f9*;*.f,*.for|")
	_T("HTML/XHTML (*.html, *.htm)|*.htm*|")
	_T("Java (*.java)|*.java|")
	_T("JavaScript(*.js)|*.js|")
	_T("Makefile |Makefile;makefile.*;MAKEFILE;configure.*;*.mak|")
	_T("Pascal (*.pas, *.inc, *.pp)|*.pas;*.inc;*.pp|")
	_T("Perl (*.pl, *.cgi)|*.pl;*.pm;*.cgi;*.pod|")
	_T("PHP (*.php)|*.php|")
	_T("Ruby (*.rb)|*.rb|")
	_T("SQL (*.sql)|*.sql|")
	_T("TCL (*.tcl)|*.tcl|")
	_T("Text (*.txt)|*.txt");




#endif	/* DEFINITIONS_H */


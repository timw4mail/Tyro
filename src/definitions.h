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
typedef map<string, int> StringConstMap;
typedef map<string, string> StringMap;

const wxString TYRO_FILE_OPEN_WILDCARDS =
	"All files (*.*)|*|"
	"Batch (*.bat, *.cmd, *.nt)|*.bat;*.cmd,*.nt|"
	"Caml (*.ml,*.mli,*.sml,*.thy)|*.ml;*.mli;*.sml;*.thy|"
	"Cmake (*.cmake) |*.cmake|"
	"C/C++ (*.c,*.cpp,*.h)|*.c;*.cc;*.cpp;*.cxx;*.cs;*.h;*.hh;*.hpp;*.hxx;*.sma;*.cp|"
	"Coffeescript (*.coffee)|*.coffee;*.litcoffee|"
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
	"Shell (*.sh, *.bsh)|*.sh;*.bsh|"
	"SQL (*.sql)|*.sql|"
	"Text (*.txt)|*.txt|"
	"Yaml (.yml,*.yaml)| *.yml;*.yaml";

const wxString TYRO_FILE_SAVE_WILDCARDS = 
	"All files (*.*)|*|"
	"Batch (*.bat, *.cmd, *.nt)|*.bat;*.cmd,*.nt|"
	"Caml (*.ml,*.mli,*.sml,*.thy)|*.ml;*.mli;*.sml;*.thy|"
	"Cmake (*.cmake) |*.cmake|"
	"C/C++ (*.c,*.cpp,*.h)|*.c;*.cc;*.cpp;*.cxx;*.cs;*.h;*.hh;*.hpp;*.hxx;*.sma;*.cp|"
	"Coffeescript (*.coffee)|*.coffee;*.litcoffee|"
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
	"Shell (*.sh, *.bsh)|*.sh;*.bsh|"
	"SQL (*.sql)|*.sql|"
	"Text (*.txt)|*.txt|"
	"Yaml (.yml,*.yaml)| *.yml;*.yaml";

#endif	/* DEFINITIONS_H */


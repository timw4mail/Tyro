

#ifndef EDITPANEDEFINITIONS_H
#define	EDITPANEDEFINITIONS_H

#include <wx/stc/stc.h>

// EditPane file extension to lexer mapping
map<string,int> TYROLEXER_MAPPING = {
	{"c",	wxSTC_LEX_CPP},
	{"h",	wxSTC_LEX_CPP},
	{"cpp", wxSTC_LEX_CPP},
	{"cxx", wxSTC_LEX_CPP},
	{"py",	wxSTC_LEX_PYTHON},
	{"php", wxSTC_LEX_PHPSCRIPT}
};

#endif	/* EDITPANEDEFINITIONS_H */


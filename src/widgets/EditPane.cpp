#include "EditPane.h"

EditPane::EditPane(
	wxWindow *parent, wxWindowID id, const wxPoint &pos, 
	const wxSize &size, long style
) : wxStyledTextCtrl (parent, id, pos, size, style)
{
	StringConstMapData map_data[] = {
		{"c", wxSTC_LEX_CPP},
		{"h", wxSTC_LEX_CPP},
		{"cpp", wxSTC_LEX_CPP},
		{"cxx", wxSTC_LEX_CPP},
		{"py", wxSTC_LEX_PYTHON},
		{"php", wxSTC_LEX_PHPSCRIPT}
	};

	lexer_map = StringConstMap(
		map_data,
		map_data + sizeof map_data / sizeof map_data[0]
	);
}

EditPane::~EditPane() {}

/**
 * Encapsulate lexer selection when opening a file
 *
 * @param wxString filePath
 * @return bool
 */
bool EditPane::LoadAndHighlight(wxString filePath)
{
	wxFileName file(filePath);
	wxString ext = file.GetExt();

	//lexer_map_it = lexer_map.find((string) ext);
	//this->SetLexer(lexer_map_it->second);

	return this->LoadFile(filePath);
}

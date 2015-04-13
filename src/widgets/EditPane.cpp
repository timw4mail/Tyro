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
		{"php", wxSTC_LEX_PHPSCRIPT},
		{"js", wxSTC_LEX_ESCRIPT},
		{"json", wxSTC_LEX_ESCRIPT}
	};

	lexer_map = StringConstMap(
		map_data,
		map_data + sizeof map_data / sizeof map_data[0]
	);
}

EditPane::~EditPane() {}

void EditPane::OnSize(wxSizeEvent &event)
{
		
}

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
	
	this->StyleSetForeground (wxSTC_STYLE_DEFAULT, wxColor(101, 123, 131));
    this->StyleSetBackground (wxSTC_STYLE_DEFAULT, wxColor(253, 246, 227));
    this->StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColor(147, 161, 161));
	
	this->SetMarginWidth (MARGIN_LINE_NUMBERS, 50);
	this->StyleSetForeground (wxSTC_STYLE_LINENUMBER, wxColor(147, 161, 161));
	this->StyleSetBackground (wxSTC_STYLE_LINENUMBER, wxColor(238, 232, 213));
	this->SetMarginType (MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);
	
	this->SetProperty (wxT("fold"),         wxT("1") );
	this->SetProperty (wxT("fold.comment"), wxT("1") );
	this->SetProperty (wxT("fold.compact"), wxT("1") );
	
	
	this->StyleSetForeground (wxSTC_C_STRING,            wxColour(150,0,0));
	this->StyleSetBackground (wxSTC_C_STRING, wxColor(253, 246, 227));
	this->StyleSetForeground (wxSTC_C_PREPROCESSOR,      wxColour(165,105,0));
	this->StyleSetBackground (wxSTC_C_PREPROCESSOR, wxColor(253, 246, 227));
	this->StyleSetForeground (wxSTC_C_IDENTIFIER,        wxColour(40,0,60));
	this->StyleSetBackground (wxSTC_C_IDENTIFIER, wxColor(253, 246, 227));
	this->StyleSetForeground (wxSTC_C_NUMBER,            wxColour(0,150,0));
	this->StyleSetBackground (wxSTC_C_NUMBER, wxColor(253, 246, 227));
	this->StyleSetForeground (wxSTC_C_CHARACTER,         wxColour(150,0,0));
	this->StyleSetBackground (wxSTC_C_CHARACTER, wxColor(253, 246, 227));
	this->StyleSetForeground (wxSTC_C_WORD,              wxColour(0,0,150));
	this->StyleSetBackground (wxSTC_C_WORD, wxColor(253, 246, 227));
	this->StyleSetForeground (wxSTC_C_WORD2,             wxColour(0,150,0));
	this->StyleSetBackground (wxSTC_C_WORD2, wxColor(253, 246, 227));
	this->StyleSetForeground (wxSTC_C_COMMENT,           wxColor(147, 161, 161));
	this->StyleSetBackground (wxSTC_C_COMMENT, wxColor(253, 246, 227));
	this->StyleSetForeground (wxSTC_C_COMMENTLINE,       wxColor(147, 161, 161));
	this->StyleSetBackground (wxSTC_C_COMMENTLINE, wxColor(253, 246, 227));
	this->StyleSetForeground (wxSTC_C_COMMENTDOC,        wxColor(147, 161, 161));
	this->StyleSetBackground (wxSTC_C_COMMENTDOC, wxColor(253, 246, 227));
	this->StyleSetForeground (wxSTC_C_COMMENTDOCKEYWORD, wxColour(0,0,200));
	this->StyleSetBackground (wxSTC_C_COMMENTDOCKEYWORD, wxColor(253, 246, 227));
	this->StyleSetForeground (wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour(0,0,200));
	this->StyleSetBackground (wxSTC_C_COMMENTDOCKEYWORDERROR, wxColor(253, 246, 227));
	/*this->StyleSetBold(wxSTC_C_WORD, true);
	this->StyleSetBold(wxSTC_C_WORD2, true);
	this->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);*/

	lexer_map_it = lexer_map.find((string) ext);
	this->SetLexer(lexer_map_it->second);

	return this->LoadFile(filePath);
}

#include "EditPane.h"

EditPane::EditPane(
	wxWindow *parent, wxWindowID id, const wxPoint &pos, 
	const wxSize &size, long style
) : wxStyledTextCtrl (parent, id, pos, size, style)
{	
	config = new TyroConfig();
	lexerMap["batch"] = wxSTC_LEX_BATCH;
	lexerMap["caml"] = wxSTC_LEX_CAML;
	lexerMap["cmake"] = wxSTC_LEX_CMAKE;
	lexerMap["cpp"] = wxSTC_LEX_CPP;
	lexerMap["css"] = wxSTC_LEX_CSS;
	lexerMap["js"] = wxSTC_LEX_CPP;
	lexerMap["html"] = wxSTC_LEX_HTML;
	lexerMap["makefile"] = wxSTC_LEX_MAKEFILE;
	lexerMap["php"] = wxSTC_LEX_PHPSCRIPT;
	lexerMap["perl"] = wxSTC_LEX_PERL;
	lexerMap["python"] = wxSTC_LEX_PYTHON;
	lexerMap["shell"] = wxSTC_LEX_BASH;
}

EditPane::~EditPane() 
{
	delete config;
}

string EditPane::GetLangByFile(const wxString &filename)
{
	JsonValue langList = config->GetRoot();
	JsonValue::iterator it;
	
	// Loop through each language to find a matching file pattern
	for (it = langList.begin(); it != langList.end(); ++it)
	{
		string lang = it.key().asString();
		
		
		// Parse the file pattern
		wxString file_pattern((*it)["file_pattern"].asString());
		
		file_pattern.Lower();
		
		while ( ! file_pattern.empty())
		{
			wxString cur = file_pattern.BeforeFirst(';');
			if (
				(cur == filename) ||
				(cur == (filename.BeforeLast('.') + _T(".*"))) ||
				(cur == (_T("*.") + filename.AfterLast('.')))
			)
			{
				return lang;
			}
			file_pattern = file_pattern.AfterFirst(';');
		}
	}
	
	return "";
}

/**
 * Encapsulate lexer selection when opening a file
 *
 * @param wxString filePath
 * @return bool
 */
bool EditPane::LoadAndHighlight(wxString filePath)
{
	fileName = filePath;
	string lang = this->GetLangByFile(filePath);
	
	this->StyleClearAll();
	
	if (lexerMap.count(lang) > 0)
	{
		this->SetLexer(lexerMap[lang]);
	}
	else
	{
		this->SetLexer(wxSTC_LEX_NULL);
	}
	
	// Get the list of keywords for the current language
	JsonValue keywords_array = config->GetLangKeywords(lang);
	
	// Make sure every background is the same color!
	for(int i = 0; i <= wxSTC_STYLE_MAX; i++)
	{
		this->StyleSetBackground(i, wxColor(253, 246, 227));
		this->StyleSetFaceName(i, "Anonymous Pro");
	}
	
	this->StyleSetForeground (wxSTC_STYLE_DEFAULT, wxColor(101, 123, 131));
	this->StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColor(147, 161, 161));
	
	this->SetMarginWidth (MARGIN_LINE_NUMBERS, 50);
	this->StyleSetForeground (wxSTC_STYLE_LINENUMBER, wxColor(147, 161, 161));
	this->StyleSetBackground (wxSTC_STYLE_LINENUMBER, wxColor(238, 232, 213));
	this->SetMarginType (MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);
	
	this->SetProperty (wxT("fold"),         wxT("1") );
	this->SetProperty (wxT("fold.comment"), wxT("1") );
	this->SetProperty (wxT("fold.compact"), wxT("1") );
	
	this->StyleSetForeground(wxSTC_C_DEFAULT, wxColor(101, 123, 131));
	this->StyleSetForeground(wxSTC_C_OPERATOR, wxColor(101, 123, 131));
	this->StyleSetForeground(wxSTC_C_HASHQUOTEDSTRING, wxColor(181, 137, 0));
	this->StyleSetForeground(wxSTC_C_STRING, wxColor(108, 113, 196));
	this->StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColor(181, 137,   0));
	this->StyleSetForeground(wxSTC_C_PREPROCESSORCOMMENT, wxColor(181, 137,   0));
	this->StyleSetForeground(wxSTC_C_IDENTIFIER, wxColor(38, 139, 210));
	this->StyleSetForeground(wxSTC_C_NUMBER, wxColor(211, 54, 130));
	this->StyleSetForeground(wxSTC_C_CHARACTER, wxColor(108, 113, 196));
	this->StyleSetForeground(wxSTC_C_WORD2, wxColor(203,  75,  22));
	this->StyleSetForeground(wxSTC_C_WORD, wxColor(220,  50,  47));
	this->StyleSetForeground(wxSTC_C_COMMENT, wxColor(147, 161, 161));
	this->StyleSetForeground(wxSTC_C_COMMENTLINE, wxColor(147, 161, 161));
	this->StyleSetForeground(wxSTC_C_COMMENTLINEDOC, wxColor(147, 161, 161));
	this->StyleSetForeground(wxSTC_C_COMMENTDOC, wxColor(147, 161, 161));
	this->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, wxColor(131, 148, 150));
	this->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, wxColor(101, 123, 131));
	this->StyleSetBold(wxSTC_C_WORD, false);
	this->StyleSetBold(wxSTC_C_WORD2, true);
	this->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);
	this->StyleSetBold(wxSTC_C_OPERATOR, true);
	
	if (keywords_array.isArray())
	{
		for(int i = 0; i < keywords_array.size(); i++)
		{
			this->SetKeyWords(i, keywords_array[i].asString());
		}
	}
	else
	{
		cerr << "keywords array is not an array" << endl;
		cerr << "keyword array is a " << keywords_array.type() << endl;
	}

	return this->LoadFile(filePath);
}

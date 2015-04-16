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
	lexerMap["php"] = wxSTC_LEX_PHPSCRIPT | wxSTC_LEX_HTML;
	lexerMap["perl"] = wxSTC_LEX_PERL;
	lexerMap["python"] = wxSTC_LEX_PYTHON;
	lexerMap["shell"] = wxSTC_LEX_BASH;
	
	this->BindEvents();
}

EditPane::~EditPane() 
{
	delete config;
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
	string lang = this->GetLangByFile();
	
	this->StyleClearAll();
	
	// Font setup
	wxFont *defaultFont = wxFont::New(
		14, 
		wxFONTFAMILY_MODERN, 
		wxFONTFLAG_ANTIALIASED,
		"Anonymous Pro"
	);
	
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
		this->StyleSetFont(i, *defaultFont);
	}
	
	this->StyleSetForeground (wxSTC_STYLE_DEFAULT, wxColor(101, 123, 131));
	this->StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColor(147, 161, 161));
	
	this->SetMarginWidth (MARGIN_LINE_NUMBERS, TextWidth(wxSTC_STYLE_LINENUMBER, _T("_9999")));
	this->StyleSetForeground (wxSTC_STYLE_LINENUMBER, wxColor(147, 161, 161));
	this->StyleSetBackground (wxSTC_STYLE_LINENUMBER, wxColor(238, 232, 213));
	this->SetMarginType (MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);
	
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
	
	// Set up Code folding
	this->SetProperty(wxT("fold"), wxT("1"));
	this->SetProperty(wxT("fold.comment"), wxT("1"));
	this->SetProperty(wxT("fold.compact"), wxT("1"));
	this->SetProperty(wxT("fold.html"), wxT("1"));
	this->SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);
	this->SetMarginType(MARGIN_FOLD, wxSTC_MARGIN_SYMBOL);
	this->SetMarginWidth(MARGIN_FOLD, 16);
	this->SetMarginSensitive(MARGIN_FOLD, true);
	this->SetMarginMask(MARGIN_FOLD, wxSTC_MASK_FOLDERS);
	this->MarkerDefine (wxSTC_MARKNUM_FOLDER,        wxSTC_MARK_BOXPLUSCONNECTED, _T("WHITE"), _T("BLACK"));
    this->MarkerDefine (wxSTC_MARKNUM_FOLDEROPEN,    wxSTC_MARK_BOXMINUSCONNECTED, _T("WHITE"), _T("BLACK"));
    this->MarkerDefine (wxSTC_MARKNUM_FOLDERSUB,     wxSTC_MARK_VLINE,     _T("BLACK"), _T("BLACK"));
    this->MarkerDefine (wxSTC_MARKNUM_FOLDEREND,     wxSTC_MARK_CIRCLEPLUSCONNECTED,  _T("WHITE"), _T("BLACK"));
    this->MarkerDefine (wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_CIRCLEMINUSCONNECTED,  _T("WHITE"), _T("BLACK"));
    this->MarkerDefine (wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER,     _T("BLACK"), _T("BLACK"));
    this->MarkerDefine (wxSTC_MARKNUM_FOLDERTAIL,    wxSTC_MARK_LCORNER,     _T("BLACK"), _T("BLACK"));
	
	this->SetLayoutCache (wxSTC_CACHE_NONE);
    this->SetUseHorizontalScrollBar(1);
	
	// set spaces and indention
    this->SetTabWidth(4);
    this->SetUseTabs(true);
    this->SetTabIndents(true);
    this->SetBackSpaceUnIndents(true);
	
	if (keywords_array.isArray())
	{
		for(int i = 0; i < keywords_array.size(); i++)
		{
			this->SetKeyWords(i, keywords_array[i].asString());
		}
	}
	else
	{
		string typeMap[] = {"null", "int", "unsigned int", "double", "string", "boolean", "array", "object"};
		
		cerr << "current lang is:" << lang << endl;
		cerr << "keywords array is not an array" << endl;
		cerr << "keyword array is a " << typeMap[keywords_array.type()] << endl;
	}

	return this->LoadFile(filePath);
}

string EditPane::GetLangByFile()
{
	JsonValue langList = config->GetRoot();
	JsonValue::iterator it;
	
	wxFileName fname(this->fileName);
	wxString curr_file = fname.GetFullName();
	
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
				(cur == curr_file) ||
				(cur == (curr_file.BeforeLast('.') + _T(".*"))) ||
				(cur == (_T("*.") + curr_file.AfterLast('.')))
			)
			{
				return lang;
			}
			file_pattern = file_pattern.AfterFirst(';');
		}
	}
	
	return "";
}

bool EditPane::SaveFile()
{
	if ( ! this->fileName)
	{
		wxFileDialog dlg (
			this, 
			_T("Save file"), 
			wxEmptyString, 
			wxEmptyString,
			TYRO_FILE_SAVE_WILDCARDS,
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT
		);
		
		if (dlg.ShowModal() != wxID_OK) return false;
		this->fileName = dlg.GetPath();
	}
	
	return this->SaveFile(this->fileName);
}

bool EditPane::SaveFile(const wxString &filename)
{
	if ( ! this->IsModified()) return true;
    return wxStyledTextCtrl::SaveFile(filename);
}

bool EditPane::IsModified()
{
	return (GetModify() && !GetReadOnly());
}

void EditPane::BindEvents()
{
	Bind(wxEVT_STC_MARGINCLICK, &EditPane::OnMarginClick, this, wxID_ANY);
}

void EditPane::OnMarginClick(wxStyledTextEvent& event)
{
	if (event.GetMargin() == MARGIN_FOLD) {
        int lineClick = this->LineFromPosition (event.GetPosition());
        int levelClick = this->GetFoldLevel (lineClick);
        if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) {
            this->ToggleFold (lineClick);
        }
    }
}

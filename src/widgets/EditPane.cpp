#include "EditPane.h"

EditPane::EditPane(
	wxWindow *parent, wxWindowID id, const wxPoint &pos,
	const wxSize &size, long style
) : wxStyledTextCtrl (parent, id, pos, size, style)
{
	#include "../../config/languages_json.h"
	lang_config = new TyroConfig();
	lang_config->LoadJson(languages_json);

	#include "../../config/themes_json.h"
	theme_config = new TyroConfig();
	theme_config->LoadJson(themes_json);

	// Map language types to their lexers
	lexerMap["batch"] = wxSTC_LEX_BATCH;
	lexerMap["caml"] = wxSTC_LEX_CAML;
	lexerMap["cmake"] = wxSTC_LEX_CMAKE;
	lexerMap["coffeescript"] = wxSTC_LEX_COFFEESCRIPT;
	lexerMap["cpp"] = wxSTC_LEX_CPP;
	lexerMap["css"] = wxSTC_LEX_CSS;
	lexerMap["fortran"] = wxSTC_LEX_FORTRAN;
	lexerMap["haskell"] = wxSTC_LEX_HASKELL;
	lexerMap["html"] = wxSTC_LEX_HTML;
	lexerMap["java"] = wxSTC_LEX_CPP;
	lexerMap["js"] = wxSTC_LEX_CPP;
	lexerMap["lisp"] = wxSTC_LEX_LISP;
	lexerMap["lua"] = wxSTC_LEX_LUA;
	lexerMap["makefile"] = wxSTC_LEX_MAKEFILE;
	lexerMap["markdown"] = wxSTC_LEX_MARKDOWN;
	lexerMap["php"] = wxSTC_LEX_PHPSCRIPT;
	lexerMap["perl"] = wxSTC_LEX_PERL;
	lexerMap["python"] = wxSTC_LEX_PYTHON;
	lexerMap["ruby"] = wxSTC_LEX_RUBY;
	lexerMap["shell"] = wxSTC_LEX_BASH;
	lexerMap["sql"] = wxSTC_LEX_SQL;
	lexerMap["xml"] = wxSTC_LEX_XML;
	lexerMap["yaml"] = wxSTC_LEX_YAML;

	this->BindEvents();
}

EditPane::~EditPane()
{
	delete lang_config;
	delete theme_config;
}

/**
 * Handle the highlighting config for the
 * selected file
 *
 * @param wxString filePath
 * @return void
 */
void EditPane::Highlight(wxString filePath)
{
	this->fileName.Assign(filePath);

	wxLogDebug("Highlighting method");

	// Get the configuration name for the selected language
	string lang = this->GetLangByFile();

	this->StyleClearAll();

	if (lexerMap.count(lang) > 0)
	{
		this->SetLexer(lexerMap[lang]);
	}
	else
	{
		this->SetLexer(wxSTC_LEX_NULL);
	}

	// Some basic properties to set
	this->SetProperty("technology", "2");
	this->SetProperty("error.inline", "0");
	this->SetProperty("font.quality", "3"); // LCD Optimized

	// Apply the theme
	this->ApplyTheme(lang);

	// Set up Code folding
	this->SetProperty("fold", "1");
	this->SetProperty("fold.comment", "1");
	this->SetProperty("fold.compact", "1");
	this->SetProperty("fold.html", "1");
	this->SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);
	this->SetMarginType(MARGIN_FOLD, wxSTC_MARGIN_SYMBOL);
	this->SetMarginWidth(MARGIN_FOLD, 16);
	this->SetMarginSensitive(MARGIN_FOLD, true);
	this->SetMarginMask(MARGIN_FOLD, wxSTC_MASK_FOLDERS);
	this->MarkerDefine (wxSTC_MARKNUM_FOLDER,        wxSTC_MARK_BOXPLUSCONNECTED, "WHITE", "BLACK");
    this->MarkerDefine (wxSTC_MARKNUM_FOLDEROPEN,    wxSTC_MARK_BOXMINUSCONNECTED, "WHITE", "BLACK");
    this->MarkerDefine (wxSTC_MARKNUM_FOLDERSUB,     wxSTC_MARK_VLINE,     "BLACK", "BLACK");
    this->MarkerDefine (wxSTC_MARKNUM_FOLDEREND,     wxSTC_MARK_CIRCLEPLUSCONNECTED,  "WHITE", "BLACK");
    this->MarkerDefine (wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_CIRCLEMINUSCONNECTED,  "WHITE", "BLACK");
    this->MarkerDefine (wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER,     "BLACK", "BLACK");
    this->MarkerDefine (wxSTC_MARKNUM_FOLDERTAIL,    wxSTC_MARK_LCORNER,     "BLACK", "BLACK");

	this->SetLayoutCache (wxSTC_CACHE_NONE);
    this->SetUseHorizontalScrollBar(1);

	// set spaces and indention
    this->SetTabWidth(4);

	bool use_tabs = (lang != "yaml");

    this->SetUseTabs(use_tabs);
    this->SetTabIndents(true);
    this->SetBackSpaceUnIndents(true);
}

/**
 * Set the current theme for the current language
 * 
 * @param string lang
 * @param string theme
 * @return void
 */
void EditPane::ApplyTheme(string lang, string theme)
{
	this->SetTheme(theme);

	JsonValue lang_list = lang_config->GetRoot();
	JsonValue lexer_map = lang_list.get(lang, JsonValue()).get("lexer_map", JsonValue());

	// Get the list of keywords for the current language
	JsonValue keywords_array = this->GetKeywordList(lang);
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
		stringstream output;

		output << "current lang is:" << lang << endl;
		output << "keywords array is not an array" << endl;
		output << "keyword array is a " << typeMap[keywords_array.type()] << endl;

		wxLogDebug(output.str().c_str());
	}

	// Do the appropriate mappings to load the selected theme
	this->_ApplyTheme(lexer_map);
}

/**
 * Check file path and open the selected file
 *
 * @param wxString filePath
 * @return bool
 */
bool EditPane::Load(wxString filePath)
{
	this->fileName.Assign(filePath);

	if (this->FileReadable())
	{
		this->Highlight(filePath);
		bool didLoad = this->LoadFile(filePath);

		// @TODO Toggle controls based on write permission

		return didLoad;
	}

	return false;
}

/**
 * Determine the format of the current file by
 * matching its extension against the patterns
 * in the configuration files
 *
 * @return string
 */
string EditPane::GetLangByFile()
{
	JsonValue langList = lang_config->GetRoot();
	JsonValue::iterator it;

	wxString curr_file = this->fileName.GetFullName();

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
				(cur == (curr_file.BeforeLast('.') + ".*")) ||
				(cur == ("*." + curr_file.AfterLast('.')))
			)
			{
				return lang;
			}
			
			// Go to the next pattern for this language
			file_pattern = file_pattern.AfterFirst(';');
		}
	}

	return "";
}

bool EditPane::SaveFile()
{
	wxString fname;

	if ( ! this->fileName.IsOk())
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
		fname = dlg.GetPath();
	}
	else
	{
		fname = this->fileName.GetFullPath();
	}

	const wxString cfname(fname);

	return this->SaveFile(cfname);
}

bool EditPane::SaveFile(const wxString &filename)
{
	if ( ! this->IsModified()) return true;

	this->fileName.Assign(filename);

	// Check file permissions
	if (this->FileWritable())
	{
		this->SetSavePoint();
		return wxStyledTextCtrl::SaveFile(filename);
	}

	return false;
}

bool EditPane::IsModified()
{
	return this->GetModify();
}

/**
 * Check that the current file can be opened
 *
 * @return bool
 */
bool EditPane::FileReadable()
{
	if (
		this->fileName.IsOk() &&
		this->fileName.Exists() &&
		this->fileName.IsFileReadable()
	) return true;

	// Hmm...well, let's give an error
	wxMessageDialog errDlg(
		this,
		TYRO_OPEN_ERROR,
		TYRO_OPEN_ERROR_CAPTION,
		wxOK | wxICON_ERROR | wxCENTER
	);
	errDlg.ShowModal();

	return false;
}

/**
 * Check that the current file can be saved
 *
 * @return bool
 */
bool EditPane::FileWritable()
{
	// Lets see...can we write somewhere?
	if (
		this->fileName.IsOk() &&
		((this->fileName.Exists() && this->fileName.IsFileWritable()) ||
		(( ! this->fileName.Exists()) && this->fileName.IsDirWritable()))
	) return true;

	// Hmm...well, let's give an error
	wxMessageDialog errDlg(
		this,
		TYRO_SAVE_ERROR,
		TYRO_SAVE_ERROR_CAPTION,
		wxOK | wxICON_ERROR | wxCENTER
	);
	errDlg.ShowModal();

	return false;
}

void EditPane::BindEvents()
{
	Bind(wxEVT_STC_MARGINCLICK, &EditPane::OnMarginClick, this, wxID_ANY);
}

/**
 * Code folding event handler
 * 
 * @param wxStyledTextEvent& event
 * @return void
 */
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

/**
 * Get the list of keywords for the selected language
 * 
 * @param string lang
 * @return JsonValue
 */
JsonValue EditPane::GetKeywordList(string lang)
{
	return lang_config->GetRoot()
		.get(lang, JsonValue())
		.get("keywords", JsonValue());
}

/**
 * Retrieve a setting from the current theme
 * 
 * @param string type
 * @param string key
 * @return JsonValue
 */
JsonValue EditPane::GetThemeValue(string type, string key)
{
	JsonValue value = this->current_theme
		.get(type, JsonValue())
		.get(key, JsonValue());

	return value;
}

wxColor EditPane::GetThemeColor(string type, string key)
{
	JsonValue color_value = this->GetThemeValue(type, key);

	if (color_value.isArray())
	{
		return wxColor(
			(unsigned char) color_value[0].asUInt(),
			(unsigned char) color_value[1].asUInt(),
			(unsigned char) color_value[2].asUInt()
		);
	}
	else
	{
		return wxColor("black");
	}
}

/**
 * Iterate through the theme settings and apply them
 *
 * @param JsonValue lexer_map - Maps token types to theme colors
 * @return void
 */
void EditPane::_ApplyTheme(JsonValue lexer_map)
{
	// Font setup
#ifdef __WXMAC__
	wxFont *defaultFont = wxFont::New(
		14,
		wxFONTFAMILY_MODERN,
		wxFONTFLAG_ANTIALIASED
	);
#else
	wxFont *defaultFont = wxFont::New(
		10,
		wxFONTFAMILY_MODERN,
		wxFONTFLAG_ANTIALIASED
	);
#endif

	static const wxColor default_background = this->GetThemeColor("background", "default");
	static const wxColor default_foreground = this->GetThemeColor("foreground", "default");
	wxColor line_number_background = ( ! this->GetThemeValue("line_numbers", "background").isNull())
		? (this->GetThemeColor("line_numbers", "background"))
		: default_background;

	wxColor line_number_foreground = ( ! this->GetThemeValue("line_numbers", "foreground").isNull())
		? (this->GetThemeColor("line_numbers", "foreground"))
		: default_foreground;

	// Set default colors/ fonts
	for(int i = 0; i <= wxSTC_STYLE_MAX; i++)
	{
		this->StyleSetBackground(i, default_background);
		this->StyleSetForeground(i, default_foreground);
		this->StyleSetFont(i, *defaultFont);
	}

	this->StyleSetForeground (wxSTC_STYLE_DEFAULT, default_foreground);
	this->StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColor(147, 161, 161));

	this->SetMarginWidth (MARGIN_LINE_NUMBERS, TextWidth(wxSTC_STYLE_LINENUMBER, _T("_9999")));
	this->StyleSetForeground (wxSTC_STYLE_LINENUMBER, line_number_foreground);
	this->StyleSetBackground (wxSTC_STYLE_LINENUMBER, line_number_background);
	this->SetMarginType (MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

	for (int i = 0; i < lexer_map.size(); i++)
	{
		string key = lexer_map[i].asString();

		// Set the foreground color, if it exists
		if ( ! this->GetThemeValue("foreground", key).isNull())
		{
			this->StyleSetForeground(i, this->GetThemeColor("foreground", key));
		}

		// Set the background color, if it exists
		if ( ! this->GetThemeValue("background", key).isNull())
		{
			this->StyleSetBackground(i, this->GetThemeColor("background", key));
		}

		// Set bold, if it applies
		if (this->GetThemeValue("bold", key).isBool())
		{
			this->StyleSetBold(i, this->GetThemeValue("bold", key).asBool());
		}
		
		// Italic
		if (this->GetThemeValue("italic", key).isBool())
		{
			this->StyleSetItalic(i, this->GetThemeValue("italic", key).asBool());
		}
	}
}

void EditPane::SetTheme(string theme_name)
{
	JsonValue theme_list = this->theme_config->GetRoot();

	this->current_theme = theme_list.get(theme_name, JsonValue());
}

/**
 * The editor widget
 */

#include "src/widgets/EditPane.h"

extern StringConstMap Glob_lexer_map;
extern wxConfig *Glob_config;

/**
 * Constructor
 * 
 * @param wxWindow* parent
 * @param wxWindowID id
 * @param const wxPoint& pos
 * @param const wxSize& size
 */
EditPane::EditPane(
	wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size
) : wxStyledTextCtrl (parent, id, pos, size, wxBORDER_NONE)
{
	lang_config = new LangConfig();
	theme_config = new ThemeConfig();

	this->BindEvents();
	
	// Some basic properties to set
	this->SetStyleBits(8);
	this->SetScrollWidthTracking(true);// Set scroll width by longest line
	this->SetProperty("technology", "2");
	this->SetProperty("error.inline", "0");
	this->SetProperty("styling.within.preprocessor", "1");
	this->SetProperty("lexer.cpp.track.preprocessor", "1");
	this->SetProperty("font.quality", "3"); // LCD Optimized
		
	//this->SetLayoutCache (wxSTC_CACHE_DOCUMENT);

	// set spaces and indention
	this->SetTabWidth(4);
	
	// Blank highlight
	this->Highlight("");
}

/**
 * Destructor
 */
EditPane::~EditPane()
{
	wxLogDebug("Called EditPane Destructor");
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

	// Get the configuration name for the selected language
	string lang = lang_config->GetLangByFile(this->fileName);

	// Apply the theme
	this->ApplyTheme(lang);

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
	this->StyleClearAll();

	if (Glob_lexer_map.count(lang) > 0)
	{
		this->SetLexer(Glob_lexer_map[lang]);
	}
	else
	{
		this->SetLexer(wxSTC_LEX_NULL);
	}
	
	if (theme != "")
	{
		theme_config->SetTheme(theme);
	}

	// Get the keywords and mapping for the selected language
	JsonValue lexer_map = lang_config->GetLexerMap(lang);
	JsonValue keywords_array = lang_config->GetKeywordList(lang);
	
	if (keywords_array.isArray())
	{
		for(unsigned int i = 0; i < keywords_array.size(); i++)
		{
			this->SetKeyWords(i, keywords_array[i].asString());
		}
	}
	else
	{
		if (lang != "")
		{
			string typeMap[] = {"null", "int", "unsigned int", "double", "string", "boolean", "array", "object"};
			stringstream output;

			output << "current lang is:" << lang << endl;
			output << "keywords array is not an array" << endl;
			output << "keyword array is a " << typeMap[keywords_array.type()] << endl;

			wxLogDebug(output.str().c_str());
		}
	}

	// Do the appropriate mappings to load the selected theme
	this->_ApplyTheme(lexer_map);
}

/**
 * Re-style the control based on changed preferences
 * 
 * @param string [theme]
 * @return void
 */
void EditPane::ReApplyTheme(string theme)
{
	this->ApplyTheme(lang_config->GetLangByName(this->GetCurrentLang()), theme);
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
 * Save the current file
 * 
 * @return bool
 */
bool EditPane::SaveFile()
{
	wxString fname = this->fileName.GetFullPath();

	const wxString cfname(fname);

	return this->SaveFile(cfname);
}

/**
 * Save the current file with the specified filename
 * 
 * @param const wxString filename
 * @return bool
 */
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

/**
 * Wire Event handlers
 * 
 * @return void
 */
void EditPane::BindEvents()
{
	// Code folding event handler
	this->Bind(wxEVT_STC_MARGINCLICK, [=](wxStyledTextEvent& event) {
		if (event.GetMargin() == MARGIN_FOLD) 
		{
			int lineClick = this->LineFromPosition (event.GetPosition());
			int levelClick = this->GetFoldLevel (lineClick);
			if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) {
				this->ToggleFold (lineClick);
			}
		}
	}, wxID_ANY);

	this->Bind(wxEVT_STC_CHARADDED, &EditPane::OnCharAdded, this, wxID_ANY);
}

/**
 * Look at characters added to help with indentation
 * 
 * @param wxStyledTextEvent& event
 * @return void
 */
void EditPane::OnCharAdded(wxStyledTextEvent& event)
{
	char chr = (char) event.GetKey();
	int currentLine = this->GetCurrentLine();
	
	if (chr == '\n')
	{
		int lineInd = 0;
		if (currentLine > 0)
		{
			lineInd = this->GetLineIndentation(currentLine - 1);
		}
		if (lineInd == 0) return;
		
		this->SetLineIndentation(currentLine, lineInd);
		this->GotoPos(this->PositionFromLine(currentLine) + (lineInd / 4));
	}
}

/**
 * Iterate through the theme settings and apply them
 *
 * @param JsonValue lexer_map - Maps token types to theme colors
 * @param int addtoi - Offset for some languages
 * @return void
 */
void EditPane::_ApplyTheme(JsonValue &lexer_map)
{
	// Font setup
	wxFont *defaultFont = wxFont::New(
		TYRO_DEFAULT_FONT_SIZE,
		wxFONTFAMILY_MODERN,
		wxFONTFLAG_ANTIALIASED
	);

	static const wxColor default_background = theme_config->GetThemeColor("background", "default");
	static const wxColor default_foreground = theme_config->GetThemeColor("foreground", "default");
	wxColor line_number_background = ( ! theme_config->GetThemeValue("line_numbers", "background").isNull())
		? (theme_config->GetThemeColor("line_numbers", "background"))
		: default_background;

	wxColor line_number_foreground = ( ! theme_config->GetThemeValue("line_numbers", "foreground").isNull())
		? (theme_config->GetThemeColor("line_numbers", "foreground"))
		: default_foreground;

	// Set default colors/ fonts
	for(int i = 0; i <= wxSTC_STYLE_MAX; i++)
	{
		this->StyleSetBackground(i, default_background);
		this->StyleSetForeground(i, default_foreground);
		
		if ( ! HAS_FONT_BUG())
		{
			wxFont globalFont;
			wxString fontFace;
			if ( ! Glob_config->Read("global_font", &globalFont))
			{
				this->StyleSetFont(i, *defaultFont);
			}
			else
			{
				this->StyleSetFont(i, globalFont);
			}
		}
		else
		{
			this->StyleSetFont(i, *defaultFont);
		}
	}
	
	// Set up Code folding
	if (Glob_config->ReadBool("show_code_folding", false))
	{
		this->SetProperty("fold", "1");
		this->SetProperty("fold.comment", "1");
		this->SetProperty("fold.compact", "1");
		this->SetProperty("fold.html", "1");
		this->SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);
		
		this->SetMarginType(MARGIN_FOLD, wxSTC_MARGIN_SYMBOL);
		this->SetMarginWidth(MARGIN_FOLD, 16);
		this->SetMarginSensitive(MARGIN_FOLD, true);
		this->SetMarginMask(MARGIN_FOLD, wxSTC_MASK_FOLDERS);
		
		this->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUSCONNECTED, "WHITE", "BLACK");
		this->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUSCONNECTED, "WHITE", "BLACK");
		this->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE,     "BLACK", "BLACK");
		this->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_CIRCLEPLUSCONNECTED,  "WHITE", "BLACK");
		this->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_CIRCLEMINUSCONNECTED,  "WHITE", "BLACK");
		this->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER,     "BLACK", "BLACK");
		this->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER,     "BLACK", "BLACK");
	}
	else
	{
		this->SetProperty("fold", "0");
		this->SetProperty("fold.comment", "0");
		this->SetProperty("fold.compact", "0");
		this->SetProperty("fold.html", "0");
		this->SetMarginWidth(MARGIN_FOLD, 0);
	}
	
	// Setup indent guides
	if (Glob_config->ReadBool("show_indent_guides", false))
	{
		this->StyleSetForeground(wxSTC_STYLE_DEFAULT, default_foreground);
		this->StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, default_foreground);
		this->SetIndentationGuides(1);
	}
	else
	{
		this->SetIndentationGuides(0);
	}

	// Setup line numbers
	if (Glob_config->ReadBool("show_line_numbers", true))
	{
		this->SetMarginWidth (MARGIN_LINE_NUMBERS, TextWidth(wxSTC_STYLE_LINENUMBER, "_999"));
		this->StyleSetForeground (wxSTC_STYLE_LINENUMBER, line_number_foreground);
		this->StyleSetBackground (wxSTC_STYLE_LINENUMBER, line_number_background);
		this->SetMarginType (MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);
	}
	else
	{
		this->SetMarginWidth (MARGIN_LINE_NUMBERS, 0);
	}
	
	int max = lexer_map.size();
	for (int i = 0; i < max; i++)
	{
		string key = lexer_map[i].asString();

		// Set the foreground color, if it exists
		if ( ! theme_config->GetThemeValue("foreground", key).isNull())
		{
			this->StyleSetForeground(i, theme_config->GetThemeColor("foreground", key));
		}

		// Set the background color, if it exists
		if ( ! theme_config->GetThemeValue("background", key).isNull())
		{
			this->StyleSetBackground(i, theme_config->GetThemeColor("background", key));
		}

		// Set bold, if it applies
		if (theme_config->GetThemeValue("bold", key).isBool())
		{
			this->StyleSetBold(i, theme_config->GetThemeValue("bold", key).asBool());
		}
		
		// Italic
		if (theme_config->GetThemeValue("italic", key).isBool())
		{
			this->StyleSetItalic(i, theme_config->GetThemeValue("italic", key).asBool());
		}
		
		// Underline
		if (theme_config->GetThemeValue("underline", key).isBool())
		{
			this->StyleSetUnderline(i, theme_config->GetThemeValue("underline", key).asBool());
		}
	}
}

/**
 * Get the display name of the currently selected 
 * language used for highlighting
 * 
 * @return string
 */
string EditPane::GetCurrentLang()
{
	return lang_config->GetCurrentLangName();
}

/**
 * Set the highlighting language
 * 
 * @param string name
 * @return void
 */
void EditPane::SetCurrentLang(string name)
{
	// Update the current lang in the config
	string langKey = lang_config->GetLangByName(name);
	lang_config->SetLang(langKey);
	
	// Re-highlight the page with the new langauge
	this->ApplyTheme(langKey);
}

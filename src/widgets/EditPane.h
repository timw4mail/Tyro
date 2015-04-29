#ifndef TYROEDIT_PANE_H
#define TYROEDIT_PANE_H

#include "../wx_common.h"
#include "../base/settings/Config.h"
#include "../settings/LangConfig.h"

#include <wx/stc/stc.h>

class EditPane: public wxStyledTextCtrl
{
public:
	EditPane(
		wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxPoint &post = wxDefaultPosition,
		const wxSize &size = wxDefaultSize
	);
	~EditPane();
	wxFileName fileName;
	bool Load(wxString filePath);
	void Highlight(wxString filePath);
	bool SaveFile();
	bool SaveFile(const wxString &filename);
	void ApplyTheme(string lang, string theme="Solarized");
private:
	StringConstMap lexerMap;
	StringConstMap::iterator lexerMapIt;
	LangConfig *lang_config;
	TyroConfig *theme_config;
	JsonValue current_theme;
	enum myMargins
	{
		MARGIN_FOLD,
		MARGIN_SYMBOL,
		MARGIN_LINE_NUMBERS
	};
	bool FileReadable();
	bool FileWritable();
	void BindEvents();
	void OnMarginClick(wxStyledTextEvent &event);
	void OnCharAdded(wxStyledTextEvent &event);
	void SetTheme(string theme_name);
	JsonValue GetThemeValue(string type, string key);
	wxColor GetThemeColor(string type, string key);
	void _ApplyTheme(JsonValue &lexer_map);
};

#endif // TYRODOC_FRAME_H

#ifndef TYROEDIT_PANE_H
#define TYROEDIT_PANE_H

#include "../settings/LangConfig.h"
#include "../settings/ThemeConfig.h"

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
	void ReApplyTheme(string theme="");
	void ApplyTheme(string lang, string theme="");
	string GetCurrentLang();
	void SetCurrentLang(string name);
private:
	StringConstMap::iterator lexerMapIt;
	LangConfig *lang_config = nullptr;
	ThemeConfig *theme_config = nullptr;
	bool FileReadable();
	bool FileWritable();
	void BindEvents();
	void OnCharAdded(wxStyledTextEvent &event);
	void SetTheme(string theme_name);
	void _ApplyTheme(JsonValue &lexer_map);
};

#endif // TYRODOC_FRAME_H

#pragma once
#include "src/widgets/widget.h"
#include "src/settings/LangConfig.h"
#include "src/settings/ThemeConfig.h"


class EditPane: public wxStyledTextCtrl
{
public:
	explicit EditPane(
		wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxPoint &post = wxDefaultPosition,
		const wxSize &size = wxDefaultSize
	);
	~EditPane();
	wxFileName fileName;
	bool Load(const wxString &filePath);
	void Highlight(const wxString &filePath);
	bool SaveFile();
	bool SaveFile(const wxString &filename);
	void ReApplyTheme(const string &theme="");
	void ApplyTheme(const string &lang, const string &theme="");
	string GetCurrentLang();
	void SetCurrentLang(const string &name);
protected:
	StringConstMap::iterator lexerMapIt;
	LangConfig *lang_config = nullptr;
	ThemeConfig *theme_config = nullptr;
	bool FileReadable();
	bool FileWritable();
	void BindEvents();
	void _ApplyTheme(JsonValue &lexer_map);
};

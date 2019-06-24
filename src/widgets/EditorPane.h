#pragma once
#include "src/widgets/widget.h"
#include "src/settings/LangConfig.h"
#include "src/settings/ThemeConfig.h"


class EditorPane: public wxStyledTextCtrl
{
public:
	explicit EditorPane(
		wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxPoint &post = wxDefaultPosition,
		const wxSize &size = wxDefaultSize
	);
	~EditorPane();
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
	bool FileReadable();
	bool FileWritable();
	void BindEvents();
	void _ApplyTheme(Json::Value &lexer_map);
};

#pragma once

#include "src/wx_common.h"
#include "src/settings/Config.h"

class LangConfig : TyroConfig {
public:
	LangConfig();
	~LangConfig();
	void SetLang(const string &lang);
	string GetLang();
	string GetLangByFile(wxFileName &fileName);
	JsonValue GetKeywordList(string lang="none");
	JsonValue GetLexerMap(string lang="none");
	StringMap GetLangList();
	string GetCurrentLangName();
	string GetLangByName(const string &name);
private:
	string language;
	StringMap reverseMap;
};

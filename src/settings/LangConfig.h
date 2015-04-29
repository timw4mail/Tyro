#ifndef TYRO_LANG_CONFIG_H
#define TYRO_LANG_CONFIG_H

#include "../wx_common.h"
#include "../base/settings/Config.h"

class LangConfig : TyroConfig {
public:
	LangConfig();
	~LangConfig();
	void SetLang(string lang);
	string GetLang();
	string GetLangByFile(wxFileName &fileName);
	JsonValue GetKeywordList(string lang="none");
	JsonValue GetLexerMap(string lang="none");
private:
	string lang;
};

#endif

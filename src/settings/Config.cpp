/**
 * Object to control settings reading/writing
 */

#include "Config.h"

TyroConfig::TyroConfig()
{
	string raw_json;
	JSON_INCLUDE("../../config/scintilla.json", raw_json);
	
	cout << raw_json << endl;
	
	reader.parse(raw_json, default_root);
}

TyroConfig::~TyroConfig()
{
	
}

JsonValue TyroConfig::GetRoot()
{
	return default_root;
}

JsonValue TyroConfig::GetLang(string name)
{
	JsonValue root = this->GetRoot();
	JsonValue lang = root.get("languages", "");
	
	if (lang != "")
	{
		return lang.get(name, "");
	}
	
	return JsonValue("");
}
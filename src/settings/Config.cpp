/**
 * Object to control settings reading/writing
 */

#include "Config.h"

TyroConfig::TyroConfig()
{
	// Defines languages_json
	#include "../../config/languages_json.h"
	string json_string(languages_json);
	
	if ( ! reader.parse(json_string, default_root))
	{
		cerr << reader.getFormattedErrorMessages() << endl;
	}
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
	return default_root.get(name, JsonValue());
}
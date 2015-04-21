/**
 * Object to control settings reading/writing
 */

#include "Config.h"

TyroConfig::TyroConfig() {}
TyroConfig::~TyroConfig() {}

void TyroConfig::LoadJson(const char json[])
{
	string json_string(json);
	
	if ( ! reader.parse(json_string, default_root))
	{
		cerr << reader.getFormattedErrorMessages() << endl;
	}
}

JsonValue TyroConfig::GetRoot()
{
	return default_root;
}
/**
 * Object to control settings reading/writing
 */

#include "Config.h"

TyroConfig::TyroConfig() = default;
TyroConfig::~TyroConfig() = default;

void TyroConfig::LoadJson(const char json[])
{
	string json_string(json);
	
	if ( ! reader.parse(json_string, default_root))
	{
		cerr << reader.getFormattedErrorMessages() << endl;
	}
}

Json::Value TyroConfig::GetRoot()
{
	return default_root;
}

/**
 * Object to control settings reading/writing
 */

#include "Settings.h"

TyroSettings::TyroSettings()
{
	ifstream file("../../config/scintilla.json");
	reader.parse(file, default_root);
}

TyroSettings::~TyroSettings()
{
	
}

Json::Value TyroSettings::GetRoot()
{
	return default_root;
}
/**
 * Theme manager
 */
#pragma once

#include "src/wx_common.h"
#include "src/settings/Config.h"

class ThemeConfig : TyroConfig {
public:
	ThemeConfig();
	~ThemeConfig();
	bool SetTheme(const string &theme_name);
	JsonValue GetTheme();
	JsonValue GetThemeValue(const string &type, const string &key);
	wxColor GetThemeColor(const string &type, const string &key);
private:
	JsonValue current_theme;
};


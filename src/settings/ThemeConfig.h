/**
 * Theme manager
 */

#ifndef TYRO_THEME_CONFIG_H
#define	TYRO_THEME_CONFIG_H

#include "src/wx_common.h"
#include "src/settings/Config.h"

class ThemeConfig : TyroConfig {
public:
	ThemeConfig();
	~ThemeConfig();
	bool SetTheme(string theme_name);
	JsonValue GetTheme();
	JsonValue GetThemeValue(string type, string key);
	wxColor GetThemeColor(string type, string key);
private:
	JsonValue current_theme;
};

#endif	/* TYRO_THEME_CONFIG_H */


/**
 * Theme manager
 */

#include "ThemeConfig.h"
#include <config/themes_json.h>

ThemeConfig::ThemeConfig()
{
	this->LoadJson(themes_json);
	this->SetTheme("Solarized");
}

ThemeConfig::~ThemeConfig()
{
	wxLogDebug("Called ThemeConfig Destructor");
}

void ThemeConfig::SetTheme(string theme_name)
{
	JsonValue theme_list = this->GetRoot();
	this->current_theme = theme_list.get(theme_name, JsonValue());
}

JsonValue ThemeConfig::GetTheme()
{
	return this->current_theme;
}

/**
 * Retrieve a setting from the current theme
 * 
 * @param string type
 * @param string key
 * @return JsonValue
 */
JsonValue ThemeConfig::GetThemeValue(string type, string key)
{
	JsonValue value = this->current_theme
		.get(type, JsonValue())
		.get(key, JsonValue());

	return value;
}

/**
 * Retrieve the configured color for the specified theme
 * @param type
 * @param key
 * @return 
 */
wxColor ThemeConfig::GetThemeColor(string type, string key)
{
	JsonValue color_value = this->GetThemeValue(type, key);

	if (color_value.isArray())
	{
		return wxColor(
			(unsigned char) color_value[0].asUInt(),
			(unsigned char) color_value[1].asUInt(),
			(unsigned char) color_value[2].asUInt()
		);
	}
	else
	{
		return wxColor("black");
	}
}
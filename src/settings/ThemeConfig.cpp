/**
 * Theme manager
 */

#include "ThemeConfig.h"
#include <config/themes_json.h>

/**
 * Constructor
 */ 
ThemeConfig::ThemeConfig()
{
	this->LoadJson(themes_json);
	this->SetTheme("Solarized");
}

/**
 * Destructor
 */ 
ThemeConfig::~ThemeConfig()
{
	wxLogDebug("ThemeConfig Destructor Called.");
}

/**
 * Set the current theme
 * 
 * @param string theme_name
 * @return bool
 */ 
bool ThemeConfig::SetTheme(const string &theme_name)
{
	Json::Value theme_list = this->GetRoot();
	Json::Value selected_theme = theme_list.get(theme_name, Json::Value());
	
	if (selected_theme.isNull()) return FALSE;
	
	if (selected_theme.isObject())
	{
		this->current_theme = selected_theme;
		return TRUE;
	}

	return FALSE;
}

/**
 * Get the name of the currently selected theme
 *
 * @return string
 */ 
Json::Value ThemeConfig::GetTheme()
{
	return this->current_theme;
}

/**
 * Retrieve a setting from the current theme
 * 
 * @param string type
 * @param string key
 * @return Json::Value
 */
Json::Value ThemeConfig::GetThemeValue(const string &type, const string &key)
{
	Json::Value value = this->current_theme
		.get(type, Json::Value())
		.get(key, Json::Value());

	return value;
}

/**
 * Retrieve the configured color for the specified theme
 * @param type
 * @param key
 * @return 
 */
wxColor ThemeConfig::GetThemeColor(const string &type, const string &key)
{
	Json::Value color_value = this->GetThemeValue(type, key);

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
		return wxColor("BLACK");
	}
}

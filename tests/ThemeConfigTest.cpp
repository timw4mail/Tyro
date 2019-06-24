#include "tests/catch.hpp"
#include "src/settings/ThemeConfig.h"

TEST_CASE("Theme Config Library")
{
	ThemeConfig *config = new ThemeConfig();
	
	SECTION("GetTheme()")
	{
		Json::Value theme = config->GetTheme();
		REQUIRE(theme.isObject());
	}
	
	SECTION("SetTheme()")
	{
		REQUIRE_FALSE(config->SetTheme("foobar"));
		REQUIRE(config->SetTheme("Solarized"));
	}
	
	SECTION("GetThemeColor()")
	{
		REQUIRE(config->SetTheme("Solarized"));
		
		// Bad color
		wxColor theme_color = config->GetThemeColor("foo", "bar");
		REQUIRE(wxColor("BLACK") == theme_color);
	}
}

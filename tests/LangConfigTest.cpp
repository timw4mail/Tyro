
#include "catch.hpp"
#include "../src/settings/LangConfig.h"

TEST_CASE("Language Config Library")
{
	LangConfig *config = new LangConfig();
	
	SECTION("GetLangList()")
	{
		StringMap list = config->GetLangList();
		REQUIRE(list.size() > 0);
	}
	
	SECTION("GetLang()")
	{
		string lang = config->GetLang();
		REQUIRE(lang == "");
	}
	
	SECTION("SetLang()")
	{
		string lang="cpp";
		config->SetLang(lang);
		REQUIRE(lang == config->GetLang());
	}
	
	SECTION("GetLangByFile()")
	{
		wxFileName file("foo.cpp");
		
		string lang = config->GetLangByFile(file);
		REQUIRE("cpp" == lang);
	}
	
	SECTION("GetLangByName()")
	{
		string lang = config->GetLangByName("C / C++");
		REQUIRE("cpp" == lang);
	}
	
	SECTION("GetCurrentLangName()")
	{
		
	}
	
	SECTION("GetLexerMap()")
	{
		
	}
	
	SECTION("GetKeywordList()")
	{
		
	}
}

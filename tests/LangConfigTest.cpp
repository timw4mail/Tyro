
#include "tests/catch.hpp"
#include "src/settings/LangConfig.h"

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
		config->SetLang("cpp");
		REQUIRE("C / C++" == config->GetCurrentLangName());
	}
	
	SECTION("GetLexerMap()")
	{
		JsonValue lexer_map = config->GetLexerMap("none");
		REQUIRE(lexer_map.isNull());
		
		lexer_map = config->GetLexerMap("cpp");
		REQUIRE(lexer_map.isArray());
	}
	
	SECTION("GetKeywordList()")
	{
		JsonValue keyword_list = config->GetKeywordList("none");
		REQUIRE(keyword_list.isNull());
		
		keyword_list = config->GetKeywordList("cpp");
		REQUIRE(keyword_list.isArray());
	}
}

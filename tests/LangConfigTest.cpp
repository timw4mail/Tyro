
#include <CppUTest/TestHarness.h>
#include "../src/settings/LangConfig.h"
TEST_GROUP(LangConfig)
{
	LangConfig *config;
	
	void setup()
	{
		config = new LangConfig();
	}
	void teardown()
	{
		delete config;
	}
};

TEST(LangConfig, GetLangList)
{	
	StringMap list = config->GetLangList();
	CHECK(list.size() > 0);
}

TEST(LangConfig, GetLang)
{
	string lang;
	lang = config->GetLang();
	CHECK_EQUAL(lang, "");
}

TEST(LangConfig, SetLang)
{
	string lang="cpp";
	config->SetLang(lang);
	
	CHECK_EQUAL(lang, config->GetLang());
}

TEST(LangConfig, GetLangByFile)
{
	wxFileName file("foo.cpp");
	
	string lang = config->GetLangByFile(file);
	CHECK_EQUAL("cpp", lang);
}

TEST(LangConfig, GetLangByName)
{
	string lang = config->GetLangByName("C / C++");
	CHECK_EQUAL("cpp", lang);
}
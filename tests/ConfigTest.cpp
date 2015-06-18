#include <CppUTest/TestHarness.h>
#include "../src/base/settings/Config.h"

TEST_GROUP(Config)
{
	TyroConfig *config;
	
	void setup()
	{
		config = new TyroConfig();
	}
	void teardown()
	{
		delete config;
	}
};

TEST(Config, Create)
{
	config->LoadJson("{\"foo\":\"bar\"}");
	
	CHECK(config->GetRoot().isObject());
}
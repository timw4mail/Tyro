#include "tests/catch.hpp"
#include "src/settings/Config.h"

TEST_CASE ("Base config class load json")
{
	TyroConfig *config = new TyroConfig();
	
	SECTION("Create Base Config Class")
	{
		config->LoadJson("{\"foo\":\"bar\"}");
		REQUIRE(config->GetRoot().isObject());
	}
}

#include "catch.hpp"
#include "../src/base/network/SFTP.h"

TEST_CASE("SFTP Library")
{
	SFTP *sftp = new SFTP("localhost", "user", "pass", "/", "22");
	
	SECTION("Sanity check")
	{
		REQUIRE(sftp != nullptr);
	}
}
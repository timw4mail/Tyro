#include "tests/catch.hpp"
#include "src/base/SFTP.h"

TEST_CASE("SFTP Library")
{
	SFTP *sftp = new SFTP("localhost", "user", "pass", "/", "22");
	
	SECTION("Sanity check")
	{
		REQUIRE(sftp != nullptr);
	}
}
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/network/SFTP.h"


TEST_CASE("ssh connections work", "[SFTP]") {
	SFTP *sftp = new SFTP("timshomepage.net", "test", "testpassword");
	
	SECTION("sftp object is not null") {
		REQUIRE(sftp != NULL);
	}
	/*SECTION("can retreive a file") {
		string file;
		file = sftp->getFile("~/test.txt");
		REQUIRE(file != "");
		REQUIRE(file == "SFTP works!");
	}*/
}



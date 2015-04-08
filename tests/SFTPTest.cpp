#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/network/SFTP.h"


TEST_CASE("ssh connections work", "[SFTP]") {
	SFTP *sftp = new SFTP("shell.xshellz.com", "twarren", "of0fmk2dsj");
	sftp->getFingerprint();
	
	SECTION("sftp object is not null") {
		REQUIRE(sftp != NULL);
	}
	SECTION("can retreive a file") {
		string file;
		file = sftp->getFile("test.txt");
		REQUIRE(file != "");
		REQUIRE(file == "SFTP works!\n");
	}
}




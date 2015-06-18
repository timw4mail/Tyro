#include <CppUTest/CommandLineTestRunner.h>

int main(int argc, char *argv[])
{
	MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
	return CommandLineTestRunner::RunAllTests(argc, argv);
}

IMPORT_TEST_GROUP(Config);
IMPORT_TEST_GROUP(LangConfig);
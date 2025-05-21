#include "alltests.h"

MunitSuite suite[] = {
	{"/generator",    generator_tests,    NULL, 1, MUNIT_SUITE_OPTION_NONE},
	{NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE},
};

static const MunitSuite main_suite = {
	"", NULL, suite, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char** argv) {
	return munit_suite_main(&main_suite, NULL, argc, argv);
}

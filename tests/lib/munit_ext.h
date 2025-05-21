#ifndef MUNIT_EXT_H
#define MUNIT_EXT_H
#include "munit.h"

#define munit_assert_file_exists(filename)\
	do {\
		struct stat buff;\
		if (stat(filename, &buff) == -1) {\
			munit_errorf("assertion failed: file '%s' does not exist", filename);\
		}\
    MUNIT_PUSH_DISABLE_MSVC_C4127_ \
	} while (0)
  MUNIT_POP_DISABLE_MSVC_C4127_


#endif

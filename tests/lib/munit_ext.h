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

#define munit_assert_replacement_contains(replacement, needle_, with_)\
	do {\
		int i = 0;\
		for (i = 0; i < replacement.replacements_count; i++) {\
			const char* r_needle = replacement.replacements[i].needle;\
			const char* r_with = replacement.replacements[i].with;\
			int needle_cmp = (needle_ == NULL && r_needle == NULL) || (needle_ && r_needle && strcmp(needle_, r_needle) == 0);\
			int with_cmp = (with_ == NULL && r_with == NULL) || (with_ && r_with && strcmp(with_, r_with) == 0);\
			if (needle_cmp && with_cmp) {\
				break;\
			}\
		}\
		if (i == replacement.replacements_count) {\
			munit_errorf("assertion failed: does not contain { needle: %s, with: %s }", (char*)needle_, (char*)with_);\
		}\
    MUNIT_PUSH_DISABLE_MSVC_C4127_ \
	} while (0)
  MUNIT_POP_DISABLE_MSVC_C4127_

#endif

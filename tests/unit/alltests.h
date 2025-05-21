#ifndef ALL_TESTS_H
#define ALL_TESTS_H
#include "../lib/munit_ext.h"
#include "../lib/munit.h"
#include "../../gengen.h"

/* ===== SETTINGS ===== */
static MunitResult test_settings_custom_empty_paths(const MunitParameter params[], void *userdata);
static MunitResult test_settings_custom_with_single_path(const MunitParameter params[], void *userdata);
static MunitResult test_settings_custom_with_multiple_paths(const MunitParameter params[], void *userdata);
extern MunitTest settings_tests[];

/* ===== REPLACEMENT ===== */
static MunitResult test_replacement_create(const MunitParameter params[], void *userdata);
static MunitResult test_replacement_add(const MunitParameter params[], void *userdata);
static MunitResult test_replacement_forward(const MunitParameter params[], void *userdata);
static MunitResult test_replacement_get(const MunitParameter params[], void *userdata);
extern MunitTest replacement_tests[];

/* ===== FORWARD TABLE ===== */
static MunitResult test_forward_table_create(const MunitParameter params[], void *userdata);
static MunitResult test_forward_table_forward(const MunitParameter params[], void *userdata);

extern MunitTest forward_table_tests[];

#endif

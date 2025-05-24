# ====================================
# Define handy reusable constants
# ====================================
UNIT_TEST_SOURCES 			   := $(wildcard unit/*.c)
INTEGRATION_TEST_SOURCES   := $(wildcard integration/*.c)

TEST_OUTPUT_DIR 				   := out
INTEGRATION_GEN_OUTPUT_DIR := integration/output

# ====================================
# Define main phony build targets
# ====================================
.PHONY: build_all_tests build_unit_tests build_integration_tests
build_all_tests: build_unit_tests build_integration_tests
build_unit_tests: always $(TEST_OUTPUT_DIR)/unit_tests_main
build_integration_tests: always $(TEST_OUTPUT_DIR)/integration_tests_main

# ====================================
# Define main phony test runner targets
# ====================================
.PHONY: run_all_tests run_unit_tests run_integration_tests
run_all_tests: build_all_tests run_unit_tests run_integration_tests
run_unit_tests:
	./$(TEST_OUTPUT_DIR)/unit_tests_main
run_integration_tests:
	cd integration; ../$(TEST_OUTPUT_DIR)/integration_tests_main

# ====================================
# Define boilerplate nice to have targets
# ====================================
.PHONY: always clean
always:
	mkdir -p $(TEST_OUTPUT_DIR)
	mkdir -p $(INTEGRATION_GEN_OUTPUT_DIR)

clean:
	rm -r $(TEST_OUTPUT_DIR)
	rm -r $(INTEGRATION_GEN_OUTPUT_DIR)

# ====================================
# Define main build targets
# 	- unit_tests
# 	- integration_tets
# ====================================
$(TEST_OUTPUT_DIR)/unit_tests_main: $(UNIT_TEST_SOURCES)
	gcc $^ lib/munit.c lib/util.c -ggdb -fsanitize=address -o $@

$(TEST_OUTPUT_DIR)/integration_tests_main: $(INTEGRATION_TEST_SOURCES)
	gcc $^ lib/munit.c lib/util.c -ggdb -fsanitize=address -o $@

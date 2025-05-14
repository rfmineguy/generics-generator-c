#ifndef GENGEN_H
#define GENGEN_H
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

typedef struct {
	size_t count;
	const char* data;
} string_view;

#define assert(cond, teardown)\
	if (!(cond)) {\
		teardown\
		exit(1);\
	}

typedef struct {
	const char *infilename, *outfilename_fmt;
} template_file;

typedef struct ctemplate {
	const char* template_name;

	template_file* template_files;
	int template_files_count, template_files_cap;

	struct ctemplate* dep;
	int dep_count, dep_cap;
} ctemplate;
#define template_create(name) (ctemplate) {.template_name = name, .template_files = NULL, .template_files_count = 0, .template_files_cap = 10}

typedef struct {
	const char *needle, *with;
} replacement_item;

typedef struct {
	replacement_item *replacements;
	int replacements_count, replacements_capacity;
} replacement;
#define replacement_create() (replacement){.replacements = NULL, .replacements_count = 0, .replacements_capacity = 10}

typedef struct str_ll_node {
	string_view data;
	struct str_ll_node *next, *prev;
} str_ll_node;

typedef struct str_ll {
	str_ll_node *head, *tail;
} str_ll;

typedef const char* path;
typedef struct generator_settings {
	path *search_paths;
	int path_count;
	const char* outdir;
} generator_settings;
#define settings_default() (generator_settings) {.search_path={"."}}
#define settings_custom(...) (generator_settings) {__VA_ARGS__}
#define paths(...) (path[]){__VA_ARGS__}, .path_count = (sizeof((path[]){__VA_ARGS__})/sizeof(path))

void   strll_pushback(str_ll*, string_view);
void 	 strll_free(str_ll*);

void   template_addfile(ctemplate*, const char*, const char*);
void 	 template_adddep(ctemplate*, ctemplate);
void   template_replacement(ctemplate*, const char*, const char*, const char*);

void    						replacement_add(replacement*, const char*, const char*);
replacement_item*   replacement_get(replacement*, const char*);

void 	 generator_run(generator_settings, ctemplate, replacement);

#endif

#define GENGEN_IMPLEMENTATION
#ifdef GENGEN_IMPLEMENTATION
#endif

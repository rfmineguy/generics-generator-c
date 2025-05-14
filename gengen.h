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
/*
 * Time Complexity
 * O(1) in all cases
 * @desc 			         Adds a template file to the given template, and describes the output fileformat
 * @param tplt         Pointer to the template being modified
 * @param templatepath Path to the template file in your project
 * @param templatefmt  The format of the output file's name
 */
void template_addfile(ctemplate* tplt, const char* templatepath, const char* templatefmt) {
	template_file file = (template_file){templatepath, templatefmt};
	if (tplt->template_files_count == 0) {
		tplt->template_files = (template_file*)calloc(tplt->template_files_cap, sizeof(ctemplate));
	}
	else if (tplt->template_files_count + 1 >= tplt->template_files_cap) {
		tplt->template_files = (template_file*)realloc(tplt->template_files, tplt->template_files_cap * 2);
		tplt->template_files_cap *= 2;
	}
	tplt->template_files[tplt->template_files_count++] = file;
}

/* 
 * @complexity 	   :   O(1) in all cases
 * @desc 			 	   : 	 Adds a dependent template to this template
 * @param tplt 	   : 	 Pointer to the template being modified
 * @param dep_tplt :   The template that this one depends on
 * @note 					 :   For future, not used at the moment
 */
void template_adddep(ctemplate* tplt, ctemplate dep_tplt) {
	if (tplt->dep_count == 0) {
		tplt->dep = (ctemplate*)calloc(tplt->dep_cap, sizeof(ctemplate));
	}
	if (tplt->dep_count + 1 >= tplt->dep_cap) {
		tplt->dep = (ctemplate*)realloc(tplt->dep, tplt->dep_cap * 2);
		tplt->dep_cap *= 2;
	}
	tplt->dep[tplt->dep_count++] = dep_tplt;
}

/*
 * @complexity 	   :   O(1) in all cases
 * @desc 			 	   : 	 Adds a dependent template to this template
 * @param tplt 	   : 	 Pointer to the template being modified
 * @param dep_tplt :   The template that this one depends on
 * @note 					 :   For future, not used at the moment
 */
void replacement_add(replacement* repl, const char* needle, const char* with) {
	if (repl->replacements_count == 0) {
		repl->replacements = (replacement_item*)calloc(repl->replacements_capacity, sizeof(replacement));
	}
	if (repl->replacements_count + 1 >= repl->replacements_capacity) {
		repl->replacements = (replacement_item*)realloc(repl->replacements, repl->replacements_capacity * 2);
		repl->replacements_capacity *= 2;
	}
	replacement_item rep = {.needle = needle, .with = with};
	repl->replacements[repl->replacements_count++] = rep;
}

/*
 * Time Complexity
 * O(n*m) where 
 * 			n is the number of replacements
 * 			m is the length of those replacements
 *
 * @desc          Looks for the replacement for a given cursor position
 * @param repl    The replacement context to search in
 * @param cursor  The cursor position to search from
 * @return 		 		NULL if no replacement exists for the cursor position
 * 								pointer to replacement item that is used for the cursor position
 */
replacement_item* replacement_get(replacement* repl, const char* cursor) {
	for (int i = 0; i < repl->replacements_count; i++) {
		replacement_item *item = &repl->replacements[i];
		size_t len = strlen(item->needle);
		if (strncmp(cursor, item->needle, len) == 0)
			return item;
	}
	return NULL;
}
#endif

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
	size_t template_files_count, template_files_cap;

	struct ctemplate* dep;
	size_t dep_count, dep_cap;
} ctemplate;
#define template_create(name) (ctemplate) {.template_name = name, .template_files = NULL, .template_files_count = 0, .template_files_cap = 10}

typedef struct {
	const char *needle, *with;
} replacement_item;

typedef struct {
	replacement_item *replacements;
	size_t replacements_count, replacements_capacity;
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
	size_t path_count;
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
ctemplate template_create();
void      template_free(ctemplate*);

void    						replacement_add(replacement*, const char*, const char*);
replacement_item*   replacement_get(replacement*, const char*);
replacement       replacement_create();
void              replacement_free(replacement*);

void 	 generator_run(generator_settings, ctemplate, replacement);

#endif

#define GENGEN_IMPLEMENTATION
#ifdef GENGEN_IMPLEMENTATION

/*
 * Time Complexity
 * O(1) in all cases
 * @desc 			  Creates and initializes a ctemplate for use later
 * */
ctemplate template_create() {
  return (ctemplate) {.template_name = "unused", .template_files = NULL, .template_files_count = 0, .template_files_cap = 10};
}

/*
 * Time Complexity
 * O(1) in all cases
 * @desc 			  Free the internal memory used by the template
 * @param tplt  Pointer to the template being freed
 */
void template_free(ctemplate* tplt) {
	free(tplt->template_files);
}

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

replacement replacement_create() {
	return (replacement){.replacements = NULL, .replacements_count = 0, .replacements_capacity = 10};
}

void replacement_free(replacement* repl) {
	free(repl->replacements);
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

char* read_file(const char* filepath) {
	FILE* f = fopen(filepath, "r");
	assert(f, {
		fprintf(stderr, "Failed to open '%s'\n", filepath);
	});
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* buf = (char*)malloc(size);
	assert(fread(buf, 1, size, f) == size, { 
		fclose(f);
		free(buf);
		fprintf(stderr, "Failed to read '%s'\n", filepath); 
	});

	fclose(f);
	return buf;
}

void generator_run(generator_settings settings, ctemplate ctemplate, replacement replacement_) {
	// 1. Read in the template file
	static char path[PATH_MAX];
	static char realpath_outpath[PATH_MAX];
	static char outfilepath[PATH_MAX];
	for (int i = 0; i < ctemplate.template_files_count; i++) {
		template_file tf = ctemplate.template_files[i];
		struct stat buffer;
		// Look in the search paths until its found
		int j = 0;
		for (j = 0; j < settings.path_count; j++) {
			path[0] = 0;
			const char* search_path = settings.search_paths[j];
			assert(realpath(search_path, path) != NULL, {
				continue;
			});
			strncat(path, "/", PATH_MAX);
			strncat(path, tf.infilename, PATH_MAX);
			assert(stat(path, &buffer) == 0, {
				// printf("File: '%s' does not exist here\n", path);
				continue;
			})
			printf("valid: %s\n", path);
			break;
		}
		if (j == settings.path_count) {
			fprintf(stderr, "No template file '%s'\n", tf.infilename);
			continue;
		}

		assert(realpath(settings.outdir, realpath_outpath) != NULL, {
			printf("Failed:: path = %s\n", path);
			continue;
		});

		// Do any replacement needed for the output filepath, given the
		//    set of replacements given in 'replacement_'
		strncpy(outfilepath, realpath_outpath, PATH_MAX);
		strncat(outfilepath, "/", PATH_MAX);
		const char* cursorfp = tf.outfilename_fmt;
		replacement_item* found = NULL;
		while (*cursorfp) {
			if ((found = replacement_get(&replacement_, cursorfp))) {
				strncat(outfilepath, found->with, PATH_MAX);
				cursorfp += strlen(found->needle);
			}
			else {
				strncat(outfilepath, cursorfp, 1);
				cursorfp++;
			}
		}

		printf("outfilepath: %s\n", outfilepath);
		FILE* outputfile = fopen(outfilepath, "w");
		if (!outputfile) {
			fprintf(stderr, "Couldn't create %s for reason: %s\n", outfilepath, strerror(errno));
			fclose(outputfile);
			continue;
		}

		const char* content = read_file(path);
		const char* cursor = content;

		while (*cursor) {
			if ((found = replacement_get(&replacement_, cursor))) {
				// printf("Found replacement for needle: %s\n", found->needle);
				printf("%s", found->with);
				cursor += strlen(found->needle);
				fprintf(outputfile, "%s", found->with);
			}
			else {
				printf("%c", *cursor);
				fprintf(outputfile, "%c", *cursor);
				cursor += 1;
			}
		}

		free((char*)content);
	}
}

#endif

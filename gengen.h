#ifndef GENGEN_H
#define GENGEN_H
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
	const char *symbol, *as;
} forward_item;
#define fwd(...) (forward_item) {__VA_ARGS__}

typedef struct {
	forward_item *fwd_items;
	size_t fwd_items_count, fwd_items_capacity;
} forward_table;

typedef struct {
	const char *needle, *with;
} replacement_item;

typedef struct {
	replacement_item *replacements;
	size_t replacements_count, replacements_capacity;
} replacement;

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

struct dependency;
typedef struct ctemplate {
	const char* template_name;

	template_file* template_files;
	size_t template_files_count, template_files_cap;

	replacement replacement;

	struct dependency* deps;
	size_t deps_count, deps_cap;
} ctemplate;

typedef struct dependency {
	forward_table fwd_table;
	ctemplate template_;
} dependency;

typedef const char* path;
typedef struct generator_settings {
	path *search_paths;
	size_t path_count;
	const char* outdir;
	bool verbose;
} generator_settings;
#define settings_default() (generator_settings) {.search_path={"."}}
#define settings_custom(...) (generator_settings) {__VA_ARGS__}
#define paths(...) (path[]){__VA_ARGS__}, .path_count = (sizeof((path[]){__VA_ARGS__})/sizeof(path))

/*
 * @desc 			  Creates and initializes a ctemplate for use later
 * @notes 			Must be cleaned up with template_free
 * @return 			The created template instance
 */
ctemplate template_create();

/*
 * @desc 			  Free the internal memory used by the template
 * @param tplt  Pointer to the template being freed
 */
void template_free(ctemplate*);

/*
 * @desc 			         Adds a template file to the given template, and describes the output fileformat
 * @param tplt         Pointer to the template being modified
 * @param templatepath Path to the template file in your project
 * @param templatefmt  The format of the output file's name
 */
void template_addfile(ctemplate*, const char*, const char*);

/*
 * @desc 						Adds a new replacement instance to the template
 * @param tplt 			Pointer to the template being modified
 * @param symbol 		The symbol that should be replaced
 * @param with 			What should the symbol replaced with?
 */
void template_addreplacement(ctemplate*, const char*, const char*);

/* 
 * @desc 			 	     Adds a dependent template to this template
 * @param tplt 	     Pointer to the template being modified
 * @param dep_tplt   The template that this one depends on
 */
void template_adddep(ctemplate*, ctemplate, forward_table);

/*
 * @desc 			  Creates and initializes a replacement for use later
 * @return 			The created replacement instance
 * @notes 			Must be cleaned up with replacement_free
 */
replacement replacement_create();

/*
 * @desc 				Frees the supplied replacement instance
 * @notes 			'repl' should have been created via replacement_create
 */
void replacement_free(replacement*);

/*
 * @desc 				Prints out this replacement
 * @param repl  Pointer to a replacement context
 * @notes 			'repl' should have been created via replacement_create
 */
void replacement_print(const replacement* repl);

/*
 * @desc 				Forwards the replacement from one replacement instance to another
 * 								while maintaining the original replacements
 * @return 			A new replacement instance 
 * @notes 			Returned instance must be freed with replacement_free
 */
replacement replacement_forward(replacement tplt, replacement dep_tplt, forward_table with); 

/*
 * @desc 					Adds a replacement item to this replacement context
 * @param repl 		Pointer to a replacement context
 * @param needle  The string to search for
 * @param with    The string to replace needle with
 * @notes 			  'repl' should have been created via replacement_create
 */
void replacement_add(replacement* repl, const char* needle, const char* with);

/*
 * @desc          Looks for the replacement for a given cursor position
 * @param repl    The replacement context to search in
 * @param cursor  The cursor position to search from
 * @return 		 		NULL if no replacement exists for the cursor position
 * 								pointer to replacement item that is used for the cursor position
 * @notes 				'repl' should have been created via 'replacement_create'
 */
replacement_item* replacement_get(replacement* repl, const char* cursor);

/*
 * @desc          Prints out the replacement data to stdout
 * @return 				A new forward table
 * @notes 				Returned table must be freed via 'forward_table_free'
 */
forward_table forward_table_create();

/*
 * @desc        		 Prints out the replacement data to stdout
 * @param fwd_table  A valid forward table pointer 
 * @notes 			     'fwd_table' should have been created via 'forward_table_create'
 */
void forward_table_free(forward_table* fwd_table);

/*
 * @desc        		 Adds a new forward rule to the forward table
 * @param fwd_table  A valid forward table pointer 
 * @param fwd_item   A valid forward item ('fwd' macro can be used for convience)
 * @notes 			     'fwd_table' should have been created via 'forward_table_create'
 */
void forward_table_forward(forward_table* fwd_table, forward_item fwd_item);

/*
 * @desc             Produces the final template given replacements and settings
 * @param settings   Provides a set of configuration options that influence how 
 *                      the generator functions
 *
 *                   .search_paths   - specify where the generator should look for templates
 *                   .outdir      - specify where the generator should put output templates
 *
 *                   @see settings_custom(...)
 *                   @see settings_default()
 * @param tplt       Specify what template we are generating
 * @param repl       Specify what replacement set we should use for generating
 * @notes            'tplt' should have been created via 'template_create'
 * @notes            'repl' should have been created via 'replacement_create'
 */
void generator_run(generator_settings settings, ctemplate tplt, replacement repl);

#endif

// #define GENGEN_IMPLEMENTATION
#ifdef GENGEN_IMPLEMENTATION

ctemplate template_create() {
  return (ctemplate) {
		.template_name = "unused",
		.template_files = NULL,
		.template_files_count = 0,
		.template_files_cap = 10,
		.deps_count = 0,
		.deps_cap = 10,
		.replacement = replacement_create()};
}

void template_free(ctemplate* tplt) {
	free(tplt->template_files);
}

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

void template_addreplacement(ctemplate *tplt, const char *symbol, const char *with) {
	replacement_add(&tplt->replacement, symbol, with);
}

void template_adddep(ctemplate* tplt, ctemplate dep_tplt, forward_table fwd_table) {
	if (tplt->deps_count == 0) {
		tplt->deps = (dependency*)calloc(tplt->deps_cap, sizeof(dependency));
	}
	if (tplt->deps_count + 1 >= tplt->deps_cap) {
		tplt->deps = (dependency*)realloc(tplt->deps, tplt->deps_cap * 2 * sizeof(dependency));
		tplt->deps_cap *= 2;
	}
	tplt->deps[tplt->deps_count++] = (dependency){.fwd_table = fwd_table, .template_ = dep_tplt};
}

replacement replacement_create() {
	return (replacement){.replacements = NULL, .replacements_count = 0, .replacements_capacity = 10};
}

void replacement_free(replacement* repl) {
	free(repl->replacements);
}

/*
 * forward replacements from 'from' to 'to' based on the 'with' forward table
 * Example:
 * from (replacement):
 * 	'$T' -> 'int'
 *  '^T' -> 'age'
 * to (replacment):
 * 	'@T' -> NULL
 * 	'&T' -> NULL
 * 	'PRINT' -> 'printf'
 * 	'FREE'  -> 'free'
 * with:
 * 	.symbol='$T', .as='@T'
 * 	.symbol='^T', .as='&T'
 *
 * expected:
 *  '@T' -> 'int'
 *  '^T' -> 'age'
 */
replacement replacement_forward(replacement to, replacement from, forward_table with) {
	replacement r = replacement_create();
	for (int i = 0; i < with.fwd_items_count; i++) {
		int j = 0;
		// 1. find replacement in 'to' whose needle is equal to fwd.as
		forward_item fwd = with.fwd_items[i];
		for (j = 0; j < to.replacements_count; j++) {
			if (to.replacements[j].needle == fwd.as) {
				break;
			}
		}

		// if j is bigger than the replacement count we didnt find one
		//    j represents the index of the replacement in 'to' we are forwarding to
		if (j >= to.replacements_count) continue;

		int k = 0;
		// 2. find replacement in 'from' whose needle is equal to fwd.symbol
		for (k = 0; k < from.replacements_count; k++) {
			if (from.replacements[k].needle == fwd.symbol) {
				break;
			}
		}

		// if k is bigger than the replacement count we didnt find one
		//    k represents the index of the replacement in 'from' we are forwarding from
		if (k >= from.replacements_count) continue;

		// at this point 'j' and 'k' should have valid indices
		// we need to set the .needle of to.replacments[j] equal to the .with of from.replacements[k]
		replacement_add(&r, to.replacements[j].needle, from.replacements[k].with);
	}

	// when we get here, 'r' should contain all of the symbols from the forward table
	// now we need add back in the symbols that already had valid replacements
	for (int i = 0; i < to.replacements_count; i++) {
		// 1. determine if this replacement is NULL. if not we simply add it into the new repalcement
		if (to.replacements[i].with != NULL) {
			replacement_add(&r, to.replacements[i].needle, to.replacements[i].with);
		}
	}

	return r;
}
void replacement_add(replacement* repl, const char* needle, const char* with) {
	if (repl->replacements_count == 0) {
		repl->replacements = (replacement_item*)calloc(repl->replacements_capacity, sizeof(replacement));
	}
	if (repl->replacements_count + 1 >= repl->replacements_capacity) {
		repl->replacements = (replacement_item*)realloc(repl->replacements, sizeof(replacement) * repl->replacements_capacity * 2);
		repl->replacements_capacity *= 2;
	}
	replacement_item rep = {.needle = needle, .with = with};
	repl->replacements[repl->replacements_count++] = rep;
}

replacement_item* replacement_get(replacement* repl, const char* cursor) {
	for (int i = 0; i < repl->replacements_count; i++) {
		replacement_item *item = &repl->replacements[i];
		size_t len = strlen(item->needle);
		if (strncmp(cursor, item->needle, len) == 0)
			return item;
	}
	return NULL;
}

/*
 * Time Complexity
 * O(n) in all cases
 * @desc          Prints out the replacement data to stdout
 * @param repl    The replacement context to search in
 * @notes 				'repl' should have been created via 'replacement_create'
 */
void replacement_print(replacement* repl) {
	printf("Replacement {\n");
	for (int i = 0; i < repl->replacements_count; i++) {
		printf("\t{needle: %s, with: %s}\n", repl->replacements[i].needle, repl->replacements[i].with);
	}
	printf("}\n");
}

forward_table forward_table_create() {
	return (forward_table){.fwd_items = NULL, .fwd_items_count = 0, .fwd_items_capacity = 10};
}

void forward_table_free(forward_table* fwd_table) {
	free(fwd_table->fwd_items);
}

void forward_table_forward(forward_table* fwd_table, forward_item fwd_item) {
	if (fwd_table->fwd_items_count == 0) {
		fwd_table->fwd_items = (forward_item*)calloc(fwd_table->fwd_items_capacity, sizeof(forward_item));
	}
	if (fwd_table->fwd_items_count + 1 >= fwd_table->fwd_items_capacity) {
		fwd_table->fwd_items = (forward_item*)realloc(fwd_table->fwd_items, fwd_table->fwd_items_capacity * 2);
		fwd_table->fwd_items_capacity *= 2;
	}
	fwd_table->fwd_items[fwd_table->fwd_items_count++] = fwd_item;
}

/* Private function */
static char* read_file(const char* filepath) {
	FILE* f = fopen(filepath, "r");
	assert(f, {
		fprintf(stderr, "Failed to open '%s'\n", filepath);
	});
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	assert(size != -1, {
		fprintf(stderr, "Failed to ftell file '%s'. Reason: %s\n", filepath, strerror(errno));
	})
	fseek(f, 0, SEEK_SET);

	char* buf = (char*)malloc((size_t)size + 1);
	assert(buf != NULL, {
		fclose(f);
		fprintf(stderr, "Malloc failed\n");
	});
	assert(fread(buf, 1, (unsigned long)size, f) == size, { 
		fclose(f);
		free(buf);
		fprintf(stderr, "Failed to read '%s'\n", filepath); 
	});
	buf[size] = 0;

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
		struct stat buffer, buffer2;

		// Search for specified template file
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
				continue;
			})
			break;
		}
		if (j == settings.path_count) {
			if (settings.verbose)printf("\033[33mWarn: Ignoring template file '%s'. Not found.\n\033[0m", tf.infilename);
			continue;
		}

		assert(realpath(settings.outdir, realpath_outpath) != NULL, {
			printf("Failed:: outdir '%s' doesn't exist\n", realpath_outpath);
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

		// Figure out if the file needs to be regenerated or not
		if (stat(path, &buffer2) == -1) {
			printf("Error: Infile stat failed\n");
			goto skip_dependency_gen;
		}

		if (stat(outfilepath, &buffer) == -1) {
			if (settings.verbose)printf("\033[32mNotice: Regenerating '%s'\n", outfilepath);
		}
		else {
			struct timespec outfile_modified = buffer.st_mtimespec;
			struct timespec infile_modified = buffer2.st_mtimespec;
		  if (infile_modified.tv_sec >= outfile_modified.tv_sec ||
        (infile_modified.tv_sec == outfile_modified.tv_sec && infile_modified.tv_nsec >= outfile_modified.tv_nsec)) {
				if (settings.verbose) printf("\033[32mNotice: Regenerating '%s'\n", outfilepath);
			} else {
				goto skip_dependency_gen;
			}
		}

		{
			// Begin generating output file
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
					cursor += strlen(found->needle);
					fprintf(outputfile, "%s", found->with);
				}
				else {
					fprintf(outputfile, "%c", *cursor);
					cursor += 1;
				}
			}
			if (settings.verbose)printf("\033[32mNotice: Regenerated '%s'\n", outfilepath);

			free((char*)content);
			fclose(outputfile);
		}

skip_dependency_gen:
		for (int i = 0; i < ctemplate.deps_count; i++) {
			forward_table fwd = ctemplate.deps[i].fwd_table;
			replacement parent = replacement_;
			replacement r = replacement_forward(ctemplate.deps[i].template_.replacement, parent, fwd);
			generator_run(settings, ctemplate.deps[i].template_, r);
			replacement_free(&r);
		}
	}
}

#endif

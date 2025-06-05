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
#include <ctype.h>
#include <assert.h>

typedef struct {
	union {
		struct {
			const char *symbol;
		} normal;
		struct {
			const char* fmt;
		} formatted;
	};
	int type; // 0 = undef, 1 = normal, 2 = formatted
	const char *as;
} forward_item;
#define fwd(...) (forward_item) {__VA_ARGS__}
#define symbollit(sym) .normal.symbol=sym, .type = 1
#define symbolfmt(fmt_, ...) .formatted.fmt=fmt_,\
														 .type = 2

typedef struct {
	forward_item *fwd_items;
	size_t fwd_items_count, fwd_items_capacity;
} forward_table;

typedef struct {
	const char *needle, *with;
	int type; // 0 - stack allocated 'with', 1 - heap allocated 'with'
	int with_refcounter;
} replacement_item;

typedef struct {
	replacement_item *replacements;
	size_t replacements_count, replacements_capacity;
} replacement;

typedef struct {
	size_t count;
	const char* data;
} string_view;

#define assert_(cond, teardown)\
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

typedef struct dependency_settings {
	bool embed;
	int embedloc;
} dependency_settings;
#define dep_settings(...) (dependency_settings) {__VA_ARGS__}

typedef struct dependency {
	forward_table fwd_table;
	ctemplate template_;
	dependency_settings settings;
} dependency;

typedef const char* path;
typedef struct generator_settings {
	path *search_paths;
	size_t path_count;
	const char* outdir;
	bool verbose;
	bool dryrun;
	bool embed_deps; // ?
} generator_settings;
#define gen_settings_def() (generator_settings) {.search_path={"."}}
#define gen_settings(...) (generator_settings) {__VA_ARGS__}
#define paths(...) (path[]){__VA_ARGS__}, .path_count = (sizeof((path[]){__VA_ARGS__})/sizeof(path))

/*
 * @desc 			  Creates and initializes a ctemplate for use later
 * @notes 			Must be cleaned up with template_free
 * @return 			The created template instance
 */
ctemplate template_create(const char*);

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
void template_adddep(ctemplate*, ctemplate, forward_table, dependency_settings);

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
replacement replacement_forward(generator_settings settings, replacement tplt, replacement dep_tplt, forward_table with); 

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

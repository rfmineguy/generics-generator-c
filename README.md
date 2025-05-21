# generics-generator-c
This project is version two of the generics generator series. The original was written in the Zig programming language, and was fully functional. However when it came down to the actual use of that version it fell short.
The reasons it fell short are as follows:<br>
- Didn't integrate very easily into common C programming build systems such as cmake and make
- Was overly complicated in the way it handled template files
- Had too many moving parts

That said, the Zig version informed almost all of what makes this version better

# Features
- Templates
- Forwarding Tables
- Replacements
- Generators
- Header only


## Templates
Templates define the expected symbols, replacements, source files, and output formats of your template.

Example:
```c
ctemplate linkedlist = template_create();
template_addfile       (&linkedlist, "linked_list.htpl", "linked_list_$T.h");
template_addfile       (&linkedlist, "linked_list.ctpl", "linked_list_$T.c");
template_addreplacement(&linkedlist, "$T",     NULL);
template_addreplacement(&linkedlist, "^T",     NULL);
template_addreplacement(&linkedlist, "PRINTF", "printf");
template_addreplacement(&linkedlist, "FREE",   "free");
template_addreplacement(&linkedlist, "CALLOC", "calloc");
template_addreplacement(&linkedlist, "HEADER", "stdint.h");
```

**template_create()**
- gives you a fresh template to use for later

**addfile()** 
- associates the given template file to the template and defines the output format

**addreplacement()**
- defines a symbol and its default replacement string
- when the default replacement string is *NULL* this means that there is none and it must be provided later

## Forwarding Tables
Forwarding tables are used when one template depends on another. They define how the replacements for the parent get forwarded to the dependency

Example:
```c
// Setup the linkedlist template
ctemplate linkedlist = template_create();
template_addfile       (&linkedlist, "linked_list.htpl", "linked_list_$T.h");
template_addfile       (&linkedlist, "linked_list.ctpl", "linked_list_$T.c");
template_addreplacement(&linkedlist, "$T",     NULL);
template_addreplacement(&linkedlist, "^T",     NULL);
template_addreplacement(&linkedlist, "PRINTF", "printf");
template_addreplacement(&linkedlist, "FREE",   "free");
template_addreplacement(&linkedlist, "CALLOC", "calloc");
template_addreplacement(&linkedlist, "HEADER", "stdint.h");

// Setup the queue template
ctemplate queue     =  template_create();
template_addfile       (&queue, "queue.htpl", "queue_$T.h");
template_addfile       (&queue, "queue.ctpl", "queue_$T.c");
template_addreplacement(&queue, "$T",     NULL);
template_addreplacement(&queue, "^T",     NULL);
template_addreplacement(&queue, "PRINTF", "printf");
template_addreplacement(&queue, "FREE",   "free");
template_addreplacement(&queue, "CALLOC", "calloc");
template_addreplacement(&queue, "HEADER", "stdint.h");

// Setup the forwarding table
forward_table fwd_q_ll = forward_table_create();
forward_table_forward(&fwd_q_ll, fwd(.symbol="$T", .as="$T"));
forward_table_forward(&fwd_q_ll, fwd(.symbol="^T", .as="^T"));

// Setup the dependency and specify how we forward to it (fwd_q_ll)
template_adddep(&queue, linkedlist, fwd_q_ll);
```

**forward_table_create()**
- gives you a fresh forwarding table to use for later

**forward_table_forward()**
- defines how symbols get forwarded
- the generator will forward the replacement of the parent's `.symbol` to the dependent's `.as` symbol

**template_adddep()**
- tells the template that it now should depend on a template and with what forwarding table

## Replacements
Replacements are multipurpose. They are used to both define initial values for symbols
in templates, and to specify overrides for those template symbols.
This section will specifically discuss the override functionality.

```c
replacement ll_int = replacement_create();
replacement_add(&ll_int, "$T", "int");
replacement_add(&ll_int, "^T", "int");

replacement ll_string = replacement_create();
replacement_add(&ll_string, "$T",  "string");
replacement_add(&ll_string, "^T",  "const char*");

replacement bt_int = replacement_create();
replacement_add(&bt_int, "$KEY", "const char*");
replacement_add(&bt_int, "^KEY", "string");
replacement_add(&bt_int, "$VAL", "int");
replacement_add(&bt_int, "^VAL", "int");

replacement queue_int = replacement_create();
replacement_add(&queue_int, "$T",  "int");
replacement_add(&queue_int, "^T",  "int");

replacement queue_long = replacement_create();
replacement_add(&queue_long, "$T",  "long");
replacement_add(&queue_long, "^T",  "long");
```

**replacement_create()**
- gives you a fresh relpacement map to use for later

**replacement_add()**
- associates a replacement string with a symbol

## Generators
Generators perform the actual work in this too. They take all the data you've built up in the earlier stages
and use it to produce a final generated file.
Generators also have incremental builds, which means they won't run unless something has changed in your template sources.

```c
generator_run(settings_custom(.search_paths=paths("demo_templates", "."), .outdir="."), linkedlist, ll_int);
generator_run(settings_custom(.search_paths=paths("demo_templates", "."), .outdir="."), linkedlist, ll_string);
generator_run(settings_custom(.search_paths=paths("demo_templates", "."), .outdir="."), queue     , queue_int);
generator_run(settings_custom(.search_paths=paths("demo_templates", "."), .outdir="."), queue     , queue_long);
```

**generator_run()**
- giving some settings, a template, and forwarding table this will generate a file based on the template and fill it in with
the data in the forwarding table

## Header Only
This tool is distributed in a header only style with convensions inspired by the [stb](https://github.com/nothings/stb) set of tools<br>
This means to use this tool, you need only two files: `gengen.h` and your own usage file (i.e. `build.c`)

```c
#define GENGEN_IMPLEMENTATION
#include "gengen.h"

int main() {
    // here goes your template making code
}
```

Building this is as easy as using any c99 compliant compiler
```bash
gcc build.c -o build
./build
```

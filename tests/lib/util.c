#include "util.h"
#include "../../gengen.h"

char* read_file_(const char* filepath) {
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

	char* buf = (char*)malloc((size_t)size);
	assert(fread(buf, 1, (unsigned long)size, f) == size, { 
		fclose(f);
		free(buf);
		fprintf(stderr, "Failed to read '%s'\n", filepath); 
	});

	fclose(f);
	return buf;
}

int util_count_symbols(const char* filepath, const char* target_symbol) {
	int total = 0;
	char* content = read_file_(filepath);
	const char* cursor = content;

	while (*cursor) {
		if (strncmp(cursor, target_symbol, strlen(target_symbol)) == 0) {
			total++;
			cursor += strlen(target_symbol);
			continue;
		}
		cursor++;
	}

	free(content);
	return total;
}

#include <stdio.h>
#include <stdlib.h>

#include "vm.h"

#define WINDOWS

static void repl() {
	for(;;) {
		char line[1024];
		printf("> ");

		if(!fgets(line, sizeof(line), stdin)) {
			printf("\n");
			break;
		}

		interpret(line);
	}
}

static char* readFile(const char* path) {
#pragma warning(suppress : 4996)
	// ReSharper disable once CppDeprecatedEntity
	FILE* file;

#ifdef WINDOWS
	fopen_s(&file, path, "rb");  // NOLINT(clang-diagnostic-deprecated-declarations)
#else
	file = fopen(path, "rb");
#endif

	if(file == NULL) {
		fprintf(stderr, "Could not open file \"%s\".\n", path);
		exit(74);  // NOLINT(concurrency-mt-unsafe)
	}

	fseek(file, 0L, SEEK_END);
	const size_t fileSize = ftell(file);
	rewind(file);

	char* buffer = (char*)malloc(fileSize + 1);
	if(buffer == NULL) {
		fprintf(stderr, "Out of memory reading \"%s\".\n", path);
		exit(74); // NOLINT(concurrency-mt-unsafe)
	}
	
	const size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
	if(bytesRead < fileSize) {
		fprintf(stderr, "Could not read file \"%s\".\n", path);
		exit(74); // NOLINT(concurrency-mt-unsafe)
	}

	buffer[bytesRead] = '\0';

	fclose(file);
	return buffer;
}

static void runFile(const char* path) {
	char* source = readFile(path);
	const InterpretResult result = interpret(source);
	free(source);

	if(result == INTERPRET_COMPILE_ERROR) exit(65); // NOLINT(concurrency-mt-unsafe)
	if(result == INTERPRET_RUNTIME_ERROR) exit(70); // NOLINT(concurrency-mt-unsafe)
}



int main(const int argc, const char *argv[]) {
	initVm();

	if(argc == 1) {
		repl();
	} else if(argc == 2) {
		runFile(argv[1]);
	} else {
		fprintf(stderr, "Usage: clox [path]\n");
		exit(64); // NOLINT(concurrency-mt-unsafe)
	}

	// Chunk chunk;
	
	// initChunk(&chunk);
	// int constant = addConstant(&chunk, 1.2);
	// writeChunk(&chunk, OP_CONSTANT, 123);
	// writeChunk(&chunk, constant, 123);

	// constant = addConstant(&chunk, 3.6);
	// writeChunk(&chunk, OP_CONSTANT, 123);
	// writeChunk(&chunk, constant, 123);

	// writeChunk(&chunk, OP_ADD, 123);

	// constant = addConstant(&chunk, 5.6);
	// writeChunk(&chunk, OP_CONSTANT, 123);
	// writeChunk(&chunk, constant, 123);

	// writeChunk(&chunk, OP_DIVIDE, 123);

	// writeChunk(&chunk, OP_NEGATE, 123);
	// writeChunk(&chunk, OP_RETURN, 123);
	// disassembleChunk(&chunk, "test chunk");
	// interpret(&chunk);

	// freeChunk(&chunk);

	freeVm();

	return 0;
}
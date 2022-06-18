#include <stdlib.h>
#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* pChunk) {
	pChunk->count = 0;
	pChunk->capacity = 0;
	pChunk->code = NULL;
	pChunk->lines = NULL;
	initValueArray(&pChunk->constants);
}

void writeChunk(Chunk* pChunk, uint8_t byte, int line) {
	if(pChunk->capacity < pChunk->count + 1) {
		int oldCapacity = pChunk->capacity;
		pChunk->capacity = GROW_CAPACITY(oldCapacity);
		pChunk->code = GROW_ARRAY(uint8_t, pChunk->code, oldCapacity, pChunk->capacity);
		pChunk->lines = GROW_ARRAY(int, pChunk->lines, oldCapacity, pChunk->capacity);
	}

	pChunk->code[pChunk->count] = byte;
	pChunk->lines[pChunk->count] = line;
	pChunk->count++;
}

void freeChunk(Chunk* pChunk) {
	FREE_ARRAY(uint8_t, pChunk->code, pChunk->capacity);
	freeValueArray(&pChunk->constants);
	FREE_ARRAY(int, pChunk->lines, pChunk->capacity);
	initChunk(pChunk);
}

int addConstant(Chunk* chunk, Value value) {
	writeValueArray(&chunk->constants, value);
	return chunk->constants.count - 1;
}
#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

void disassembleChunk(Chunk* pChunk, const char* name);
int disassembleInstruction(Chunk* pChunk, int offset);


#endif
#include <stdio.h>

#include "debug.h"
#include "value.h"


void disassembleChunk(Chunk* pChunk, const char* name) {
	printf("== %s ==\n", name);

	for(int offset = 0; offset < pChunk->count;) {
		offset = disassembleInstruction(pChunk, offset);
	}
}

static int simpleInstruction(const char* name, const int offset) {
	printf("%s\n", name);
	return offset + 1;
}

static int byteInstruction(const char* name, const Chunk* chunk, const int offset) {
	const uint8_t slot = chunk->code[offset + 1];
	printf("%-16s %4d\n", name, slot);
	return offset + 2;
}

static int jumpInstruction(const char* name, const int sign, const Chunk* chunk, const int offset) {
	uint16_t jump = (uint16_t)(chunk->code[offset + 1] << 8);
	jump |= chunk->code[offset + 2];

	printf("%-16s %4d -> %d\n", name, offset, offset + 3 + sign * jump);
	return offset + 3;
}

static int constantInstruction(const char* name, const Chunk* chunk, const int offset) {
	const uint8_t constant = chunk->code[offset + 1];
	printf("%-16s %4d '", name, constant);
	printValue(chunk->constants.values[constant]);
	printf("'\n");

	return offset + 2;
}

int disassembleInstruction(const Chunk* pChunk, const int offset) {
	printf("%04d ", offset);

	if(offset > 0 && pChunk->lines[offset] == pChunk->lines[offset - 1]) {
		printf("   | ");
	} else {
		printf("%4d ", pChunk->lines[offset]);
	}

	const uint8_t instruction = pChunk->code[offset];
	switch(instruction) {
		case OP_RETURN:
			return simpleInstruction("OP_RETURN", offset);
		case OP_POP:
			return simpleInstruction("OP_POP", offset);
		case OP_PRINT:
			return simpleInstruction("OP_PRINT", offset);
		case OP_INCREMENT:
			return simpleInstruction("OP_INCREMENT", offset);
		case OP_DECREMENT:
			return simpleInstruction("OP_DECREMENT", offset);
		case OP_NEGATE:
			return simpleInstruction("OP_NEGATE", offset);
		case OP_GREATER:
			return simpleInstruction("OP_GREATER", offset);
		case OP_EQUAL:
			return simpleInstruction("OP_EQUAL", offset);
		case OP_LESS:	
			return simpleInstruction("OP_LESS", offset);
		case OP_ADD:
			return simpleInstruction("OP_ADD", offset);
		case OP_SUBTRACT:
			return simpleInstruction("OP_SUBTRACT", offset);
		case OP_MULTIPLY:
			return simpleInstruction("OP_MULTIPLY", offset);
		case OP_DIVIDE:
			return simpleInstruction("OP_DIVIDE", offset);
		case OP_NOT:
			return simpleInstruction("OP_NOT", offset);
		case OP_CONSTANT:	
			return constantInstruction("OP_CONSTANT", pChunk, offset);
		case OP_NIL:
			return simpleInstruction("OP_NIL", offset);
		case OP_TRUE:
			return simpleInstruction("OP_TRUE", offset);
		case OP_DEFINE_GLOBAL:
			return constantInstruction("OP_DEFINE_GLOBAL", pChunk, offset);
		case OP_GET_GLOBAL:
			return constantInstruction("OP_GET_GLOBAL", pChunk, offset);
		case OP_SET_GLOBAL:
			return constantInstruction("OP_SET_GLOBAL", pChunk, offset);
		case OP_GET_LOCAL:
			return byteInstruction("OP_GET_LOCAL", pChunk, offset);
		case OP_SET_LOCAL:
			return byteInstruction("OP_SET_LOCAL", pChunk, offset);
		case OP_FALSE:
			return simpleInstruction("OP_FALSE", offset);
		case OP_JUMP:
			return jumpInstruction("OP_JUMP", 1, pChunk, offset);
		case OP_JUMP_IF_FALSE:
			return jumpInstruction("OP_JUMP_IF_FALSE", 1, pChunk, offset);
		case OP_LOOP:
			return jumpInstruction("OP_LOOP", -1, pChunk, offset);
		default:
			printf("Unknown opcode %d\n", instruction);
			return offset + 1;
	}
}


#include <stdlib.h>
#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
	chunk->lineRuns = NULL;
	chunk->lineRunCount = 0;
	chunk->lineRunCapacity = 0;
	initValueArray(&chunk->constants);
}

void writeConstant(Chunk* chunk, Value value, int line) {
	// Get the index of the value in the constant array.
	int index = addConstant(chunk, value);

	// If the index fits in a single byte, use OP_CONSTANT.
	if (index < 256) {
		writeChunk(chunk, OP_CONSTANT, line);
		writeChunk(chunk, index, line);
	}
	// Otherwise, use OP_CONSTANT_LONG.
	else {
		writeChunk(chunk, OP_CONSTANT_LONG, line);
		// Write the 24-bit operand in three bytes.
		writeChunk(chunk, (index & 0xFF0000) >> 16, line); // Higher 8 bits
		writeChunk(chunk, (index & 0x00FF00) >> 8, line);  // Middle 8 bits
		writeChunk(chunk, (index & 0x0000FF), line);       // Lower 8 bits
	}
}

void writeChunk(Chunk* chunk, uint8_t byte,int line){
	if (chunk->capacity < chunk->count + 1)
	{
		int oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(oldCapacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
	}
	chunk->code[chunk->count] = byte;

	if (chunk->lineRunCount > 0 && chunk->lineRuns[chunk->lineRunCount - 1].line == line) {
		chunk->lineRuns[chunk->lineRunCount - 1].runLength++;
	}
	else {
		if (chunk->lineRunCapacity < chunk->lineRunCount + 1) {
			int oldCapacity = chunk->lineRunCapacity;
			chunk->lineRunCapacity = GROW_CAPACITY(oldCapacity);
			chunk->lineRuns = GROW_ARRAY(LineRun, chunk->lineRuns, oldCapacity, chunk->lineRunCapacity);
		}
		chunk->lineRuns[chunk->lineRunCount].line = line;
		chunk->lineRuns[chunk->lineRunCount].runLength = 1;
		chunk->lineRunCount++;
	}

	chunk->count++;
}

void freeChunk(Chunk* chunk) {
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	FREE_ARRAY(LineRun, chunk->lineRuns, chunk->lineRunCapacity);
	freeValueArray(&chunk->constants);
	initChunk(chunk);
}
int addConstant(Chunk* chunk, Value value) {
	writeValueArray(&chunk->constants, value);
	return chunk->constants.count - 1;
}
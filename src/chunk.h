#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
	OP_CONSTANT,
	OP_CONSTANT_LONG,
	OP_NIL,
	OP_TRUE,
	OP_FALSE,
	OP_POP,
	OP_GET_LOCAL,
	OP_SET_LOCAL,
	OP_GET_GLOBAL,
	OP_SET_GLOBAL,
	OP_GET_UPVALUE,
	OP_SET_UPVALUE,
	OP_GET_PROPERTY,
	OP_SET_PROPERTY,
	OP_DEFINE_GLOBAL,
	OP_EQUAL,
	OP_GREATER,
	OP_LESS,
	OP_ADD,
	OP_SUBTRACT,
	OP_MULTIPLY,
	OP_DIVIDE,
	OP_NOT,
	OP_NEGATE,
	OP_PRINT,
	OP_DELETE,
	OP_JUMP,
	OP_JUMP_IF_FALSE,
	OP_JUMP_IF_NOT_EQUAL,
	OP_LOOP,
	OP_CALL,
	OP_CLOSURE,
	OP_CLOSE_UPVALUE,
	OP_CLASS,
	OP_RETURN,
} OpCode;

typedef struct {
	int line;
	int runLength; // Number of consecutive instructions corresponding to `line`.
} LineRun;

typedef struct {
	int count;
	int capacity;
	uint8_t* code;
	LineRun* lineRuns; // Array of RLE data.
	int lineRunCount;
	int lineRunCapacity;
	ValueArray constants;
} Chunk;

typedef struct LoopMarkers {
	int loopStart;  // Where the loop starts.
	int loopExit;   // Where the loop should exit to.
	IntArray breaks;     // Dynamically growing array for break jumps.
	IntArray continues;  // Dynamically growing array for continue jumps.
} LoopMarkers;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeConstant(Chunk* chunk, Value value, int line);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);
#endif
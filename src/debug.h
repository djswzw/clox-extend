#ifndef clox_debug_h
#define clox_debug_h
#include "chunk.h"
int getLine(Chunk chunk, int instructionIndex);
void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstruction(Chunk* chunk, int offset);
int simpleInstruction(const char* name, int offset);
#endif 
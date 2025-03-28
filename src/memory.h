#ifndef MEMORY_INSTRUCTIONS_H
#define MEMORY_INSTRUCTIONS_H

#include <stdint.h>

// MOV y ADR
void movz(uint32_t instruction);
void adr(uint32_t instruction);

// LOAD
void ldur(uint32_t instruction);
void ldurb(uint32_t instruction);
void ldurh(uint32_t instruction);

// STORE
void stur(uint32_t instruction);
void sturb(uint32_t instruction);
void sturh(uint32_t instruction);

#endif // MEMORY_INSTRUCTIONS_H

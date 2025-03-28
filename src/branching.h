#ifndef BRANCH_INSTRUCTIONS_H
#define BRANCH_INSTRUCTIONS_H

#include <stdint.h>

// Declaraciones de funciones para instrucciones de salto
void b(uint32_t instruction);
void br(uint32_t instruction);
void bcond(uint32_t instruction);
void cbz(uint32_t instruction);
void cbnz(uint32_t instruction);

#endif // BRANCH_INSTRUCTIONS_H

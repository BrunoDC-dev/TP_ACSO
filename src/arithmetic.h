#ifndef ARITHMETIC_INSTRUCTIONS_H
#define ARITHMETIC_INSTRUCTIONS_H

#include <stdint.h>

// Declaraciones de funciones aritméticas
void adds_imm(uint32_t instruction);
void adds_reg(uint32_t instruction);
void subs_imm(uint32_t instruction);
void subs_reg(uint32_t instruction);
void add(uint32_t instruction);
void mul(uint32_t instruction);

#endif // ARITHMETIC_INSTRUCTIONS_H

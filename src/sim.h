
#include <inttypes.h>
#include <stdio.h>


#ifndef _SIM_H_
#define _SIM_H_
// headers
void adds_imm(uint32_t instruction);
void adds_reg(uint32_t instruction);
void subs_imm(uint32_t instruction);
void subs_reg(uint32_t instruction);
void hlt(uint32_t instruction);
void cmp_imm(uint32_t instruction);
void b(uint32_t instruction);
void br(uint32_t instruction);
uint32_t get_bits(uint32_t number, int start, int end);

void adds_imm(uint32_t instruction);
void adds_reg(uint32_t instruction);
void subs_imm(uint32_t instruction);
void subs_reg(uint32_t instruction);
void hlt(uint32_t instruction);
void cmp_imm(uint32_t instruction);
void b(uint32_t instruction);
void br(uint32_t instruction);
void ands_reg(uint32_t instruction);
void cmp_reg(uint32_t instruction);
void beq(uint32_t instruction);
#endif
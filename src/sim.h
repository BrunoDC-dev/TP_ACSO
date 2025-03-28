
#include <inttypes.h>
#include <stdio.h>


#ifndef _SIM_H_
#define _SIM_H_

int64_t sign_extend(uint32_t number, int bits);



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
void bcond(uint32_t instruction);
void eor(uint32_t instruction);
void movz(uint32_t instruction);
void shifts_inm(uint32_t instruction);
void stur(uint32_t instruction);
void sturb(uint32_t instruction);
void ldur(uint32_t instruction);
void ldurb(uint32_t instruction);
void ldurh(uint32_t instruction);
void orr(uint32_t instruction);
void adr(uint32_t instruction);
void sturh(uint32_t instruction);
void mul (uint32_t instruction);
void cbz (uint32_t instruction);
void cbnz (uint32_t instruction);
void add_reg (uint32_t instruction);
void add_inm (uint32_t instruction);

#endif
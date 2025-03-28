#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include "sim.h"
#include "utils.h"

void adds_imm(uint32_t instruction) {
    printf("adds_imm function enter\n");
    DecodedInstr d = decode_instruction(instruction, 1);

    if (d.shift){
      printf("shifting imm12\n");
      d.imm = d.imm << 12;
    }
  
    NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] + d.imm;
    update_flags(NEXT_STATE.REGS[d.rd]);
}
  
void subs_imm(uint32_t instruction) {
    printf("subs_imm function enter\n");
    DecodedInstr d = decode_instruction(instruction, 1);
    if (d.shift){
      printf("shifting imm12\n");
      d.imm = d.imm << 12;
    }
    NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] - d.imm;
    update_flags(NEXT_STATE.REGS[d.rd]);
}

void adds_reg(uint32_t instruction) {
    printf("adds_reg function enter\n");
    DecodedInstr d = decode_instruction(instruction, 0);
    NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] + CURRENT_STATE.REGS[d.rm];
    update_flags(NEXT_STATE.REGS[d.rd]);
}
  
void subs_reg(uint32_t instruction) {
    printf("subs_reg function enter\n");
    DecodedInstr d = decode_instruction(instruction, 0);
    
    if (d.rd == 31) {
        // CMP - solo actualiza flags
        printf("cmp_reg function enter\n");
        int64_t result = CURRENT_STATE.REGS[d.rn] - CURRENT_STATE.REGS[d.rm];
        update_flags(result);
    } else {
        // SUBS - actualiza flags y guarda resultado
        printf("subs_reg function enter\n");
        NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] - CURRENT_STATE.REGS[d.rm];
        update_flags(NEXT_STATE.REGS[d.rd]);
    }
}

void add(uint32_t instruction) {
    printf("add function enter\n");
    DecodedInstr d = decode_instruction(instruction, 0);
    NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] + CURRENT_STATE.REGS[d.rm];
}

void mul (uint32_t instruction){
    printf("mul function enter\n");
    DecodedInstr d = decode_instruction(instruction, 0);
    NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] * CURRENT_STATE.REGS[d.rm];
}
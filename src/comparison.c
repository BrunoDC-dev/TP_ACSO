#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include "sim.h"
#include "utils.h"

void cmp_imm(uint32_t instruction) {
    DecodedInstr d = decode_instruction(instruction, 1);
    int64_t result = CURRENT_STATE.REGS[d.rn] - d.imm;
    update_flags(result);
}

void ands_reg (uint32_t instruction) {
    DecodedInstr d = decode_instruction(instruction, 0);
    NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] & CURRENT_STATE.REGS[d.rm];
    update_flags(NEXT_STATE.REGS[d.rd]);
}
  
void eor(uint32_t instruction){
    DecodedInstr d = decode_instruction(instruction, 0);
    NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] ^ CURRENT_STATE.REGS[d.rm];
}

void orr(uint32_t instruction){
    DecodedInstr d = decode_instruction(instruction, 0);
    NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] | CURRENT_STATE.REGS[d.rm];
}
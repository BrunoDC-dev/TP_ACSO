#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include "sim.h"

void cmp_imm(uint32_t instruction) {
    uint32_t rn = get_bits(instruction, 5, 9);
    int64_t imm12 = get_bits(instruction, 10, 21);
    uint32_t shift = get_bits(instruction, 22, 23);
    printf("rn: %x\n", rn);
  
    if (shift){
      printf("shifting imm12\n");
      imm12 = imm12 << 12;
    }
  
    int64_t result = CURRENT_STATE.REGS[rn] - imm12;
    update_flags(result);
}

void ands_reg (uint32_t instruction) {
    printf("ands_reg function enter\n");
    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t rm = get_bits(instruction, 16, 20);
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] & CURRENT_STATE.REGS[rm];
    update_flags(NEXT_STATE.REGS[rd]);
}
  
void eor(uint32_t instruction){
    printf("eor function enter\n");
    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t rm = get_bits(instruction, 16, 20);
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] ^ CURRENT_STATE.REGS[rm];
}

void orr(uint32_t instruction){
    printf("orr function enter\n");
    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t rm = get_bits(instruction, 16, 20);
    uint32_t shift = get_bits(instruction, 22, 23);
  
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] | CURRENT_STATE.REGS[rm];
    update_flags(NEXT_STATE.REGS[rd]);
}
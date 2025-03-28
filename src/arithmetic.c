#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include "sim.h"

void adds_imm(uint32_t instruction) {
    printf("adds_imm function enter\n");
  
  
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t rd = get_bits(instruction, 0, 4);
    int64_t imm12 = get_bits(instruction, 10, 21);
    uint32_t shift = get_bits(instruction, 22, 23);
  
  
    if (shift){
      printf("shifting imm12\n");
      imm12 = imm12 << 12;
    }
  
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] + imm12;
    update_flags(NEXT_STATE.REGS[rd]);
}
  
void adds_reg(uint32_t instruction) {
    printf("adds_reg function enter\n");
  
    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t rm = get_bits(instruction, 16, 20);
    
  
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] + CURRENT_STATE.REGS[rm];
    update_flags(NEXT_STATE.REGS[rd]);
}
  
  
  
void subs_imm(uint32_t instruction) {
    printf("subs_imm function enter\n");
  
    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    int64_t imm12 = get_bits(instruction, 10, 21);
    uint32_t shift = get_bits(instruction, 22, 23);
  
    if (shift){
      printf("shifting imm12\n");
      imm12 = imm12 << 12;
    }
  
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] - imm12;
    update_flags(NEXT_STATE.REGS[rd]);
}
  
void subs_reg(uint32_t instruction) {
    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t rm = get_bits(instruction, 16, 20);
    
    // Determinar si es CMP o SUBS basado en el registro de destino
    if (rd == 31) {
        printf("cmp_reg function enter\n");
        // Es CMP (comparación) - solo actualiza flags, no guarda resultado
        int64_t result = CURRENT_STATE.REGS[rn] - CURRENT_STATE.REGS[rm];
    } else {
        printf("subs_reg function enter\n");
        // Es SUBS - actualiza flags y guarda resultado
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] - CURRENT_STATE.REGS[rm];
    }
    update_flags(NEXT_STATE.REGS[rd]);
}

void add(uint32_t instruction) {
    printf("add function enter\n");
    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t rm = get_bits(instruction, 16, 20);
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] + CURRENT_STATE.REGS[rm];
    update_flags(NEXT_STATE.REGS[rd]);
}


void mul (uint32_t instruction){
    printf("mul function enter\n");
    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t rm = get_bits(instruction, 16, 20);
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] * CURRENT_STATE.REGS[rm];
    update_flags(NEXT_STATE.REGS[rd]);
   }
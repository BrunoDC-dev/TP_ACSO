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


// void adds_reg(uint32_t instruction) {
//     printf("adds_reg function enter\n");
//     DecodedInstr d = decode_instruction(instruction, 0);
    
//     NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] + CURRENT_STATE.REGS[d.rm];
//     update_flags(NEXT_STATE.REGS[d.rd]);
// }

void adds_reg(uint32_t instruction) {
    DecodedInstr d = decode_instruction(instruction, 0); // R-type
    uint32_t shift_type = get_bits(instruction, 21, 22); 
    uint32_t shift_amount = get_bits(instruction, 10, 15); 

    uint32_t operand1 = CURRENT_STATE.REGS[d.rn];
    uint32_t operand2 = CURRENT_STATE.REGS[d.rm];

    switch (shift_type) {
        case 0: // LSL (Logical Shift Left)
            operand2 = operand2 << shift_amount;
            break;
        case 1: // LSR (Logical Shift Right)
            operand2 = operand2 >> shift_amount;
            break;
        case 2: // ASR (Arithmetic Shift Right)
            operand2 = (int32_t)operand2 >> shift_amount;
            break;
        case 3: // ROR (Rotate Right)
            operand2 = (operand2 >> shift_amount) | (operand2 << (32 - shift_amount));
            break;
        default:
            break;
    }

    uint32_t result = operand1 + operand2;
    NEXT_STATE.REGS[d.rd] = result;

    update_flags(result);
}
  
// void subs_reg(uint32_t instruction) {
//     printf("subs_reg function enter\n");
//     DecodedInstr d = decode_instruction(instruction, 0);
    
//     if (d.rd == 31) {
//         // CMP - solo actualiza flags
//         printf("cmp_reg function enter\n");
//         int64_t result = CURRENT_STATE.REGS[d.rn] - CURRENT_STATE.REGS[d.rm];
//         update_flags(result);
//     } else {
//         // SUBS - actualiza flags y guarda resultado
//         printf("subs_reg function enter\n");
//         NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] - CURRENT_STATE.REGS[d.rm];
//         update_flags(NEXT_STATE.REGS[d.rd]);
//     }
// }

void subs_reg(uint32_t instruction) {
    DecodedInstr d = decode_instruction(instruction, 0); // 0 indica tipo R

    uint32_t operand1 = CURRENT_STATE.REGS[d.rn];
    uint32_t operand2 = CURRENT_STATE.REGS[d.rm];
    
    uint32_t shift_type = get_bits(instruction, 21, 22);
    uint32_t shift_amount = get_bits(instruction, 10, 15);

    switch (shift_type) {
        case 0: // LSL (Logical Shift Left)
            operand2 = operand2 << shift_amount;
            break;
        case 1: // LSR (Logical Shift Right)
            operand2 = operand2 >> shift_amount;
            break;
        case 2: // ASR (Arithmetic Shift Right)
            operand2 = (int32_t)operand2 >> shift_amount;
            break;
        case 3: // ROR (Rotate Right)
            operand2 = (operand2 >> shift_amount) | (operand2 << (32 - shift_amount));
            break;
        default:
            return;
    }

    int32_t result = (int32_t)operand1 - (int32_t)operand2;
    update_flags(result);
    
    if (d.rd != 31) {
        NEXT_STATE.REGS[d.rd] = result;
    }
}

void add(uint32_t instruction) {
    DecodedInstr d = decode_instruction(instruction, 0);
    NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] + CURRENT_STATE.REGS[d.rm];
}

void add_reg (uint32_t instruction) {
    printf("add_reg function enter\n");
    DecodedInstr d = decode_instruction(instruction, 0);
    NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] + CURRENT_STATE.REGS[d.rm];
}


void add_inm (uint32_t instruction) {
    printf("add_imm function enter\n");
    DecodedInstr d = decode_instruction(instruction, 1);

    if (d.shift){
      printf("shifting imm12\n");
      d.imm = d.imm << 12;
    }
    
    NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] + d.imm;
}

void mul (uint32_t instruction){
    DecodedInstr d = decode_instruction(instruction, 0);
    NEXT_STATE.REGS[d.rd] = CURRENT_STATE.REGS[d.rn] * CURRENT_STATE.REGS[d.rm];
}
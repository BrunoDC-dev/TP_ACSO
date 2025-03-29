#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include "sim.h"
#include "utils.h"

void movz(uint32_t instruction) {
    DecodedInstr d = decode_instruction(instruction, 5); // IM-type
    uint32_t shift_amount = 0;

    if (d.shift == 1) {
      shift_amount = 16;
    } 
    
    NEXT_STATE.REGS[d.rd] = d.imm << shift_amount;
}

void stur(uint32_t instruction) {
    DecodedInstr d = decode_instruction(instruction, 2); // D-type

    uint64_t mem_addr = CURRENT_STATE.REGS[d.rn] + d.imm;
    uint64_t value = CURRENT_STATE.REGS[d.rt];

    uint32_t lower_half = value & 0xFFFFFFFF;
    uint32_t upper_half = (value >> 32) & 0xFFFFFFFF;

    mem_write_32(mem_addr, lower_half);
    mem_write_32(mem_addr + 4, upper_half);
}

void sturb(uint32_t instruction) {
    DecodedInstr d = decode_instruction(instruction, 2); // D-type

    uint64_t mem_addr = CURRENT_STATE.REGS[d.rn] + d.imm;
    uint8_t byte_to_store = CURRENT_STATE.REGS[d.rt] & 0xFF;

    uint32_t word_data = mem_read_32(mem_addr);

    word_data = (word_data & 0xFFFFFF00) | byte_to_store;

    mem_write_32(mem_addr, word_data);
}

void ldur(uint32_t instruction) {
    DecodedInstr d = decode_instruction(instruction, 2); // D-type

    uint64_t mem_addr = CURRENT_STATE.REGS[d.rn] + d.imm;

    uint64_t upper_half = mem_read_32(mem_addr + 4);
    uint64_t lower_half = mem_read_32(mem_addr);

    uint64_t full_value = (upper_half << 32) | lower_half;

    NEXT_STATE.REGS[d.rt] = full_value;
}

void ldurb(uint32_t instruction) {
    DecodedInstr d = decode_instruction(instruction, 2); // D-type

    uint64_t mem_addr = CURRENT_STATE.REGS[d.rn] + d.imm;

    uint32_t full_word = mem_read_32(mem_addr);
    uint8_t byte_value = full_word & 0xFF;

    NEXT_STATE.REGS[d.rt] = byte_value;
}

void adr (uint32_t instruction){
    DecodedInstr d = decode_instruction(instruction, 1); // I-type

    NEXT_STATE.REGS[d.rd] = CURRENT_STATE.PC + d.imm;

    update_flags(NEXT_STATE.REGS[d.rd]);
}  

void sturh(uint32_t instruction){
    DecodedInstr d = decode_instruction(instruction, 2); // D-type

    uint64_t mem_addr = CURRENT_STATE.REGS[d.rn] + d.imm;

    uint32_t lower_half = CURRENT_STATE.REGS[d.rt] & 0xFFFF;

    mem_write_32(mem_addr, lower_half);
} 
  
void ldurh(uint32_t instruction){
    DecodedInstr d = decode_instruction(instruction, 2); // D-type

    uint64_t mem_addr = CURRENT_STATE.REGS[d.rn] + d.imm;

    uint32_t full_word = mem_read_32(mem_addr);
    uint32_t halfword = full_word & 0xFFFF;

    NEXT_STATE.REGS[d.rt] = halfword;
}
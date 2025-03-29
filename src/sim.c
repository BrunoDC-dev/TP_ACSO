#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "shell.h"
#include "sim.h"
#include "utils.h"
#include "arithmetic.h"
#include "memory.h"
#include "comparison.h"
#include "branching.h"

void (*instruction_set[])(uint32_t) = {
    subs_imm, subs_reg, adds_imm, adds_reg, hlt, cmp_imm, b, br, ands_reg,
    bcond, eor, movz, shifts_inm, stur, sturb, ldur, ldurb, orr, sturh,
    adr, ldurh, mul, cbz, cbnz
};

uint32_t opcodes[] = {
    0xf1, 0x758, 0xb1, 0x558, 0x6a2, 0x7d2, 0b000101, 0x3587C0, 0xea,
    0x54, 0xca, 0x1a5, 0x34d, 0x7C0, 0x1C0, 0x7c2, 0x1c2, 0x550,
    0x3C0, 0x10, 0x3c2, 0x4d8, 0xb4, 0xb5
};

int starts[] = {
    24, 21, 24, 21, 21, 24, 26, 10, 24,
    24, 24, 23, 22, 21, 21, 21, 21, 21,
    21, 24, 21, 21, 24, 24
};

int N = 24;

void print_binary(uint32_t number) {
    for (int i = 0; i < 32; i++) {
        printf("%d", (number >> (31 - i)) & 1);
    }
}

void process_instruction() {
    uint32_t instruction = mem_read_32(CURRENT_STATE.PC);
    int matched = -1;

    for (int i = 0; i < N; i++) {
        uint32_t extracted = get_bits(instruction, starts[i], 31);
        if (extracted == opcodes[i]) {
            matched = i;
            break;
        }
    }

    if (matched != -1) {
        instruction_set[matched](instruction);
    }

  NEXT_STATE.PC += 4;
}

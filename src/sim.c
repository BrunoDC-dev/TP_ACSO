#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include "sim.h"



// make a global variable to keep track of the number of instructions
int COUNT = 0;

// array of function pointers
void (*instruction_set[])(uint32_t) = {subs_imm, subs_reg,adds_imm, adds_reg, hlt, cmp_imm, b, br ,ands_reg, beq};
uint32_t opcodes[] = {0xf1, 0x758,0xb1, 0x558, 0x6a2, 0x7d2,0b000101,0x3587C0 ,0xea ,0x54};
int starts[] = {24, 21, 24, 21, 21, 24, 26,10, 24,24 };
int N =10 ;


void print_binary(uint32_t number) {
  for (int i = 0; i < 32; i++) {
    printf("%d", (number >> (31 - i)) & 1);
  }

}

void process_instruction()
{

  uint32_t instruction = mem_read_32(CURRENT_STATE.PC);
  printf("Instruction: %x\n", instruction);
  printf("\n");
  int flag = 1;

  for (int i = 0; i < N; i++) {
    uint32_t extracted_opcode = get_bits(instruction, starts[i], 32);
    printf("Extracted opcode (bits %d-31): 0x%x\n", starts[i], extracted_opcode);

    if (extracted_opcode == opcodes[i]) {
        printf("Matched opcode: 0x%x\n", opcodes[i]);
        instruction_set[i](instruction);
        flag = 0;
        break;
    }
}
  if (flag) {
    printf("NO MATCH \n");
  }


  NEXT_STATE.PC += 4;

}

uint32_t get_bits(uint32_t number, int start, int end) {

  if (start < 0 || end < 0 || start > end) {
    return 0;
  }

  uint32_t mask = ((1 << (end - start + 1)) - 1) << start;
  uint32_t extracted_bits = number & mask;

  return extracted_bits >> start;
}


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
  NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] < 0) ? 1 : 0;
  NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;
}

void adds_reg(uint32_t instruction) {
  printf("adds_reg function enter\n");

  uint32_t rd = get_bits(instruction, 0, 4);
  uint32_t rn = get_bits(instruction, 5, 9);
  uint32_t rm = get_bits(instruction, 16, 20);
  

  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] + CURRENT_STATE.REGS[rm];
  NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] < 0) ? 1 : 0;
  NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;
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
  NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] < 0) ? 1 : 0;
  NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;
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
      NEXT_STATE.FLAG_N = (result < 0) ? 1 : 0;
      NEXT_STATE.FLAG_Z = (result == 0) ? 1 : 0;
  } else {
      printf("subs_reg function enter\n");
      // Es SUBS - actualiza flags y guarda resultado
      NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] - CURRENT_STATE.REGS[rm];
      NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] < 0) ? 1 : 0;
      NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;
  }
}

void hlt(uint32_t instruction) {
  printf("HLT function enter\n");
  RUN_BIT = 0;
}

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
  NEXT_STATE.FLAG_N = (result < 0) ? 1 : 0;
  NEXT_STATE.FLAG_Z = (result == 0) ? 1 : 0;
}


void b(uint32_t instruction) {
  printf("b function enter\n");
  int64_t imm26 = get_bits(instruction, 0, 25);
  imm26 = imm26 << 2;
  NEXT_STATE.PC = CURRENT_STATE.PC + imm26;

}



void br(uint32_t instruction) {

}
void ands_reg (uint32_t instruction) {
  printf("ands_reg function enter\n");
  uint32_t rd = get_bits(instruction, 0, 4);
  uint32_t rn = get_bits(instruction, 5, 9);
  uint32_t rm = get_bits(instruction, 16, 20);
  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] & CURRENT_STATE.REGS[rm];
  NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] < 0) ? 1 : 0;
  NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;
}



void beq(uint32_t instruction) {
  printf("beq function enter\n");
  
    // Para BEQ en ARM de 32 bits, el inmediato suele estar en bits 0-23 (campo imm19)
    // Pero solo los bits 5-23 son el inmediato real
    uint32_t imm19 = get_bits(instruction, 5, 23);
    uint32_t cond = get_bits(instruction, 0, 3);
    printf("cond: %x\n", cond);
    // Códigos de condición para beq y blt
    // if (cond == 0b0000) {
    //     return 0; // beq (Branch if Equal)
    // } else if (cond == 0b1001) {
    //     return 1; // blt (Branch if Less Than, que es sinónimo de bmi)
    // }
    
    // Aplicar extensión de signo (si el bit más significativo está activo)
    if (imm19 & (1 << 18)) {
        // Extender el signo para un valor de 32 bits
        imm19 |= 0xFFF80000;
    }
    
    // El desplazamiento está alineado a palabras, por lo que se multiplica por 4
    imm19 = imm19 << 2;
    
    printf("Immediate value: 0x%x (%d in decimal)\n", imm19, imm19);
    printf("Current PC: 0x%x, Z flag: %d\n", CURRENT_STATE.PC, CURRENT_STATE.FLAG_Z);
    
    // Solo salta si la bandera Z está activa (resultado igual)
    if (CURRENT_STATE.FLAG_Z) {
        printf("Taking branch: PC = 0x%x + 0x%x = 0x%x\n", 
        CURRENT_STATE.PC, imm19, CURRENT_STATE.PC + imm19);
        NEXT_STATE.PC = CURRENT_STATE.PC + imm19-4;
  } else {
        printf("Not taking branch\n");
    }
}
// mia miau miau miaU...
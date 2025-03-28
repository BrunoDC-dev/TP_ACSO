#include <assert.h>
#include <string.h>
#include "shell.h"
#include "sim.h"

void b(uint32_t instruction) {
    printf("b function enter\n");
    int64_t imm26 = get_bits(instruction, 0, 25); // Extraer los bits 0-25
    int32_t offset = sign_extend(imm26, 26) << 2;          // Extender signo y desplazar 2 bits
    NEXT_STATE.PC = CURRENT_STATE.PC + offset -4;     // Actualizar el PC con el desplazamiento
}
  
void br(uint32_t instruction) {
    printf("br function enter\n");
    uint32_t rn = get_bits(instruction, 5, 9);
    NEXT_STATE.PC = CURRENT_STATE.REGS[rn]-4;
  
}

void bcond(uint32_t instruction){
    printf("b_cond function enter\n");
    uint32_t cond = get_bits(instruction, 0, 3); 
    uint32_t imm19 = get_bits(instruction, 5, 23);
    int32_t offset = sign_extend(imm19, 19) << 2;
  
    // Condiciones basadas en el valor de 'cond'
    if ((cond == 0 && CURRENT_STATE.FLAG_Z)  ||  // cond == 0: BEQ (Branch if Equal, Z == 1)
       (cond == 1 && !CURRENT_STATE.FLAG_Z) ||  // cond == 1: BNE (Branch if Not Equal, Z == 0)
       (cond == 12 && (!CURRENT_STATE.FLAG_Z && !CURRENT_STATE.FLAG_N)) ||  // cond == 12: BGT (Branch if Greater Than, Z == 0 && N == 0)
       (cond == 11 && CURRENT_STATE.FLAG_N) ||  // cond == 11: BLT (Branch if Less Than, N == 1)
       (cond == 10 && !CURRENT_STATE.FLAG_N) || // cond == 10: BGE (Branch if Greater or Equal, N == 0)
       (cond == 13 && !(!CURRENT_STATE.FLAG_Z && !CURRENT_STATE.FLAG_N))){  // cond == 13: BLE (Branch if Less or Equal, !(Z == 0 && N == 0))
  
      NEXT_STATE.PC = CURRENT_STATE.PC + offset - 4;
    } 
}

void cbz (uint32_t instruction){
    printf("cbz function enter\n");
    uint32_t rt = get_bits(instruction, 0, 4);
    uint32_t imm19 = get_bits(instruction, 5, 23);
    int32_t offset = sign_extend(imm19, 19) << 2;
    if (CURRENT_STATE.REGS[rt] == 0){
      NEXT_STATE.PC = CURRENT_STATE.PC + offset - 4;
    }
}
  
void cbnz (uint32_t instruction){
    printf("cbnz function enter\n");
    uint32_t rt = get_bits(instruction, 0, 4);
    uint32_t imm19 = get_bits(instruction, 5, 23);
    int32_t offset = sign_extend(imm19, 19) << 2;
    if (CURRENT_STATE.REGS[rt] != 0){
      NEXT_STATE.PC = CURRENT_STATE.PC + offset - 4;
    }
}
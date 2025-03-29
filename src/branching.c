#include <assert.h>
#include <string.h>
#include "shell.h"
#include "sim.h"
#include "utils.h"

void b(uint32_t instruction) {
    DecodedInstr d = decode_instruction(instruction, 3);
    int32_t offset = d.imm; // El desplazamiento ya está extendido y desplazado
    NEXT_STATE.PC = CURRENT_STATE.PC + offset - 4; // Actualizar el PC con el desplazamiento
}
  
void br(uint32_t instruction) {
  DecodedInstr d = decode_instruction(instruction, 3);
  NEXT_STATE.PC = CURRENT_STATE.REGS[d.rn] - 4; // Actualizar el PC con el valor del registro
}

void bcond(uint32_t instruction){
    DecodedInstr d = decode_instruction(instruction, 6); 

    if ((d.cond == 0 && CURRENT_STATE.FLAG_Z)  ||  
        (d.cond == 1 && !CURRENT_STATE.FLAG_Z) ||  
        (d.cond == 12 && (!CURRENT_STATE.FLAG_Z && !CURRENT_STATE.FLAG_N)) ||
        (d.cond == 11 && CURRENT_STATE.FLAG_N) ||  
        (d.cond == 10 && !CURRENT_STATE.FLAG_N) || 
        (d.cond == 13 && !(CURRENT_STATE.FLAG_Z == 0 && CURRENT_STATE.FLAG_N == 0))) {
        NEXT_STATE.PC = CURRENT_STATE.PC + d.imm - 4;
    }
}


void cbz (uint32_t instruction){
    DecodedInstr d = decode_instruction(instruction, 4);
    if (CURRENT_STATE.REGS[d.rt] == 0) {
        NEXT_STATE.PC = CURRENT_STATE.PC + d.imm - 4; // Actualizar el PC con el desplazamiento
    }
}
  
void cbnz (uint32_t instruction){
    DecodedInstr d = decode_instruction(instruction, 4);
    if (CURRENT_STATE.REGS[d.rt] != 0) {
        NEXT_STATE.PC = CURRENT_STATE.PC + d.imm - 4; // Actualizar el PC con el desplazamiento
    }
}

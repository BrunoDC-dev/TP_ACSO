#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include "sim.h"
#include "utils.h"

DecodedInstr decode_instruction(uint32_t instruction, int instr_type) {
  DecodedInstr d = {0};

  switch(instr_type) {
      case 0: // R-type
          d.rd = get_bits(instruction, 0, 4);
          d.rn = get_bits(instruction, 5, 9);
          d.rm = get_bits(instruction, 16, 20);
          d.shift = get_bits(instruction, 22, 23);
          break;

      case 1: // I-type
          d.rd = get_bits(instruction, 0, 4);
          d.rn = get_bits(instruction, 5, 9);
          d.imm = get_bits(instruction, 10, 21);
          d.shift = get_bits(instruction, 22, 23);
          break;

      case 2: // D-type
          d.rt = get_bits(instruction, 0, 4);
          d.rn = get_bits(instruction, 5, 9);
          d.imm = sign_extend(get_bits(instruction, 12, 20), 9);
          break;

      case 3: // B-type (incondicional)']
            d.rn = get_bits(instruction, 5, 9);
          break;

      case 4: // CB-type
          d.rt = get_bits(instruction, 0, 4);
          d.imm = sign_extend(get_bits(instruction, 5, 23), 19) << 2;
          break;

      case 5: // IM-type (movz)
          d.rd = get_bits(instruction, 0, 4);
          d.imm = get_bits(instruction, 5, 20);
          d.shift = get_bits(instruction, 21, 22);
          break;

      case 6: // BCOND-type
      printf("BCOND function enter\n");

        d.cond = get_bits(instruction, 0, 3);
        uint32_t imm19 = get_bits(instruction, 5, 23);
        int32_t b_cond_offset = sign_extend(imm19, 19)<<2;
        d.imm = b_cond_offset;
        break;
    case 7: // B-type (br)
        uint32_t imm26 = get_bits(instruction, 0, 25);
        int32_t b_offset = sign_extend(imm26, 26) << 2;
        d.imm = b_offset;
        break;
    case 8: //add
        d.rd = get_bits(instruction, 0, 4);
        d.rn = get_bits(instruction, 5, 9);
        d.rm = get_bits(instruction, 16, 20);
        d.ra= get_bits(instruction, 10, 14);
        break;

  }

  d.instr_type = instr_type;
  return d;
}


uint32_t get_bits(uint32_t number, int start, int end) {
    if (start < 0 || end < 0 || start > end) {
      return 0;
    }
    uint32_t mask = ((1 << (end - start + 1)) - 1) << start;
    uint32_t extracted_bits = number & mask;
  
    return extracted_bits >> start;
}
  
void update_flags(int64_t result) {
    NEXT_STATE.FLAG_N = (result < 0) ? 1 : 0;
    NEXT_STATE.FLAG_Z = (result == 0) ? 1 : 0;
}

void hlt(uint32_t instruction) {
    printf("HLT function enter\n");
    RUN_BIT = 0;
}

int64_t sign_extend(uint32_t number, int bits) {
    //if number is negative extends with ones, else extends with zeros 
    if ((number >> (bits - 1))) {
      number |= 0xFFFFFFFFFFFFFFFF << bits;
    }
    return number;
}

uint32_t negate_number(uint32_t number){
    uint32_t temp = number;
    int position = 0;
    while (temp) {
        temp >>= 1;
        position++;
    }
    
    number = ~number;
    number = number << (32 - position);
    number = number >> (32 - position);
    
    return number;
}

void shifts_inm(uint32_t instruction){
    /**
   * shifts_inm - Implementación de la instrucción de desplazamiento inmediato.
   *
   * Esta función realiza un desplazamiento lógico (a la derecha o izquierda) 
   * sobre el valor de un registro fuente y almacena el resultado en un registro destino.
   * También actualiza las banderas N (Negative) y Z (Zero) según el resultado.
   *
   * Pasos:
   * 1. Extrae los registros destino (rd) y fuente (rn) de la instrucción.
   * 2. Obtiene los valores inmediatos `imms` y `immr` de la instrucción.
   * 3. Si `imms == 63`, realiza un desplazamiento lógico a la derecha (`>>`) 
   *    usando `immr` como cantidad de bits a desplazar.
   * 4. Si `imms != 63`, calcula el complemento de `imms` y realiza un desplazamiento 
   *    lógico a la izquierda (`<<`) usando el valor calculado.
   * 5. Actualiza las banderas N y Z:
   *    - N: Se activa si el bit más significativo del resultado es 1.
   *    - Z: Se activa si el resultado es 0.
   *
   * Parámetros:
   * @instruction: Instrucción de 32 bits que contiene los registros y valores inmediatos.
   *
   * Ejemplo:
   * Si la instrucción especifica un desplazamiento lógico a la derecha de 3 bits:
   *   - rn = 0b10101010
   *   - immr = 3
   *   - Resultado: rd = 0b00010101
   */
    printf("shifts_inm function enter\n");
    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t imms = get_bits(instruction, 10, 15); 
    uint32_t immr = get_bits(instruction, 16, 21); 
  
    if(imms == 63){
      NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] >> immr; 
    }
    else{
      uint32_t negaitve_imms = negate_number(imms);
      NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] << negaitve_imms;
      printf("Valor de negaitve_imms: %d\n", negaitve_imms);
      printf("Valor de imms: %d\n", imms);
    }
    update_flags(NEXT_STATE.REGS[rd]);
  }


#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include "sim.h"

// make a global variable to keep track of the number of instructions
int COUNT = 0;

// array of function pointers
void (*instruction_set[])(uint32_t) = {
  subs_imm, subs_reg, adds_imm, adds_reg, hlt, cmp_imm, b, br, ands_reg,
  bcond, eor, movz, shifts_inm, stur, sturb, ldur, ldurb, orr ,sturh,adr 
};
uint32_t opcodes[] = {
  0xf1, 0x758, 0xb1, 0x558, 0x6a2, 0x7d2, 0b000101, 0x3587C0, 0xea,
  0x54, 0xca, 0x1a5, 0x34d, 0x7C0, 0x1C0, 0x7c2, 0x1c2, 0x550 ,0x3C0, 0x10
};
int starts[] = {
  24, 21, 24, 21, 21, 24, 26, 10, 24, 24, 24, 23, 22, 21, 21, 21, 21, 21 ,21, 24
};
int N =20;


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
  int64_t imm26 = get_bits(instruction, 0, 25); // Extraer los bits 0-25
  int32_t offset = sign_extend(imm26, 26) << 2;          // Extender signo y desplazar 2 bits
  NEXT_STATE.PC = CURRENT_STATE.PC + offset -4;     // Actualizar el PC con el desplazamiento
}

void br(uint32_t instruction) {
  printf("br function enter\n");
  uint32_t rn = get_bits(instruction, 5, 9);
  NEXT_STATE.PC = CURRENT_STATE.REGS[rn]-4;

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

int64_t sign_extend(uint32_t number, int bits) {
  //if number is negative extends with ones, else extends with zeros 
  if ((number >> bits - 1)) {
    number |= 0xFFFFFFFFFFFFFFFF << bits;
  }
  return number;
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
void eor(uint32_t instruction){
  printf("eor function enter\n");
  uint32_t rd = get_bits(instruction, 0, 4);
  uint32_t rn = get_bits(instruction, 5, 9);
  uint32_t rm = get_bits(instruction, 16, 20);
  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] ^ CURRENT_STATE.REGS[rm];
  NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] < 0) ? 1 : 0;
  NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;

  // todo se prende flags si o no ?
}

void movz(uint32_t instruction){
  printf("movz function enter\n");
  uint32_t rd = get_bits(instruction, 0, 4);
  uint32_t imm16 = get_bits(instruction, 5, 20);
  uint32_t shift = get_bits(instruction, 21, 22);
  uint32_t shift_amount = 0;
  if (shift){
    shift_amount = 16;
  }
  NEXT_STATE.REGS[rd] = imm16 << shift_amount;
  NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] < 0) ? 1 : 0;
  NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;
}

uint32_t negate_number(uint32_t number){
  uint32_t temp = number;
  int position = 0;
  while (temp) {
      temp >>= 1;
      position++;
  }
  
  number = ~number;
  
  number = number << 32 - position;
  number = number >> 32 - position;
  
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
    NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] <0) ? 1 : 0;  
    NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;
  }
  else{
    uint32_t negaitve_imms = negate_number(imms);
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] << negaitve_imms;
    NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] <0) ? 1 : 0;  
    NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;
    printf("Valor de negaitve_imms: %d\n", negaitve_imms);
    printf("Valor de imms: %d\n", imms);
  }
}

void stur(uint32_t instruction){
  /**
 * store_register - Implementación de la instrucción STUR (Store Register).
 *
 * Esta función almacena un valor de 64 bits desde un registro en la memoria.
 * Dado que la memoria trabaja con palabras de 32 bits, el valor se divide en 
 * dos partes y se almacenan por separado.
 *
 * Pasos:
 * 1. Extrae el número del registro fuente (rt) y el registro base (rn) de la instrucción.
 * 2. Obtiene el desplazamiento inmediato de 9 bits y lo extiende con signo.
 * 3. Calcula la dirección de memoria sumando el valor del registro base con el desplazamiento.
 * 4. Obtiene el valor de 64 bits del registro fuente.
 * 5. Divide el valor en dos partes de 32 bits:
 *    - `lower_half`: los 4 bytes menos significativos.
 *    - `upper_half`: los 4 bytes más significativos.
 * 6. Escribe la primera mitad en la dirección calculada.
 * 7. Escribe la segunda mitad en la dirección +4 para completar los 64 bits.
 *
 * Esto permite almacenar correctamente un registro de 64 bits en una memoria 
 * de 32 bits por acceso.
 */
  printf("stur function enter\n");
  uint32_t reg_dest = get_bits(instruction, 0, 4);   // Registro fuente
    uint32_t reg_base = get_bits(instruction, 5, 9);   // Registro base
    int32_t offset = sign_extend(get_bits(instruction, 12, 20), 9); // Extensión de signo del desplazamiento

    uint64_t mem_addr = CURRENT_STATE.REGS[reg_base] + offset;
    uint64_t value = CURRENT_STATE.REGS[reg_dest];

    // Almacenar en memoria en partes de 32 bits
    uint32_t lower_half = value & 0xFFFFFFFF;
    uint32_t upper_half = (value >> 32) & 0xFFFFFFFF;

    mem_write_32(mem_addr, lower_half);
    mem_write_32(mem_addr + 4, upper_half);
}

void sturb(uint32_t instruction){
  /**
 * store_byte - Implementación de la instrucción STURB (Store Register Byte).
 *
 * Esta función almacena un solo byte de un registro en la memoria sin afectar
 * el resto de la palabra de 32 bits en la dirección de destino.
 *
 * Pasos:
 * 1. Extrae el número del registro fuente (rt) y el registro base (rn) de la instrucción.
 * 2. Obtiene el desplazamiento inmediato de 9 bits y lo extiende con signo.
 * 3. Calcula la dirección de memoria sumando el valor del registro base con el desplazamiento.
 * 4. Obtiene el byte menos significativo del registro fuente.
 * 5. Lee la palabra de 32 bits en la dirección de memoria para preservar los otros bytes.
 * 6. Reemplaza solo el primer byte de la palabra con el nuevo valor.
 * 7. Escribe la palabra modificada de vuelta en la memoria.
 *
 * Esto permite almacenar un solo byte sin sobrescribir el resto de la palabra en memoria.
 */
  uint32_t reg_src = get_bits(instruction, 0, 4);  
  uint32_t reg_base = get_bits(instruction, 5, 9);  
  int32_t displacement = sign_extend(get_bits(instruction, 12, 20), 9);  

  uint64_t mem_addr = CURRENT_STATE.REGS[reg_base] + displacement;  

  uint8_t byte_to_store = CURRENT_STATE.REGS[reg_src] & 0xFF;  // Extraer solo el byte menos significativo

  // Leer la palabra completa de la memoria antes de modificar un solo byte
  uint32_t word_data = mem_read_32(mem_addr);

  // Sustituir solo el byte menos significativo sin afectar los otros
  word_data = (word_data & 0xFFFFFF00) | byte_to_store;

  mem_write_32(mem_addr, word_data);  // Guardar la palabra actualizad
}

void ldur(uint32_t instruction){
  /**
 * load_register - Implementación de la instrucción LDUR (Load Register).
 *
 * Esta función carga un valor de 64 bits desde la memoria a un registro.
 * Como la memoria usa palabras de 32 bits, se leen dos partes y se combinan.
 *
 * Pasos:
 * 1. Extrae el registro de destino (rt) y el registro base (rn) de la instrucción.
 * 2. Obtiene el desplazamiento inmediato de 9 bits y lo extiende con signo.
 * 3. Calcula la dirección de memoria sumando el registro base con el desplazamiento.
 * 4. Lee la palabra superior (32 bits) desde `mem_addr + 4`.
 * 5. Lee la palabra inferior (32 bits) desde `mem_addr`.
 * 6. Junta ambos valores desplazando la parte superior 32 bits a la izquierda y 
 *    combinándola con la inferior.
 * 7. Almacena el valor de 64 bits en el registro destino.
 *
 * Esto permite leer correctamente registros de 64 bits en una memoria de 32 bits.
 */
  printf("ldur function enter\n");

  uint32_t reg_dest = get_bits(instruction, 0, 4);
  uint32_t reg_base = get_bits(instruction, 5, 9);
  int32_t displacement = sign_extend(get_bits(instruction, 12, 20), 9);

  uint64_t mem_addr = CURRENT_STATE.REGS[reg_base] + displacement;

  // Leer los datos en dos partes de 32 bits y reconstruir el valor de 64 bits
  uint64_t upper_half = mem_read_32(mem_addr + 4);
  uint64_t lower_half = mem_read_32(mem_addr);

  uint64_t full_value = (upper_half << 32) | lower_half;

  NEXT_STATE.REGS[reg_dest] = full_value;
}

void ldurb(uint32_t instruction){
  /**
 * load_byte - Implementación de la instrucción LDURB (Load Register Byte).
 *
 * Esta función carga un solo byte desde la memoria a un registro sin alterar
 * los demás bits.
 *
 * Pasos:
 * 1. Extrae el número del registro destino (rt) y el registro base (rn).
 * 2. Obtiene el desplazamiento inmediato de 9 bits y lo extiende con signo.
 * 3. Calcula la dirección de memoria sumando el registro base con el desplazamiento.
 * 4. Lee una palabra de 32 bits desde la dirección de memoria.
 * 5. Extrae solo el byte menos significativo de la palabra.
 * 6. Almacena el byte en el registro destino.
 *
 * Esta operación permite cargar solo un byte sin sobrescribir otros datos.
 */
  printf("ldurb function enter\n");

  uint32_t reg_dest = get_bits(instruction, 0, 4);
  uint32_t reg_base = get_bits(instruction, 5, 9);
  int32_t displacement = sign_extend(get_bits(instruction, 12, 20), 9);

  uint64_t mem_addr = CURRENT_STATE.REGS[reg_base] + displacement;

  // Leer la palabra de 32 bits y extraer solo el primer byte
  uint32_t full_word = mem_read_32(mem_addr);
  uint8_t byte_value = full_word & 0xFF;

  NEXT_STATE.REGS[reg_dest] = byte_value;

}
// mia miau miau miaU...


void orr(uint32_t instruction){
  printf("orr function enter\n");
  uint32_t rd = get_bits(instruction, 0, 4);
  uint32_t rn = get_bits(instruction, 5, 9);
  uint32_t rm = get_bits(instruction, 16, 20);
  uint32_t shift = get_bits(instruction, 22, 23);

  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] | CURRENT_STATE.REGS[rm];
  NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] < 0) ? 1 : 0;
  NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;
}

void adr (uint32_t instruction){
  printf("adr function enter\n");
  uint32_t rd = get_bits(instruction, 0, 4);
  uint32_t imm19 = get_bits(instruction, 5, 23);
  int32_t offset = sign_extend(imm19, 19) << 2;
  NEXT_STATE.REGS[rd] = CURRENT_STATE.PC + offset;
  NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] < 0) ? 1 : 0;
  NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;
}



void add(uint32_t instruction) {
  printf("add function enter\n");
  uint32_t rd = get_bits(instruction, 0, 4);
  uint32_t rn = get_bits(instruction, 5, 9);
  uint32_t rm = get_bits(instruction, 16, 20);
  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] + CURRENT_STATE.REGS[rm];
  NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] < 0) ? 1 : 0;
  NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;
}

// TODO
// chcequea flags en cada instruccion

// STURH
// sturh W1, [X2, #0x10] (descripción: M[X2 + 0x10](15:0) = X1(15:0), osea los primeros 16 bits 
// del  registro  son guardados en los primeros 16 bits guardados en la   dirección de memoria). 
// Importante acordarse que la memoria es little endian en Arm. 
// Acuerdense que en el simulador la memoria empieza en 0x10000000, ver especificaciones, no 
// cambia la implementación pero si el testeo

void sturh(uint32_t instruction){
  printf("struh function enter\n");
  uint32_t rd = get_bits(instruction, 0, 4);
  uint32_t rn = get_bits(instruction, 5, 9);
  int32_t offset = sign_extend(get_bits(instruction, 12, 20), 9);

  printf("rd: %d\n", rd);
  printf("rn: %d\n", rn);
  printf("offset: %d\n", offset);


  uint64_t mem_addr = CURRENT_STATE.REGS[rn] + offset;
  printf("mem_addr: %x\n", mem_addr);
  uint32_t lower_half = CURRENT_STATE.REGS[rd] & 0xFFFF;
  printf("lower_half: %x\n", lower_half);

  mem_write_32(mem_addr, lower_half);
}

 // ldurh  W1,  [X2,  #0x10]  (descripción:  X1=  48’b0,  M[X2  +  0x10](15:0),  osea  48  ceros  y  los  
 // primeros 16 bits guardados en la dirección de memoria) 
void ldurh(uint32_t instruction){
  printf("ldurh function enter\n");
  uint32_t rd = get_bits(instruction, 0, 4);
  uint32_t rn = get_bits(instruction, 5, 9);
  int32_t offset = sign_extend(get_bits(instruction, 12, 20), 9);

  uint64_t mem_addr = CURRENT_STATE.REGS[rn] + offset;
  printf('Reading half-word from memory at address 0x%x\n', mem_addr);

  uint32_t lower_half = mem_read_32(mem_addr);
  NEXT_STATE.REGS[rd] = lower_half;
}

// TODO

// Br
// LDURH
// MUL
// CBZ
// CBNZ
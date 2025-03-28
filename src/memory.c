#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include "sim.h"

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
    update_flags(NEXT_STATE.REGS[rd]);
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

void adr (uint32_t instruction){
    printf("adr function enter\n");
    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t imm19 = get_bits(instruction, 5, 23);
    int32_t offset = sign_extend(imm19, 19) << 2;
    NEXT_STATE.REGS[rd] = CURRENT_STATE.PC + offset;
    update_flags(NEXT_STATE.REGS[rd]);
}  

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
  
void ldurh(uint32_t instruction){
    printf("ldurh function enter\n");
    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    int32_t offset = sign_extend(get_bits(instruction, 12, 20), 9);
  
    uint64_t mem_addr = CURRENT_STATE.REGS[rn] + offset;
    uint32_t full_word = mem_read_32(mem_addr);
    uint32_t halfword = full_word & 0xFFFF; 
    NEXT_STATE.REGS[rd] = halfword; 
}
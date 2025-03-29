#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

//
// ─── ESTRUCTURAS ────────────────────────────────────────────────────────────────
//

/**
 * Estructura general para representar una instrucción decodificada.
 * Se adapta a múltiples formatos: R, I, D, B, CB, IM.
 */
typedef struct {
    uint32_t rd;
    uint32_t rn;
    uint32_t rm;
    uint32_t rt;
    uint32_t shift;
    uint32_t cond;  // Solo para instrucciones de tipo CB
    int64_t imm;
    int instr_type;  // 0=R, 1=I, 2=D, 3=B, 4=CB, 5=IM
} DecodedInstr;

//
// ─── FUNCIONES DE DECOCIFICACIÓN ───────────────────────────────────────────────
//

/**
 * Decodifica una instrucción en formato genérico.
 * @param instruction La instrucción de 32 bits
 * @param instr_type Tipo de instrucción (R, I, D, etc.)
 * @return Estructura DecodedInstr con los campos relevantes
 */
DecodedInstr decode_instruction(uint32_t instruction, int instr_type);

//
// ─── FUNCIONES UTILITARIAS ─────────────────────────────────────────────────────
//

/**
 * Extrae un rango de bits [start:end] de un número.
 */
uint32_t get_bits(uint32_t number, int start, int end);

/**
 * Extiende el signo de un número con 'bits' bits.
 */
int64_t sign_extend(uint32_t number, int bits);

/**
 * Actualiza los flags N y Z según el resultado.
 */
void update_flags(int64_t result);

/**
 * Devuelve el complemento de un número limitado a su ancho efectivo.
 */
uint32_t negate_number(uint32_t number);

//
// ─── INSTRUCCIONES ESPECIALES ──────────────────────────────────────────────────
//

void hlt(uint32_t instruction);
void shifts_inm(uint32_t instruction);

#endif // UTILS_H

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

// Funciones utilitarias
uint32_t get_bits(uint32_t number, int start, int end);
int64_t sign_extend(uint32_t number, int bits);
void update_flags(int64_t result);
uint32_t negate_number(uint32_t number);

// Instrucciones especiales
void hlt(uint32_t instruction);
void shifts_inm(uint32_t instruction);

#endif // UTILS_H

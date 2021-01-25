/*
 * AES.h
 *
 *  Created on: Jan 24, 2021
 *      Author: Amir
 */

#ifndef AES_H_
#define AES_H_

#include "stdint.h"

uint8_t* substitute_bytes(uint8_t *array);
uint8_t* shift_rows(uint8_t *basic_array);
uint8_t* Mix_columns(uint8_t *basic_array);
uint8_t* add_round_key(uint8_t *basic_array, uint8_t *key_round);
uint8_t* AES(uint8_t *basic_array, uint8_t *key);
uint8_t* generate_round_key(uint8_t *key, uint8_t round_number);
uint8_t S_Box(uint8_t input_byte);


#endif /* AES_H_ */

/*
 * DES.h
 *
 *  Created on: Jan 21, 2021
 *      Author: Amir
 */

#ifndef DES_H_
#define DES_H_

#include "stdint.h"


uint64_t Initital_Permutation(uint64_t permutation_input);
uint64_t Final_Permutation(uint64_t permutation_input);
uint32_t Permutation_Box(uint32_t permutation_input);
uint64_t Expand(uint32_t expansion_input);
uint64_t first_permutation_choice(uint64_t permutation_input);
uint64_t second_permutation_choice(uint64_t permutation_input);
uint64_t circular_shift_left(uint64_t shifting_input,uint8_t round_number);
uint32_t Round_function(uint32_t input_half, uint64_t round_key);
uint64_t DES(uint64_t plaintext, uint64_t original_key);
uint8_t S_Box(uint8_t box_input,uint8_t box_number);


#endif /* DES_H_ */

/*
 * DES.cpp
 *
 *  Created on: Jan 21, 2021
 *      Author: Amir
 */
#include "DES.h"

uint64_t Initital_Permutation(uint64_t permutation_input)
{
	uint64_t permutation_output = 0, permutation_bitwise_temp = 0;
	uint8_t remaining_value = 2, single_bit_shift_amout = 63;

	while(1)
	{
		for(uint8_t i=64; i>0 ; i--)		// as i decreases, the bit importance increases
		{
			if (i%8 == remaining_value)
			{
					permutation_bitwise_temp = ( (uint64_t)1 << (64 - i) );				// without casting, the shift limit is 32
					// construct the output bit-by-bit
					permutation_output += ( (permutation_bitwise_temp & permutation_input) >> (64 - i) ) << single_bit_shift_amout;
					single_bit_shift_amout--;
			}

		}
		remaining_value += 2;

		if (remaining_value == 8)			// last even column
			remaining_value = 0;

		if (remaining_value == 2)			// even columns ended
			remaining_value = 1;


		if (remaining_value == 9)			// all columns are ended
			break;
	}

	return permutation_output;
}


uint64_t Final_Permutation(uint64_t permutation_input)
{
	uint64_t permutation_output = 0, permutation_bitwise_temp = 0;
	uint8_t  row_weight = 0, current_row_index=4, old_row_index=4;
	int8_t column_weight = 7;


	while (column_weight >= 0)
	{
		for(uint8_t i=1; i<=64; i++)
		{
			if ( (8*current_row_index) < i && i <= 8*(current_row_index +1) )
			{
				permutation_bitwise_temp = ( (uint64_t)1 << (64 - i) );				// without casting, the shift limit is 32
				permutation_bitwise_temp = (permutation_bitwise_temp & permutation_input ) >> (64 - i);
				permutation_output += (permutation_bitwise_temp) << ((8*row_weight) + column_weight);
				row_weight++;

			}
		}

		column_weight--;

		row_weight = 0;

		if (old_row_index - current_row_index == 4)
			current_row_index = old_row_index + 1;

		else
		{
			old_row_index = current_row_index;
			current_row_index -= 4;
		}

	}

	return permutation_output;
}



uint32_t Permutation_Box(uint32_t permutation_input)			// Unfortunately, there is no clear pattern for this permutation
{
	uint32_t permutation_output = 0, permutation_bitwise_temp = 0, bit_shift_amout = 0;
	uint8_t single_bit_index = 1;

	while (single_bit_index <= 32)
	{
		switch(single_bit_index)
		{
		case 1:
		{
			bit_shift_amout = 23;
			break;
		}
		case 2:
		{
			bit_shift_amout = 15;
			break;
		}
		case 3:
		{
			bit_shift_amout = 9;
			break;
		}
		case 4:
		{
			bit_shift_amout = 1;
			break;
		}
		case 5:
		{
			bit_shift_amout = 19;
			break;
		}
		case 6:
		{
			bit_shift_amout = 4;
			break;
		}
		case 7:
		{
			bit_shift_amout = 30;
			break;
		}
		case 8:
		{
			bit_shift_amout = 14;
			break;
		}
		case 9:
		{
			bit_shift_amout = 8;
			break;
		}
		case 10:
		{
			bit_shift_amout = 16;
			break;
		}
		case 11:
		{
			bit_shift_amout = 2;
			break;
		}
		case 12:
		{
			bit_shift_amout = 26;
			break;
		}
		case 13:
		{
			bit_shift_amout = 6;
			break;
		}
		case 14:
		{
			bit_shift_amout = 12;
			break;
		}
		case 15:
		{
			bit_shift_amout = 22;
			break;
		}
		case 16:
		{
			bit_shift_amout = 31;
			break;
		}
		case 17:
		{
			bit_shift_amout = 24;
			break;
		}
		case 18:
		{
			bit_shift_amout = 18;
			break;
		}
		case 19:
		{
			bit_shift_amout = 7;
			break;
		}
		case 20:
		{
			bit_shift_amout = 29;
			break;
		}
		case 21:
		{
			bit_shift_amout = 28;
			break;
		}
		case 22:
		{
			bit_shift_amout = 3;
			break;
		}
		case 23:
		{
			bit_shift_amout = 21;
			break;
		}
		case 24:
		{
			bit_shift_amout = 13;
			break;
		}
		case 25:
		{
			bit_shift_amout = 0;
			break;
		}
		case 26:
		{
			bit_shift_amout = 20;
			break;
		}
		case 27:
		{
			bit_shift_amout = 10;
			break;
		}
		case 28:
		{
			bit_shift_amout = 25;
			break;
		}
		case 29:
		{
			bit_shift_amout = 27;
			break;
		}
		case 30:
		{
			bit_shift_amout = 5;
			break;
		}
		case 31:
		{
			bit_shift_amout = 17;
			break;
		}
		case 32:
		{
			bit_shift_amout = 11;
			break;
		}

		default:
		{
			bit_shift_amout = 50;
			break;
		}
		}

		permutation_bitwise_temp = 1 << (32 - single_bit_index);
		permutation_bitwise_temp = (permutation_bitwise_temp & permutation_input) >> (32 - single_bit_index);
		permutation_output += permutation_bitwise_temp << bit_shift_amout ;

		single_bit_index++;
	}


	return permutation_output;
}


uint64_t Expand(uint32_t expansion_input)
{
	uint64_t expansion_out = 0, expansion_bitwise_temp = 0;
	uint8_t expansion_array[6] = {0,0,0,0,0,0};
	uint8_t bit_shift_amout = 7, expansion_counter = 0;
	int8_t row_shift_amout = 7;

	for (uint8_t i=1; i<=32; i++)
	{
		expansion_bitwise_temp = 1 << (32 - i);
		expansion_bitwise_temp = ( expansion_input & expansion_bitwise_temp ) >> (32 - i);

		if (i%4 == 1)
			expansion_array[1] += expansion_bitwise_temp << bit_shift_amout;

		else if (i%4 == 2)
			expansion_array[2] += expansion_bitwise_temp << bit_shift_amout;

		else if (i%4 == 3)
			expansion_array[3] += expansion_bitwise_temp << bit_shift_amout;

		else if (i%4 == 0)
		{
			expansion_array[4] += expansion_bitwise_temp << bit_shift_amout;
			bit_shift_amout--;
		}

	}


	expansion_bitwise_temp = expansion_array[4] & (1<<0);
	expansion_array[0] = ( expansion_array[4] >> 1 ) + (expansion_bitwise_temp << 7) ;

	expansion_bitwise_temp = expansion_array[1] & (1<<7);
	expansion_array[5] = ( expansion_array[1] << 1 ) + (expansion_bitwise_temp >> 7) ;


	bit_shift_amout = 7;

	while (row_shift_amout >= 0)
	{
		expansion_bitwise_temp = expansion_array[expansion_counter] & (1<<bit_shift_amout);
		expansion_bitwise_temp = expansion_bitwise_temp >> bit_shift_amout;
		expansion_out += expansion_bitwise_temp << ( (row_shift_amout * 6) + (5 - expansion_counter) );

		expansion_counter++;

		if (expansion_counter == 6)
		{
			expansion_counter = 0;
			row_shift_amout--;
			bit_shift_amout--;
		}

	}


	return expansion_out;
}


uint64_t first_permutation_choice(uint64_t permutation_input)
{
	uint64_t permutation_output = 0, permutation_bitwise_temp, left_half = 0, right_half = 0;;
	uint32_t  left_array[4] = {0,0,0,0};
	uint32_t  right_array[4] = {0,0,0,0};
	uint8_t left_index = 3 , right_index = 0,  bit_shift_amount = 8;

	for (uint8_t i = 64; i>0 ; i--)
	{
		if (i%8 == 0)
		{
			 bit_shift_amount--;

			 if (i > 33)
				 left_index = 3;
			 else
				 left_index = 2;
			 right_index = 0;
			 continue;
		}


		permutation_bitwise_temp = (uint64_t)1 << ( 64 - i);
		permutation_bitwise_temp = ( permutation_bitwise_temp & permutation_input ) >> ( 64 - i) ;

		if (i%8 < 4)
		{
			left_array[left_index] += permutation_bitwise_temp << bit_shift_amount;
			left_index--;
		}

		else if (i%8 > 4)
		{
			right_array[right_index] += permutation_bitwise_temp << bit_shift_amount;
			right_index++;
		}

		else 		//i%8 = 4
		{
			if (i > 30)
			{
				left_array[left_index] += permutation_bitwise_temp << bit_shift_amount;
				left_index--;
			}

			else
			{
				right_array[right_index] += permutation_bitwise_temp << ( bit_shift_amount + 4);
				right_index++;
			}

		}


	}



	bit_shift_amount = 0;

	for (uint8_t j = 0; j < 4 ; j++)
	{
		if (j == 3)
			bit_shift_amount = 4;

		left_half += left_array[j] << ( (8 * (3-j) ) + bit_shift_amount ) ;

		right_half += right_array[j] << ( (8 * (3-j) ) + bit_shift_amount ) ;
	}


	left_half =  ( ( left_half >> 4) << 28 );
	right_half = right_half >> 4;


	permutation_output +=  (left_half | right_half);

	return permutation_output;
}


uint64_t second_permutation_choice(uint64_t permutation_input)	// unfortunately, there is no clear relation between 56-bits input and 48-bits output
{
	uint64_t permutation_output = 0, permutation_bitwise_temp = 0;
	uint8_t bit_shift_amount = 0;

	for (uint8_t i = 1; i <= 56; i++)
	{
		if (i== 9 ||i== 18 || i== 22 || i== 25 || i== 35 || i== 38 || i== 43 || i== 54)	// discarded values
			continue;

		switch (i)
		{
		case 1: bit_shift_amount = 43; break;
		case 2: bit_shift_amount = 24; break;
		case 3: bit_shift_amount = 41; break;
		case 4: bit_shift_amount = 32; break;
		case 5: bit_shift_amount = 42; break;
		case 6: bit_shift_amount = 38; break;
		case 7: bit_shift_amount = 28; break;
		case 8: bit_shift_amount = 30; break;
		case 10: bit_shift_amount = 36; break;
		case 11: bit_shift_amount = 45; break;
		case 12: bit_shift_amount = 33; break;
		case 13: bit_shift_amount = 25; break;
		case 14: bit_shift_amount = 47; break;
		case 15: bit_shift_amount = 39; break;
		case 16: bit_shift_amount = 29; break;
		case 17: bit_shift_amount = 46; break;
		case 19: bit_shift_amount = 34; break;
		case 20: bit_shift_amount = 26; break;
		case 21: bit_shift_amount = 37; break;
		case 23: bit_shift_amount = 35; break;
		case 24: bit_shift_amount = 44; break;
		case 26: bit_shift_amount = 31; break;
		case 27: bit_shift_amount = 27; break;
		case 28: bit_shift_amount = 40; break;
		case 29: bit_shift_amount = 1; break;
		case 30: bit_shift_amount = 17; break;
		case 31: bit_shift_amount = 21; break;
		case 32: bit_shift_amount = 0; break;
		case 33: bit_shift_amount = 13; break;
		case 34: bit_shift_amount = 7; break;
		case 36: bit_shift_amount = 2; break;
		case 37: bit_shift_amount = 20; break;
		case 39: bit_shift_amount = 9; break;
		case 40: bit_shift_amount = 16; break;
		case 41: bit_shift_amount = 23; break;
		case 42: bit_shift_amount = 4; break;
		case 44: bit_shift_amount = 11; break;
		case 45: bit_shift_amount = 14; break;
		case 46: bit_shift_amount = 5; break;
		case 47: bit_shift_amount = 19; break;
		case 48: bit_shift_amount = 12; break;
		case 49: bit_shift_amount = 10; break;
		case 50: bit_shift_amount = 3; break;
		case 51: bit_shift_amount = 15; break;
		case 52: bit_shift_amount = 22; break;
		case 53: bit_shift_amount = 6; break;
		case 55: bit_shift_amount = 18; break;
		case 56: bit_shift_amount = 8; break;
		default : bit_shift_amount = 60; break;
		}


		permutation_bitwise_temp = (uint64_t)1 << (56-i);
		permutation_bitwise_temp = ( permutation_bitwise_temp & permutation_input ) >> (56-i);
		permutation_output += permutation_bitwise_temp << bit_shift_amount;

	}

	return permutation_output;
}


uint64_t circular_shift_left(uint64_t shifting_input,uint8_t round_number)
{

	uint64_t shifting_output = 0, output_right_half = 0,  output_left_half = 0;
	uint32_t input_right_half = 0, right_half_primary_temp = 0, right_half_secondary_temp = 0;
	uint32_t input_left_half = 0, left_half_primary_temp = 0, left_half_secondary_temp = 0;
	uint8_t shift_amount = 0;


	input_right_half = shifting_input & 0xFFFFFFF;
	input_left_half = (shifting_input & 0xFFFFFFF0000000) >> 28;



	if ( round_number == 1 || round_number == 2 || round_number == 9 || round_number == 16 )
	{
		shift_amount = 1;
		right_half_primary_temp = ( input_right_half & (1<< 27) ) >> 27;
		right_half_secondary_temp = 0;
		input_right_half &= ~(1<<27);
		left_half_primary_temp = ( input_left_half & (1<< 27) ) >> 27;
		left_half_secondary_temp = 0;
		input_left_half &= ~(1<<27);
	}
	else
	{
		shift_amount = 2;
		right_half_primary_temp = ( input_right_half & (1<< 27) ) >> 27;
		right_half_secondary_temp = ( input_right_half & (1<< 26) ) >> 26;
		input_right_half &= ~(3<<26);
		left_half_primary_temp = ( input_left_half & (1<< 27) ) >> 27;
		left_half_secondary_temp = ( input_left_half & (1<< 26) ) >> 26;
		input_left_half &= ~(3<<26);
	}


	output_right_half += (input_right_half << shift_amount) | (right_half_primary_temp << (shift_amount-1) ) | (right_half_secondary_temp << 0);
	output_left_half += (input_left_half << shift_amount) | (left_half_primary_temp << (shift_amount-1) ) | (left_half_secondary_temp << 0);


	shifting_output = ( output_left_half << 28 ) + output_right_half;

	return shifting_output;
}

uint32_t Round_function(uint32_t input_half, uint64_t round_key)
{
	uint32_t output_half = 0;
	uint64_t round_temp = 0;
	uint32_t round_array[8] = {0,0,0,0,0,0,0,0};


	round_temp = Expand(input_half);

	round_temp = round_temp ^ round_key;

	round_array [0] = S_Box( ( (round_temp & ( (uint64_t)0x3F << 42 ) ) >> 42),1);
	round_array [1] = S_Box( ( (round_temp & ( (uint64_t)0x3F << 36 ) ) >> 36),2);
	round_array [2] = S_Box( ( (round_temp & ( (uint64_t)0x3F << 30 ) ) >> 30),3);
	round_array [3] = S_Box( ( (round_temp & ( (0x3F) << 24 ) ) >> 24),4);
	round_array [4] = S_Box( ( (round_temp & ( (0x3F) << 18 ) ) >> 18),5);
	round_array [5] = S_Box( ( (round_temp & ( (0x3F) << 12 ) ) >> 12),6);
	round_array [6] = S_Box( ( (round_temp & ( (0x3F) << 6 ) ) >> 6),7);
	round_array [7] = S_Box( ( (round_temp & ( (0x3F) << 0 ) ) >> 0),8);

	round_temp = 0;

	for (uint8_t i=0; i<8; i++)
	{
		round_temp += ( (round_array[i]) << (4* (7-i) ) );
	}

	output_half = Permutation_Box(round_temp);

	return output_half;
}


uint64_t DES(uint64_t plaintext, uint64_t original_key)			// the comprehensive function
{
	uint64_t ciphertext = 0, key_temp = 0, Round_key = 0, temp = 0, right_half = 0, left_half = 0;


	temp = Initital_Permutation(plaintext);
	left_half = ( temp & 0xFFFFFFFF00000000 ) >> 32;
	right_half = temp & 0x00000000FFFFFFFF;
	key_temp = first_permutation_choice(original_key);


	for (uint8_t i = 1; i <= 16; i++)
	{
		key_temp = circular_shift_left(key_temp,i);
		Round_key = second_permutation_choice(key_temp);
		temp = right_half;
		right_half = left_half ^ (Round_function(temp,Round_key));
		left_half = temp;
	}

	temp = right_half;
	right_half = left_half;
	left_half = temp;

	temp = (left_half << 32) | right_half;

	ciphertext = Final_Permutation(temp);

	return ciphertext;
}



uint8_t S_Box(uint8_t box_input,uint8_t box_number)			// Since all s-boxes are non linear, there is no clear relation between 6-bits input and 4-bits output
{
	uint8_t box_output = 0, box_selectors = 0, box_data = 0;
	box_selectors = ( box_input & (1<<0) ) + ( ( box_input & (1<<5) ) >> 4 );
	box_data = (box_input &  0x1E) >> 1 ;
	switch(box_number)
	{
	case 1:
	{
		switch(box_selectors)
		{
		case 0:
		{
			switch(box_data)
			{
			case 0: box_output = 14; break;
			case 1: box_output = 4; break;
			case 2: box_output = 13; break;
			case 3: box_output = 1; break;
			case 4: box_output = 2; break;
			case 5: box_output = 15; break;
			case 6: box_output = 11; break;
			case 7: box_output = 8; break;
			case 8: box_output = 3; break;
			case 9: box_output = 10; break;
			case 10: box_output = 6; break;
			case 11: box_output = 12; break;
			case 12: box_output = 5; break;
			case 13: box_output = 9; break;
			case 14: box_output = 0; break;
			case 15: box_output = 7; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 1:
		{
			switch(box_data)
			{
			case 0: box_output = 0; break;
			case 1: box_output = 15; break;
			case 2: box_output = 7; break;
			case 3: box_output = 4; break;
			case 4: box_output = 14; break;
			case 5: box_output = 2; break;
			case 6: box_output = 13; break;
			case 7: box_output = 1; break;
			case 8: box_output = 10; break;
			case 9: box_output = 8; break;
			case 10: box_output = 12; break;
			case 11: box_output = 11; break;
			case 12: box_output = 9; break;
			case 13: box_output = 5; break;
			case 14: box_output = 3; break;
			case 15: box_output = 8; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 2:
		{
			switch(box_data)
			{
			case 0: box_output = 4; break;
			case 1: box_output = 1; break;
			case 2: box_output = 14; break;
			case 3: box_output = 8; break;
			case 4: box_output = 13; break;
			case 5: box_output = 6; break;
			case 6: box_output = 2; break;
			case 7: box_output = 11; break;
			case 8: box_output = 15; break;
			case 9: box_output = 12; break;
			case 10: box_output = 9; break;
			case 11: box_output = 7; break;
			case 12: box_output = 3; break;
			case 13: box_output = 10; break;
			case 14: box_output = 5; break;
			case 15: box_output = 0; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 3:
		{
			switch(box_data)
			{
			case 0: box_output = 15; break;
			case 1: box_output = 12; break;
			case 2: box_output = 8; break;
			case 3: box_output = 2; break;
			case 4: box_output = 4; break;
			case 5: box_output = 9; break;
			case 6: box_output = 1; break;
			case 7: box_output = 7; break;
			case 8: box_output = 5; break;
			case 9: box_output = 11; break;
			case 10: box_output = 3; break;
			case 11: box_output = 14; break;
			case 12: box_output = 10; break;
			case 13: box_output = 0; break;
			case 14: box_output = 6; break;
			case 15: box_output = 13; break;
			default: box_output = 35; break;
			}
			break;
		}
		default: box_output = 42; break;
		}
		break;
	}

	case 2:
	{
		switch(box_selectors)
		{
		case 0:
		{
			switch(box_data)
			{
			case 0: box_output = 15; break;
			case 1: box_output = 1; break;
			case 2: box_output = 8; break;
			case 3: box_output = 14; break;
			case 4: box_output = 6; break;
			case 5: box_output = 11; break;
			case 6: box_output = 3; break;
			case 7: box_output = 4; break;
			case 8: box_output = 9; break;
			case 9: box_output = 7; break;
			case 10: box_output = 2; break;
			case 11: box_output = 13; break;
			case 12: box_output = 12; break;
			case 13: box_output = 0; break;
			case 14: box_output = 5; break;
			case 15: box_output = 10; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 1:
		{
			switch(box_data)
			{
			case 0: box_output = 3; break;
			case 1: box_output = 13; break;
			case 2: box_output = 4; break;
			case 3: box_output = 7; break;
			case 4: box_output = 15; break;
			case 5: box_output = 2; break;
			case 6: box_output = 8; break;
			case 7: box_output = 14; break;
			case 8: box_output = 12; break;
			case 9: box_output = 0; break;
			case 10: box_output = 1; break;
			case 11: box_output = 10; break;
			case 12: box_output = 6; break;
			case 13: box_output = 9; break;
			case 14: box_output = 11; break;
			case 15: box_output = 5; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 2:
		{
			switch(box_data)
			{
			case 0: box_output = 0; break;
			case 1: box_output = 14; break;
			case 2: box_output = 7; break;
			case 3: box_output = 11; break;
			case 4: box_output = 10; break;
			case 5: box_output = 4; break;
			case 6: box_output = 13; break;
			case 7: box_output = 1; break;
			case 8: box_output = 5; break;
			case 9: box_output = 8; break;
			case 10: box_output = 12; break;
			case 11: box_output = 6; break;
			case 12: box_output = 9; break;
			case 13: box_output = 3; break;
			case 14: box_output = 2; break;
			case 15: box_output = 15; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 3:
		{
			switch(box_data)
			{
			case 0: box_output = 13; break;
			case 1: box_output = 8; break;
			case 2: box_output = 10; break;
			case 3: box_output = 1; break;
			case 4: box_output = 3; break;
			case 5: box_output = 15; break;
			case 6: box_output = 4; break;
			case 7: box_output = 2; break;
			case 8: box_output = 11; break;
			case 9: box_output = 6; break;
			case 10: box_output = 7; break;
			case 11: box_output = 12; break;
			case 12: box_output = 0; break;
			case 13: box_output = 5; break;
			case 14: box_output = 14; break;
			case 15: box_output = 9; break;
			default: box_output = 35; break;
			}
			break;
		}
		default: box_output = 42; break;
		}
		break;
	}

	case 3:
	{
		switch(box_selectors)
		{
		case 0:
		{
			switch(box_data)
			{
			case 0: box_output = 10; break;
			case 1: box_output = 0; break;
			case 2: box_output = 9; break;
			case 3: box_output = 14; break;
			case 4: box_output = 6; break;
			case 5: box_output = 3; break;
			case 6: box_output = 15; break;
			case 7: box_output = 5; break;
			case 8: box_output = 1; break;
			case 9: box_output = 13; break;
			case 10: box_output = 12; break;
			case 11: box_output = 7; break;
			case 12: box_output = 11; break;
			case 13: box_output = 4; break;
			case 14: box_output = 2; break;
			case 15: box_output = 8; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 1:
		{
			switch(box_data)
			{
			case 0: box_output = 13; break;
			case 1: box_output = 7; break;
			case 2: box_output = 0; break;
			case 3: box_output = 9; break;
			case 4: box_output = 3; break;
			case 5: box_output = 4; break;
			case 6: box_output = 6; break;
			case 7: box_output = 10; break;
			case 8: box_output = 2; break;
			case 9: box_output = 8; break;
			case 10: box_output = 5; break;
			case 11: box_output = 14; break;
			case 12: box_output = 12; break;
			case 13: box_output = 11; break;
			case 14: box_output = 15; break;
			case 15: box_output = 1; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 2:
		{
			switch(box_data)
			{
			case 0: box_output = 13; break;
			case 1: box_output = 6; break;
			case 2: box_output = 4; break;
			case 3: box_output = 9; break;
			case 4: box_output = 8; break;
			case 5: box_output = 15; break;
			case 6: box_output = 3; break;
			case 7: box_output = 0; break;
			case 8: box_output = 11; break;
			case 9: box_output = 1; break;
			case 10: box_output = 2; break;
			case 11: box_output = 12; break;
			case 12: box_output = 5; break;
			case 13: box_output = 10; break;
			case 14: box_output = 14; break;
			case 15: box_output = 7; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 3:
		{
			switch(box_data)
			{
			case 0: box_output = 1; break;
			case 1: box_output = 10; break;
			case 2: box_output = 13; break;
			case 3: box_output = 0; break;
			case 4: box_output = 6; break;
			case 5: box_output = 9; break;
			case 6: box_output = 8; break;
			case 7: box_output = 7; break;
			case 8: box_output = 4; break;
			case 9: box_output = 15; break;
			case 10: box_output = 14; break;
			case 11: box_output = 3; break;
			case 12: box_output = 11; break;
			case 13: box_output = 5; break;
			case 14: box_output = 2; break;
			case 15: box_output = 12; break;
			default: box_output = 35; break;
			}
			break;
		}
		default: box_output = 42; break;
		}
		break;
	}

	case 4:
	{
		switch(box_selectors)
		{
		case 0:
		{
			switch(box_data)
			{
			case 0: box_output = 7; break;
			case 1: box_output = 13; break;
			case 2: box_output = 14; break;
			case 3: box_output = 3; break;
			case 4: box_output = 0; break;
			case 5: box_output = 6; break;
			case 6: box_output = 9; break;
			case 7: box_output = 10; break;
			case 8: box_output = 1; break;
			case 9: box_output = 2; break;
			case 10: box_output = 8; break;
			case 11: box_output = 5; break;
			case 12: box_output = 11; break;
			case 13: box_output = 12; break;
			case 14: box_output = 4; break;
			case 15: box_output = 15; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 1:
		{
			switch(box_data)
			{
			case 0: box_output = 13; break;
			case 1: box_output = 8; break;
			case 2: box_output = 11; break;
			case 3: box_output = 5; break;
			case 4: box_output = 6; break;
			case 5: box_output = 15; break;
			case 6: box_output = 0; break;
			case 7: box_output = 3; break;
			case 8: box_output = 4; break;
			case 9: box_output = 7; break;
			case 10: box_output = 2; break;
			case 11: box_output = 12; break;
			case 12: box_output = 1; break;
			case 13: box_output = 10; break;
			case 14: box_output = 14; break;
			case 15: box_output = 9; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 2:
		{
			switch(box_data)
			{
			case 0: box_output = 10; break;
			case 1: box_output = 6; break;
			case 2: box_output = 9; break;
			case 3: box_output = 0; break;
			case 4: box_output = 12; break;
			case 5: box_output = 11; break;
			case 6: box_output = 7; break;
			case 7: box_output = 13; break;
			case 8: box_output = 15; break;
			case 9: box_output = 1; break;
			case 10: box_output = 3; break;
			case 11: box_output = 14; break;
			case 12: box_output = 5; break;
			case 13: box_output = 2; break;
			case 14: box_output = 8; break;
			case 15: box_output = 4; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 3:
		{
			switch(box_data)
			{
			case 0: box_output = 3; break;
			case 1: box_output = 15; break;
			case 2: box_output = 0; break;
			case 3: box_output = 6; break;
			case 4: box_output = 10; break;
			case 5: box_output = 1; break;
			case 6: box_output = 13; break;
			case 7: box_output = 8; break;
			case 8: box_output = 9; break;
			case 9: box_output = 4; break;
			case 10: box_output = 6; break;
			case 11: box_output = 11; break;
			case 12: box_output = 12; break;
			case 13: box_output = 7; break;
			case 14: box_output = 2; break;
			case 15: box_output = 14; break;
			default: box_output = 35; break;
			}
			break;
		}
		default: box_output = 42; break;
		}
		break;
	}
	case 5:
	{
		switch(box_selectors)
		{
		case 0:
		{
			switch(box_data)
			{
			case 0: box_output = 2; break;
			case 1: box_output = 12; break;
			case 2: box_output = 4; break;
			case 3: box_output = 1; break;
			case 4: box_output = 7; break;
			case 5: box_output = 10; break;
			case 6: box_output = 11; break;
			case 7: box_output = 6; break;
			case 8: box_output = 8; break;
			case 9: box_output = 5; break;
			case 10: box_output = 3; break;
			case 11: box_output = 15; break;
			case 12: box_output = 13; break;
			case 13: box_output = 0; break;
			case 14: box_output = 14; break;
			case 15: box_output = 9; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 1:
		{
			switch(box_data)
			{
			case 0: box_output = 14; break;
			case 1: box_output = 11; break;
			case 2: box_output = 2; break;
			case 3: box_output = 12; break;
			case 4: box_output = 4; break;
			case 5: box_output = 7; break;
			case 6: box_output = 13; break;
			case 7: box_output = 1; break;
			case 8: box_output = 5; break;
			case 9: box_output = 0; break;
			case 10: box_output = 15; break;
			case 11: box_output = 10; break;
			case 12: box_output = 3; break;
			case 13: box_output = 9; break;
			case 14: box_output = 8; break;
			case 15: box_output = 6; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 2:
		{
			switch(box_data)
			{
			case 0: box_output = 4; break;
			case 1: box_output = 2; break;
			case 2: box_output = 1; break;
			case 3: box_output = 11; break;
			case 4: box_output = 10; break;
			case 5: box_output = 13; break;
			case 6: box_output = 7; break;
			case 7: box_output = 8; break;
			case 8: box_output = 15; break;
			case 9: box_output = 9; break;
			case 10: box_output = 12; break;
			case 11: box_output = 5; break;
			case 12: box_output = 6; break;
			case 13: box_output = 3; break;
			case 14: box_output = 0; break;
			case 15: box_output = 14; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 3:
		{
			switch(box_data)
			{
			case 0: box_output = 11; break;
			case 1: box_output = 8; break;
			case 2: box_output = 12; break;
			case 3: box_output = 7; break;
			case 4: box_output = 1; break;
			case 5: box_output = 14; break;
			case 6: box_output = 2; break;
			case 7: box_output = 13; break;
			case 8: box_output = 6; break;
			case 9: box_output = 15; break;
			case 10: box_output = 0; break;
			case 11: box_output = 9; break;
			case 12: box_output = 10; break;
			case 13: box_output = 4; break;
			case 14: box_output = 5; break;
			case 15: box_output = 3; break;
			default: box_output = 35; break;
			}
			break;
		}
		default: box_output = 42; break;
		}
		break;
	}
	case 6:
	{
		switch(box_selectors)
		{
		case 0:
		{
			switch(box_data)
			{
			case 0: box_output = 12; break;
			case 1: box_output = 1; break;
			case 2: box_output = 10; break;
			case 3: box_output = 15; break;
			case 4: box_output = 9; break;
			case 5: box_output = 2; break;
			case 6: box_output = 6; break;
			case 7: box_output = 8; break;
			case 8: box_output = 0; break;
			case 9: box_output = 13; break;
			case 10: box_output = 3; break;
			case 11: box_output = 4; break;
			case 12: box_output = 14; break;
			case 13: box_output = 7; break;
			case 14: box_output = 5; break;
			case 15: box_output = 11; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 1:
		{
			switch(box_data)
			{
			case 0: box_output = 10; break;
			case 1: box_output = 15; break;
			case 2: box_output = 4; break;
			case 3: box_output = 2; break;
			case 4: box_output = 7; break;
			case 5: box_output = 12; break;
			case 6: box_output = 9; break;
			case 7: box_output = 5; break;
			case 8: box_output = 6; break;
			case 9: box_output = 1; break;
			case 10: box_output = 13; break;
			case 11: box_output = 14; break;
			case 12: box_output = 0; break;
			case 13: box_output = 11; break;
			case 14: box_output = 3; break;
			case 15: box_output = 8; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 2:
		{
			switch(box_data)
			{
			case 0: box_output = 9; break;
			case 1: box_output = 14; break;
			case 2: box_output = 15; break;
			case 3: box_output = 5; break;
			case 4: box_output = 2; break;
			case 5: box_output = 8; break;
			case 6: box_output = 12; break;
			case 7: box_output = 3; break;
			case 8: box_output = 7; break;
			case 9: box_output = 0; break;
			case 10: box_output = 4; break;
			case 11: box_output = 10; break;
			case 12: box_output = 1; break;
			case 13: box_output = 13; break;
			case 14: box_output = 11; break;
			case 15: box_output = 6; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 3:
		{
			switch(box_data)
			{
			case 0: box_output = 4; break;
			case 1: box_output = 3; break;
			case 2: box_output = 2; break;
			case 3: box_output = 12; break;
			case 4: box_output = 9; break;
			case 5: box_output = 5; break;
			case 6: box_output = 15; break;
			case 7: box_output = 10; break;
			case 8: box_output = 11; break;
			case 9: box_output = 14; break;
			case 10: box_output = 1; break;
			case 11: box_output = 7; break;
			case 12: box_output = 6; break;
			case 13: box_output = 0; break;
			case 14: box_output = 8; break;
			case 15: box_output = 13; break;
			default: box_output = 35; break;
			}
			break;
		}
		default: box_output = 42; break;
		}
		break;
	}
	case 7:
	{
		switch(box_selectors)
		{
		case 0:
		{
			switch(box_data)
			{
			case 0: box_output = 4; break;
			case 1: box_output = 11; break;
			case 2: box_output = 2; break;
			case 3: box_output = 14; break;
			case 4: box_output = 15; break;
			case 5: box_output = 0; break;
			case 6: box_output = 8; break;
			case 7: box_output = 13; break;
			case 8: box_output = 3; break;
			case 9: box_output = 12; break;
			case 10: box_output = 9; break;
			case 11: box_output = 7; break;
			case 12: box_output = 5; break;
			case 13: box_output = 10; break;
			case 14: box_output = 6; break;
			case 15: box_output = 1; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 1:
		{
			switch(box_data)
			{
			case 0: box_output = 13; break;
			case 1: box_output = 0; break;
			case 2: box_output = 11; break;
			case 3: box_output = 7; break;
			case 4: box_output = 4; break;
			case 5: box_output = 9; break;
			case 6: box_output = 1; break;
			case 7: box_output = 10; break;
			case 8: box_output = 14; break;
			case 9: box_output = 3; break;
			case 10: box_output = 5; break;
			case 11: box_output = 12; break;
			case 12: box_output = 2; break;
			case 13: box_output = 15; break;
			case 14: box_output = 8; break;
			case 15: box_output = 6; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 2:
		{
			switch(box_data)
			{
			case 0: box_output = 1; break;
			case 1: box_output = 4; break;
			case 2: box_output = 11; break;
			case 3: box_output = 13; break;
			case 4: box_output = 12; break;
			case 5: box_output = 3; break;
			case 6: box_output = 7; break;
			case 7: box_output = 14; break;
			case 8: box_output = 10; break;
			case 9: box_output = 15; break;
			case 10: box_output = 6; break;
			case 11: box_output = 8; break;
			case 12: box_output = 0; break;
			case 13: box_output = 5; break;
			case 14: box_output = 9; break;
			case 15: box_output = 2; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 3:
		{
			switch(box_data)
			{
			case 0: box_output = 6; break;
			case 1: box_output = 11; break;
			case 2: box_output = 13; break;
			case 3: box_output = 8; break;
			case 4: box_output = 1; break;
			case 5: box_output = 4; break;
			case 6: box_output = 10; break;
			case 7: box_output = 7; break;
			case 8: box_output = 9; break;
			case 9: box_output = 5; break;
			case 10: box_output = 0; break;
			case 11: box_output = 15; break;
			case 12: box_output = 14; break;
			case 13: box_output = 2; break;
			case 14: box_output = 3; break;
			case 15: box_output = 12; break;
			default: box_output = 35; break;
			}
			break;
		}
		default: box_output = 42; break;
		}
		break;
	}
	case 8:
	{
		switch(box_selectors)
		{
		case 0:
		{
			switch(box_data)
			{
			case 0: box_output = 13; break;
			case 1: box_output = 2; break;
			case 2: box_output = 8; break;
			case 3: box_output = 4; break;
			case 4: box_output = 6; break;
			case 5: box_output = 15; break;
			case 6: box_output = 11; break;
			case 7: box_output = 1; break;
			case 8: box_output = 10; break;
			case 9: box_output = 9; break;
			case 10: box_output = 3; break;
			case 11: box_output = 14; break;
			case 12: box_output = 5; break;
			case 13: box_output = 0; break;
			case 14: box_output = 12; break;
			case 15: box_output = 7; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 1:
		{
			switch(box_data)
			{
			case 0: box_output = 1; break;
			case 1: box_output = 15; break;
			case 2: box_output = 13; break;
			case 3: box_output = 8; break;
			case 4: box_output = 10; break;
			case 5: box_output = 3; break;
			case 6: box_output = 7; break;
			case 7: box_output = 4; break;
			case 8: box_output = 12; break;
			case 9: box_output = 5; break;
			case 10: box_output = 6; break;
			case 11: box_output = 11; break;
			case 12: box_output = 0; break;
			case 13: box_output = 14; break;
			case 14: box_output = 9; break;
			case 15: box_output = 2; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 2:
		{
			switch(box_data)
			{
			case 0: box_output = 7; break;
			case 1: box_output = 11; break;
			case 2: box_output = 4; break;
			case 3: box_output = 1; break;
			case 4: box_output = 9; break;
			case 5: box_output = 12; break;
			case 6: box_output = 14; break;
			case 7: box_output = 2; break;
			case 8: box_output = 0; break;
			case 9: box_output = 6; break;
			case 10: box_output = 10; break;
			case 11: box_output = 13; break;
			case 12: box_output = 15; break;
			case 13: box_output = 3; break;
			case 14: box_output = 5; break;
			case 15: box_output = 8; break;
			default: box_output = 35; break;
			}
			break;
		}
		case 3:
		{
			switch(box_data)
			{
			case 0: box_output = 2; break;
			case 1: box_output = 1; break;
			case 2: box_output = 14; break;
			case 3: box_output = 7; break;
			case 4: box_output = 4; break;
			case 5: box_output = 10; break;
			case 6: box_output = 8; break;
			case 7: box_output = 13; break;
			case 8: box_output = 15; break;
			case 9: box_output = 12; break;
			case 10: box_output = 9; break;
			case 11: box_output = 0; break;
			case 12: box_output = 3; break;
			case 13: box_output = 5; break;
			case 14: box_output = 6; break;
			case 15: box_output = 11; break;
			default: box_output = 35; break;
			}
			break;
		}
		default: box_output = 42; break;
		}
		break;
	}
	default: box_output = 60; break;
	}

	return box_output;
}

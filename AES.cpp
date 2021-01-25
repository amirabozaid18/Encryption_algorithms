/*
 * AES.cpp
 *
 *  Created on: Jan 24, 2021
 *      Author: Amir
 */

#include "AES.h"


uint8_t* substitute_bytes(uint8_t *basic_array)
{
	for (uint8_t i = 0; i<16; i++)
		basic_array[i] = S_Box(basic_array[i]);

	return basic_array;
}

uint8_t* shift_rows(uint8_t *basic_array)
{
	uint8_t first_row_array[4] = {0,0,0,0};
	uint8_t second_row_array[4] = {0,0,0,0};
	uint8_t third_row_array[4] = {0,0,0,0};
	uint8_t fourth_row_array[4] = {0,0,0,0};
	uint8_t shifting_temp = 0;

	// filling rows arrays
	for (uint8_t i = 0; i<16; i++)
	{
		if(i%4==0)
			first_row_array[i/4] = basic_array[i];
		else if(i%4==1)
			second_row_array[i/4] = basic_array[i];
		else if(i%4==2)
			third_row_array[i/4] = basic_array[i];
		else if(i%4==3)
			fourth_row_array[i/4] = basic_array[i];
	}

	// second_row_left rotation over one byte
	shifting_temp = second_row_array[3];
	for (uint8_t j = 0; j< 3; j++)
		second_row_array[( (j+3) % 4)] = second_row_array[j];

	second_row_array[2] = shifting_temp;


	// third_row left rotation over two bytes
	for (uint8_t j = 0; j< 2; j++)
	{
		shifting_temp = third_row_array[( (j+2) % 4)];
		third_row_array[( (j+2) % 4)] = third_row_array[j];
		third_row_array[j] = shifting_temp;
	}


	// fourth_row_left rotation over three bytes
	shifting_temp = fourth_row_array[0];
	for (uint8_t j = 3; j > 0; j--)
		fourth_row_array[( (j+1) % 4)] = fourth_row_array[j];

	fourth_row_array[1] = shifting_temp;


	// reconstructing the basuc array
	for (uint8_t i = 0; i<16; i++)
	{
		if(i%4==0)
			  basic_array[i] = first_row_array[i/4];
		else if(i%4==1)
			  basic_array[i] = second_row_array[i/4];
		else if(i%4==2)
			  basic_array[i] = third_row_array[i/4];
		else if(i%4==3)
			  basic_array[i] = fourth_row_array[i/4];
	}


	return basic_array;
}


uint8_t* Mix_columns(uint8_t *basic_array)
{

	uint8_t input_first_column_array[4] = {0,0,0,0};
	uint8_t input_second_column_array[4] = {0,0,0,0};
	uint8_t input_third_column_array[4] = {0,0,0,0};
	uint8_t input_fourth_column_array[4] = {0,0,0,0};
	uint8_t output_first_column_array[4] = {0,0,0,0};
	uint8_t output_second_column_array[4] = {0,0,0,0};
	uint8_t output_third_column_array[4] = {0,0,0,0};
	uint8_t output_fourth_column_array[4] = {0,0,0,0};
	uint64_t result = 0;

	uint8_t circular_matrix[4][4] = {
			{0x02,0x03,0x01,0x01},
			{0x01,0x02,0x03,0x01},
			{0x01,0x01,0x02,0x03},
			{0x03,0x01,0x01,0x02}
	};

	// filling input columns arrays
	for (uint8_t i = 0; i<16; i++)
	{
		if(i>= 0 && i < 4)
			input_first_column_array[i] = basic_array[i];
		else if(i>= 4 && i < 8)
			input_second_column_array[i-4] = basic_array[i];
		else if(i>= 8 && i < 12)
			input_third_column_array[i-8] = basic_array[i];
		else if(i>= 12 && i < 16)
			input_fourth_column_array[i-12] = basic_array[i];
	}

	// constructing the first output column array
	for (uint8_t i=0; i<4; i++)
	{
		for (uint8_t j=0; j<4; j++)
		{
			if (circular_matrix[i][j] == 0x03)
				result ^= ( input_first_column_array[j] ^ (2 * input_first_column_array[j]));		// addition in GF(2) can be translated easily to xor
			else
				result ^= (circular_matrix[i][j] * input_first_column_array[j]);
		}

		while (result > 0xFF)		// its the greates possible value in GF(2^8)
			result ^= 0x11B;		// 11B is the irreducible polynomial x^8 + x^4 + x^3 + x + 1

		output_first_column_array[i] = result;
		result = 0;
	}

	// constructing the second output column array
	for (uint8_t i=0; i<4; i++)
	{
		for (uint8_t j=0; j<4; j++)
		{
			if (circular_matrix[i][j] == 0x03)
				result ^= ( input_second_column_array[j] ^ (2 * input_second_column_array[j]));
			else
				result ^= (circular_matrix[i][j] * input_second_column_array[j]);
		}

		while (result > 0xFF)
			result ^= 0x11B;

		output_second_column_array[i] = result;
		result = 0;
	}

	// constructing the third output column array
	for (uint8_t i=0; i<4; i++)
	{
		for (uint8_t j=0; j<4; j++)
		{
			if (circular_matrix[i][j] == 0x03)
				result ^= ( input_third_column_array[j] ^ (2 * input_third_column_array[j]));
			else
				result ^= (circular_matrix[i][j] * input_third_column_array[j]);
		}

		while (result > 0xFF)
			result ^= 0x11B;

		output_third_column_array[i] = result;
		result = 0;
	}

	// constructing the fourth output column array
	for (uint8_t i=0; i<4; i++)
	{
		for (uint8_t j=0; j<4; j++)
		{
			if (circular_matrix[i][j] == 0x03)
				result ^= ( input_fourth_column_array[j] ^ (2 * input_fourth_column_array[j]));
			else
				result ^= (circular_matrix[i][j] * input_fourth_column_array[j]);
		}

		while (result > 0xFF)
			result ^= 0x11B;

		output_fourth_column_array[i] = result;
		result = 0;
	}


	// reconstructing the basic array
	for (uint8_t i = 0; i<16; i++)
	{
		if(i>= 0 && i < 4)
			  basic_array[i] = output_first_column_array[i];
		else if(i>= 4 && i < 8)
			  basic_array[i] = output_second_column_array[i-4];
		else if(i>= 8 && i < 12)
			  basic_array[i] = output_third_column_array[i-8];
		else if(i>= 12 && i < 16)
			  basic_array[i] = output_fourth_column_array[i-12];
	}



	return basic_array;
}


uint8_t* add_round_key(uint8_t *basic_array, uint8_t *key_round)
{
	for (uint8_t i=0; i<16; i++)
		basic_array[i] ^= key_round[i];

	return basic_array;
}

uint8_t* AES(uint8_t *basic_array, uint8_t *key)
{
	//initial transformation
	basic_array = add_round_key(basic_array, key);


	// 9 similar rounds		(note that total rounds are 10 since the key is 128 bits)
	for (uint8_t i=1; i<10; i++)
	{
		key =  generate_round_key(key,i);
		basic_array = substitute_bytes(basic_array);
		basic_array = shift_rows(basic_array);
		basic_array = Mix_columns(basic_array);
		basic_array =  add_round_key(basic_array, key);
	}


	// the tenth round does not contain Mixcolumns step

	key =  generate_round_key(key,10);
	basic_array = substitute_bytes(basic_array);
	basic_array = shift_rows(basic_array);
	basic_array =  add_round_key(basic_array, key);


	return basic_array;
}



uint8_t* generate_round_key(uint8_t *key, uint8_t round_number)
{
	uint8_t previous_key_first_column[4] = {0,0,0,0};
	uint8_t previous_key_second_column[4] = {0,0,0,0};
	uint8_t previous_key_third_column[4] = {0,0,0,0};
	uint8_t previous_key_fourth_column[4] = {0,0,0,0};
	uint8_t generated_key_first_column[4] = {0,0,0,0};
	uint8_t generated_key_second_column[4] = {0,0,0,0};
	uint8_t generated_key_third_column[4] = {0,0,0,0};
	uint8_t generated_key_fourth_column[4] = {0,0,0,0};
	uint8_t round_array[4] = {0,0,0,0};
	uint8_t round_constant = 0;

	// filling the previous key columns
	for (uint8_t i=0; i< 16; i++)
	{
		if(i>= 0 && i < 4)
			previous_key_first_column[i] = key[i];
		else if(i>= 4 && i < 8)
			 previous_key_second_column[i-4] = key[i];
		else if(i>= 8 && i < 12)
			previous_key_third_column[i-8] = key[i];
		else if(i>= 12 && i < 16)
			previous_key_fourth_column[i-12] = key[i];
	}


	// selecting the round constant

	switch(round_number)
	{
	case 1: round_constant = 0x01; break;
	case 2: round_constant = 0x02; break;
	case 3: round_constant = 0x04; break;
	case 4: round_constant = 0x08; break;
	case 5: round_constant = 0x10; break;
	case 6: round_constant = 0x20; break;
	case 7: round_constant = 0x40; break;
	case 8: round_constant = 0x80; break;
	case 9: round_constant = 0x1B; break;
	case 10: round_constant = 0x36; break;
	default : round_constant = 0xFF; break;
	}
	round_array[0] = round_constant;


	// constructing the generated_key_first_column
	for (uint8_t j=0; j<4; j++)
	{
		generated_key_first_column[j] = previous_key_fourth_column[(j+1)%4];
		generated_key_first_column[j] = S_Box(generated_key_first_column[j]);
		generated_key_first_column[j] ^= (previous_key_first_column[j] ^ round_array[j]);
	}


	// constructing the generated_key_second_column
	for (uint8_t j=0; j<4; j++)
		generated_key_second_column[j] = generated_key_first_column[j] ^ previous_key_second_column[j];


	// constructing the generated_key_third_column
	for (uint8_t j=0; j<4; j++)
		generated_key_third_column[j] = generated_key_second_column[j] ^ previous_key_third_column[j];


	// constructing the generated_key_fourth_column
	for (uint8_t j=0; j<4; j++)
		generated_key_fourth_column[j] = generated_key_third_column[j] ^ previous_key_fourth_column[j];


	// reconstructing the key round
	for (uint8_t i=0; i< 16; i++)
	{
		if(i>= 0 && i < 4)
			  key[i] = generated_key_first_column[i];
		else if(i>= 4 && i < 8)
			   key[i] = generated_key_second_column[i-4];
		else if(i>= 8 && i < 12)
			  key[i] = generated_key_third_column[i-8];
		else if(i>= 12 && i < 16)
			  key[i] = generated_key_fourth_column[i-12];
	}



	return key;
}

uint8_t S_Box(uint8_t input_byte)
{
	uint8_t output_byte = 0, row_byte = 0, column_byte = 0;
	row_byte = (input_byte & 0xF0) >> 4;
	column_byte = input_byte & 0x0F;

	switch(row_byte)
	{
	case 0:
		switch (column_byte)
		{
		case 0: output_byte = 0x63; break;
		case 1: output_byte = 0x7C;	break;
		case 2: output_byte = 0x77;	break;
		case 3: output_byte = 0x7B;	break;
		case 4: output_byte = 0xF2;	break;
		case 5: output_byte = 0x6B;	break;
		case 6: output_byte = 0x6F;	break;
		case 7: output_byte = 0xC5;	break;
		case 8: output_byte = 0x30;	break;
		case 9: output_byte = 0x01;	break;
		case 10: output_byte = 0x67; break;
		case 11: output_byte = 0x2B; break;
		case 12: output_byte = 0xFE; break;
		case 13: output_byte = 0xD7; break;
		case 14: output_byte = 0xAB; break;
		case 15: output_byte = 0x76; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 1:
		switch (column_byte)
		{
		case 0: output_byte = 0xCA; break;
		case 1: output_byte = 0x82;	break;
		case 2: output_byte = 0xC9;	break;
		case 3: output_byte = 0x7D;	break;
		case 4: output_byte = 0xFA;	break;
		case 5: output_byte = 0x59;	break;
		case 6: output_byte = 0x47;	break;
		case 7: output_byte = 0xF0;	break;
		case 8: output_byte = 0xAD;	break;
		case 9: output_byte = 0xD4;	break;
		case 10: output_byte = 0xA2; break;
		case 11: output_byte = 0xAF; break;
		case 12: output_byte = 0x9C; break;
		case 13: output_byte = 0xA4; break;
		case 14: output_byte = 0x72; break;
		case 15: output_byte = 0xC0; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 2:
		switch (column_byte)
		{
		case 0: output_byte = 0xB7; break;
		case 1: output_byte = 0xFD;	break;
		case 2: output_byte = 0x93;	break;
		case 3: output_byte = 0x26;	break;
		case 4: output_byte = 0x36;	break;
		case 5: output_byte = 0x3F;	break;
		case 6: output_byte = 0xF7;	break;
		case 7: output_byte = 0xCC;	break;
		case 8: output_byte = 0x34;	break;
		case 9: output_byte = 0xA5;	break;
		case 10: output_byte = 0xE5; break;
		case 11: output_byte = 0xF1; break;
		case 12: output_byte = 0x71; break;
		case 13: output_byte = 0xD8; break;
		case 14: output_byte = 0x31; break;
		case 15: output_byte = 0x15; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 3:
		switch (column_byte)
		{
		case 0: output_byte = 0x04; break;
		case 1: output_byte = 0xC7;	break;
		case 2: output_byte = 0x23;	break;
		case 3: output_byte = 0xC3;	break;
		case 4: output_byte = 0x18;	break;
		case 5: output_byte = 0x96;	break;
		case 6: output_byte = 0x05;	break;
		case 7: output_byte = 0x9A;	break;
		case 8: output_byte = 0x07;	break;
		case 9: output_byte = 0x12;	break;
		case 10: output_byte = 0x80; break;
		case 11: output_byte = 0xE2; break;
		case 12: output_byte = 0xEB; break;
		case 13: output_byte = 0x27; break;
		case 14: output_byte = 0xB2; break;
		case 15: output_byte = 0x75; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 4:
		switch (column_byte)
		{
		case 0: output_byte = 0x09; break;
		case 1: output_byte = 0x83;	break;
		case 2: output_byte = 0x2C;	break;
		case 3: output_byte = 0x1A;	break;
		case 4: output_byte = 0x1B;	break;
		case 5: output_byte = 0x6E;	break;
		case 6: output_byte = 0x5A;	break;
		case 7: output_byte = 0xA0;	break;
		case 8: output_byte = 0x52;	break;
		case 9: output_byte = 0x3B;	break;
		case 10: output_byte = 0xD6; break;
		case 11: output_byte = 0xB3; break;
		case 12: output_byte = 0x29; break;
		case 13: output_byte = 0xE3; break;
		case 14: output_byte = 0x2F; break;
		case 15: output_byte = 0x84; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 5:
		switch (column_byte)
		{
		case 0: output_byte = 0x53; break;
		case 1: output_byte = 0xD1;	break;
		case 2: output_byte = 0x00;	break;
		case 3: output_byte = 0xED;	break;
		case 4: output_byte = 0x20;	break;
		case 5: output_byte = 0xFC;	break;
		case 6: output_byte = 0xB1;	break;
		case 7: output_byte = 0x5B;	break;
		case 8: output_byte = 0x6A;	break;
		case 9: output_byte = 0xCB;	break;
		case 10: output_byte = 0xBE; break;
		case 11: output_byte = 0x39; break;
		case 12: output_byte = 0x4A; break;
		case 13: output_byte = 0x4C; break;
		case 14: output_byte = 0x58; break;
		case 15: output_byte = 0xCF; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 6:
		switch (column_byte)
		{
		case 0: output_byte = 0xD0; break;
		case 1: output_byte = 0xEF;	break;
		case 2: output_byte = 0xAA;	break;
		case 3: output_byte = 0xFB;	break;
		case 4: output_byte = 0x43;	break;
		case 5: output_byte = 0x4D;	break;
		case 6: output_byte = 0x33;	break;
		case 7: output_byte = 0x85;	break;
		case 8: output_byte = 0x45;	break;
		case 9: output_byte = 0xF9;	break;
		case 10: output_byte = 0x02; break;
		case 11: output_byte = 0x7F; break;
		case 12: output_byte = 0x50; break;
		case 13: output_byte = 0x3C; break;
		case 14: output_byte = 0x9F; break;
		case 15: output_byte = 0xA8; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 7:
		switch (column_byte)
		{
		case 0: output_byte = 0x51; break;
		case 1: output_byte = 0xA3;	break;
		case 2: output_byte = 0x40;	break;
		case 3: output_byte = 0x8F;	break;
		case 4: output_byte = 0x92;	break;
		case 5: output_byte = 0x9D;	break;
		case 6: output_byte = 0x38;	break;
		case 7: output_byte = 0xF5;	break;
		case 8: output_byte = 0xBC;	break;
		case 9: output_byte = 0xB6;	break;
		case 10: output_byte = 0xDA; break;
		case 11: output_byte = 0x21; break;
		case 12: output_byte = 0x10; break;
		case 13: output_byte = 0xFF; break;
		case 14: output_byte = 0xF3; break;
		case 15: output_byte = 0xD2; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 8:
		switch (column_byte)
		{
		case 0: output_byte = 0xCD; break;
		case 1: output_byte = 0x0C;	break;
		case 2: output_byte = 0x13;	break;
		case 3: output_byte = 0xEC;	break;
		case 4: output_byte = 0x5F;	break;
		case 5: output_byte = 0x97;	break;
		case 6: output_byte = 0x44;	break;
		case 7: output_byte = 0x17;	break;
		case 8: output_byte = 0xC4;	break;
		case 9: output_byte = 0xA7;	break;
		case 10: output_byte = 0x7E; break;
		case 11: output_byte = 0x3D; break;
		case 12: output_byte = 0x64; break;
		case 13: output_byte = 0x5D; break;
		case 14: output_byte = 0x19; break;
		case 15: output_byte = 0x73; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 9:
		switch (column_byte)
		{
		case 0: output_byte = 0x60; break;
		case 1: output_byte = 0x81;	break;
		case 2: output_byte = 0x4F;	break;
		case 3: output_byte = 0xDC;	break;
		case 4: output_byte = 0x22;	break;
		case 5: output_byte = 0x2A;	break;
		case 6: output_byte = 0x90;	break;
		case 7: output_byte = 0x88;	break;
		case 8: output_byte = 0x46;	break;
		case 9: output_byte = 0xEE;	break;
		case 10: output_byte = 0xB8; break;
		case 11: output_byte = 0x14; break;
		case 12: output_byte = 0xDE; break;
		case 13: output_byte = 0x5E; break;
		case 14: output_byte = 0x0B; break;
		case 15: output_byte = 0xDB; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 10:
		switch (column_byte)
		{
		case 0: output_byte = 0xE0; break;
		case 1: output_byte = 0x32;	break;
		case 2: output_byte = 0x3A;	break;
		case 3: output_byte = 0x0A;	break;
		case 4: output_byte = 0x49;	break;
		case 5: output_byte = 0x06;	break;
		case 6: output_byte = 0x24;	break;
		case 7: output_byte = 0x5C;	break;
		case 8: output_byte = 0xC2;	break;
		case 9: output_byte = 0xD3;	break;
		case 10: output_byte = 0xAC; break;
		case 11: output_byte = 0x62; break;
		case 12: output_byte = 0x91; break;
		case 13: output_byte = 0x95; break;
		case 14: output_byte = 0xE4; break;
		case 15: output_byte = 0x79; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 11:
		switch (column_byte)
		{
		case 0: output_byte = 0xE7; break;
		case 1: output_byte = 0xC8;	break;
		case 2: output_byte = 0x37;	break;
		case 3: output_byte = 0x6D;	break;
		case 4: output_byte = 0x8D;	break;
		case 5: output_byte = 0xD5;	break;
		case 6: output_byte = 0x4E;	break;
		case 7: output_byte = 0xA9;	break;
		case 8: output_byte = 0x6C;	break;
		case 9: output_byte = 0x56;	break;
		case 10: output_byte = 0xF4; break;
		case 11: output_byte = 0xEA; break;
		case 12: output_byte = 0x65; break;
		case 13: output_byte = 0x7A; break;
		case 14: output_byte = 0xAE; break;
		case 15: output_byte = 0x08; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 12:
		switch (column_byte)
		{
		case 0: output_byte = 0xBA; break;
		case 1: output_byte = 0x78;	break;
		case 2: output_byte = 0x25;	break;
		case 3: output_byte = 0x2E;	break;
		case 4: output_byte = 0x1C;	break;
		case 5: output_byte = 0xA6;	break;
		case 6: output_byte = 0xB4;	break;
		case 7: output_byte = 0xC6;	break;
		case 8: output_byte = 0xE8;	break;
		case 9: output_byte = 0xDD;	break;
		case 10: output_byte = 0x74; break;
		case 11: output_byte = 0x1F; break;
		case 12: output_byte = 0x4B; break;
		case 13: output_byte = 0xBD; break;
		case 14: output_byte = 0x8B; break;
		case 15: output_byte = 0x8A; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 13:
		switch (column_byte)
		{
		case 0: output_byte = 0x70; break;
		case 1: output_byte = 0x3E;	break;
		case 2: output_byte = 0xB5;	break;
		case 3: output_byte = 0x66;	break;
		case 4: output_byte = 0x48;	break;
		case 5: output_byte = 0x03;	break;
		case 6: output_byte = 0xF6;	break;
		case 7: output_byte = 0x0E;	break;
		case 8: output_byte = 0x61;	break;
		case 9: output_byte = 0x35;	break;
		case 10: output_byte = 0x57; break;
		case 11: output_byte = 0xB9; break;
		case 12: output_byte = 0x86; break;
		case 13: output_byte = 0xC1; break;
		case 14: output_byte = 0x1D; break;
		case 15: output_byte = 0x9E; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 14:
		switch (column_byte)
		{
		case 0: output_byte = 0xE1; break;
		case 1: output_byte = 0xF8;	break;
		case 2: output_byte = 0x98;	break;
		case 3: output_byte = 0x11;	break;
		case 4: output_byte = 0x69;	break;
		case 5: output_byte = 0xD9;	break;
		case 6: output_byte = 0x8E;	break;
		case 7: output_byte = 0x94;	break;
		case 8: output_byte = 0x9B;	break;
		case 9: output_byte = 0x1E;	break;
		case 10: output_byte = 0x87; break;
		case 11: output_byte = 0xE9; break;
		case 12: output_byte = 0xCE; break;
		case 13: output_byte = 0x55; break;
		case 14: output_byte = 0x28; break;
		case 15: output_byte = 0xDF; break;
		default: output_byte = 0x00; break;
		}
		break;
	case 15:
		switch (column_byte)
		{
		case 0: output_byte = 0x8C; break;
		case 1: output_byte = 0xA1;	break;
		case 2: output_byte = 0x89;	break;
		case 3: output_byte = 0x0D;	break;
		case 4: output_byte = 0xBF;	break;
		case 5: output_byte = 0xE6;	break;
		case 6: output_byte = 0x42;	break;
		case 7: output_byte = 0x68;	break;
		case 8: output_byte = 0x41;	break;
		case 9: output_byte = 0x99;	break;
		case 10: output_byte = 0x2D; break;
		case 11: output_byte = 0x0F; break;
		case 12: output_byte = 0xB0; break;
		case 13: output_byte = 0x54; break;
		case 14: output_byte = 0xBB; break;
		case 15: output_byte = 0x16; break;
		default: output_byte = 0x00; break;
		}
		break;

		default: output_byte = 0x00; break;
	}

	return output_byte;
}



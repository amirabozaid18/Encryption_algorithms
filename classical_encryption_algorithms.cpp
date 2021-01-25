/*
 * classical_encryption_algorithms.cpp
 *
 *  Created on: Jan 19, 2021
 *      Author: Amir
 */
#include "classical_encryption_algorithms.h"

string Ceaser_Cipher(string p,int k)
{
	string c = "";
	for (size_t i=0; i<p.length(); i++)
	{
		p[i] = toupper(p[i]);

		if ( (p[i] + k) > 90 )
			c +=  (p[i] + k -26);
		else
			c +=  (p[i] + k);
	}
	c += '\0';
	return c;
}


string Playfair_Cipher(string p,string k)
{
	char  key_matrix[5][5] = {
				{'\0','\0','\0','\0','\0'},
				{'\0','\0','\0','\0','\0'},
				{'\0','\0','\0','\0','\0'},
				{'\0','\0','\0','\0','\0'},
				{'\0','\0','\0','\0','\0'}
		};
		int key_length = k.length();
		int key_index = 0, alphabet_index = 65;
		string previous_written = "",c = "",modified_p="";
		int first_char_row, first_char_column, second_char_row, second_char_column, counter_to_exit, characters_added = 0;

		// constructing key matrix
		for (int i=0; i<5; i++)
		{
			for (int j=0; j<5; j++)
				{
					while ( key_index < key_length)											// key did not end
					{
						if (k[key_index] == 'j' || k[key_index] == 'J')
							k[key_index] = 'i';

						if ( !( (k[key_index] >= 65 && k[key_index] <= 90) || (k[key_index] >= 97 && k[key_index] <= 122) ) )			// if not alphabet
						{
							key_index++;
							continue;
						}

						k[key_index] = toupper(k[key_index]);
						if ( (previous_written.find(k[key_index]) == string::npos) )		// not found in the previous written characters
						{
							key_matrix[i][j] = k[key_index];
							previous_written += k[key_index];
							break;
						}
						else																// already exists in the previous written characters
							key_index++;


					}
					if (key_index == key_length)											// key ended but key matrix is not completely full
					{
						while (alphabet_index <= 90)
						{
							if (alphabet_index == 74)			// 'J'
							{
								alphabet_index++;
								continue;
							}

							if ((previous_written.find( char(alphabet_index) ) == string::npos ) ) // not found in the previous written characters
							{
								key_matrix[i][j] = char(alphabet_index);
								previous_written += char(alphabet_index);
								break;
							}
							else																// already exists in the previous written characters
								alphabet_index++;
						}
					}


				}


		}

		// adjusting the plaintext
		for (size_t len=0; len<p.length(); len++)
		{
			if ( !( (p[len] >= 65 && p[len] <= 90) || (p[len] >= 97 && p[len] <= 122) ) )			// if not alphabet
				continue;
			else
			{
				if (!(characters_added % 2))					//the coming character is the beginning of the pair
					modified_p += toupper(p[len]);
				else											//the coming character is the end of the pair
				{
					if (p[len] != p[len-1])						// the 2 characters of the pair are different
						modified_p += toupper(p[len]);
					else 										// the 2 characters of the pair are similar
					{
						if (p[len] != 'X' || p[len] != 'x')
						{
							modified_p += 'X';
							modified_p += toupper(p[len]);
							characters_added++;
						}

						else 									// the repeated character is 'x' or 'X'
						{
							modified_p += 'A';
							modified_p += 'X';
							characters_added++;
						}

					}


				}
			}
			characters_added++;
		}

		if (characters_added%2)									// the last pair is incomplete
		{
			if ( modified_p[characters_added-1] != 'X' )
				modified_p += 'X';
			else
				modified_p += 'A';
		}


		// generating the ciphertext
		for (size_t len=0; len<modified_p.length(); len += 2)
		{
			counter_to_exit = 0;
			// search for the positions of the 2 characters of each pair inside the key matrix
			for (int i=0; i<5; i++)
			{
				for (int j=0; j<5; j++)
				{
					if (key_matrix[i][j] == modified_p[len])
					{
						first_char_row = i;
						first_char_column = j;
						counter_to_exit++;
					}

					if (key_matrix[i][j] == modified_p[len+1])
					{
						second_char_row = i;
						second_char_column = j;
						counter_to_exit++;
					}


					if (counter_to_exit == 2)		// no need for looping since the 2 characters are identified
						break;
				}

				if (counter_to_exit == 2)
						break;
			}

			if (first_char_row == second_char_row)
			{
				c += key_matrix[first_char_row][(first_char_column + 1)%5];		// wrapped shift right
				c += key_matrix[second_char_row][(second_char_column + 1)%5];
			}
			else if (first_char_column == second_char_column)
			{
				c += key_matrix[(first_char_row + 1)%5][first_char_column];		// wrapped shift down
				c += key_matrix[(second_char_row + 1)%5][second_char_column];
			}
			else
			{
				c += key_matrix[first_char_row][second_char_column];
				c += key_matrix[second_char_row][first_char_column];
			}

		}
		return c;



}


string Hill_Cipher(string p, vector<int> temp_vector)
{
	int size = temp_vector.size(), inserted_keys = 0, row_length = (size==4)? 2: 3;
	string modified_p = "",c = "";
	vector<vector<int>> key_matrix(row_length,vector<int>(row_length,0));

	while (inserted_keys < size)
	{

		key_matrix[inserted_keys/row_length][inserted_keys%row_length] = temp_vector[inserted_keys];
		inserted_keys++;
	}


	// adjusting the plaintext
	for (size_t len=0; len<p.length(); len++)
	{
		if ( !( (p[len] >= 65 && p[len] <= 90) || (p[len] >= 97 && p[len] <= 122) ) )			// if not alphabet
			continue;
		else
			modified_p += toupper(p[len]);
	}

	if (row_length == 2 && modified_p.length() % 2)							// last pair is incomplete
		modified_p += 'X';
	else if (row_length == 3 && modified_p.length() % 3 == 1)					// last triple lacks 2 characters
		modified_p += "XX";
	else if (row_length == 3 && modified_p.length() % 3 == 2)					// last triple lacks 1 character
		modified_p += 'X';




	// generating the ciphertext

	if (row_length == 2)
	{
		for (size_t len=0; len<modified_p.length(); len += 2)
		{
			// encrypting the first character in the pair, note that we must subtract 65 since ASCHI represents A as 65
			c += ( ( ( (key_matrix[0][0] * (modified_p[len] - 65) ) + (key_matrix[0][1] * (modified_p[len+1] - 65) ) ) % 26 ) + 65 ) ;
			// encrypting the second character in the pair
			c += ( ( ( (key_matrix[1][0] * (modified_p[len] - 65) ) + (key_matrix[1][1] * (modified_p[len+1] - 65) ) ) % 26 ) + 65 ) ;
		}
	}

	else if (row_length == 3)
	{
		for (size_t len=0; len<modified_p.length(); len += 3)
		{
			// encrypting the first character in the triple, note that we must subtract 65 since ASCHI represents A as 65
			c += ( ( ( (key_matrix[0][0] * (modified_p[len] - 65) ) + (key_matrix[0][1] * (modified_p[len+1] - 65) ) + (key_matrix[0][2] * (modified_p[len+2] - 65) ) ) % 26 ) + 65 ) ;
			// encrypting the second character in the triple
			c += ( ( ( (key_matrix[1][0] * (modified_p[len] - 65) ) + (key_matrix[1][1] * (modified_p[len+1] - 65) ) + (key_matrix[1][2] * (modified_p[len+2] - 65) ) ) % 26 ) + 65 ) ;
			// encrypting the third character in the triple
			c += ( ( ( (key_matrix[2][0] * (modified_p[len] - 65) ) + (key_matrix[2][1] * (modified_p[len+1] - 65) ) + (key_matrix[2][2] * (modified_p[len+2] - 65) ) ) % 26 ) + 65 ) ;
		}
	}

	return c;

}


string Vigenere_Cipher(string p,string keyword, bool mode)
{
	string modified_p = "", c = "", key = "";

	// adjusting the plaintext
		for (size_t len=0; len<p.length(); len++)
		{
			if ( !( (p[len] >= 65 && p[len] <= 90) || (p[len] >= 97 && p[len] <= 122) ) )			// if not alphabet
				continue;
			else
				modified_p += toupper(p[len]);
		}


		if (mode == true)									// generating ciphertext in autokey mode
		{
			for (size_t len=0; len<modified_p.length(); len++)
			{
				if (len < keyword.length())
					key += toupper( keyword[len] );
				else
					key += toupper( modified_p[len- keyword.length() ] );

				c += ( ( ( (modified_p[len] - 65) + (key[len] - 65) ) % 26 ) +65 );
			}

		}

		else												// generating ciphertext in repeating mode
		{
			for (size_t len=0; len<modified_p.length(); len++)
			{
				key += toupper( keyword[len% ( keyword.length() )] );
				c += ( ( ( (modified_p[len] - 65) + (key[len] - 65) ) % 26 ) +65 );
			}
		}

		return c;
}


string Vernam_Cipher(string p,string key)
{
	string modified_p = "", c = "";


	// adjusting the plaintext
	for (size_t len=0; len<p.length(); len++)
	{
		if ( !( (p[len] >= 65 && p[len] <= 90) || (p[len] >= 97 && p[len] <= 122) ) )			// if not alphabet
			continue;
		else
			modified_p += toupper(p[len]);
	}

	for (size_t len=0; len<key.length(); len++)
		key[len] = toupper(key[len]);


	// generating the ciphertext
	for (size_t len=0; len<modified_p.length(); len++)
	{
		c += ( (modified_p[len] - 65) ^ (key[len] -65) ) + 65;
	}

	return c;

}


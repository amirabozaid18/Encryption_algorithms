/*
 * classical_encryption_algorithms.h
 *
 *  Created on: Jan 19, 2021
 *      Author: Amir
 */

#ifndef CLASSICAL_ENCRYPTION_ALGORITHMS_H_
#define CLASSICAL_ENCRYPTION_ALGORITHMS_H_


#include <iostream>
#include <string>
#include <vector>
using namespace std;

string Ceaser_Cipher(string p,int k);
string Playfair_Cipher(string p,string k);
string Hill_Cipher(string p, vector<int> temp_vector);
string Vigenere_Cipher(string p,string keyword, bool mode);
string Vernam_Cipher(string p,string key);



#endif /* CLASSICAL_ENCRYPTION_ALGORITHMS_H_ */

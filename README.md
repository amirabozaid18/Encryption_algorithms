Note that Hill cipher is row major, while AES is column major. 
You do not have to use each of the functions in your main, the ones which you may need are:
Ceaser_Cipher(), Playfair_Cipher(), Hill_Cipher(), Vigenere_Cipher(), Vernam_Cipher(), AES(), and DES().
Note that the mode field in Vigenere_Cipher() is a boolean field, if it's set to true then the mode
is autokey, while if it's set to zero the mode is repetitive.
the key used in AES is 16 bytes, so the number of rounds is 10.
to enter both plaintext and keys once in AES(), search for stringstream since you can not receive 16 bytes in one integer
because the maximum integer size is 8 bytes, so you must receive the plaintext as a string, divide
it into 2 substrings each with 16 hex digits, convert them to integers then fill the 1d array with 16 entries
used as an input to the AES(), do the same exact steps with the key.

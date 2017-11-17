/* This file contains function prototypes and includes for verification functions
*
* Author: Josh McIntyre
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <openssl/sha.h>

#define MAX_INPUT_LENGTH 2048
#define BITS_PER_BYTE 8

int verify_sha256_nonce(const char* message, const char* nonce, const int difficulty_exponent);
int meets_difficulty_target(const unsigned char* hash, const int difficulty_target, const int max_hash_index);
int two_power(const int exponent);

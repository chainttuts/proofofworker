/* This file contains code that verifies a proof of work nonce
* using a cryptographic hash function
*
* Author: Josh McIntyre
*/

#include <crypto.h>

/* This function verifies a proof of work nonce
* The hash_input argument should be the message + nonce (string concatenated)
* It returns 1 if the nonce is valid and 0 if it is not
*/
int verify_sha256_nonce(const char* message, const char* nonce, const int difficulty_exponent)
{
	char buffer[MAX_INPUT_LENGTH];
        strcpy(buffer, "");
	strcat(buffer, message);
	strcat(buffer, nonce);
	unsigned char* input = (unsigned char*) buffer;
	unsigned long input_length = strlen(buffer);

        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX context;

        SHA256_Init(&context);
        SHA256_Update(&context, input, input_length);
        SHA256_Final(hash, &context);

	int is_valid_nonce = meets_difficulty_target(hash, difficulty_exponent, SHA256_DIGEST_LENGTH);

        return is_valid_nonce;
}

/* This function checks whether or not a given hash meets the desired difficulty target
* In other words, the hash (interpreted as an N bit integer) must be less than the difficulty target value
*
* We need to understand a little bit of binary to understand this function
* Take for a example a SHA-256 hash output. This function outputs a 256 bit number
* The numerical value of this hash can be 0 to 2^256-1
*
* However in C, the smallest addressble unit is a byte (8 bits), so we can't just check
* up to the desired bit (ex: 2^255) to see if the hash is less than the target
*
* This function does some math to determine that comparison on a byte array hash format
*/
int meets_difficulty_target(const unsigned char* hash, const int difficulty_exponent, const int max_hash_index)
{
	/* Retrieve the byte array index where the difficulty target is represented
	* The bytes are stored in the hash array in reverse order, so we'll need to "invert"
	* the index based on the max byte (-1, since C uses 0 based array indexing)
	*
	* We then calculate the value of the remaining byte by taking the remainder of
	* N bits / 8 bits per byte, and raising 2 to the power of that remainder
	*/
	int target_byte = difficulty_exponent / BITS_PER_BYTE;
	int target_index = (max_hash_index - 1) - target_byte;
	int target_remainder_exponent = difficulty_exponent % BITS_PER_BYTE;
	int target_remainder = two_power(target_remainder_exponent);

	/* There will be two cases here: Either the remainder exponent is 0 or greater than 0 */
	if (target_remainder_exponent > 0)
	{
		/* First, check that all the preceding bytes before the target byte are 0
		* Otherwise, the hash is greater than the difficulty target
		*/
		int i;
		for (i = 0; i < target_index; i++)
		{
			if (hash[i] != 0)
			{
				return 0;
			}
		}

		/* If all the preceding bytes check out to 0, then we can compare
		* the bits in the target byte with a normal integer comparison
		* If the hash byte's value is less than the target byte, the hash
		* therefore meets the difficulty target
		*/
		if (hash[target_index] < target_remainder)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		/* In the case the difficulty exponent is evenly divisible by 8,
		* just check that the hash at the target byte is 0
		* Therefore, the difficulty target has been met
		*/
		if (hash[target_index] == 0)
		{
			return 1;
		}
	}
}

/* This utility function takes 2 to the specified power
* It does not do negative exponents and assumes a 32 bit integer
* for overflow check
*
* It returns the result if the exponent is valid,
* and returns -1 if it is not
*/
int two_power(const int exponent)
{
	if (exponent < 0 || exponent > 31)
	{
		return -1;
	}

	int result = 1 << exponent;

	return result;
}

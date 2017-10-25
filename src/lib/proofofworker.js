/* This script contains code for executing a proof of work algorithm in a web application
*
* Author: Josh McIntyre
*/

function ProofOfWorker(){

	// Define private variables global to this module
	
		// Define error messages
		const ERROR_BAD_ALGORITHM = "Bad algorithm";
		const ERROR_BAD_DIFFICULTY = "Bad difficulty";
	
	// Define  private functions for this module
	
		// This function verifies that a difficulty is valid for a hashing algorithm
		function isValidExponent(difficultyExponent, maxExponent)
		{
			if (isNaN(difficultyExponent))
			{
				return false;
			}
			if (difficultyExponent > maxExponent || difficultyExponent < 0)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	
		// This function generates a proof of work nonce using SHA-256
		function generateNonceSha256(message, difficultyExponent)
		{
			if (!isValidExponent(difficultyExponent, 256))
			{
				return ERROR_BAD_DIFFICULTY;
			}
			
			var difficultyTarget = 2 ** difficultyExponent;
			var nonce = 0;
			var nonceFound = false;
			while (! nonceFound)
			{
				// Compute the hash for this attempt by concatenating the message and nonce as the input
				var input = message + nonce.toString();
				var hashRaw = CryptoJS.SHA256(input);
				
				// Convert the hash output of CryptoJS' (word array) to an integer for comparison
				var hashHex = hashRaw.toString(CryptoJS.enc.Hex);
				var hashInt = parseInt(hashHex, 16); 
				
				// Check if the hash is less than or equal to the difficulty target
				if (hashInt <= difficultyTarget)
				{
					// If it is, we've found the nonce that serves as our proof of work
					nonceFound = true;
				}
				else
				{
					// If the hash didn't meet the difficulty target, increment the nonce and try again
					nonce++;	
				}
			}
			
			return nonce;
		}

	// Return a public interface to this module
	return {
	
		/* This is the main library function used for generating proof of work
		* The user will specify the hashing algorithm to use (supported: SHA-256),
		* the message to append the nonce to, and the difficult target exponent.
		* For example, for a difficulty target of 2^255 the user should specify 255.
		* This function calls the appropriate worker function and returns the found nonce
		*/
		prove : function prove(algorithm, message, difficultyExponent)
		{
			var nonce;
			if (algorithm == "SHA-256")
			{
				nonce = generateNonceSha256(message, difficultyExponent);
			}
			else
			{
				return ERROR_BAD_ALGORITHM;
			}
			
			return nonce;
		},
	};
	
};

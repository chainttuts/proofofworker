/* This file contains function definitions for server initialization and processing functions
*
* Author: Josh McIntyre
*/
#include <server.h>

/* This function initializes and runs the server on the specified port */
int run_server(const int port)
{
	/* Create server and client address and descriptor variables */
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int server_descriptor;
	int client_descriptor;

	/* Initialize the server socket and gets the resulting descriptor */
	server_descriptor = init_server(&server_address, port);
	if (server_descriptor == SERVER_FAILURE)
	{
		return SERVER_FAILURE;
	}

	/* Loop and accept client connections as they come in */
	while (1)
	{
		/* Initialize the client connection */
		client_descriptor = init_client(server_descriptor, &client_address);
		if (client_descriptor == SERVER_FAILURE)
		{
			return SERVER_FAILURE;
		}

		/* Read the verification request from the socket with recv 
		* A properly formed verification request string has the format 
		* message:nonce:difficulty_exponent:algorithm
		*/
		char verify_request[MAX_VERIFY_REQUEST_SIZE];
		int verify_request_size = recv(client_descriptor, verify_request, MAX_VERIFY_REQUEST_SIZE, 0);

		char result[RESULT_LENGTH];
		process_verify_request(verify_request, result);

		/* Write the confirmation back to the socket and 
		* wait for the client to close the connection
		*/
		write(client_descriptor, result, RESULT_LENGTH);
	}

	/* Close the server socket when finished */
	close(server_descriptor);

	return SERVER_SUCCESS;
}

/* This function initializes a socket the server will need on the specified port */
int init_server(struct sockaddr_in* server_address, const int port)
{
	/* Initialize the socket address structure */
	server_address -> sin_family = AF_INET;
	server_address -> sin_addr.s_addr = INADDR_ANY;
	server_address -> sin_port = htons(port);

	/* Create the socket descriptor and return if there is a failure */
	int server_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (server_descriptor == -1)
	{
		return SERVER_FAILURE;
	}


	/* Bind the address to the socket */
	int bind_result = bind(server_descriptor, (struct sockaddr*) server_address, sizeof(*server_address));
	if (bind_result < 0)
	{
		return SERVER_FAILURE;
	}

	return server_descriptor;
}

/* This function initializes a client connection using listen and accept */
int init_client(int server_descriptor, struct sockaddr_in* client_address)
{
	/* Start listening on the server socket */
	listen(server_descriptor, 3);

	/* Accept the client connection */
	int client_size = sizeof(struct sockaddr_in);
	int client_descriptor = accept(server_descriptor, (struct sockaddr*) client_address, (socklen_t*) &client_size);
	if (client_descriptor < 0)
	{
		return SERVER_FAILURE;
	}

	return client_descriptor;
}

/* This function processes a verification request string received on the socket
* It function calls the appropriate crypto function and verifies a nonce is valid
* It returns a string "true" or "false" that will be written back to the socket
*/
void process_verify_request(const char* verify_request, char* result)
{
	int is_valid_nonce = 0;

	/* First, try to parse the verify request */
	char message[MAX_MESSAGE_SIZE];
	char nonce[MAX_NONCE_SIZE];
	int difficulty_exponent;
	char algorithm[MAX_ALGORITHM_SIZE];

	int parse_result = parse_verify_request(verify_request, message, nonce, &difficulty_exponent, algorithm); 
	if (parse_result == PARSE_FAILURE)
	{
		strcpy(result, "false");
		return;
	}

	/* If the verify request can be processed, call the appropriate verify function */
	if (strstr(algorithm, "SHA-256") != NULL)
	{
		is_valid_nonce = verify_sha256_nonce(message, nonce, difficulty_exponent);
	}

	/* Generate the return string */
	if (is_valid_nonce == 0)
	{
		strcpy(result, "false");
	}
	else
	{
		strcpy(result, "true");
	}
}

/* This function parses a verification request in the format 
* message:nonce:difficulty_exponent:algorithm
* If the request is well formed, this function returns PARSE_SUCCESS
* If there's a problem it returns PARSE_FAILURE
*/
int parse_verify_request(const char* verify_request, char* message, char* nonce, int* difficulty_exponent, char* algorithm)
{
	/* A well formed verification request has the form
	* message:nonce:difficulty_exponenet:algorithm
	*/
	char request_temp[MAX_VERIFY_REQUEST_SIZE];
	strcpy(request_temp, verify_request);

	char* message_token = strtok(request_temp, ":");
	char* nonce_token = strtok(NULL, ":");
	char* difficulty_exponent_string = strtok(NULL, ":");
	char* algorithm_token = strtok(NULL, ":");

	/* If any of the tokens couldn't be processed, the request was not well formed */
	if (message_token == NULL || nonce_token == NULL || difficulty_exponent_string == NULL || algorithm_token == NULL)
	{
		return PARSE_FAILURE;
	}

	/* Copy the parsed tokens into their argument return pointers */
	strcpy(message, message_token);
	strcpy(nonce, nonce_token);
	strcpy(algorithm, algorithm_token);

	/* The difficulty exponent is the only integer, we'll need to try and convert the token with atoi */
	*difficulty_exponent = atoi(difficulty_exponent_string);
	if (*difficulty_exponent == 0)
	{
		return PARSE_FAILURE;
	}

	return PARSE_SUCCESS;
}

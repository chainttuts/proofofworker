/* This file contains function definitions and includes for server initialization and processing functions
*
* Author : Josh McIntyre
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <crypto.h>

#define SERVER_SUCCESS 0
#define SERVER_FAILURE 1
#define MAX_VERIFY_REQUEST_SIZE 3072
#define RESULT_LENGTH 5
#define PARSE_SUCCESS 1
#define PARSE_FAILURE 0
#define MAX_MESSAGE_SIZE 2048
#define MAX_NONCE_SIZE 64
#define MAX_DIFFICULTY_EXPONENT_SIZE 3
#define MAX_ALGORITHM_SIZE 32

int run_server(const int port);
int init_server(struct sockaddr_in* server_address, const int port);
int init_client(int server_descriptor, struct sockaddr_in* client_address);
void process_verify_request(const char* verify_request, char* result);
int parse_verify_request(const char* verify_request, char* message, char* nonce, int* difficulty_exponent, char* algorithm);

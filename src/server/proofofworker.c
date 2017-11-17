/* This file is the main entry point for the ProofOfWorker server program
*
* Author: Josh McIntyre
*/

#include <proofofworker.h>

/* This function is the main entry point for the program */
int main(int argc, char* argv[])
{
        /* This block retrieves command line arguments */
        int port = 0;

        if (argc > 1)
        {
                port = atoi(argv[1]);
        }

        if (port == 0)
        {
                printf("Usage proofofworker [required: <port>]\n");
                exit(EXIT_FAILURE);
        }

	/* This block starts the server */
        int run_result = run_server(port);

	return 0;
}

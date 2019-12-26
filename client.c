#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <string.h>
#include <unistd.h>
#include <netdb.h>

#include <ctype.h>

#define PORT 9006

// Define main error function for errors in the socket connection
void error(const char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]) {

	printf("Welcome to the chat app\nAuthor: Will Ryan\n");

	// Define a buffer to hold 255 characters
	char buffer[255];

	int network_socket, connection_status, transfer, check_exit;

	// Ensure an ip was entered
	if(argc < 1) {
		printf("The commandline arguments were incorrect");
		exit(1);
	}

	// creating a socket
	// format: int network_socket = socket(domain, type, protocol);
	// AF_NET - Constant that specifies that it is an internet socket
	// SOCK_STREAM - Defines type as tcp/connection socket
	// Protocol is 0 because it is TCP and we want the default tcp protocol
	network_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(network_socket < 0) {
		error("Error opening socket");
	}

	// specify an address for the socket
	struct sockaddr_in server_addr;
	struct hostent *server;

	// Use commandline ip to create a host and store it in server
	server = gethostbyname(argv[1]);

	// if server is null host does not exist
	if(server == NULL) {
		printf("Error: no such host\n");
		exit(1);
	}

	// clear the server_addr
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;

	// Port is the constant defined as PORT
	server_addr.sin_port = htons(PORT);
	// Copy the address of the server_addr from the server variable
	bcopy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);

	// 0 = ok -1 = error
	connection_status = connect(network_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

	if (connection_status == -1) {
		error("There was an error in making a connection to the remote socket");
	}
	printf("\n+++++++++++++++ Connected to the server +++++++++++++++\n\n------ Write a message to begin the conversation ------\n\n");

	while(1) {
		// clear the buffer
		bzero(buffer, 255);
		printf("You: ");
		// get message from stdin
		fgets(buffer, 255, stdin);
		transfer = write(network_socket, buffer, strlen(buffer));
		if(transfer < 0) {
			error("Error on writing");
		}
		bzero(buffer, 255);
		// Read the message from the serrver
		transfer = read(network_socket, buffer, 255);
		if(transfer < 0) {
			error("Error on reading");
		}
		printf("%s : %s", argv[1], buffer);
		// exit and close the socket if "quit" is entered
		check_exit = strncmp("quit", buffer, 4);
		if(check_exit == 0) {
			break;
		}
	}

	// close the socket
	close(network_socket);

	return 0;
}
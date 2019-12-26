#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <string.h>
#include <unistd.h>
#include <netdb.h>

#include <ctype.h>
#include <arpa/inet.h>

#define PORT 9006

// Define main error function for errors in the socket connection
void error(const char *msg) {
	perror(msg);
	exit(1);
}

int main() {

	printf("Welcome to the chat app\nAuthor: Will Ryan\n");

	// Create a buffer string that can accept 255 characters
	char buffer[255];

	// Define integer variables
	int server_socket, client_socket, transfer, check_exit;

	// Make a server socket
	// format: int server_socket = socket(domain, type, protocol);
	// AF_NET - Constant that specifies that it is an internet socket
	// SOCK_STREAM - Defines type as tcp/connection socket
	// Protocol is 0 because it is TCP and we want the default tcp protocol
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	// Check if the server_socket was created succesfully
	if(server_socket < 0) {
		error("There was an error creating the Socket");
	}

	// Define the server address
	struct sockaddr_in server_addr, client_addr;
	// use client_addr_length for the length of the client address
	socklen_t client_addr_length;
	// clear the server_addr
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	// INADDR_ANY is equivalent to conneting to 0.0.0.0
	server_addr.sin_addr.s_addr = INADDR_ANY;

	// bind the socket to the ip and port of choice
	if(bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
		error("Binding Error");
	}

	// have the server_socket listen for clients to connect
	// second argument is a backlog - how many connections can be waiting for this socket
	listen(server_socket, 5);

	client_addr_length = sizeof(client_addr);

	printf("\n+++++++++++++ Waiting for a new client connection +++++++++++++\n\n");

	// allow for a two way connection between client and server
	// second arg - the address of the client with a sockaddr structure
	// third arg - client address length
	// will update the client_addr.sin_addr with the address of the client
	client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_length);

	if(client_socket < 0) {
		error("Error trying to accept a new client.");
	}

	printf("--------------- A new client has been accepted! ---------------\n\n");

	while(1) {
		// Clear the buffer
		bzero(buffer, 255);
		// read the information given from the client
		transfer = read(client_socket, buffer, 255);
		if(transfer < 0) {
			error("Error while reading");
		}
		// print the clients message then clear the buffer
		printf("%s : %s", inet_ntoa(client_addr.sin_addr), buffer);
		bzero(buffer, 255);
		printf("You: ");
		fgets(buffer, 255, stdin);
		// After every read write a message
		transfer = write(client_socket, buffer, strlen(buffer));
		if(transfer < 0) {
			error("Error while writing");
		}
		check_exit = strncmp("quit", buffer, 4);
		if(check_exit == 0) {
			break;
		}
	}

	// close the socket
	close(client_socket);
	close(server_socket);

	return 0;
}
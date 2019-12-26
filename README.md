# TCP-Socket-Chat-App
> CLI chat app using tcp socket programming in C

Client - Server application allowing for messaging between computers on port 9006. Both the client and server send messages, and the application is terminated once the client exits, or the server sends 'quit'.

## Compilation

```sh
gcc server.c -o server
gcc client.c -o client
```

## Usage

Server:

```sh
./server
```

Client:

```sh
./client server_ip_address
```

## Example

This is an example of me messaging my own computer.

Server:

```
Wills-MBP:http_server willryan$ ./server
Welcome to the chat app
Author: Will Ryan

+++++++++++++ Waiting for a new client connection +++++++++++++

--------------- A new client has been accepted! ---------------

127.0.0.1 : Hello, this is a message from the client
You: This is a message from the server
127.0.0.1 : Great!
You: quit
Wills-MBP:http_server willryan$
```

Client:

```
Wills-MBP:http_server willryan$ ./client 127.0.0.1
Welcome to the chat app
Author: Will Ryan

+++++++++++++++ Connected to the server +++++++++++++++

------ Write a message to begin the conversation ------

You: Hello, this is a message from the client
127.0.0.1 : This is a message from the server
You: Great!
127.0.0.1 : quit
Wills-MBP:http_server willryan$ 

```

When the computers are different, then the ip's will not be the loopback address, 127.0.0.1.



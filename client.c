// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void removeNewLine(char *message){
	char *pos;
	if ((pos=strchr(message, '\n')) != NULL)
		*pos = '\0';

}

int connectToServer(const char *protocol, int PORT){

	int sock = 0, valread;

	if (!strcmp(protocol, "v4")){

		struct sockaddr_in serv_addr;
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("\n Socket creation error \n");
			exit(1);
		}

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);

		// Convert IPv4 and IPv6 addresses from text to binary
		// form
		if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
			<= 0) {
			printf(
				"\nInvalid address/ Address not supported \n");
			exit(1);
		}

		if (connect(sock, (struct sockaddr*)&serv_addr,
					sizeof(serv_addr))
			< 0) {
			printf("\nConnection Failed \n");
			exit(1);
		}

		return sock;

	}

	else if (!strcmp(protocol, "v6")){

		struct sockaddr_in6 serv_addr;
		if ((sock = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
			printf("\n Socket creation error \n");
			exit(1);
		}

		// struct in_addr serveraddr;
		// talvez usar essa struct para conectar com IPV6

		serv_addr.sin6_family = AF_INET6;
		serv_addr.sin6_port = htons(PORT);
		serv_addr.sin6_addr = in6addr_any;
		
		// Convert IPv4 and IPv6 addresses from text to binary
		// form
		if (inet_pton(AF_INET6, "::1", &serv_addr.sin6_addr)
			<= 0) {
			printf(
				"\nInvalid address/ Address not supported \n");
			exit(1);
		}

		if (connect(sock, (struct sockaddr*)&serv_addr,
					sizeof(serv_addr))
			< 0) {
			printf("\nConnection Failed \n");
			exit(1);
		}

		return sock;
	}
	
	else{
		printf("Protocolo nao reconhecido.\n");
		exit(1);
	}
}


int main(int argc, char const* argv[]){

	const char *protocol = argv[1];
	int PORT = (int)strtol(argv[2], NULL, 10);

	int valread;
	char buffer[1024] = { 0 };

	int sock = connectToServer(protocol, PORT);
	printf("Connected with server.\n");

	char *message;
	size_t characters;
	size_t buff_size = 50;

	while(1){

		message = (char *)malloc(buff_size * sizeof(char));

		characters = getline(&message, &buff_size, stdin);

		removeNewLine(message);

		send(sock, message, characters, 0);

		valread = read(sock, buffer, 1024);
		// printf("%s\n", buffer);
		
		free(message);

	}

	return 0;
}
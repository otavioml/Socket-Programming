#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

//Metodo para estabelecer conexão com servidor
int connectToServer(const char *protocol, int PORT){

	int sock = 0, valread;

	if (!strcmp(protocol, "127.0.0.1")){

		struct sockaddr_in serv_addr;
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("\n Socket creation error \n");
			exit(1);
		}

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);

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

	else if (!strcmp(protocol, "::1")){

		struct sockaddr_in6 serv_addr;
		if ((sock = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
			printf("\n Socket creation error \n");
			exit(1);
		}

		serv_addr.sin6_family = AF_INET6;
		serv_addr.sin6_port = htons(PORT);
		serv_addr.sin6_addr = in6addr_any;

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
		printf("Unknown address.\n");
		exit(1);
	}
}


int main(int argc, char const* argv[]){

	const char *protocol = argv[1];
	int PORT = (int)strtol(argv[2], NULL, 10);


	int valread;
	char buffer[1024] = { 0 };

	int sock = connectToServer(protocol, PORT);

	char *message;
	size_t characters;
	size_t buff_size = 50;


	//loop utilizado para possibilitar o envio de multiplas mensagens
	while(1){

		message = (char *)malloc(buff_size * sizeof(char));
		int count = recv(sock, buffer, buff_size, 0);

		if (count == 0) {
			// Conexão interrompida.
			break;
		}

		characters = getline(&message, &buff_size, stdin);
		send(sock, message, characters, 0);
		
		free(message);

	}
	close (sock);
	free(message);

	return 0;
}
// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

char *equipmentSensors[4][4];

int connectToClient(const char *protocol, int PORT){

	int server_fd, new_socket, opt = 1;

	if (!strcmp(protocol, "v4")){

		struct sockaddr_in address;
		int addrlen = sizeof(address);

		// Creating socket file descriptor
		if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))
			== 0) {
			perror("socket failed");
			exit(EXIT_FAILURE);
		}

		// Forcefully attaching socket to the port 8080
		if (setsockopt(server_fd, SOL_SOCKET,
					SO_REUSEADDR, (char *)&opt,
					sizeof(opt)) < 0) {
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(PORT);

		printf("Waiting client.\n");

		// Forcefully attaching socket to the port 8080
		if (bind(server_fd, (struct sockaddr*)&address,
				sizeof(address))
			< 0) {
			perror("bind failed");
			exit(EXIT_FAILURE);
		}
		if (listen(server_fd, 3) < 0) {
			perror("listen");
			exit(EXIT_FAILURE);
		}
		if ((new_socket
			= accept(server_fd, (struct sockaddr*)&address,
					(socklen_t*)&addrlen))
			< 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		return new_socket;

	}

	else if (!strcmp(protocol, "v6")){

		struct sockaddr_in6 address;
		int addrlen = sizeof(address);

		// Creating socket file descriptor
		if ((server_fd = socket(AF_INET6, SOCK_STREAM, 0))
			== 0) {
			perror("socket failed");
			exit(EXIT_FAILURE);
		}

		// Forcefully attaching socket to the port 8080
		if (setsockopt(server_fd, SOL_SOCKET,
					SO_REUSEADDR, (char *)&opt,
					sizeof(opt)) < 0) {
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}
		address.sin6_family = AF_INET6;
		address.sin6_addr = in6addr_any;
		address.sin6_port = htons(PORT);

		printf("Waiting client.\n");

		// Forcefully attaching socket to the port 8080
		if (bind(server_fd, (struct sockaddr*)&address,
				sizeof(address))
			< 0) {
			perror("bind failed");
			exit(EXIT_FAILURE);
		}
		if (listen(server_fd, 3) < 0) {
			perror("listen");
			exit(EXIT_FAILURE);
		}
		if ((new_socket
			= accept(server_fd, (struct sockaddr*)&address,
					(socklen_t*)&addrlen))
			< 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		return new_socket;

	}


	else{
		printf("Protocolo nao reconhecido.\n");
		exit(1);
	}

}

void performAction(char *command){

	char * action = strtok(command, " ");

	if (!strcmp(action, "add")){

		char *sensors[4];

		char * sensor = strtok(NULL, " ");
		sensor = strtok(NULL, " ");		//removendo string 'sensor' do comando

		int i = 0;

		while((sensor != NULL)){

			if (!strcmp(sensor, "in")) break;

			sensors[i] = sensor;
			i++;

			sensor = strtok(NULL, " ");
		}


		char *equipmentId = strtok(NULL, " ");
		printf("Equipment id: '%s'\n", equipmentId);

		int equipmentIdInt;

		if (!strcmp(equipmentId, "01")) equipmentIdInt = 0;
		if (!strcmp(equipmentId, "02")) equipmentIdInt = 1;
		if (!strcmp(equipmentId, "03")) equipmentIdInt = 2;
		if (!strcmp(equipmentId, "04")) equipmentIdInt = 3;
		
		printf("Sensors: ");
		for (int j = 0; j < i; j++){

			printf("'%s' ", sensors[j]);
			equipmentSensors[equipmentIdInt][j] = sensors[j];
		}
		printf("\n");

		// printf("Sensors in equipment %d: ", equipmentIdInt + 1);
		// for (int j = 0; j < i; j++){

		// 	printf("'%s' ", equipmentSensors[equipmentIdInt][j]);
		// }
		// printf("\n");

	}


// 	while( action != NULL ) {
//       printf( " %s\n", action ); //printing each token
//       action = strtok(NULL, " ");
//    }

// 	// printf("Action: %s\n", action);
// 	printf("Command after strtok: %s\n", command);

}

int main(int argc, char const* argv[]){

	const char *protocol = argv[1];
	int PORT = (int)strtol(argv[2], NULL, 10);


	int valread;
	char* hello = "Hello from server";
	char command[1024] = { 0 };

	int new_socket = connectToClient(protocol, PORT);
	
	printf("Client conected.\n");

	while(1){

		valread = read(new_socket, command, 1024);
		printf("'%s'\n", command);
		send(new_socket, hello, strlen(hello), 0);

		performAction(command);


		printf("Sensors in equipment 3: ");
		for (int j = 0; j < 3; j++){

			printf("'%s' ", equipmentSensors[3][j]);
		}
		printf("\n");


		// printf("Hello message sent\n");

	}

	return 0;
}
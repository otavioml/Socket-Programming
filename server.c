#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>

int socket_, server_fd;

/*Estrutura global para armazenar equipamentos e seus sensores.
As linhas representam o equipamento e as colunas representam sensores. */
bool equipmentSensors[4][4];
int databaseSize = 0;
int databaseLimit = 15;

//Metodo para remover o caractere newline e manter a mensagem limpa 
void removeNewLine(char *message){
	char *pos;
	if ((pos=strchr(message, '\n')) != NULL)
		*pos = '\0';

}

//Metodo para estabelecer conexao com cliente
int connectToClient(const char *protocol, int PORT){

	int opt = 1;

	if (!strcmp(protocol, "v4")){

		struct sockaddr_in address;
		int addrlen = sizeof(address);

		if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))
			== 0) {
			perror("socket failed");
			exit(EXIT_FAILURE);
		}

		if (setsockopt(server_fd, SOL_SOCKET,
					SO_REUSEADDR, (char *)&opt,
					sizeof(opt)) < 0) {
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(PORT);

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
		if ((socket_
			= accept(server_fd, (struct sockaddr*)&address,
					(socklen_t*)&addrlen))
			< 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		return socket_;

	}

	else if (!strcmp(protocol, "v6")){

		struct sockaddr_in6 address;
		int addrlen = sizeof(address);

		if ((server_fd = socket(AF_INET6, SOCK_STREAM, 0))
			== 0) {
			perror("socket failed");
			exit(EXIT_FAILURE);
		}


		if (setsockopt(server_fd, SOL_SOCKET,
					SO_REUSEADDR, (char *)&opt,
					sizeof(opt)) < 0) {
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}
		address.sin6_family = AF_INET6;
		address.sin6_addr = in6addr_any;
		address.sin6_port = htons(PORT);


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
		if ((socket_
			= accept(server_fd, (struct sockaddr*)&address,
					(socklen_t*)&addrlen))
			< 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		return socket_;

	}


	else{
		printf("Unknown protocol.\n");
		exit(EXIT_FAILURE);
	}

}

/*Metodo que retorna o ID do sensor ou equipamento para ser utilizado como
indexador na matriz*/
int getIntId(char *ID){

	if (!strcmp(ID, "01")) return 0;
	else if (!strcmp(ID, "02")) return 1;
	else if (!strcmp(ID, "03")) return 2;
	else if (!strcmp(ID, "04")) return 3;
	else return -1;

}

//Retorna um numero aleatorio
double getRandom(){
	double div = RAND_MAX/10;
	return rand()/div;
}

//Executa os comandos especificados
void performAction(char *command){

	char * action = strtok(command, " ");
	removeNewLine(action);

	if (!strcmp(action, "add")){

		char *sensors[4];

		char * sensor = strtok(NULL, " ");
		sensor = strtok(NULL, " ");		//removendo string 'sensor' do comando
		int countSensorAdded = 0;

		int i = 0;

		//loop para capturar todos os sensores descritos no comando
		while((sensor != NULL)){

			if (!strcmp(sensor, "in")) break;

			sensors[i] = sensor;
			i++;
			countSensorAdded ++;

			sensor = strtok(NULL, " ");
		}

		//for usado para verficar se todos os sensores existem	
		for (int j = 0; j < i; j++){	
			if (getIntId(sensors[j]) < 0){
				printf("invalid sensor\n");
				return ;
			}
		}

		if (databaseSize + countSensorAdded > databaseLimit){
			printf("limit exceeded\n");
			return ;
		}
		else databaseSize += countSensorAdded;


		char *equipmentId = strtok(NULL, " ");
		removeNewLine(equipmentId);
		int equipmentIdInt = getIntId(equipmentId);
		if (equipmentIdInt < 0){
			printf("invalid equipment\n");
			return ;
		}


		char *sensorsAlreadyExists[4];
		int k = 0;
		
		//loop para executar a acao e imprimir os resultados
		printf("sensor ");
		int sensorsAdded = 0;
		for (int j = 0; j < i; j++){

			int sensorId = getIntId(sensors[j]);

			if (equipmentSensors[equipmentIdInt][sensorId]){
				sensorsAlreadyExists[k] = sensors[j];
				k ++;
			}
			else {
				equipmentSensors[equipmentIdInt][sensorId] = true;
				printf("%s ", sensors[j]);
				sensorsAdded ++;
			}
		}
		if (sensorsAdded > 0) printf("added ");
		if (k > 0){
			for (int j = 0; j < k; j++){
				printf("%s ", sensorsAlreadyExists[j]);
			}
			printf("already exists in %s\n", equipmentId);
		}
		else{
			printf("\n");
		}

	}

	else if (!strcmp(action, "list")){

		char *equipmentId = strtok(NULL, " ");
		equipmentId = strtok(NULL, " ");
		equipmentId = strtok(NULL, " ");

		removeNewLine(equipmentId);
		int equipmentIdInt = getIntId(equipmentId);
		if (equipmentIdInt < 0){
			printf("invalid equipment\n");
			return ;
		}

		bool none = true;

		//loop para executar a acao e imprimir os resultados
		for (int i = 0; i < 4; i++){
			if (equipmentSensors[equipmentIdInt][i]){
				printf("0%d ", i+1);
				none = false;
			}
			
		}
		if (none) printf("none\n");
		else printf("\n");

	}

	else if (!strcmp(action, "read")){

		char *sensors[4];

		char *sensor = strtok(NULL, " ");
		int i = 0;

		//loop para capturar todos os sensores descritos no comando
		while((sensor != NULL)){

			if (!strcmp(sensor, "in")) break;

			sensors[i] = sensor;
			i++;

			sensor = strtok(NULL, " ");
		}

		for (int j = 0; j < i; j++){	//for usado para verficar se todos os sensores existem
			if (getIntId(sensors[j]) < 0){
				printf("invalid sensor\n");
				return ;
			}
		}

		char *sensorsNotIntalled[4];
		int countUninstalledSensors = 0;
		int countInstalledSensors = 0;

		char *equipmentId = strtok(NULL, " ");
		removeNewLine(equipmentId);
		int equipmentIdInt = getIntId(equipmentId);
		if (equipmentIdInt < 0){
			printf("invalid equipment\n");
			return ;
		}

		//loop para executar a acao e imprimir os resultados
		for (int j = 0; j < i; j++){
			if (equipmentSensors[equipmentIdInt][getIntId(sensors[j])]){
				printf("%.2lf ", getRandom());
				countInstalledSensors ++;
			} else {
				sensorsNotIntalled[countUninstalledSensors] = sensors[j];
				countUninstalledSensors ++;
			}
		}

		if (countUninstalledSensors > 0){
			if (countInstalledSensors > 0) printf("and ");
			for (int j = 0; j < countUninstalledSensors; j++){
				printf("%s ", sensorsNotIntalled[j]);
			}
			printf("not installed\n");
		}
		else{
			printf("\n");
		}
		
		
	}

	else if (!strcmp(action, "remove")){

		char *sensors[4];

		char * sensor = strtok(NULL, " ");
		sensor = strtok(NULL, " ");		//removendo string 'sensor' do comando

		int i = 0;

		//loop para capturar todos os sensores descritos no comando
		while((sensor != NULL)){

			if (!strcmp(sensor, "in")) break;

			sensors[i] = sensor;
			i++;

			sensor = strtok(NULL, " ");
		}

		//for usado para verficar se todos os sensores existem
		for (int j = 0; j < i; j++){	
			if (getIntId(sensors[j]) < 0){
				printf("invalid sensor\n");
				return ;
			}
		}


		char *equipmentId = strtok(NULL, " ");
		removeNewLine(equipmentId);
		int equipmentIdInt = getIntId(equipmentId);
		if (equipmentIdInt < 0){
			printf("invalid equipment\n");
			return ;
		}

		char *sensorsDoeNotExist[4];
		int k = 0;
		int countRemovedSensors = 0;

		//loop para executar a acao e imprimir os resultados
		printf("sensor ");
		for (int j = 0; j < i; j++){

			int sensorId = getIntId(sensors[j]);

			if (equipmentSensors[equipmentIdInt][sensorId]){
				printf("%s ", sensors[j]);
				countRemovedSensors ++;
				databaseSize --;
				equipmentSensors[equipmentIdInt][sensorId] = false;
			}
			else {
				sensorsDoeNotExist[k]= sensors[j];
				k++;
			}
		}
		if (countRemovedSensors > 0) printf("removed ");
		if (k > 0){
			for (int j = 0; j < k; j++){
				printf("%s ", sensorsDoeNotExist[j]);
			}
			printf("does not exists in %s\n", equipmentId);

		}
		else{
			printf("\n");
		}

	}

	else if (!strcmp(action, "kill")){
		close(server_fd);

		exit(EXIT_SUCCESS);
	}

	else{
		close(server_fd);

		exit(EXIT_SUCCESS);
	}

}

int main(int argc, char const* argv[]){

	const char *protocol = argv[1];
	int PORT = (int)strtol(argv[2], NULL, 10);


	int valread;
	char* hello = "Hello from server";
	char command[1024] = { 0 };

	socket_ = connectToClient(protocol, PORT);

	//loop utilizado para receber multiplas mensagens
	while(1){

		//Metodo send eh necessario para desconectar o cliente
		send(socket_, hello, strlen(hello), 0);

		valread = read(socket_, command, 1024);

		performAction(command);
	}
	
	free(hello);
	
	return 0;
}
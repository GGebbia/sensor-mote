#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dirent.h>

#define PORT 8080
 

//Socket Creation
int CreateSocket(void){

	int network_socket;
	printf("Creating Socket\n");
	network_socket = socket(AF_INET, SOCK_STREAM, 0);
		
	return network_socket;
}

//Bind assigment to the socket
int BindSocket(int network_socket){
 
	int bind_status = -1;
	struct sockaddr_in  address;

	address.sin_family = AF_INET; /* Internet address family --> IPv4 */
	address.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
	address.sin_port = htons(PORT); /* Local port --> 8080*/
	bind_status = bind(network_socket,(struct sockaddr *)&address, sizeof(address));

    return bind_status;
}
 
  
//-------------------------------------------------------------------------------------------------
//------------------------------------------Main Program-------------------------------------------
//-------------------------------------------------------------------------------------------------


int main(int argc , char *argv[]){

	FILE *fp;
	int network_socket , sock , clientLen;
	struct sockaddr_in server ,client;
	char client_message[10]={0};
	char cwd[PATH_MAX];

	//Get current diretory and concatetate it with the output text file
    getcwd(cwd, sizeof(cwd));
    strcat(cwd, "/temperature.txt");

	//Create socket
	network_socket = CreateSocket();
	if (network_socket == -1){
		printf("Unable to create the socket\n");
		return 1;
	}
	printf("Socket created\n");

	//Bind to the socket
	if (BindSocket(network_socket) < 0){
	
		perror("Bind Failed:");
		return 1;
	}
	printf("Bind done\n");

	//Listen
	listen(network_socket , 3);

	//Accept and incoming connection
	while(1){

		printf("Waiting for incoming connections...\n");
		clientLen = sizeof(struct sockaddr_in);

		//accept connection from an incoming client
		sock = accept(network_socket, (struct sockaddr *)&client, (socklen_t*)&clientLen);
		if (sock < 0){
			perror("accept failed");
			return 1;
		}
		printf("Connection accepted\n");
		
		//Receive a reply from the client
		if( recv(sock , client_message , 10 , 0) < 0){
			printf("recv failed");
			break;
		}
		fp = fopen(cwd, "a");
		fprintf(fp, "CPU temperature = %s ºC\n", client_message);
					
		fclose(fp);						
		close(sock);
		sleep(1);
		
	}
	
	return 0;
}

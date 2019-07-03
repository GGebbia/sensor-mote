#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080 
#define MAX 10

float cpu_temperature(void){
	
	// Declaring all variables
	FILE *fp;
	int status, count = 0;
	char cpu_temp[PATH_MAX];
	float avg_temp;

	// Use popen to get all thermal temperatures from all cores
	fp = popen("cat /sys/class/thermal/thermal_zone*/temp", "r");
	if (fp == NULL){
		printf("Unable to obtain CPU temperature\n");
		return 0;
	}
		
	// Sum temperature when it's not zero.
	while (fgets(cpu_temp, PATH_MAX, fp) != NULL){
		if (atof(cpu_temp) != 0.00){
			avg_temp += atof(cpu_temp);
			count++;
		}
	}

	status = pclose(fp);
	if (status == -1) {
		printf("Error during pipe closing.\n");
		exit(0);
	} 

	// Averaging temperature and conveting from millicelsius degrees to celsius degrees.
	avg_temp = avg_temp / (1000 * count);
	
	return avg_temp;
	
}

//Socket Creation
int CreateSocket(void){

	int network_socket;
	printf("Creating Socket\n");
	network_socket = socket(AF_INET, SOCK_STREAM, 0);
		
	return network_socket;
}

// Connection configuration to the socket
int ConnectSocket(int network_socket){
	
	int conn_status = -1;
	struct sockaddr_in server_address;
	
	server_address.sin_family = AF_INET; 
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local Host
    server_address.sin_port = htons(PORT);
    conn_status = connect(network_socket, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in));
	
	return conn_status;
} 

// Send data to the server, then timeout of 10 secs
int SocketSend(int network_socket, char *message, int len_message){

	int send_status = -1;
    struct timeval tv;
    tv.tv_sec = 10;  /* 10 seconds */
    tv.tv_usec = 0;  /* 0 microseconds */

    if(setsockopt(network_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&tv, sizeof(tv)) < 0){
    	printf("Time Out\n");
    	return -1;
    }
    send_status = send(network_socket, message, len_message, 0); 
    return send_status;
}


//-------------------------------------------------------------------------------------------------
//------------------------------------------Main Program-------------------------------------------
//-------------------------------------------------------------------------------------------------


int main(void){

	int  network_socket;
 	float temp;
 	char message[100] = {0};
 	
	//Create Socket
	network_socket = CreateSocket();
	if (network_socket == -1){
		printf("Unable to create the socket\n");
		return 1;
	}
	printf("Socket Created\n");
	
	//Connect to server
	if (ConnectSocket(network_socket) <0){
		printf("Connection failed\n");
		return 1;
	}
	printf("Connected with server\n");
	
	//Store cpu temperature as a string 
	temp = cpu_temperature();
	gcvt(temp, 5, message);
	
	//Send data to server
	if (SocketSend(network_socket, message, strlen(message)) == -1){
		printf("Failed sending CPU temperature\n");
		return 1;
	}
	printf("Message sent to server\n");
	
	//Closing socket file descriptor
	close(network_socket);
	shutdown(network_socket, 2);
	
	return 0;
}


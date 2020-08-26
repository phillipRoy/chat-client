#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const *argv[]) {
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0}; 
    char hello[] = "Hello from server";
	
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	//Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
		printf("\nInavalid address/ Address not supported \n");
		return -1;
	}
	
	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	
	do {
		scanf("%s", hello);
		send(sock, hello, strlen(hello), 0);
	} while(strcmp(hello, "quit") != 0);
	
    return 0; 
}
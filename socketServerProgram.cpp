//Headers are POSIX because Gates doesn't like free (as in freedom) software :/
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <thread>
#include <vector>
#define PORT 8080

void clientHandler(int new_socket, int valread) { 
	do {
		char buffer[1024] = {0};
		valread = read(new_socket, buffer, 1024);
		if(strcmp(buffer, "quit") == 0)
			break; 
		printf("%s\n", buffer);
	} while(true);
} 

int main(int argc, char const *argv[]) {
	int serverfd, new_socket, valread;
	int maxClients = 2;
	int opt = 1;
	std::vector<std::thread> threads;
	struct sockaddr_in address;
	int addrlen = sizeof(address); 
	
	//Creating socket file descriptor
	if((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	//Forcefully attaching socket to port 8080
	if(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR |SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	//Forcefully attaching socket to port 8080
	if(bind(serverfd, (struct sockaddr *)&address, sizeof(address))<0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if(listen(serverfd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	
	for(int i = 0; i < maxClients; ++i) { 
		if((new_socket = accept(serverfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		threads.emplace_back(clientHandler, new_socket, valread);
	}
	
	for(std::thread & t : threads) {
		t.join();
	}
	
	threads.clear();
	
    return 0; 
}
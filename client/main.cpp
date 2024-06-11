// Client side implementation of UDP client-server model 
#include <iostream>
#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <string>

#define PORT	 8080 
#define MAXLINE 1024 

struct ClientData
{
	int nums[2];
};

int main() { 
	ClientData data = {{0,0}}; 

	int sockfd; 
	char buffer[MAXLINE]; 
	const char *hello = "Hello from client"; 
	struct sockaddr_in	 servaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	
	while (true)
	{
		for (size_t i = 0; i < 2; i++)
		{
			std::string input;
			std::cin >> input;
			data.nums[i] = stoi(input);
		}
		
		std::string sendData;
		sendData += std::to_string(data.nums[0]);
		sendData += ",";
		sendData += std::to_string(data.nums[1]);

		sendto(sockfd, sendData.c_str(), sendData.length(), 
			MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
				sizeof(servaddr)); 
		std::cout<<"Hello message sent."<<std::endl; 
			
		int n;
		socklen_t len; 
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
					MSG_WAITALL, (struct sockaddr *) &servaddr, 
					&len); 

		buffer[n] = '\0'; 
		printf("Server : %s\n", buffer);

		if(sendData == "stop")
		{
			close(sockfd); 
			break;
		}
	}
	
	
	return 0; 
}

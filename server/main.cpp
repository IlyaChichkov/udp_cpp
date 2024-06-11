#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#include <vector>

#define PORT 8080
#define MAXLINE 1024

void parce_input(std::string* msg, std::vector<int>* result)
{
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = msg->find(delimiter)) != std::string::npos) {
        token = msg->substr(0, pos);
        result->push_back(stoi(token));
        msg->erase(0, pos + delimiter.length());
    }
    result->push_back(stoi(*msg));
}

int sum(std::vector<int>* arr)
{
    int s = 0;
    for (size_t i = 0; i < arr->size(); i++)
    {
        s += arr->at(i);
    }
    return s;
}

int main()
{
    int sockfd;
    char buffer[MAXLINE];
    const char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if(bind(sockfd, (const struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
    {
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        socklen_t len;
        int n;
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, (char*) buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*) &cliaddr, &len);
        buffer[n] = '\0';
        std::string msg(buffer);
        std::vector<int> arr;
        parce_input(&msg, &arr);
        int s = sum(&arr);

        printf("Client : %s\n", buffer);
        
        std::cout << s<< std::endl;
        std::string response = std::to_string(s);
        std::cout << response << std::endl;
        sendto(sockfd, response.c_str(), response.length(), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
        std::cout << "Hello message sent." << std::endl;

        if(strcmp(buffer, "stop") == 0)
        {
            break;
        }
    }
    

    return 0;
}
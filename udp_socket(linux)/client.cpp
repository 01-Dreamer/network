#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024


int main(int argc,char* argv[])
{
    if(argc < 3)
    {
        std::cout<<"Please enter ip and port!"<<std::endl;
        return -1;
    }


    int client_socket = socket(AF_INET,SOCK_DGRAM,0);
    if(client_socket < 0)
    {
        std::cout<<"fail to create socket"<<std::endl;
        return -1;
    }
    std::cout<<"success to create socket!"<<std::endl;


    int port = atoi(argv[2]);
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof server_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if(inet_pton(AF_INET,argv[1],&server_addr.sin_addr) <= 0)
    {
        perror("invalid server ip address");
        close(client_socket);
        return -1;
    }
    std::cout<<"client is ready to send messages to server!"<<std::endl;


    char buf[BUF_SIZE];
    socklen_t len = sizeof server_addr;
    while(true)
    {
        std::string message;
        std::cout<<"Enter messages>";
        std::getline(std::cin,message);

        sendto(client_socket,message.c_str(),message.size(),0,(struct sockaddr*)&server_addr,sizeof server_addr);

        int n = recvfrom(client_socket,buf,BUF_SIZE-1,0,(struct sockaddr*)&server_addr,&len);
        if(n < 0)
        {
            perror("fail to receive!");
            close(client_socket);
            return -1;
        }
        buf[n]='\0';

        char server_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &server_addr.sin_addr, server_ip, INET_ADDRSTRLEN);
        int server_port = ntohs(server_addr.sin_port);

        std::cout<<server_ip<<':'<<server_port<<'>'<<buf<<std::endl;
    }

    close(client_socket);


    return 0;
}

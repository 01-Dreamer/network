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
    if(argc < 2)
    {
        std::cout<<"Please enter a port!"<<std::endl;
        return -1;
    }


    int server_socket = socket(AF_INET,SOCK_DGRAM,0);
    if(server_socket < 0)
    {
        perror("fail to create socket");
        return -1;
    }
    std::cout<<"success to create socket!"<<std::endl;


    int port = atoi(argv[1]);
    struct sockaddr_in server_addr,client_addr;
    memset(&server_addr,0,sizeof server_addr);
    memset(&client_addr,0,sizeof client_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;


    int st = bind(server_socket,(struct sockaddr*)&server_addr,sizeof server_addr);
    if(st < 0)
    {
        perror("fail to bind");
        close(server_socket);
        return -1;
    }
    std::cout<<"success to bind!"<<std::endl;


    char buf[BUF_SIZE];
    socklen_t len = sizeof client_addr;
    while(true)
    {
        int n = recvfrom(server_socket,buf,BUF_SIZE-1,0,(struct sockaddr*)&client_addr,&len);
        if(n < 0)
        {
            perror("fail to receive!");
            close(server_socket);
            return -1;
        }
        buf[n]='\0';

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        int client_port = ntohs(client_addr.sin_port);

        std::cout<<client_ip<<':'<<client_port<<'>'<<buf<<std::endl;

        std::string response = "server has received your message:";
        response += buf;

        sendto(server_socket,response.c_str(),response.size(),0,(struct sockaddr*)&client_addr,len);
    }

    close(server_socket);


    return 0;
}

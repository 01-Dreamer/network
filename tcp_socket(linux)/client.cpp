#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_BUF_SIZE 1024


int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        std::cout << "Please enter server's ip and port!" << std::endl;
        return 1;
    }
    std::cout << "ip:" << argv[1] << std::endl;
    std::cout << "port:" << argv[2] << std::endl;


    int client_fd = socket(AF_INET,SOCK_STREAM,0);
    if(client_fd < 0)
    {
        std::cout << "Fail to create client socket!" << std::endl;
        return 1;
    }
    std::cout << "Success to create server socket,fd:" << client_fd << std::endl;


    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    if(inet_pton(AF_INET,argv[1],&server_addr.sin_addr) < 0)
    {
        std::cout << "Invalid address or address not supported!" << std::endl;
        close(client_fd);
        return 1;
    }


    if(connect(client_fd,(struct sockaddr*)&server_addr,sizeof server_addr) < 0)
    {
        std::cout << "Fail to connect server!" << std::endl;
        close(client_fd);
        return 1;
    }
    std::cout << "Success to connect server!" << std::endl;


    std::string message;
    char buf[MAX_BUF_SIZE];
    while(true)
    {
        std::cout << "Enter a message:";
        getline(std::cin,message);
        send(client_fd,message.c_str(),message.size(),0);

        int len =recv(client_fd,buf,MAX_BUF_SIZE-1,0);
        if(len > 0)
        {
            buf[len]='\0';
            std::cout << "Receive a message:" << buf << std::endl;
        }
        else if(len == 0)
        {
            std::cout << "Server disconnected!" << std::endl;
            break;
        }
        else
        {
            std::cout << "Recv error!" << std::endl;
            break;
        }
    }

    close(client_fd);


    return 0;
}
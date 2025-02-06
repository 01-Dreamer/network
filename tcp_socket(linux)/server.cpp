#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>

#define MAX_BUF_SIZE 1024


void work(int new_fd)
{
    char buf[MAX_BUF_SIZE];
    while(true)
    {
        int len = recv(new_fd,buf,MAX_BUF_SIZE-1,0);
        if(len > 0)
        {
            buf[len] = '\0';
            std::cout << "Receive a message:" << buf << std::endl;
        }
        else if(len == 0)
        {
            std::cout << "Client disconnected!" << std::endl;
            break;
        }
        else
        {
            std::cout << "Recv error!" << std::endl;
            break;
        }

        const char* message = "I am server!";
        send(new_fd,message,strlen(message),0);
    }
    close(new_fd);
}


int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        std::cout << "Please enter a port!" << std::endl;
        return 1;
    }
    std::cout << "port:" << argv[1] << std::endl;


    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd < 0)
    {
        std::cout << "Fail to create server socket!" << std::endl;
        return 1;
    }
    std::cout << "Success to create server socket,fd:" << server_fd << std::endl;


    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));
    if(bind(server_fd,(struct sockaddr*)&server_addr,sizeof server_addr) < 0)
    {
        std::cout << "Fail to bind!" << std::endl;
        close(server_fd);
        return 1;
    }
    std::cout << "Success to bind!" << std::endl;


    if(listen(server_fd,5) < 0)
    {
        std::cout << "Fail to listen!" << std::endl;
        close(server_fd);
        return 1;
    }
    std::cout << "Success to listen!" << std::endl;


    while(true)
    {
        struct sockaddr_in client_addr;
        memset(&client_addr,0,sizeof client_addr);
        socklen_t addr_len = sizeof client_addr;
        int new_fd = accept(server_fd,(struct sockaddr*)&client_addr,&addr_len);
        if(new_fd < 0)
        {
            std::cout << "Fail to accept!" << std::endl;
            continue;
        }
        std::cout << "Success to accept!" << std::endl;
        std::thread t(work,new_fd);
        t.detach();
    }

    close(server_fd);


    return 0;
}

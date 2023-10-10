#include "server.h"
#include <chrono>


server::server(int port) : port(port)
{
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd == -1) {
        perror("socket");
        return;
    }
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind");
        close(sockfd);
        return;
    }
    
    if (listen(sockfd, 5) == -1) {
        perror("listen");
        close(sockfd);
        return;
    }
    
    cout << "Server is listening on port " << port << "..." << endl;
}

int server::StartRecv()
{
    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    cout<<"Waiting for connection..."<<endl;
    
    
    int client_sockfd = accept(sockfd, (struct sockaddr*)(&clientAddr), &clientLen);
    if (client_sockfd == -1) {
        perror("accept");
        return -1;
    }
    
    cout<<"Accepted connection from "<<inet_ntoa(clientAddr.sin_addr)<<endl;

    char buffer[1000];
    this->recvKB = 0;
    int bytes_received;
    
    cout<<"Receiving data..."<<endl;
    auto start_time = chrono::high_resolution_clock::now();
    
    while((bytes_received = recv(client_sockfd, buffer, 1000, 0)) > 0)
    {
        this->recvKB+=bytes_received/1000.0;
    }
    
    if(bytes_received == -1){
        perror("recv");
        close(client_sockfd);
        this->Close();
        return -1;
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count();
    
    this->Mbps = this->recvKB*8/1000/(duration/1000.0);
    close(client_sockfd);
    
    return 1;
}

void server::Close()
{
    close(sockfd);
}

void server::PrintResult()
{
    cout<<"Received="<<(int)this->recvKB<<" KB"<<" | Rate="<<this->Mbps<<" Mbps"<<endl;
}

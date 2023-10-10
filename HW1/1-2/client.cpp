#include "client.h"


client::client(string serverIP, int port, int time):serverIP(serverIP), port(port), time(time)
{
    cout<<"connecting to "<<serverIP<<":"<<port<<"..."<<endl;
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd == -1) {
        perror("socket");
        return;
    }
    
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, serverIP.c_str(), &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        return;
    }

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        return;
    }
    
    cout<<"Connected"<<endl;
}

int client::StartSend()
{
    cout<<"Sending data..."<<endl;
    auto start_time = chrono::high_resolution_clock::now();
    auto end_time = start_time;
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    char data[1000];
    memset(data, '0', sizeof(data));
    int bytes_sent;
    while(duration < this->time*1000)
    {
        if((bytes_sent = send(sockfd, data, 1000, 0))==-1)
        {
            perror("send");
            this->Close();
            return -1;
        }
        this->sentKB += bytes_sent/1000.0;
        end_time = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    }
    this->Mbps = this->sentKB*8/1000/(duration/1000.0);
    
    return 1;
}

void client::Close()
{
    close(sockfd);
}

void client::PrintResult()
{
    cout<<"Sent = "<<(int)this->sentKB<<" KB | Rate = "<<this->Mbps<<" Mbps"<<endl;
}

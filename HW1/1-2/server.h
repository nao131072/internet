#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctime>
using namespace std;

class server{
public:
    server(int port);
    int sockfd;
    int port;
    double recvKB=0;
    double Mbps=0;
    int StartRecv();
    void Close();
    void PrintResult();
};
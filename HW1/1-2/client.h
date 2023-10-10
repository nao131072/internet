#include <iostream>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctime>
#include <chrono>

using namespace std;

class client{
public:
    client(string serverIP, int port, int time);
    string serverIP="";
    int port=0;
    int time=0;
    int sockfd=0;
    double sentKB=0;
    double Mbps=0;
    int StartSend();
    void Close();
    void PrintResult();
};
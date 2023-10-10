#include <iostream>
#include <stdio.h>        
#include <stdlib.h>          
#include <string.h>          
#include <sys/socket.h>       
#include <sys/types.h>        
#include <arpa/inet.h>       
#include <unistd.h>
#include "client.h"
#include "server.h"

using namespace std;

int main(int argc, char *argv[])
{   
    string mode=argv[1];
    string serverIP="";
    int port=0;
    int time=0;
    
    if(!((mode=="-c" && argc==8)||(mode=="-s" && argc==4)))
    {
        cout<<"Error: missing or additional arguments"<<endl;
        return 0;
    }

    int argCount=2;
    string prefix="";
    while(argCount<argc)
    {
        prefix=argv[argCount];
        if(prefix[0]!='-'){
            argCount++;
            continue;
        }
        if(prefix=="-h") serverIP=argv[argCount+1];
        else if(prefix=="-p") port=atoi(argv[argCount+1]);
        else if(prefix=="-t") time=atoi(argv[argCount+1]);
        argCount++;
    }
    
    if(port<1024 || port>65535)
    {
        cout<<"Error: port number must be in the range 1024 to 65535"<<endl;
        return 0;
    }
    
    if(mode=="-c"){
        client c(serverIP, port, time);
        if(c.StartSend()==1)
            c.PrintResult();
        else
            cout<<"Failed to send data."<<endl;
        c.Close();
    }
    else if(mode=="-s"){
        server s(port);
        if(s.StartRecv()==1)
            s.PrintResult();
        else
            cout<<"Failed to receive data."<<endl;
        s.Close();
    }
}
// email sender
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "base64.hpp"
using namespace std;
using namespace base64;

void receive(int sockfd, bool show=false)
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    if (recv(sockfd, buffer, sizeof(buffer), 0) == -1) {
        
        perror("recv");
        return;
    }
    if(!show) return;
    cout<<buffer<<endl;
}

bool sendSMTP(int sockfd, string msg)
{
    if (send(sockfd, msg.c_str(), msg.length(), 0) == -1) {
        perror("send");
        return 0;
    }
    else return 1;
}

int main()
{
    string serverIP = "140.118.31.60";
    string senderMail = "";
    string receiverMail = "";
    string username = "";
    string password = "";
    string subject = "";
    string content = "";
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int port = 25;
    
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr=inet_addr(serverIP.c_str());
    
    int status = 0;
    status=connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(!sendSMTP(sockfd, "EHLO mail.ntust.edu.tw\r\n")) return -1;
    receive(sockfd);
    
    // HELO
    string ehlo = "HELO mail.ntust.edu.tw \r\n";
    if(!sendSMTP(sockfd, ehlo)) return -1;
    receive(sockfd);
    
    // AUTH LOGIN
    if(!sendSMTP(sockfd, "AUTH LOGIN\r\n")) return -1;
    receive(sockfd);
    
    cout<<"NEED TO LOG IN YOUR NTUST ACOUNT"<<endl;
    cout<<"username: ";
    cin>>username;
    cout<<"password: ";
    cin>>password;
    
    username=to_base64(username);
    password=to_base64(password);
    
    // username
    if(!sendSMTP(sockfd, username+"\r\n")) return -1;
    receive(sockfd, 0);
    
    // password
    if(!sendSMTP(sockfd, password+"\r\n"))
    {
        cout<<"Login failed!"<<endl;
        return -1;
    }
    
    receive(sockfd, 0);
    
    cout<<"Login success!"<<endl;
    
    // MAIL FROM
    cout<<"From: ";
    cin>>senderMail;
    string mailFrom = "MAIL FROM: "+senderMail+"\r\n";
    if(!sendSMTP(sockfd, mailFrom)) return -1;
    receive(sockfd);
    
    // RCPT TO
    cout<<"To: ";
    cin>>receiverMail;
    string rcptTo = "RCPT TO: "+receiverMail+"\r\n";
    if(!sendSMTP(sockfd, rcptTo)) return -1;
    receive(sockfd);
    
    // DATA
    if(!sendSMTP(sockfd, "DATA\r\n")) return -1;
    receive(sockfd);
    
    //from
    if(!sendSMTP(sockfd, "From: "+senderMail+"\r\n")) return -1;
    
    //to
    if(!sendSMTP(sockfd, "To: "+receiverMail+"\r\n")) return -1;
        
    // subject
    cout<<"Subject: ";
    cin>>subject;
    if(!sendSMTP(sockfd, "Subject: "+subject+"\r\n\n")) return -1;
    
    // content
    cout<<"Content: "<<endl;
    cout<<"(end with a single dot)"<<endl;
    string line;
    while(getline(cin, line))
    {
        if(line==".") break;
        content+=line+"\n";
    }
    if(!sendSMTP(sockfd, content+"\r\n.\r\n")) return -1;
    receive(sockfd);
    cout<<"Send success!"<<endl;
    
    // QUIT
    if(!sendSMTP(sockfd, "QUIT\r\n")) return -1;
    receive(sockfd);

    // 關閉套接字
    close(sockfd);
    

    return 0;
}
    
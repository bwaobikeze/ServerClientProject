#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
// Second COSC 3360 assignment for spring 2023
//  Client part
// Brian Waobikeze
// March 2023
#define MSG_CONFIRM 0
using namespace std;
#define MAXLINE 1024
int portno;
struct hostent *server;
char buffer[MAXLINE];
char RECVbuffer[MAXLINE];
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void promptHost()
{
    try
    {
        string hostName = "";
        while (hostName != "localhost")
        {
            cout << "Enter the server hostname; typically 'localhost': ";
            cin >> hostName;
        }
        cout << "Host name: " << hostName << endl; // check host name
        int hostNameLength = hostName.length();
        char hostNameCharArray[hostNameLength + 1];
        strcpy(hostNameCharArray, hostName.c_str());
        server = gethostbyname(hostNameCharArray);
    }
    catch (...)
    {
        error("Error setting host name");
    }
}

void promptPortNo()
{
    try
    {
        cout << "Enter the server portnumber: ";
        cin >> portno;
        cout << "Port number: " << portno << endl;
    }
    catch (...)
    {
        error("Error setting port number");
    }
}

void promptLicensePlate()
{
    try
    { // make sure total client message length is 18
        string licenseNumber;
        string localMessage;
        while (1)
        {
            cout << "Enter License plate number: ";
            cin >> licenseNumber;
            localMessage += licenseNumber;
            break;
        }

//        cout << "Local message " << localMessage << endl;
        int clientMessageLength = localMessage.length();
        char clientMessageCharArray[clientMessageLength + 1];
        strcpy(buffer, localMessage.c_str());
    }
    catch (...)
    {
        error("Error setting Licenses Plate");
    }
}

int main(int argc, char *argv[])
{
    socklen_t len;
    int sockfd, n;
    struct sockaddr_in serv_addr;
    promptHost();
    promptPortNo();

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    memset(&serv_addr, 0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    promptLicensePlate();
//    printf("%s", buffer);
    sendto(sockfd, (const char *)buffer, strlen(buffer),
           MSG_CONFIRM, (const struct sockaddr *) &serv_addr,
           sizeof(serv_addr));

    n=recvfrom(sockfd, (char *)RECVbuffer, MAXLINE,
               MSG_WAITALL, (struct sockaddr *) &serv_addr,
               &len);
    if (n < 0)
        error("ERROR reading from socket");
    RECVbuffer[n] = '\0';
    cout<<RECVbuffer<<endl;
    close(sockfd);
    return 0;
}

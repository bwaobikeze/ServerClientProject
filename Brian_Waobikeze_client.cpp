#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
// Second COSC 3360 assignment for spring 2022
//  Client part
//
// Brian Waobikeze
// November 2021
#define MSG_CONFIRM 0
using namespace std;

int portno;
struct hostent *server;
// char clientMessage[];
char buffer[256];
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
            cout << licenseNumber.size() << endl;
            if (licenseNumber.size() != 9)
//            {
//                cout << "Invalid test id" << endl;
//                continue;
//            }
            localMessage += licenseNumber;
            break;
        }

        cout << "Local message " << localMessage << endl;
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
//    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
//        error("ERROR connecting");
//    bzero(buffer, 256);
    promptLicensePlate();
    sendto(sockfd, (const char *)buffer, strlen(buffer),
           MSG_CONFIRM, (const struct sockaddr *) &serv_addr,
           sizeof(serv_addr));
    // fgets(buffer, 255, stdin); // get input from stdin
    //n = write(sockfd, buffer, strlen(buffer));
//    if (n < 0)
//        error("ERROR writing to socket");
//    bzero(buffer, 256);
    //n = read(sockfd, buffer, 255);
    n=recvfrom(sockfd, (char *)buffer, 255,
               MSG_WAITALL, (struct sockaddr *) &serv_addr,
               &len);
    if (n < 0)
        error("ERROR reading from socket");
    buffer[n] = '\0';
//    if (buffer[0] == 'N')
//        cout << "Your test result was NEGATIVE." << endl;
//    else if (buffer[0] == 'P')
//        cout << "Your test result was POSITIVE" << endl;
//    else
//        cout << "The test result you requested is not in our database." << endl;
    close(sockfd);
    return 0;
}

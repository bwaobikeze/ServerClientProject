#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Second COSC 3360 assignment for fall 2021
// Server part
//
// Brian Waobikeze
// November 2021


using namespace std;

vector<string> allTestResults;
int portno;
char clientResult[1];
int clientMessageLength = 18;
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void printAllTestResults()
{
    if (allTestResults.size() < 1)
        error("All results vector is empty");
    for (int i = 0; i < allTestResults.size(); i++)
    {
        cout << "License # "<< i<< ": "<< allTestResults[i] << endl;
        cout << "==========\n";
    }
}

void formatResults(string line)
{
    vector<string> currentLine;
    allTestResults.push_back(line);
}

void readResults(string inputPath)
{
    fstream fileVar;
    fileVar.open(inputPath);
    if (!fileVar.is_open())
    {
        error("Unable to read results");
    }
    string line;
    while (getline(fileVar, line))
    {
        formatResults(line);
    }
    //printAllTestResults();
}

//void getTestResult(string LicensePlateNum)
//{
//    cout << "getting results for: " << testID << endl;
//    if (LicensePlateNum.size() < 1)
//        return;
//    for (int i = 0; i < allTestResults.size(); i++)
//    {
//
//        if (allTestResults[i]==LicensePlateNum)
//        {
//            cout << "Test ID: " << allTestResults[i].testID << endl;
//            cout << "Birthday: " << allTestResults[i].birthday << endl;
//            cout << "Test result: " << allTestResults[i].result << endl;
//            clientResult[0] = allTestResults[i].result[0];
//        }
//    }
//}

void promptPortNumber()
{
    try
    {
        cout << "Enter the server port number: ";
        cin >> portno;
    }
    catch (...)
    {
        error("Error setting port number");
    }
}

//void getClientResponse(char clientMessage[])
//{
//    try
//    {
//        string testID;
//        string birthday;
//        int splitterIDX = -1;
//        for (int i = 0; i < clientMessageLength;
//             i++)
//        {
//                    testID += clientMessage[i];
//
//        }
//        getTestResult(testID);
//    }
//    catch (...)
//    {
//        cout << "Error attempting to get your result" << endl;
//    }
//}

int main(int argc, char *argv[])
{
    readResults("/Users/brianwaobikeze/Desktop/ServerClientProj2023/read.txt");

    int sockfd, newsockfd, n;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // creates an internet socket of stream type
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr)); // sets all values in buffer  to 0
    // portno = atoi(argv[1]);                       // sets port number for server to listen
    promptPortNumber();
    // configure server  addres struct
    serv_addr.sin_family = AF_INET;         // address family
    serv_addr.sin_addr.s_addr = INADDR_ANY; // host IP address
    serv_addr.sin_port = htons(portno);     // host port number
    if ( ::bind(sockfd, (const struct sockaddr *)&serv_addr,
              sizeof(serv_addr)) < 0 ) {
        // bind address of host and port
        error("ERROR on binding");
    }
//    while (1)
//    {
//        clilen = sizeof(cli_addr);
            socklen_t len;
//        // runs when client connects to server
//        bzero(buffer, 256);
// place recicev from here
//        n = read(newsockfd, buffer, 255);
//        if (n < 0)
//            error("ERROR reading from socket");
//        printf("Here is the message: %s\n", buffer);
//        //getClientResponse(buffer);
// place send to here
//        n = write(newsockfd, clientResult, 18);
//        if (n < 0)
//            error("ERROR writing to socket");
//        close(newsockfd);
//    }
   close(sockfd);
    return 0;
}
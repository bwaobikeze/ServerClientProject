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

// Second COSC 3360 assignment for spring 2022
// Server part
//
// Brian Waobikeze
// November 2021


using namespace std;
#define MAXLINE 1024
vector<string> allTestResults;
int portno;
char clientResult[MAXLINE];

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
        line.erase(line.size()-1);
        formatResults(line);
    }
}

void getTestResult(string LicensePlateNum)
{
    string inputVal = LicensePlateNum;
    cout << "getting results for:" << LicensePlateNum <<endl;
    for (int i = 0; i < allTestResults.size(); i++) {
        if (inputVal == allTestResults.at(i)) {
            string MessageToCLient = LicensePlateNum + ": " + "Reported as stolen ";
            cout << MessageToCLient << endl;
            strcpy(clientResult, MessageToCLient.c_str());
        }
    }
}

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

void getClientResponse(char clientMessage[])
{
    string  LicensePlate;
    try
    {
        for (int i = 0; i <strlen(clientMessage) ;
             i++)
        {
                    LicensePlate += clientMessage[i];

        }
        getTestResult(LicensePlate);
    }
    catch (...)
    {
        cout << "Error attempting to get your result" << endl;
    }
}

int main(int argc, char *argv[]) {
    readResults("/Users/brianwaobikeze/Desktop/ServerClientProj2023/Stolen_cars.txt");
    int sockfd, newsockfd, n;
    socklen_t clilen;
    char buffer[MAXLINE];
    struct sockaddr_in serv_addr, cli_addr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // creates an internet socket of stream type
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr)); // sets all values in buffer  to 0
    promptPortNumber();
    // configure server  addres struct
    serv_addr.sin_family = AF_INET;         // address family
    serv_addr.sin_addr.s_addr = INADDR_ANY; // host IP address
    serv_addr.sin_port = htons(portno);     // host port number
    if (::bind(sockfd, (const struct sockaddr *) &serv_addr,
               sizeof(serv_addr)) < 0) {
        // bind address of host and port
        error("ERROR on binding");
    }
    while (true)
    {
    memset(&cli_addr, 0, sizeof(cli_addr));
    clilen = sizeof(cli_addr);
    //socklen_t len;
    n = recvfrom(sockfd, (char *) buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &cli_addr, &clilen);
    buffer[n] = '\0';
    string checkIfKill;
//    printf("%s", buffer);
        for (int i = 0; i <strlen(buffer) ;
             i++)
        {
            checkIfKill += buffer[i];

        }
    if(checkIfKill=="killsvc"){
        break;
    }
    else{
        getClientResponse(buffer);
    }
    sendto(sockfd, (const char *) clientResult, strlen(clientResult), 0, (const struct sockaddr *) &cli_addr, clilen);
}
    cout<<"Received request to terminate the service bye!!"<<endl;
 close(sockfd);
    return 0;

}
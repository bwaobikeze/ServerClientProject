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

// Second COSC 3360 assignment for spring 2023
// Server part
//
// Brian Waobikeze
// March 2023


using namespace std;
#define MAXLINE 1024
vector<string> allLicensePlateResults;
int portno;
char clientResult[MAXLINE];

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
/**************************************************************************************
 * This Function formatResults() pushes the sting that it is given and pushes it to the
 * allResult vector.
 *************************************************************************************/

void formatResults(string line)
{
    allLicensePlateResults.push_back(line);
}
/**************************************************************************************
 * This Function readResults() gets the input file path and gets each line of the file
 * and checks at the end of the line to see if there is a return character and then passes
 * the string into the formatResult()
 *************************************************************************************/
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
        if(line[line.size()-1]=='\r'){
            line.erase(line.size()-1);
            formatResults(line);
        }
        else{
            formatResults(line);
        }
    }
}
/**************************************************************************************
 * This Function getLicenseResult() checks the License Plate that is passed in by the user
 * input against the llLicensePlateResults vector to see if it is contained in the databse
 *************************************************************************************/

void getLicenseResult(string LicensePlateNum)
{
    string inputVal = LicensePlateNum;
        if (find(allLicensePlateResults.begin(), allLicensePlateResults.end(), inputVal) != allLicensePlateResults.end()) {
            string MessageToCLient = LicensePlateNum + ": " + "Reported as stolen";
            cout << MessageToCLient << endl;
            strcpy(clientResult, MessageToCLient.c_str());
        }
        else{
            string MessageToCLient = LicensePlateNum + ": " + "Is not in Database";
            cout << MessageToCLient << endl;
            strcpy(clientResult, MessageToCLient.c_str());
        }
}
/**************************************************************************************
 * This Function promptPortNumber() prompts the user to enter the path to the database
 * as well as the portnumber they decide to use.
 *************************************************************************************/
void promptPortNumber()
{
    string dataBaseFile;
    try
    {
        cout<<"Enter the relative or absolute file Path of DataBase: ";
        cin>>dataBaseFile;
        readResults(dataBaseFile);
        cout << "Enter the server port number: ";
        cin >> portno;
    }
    catch (...)
    {
        error("Error setting port number or file path");
    }
}
/**************************************************************************************
 * This Function getClientResponse() gets teh clients response in terms of a character array
 * and converts it into a string to compare against the database vector
 *************************************************************************************/
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
        getLicenseResult(LicensePlate);
    }
    catch (...)
    {
        cout << "Error attempting to get your result" << endl;
    }
}
/**************************************************************************************
 * This Function Main() runs the prevous function and creats the socket that wee are
 * going to be using to communcate between the client and server
 *************************************************************************************/

int main(int argc, char *argv[]) {
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
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

struct testResult
{
    string testID;
    string birthday;
    string result;
};

vector<testResult> allTestResults;
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
        cout << "testID: " + allTestResults[i].testID << endl;
        cout << "birthday: " + allTestResults[i].birthday << endl;
        cout << "result: " + allTestResults[i].result << endl;
        cout << "==========\n";
    }
}

void formatResults(string line)
{
    vector<string> currentLine;
    testResult currentTestResult;
    istringstream iss(line);
    string firstSplitter;
    string secondSplitter;
    getline(iss, firstSplitter, '\n');
    istringstream sendStream(firstSplitter);
    while (getline(sendStream, secondSplitter, ' '))
    {
        if (!secondSplitter.empty())
        {
            currentLine.push_back(secondSplitter);
        }
        // clears vector if full
        if (currentLine.size() > 2)
        {
            // fill struct content
            testResult newTestResult;
            newTestResult.testID = currentLine[0];
            newTestResult.birthday = currentLine[1];
            newTestResult.result = currentLine[2];
            allTestResults.push_back(newTestResult);
            currentLine.clear();
        }
    }
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
    printAllTestResults();
}

void getTestResult(string testID, string birthday)
{
    cout << "getting results for: " << testID << ":" << birthday << endl;
    if (testID.size() < 1 || birthday.size() < 1)
        return;
    for (int i = 0; i < allTestResults.size(); i++)
    {

        if (allTestResults[i].testID.compare(testID) == 0)
        {
            cout << "Test ID: " << allTestResults[i].testID << endl;
            cout << "Birthday: " << allTestResults[i].birthday << endl;
            cout << "Test result: " << allTestResults[i].result << endl;
            clientResult[0] = allTestResults[i].result[0];
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
    try
    {
        string testID;
        string birthday;
        int splitterIDX = -1;
        for (int i = 0; i < clientMessageLength;
             i++)
        {
            if (clientMessage[i] != ' ')
            {
                if (clientMessage[i] == ':')
                {
                    splitterIDX = i;
                    continue;
                }
                if (splitterIDX != -1 && i > splitterIDX)
                {
                    birthday += clientMessage[i];
                }
                else
                {
                    testID += clientMessage[i];
                }
            }
        }
        getTestResult(testID, birthday);
    }
    catch (...)
    {
        cout << "Error attempting to get your result" << endl;
    }
}

int main(int argc, char *argv[])
{
    readResults("./testResults20.txt");

    int sockfd, newsockfd, n;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // creates an internet socket of stream type
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr)); // sets all values in buffer  to 0
    // portno = atoi(argv[1]);                       // sets port number for server to listen
    promptPortNumber();
    // configure server  addres struct
    serv_addr.sin_family = AF_INET;         // address family
    serv_addr.sin_addr.s_addr = INADDR_ANY; // host IP address
    serv_addr.sin_port = htons(portno);     // host port number
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0) // bind address of host and port
        error("ERROR on binding");
    listen(sockfd, 5); // listen on socket for connection
    while (1)
    {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd,
                           (struct sockaddr *)&cli_addr,
                           &clilen); // wakes up process when client connects
        if (newsockfd < 0)
            error("ERROR on accept");
        // runs when client connects to server
        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        if (n < 0)
            error("ERROR reading from socket");
        printf("Here is the message: %s\n", buffer);
        getClientResponse(buffer);
        n = write(newsockfd, clientResult, 18);
        if (n < 0)
            error("ERROR writing to socket");
        close(newsockfd);
    }
    close(sockfd);
    return 0;
}
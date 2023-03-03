#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;

#define MAXLINE 1024

int main() {
    //std::cout << "Hello, World!" << std::endl;
    int number;
    char *localHost;
    //string hp;
    int PortNum;
    int socketID;
    struct sockaddr_in servaddr;

    cin>>number;
    cout<<"Please Type In Host Name: "<< endl;
    cin>>localHost;
    cout<<"Please enter A Port Number: "<<endl;
    cin>>PortNum;
    int hp=gethostname(localHost,MAXLINE);

    if((socketID= socket(AF_INET,SOCK_DGRAM,0))<0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr,0,sizeof (servaddr));
    servaddr.sin_family=
            servaddr.sin_port= htons(PortNum);

    //cout<<"Hello Number "<< number<<" Its Great to Have you!"<<endl;

    return 0;
}

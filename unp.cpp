// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 7788
#define MAXLINE 1024


int createsignalserver()
{
    int sockfd;
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));


    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

// Driver code
void informresult(int sockfd)
{
    char buffer[MAXLINE];
    char *message = "[1,1]";
    struct sockaddr_in cliaddr;

    memset(&cliaddr, 0, sizeof(cliaddr));

    for(int i=0;i<6;i++)
    {
        socklen_t n;
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                    &n);
        buffer[n] = '\0';
        printf("Client : %s\n", buffer);

        char clntIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, (struct inaddr *)&cliaddr.sin_addr,clntIP,sizeof(clntIP));

        printf("The source is %s on port %d.\n", clntIP,cliaddr.sin_port);


        send(sockfd,(const char *)message, strlen(message),NULL);

        int a=3;
        sendto(sockfd, (int *)a, 1,
            NULL, (const struct sockaddr *) &cliaddr,
                1);
        printf("%s sent.\n", message);
    }

}

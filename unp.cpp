// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

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

    fcntl(sockfd, F_SETFL, O_NONBLOCK);

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
void informresult(int sockfd, int row, int col)
{
    char buffer[MAXLINE];
    char message[256];
    struct sockaddr_in cliaddr;

    // OpenVibe expect row,col [1-6][1-6].
    sprintf(message,"[%2d,%2d]",row,col-6);

    memset(&cliaddr, 0, sizeof(cliaddr));

    socklen_t n;
    int len;
    while(true)
    {
        len = recvfrom(sockfd, (char *)buffer, MAXLINE,
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                    &n);
        if (len>0)
        {
            buffer[len] = '\0';
            printf("Message Flag : %s\n", buffer);
            if (strcmp(buffer,"1")==0)
                break;
        }
    }

    char clntIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, (struct inaddr *)&cliaddr.sin_addr,clntIP,sizeof(clntIP));

    printf("EEG source is %s on port %d.\n", clntIP,cliaddr.sin_port);

    sleep(2);

    while(true)
    {
        sendto(sockfd, message, strlen(message), 0, (const struct sockaddr *)&cliaddr, sizeof(cliaddr));

        printf("Retrying %s sent.\n", message);

        len = recvfrom(sockfd, (char *)buffer, MAXLINE,
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                    &n);

        if (len>0)
        {
            buffer[len] = '\0';
            printf("Final Message Flag : %s\n", buffer);
            if (strcmp(buffer,"2")==0)
                break;
        }
    }




}

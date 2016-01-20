/*  
 *   * tcpserver.c - A simple TCP latency server 
 *   * usage: tcpserver <port>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFSIZE (1024*1024*8)
#define ITERATION 10000
char *sendbuf = NULL;
char *recvbuf = NULL;

int main(int argc, char **argv) {
    int parentfd; /*  parent socket */
    int childfd; /*  child socket */
    int portno; /*  port to listen on */
    int clientlen; /*  byte size of client's address */
    struct sockaddr_in serveraddr; /*  server's addr */
    struct sockaddr_in clientaddr; /*  client addr */
    //struct hostent *hostp; /*  client host info */
    char *hostaddrp; /*  dotted decimal host addr string */
    //int optval; /*  flag value for setsockopt */
    long n, tot = 0; /*  message byte size */
    
    if (argc != 2) 
    {
        printf("Usage: %s <port>\n", argv[0]);
        return -1;
    }
    portno = atoi(argv[1]);

    parentfd = socket(AF_INET, SOCK_STREAM, 0);
    if (parentfd < 0) 
    {
        printf("Create parent socket error.\n");
        return -1;
    } 
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)portno);
    if(bind(parentfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        printf("Bind server address error.\n");
        return -1;
    }
    if(listen(parentfd, 5) < 0)
    {
        printf("Listen port error.\n");
        return -1;
    }
    clientlen = sizeof(clientaddr);
    childfd = accept(parentfd, (struct sockaddr *)&clientaddr, (socklen_t *)&clientlen);
    if(childfd < 0)
    {
        printf("Accept a wrong connection.\n");
        return -1;
    }
    hostaddrp = inet_ntoa(clientaddr.sin_addr);

    sendbuf = (char *)malloc(MAX_BUFSIZE);
    recvbuf = (char *)malloc(MAX_BUFSIZE);
    if((sendbuf == NULL) || (recvbuf == NULL))
    {
        printf("malloc buffer error.\n");
        return -1;
    }

    //Start testing
    int size = 0;
    int count = 0;
    for(size = 1; size < MAX_BUFSIZE; size *= 2)
    {
        for(count = 0; count < ITERATION; count++)//recv
        {
            tot = 0;
            while(tot < size)
            {
                n = read(childfd, recvbuf + tot, size - tot);
                if(n < 0)
                {
                    printf("iteration[%d]: recv data error.\n", count);
                    return -1;
                }
                tot += n;
            }
            tot = 0;
            while(tot < size)
            {
                n = write(childfd, sendbuf + tot, size - tot);
                if(n < 0)
                {
                    printf("iteration[%d]: send data error.\n", count);
                    return -1;
                }
                tot += n;
            }
        }
        printf("Size[%d] complete.\n", size);
    }

    if(sendbuf)
        free(sendbuf);
    if(recvbuf)
        free(recvbuf);
    return 0;
}

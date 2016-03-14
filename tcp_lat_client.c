/*
 * tcpclient.c - A simple TCP client
 * usage: tcpclient <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <arpa/inet.h>

/* max 8MB buffer to test */
#define MAX_BUFSIZE (1024*1024*8)
#define ITERATION 10000
char* sendbuf = NULL;
char* recvbuf = NULL;

int main(int argc, char** argv) {
    int sockfd, portno;
    long n, tot = 0;
    struct sockaddr_in serveraddr;
    struct hostent* server;
    char* hostname;

    struct timeval start, sub;
    struct timeval end;

    /* check command line arguments */
    if (argc != 3) {
        fprintf(stderr, "usage: %s <hostname/ip> <port>\n", argv[0]);
        exit(0);
    }

    hostname = argv[1];
    portno = atoi(argv[2]);
    sendbuf = (char*)malloc(MAX_BUFSIZE);
    recvbuf = (char*)malloc(MAX_BUFSIZE);

    if ((sendbuf == NULL) || (recvbuf == NULL)) {
        printf("Malloc buffer failed.\n");
        return -1;
    }

    /* socket: create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        printf("ERROR opening socket");
        return -1;
    }

    server = gethostbyname(hostname);

    if (server == NULL) {
        printf("ERROR, no such host as %s\n", hostname);
        return -1;
    }

    bzero((char*) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(portno);
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        printf("Connect to server error.\n");
        return -1;
    }

    int size = 0;
    int count = 0;
    printf("Unidirection tcp latency test\n #Size(B)  #Average Latency(us)\n");

    for (size = 1; size < MAX_BUFSIZE; size *= 2) {
        gettimeofday(&start, NULL);

        for (count = 0; count < ITERATION; count++) {
            //send
            tot = 0;
            sendbuf[0] = (char)(count % 128);

            while (tot < size) {
                n = write(sockfd, sendbuf + tot, size - tot);

                if (n < 0) {
                    printf("iteration[%d]:write socket error.\n", count);
                    return -1;
                }

                tot += n;
            }

            //recv
            tot = 0;

            while (tot < size) {
                n = read(sockfd, recvbuf + tot, size - tot);

                if (n < 0) {
                    printf("iteration[%d]:read socket error.\n", count);
                    return -1;
                }

                tot += n;
            }

            if (recvbuf[0] != (char)(count % 128)) {
                printf("Error");
            }
        }

        gettimeofday(&end, NULL);
        timersub(&end, &start, &sub);
        printf("%-10d \t %-10lf \n", size,
               ((double)(sub.tv_sec * 1000000 + sub.tv_usec)) / (ITERATION * 2));
    }

    close(sockfd);

    if (!sendbuf) {
        free(sendbuf);
    }

    if (!recvbuf) {
        free(recvbuf);
    }

    return 0;
}

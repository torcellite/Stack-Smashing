#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>

#define MAX_CONN 1000
#define BYTES 1024

const char *PAGE_NOT_FOUND = "The website you requested for was not found.\n";

char *ROOT;
int listenfd, clients[MAX_CONN];
int client_no, i;

void start_server(char *port) {
    
    struct addrinfo hints, *res, *p;

    // getaddrinfo for host
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, port, &hints, &res) != 0) {
        perror("getaddrinfo() error");
        exit(1);
    }
    
    // socket and bind
    for (p = res; p!=NULL; p=p->ai_next) {
        listenfd = socket (p->ai_family, p->ai_socktype, 0);
        if (listenfd == -1) continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
    }
    if (p==NULL) {
        perror("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    // Listen for incoming connections
    if (listen(listenfd, 1000000) != 0) {
        perror("listen() error");
        exit(1);
    }
}


// Client connection
void respond(int n) {

     /**
        Stack of function
        ----------------------------------------------------------------------------------------------------------------------------
        | path      | data_to_send | reqline  | msg       | bytes_read | fd      | rcvd    | Slack   | EBP     | RET     | n       |
        | 128 bytes | 1024 bytes   | 12 bytes | 128 bytes | 4 bytes    | 4 bytes | 4 bytes | 8 bytes | 4 bytes | 4 bytes | 4 bytes |
        ----------------------------------------------------------------------------------------------------------------------------
    **/
    
    int rcvd, fd, bytes_read;
    // This is done because n will be corrupted due to the buffer overflow
    // Following uses of n in the function will lead to a segfault
    // Using a global variable keeps it off the stack and a valid value of the index is maintained.
    client_no = n;
    char msg[128], *reqline[3], data_to_send[BYTES], path[128];

    memset((void*)msg, (int)'\0', sizeof(msg));

    // introducing vulnerability for demonstration purposes
    rcvd=recv(clients[client_no], msg, 2048, 0);

    if (rcvd < 0) {
        // receive error
        fprintf(stderr, ("recv() error\n"));
    } else if (rcvd == 0)  {
        // receive socket closed
        fprintf(stderr, "Client disconnected unexpectedly.\n");
    } else  {  
        // message received
        reqline[0] = strtok (msg, " \t\n");
        if (strncmp(reqline[0], "GET\0", 4) == 0) {
            reqline[1] = strtok (NULL, " \t");
            reqline[2] = strtok (NULL, " \t\n");
            if (strncmp(reqline[2], "HTTP/1.0", 8) != 0 && strncmp(reqline[2], "HTTP/1.1", 8) != 0) {
                send(clients[client_no], "HTTP/1.0 400 Bad Request\n", 25, 0);
            }
            else {
                if (strncmp(reqline[1], "/\0", 2) == 0)
                    reqline[1] = "/index.php";        // Default page: /index.php

                strcpy(path, ROOT);
                strcpy(&path[strlen(ROOT)], reqline[1]);
                if ((fd=open(path, O_RDONLY)) != -1) { 
                    // File found
                    printf("Client %d: HTTP/1.0 200 OK\n", client_no);
                    send(clients[client_no], "HTTP/1.0 200 OK\n\n", 17, 0);
                    while ((bytes_read=read(fd, data_to_send, BYTES)) > 0)
                        send(clients[client_no], data_to_send, bytes_read, 0);
                } else {
                    printf("Client %d: HTTP/1.0 404 Not Found\n", client_no);
                    send(clients[client_no], "HTTP/1.0 404 Not Found\n\n", 24, 0);
                    send(clients[client_no], PAGE_NOT_FOUND, strlen(PAGE_NOT_FOUND), 0);
                }
            }
        }
    }

    // Close Socket
    shutdown(clients[client_no], SHUT_RDWR); 
    close(clients[client_no]);
    clients[client_no] = -1;
}

int main(int argc, char *argv[]) {
    
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char c;    
    
    // Default Values PATH = ~/ and PORT=10000
    char PORT[6];
    ROOT = getenv("PWD");
    strcpy(PORT,"10000");

    int slot = 0;

    // Parsing the command line arguments
    while ((c = getopt (argc, argv, "p:r:")) != -1) {
        switch (c) {
            case 'r':
                ROOT = malloc(strlen(optarg));
                strcpy(ROOT, optarg);
                break;
            case 'p':
                strcpy(PORT, optarg);
                break;
            case '?':
                fprintf(stderr,"Invalid arguments.\n");
                exit(1);
            default:
                exit(1);
        }
    }
    printf("Server started at port %s%s%s with root directory set to %s%s%s\n", "\033[92m", PORT, "\033[0m", "\033[92m", ROOT, "\033[0m");
    // Setting all elements to -1: signifies there is no client connected
    int i;
    for (i = 0; i < MAX_CONN; i++) {
        clients[i] = -1;
    }
    start_server(PORT);

    // ACCEPT connections
    while (1) {
        addrlen = sizeof(clientaddr);
        clients[slot] = accept(listenfd, (struct sockaddr *) &clientaddr, &addrlen);

        if (clients[slot] < 0) {
            error ("accept() error");
        } else {
            if (fork() == 0) {
                respond(slot);
                exit(0);
            }
        }
        
        while (clients[slot] != -1) 
            slot = (slot + 1) % MAX_CONN;
    }
    return 0;
}
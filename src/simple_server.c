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

char *ROOT;
int listenfd, clients[MAX_CONN];

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
    for (p = res; p != NULL; p = p->ai_next) {
        listenfd = socket(p->ai_family, p->ai_socktype, 0);
        if (listenfd == -1) continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
    }
    if (p == NULL) {
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
        The stack was constructed using GDB and "stack_decipher.c"
        ----------------------------------------------------------------------------------------------------------------------------
        | path      | data_to_send | reqline  | msg       | bytes_read | fd      | rcvd    | Slack   | EBP     | RET     | n       |
        | 128 bytes | 1024 bytes   | 12 bytes | 128 bytes | 4 bytes    | 4 bytes | 4 bytes | 8 bytes | 4 bytes | 4 bytes | 4 bytes |
        ----------------------------------------------------------------------------------------------------------------------------
    **/
    
    int rcvd, fd, bytes_read;
    char msg[128], *reqline[3], data_to_send[BYTES], path[128];

    memset((void*)msg, (int)'\0', sizeof(msg));

    rcvd = recv(clients[n], msg, sizeof(msg), 0);

    if (rcvd < 0) {
        // receive error
        fprintf(stderr, ("recv() error\n"));
    } else if (rcvd == 0)  {
        // receive socket closed
        fprintf(stderr, "Client disconnected unexpectedly.\n");
    } else  {  
        // message received
        reqline[0] = strtok(msg, " \t\n");
        if (strncmp(reqline[0], "GET\0", 4) == 0) {
            reqline[1] = strtok (NULL, " \t");
            printf("reqline[1]: %s", reqline[1]);
            reqline[2] = strtok (NULL, " \t\n");
            printf("reqline[2]: %s", reqline[2]);
            if (strncmp(reqline[2], "HTTP/1.0", 8) != 0 && strncmp(reqline[2], "HTTP/1.1", 8) != 0) {
                write(clients[n], "HTTP/1.0 400 Bad Request\n", 25);
            }
            else {
                if (strncmp(reqline[1], "/\0", 2) == 0)
                    reqline[1] = "/index.html";        // Default page: /index.html

                strcpy(path, ROOT);
                strcpy(&path[strlen(ROOT)], reqline[1]);
                long *ptr = path + 3108;
                printf("%010p: %010p", ptr, *ptr);
                if ((fd = open(path, O_RDONLY)) != -1) { 
                    // File found
                    send(clients[n], "HTTP/1.0 200 OK\n\n", 17, 0);
                    while ((bytes_read=read(fd, data_to_send, BYTES)) > 0)
                        write(clients[n], data_to_send, bytes_read);
                } else {
                    write(clients[n], "HTTP/1.0 404 Not Found\n", 24); 
                }
            }
        }
    }

    // Close Socket
    shutdown(clients[n], SHUT_RDWR); 
    close(clients[n]);
    clients[n] = -1;
}

int main(int argc, char* argv[]) {
    
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
                fprintf(stderr, "Invalid arguments.\n");
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

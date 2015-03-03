// This program is to learn the stack structure of the respond function of simple_server
#include <stdio.h>
#include <fcntl.h>


#define MAX_CONN 1000
#define BYTES 1024

#define A 0x41

char *ROOT;
int listenfd, clients[MAX_CONN];

// Client connection
void respond(int n, char *egg) {

    /**
        Stack of function
        ------------------------------------------------------------------------------------------------------------------------------
        | path       | data_to_send | reqline  | msg        | bytes_read | fd      | rcvd    | Slack   | EBP     | RET     | n       |
        | 1024 bytes | 1024 bytes   | 12 bytes | 1024 bytes | 4 bytes    | 4 bytes | 4 bytes | 8 bytes | 4 bytes | 4 bytes | 4 bytes |
        ------------------------------------------------------------------------------------------------------------------------------
    **/
    
    int rcvd, fd, bytes_read;
    char msg[1024], *reqline[3], data_to_send[BYTES], path[1024];

    memset((void*)msg, (int)'\0', sizeof(msg));

    memset(msg, A, sizeof(msg)); // fill up message
    memcpy(msg, "GET/ HTTP/1.1", 13); // so that the message is read as a valid attempt to connect
    memcpy(msg, egg, strlen(egg));
    // rcvd = recv(clients[n], msg, sizeof(msg), 0);
    rcvd = 1; // so that the else block is executed

    if (rcvd < 0) {
        // receive error
        fprintf(stderr, ("recv() error\n"));
    } else if (rcvd == 0)  {
        // receive socket closed
        fprintf(stderr, "Client disconnected unexpectedly.\n");
    } else  {  
        // message received
        reqline[0] = strtok(msg, " \t\n");
        /* printf("\n---START of Message---\n");
        int i = 0;
        for (i = 0; i < sizeof(msg); i++)
        	printf("%c", msg[i]);
        printf("\n----END of Message----\n"); */
        if (strncmp(reqline[0], "GET\0", 4) == 0) {
            reqline[1] = strtok (NULL, " \t");
            reqline[2] = strtok (NULL, " \t\n");
            if (strncmp(reqline[2], "HTTP/1.0", 8) != 0 && strncmp(reqline[2], "HTTP/1.1", 8) != 0) {
                write(clients[n], "HTTP/1.0 400 Bad Request\n", 25);
            }
            else {
                if (strncmp(reqline[1], "/\0", 2) == 0)
                    reqline[1] = "/index.html";        // Default page: /index.html

                strcpy(path, ROOT);
                strcpy(&path[strlen(ROOT)], reqline[1]);
                printf("file: %s\n", path);

                if ((fd=open(path, O_RDONLY)) != -1) { 
                    // File found
                    // send(clients[n], "HTTP/1.0 200 OK\n\n", 17, 0);
                    while ((bytes_read=read(fd, data_to_send, BYTES)) > 0)
                    	printf ("Data to send: %s\n", data_to_send);
                       // write(clients[n], data_to_send, bytes_read);
                } else {
                	printf("HTTP/1.0 404 Not Found\n");
                    // write(clients[n], "HTTP/1.0 404 Not Found\n", 23); 
                }
            }
        }
    }

    // Close Socket
    // shutdown(clients[n], SHUT_RDWR); 
    // close(clients[n]);
    // clients[n] = -1;
}

int main(int argc, char **argv) {

	respond(1, argv[1]);

	return 0;

}
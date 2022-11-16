#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>

#define BUF 1024
#define PORT 6543

int abortRequested = 0;
int create_socket = -1;
int new_socket = -1;

enum mailCommand { Quit = 0, Send = 1, List = 2, Read = 3, Del = 4, Unknown = 5};
/**Functions*/
void signalHandler(int sig);
void clientCommunication(void *data);
void initServerSocket();
int receiveClientCommand(int *current_socket, char *buffer);
int respondToClient(int *current_socket, char *message);

/**TWMailer Protocol Functions*/
enum mailCommand getMailCommand(char *buffer);
int sendMail(char *sender, char *receiver, char * subject, char *message);
int listMails(char *username, char *buffer); //read directory /var/mail/USERNAME and send output to client
int readMail(char *username, int number); // read file from /var/mail/USERNAME/FILENUMBERFROMLIST and send output to client
int deleteMail(char *username, int number); // del file /var/mail/USERNAME/FILENUMBERFROMLIST

int main (int argc, char **argv) {
    socklen_t addrlen;
    struct sockaddr_in address, cliaddress;
    int reuseValue = 1;

    ///hm ka wofür wir das brauchen
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        perror("signal can not be registered");
        return EXIT_FAILURE;
    }

    /// Create socket
    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket error"); // errno set by socket()
        return EXIT_FAILURE;
    }

    /// Set Socket Options
    if (setsockopt(create_socket, SOL_SOCKET, SO_REUSEADDR, &reuseValue, sizeof(reuseValue)) == -1) {
        perror("set socket options - reuseAddr");
        return EXIT_FAILURE;
    }

    if (setsockopt(create_socket, SOL_SOCKET, SO_REUSEPORT, &reuseValue, sizeof(reuseValue)) == -1) {
        perror("set socket options - reusePort");
        return EXIT_FAILURE;
    }

    /// Init Address
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);  /// notwork byte order => big endian

    /// Assign Address and Port
    if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) == -1) {
        perror("bind error");
        return EXIT_FAILURE;
    }

    /// listen for connections
    if (listen(create_socket, 5) == -1) {
        perror("listen error");
        return EXIT_FAILURE;
    }

    while (!abortRequested) {
        printf("Waiting for connections...\n");

        addrlen = sizeof(struct sockaddr_in);

        ///accept connection
        if ((new_socket = accept(create_socket, (struct sockaddr *) &cliaddress,&addrlen)) == -1) {
            if (abortRequested) {
                perror("accept error after aborted");
            } else {
                perror("accept error");
            }
            break;
        }

        printf("Client connected from %s:%d...\n", inet_ntoa(cliaddress.sin_addr), ntohs(cliaddress.sin_port));
        clientCommunication(&new_socket); // returnValue can be ignored
        new_socket = -1;
    }

    // frees the descriptor
    if (create_socket != -1) {
        if (shutdown(create_socket, SHUT_RDWR) == -1) {
            perror("shutdown create_socket");
        }
        if (close(create_socket) == -1) {
            perror("close create_socket");
        }
        create_socket = -1;
    }

    return EXIT_SUCCESS;
}

void clientCommunication(void *data) {
    char buffer[BUF];
    int *current_socket = (int *) data;
    char username[9];

    strcpy(buffer, "Welcome to myserver!\r\nPlease enter your commands...\r\n");
    if (send(*current_socket, buffer, strlen(buffer), 0) == -1) {
        perror("send failed");
        return;
    }

    do {
        receiveClientCommand(current_socket, buffer);
        ///compare command strings
        printf("%s", buffer);
        enum mailCommand cmd = getMailCommand(buffer);

        switch (cmd) {
            case List:
                printf("\ntest case list\n");
                respondToClient(current_socket, "Enter username");
                receiveClientCommand(current_socket, buffer);
                printf(buffer);
                listMails(username, buffer);
                break;
            case Unknown:
                respondToClient(current_socket, "Unknown command\n");
                break;

        }

    } while (strcmp(buffer, "quit") != 0 && !abortRequested);

    // closes/frees the descriptor if not already
    if (*current_socket != -1) {
        if (shutdown(*current_socket, SHUT_RDWR) == -1) {
            perror("shutdown new_socket");
        }
        if (close(*current_socket) == -1) {
            perror("close new_socket");
        }
        *current_socket = -1;
    }
}

enum mailCommand getMailCommand(char *buffer) {
    if (strcmp(buffer,"LIST") == 0) {
        return List;
    } else {
        return Unknown;
    }
}

int listMails(char *username, char *buffer) {
    DIR *dir;
    struct dirent *ent;
    char path[256] = "/var/mail/";
    strcat(path, username);
    buffer[0] = '\0';
    char tmpBuffer[256];
    if ((dir = opendir(path)) != NULL) {
        /* print all the files and directories within directory */
        for (int i = 1; (ent = readdir (dir)) != NULL; i++) {
            sprintf(tmpBuffer, "%d %s\n", i, ent->d_name);
            strcat(buffer, tmpBuffer);
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("brbr");
        return EXIT_FAILURE;
    }
}

int receiveClientCommand(int *current_socket, char *buffer) {
    int size;
    size = recv(*current_socket, buffer, BUF - 1, 0);
    if (size == -1) {
        if (abortRequested) {
            perror("recv error after aborted");
        } else {
            perror("recv error");
        }
        printf("test1\n");
        return -1;
    }

    if (size == 0) {
        printf("Client closed remote socket\n"); // ignore error
        return -1;
    }

    // remove ugly debug message, because of the sent newline of client
    if (buffer[size - 2] == '\r' && buffer[size - 1] == '\n') {
        size -= 2;
    } else if (buffer[size - 1] == '\n') {
        --size;
    }

    buffer[size] = '\0';

    printf("Message received: %s\n", buffer); // ignore error
}

int respondToClient(int *current_socket, char *message) {
    if (send(*current_socket, message, strlen(message), 0) == -1) {
        perror("send answer failed");
        return -1;
    }
}

void signalHandler(int sig) {
    if (sig == SIGINT) {
        printf("abort Requested... "); /// ignore error
        abortRequested = 1;

        if (new_socket != -1) {
            if (shutdown(new_socket, SHUT_RDWR) == -1) {
                perror("shutdown new_socket");
            }
            if (close(new_socket) == -1) {
                perror("close new_socket");
            }
            new_socket = -1;
        }

        if (create_socket != -1) {
            if (shutdown(create_socket, SHUT_RDWR) == -1) {
                perror("shutdown create_socket");
            }
            if (close(create_socket) == -1) {
                perror("close create_socket");
            }
            create_socket = -1;
        }
    } else {
        exit(sig);
    }
}
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
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
char *receiveClientCommand(int *current_socket, char *buffer);
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
                respondToClient(current_socket, "Enter username");
                strcpy(username, receiveClientCommand(current_socket, buffer));
                if (listMails(username, buffer)) {
                    respondToClient(current_socket, buffer);
                } else {
                    respondToClient(current_socket, "Unknown Username");
                }
                break;

            case Send:

                char receiver[256];
                char subject[256];
                char message[256];
                
                respondToClient(current_socket, "Enter your username");
                strcpy(username, receiveClientCommand(current_socket, buffer));
                //////die vielleicht gleich in die Send funktion/// TODO
                respondToClient(current_socket, "Enter Receiver");              //RECEIVER             
                strcpy(receiver, receiveClientCommand(current_socket, buffer));
                 
                respondToClient(current_socket, "Enter subject (80 char max)"); //SUBJECT
                strcpy(subject, receiveClientCommand(current_socket, buffer));

                respondToClient(current_socket, "Enter your message");          //MESSAGE
                strcpy(message, receiveClientCommand(current_socket, buffer));

                respondToClient(current_socket, "Enter '.' to send");           //PUNKT ???
                if(strcmp(receiveClientCommand(current_socket, buffer), ".") == 0){
                    sendMail(username, receiver, subject, message);
                }
                else{
                    respondToClient("Your mail was not send");
                }

                break;

            case Read:
                respondToClient(current_socket, "Enter username");

                respondToClient(current_socket, "Enter message no.");

                break;

            case Del:
                respondToClient(current_socket, "Enter username");

                respondToClient(current_socket, "Enter message no.");

                break;

            case Quit:
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
    } 
    else if(strcmp(buffer,"SEND") == 0){
        return Send;
    }
    else if(strcmp(buffer,"READ") == 0){
        return Read;
    }
    else if(strcmp(buffer,"DEL") == 0){
        return Del;
    }
    else if(strcmp(buffer,"QUIT") == 0){
        return Quit;
    }
    else {
        return Unknown;
    }
}

int listMails(char *username, char *buffer) {
    DIR *dir;
    struct dirent *ent;
    char path[256] = "/var/mail/";
    strcat(path, username);
    strcat(path, "/in");
    buffer[0] = '\0';
    char tmpBuffer[256];

    ///check if directory(=maiilbox) exists
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        ///gibts oder gibts nid
        return 0;
        //mkdir(path, 0700);
    }

    if ((dir = opendir(path)) != NULL) {
        //
        /* print all the files and directories within directory */
        for (int i = 1; (ent = readdir (dir)) != NULL;) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }
            sprintf(tmpBuffer, "%d %s\n", i, ent->d_name);
            strcat(buffer, tmpBuffer);
            i++;
        }
        closedir (dir);
        return 1;
    } else {
        /* could not open directory */
        perror ("brbr");
        return EXIT_FAILURE;
    }
}

int sendMail(char *sender, char *receiver, char * subject, char *message){
    printf("TODO: implement sendMail()\n");
}

int readMail(char *username, int number){
    printf("TODO: implement readMail()\n");

}

int deleteMail(char *username, int number){
     printf("TODO: implement deleteMail()\n");
}

char *receiveClientCommand(int *current_socket, char *buffer) {

    int size;
    size = recv(*current_socket, buffer, BUF - 1, 0);
    if (size == -1) {
        if (abortRequested) {
            perror("recv error after aborted");
        } else {
            perror("recv error");
        }
        printf("test1\n");
    }

    if (size == 0) {
        printf("Client closed remote socket\n"); // ignore error
    }

    // remove ugly debug message, because of the sent newline of client
    if (buffer[size - 2] == '\r' && buffer[size - 1] == '\n') {
        size -= 2;
    } else if (buffer[size - 1] == '\n') {
        --size;
    }

    buffer[size] = '\0';
    printf("Message received: %s\n", buffer); // ignore error
    return buffer;
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
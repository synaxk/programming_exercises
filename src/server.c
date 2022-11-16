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

enum mailCommand {
    Quit = 0, Send = 1, List = 2, Read = 3, Del = 4, Unknown = 5
};

/**Functions*/
void signalHandler(int sig);

void clientCommunication(void *data);

void initServerSocket();

char *receiveClientCommand(int *current_socket, char *buffer);

int respondToClient(int *current_socket, char *message);

/**TWMailer Protocol Functions*/
enum mailCommand getMailCommand(char *buffer);

int sendMail(int *current_socket, char *buffer);

int listMails(char *username, char *buffer); //read directory /var/mail/USERNAME and send output to client
int readMail(char *username, char *number, char *listBuffer); // read file from /var/mail/USERNAME/FILENUMBERFROMLIST and send output to client
int deleteMail(char *username, char *number); // del file /var/mail/USERNAME/FILENUMBERFROMLIST
char *getFileFromList(char *listBuffer, char *number, char *fileName);
char* buildMessageParts(char* completeMessage, char* title, char *input);
int writeToInbox(char *receiver, char *completeMessage, char* subject);

int main(int argc, char **argv) {
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
        if ((new_socket = accept(create_socket, (struct sockaddr *) &cliaddress, &addrlen)) == -1) {
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
    char listBuffer[BUF];
    char filename[256];
    int number = -1;

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
                sendMail(current_socket, buffer); //return abfragen

                break;

            case Read:
                respondToClient(current_socket, "Enter username");
                strcpy(username, receiveClientCommand(current_socket, buffer));
                respondToClient(current_socket, "Enter message no.");
                getFileFromList(listBuffer, receiveClientCommand(current_socket, buffer), filename);
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
    if (strcmp(buffer, "LIST") == 0) {
        return List;
    } else if (strcmp(buffer, "SEND") == 0) {
        return Send;
    } else if (strcmp(buffer, "READ") == 0) {
        return Read;
    } else if (strcmp(buffer, "DEL") == 0) {
        return Del;
    } else if (strcmp(buffer, "QUIT") == 0) {
        return Quit;
    } else {
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
        for (int i = 1; (ent = readdir(dir)) != NULL;) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }
            sprintf(tmpBuffer, "%d %s\n", i, ent->d_name);
            strcat(buffer, tmpBuffer);
            i++;
        }
        closedir(dir);
        return 1;
    } else {
        /* could not open directory */
        perror("brbr");
        return EXIT_FAILURE;
    }
}

int sendMail(int *current_socket, char *buffer) {
    char completeMessage[BUF];
    completeMessage[0] = '\0';
    char sender[256];
    char receiver[256];
    char subject[256];
    char message[256];


    respondToClient(current_socket, "Enter your username");          //SENDER
    sprintf(sender, receiveClientCommand(current_socket, buffer));
    buildMessageParts(completeMessage, "Sender: ", sender);
    
    respondToClient(current_socket, "Enter Receiver");              //RECEIVER          
    sprintf(receiver, receiveClientCommand(current_socket, buffer));
    buildMessageParts(completeMessage, "Receiver: ", receiver);
    
    respondToClient(current_socket, "Enter subject (80 char max)"); //SUBJECT
    sprintf(subject, receiveClientCommand(current_socket, buffer));
    buildMessageParts(completeMessage, "Subject: ", subject);

    respondToClient(current_socket, "Enter your message");          //MESSAGE
    sprintf(message, receiveClientCommand(current_socket, buffer));
    buildMessageParts(completeMessage, "Message:\n", message);


    respondToClient(current_socket, "Enter '.' to send");           //PUNKT ???
    //printf("%s", completeMessage); ///TEST
    if (strcmp(receiveClientCommand(current_socket, buffer), ".") == 0) {
        //respondToClient(current_socket, "OK"); //TEST////////////////////////// message output
        fflush(stdout);
        printf("before writeToInbox");
        fflush(stdout);
         respondToClient(current_socket, "Your mail was send");
       /* if (writeToInbox(receiver, completeMessage, subject)) {
            return 1;
        } else {
            return 0;
        }*/
    } else {
        printf("in else");
        respondToClient(current_socket, "Your mail was not send");
        return 0;
        
    }


    return 1;
}

char* buildMessageParts(char* completeMessage, char* title, char *input){
    strcat(completeMessage, title);
    strcat(completeMessage, input);
    strcat(completeMessage, "\n");

}

int writeToInbox(char *receiver, char *completeMessage, char*subject){
    printf("in writeToInbox()"); ///TEST
    //making path to inbox of receiver
    char path[256] = "/var/mail/";
    strcat(path, receiver);
    strcat(path, "/in/");

   //check if directory(=mailbox) exists
    struct stat st = {0};

    if (stat(path, &st) == -1) {
        printf("invalid path in 'writeToInbox");
        return 0;
        //directory gibts nicht
    }

    //concat for filename
    strcat(path, subject);

    //write to directory

    FILE* mail;
    mail = fopen(path, "w+");
    fprintf(mail, completeMessage);
    fclose(mail);

}

int readMail(char *username, char *number, char *listBuffer) {
    FILE *mail;
    char filepath[256];

    sprintf(filepath, "/var/mail/%s/");
    return 0;
}


int deleteMail(char *username, char *number) {
    printf("TODO: implement deleteMail()\n");
    return 0;
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

char *getFileFromList(char *listBuffer, char *number, char *filename) {
    char *lineBuffer = (char *) malloc(sizeof(char) * 256);
    size_t numLen = strlen(number);

    for (int i = 1, j = 0; listBuffer[i] != '\0'; i++) {
        ///read current line into lineBuffer
        lineBuffer[j] = listBuffer[i];
        ///at newline, check if we need that line, else reset counter
        if (listBuffer[i] == '\n') {
            /// compare first byte of string
            if (strncmp(number, lineBuffer, numLen) == 0) {
                lineBuffer += numLen;
                strcpy(filename, lineBuffer);
                return filename;
            }
        }
    }
}
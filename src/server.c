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
#include <ldap.h>
#include <wait.h>
#include <time.h>


#define BUF 1024
#define PORT 6543

int abortRequested = 0;
int create_socket = -1;
int new_socket = -1;

enum mailCommand {
    Quit = 0, Send = 1, List = 2, Read = 3, Del = 4, Unknown = 5, Login = 6
};

/**Functions*/
void signalHandler(int sig);

void clientCommunication(void *data, struct in_addr clientIP);

char *receiveClientCommand(const int *current_socket, char *buffer);

int respondToClient(const int *current_socket, char *message);

int initServerSocket();


/**get user credentials and send auth request*/
int getCredLogin(char *username, int *current_socket);

/**send authentication request to ldap server*/
int authRequest(char *username, char *password);

/**parse command from client*/
enum mailCommand getMailCommand(char *buffer);

/**Get Mail-params from client, build and distribute the message*/
int sendMail(int *current_socket, char *username, char *buffer);

/**read directory /var/mail/USERNAME/in and send output to client*/
int listMails(char *username, char *buffer);

/**read file from /var/mail/USERNAME/in/ and send output to client*/
char *readMail(char *username, char *filename, char *buffer);

/**delete file /var/mail/USERNAME/in/FILENAME*/
int deleteMail(char *username, char *filename);

/**select filename from '\n' seperated list of filenames and trim leading number*/
void getFileFromList(const char *listBuffer, char *number, char *buffer);

/**Construct message and write file (name = subject) to /var/mail/USERNAME/*/
int writeToInAndOutBox(char *username, char *completeMessage, char *subject, char *inOrOut);

int checkBlacklist(struct in_addr clientIP);

int addToBlackList(struct in_addr clientIP);

/// 127.0.0.1

int main(int argc, char **argv) {
    socklen_t addrLen;
    struct sockaddr_in cliAddress;
    int pid = 1;

    ///signal listener
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        perror("signal can not be registered");
        return EXIT_FAILURE;
    }
    /// initialize the server listening socket
    if (initServerSocket()) {
        perror("Failed to init server socket.");
        return EXIT_FAILURE;
    }

    /// listen for connections
    if (listen(create_socket, 5) == -1) {
        perror("listen error");
        return EXIT_FAILURE;
    }

    /// accept connections todo: forking
    while (!abortRequested) {
        printf("Waiting for connections...\n");

        addrLen = sizeof(struct sockaddr_in);

        ///accept connection
        if ((new_socket = accept(create_socket, (struct sockaddr *) &cliAddress, &addrLen)) == -1) {
            if (abortRequested) {
                perror("accept error after aborted");
            } else {
                perror("accept error");
            }
            break;
        }

        if (checkBlacklist(cliAddress.sin_addr) == 0) {
            /**child process handles client communication*/
            if ((pid = fork()) == 0) {
                printf("Client connected from %s:%d...\n", inet_ntoa(cliAddress.sin_addr), ntohs(cliAddress.sin_port));
                clientCommunication(&new_socket, cliAddress.sin_addr); // returnValue can be ignored
                new_socket = -1;
            }

        } else {
            /// shutdown new socket -> client was blacklisted
            if (new_socket != -1) {
                if (shutdown(new_socket, SHUT_RDWR) == -1) {
                    perror("shutdown new_socket");
                }
                if (close(new_socket) == -1) {
                    perror("close new_socket");
                }
                new_socket = -1;
            }
        }


       // while(wait(NULL)); ///wait for child process

    }

    /// frees the descriptor
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


int initServerSocket() {
    struct sockaddr_in address;
    int reuseValue = 1;

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
    return EXIT_SUCCESS;
}

void clientCommunication(void *data, struct in_addr clientIP) {
    char buffer[BUF];
    int *current_socket = (int *) data;
    char username[9];
    char listBuffer[BUF];
    char message[BUF];
    char filename[256];
    int login = -1;

    strcpy(buffer, "Welcome to myserver!\r\nPlease enter your commands...\r\n");
    if (send(*current_socket, buffer, strlen(buffer), 0) == -1) {
        perror("send failed");
        return;
    }

    do {
        receiveClientCommand(current_socket, buffer);
        ///compare command strings
        enum mailCommand cmd = getMailCommand(buffer);
        if (login != 0) {
            if (cmd != Login) {
                respondToClient(current_socket, "You need to login first\n");
                continue;
            }
            login = getCredLogin(username, current_socket);
            switch (login) {
                case 0:
                    respondToClient(current_socket, "Login success");
                    continue;
                case 1:
                case 2:
                    respondToClient(current_socket, "Try again...");
                    continue; /// einfach weiter machen, falls nicht eingeloggt wird nochmal versucht
                case 3:
                default:
                    respondToClient(current_socket, "You are blacklisted for 1 min...");
                    addToBlackList(clientIP);
                    abortRequested = 1;
                    ///blacklist client ip
                    break;
            }
        }
        switch (cmd) {
            case List:
                if (listMails(username, buffer)) {
                    strcpy(listBuffer, buffer);
                    respondToClient(current_socket, buffer);
                } else {
                    respondToClient(current_socket, "Unknown Username");
                }
                break;

            case Send:
                sendMail(current_socket, username, buffer); //return abfragen
                break;

            case Read:

                respondToClient(current_socket, "Enter message no.");
                getFileFromList(listBuffer, receiveClientCommand(current_socket, buffer), filename);

                printf("Current Buffer: %s\n", filename);
                strcpy(message, readMail(username, filename, buffer));

                respondToClient(current_socket, message);
                break;
            case Del:
                respondToClient(current_socket, "Enter message no.");
                deleteMail(username, filename);
                break;
            case Unknown:
                respondToClient(current_socket, "Unknown command\n");
                break;
            case Login:
                respondToClient(current_socket, "Already logged in");
                break;
            case Quit:
            default:
                break;
        }


    } while (strcmp(buffer, "QUIT") != 0 && !abortRequested);

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
    } else if (strcmp(buffer, "LOGIN") == 0) {
        return Login;
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

int sendMail(int *current_socket, char *username, char *buffer) {
    char completeMessage[BUF];
    completeMessage[0] = '\0';
    char sender[256];
    char receiver[256];
    char subject[256];
    char message[256];

    /** get input from client for Message */
    respondToClient(current_socket, "Enter Receiver");              //RECEIVER       
    strcpy(receiver, receiveClientCommand(current_socket, buffer));

    respondToClient(current_socket, "Enter subject (80 char max)"); //SUBJECT
    strcpy(subject, receiveClientCommand(current_socket, buffer));

    respondToClient(current_socket, "Enter your message");          //MESSAGE
    strcpy(message, receiveClientCommand(current_socket, buffer));

    respondToClient(current_socket, "Enter '.' to send");           //PUNKT ???

    if (strcmp(receiveClientCommand(current_socket, buffer), ".") == 0) {
        //format mail
        sprintf(completeMessage, "Sender: %s\nReceiver: %s\nSubject: %s\nMessage: %s\n",
                sender, receiver, subject, message);

        //write to inbox of receiver and to outbox of sender
        if (writeToInAndOutBox(receiver, completeMessage, subject, "/in/")
            && writeToInAndOutBox(sender, completeMessage, subject, "/out/")) {

            respondToClient(current_socket, "Mail sent");
            return 1;

        } else {
            respondToClient(current_socket, "Your mail could not be sent");
            return 0;

        }
    } else {
        respondToClient(current_socket, "Your mail was not sent");
        return 0;

    }
}

int writeToInAndOutBox(char *username, char *completeMessage, char *subject, char *inOrOut) {
    fflush(stdout);
    //making path to inbox of receiver
    char path[256] = "/var/mail/";
    strcat(path, username);
    strcat(path, inOrOut);

    //check if directory(=mailbox) exists
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        printf("invalid path in\n");
        return 0;
    }

    //concat for filename + .txt extension
    strcat(path, subject);
    strcat(path, ".txt");

    //write to directory
    FILE *mail;
    mail = fopen(path, "w+");
    fprintf(mail, "%s", completeMessage);
    fclose(mail);
    return 1;
}

char *readMail(char *username, char *filename, char *buffer) {
    FILE *mail = NULL;
    char filepath[256];
    char testBuffer[200];
    sprintf(filepath, "/var/mail/%s/in/%s", username, filename);
    printf("%s\n", filepath);

    strcpy(testBuffer, filepath);

    mail = fopen((char *) filepath, "r");

    fgets(buffer, 200, mail);

    return buffer;
}

int deleteMail(char *username, char *filename) {
    char filepath[256];
    sprintf(filepath, "/var/mail/%s/in/%s", username, filename);

    remove(filepath);
    return 0;
}

char *receiveClientCommand(const int *current_socket, char *buffer) {
    int size;
    size = (int) recv(*current_socket, buffer, BUF - 1, 0);
    if (size == -1) {
        if (abortRequested) {
            perror("recv error after aborted");
        } else {
            perror("recv error");
        }
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

int respondToClient(const int *current_socket, char *message) {
    if (send(*current_socket, message, strlen(message), 0) == -1) {
        perror("send answer failed");
        return -1;
    }
    return 0;
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


/// select a file from a '\n' seperated list and trim the leading number
void getFileFromList(const char *listBuffer, char *number, char *buffer) {
    char *lineBuffer = (char *) malloc(sizeof(char) * 256);
    size_t numLen = strlen(number);

    for (int i = 0, j = 0; listBuffer[i] != '\0'; i++) {
        ///read current line into lineBuffer
        lineBuffer[j++] = listBuffer[i];
        ///at newline, check if we need that line, else reset counter
        if (listBuffer[i] == '\n') {
            /// compare first byte of string
            if (strncmp(number, lineBuffer, numLen) == 0) {
                lineBuffer[j - 1] = '\0';
                lineBuffer += (numLen + 1);
                strcpy(buffer, lineBuffer);
                fflush(stdout);
            }
            j = 0;
        }
    }

}

int authRequest(char *username, char *password) {
    ///setup ldap connection
    const int ldapVersion = LDAP_VERSION3;
    const char *ldapUri = "ldap://ldap.technikum-wien.at";
    char userDN[256];
    LDAP *ldapHandle;

    /// hardcoded pw
    if (strcmp(password, "tudi") == 0) {
        return 1;
    }

    sprintf(userDN, "uid=%s,ou=people,dc=technikum-wien,dc=at", username);

    if (ldap_initialize(&ldapHandle, ldapUri) != LDAP_SUCCESS) {
        perror("ldap init failed\n");
        return 0;
    }

    if (ldap_set_option(ldapHandle, LDAP_OPT_PROTOCOL_VERSION, &ldapVersion) != LDAP_OPT_SUCCESS) {
        perror("ldap opt failed\n");
        ldap_unbind_ext_s(ldapHandle, NULL, NULL);
        return 0;
    }

    if (ldap_start_tls_s(ldapHandle, NULL, NULL) != LDAP_SUCCESS) {
        perror("ldap start tls failed");
        ldap_unbind_ext_s(ldapHandle, NULL, NULL);
        return 0;
    }

    BerValue bindCredentials;
    bindCredentials.bv_val = (char *) password;
    bindCredentials.bv_len = strlen(password);
    BerValue *serverCredp; // server's credentials

    if (ldap_sasl_bind_s(ldapHandle,
                         userDN,
                         LDAP_SASL_SIMPLE,
                         &bindCredentials,
                         NULL,
                         NULL,
                         &serverCredp) != LDAP_SUCCESS) {
        perror("ldap bind error\n");
        ldap_unbind_ext_s(ldapHandle, NULL, NULL);
        return 0;
    }
    return 1;
}

int getCredLogin(char *username, int *current_socket) {
    static int attempt = 0;
    char password[20];
    ///get username
    respondToClient(current_socket, "Enter username");
    strcpy(username, receiveClientCommand(current_socket, username));
    ///get password
    respondToClient(current_socket, "Enter password");
    strcpy(password, receiveClientCommand(current_socket, password));
    if (authRequest(username, password)) {
        return 0;
    }
    return ++attempt;
}

int checkBlacklist(struct in_addr clientIP) {
    FILE *blackListFile = NULL;
    DIR *dir;
    char fileName[64] = ".blacklist/";
    struct dirent *ent;
    struct in_addr addr;
    struct stat fileStat;
    time_t raw_time;

    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    strcat(cwd, "/.blacklist");
    if ((dir = opendir(cwd)) != NULL) {
        /* print all the files and directories within directory */
        for (; (ent = readdir(dir)) != NULL;) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }
            if (strcmp(inet_ntoa(clientIP), ent->d_name) == 0) {
                strcat(fileName, ent->d_name);
                printf("Filename: %s\n", fileName);
                if (stat(fileName, &fileStat) == 0) {
                    printf("Last modified: %ld\n", *((const time_t *)&fileStat.st_mtim));
                    printf("local time: %ld\n", time(&raw_time) + 60);
                    if ((time(&raw_time) - *((const time_t *)&fileStat.st_mtim)) < 60) {
                        printf("Client IP is currently blacklisted.\n");
                        return 1;
                    }
                }
            }
        }
        closedir(dir);
    }
    return 0;
}

int addToBlackList(struct in_addr clientIP) {
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    strcat(cwd, "/.blacklist/");
    strcat(cwd, inet_ntoa(clientIP));
    FILE *blackListEntry = fopen(cwd, "w+");
    fclose(blackListEntry);

}
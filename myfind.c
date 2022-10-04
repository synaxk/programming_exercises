#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>

struct Conf {
    int recursive;
    int caseInsensitive;
};

int searchFile(char *filename, char *path, struct Conf *config);
void printUsage();

int main(int argc, char*argv[]) {
    struct Conf config = {.recursive = 0, .caseInsensitive = 0};

    for (int opt = 0; (opt = getopt(argc, argv, "hiR:")) != -1;) {
        switch(opt) {
            case '?':
            case 'h':
                printf("Help...\n");
                break;
            case 'i':
                config.caseInsensitive = 1;
                break;
            case 'R':
                config.recursive = 1;
                break;
            default:
                break;
        }
    }
    //init search path directory
    DIR *d;

    searchFile("myfind", ".", &config);

    printf("Hello, World!\n");
    return 0;
}

int searchFile(char *filename, char *path, struct Conf *config) {
    //open dir and search file
    DIR *directory;
    struct dirent *dirent;
    pid_t pid;

    if (getpid() != -1) {
        pid = fork();
    }

    if ((directory = opendir(path)) == NULL) {
        return 2;
    }

    while ((dirent = readdir(directory)) != NULL) {
        if (config->caseInsensitive) {
            if (dirent->d_type == 8 && strcasecmp(filename, dirent->d_name) == 0) {
                //juhu
                return 0;
            }
        } else {
            if (dirent->d_type == 8 && strcmp(filename, dirent->d_name) == 0) {
                //juhu case sensitive
            }
        }

        printf("%s\t%d\n", dirent->d_name, dirent->d_type);

       // if (dirent->d_type == 4 && !(strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0)) {
        //    searchFile(filename, strcat(path, dirent->d_name));
    //    }
        //if found - exit
        if (dirent->d_type == 8 && strcmp(filename, dirent->d_name) == 0) {
            printf("brtbrt");
            closedir(directory);
            // output the file
            return 0;
        }
    }
    closedir(directory);
    return 1;
}


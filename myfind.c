#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdlib.h>

struct Conf {
    int recursive;
    int caseInsensitive;
};

int searchFile(char *filename, char *path, struct Conf *config);
void printUsage();

int main(int argc, char *argv[]) {
    int status = 1;
    /**options for searchFile Function*/
    struct Conf config = {.recursive = 0, .caseInsensitive = 0};

    for (int opt = 0; (opt = getopt(argc, argv, "hiR")) != EOF;) {
        switch (opt) {
            case 'h':
                printUsage();
                break;
            case 'i':
                config.caseInsensitive = 1;
                break;
            case 'R':
                config.recursive = 1;
                break;
            default:
                return 2;
        }
    }

    /**check for empty argument list*/
    if (optind == 1 && argc == 1) {
        printUsage();
        return 0;
    }

    /**take next argument as path*/
    char *path = argv[optind];

    /**loop through filenames, fork and search*/
    for (int i = ++optind; i < argc; i++) {
        if (fork() == 0) {
            searchFile(argv[i], path, &config);
            exit(0);
        }
    }

    ///TODO: check status
    /**wait for the child processes*/
    while (wait(&status) > 0);

    return 0;
}

int searchFile(char *filename, char *path, struct Conf *config) {
    /***/
    DIR *directory;
    struct dirent *dirent;
    pid_t pid;
    char wd[PATH_MAX + 1];

    /**exit if directory can't be opened */
    if ((directory = opendir(path)) == NULL) {
        return 2;
    }

    /**get working directory, exit on error*/
    if (getcwd(wd, sizeof(wd)) == NULL) {
        return 2;
    }

    /**get current pid*/
    pid = getpid();

    while ((dirent = readdir(directory)) != NULL) {
        if (config->recursive == 1) {
            if (dirent->d_type == 4 && !(strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0)) {
                /**declare new path, so recursive calls don't change the parent path*/
                char *newPath = (char *) malloc(sizeof(wd));
                strcpy(newPath, path);

                /**append a '/' to form a valid path*/
                if (*(newPath + (int) strlen(newPath) - 1) != '/') {
                    strncat(newPath, "/", 1);
                }
                searchFile(filename, strncat(newPath, dirent->d_name, sizeof(dirent->d_name)), config);
            }
        }

        /**check if dirent is a file*/
        if (dirent->d_type == 8) {
            if (config->caseInsensitive) {
                if (strcasecmp(filename, dirent->d_name) == 0) {
                    printf("%d: %s: %s/%s/%s\n", pid, filename, wd, path, dirent->d_name);
                    return 1;
                }
            }

            if (strcmp(filename, dirent->d_name) == 0) {
                printf("%d: %s: %s/%s\n", pid, filename, wd, dirent->d_name);
                return 1;
            }
        }
    }
    return 0;
}

void printUsage() {
    printf("Usage:\n\t./myfind [-i] [-R] SEARCHPATH FILENAME1 [FILENAME2]\n");
}

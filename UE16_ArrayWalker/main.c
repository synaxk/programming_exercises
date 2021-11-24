#include <stdio.h>

int moveDown(int *posX, int *posY, char field[10][10]);

int moveUp(int *posX, int *posY, char field[10][10]);

int moveLeft(int *posX, int *posY, char field[10][10]);

int moveRight(int *posX, int *posY, char field[10][10]);

void printField(char field[10][10]);

void drawPath(const int *steps, const int *posX, const int *posY, char field[10][10]);

char getInput();

int main() {

    char field[10][10];
    char input;
    int steps = 0;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            field[i][j] = '.';
        }
    }

    int posX = 0, posY = 0;

    drawPath(&steps, &posX, &posY, field);
    printField(field);
    while ((input = getInput()) != 'x') {
        if (input == '\n') {
            continue;
        }
        switch (input) {
            case 'n':
                if (moveUp(&posX, &posY, field)) {
                    steps++;
                    drawPath(&steps, &posX, &posY, field);
                } else {
                    printf("not possible\n");
                    continue;
                }
                break;
            case 'e':
                if (moveRight(&posX, &posY, field)) {
                    steps++;
                    drawPath(&steps, &posX, &posY, field);
                } else {
                    printf("not possible\n");
                    continue;
                }
                break;
            case 's':
                if (moveDown(&posX, &posY, field)) {
                    steps++;

                    drawPath(&steps, &posX, &posY, field);
                } else {
                    printf("not possible\n");
                    continue;
                }
                break;
            case 'w':
                if (moveLeft(&posX, &posY, field)) {
                    steps++;
                    drawPath(&steps, &posX, &posY, field);
                } else {
                    printf("not possible\n");
                    continue;
                }
                break;
            default:

                printf("not possible\n");
                continue;
        }
        printField(field);
    }

    return 0;
}

int moveDown(int *posX, int *posY, char field[10][10]) {
    // if not last line and x position in next line has not been visited yet
    if (*posY <= 9 && field[*posY + 1][*posX] == '.') {
        (*posY)++;
        return 1;
    } else {
        return 0;
    }
}


int moveUp(int *posX, int *posY, char field[10][10]) {
    if (*posY >= 1 && field[*posY - 1][*posX] == '.') {
        (*posY)--;
        return 1;
    } else {
        return 0;
    }
}


int moveLeft(int *posX, int *posY, char field[10][10]) {
    if (*posX >= 1 && field[*posY][*posX - 1] == '.') {
        (*posX)--;
        return 1;
    } else {
        return 0;
    }
}

int moveRight(int *posX, int *posY, char field[10][10]) {
    if (*posX <= 9 && field[*posY][*posX + 1] == '.') {
        (*posX)++;
        return 1;
    } else {
        return 0;
    }
}

void printField(char field[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            putchar(field[i][j]);
        }
        putchar('\n');
    }
}


char getInput() {
    char c = 0;
    printf("walk to:\n");
    do {
        scanf("%c", &c);
    } while (c == '\n');
    return c;
}

void drawPath(const int *steps, const int *posX, const int *posY, char field[10][10]) {
    field[*posY][*posX] = (char) ('A' + (*steps % 26));
}

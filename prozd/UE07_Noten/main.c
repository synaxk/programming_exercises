#include <stdio.h>

#define QUANTITY 3 // Number of grades

int getGrades(int *grades);

int evaluate(const int *grades); /*pointer to const int -> value at address cannot be changed */

int main() {
    int grades[QUANTITY];
    int err = 0;

    printf("Noten: ");

    err = getGrades(grades);
    if (err)
        return 1;

    evaluate(grades);

    return 0;
}

/*get 3 (or QUANTITY) grades, check for correct amount and form */
int getGrades(int *grades) {
    int grade, c = 0;
    char *errGrade;
    int i = 0;

    for (; (c = getchar()) != '\n'; i++) {
        /*return if input is longer than QUANTITY*/
        if (i > QUANTITY - 1) {
            printf("Erwarte genau %d Noten!\n", QUANTITY);
            return 1;
        }
        /*get the numeric value of c*/
        grade = c - '0';
        /*Check if one of the grades != 1-5*/
        if (grade < 1 || grade > 5) {
            switch (i) {
                case 0:
                    errGrade = "Erste";
                    break;
                case 1:
                    errGrade = "Zweite";
                    break;
                case 2:
                    errGrade = "Dritte";
                    break;
                default:
                    errGrade = "Nte";
                    break;
            }
            printf("%s Note muss zwischen 1 und 5 liegen!\n", errGrade);
            return 1;
        }
        grades[i] = grade;
    }
    if (i < QUANTITY) {
        printf("Erwarte genau %d Noten!\n", QUANTITY);
        return 1; //input too long
    }
    return 0;
}

/*Evaluate average of grades*/
int evaluate(const int *grades) {
    int avg = 0;

    for (int i = 0; i < QUANTITY; i++) {
        avg += grades[i];
        if (grades[i] == 5) {
            printf("Nicht bestanden!\n");
            return 1;
        }
    }

    if (avg == 3) {
        printf("Ausgezeichneter Erfolg!\n");
    } else if (avg == 4) {
        printf("Guter Erfolg!\n");
    } else {
        printf("Bestanden.\n");
    }
    return 0;
}
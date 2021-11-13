#include <stdio.h>

int main (){
    int n1;
    int n2;
    char opr;
    int max;
    int min;
    char opt;

    printf("\nNumber:");
    scanf("%d", &n1);
    max = n1;       //initialize max as n1
    min = n1;       //initialize min as n1
    do{             //do-while loop for operator != '='
        printf("\nOperator: ");
        scanf(" %c", &opr);

        if (opr == '='){
            printf("\nFinal result: %d\nLargest: %d\nSmallest: %d", n1, max, min);
            break;
        }
        while(opr != '+' && opr != '-' && opr != '*' && opr != '=' && opr != '/'){
            printf("\nUnknown operator.");
            printf("\nOperator: ");
            scanf(" %c", &opr);
            if (opr == '='){
                printf("\nFinal result: %d\nLargest: %d\nSmallest: %d", n1, max, min);
            }
        }
        if (opr != '='){        //if-statement is needed in case of '=' after an unknown operator
            printf("\nNumber: ");
            scanf(" %d", &n2);
        }
        if (opr == '+'){
            printf("\nResult: %d", n1 + n2);
            n1 = n1 + n2;       //sets value of n1 to new result
            if (n1 > max){      //asks if n1 should be set as new maximum
                max = n1;
            }
            if (n1 < min){
                min = n1;
            }
        }
        if (opr == '-'){
            printf("\nResult: %d", n1 - n2);
            n1 = n1 - n2;
            if (n1 > max){
                max = n1;
            }
            if (n1 < min){
                min = n1;
            }
        }
        if (opr == '*'){
            printf("\nResult: %d", n1 * n2);
            n1 = n1 * n2;
            if (n1 > max){
                max = n1;
            }
            if (n1 < min){
                min = n1;
            }
        }
        if (opr == '/'){
            if (n2 == 0){
                printf("\nDivision by 0.\nNo new result.");
                continue;
            }
            if (n1%n2 != 0){
                printf("\nDivision has remainder %d. Round (r), cut off (c), or discard (d)?", n1%n2);
                printf("\nOption: ");
                scanf(" %c", &opt);

                while (opt != 'r' && opt != 'c' && opt != 'd'){
                    printf("\nUnknown option.");
                    printf("\nOption: ");
                    scanf(" %c", &opt);
                }
                if (opt == 'r'){
                    if ((n1%n2)*2 < n2){
                        n1 = n1 /n2;
                        printf("\nResult: %d", n1);
                        if (n1 > max){
                            max = n1;
                        }
                        if (n1 < min){
                            min = n1;
                        }
                        continue;
                    }
                    if ((n1%n2)*2 > n2){
                        n1 = n1 / n2;
                        ++n1;
                        printf("\nResult: %d", n1);
                    }
                    if (n1 > max){
                        max = n1;
                    }
                    if (n1 < min){
                        min = n1;
                    }
                }
                if (opt == 'c'){
                    printf("\nResult: %d", n1 / n2);
                    n1 = n1 / n2;
                    if (n1 > max){
                        max = n1;
                    }
                    if (n1 < min){
                        min = n1;
                    }
                }
                if (opt == 'd'){
                    printf("\nNo new result.");
                    continue;
                }
                continue;
            }

            printf("\nResult: %d", n1 / n2);
            n1 = n1 / n2;
            if (n1 > max){
                max = n1;
            }
            if (n1 < min){
                min = n1;
            }
        }
    }
    while (opr != '=');
    return 0;
}
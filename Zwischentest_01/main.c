#include <stdio.h>
#include <string.h>

/*Get Input*/
int getNumber();

/*string arguments are used to switch between operators and options and set valid options*/
char getOperator(char *type, char *opts);

/*Basic Math operations - a, b are integers*/
int add(int a, int b);
int sub(int a, int b);
/*char pointer as 3rd argument to tell main what to do in case of division with remainder*/
int div(int a, int b, char *option);
int mul(int a, int b);

int main() {
    /*result and number -> storage for input and results*/
    /*numberOfPositive, numberOfNegative are counters for results*/
    int result = 0, number = 0, numberOfPositive = 0, numberOfNegative = 0;
    /*operations: +,-,*,/   options: r, c, d*/
    char operation = 0, option = 0;

    /*The first number ist stored in result to avoid multiplication with 0 etc.*/
    result = getNumber();

    /*initialize operation with input operator and repeat if not '='*/
    for (; (operation = getOperator("perator", "+-*/=")) != '=';) {
        /*read the 2nd number for the calculation*/
        number = getNumber();
        switch (operation) {
            case '+':
                result = add(result, number);
                break;
            case '-':
                result = sub(result, number);
                break;
            case '*':
                result = mul(result, number);
                break;
            case '/':
                /*Check for valid divisor to avoid division by 0*/
                if (number == 0) {
                    printf("\nDivision by 0.\nNo new result.");
                    /*Invalid operation, continue to next iteration(read new operator)*/
                    continue;
                }
                /*call the div function with both numbers and the address of the option variable (pointer)
                 * to store the input-option for further exceptions i.e. d... discard*/
                result = div(result, number, &option);
                /*Check if Division with remainder was discarded and continue to skip result-increment*/
                if (option == 'd') {
                    continue;
                }
                break;
            default:
                break;
        }
        /*increment counters for positive or negative results*/
        result >= 0 ? numberOfPositive++ : numberOfNegative++;
    }
    /*print results*/
    printf("\nFinal result: %d", result);
    printf("\nNumber of positive: %d", numberOfPositive);
    printf("\nNumber of negative: %d", numberOfNegative);
    return 0;
}

/*get Input */
/*print input type, read, store und return the input to the calling function*/
int getNumber() {
    int num = 0;
    printf("\nNumber: ");
    scanf(" %d", &num);
    return num;
}

/* string parameter type is used to switch between operators and options
 * opts is used to define valid options*/
char getOperator(char *type, char *opts) {
    int optsLength = 0, valid = 0;
    char op = 0;
    /*print capital O + perator or ptions*/
    printf("\nO%s: ", type);
    scanf(" %c", &op);

    /*get string length of opts for the loop*/
    optsLength = strlen(opts);
    /*loop through chars (valid options) of opts*/
    for (int i = 0; i < optsLength; i++) {
        /*set valid to 1 if input matches one of the options*/
        if (op == *(opts + i)) {
            valid = 1;
        }
    }
    /*return the option if it is valid, else print "Unknown o + ption or perator, and call the function recursively */
    if (valid) {
        return op;
    } else {
        printf("\nUnknown o%s.", type);
        op = getOperator(type, opts);
    }
    return op;
}

/*Basic Math Operations*/
int add(int a, int b) {
    int result = 0;
    result = a + b;
    printf("\nResult: %d", result);
    return result;
}

int sub(int a, int b) {
    int result = 0;
    result = a - b;
    printf("\nResult: %d", result);
    return result;
}


int div(int a, int b, char *option) {
    int result = 0;
    int remainder = 0;

    /*check for division with remainder and prompt the user to choose an option*/
    if ((remainder = a % b) != 0) {
        printf("\nDivision has remainder %d. Round (r), cut off (c), or discard (d)?", remainder);

        /*value at address 'option' = getOperator...*/
        *option = getOperator("ption","rcd");
        switch (*option) {
            case 'r':
                /*round and set remainder to 0 or 1*/
                if ((2 * remainder) < b) {
                    remainder = 0;
                } else {
                    remainder = 1;
                }
                break;
            case 'c':
                /*cut = set remainder to 0*/
                remainder = 0;
                break;
            case 'd':
                printf("\nNo new result.");
                /*return the first number, because the division was discarded*/
                return a;
            default:
                break;
        }
        result = a / b + remainder;
    } else {
        result = a / b;
    }

    printf("\nResult: %d", result);
    return result;
}

//
int mul(int a, int b) {
    int result = 0;
    result = a * b;
    printf("\nResult: %d", result);
    return result;
}

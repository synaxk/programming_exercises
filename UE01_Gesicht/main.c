#include <stdio.h>
/*UE01: Gesicht*/

int main() {
    /*print 10 lines*/
    for (int i = 0; i < 10; i++) {
        /*print a line of 16 chars*/
        for (int j = 0; j < 16; j++) {
            /*first line - 0123456789012345*/
            if (i == 0) {
                if (j > 9)
                    putchar(j - 10 + '0');
                else
                    putchar(j + '0');
                /*top*/
            } else if (i == 1) {
                putchar('_');
                /*eyes*/
            } else if (i == 3 && ((j >= 4 && j <= 6) || (j >= 9 && j <= 11))) {
               putchar('"'); 
            } else if (i == 4 && (j == 5 || j == 10)) {
                putchar('o');
                /*mouth*/
            } else if (i == 5 && (j == 7 || j == 8)) {
                putchar('_');
                /*beard*/
            } else if (i == 7 && (j >= 6 && j <= 9)) {
                putchar('=');
            } else if (i == 8 && (j == 7 || j == 8)) {
                putchar('=');
                /*sides i >= 2 to start at the third row
                                  left end || right end*/
            } else if (i >= 2 && (j == i-2 || j == 17-i))
                /*split at column 8,left side '\', right side '/'' */
                j < 8 ? putchar('\\') : putchar('/');
                /*filler*/
            else {
                putchar(' ');
            }
        }
        putchar('\n');
    }
    return 0;
}


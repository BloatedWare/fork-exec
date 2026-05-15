#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>

#define FAILED_FORK -100
#define FAILED_EXEC -101
#define INSUFFICENT_NBR_OF_ARGS -102
#define NOT_A_NUMBER -103
#define ZERO_OR_NEGATIVE -104

bool isNumber(const char* str);

int main(int argc, char** argv) {

    int n;
    if (argc < 3) {
        printf("lancer <number of arguments> PATHS...\n");
        exit(INSUFFICENT_NBR_OF_ARGS);
    }

    if (!isNumber(argv[1])) {
        printf("second argument must be an int!\n");
        exit(NOT_A_NUMBER);
    }

    n = atoi(argv[1]);
    if (n < 1) {
        printf("N: cannot be 0 or negative!\n");
        exit(ZERO_OR_NEGATIVE);
    }

    for (int i = 2; i < argc; i++) {
        for (int j = 0; j < n; j++) {
            switch (fork()) {
                case -1:
                    perror("fork");
                    exit(FAILED_FORK);
                    break;
                case 0:   
                    execlp(argv[i],argv[i], NULL);
                    perror("exec");
                    exit(FAILED_EXEC);
                    break;
                //je suis le parent
            }

        }
    }

    while(wait(NULL) > 0);//retourn -1 lorsqu'il n'y a pas de processus enfants

    return 0;
}

bool isNumber(const char* str) {
    while(*str == ' ')str++;//skip white space
    
    switch(*str) {//skip the sign
        case '+':
        case '-':
            str++;
            break;
    }

    do {
        if (*str < '0' || *str > '9') {//not a digit
            return false;
        }
        str++;
    } while (*str != '\0');
    return true;
}
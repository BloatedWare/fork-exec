#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>


#define FAILED_FORK -100
#define FAILED_EXEC -101
#define INSUFFICIENT_ARGS -102
#define NOT_A_NUMBER -103
#define ZERO_OR_NEGATIVE -104
#define FAILED_MALLOC -105

bool isNumber(const char* str);


//MARK:MAIN
int main(int argc, char** argv) {

    int* children_pids = NULL;
    int fork_count = 0;
    int n;
    if (argc < 3) {
        printf("%s <number of arguments> PATHS...\n", argv[0]);
        exit(INSUFFICIENT_ARGS);
    }

    if (!isNumber(argv[1])) {
        printf("1st argument must be a positive int!\n");
        exit(NOT_A_NUMBER);
    }

    n = atoi(argv[1]);
    if (n < 1) {
        printf("N: cannot be 0 or negative!\n");
        exit(ZERO_OR_NEGATIVE);
    }

    children_pids = (int*)malloc(sizeof(int)*n*(argc-2));//argc-2 == number of apps
    if (children_pids == NULL) {
        perror("malloc");
        exit(FAILED_MALLOC);
    }

    for (int i = 2; i < argc; i++) {
        for (int j = 0; j < n; j++) {
            children_pids[fork_count] = fork();
            switch (children_pids[fork_count]) {
                case -1:
                    perror("fork");
                    
                    for (int k = 0; k < fork_count; k++) {//0 -> before last failed child
                        kill(children_pids[i], SIGKILL);
                    }
                    free(children_pids);
                    exit(FAILED_FORK);
                    break;
                case 0:   
                    execlp(argv[i],argv[i], NULL);
                    perror("exec");
                    exit(FAILED_EXEC);
                    break;
                default:
                    //je suis le parent
                    //only reaches here when the fork succeeds
                    fork_count++;
                    break;
            }

        }
    }

    while(wait(NULL) > 0);//retourn -1 lorsqu'il n'y a pas de processus enfants

    free(children_pids);
    return 0;
}

bool isNumber(const char* str) {
    if(str == NULL) return false;//forgot this edge case
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
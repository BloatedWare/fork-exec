#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define N 10
#define FAILED_FORK -100
#define FAILED_EXEC -101

int main(int argc, char** argv) {

    int p;
    char* emptyArgList[0] = {};
    for (int i = 1; i < argc; i++) {
        for (int j = 0; j < N; j++) {
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
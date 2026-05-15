#include <stdio.h>
#include <unistd.h>

#define N 10

int main(int argc, char** argv) {

    int p;

    for (int i = 1; i < argc; i++) {
        for (int j = 0; j < N; j++) {
            p = fork();
            switch (p) {
                case -1:
                    break;
                case 0:
                    printf("launching: %s\n", argv[i]);
                    execlp(argv[0],NULL);
                    printf("failed to launch: %s\n", argv[i]);
                    break;
                //je suis le parent
            }

        }
    }
    return 0;
}
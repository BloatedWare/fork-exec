#include <stdio.h>
#include <unistd.h>


int main(int argc, char **argv) {


    execlp(argv[1], "whatever I want", "hahah i tricked you", "lol stopid", NULL);
    perror("execlp");   

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define FAILED_FORK -100
#define FAILED_EXEC -101
#define FAILED_MALLOC -105
#define INSUFFICIENT_ARGS -102
#define UNASSIGNED_STATUS -1000

int* children_pids = NULL;
int fork_count = 0;
int status = UNASSIGNED_STATUS;

typedef struct proc_node{
    int pid;
    const char* name;//we will point to string literals so this is "safe"
    struct proc_node* next;
} proc_node;

//fonctions
proc_node* createNode(const char* path);
void cleanup();
void free_linkedlist(proc_node* list);
void display_linkedlist(proc_node* list);


//MARK:MAIN
int main(int argc, char** argv) {
    proc_node *head = NULL, *curr = NULL;

    if (argc < 2) {
        printf("Insufficient argument count!\nhelp: execList FILE...\n");
        exit(INSUFFICIENT_ARGS);
    }

    children_pids = (int*)malloc(sizeof(int)*(argc-1));

    head = createNode(argv[1]);
    wait(&status);
    if(status == FAILED_EXEC) {
        perror("exec");
        cleanup();
        free_linkedlist(head);
        free(children_pids);
        exit(FAILED_EXEC);
    }
    curr = head;

    for (int i = 2; i < argc; i++) {
        curr->next = createNode(argv[i]);

        wait(&status);
        if(status == FAILED_EXEC) {
                perror("exec");
                cleanup();
                free_linkedlist(head);
                free(children_pids);
                exit(FAILED_EXEC);
        }
        curr = curr->next;
    }

    display_linkedlist(head);

    free_linkedlist(head);
    free(children_pids);
    return 0;
}


void free_linkedlist(proc_node* list) {
    proc_node* prev = NULL;
    while (list != NULL) {
        prev = list;
        list = list->next;
        free(prev);
    }
} 


proc_node* createNode(const char* path) {
    
    switch((children_pids[fork_count] = fork())) {
        case -1:
            perror("fork");
            cleanup();
            exit(FAILED_FORK);
            break;
        case 0:
            execlp(path, path, NULL);
            perror("exec");
            exit(FAILED_EXEC);
            break;
        default:
            //parent here
            fork_count++;//only increment if fork succeeds
            
            proc_node* node = (proc_node*)malloc(sizeof(proc_node));
            if (node == NULL) {
                perror("malloc");
                cleanup();
                exit(FAILED_MALLOC);
            }
            
            node->pid = children_pids[fork_count-1];//cuz we incremented
            node->name = path;
            node->next = NULL;

            return node;

    }
    
}

void cleanup() {
    for (int i = 0; i < fork_count; i++) {
        kill(children_pids[i], SIGKILL);
    }

    free(children_pids);
}

void display_linkedlist(proc_node* list) {
    if(list == NULL) {
        printf("Empty!\n");
        return;
    }

    while(list->next != NULL) {
        printf("[name: %s | pid:%d] -> ", list->name, list->pid);
        list = list->next;
    }
    printf("[name: %s | pid:%d] -> %p\n", list->name, list->pid, list->next);
}
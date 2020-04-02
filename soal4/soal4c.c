#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t child_id;
    int status;
    int pip[2];
    int pip2[2];

    if (pipe(pip) < 0) {
        fprintf(stderr, "Pipe Failed");
        exit(1);
    }

    if (pipe(pip2) < 0) {
        fprintf(stderr, "Pipe Failed");
        exit(1);
    }

    child_id = fork();
    if (child_id < 0) {
    exit(EXIT_FAILURE);
    }

    if (child_id == 0) {
        // this is child
        close(pip[0]);
        dup2(pip[1], STDOUT_FILENO);
    
        char *argv[] = {"ls", NULL};
        execv("/bin/ls", argv);
    } else {
        // this is parent
        while ((wait(&status)) > 0);
        close(pip[1]);
        dup2(pip[0], STDIN_FILENO);
        
        char *argv[] = {"wc", "-l", NULL};
        execv("/usr/bin/wc", argv);
    }
}

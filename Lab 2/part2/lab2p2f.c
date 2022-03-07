#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    //create pipe
    int p[2];
    if (pipe(p) < 0) {
        perror("lab2p2f: ");
    }

    if (fork() != 0) {
        //parent process: handle slow
        close(p[0]);
        dup2(p[1], STDOUT_FILENO); //write to pipe
        execlp("./slow", "slow", "5", (char *) 0);
        close(p[1]);
        wait(NULL);
    } else {
        //child process: handle talk and writing to file
        close(p[1]); // Close the writing end
        // call talk and read from pipe
        dup2(p[0], STDIN_FILENO); //read from pipe
        int results_fp = open("./results.out", O_CREAT | O_WRONLY); //write to results
        dup2(results_fp, STDOUT_FILENO);
        execlp("./talk", "talk", (char *) 0);
        close(p[0]);
        close(results_fp);
    }
}


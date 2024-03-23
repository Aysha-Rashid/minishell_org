#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void pipe_ls_to_cat() {
    int pipefd[2];
    pid_t pid;

    // Create pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process (ls)
        // Redirect stdout to write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]); // Close unused read end
        close(pipefd[1]); // Close write end after redirection
        execlp("ls", "ls", NULL); // Execute ls
        perror("execlp ls");
        exit(EXIT_FAILURE);
    } else { // Parent process (cat)
        // Redirect stdin to read end of the pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]); // Close unused write end
        close(pipefd[0]); // Close read end after redirection
        execlp("cat", "cat", NULL); // Execute cat
        perror("execlp cat");
        exit(EXIT_FAILURE);
    }
}

int main() {
    pipe_ls_to_cat();
    return 0;
}

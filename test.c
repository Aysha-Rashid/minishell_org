#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct s_executor {
    char *cmd;
    struct s_executor *next;
    struct s_executor *prev;
} t_executor;

void execute_command(char *command, int in_fd, int out_fd) {
    // Redirect stdin/stdout if necessary
    if (in_fd != STDIN_FILENO) {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (out_fd != STDOUT_FILENO) {
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }

    // Parse and execute the command
    execlp(command, command, NULL);
    perror("exec");
    exit(EXIT_FAILURE);
}

void execution(t_executor *executor) {
    int prev_fds[2] = {-1, -1}; // File descriptors for the previous command's pipe
    int curr_fds[2]; // File descriptors for the current command's pipe

    int in_fd = STDIN_FILENO; // Initial input is stdin

    while (executor != NULL) {
        // Create pipe for the current command
        if (pipe(curr_fds) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        // Fork child process
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process
            close(curr_fds[0]); // Close read end of current pipe

            // Execute command with appropriate I/O redirection
            execute_command(executor->cmd, in_fd, curr_fds[1]);
        } else {
            // Parent process
            close(curr_fds[1]); // Close write end of current pipe

            // Close previous pipe's file descriptors (if not the first command)
            if (executor->prev != NULL) {
                close(prev_fds[0]);
                close(prev_fds[1]);
            }

            // Prepare for the next iteration
            in_fd = curr_fds[0]; // Set current pipe's read end as input for the next command
            executor = executor->next; // Move to the next command
            if (executor != NULL) {
                // Store current pipe's file descriptors for the next iteration
                prev_fds[0] = curr_fds[0];
                prev_fds[1] = curr_fds[1];
            }
        }
    }
}

int main() {
    // Example command pipeline: ls | pwd
    // t_executor *ls_executor = malloc(sizeof(t_executor));
    // ls_executor->cmd = "ls";
    // t_executor *pwd_executor = malloc(sizeof(t_executor));
    // pwd_executor->cmd = "pwd";
    
    // // Connect the two commands in the pipeline
    // ls_executor->next = pwd_executor;
    // ls_executor->prev = NULL;
    // pwd_executor->next = NULL;
    // pwd_executor->prev = ls_executor;

    // // Execute the pipeline
    // execution(ls_executor);

    // // Wait for all child processes to finish
    // while (wait(NULL) > 0);
    access(" pwd");

    return 0;
}

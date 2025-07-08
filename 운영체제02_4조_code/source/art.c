#include "../include/main.h"

void execute_command(const char *cmd) {
    if (strcmp(cmd, "exit") == 0) {
        exit(0); 
    } else if (strcmp(cmd, "start art") == 0) {
        system("./art_animation"); 
    } else {
        printf("Command not recognized.\n");
    }
}

void shell_prompt() {
    char command[256];
    printf("myshell> ");
    while (fgets(command, sizeof(command), stdin)) {
        command[strcspn(command, "\n")] = 0;  
        execute_command(command);
        printf("myshell> ");
    }
}

int main(int argc, char **argv) {
    shell_prompt();  
    return 0;
}


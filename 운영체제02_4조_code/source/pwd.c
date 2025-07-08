#include "../include/main.h"


static void display_help_pwd() {
    printf("pwd: pwd\n");
    printf("    Print the name of the current working directory.\n\n");
    printf("Options:\n");
    printf("      --help     display this help and exit\n");
}


void display_error_pwd(const char *error_type, const char *message) {
    if (!strcmp(error_type, "invalid_option")) {
        printf("pwd: Invalid option\n");
        printf("Try 'cd --help' for more information.\n");
    } else if (!strcmp(error_type, "unrecognized_option")) {
        printf("%s: Unrecognized option -- '%s'\n", message, message);
        printf("Try '%s --help' for more information.\n", message);
    }
}

void collect_directory_path(DirectoryTree *dir_tree, Deque *dir_deque) {
    DirectoryNode *temp_node = dir_tree->current;
    while (temp_node->parent) {
        push_rear(dir_deque, temp_node->name); 
        temp_node = temp_node->parent;
    }
}

void print_collected_path(Deque *dir_deque) {
    while (!is_deque_empty(dir_deque)) {
        printf("/");
        printf("%s", pop_rear(dir_deque)); 
    }
    printf("\n");
}

void print_directory_path_pwd(DirectoryTree *dir_tree, Deque *dir_deque) {
    if (dir_tree->current == dir_tree->root) {
        printf("/");
    } else {
        collect_directory_path(dir_tree, dir_deque);
        print_collected_path(dir_deque);
    }
}

int print_path(DirectoryTree *dir_tree, Deque *dir_deque) {
    print_directory_path_pwd(dir_tree, dir_deque);
    return SUCCESS;
}

int handle_pwd_options(char *command) {
    if (!strcmp(command, "--help")) {
        display_help_pwd();
        return FAIL;
    } else {
        char *str = strtok(command, "-");
        if (!str) {
            display_error_pwd("invalid_option", NULL);
            return -1;
        } else {
            display_error_pwd("unrecognized_option", command);
            return FAIL;
        }
    }
}

int execute_pwd(DirectoryTree *dir_tree, Deque *dir_deque, char *command) {
    if (!command) {
        return print_path(dir_tree, dir_deque);
    }
    if (command[0] == '-') {
        return handle_pwd_options(command);
    }
    return SUCCESS;
}

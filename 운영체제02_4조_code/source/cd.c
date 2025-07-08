#include "../include/main.h"


static void display_help_cd() {
    printf("cd: cd [dir]\n    Change the shell working directory.\n\n    --help     display this help and exit\n");
}


void display_error_cd(const char *error_type, const char *message) {
    if (!strcmp(error_type, "unrecognized_option")) {
        printf("cd: Unrecognized option '%s'\nTry 'cd --help' for more information.\n", message);
    } else if (!strcmp(error_type, "permission_denied")) {
        printf("-bash: cd: %s: Permission denied\n", message);
    } else if (!strcmp(error_type, "not_directory")) {
        printf("-bash: cd: %s: Not a directory\n", message);
    } else if (!strcmp(error_type, "no_such_file")) {
        printf("-bash: cd: %s: No such file or directory\n", message);
    } else {
        printf("-bash: cd: %s\n", message);
    }
}

void print_error(const char *message) {
    display_error_cd("generic_error", message);
}

int change_current_directory(DirectoryTree *dir_tree, char *dir_path) {
    if (!dir_tree || !dir_path) return FAIL;

    if (!strcmp(dir_path, ".")) {
        return SUCCESS;
    } else if (!strcmp(dir_path, "..")) {
        if (dir_tree->current != dir_tree->root) {
            dir_tree->current = dir_tree->current->parent;
        }
    } else {
        DirectoryNode *temp_node = dir_exists(dir_tree, dir_path, 'd');
        if (temp_node) {
            dir_tree->current = temp_node;
        } else {
            return FAIL;
        }
    }
    return SUCCESS;
}

int change_path(DirectoryTree *dir_tree, char *dir_path) {
    if (!dir_tree || !dir_path) return FAIL;

    char temp_path[MAX_DIR];
    strncpy(temp_path, dir_path, MAX_DIR);
    temp_path[MAX_DIR - 1] = '\0';

    DirectoryNode *original_node = dir_tree->current;

    if (!strcmp(dir_path, "/")) {
        dir_tree->current = dir_tree->root;
    } else {
        if (dir_path[0] == '/') {
            dir_tree->current = dir_tree->root;
        }
        char *str = strtok(temp_path, "/");
        while (str) {
            if (change_current_directory(dir_tree, str)) {
                dir_tree->current = original_node;
                return FAIL;
            }
            str = strtok(NULL, "/");
        }
    }
    return SUCCESS;
}

int execute_cd(DirectoryTree *dir_tree, char *command) {
    if (!dir_tree || !command) return FAIL;

    if (command[0] == '-') {
        if (!strcmp(command, "--help")) {
            display_help_cd();
            return FAIL;
        } else {
            display_error_cd("unrecognized_option", command + 1);
            return FAIL;
        }
    }

    DirectoryNode *temp_node = dir_exists(dir_tree, command, 'd');
    if (temp_node && check_permission(temp_node, 'x')) {
        display_error_cd("permission_denied", command);
        return FAIL;
    }
    temp_node = dir_exists(dir_tree, command, 'f');
    if (temp_node) {
        display_error_cd("not_directory", command);
        return FAIL;
    }
    if (change_path(dir_tree, command)) {
        display_error_cd("no_such_file", command);
        return FAIL;
    }
    return SUCCESS;
}


#include "../include/main.h"


static void display_help_find() {
    printf("Usage: find [path...] [expression]\n");
    printf("  Options:\n");
    printf("      -name      finds file by name\n");
    printf("      --help     display this help and exit\n");
}


void display_error_find(const char *error_type, const char *message) {
    if (!strcmp(error_type, "missing_argument")) {
        printf("find: missing argument to `-name'\n");
    } else if (!strcmp(error_type, "unknown_predicate")) {
        printf("find: unknown predicate '%s'\nTry 'find --help' for more information.\n", message);
    } else if (!strcmp(error_type, "cannot_open")) {
        printf("Cannot open Directory.txt file for reading\n");
    }
}


void print_directory_path_find(const char *dir, const char *file_name) {
    printf("%s/%s\n", strcmp(dir, "/") == 0 ? "" : dir, file_name);
}

int process_directory_entry(char *entry, const char *dir_name, int option) {
    char *str;
    char file_name[MAX_NAME];

    str = strtok(entry, " ");
    if (!str) return FAIL;

    strcpy(file_name, str);
    for (int i = 0; i < 10; i++) {
        str = strtok(NULL, " ");
    }

    if (str) {
        if ((option == 0 && strstr(file_name, dir_name)) || (option != 0 && strstr(str, dir_name))) {
            str[strlen(str) - 1] = '\0';  
            print_directory_path_find(str, file_name);
        }
    }
    return SUCCESS;
}

void read_directory_file(FILE *dir_file, DirectoryTree *dir_tree, const char *dir_name, int option) {
    char entry[MAX_LENGTH];
    while (fgets(entry, MAX_LENGTH, dir_file)) {
        process_directory_entry(entry, dir_name, option);
    }
}

void find_dir(DirectoryTree *dir_tree, char *dir_name, int option) {
    FILE *dir_file = fopen("file/Directory.txt", "r");
    if (!dir_file) {
        display_error_find("cannot_open", NULL);
        return;
    }

    read_directory_file(dir_file, dir_tree, dir_name, option);
    fclose(dir_file);
}

int handle_find_command(DirectoryTree *dir_tree, char *command) {
    char *str;

    if (!command) {
        find_dir(dir_tree, dir_tree->current->name, 1);
        return SUCCESS;
    }

    if (command[0] == '-') {
        if (!strcmp(command, "-name")) {
            str = strtok(NULL, " ");
            if (!str) {
                display_error_find("missing_argument", NULL);
                return FAIL;
            }
            find_dir(dir_tree, str, 0);
            return SUCCESS;
        } else if (!strcmp(command, "--help")) {
            display_help_find();
            return FAIL;
        } else {
            display_error_find("unknown_predicate", command + 1);
            return FAIL;
        }
    } else {
        find_dir(dir_tree, command, 0);
        find_dir(dir_tree, command, 1);
    }
    return SUCCESS;
}

int execute_find(DirectoryTree *dir_tree, char *command) {
    if (!dir_tree || !command) return FAIL;

    return handle_find_command(dir_tree, command);
}

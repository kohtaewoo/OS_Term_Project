#include "../include/main.h"


void display_help_rm() {
    printf("Manual: rm [Option]... [File]...\n");
    printf("  Remove the FILE(s).\n\n");
    printf("  Options:\n");
    printf("    -f, --force    \t ignore nonexistent files and arguments, never prompt\n");
    printf("    -r, --recursive\t remove directories and their contents recursively\n");
    printf("        --help\t Display this help and exit\n");
}


void display_error_rm(const char *error_type, const char *message) {
    if (!strcmp(error_type, "invalid_option")) {
        printf("rm: Invalid option\n");
        printf("Try 'rm --help' for more information.\n");
    } else if (!strcmp(error_type, "unrecognized_option")) {
        printf("rm: Unrecognized option -- '%s'\n", message);
        printf("Try 'rm --help' for more information.\n");
    } else if (!strcmp(error_type, "no_such_file")) {
        printf("rm: Can not remove '%s': No such file or directory.\n", message);
    }
}

void destroy_node(DirectoryNode *dir_node) {
    free(dir_node);
}

void destroy_dir(DirectoryNode *dir_node) {
    if (dir_node->next_sibling) {
        destroy_dir(dir_node->next_sibling);
    }
    if (dir_node->first_child) {
        destroy_dir(dir_node->first_child);
    }
    destroy_node(dir_node);
}

DirectoryNode *find_node_to_destroy(DirectoryTree *dir_tree, char *dir_name, DirectoryNode **prev_node) {
    DirectoryNode *temp_node = dir_tree->current->first_child;

    while (temp_node && strcmp(temp_node->name, dir_name)) {
        *prev_node = temp_node;
        temp_node = temp_node->next_sibling;
    }
    return temp_node;
}

DirectoryNode *find_and_destroy_dir(DirectoryTree *dir_tree, char *dir_name) {
    DirectoryNode *del_node = NULL;
    DirectoryNode *prev_node = NULL;

    del_node = find_node_to_destroy(dir_tree, dir_name, &prev_node);

    if (del_node) {
        if (prev_node) {
            prev_node->next_sibling = del_node->next_sibling;
        } else {
            dir_tree->current->first_child = del_node->next_sibling;
        }
        if (del_node->first_child) destroy_dir(del_node->first_child);
        destroy_node(del_node);
    } else {
        display_error_rm("no_such_file", dir_name);
        return NULL;
    }

    return del_node;
}

int remove_dir(DirectoryTree *dir_tree, char *dir_name) {
    if (!find_and_destroy_dir(dir_tree, dir_name)) {
        return FAIL;
    }
    return SUCCESS;
}

void execute_thread(DirectoryTree *dir_tree, ThreadTree *thread_tree, int option, char *command) {
    thread_tree->thread_tree = dir_tree;
    thread_tree->command = command;
    thread_tree->option = option;
}

int handle_rm_option(DirectoryTree *dir_tree, ThreadTree *thread_tree, int *thread_count, char *option, int option_code) {
    char *str = strtok(NULL, " ");
    if (!str) {
        display_error_rm("invalid_option", NULL);
        return FAIL;
    }
    while (str) {
        execute_thread(dir_tree, &thread_tree[*thread_count], option_code, str);
        (*thread_count)++;
        str = strtok(NULL, " ");
    }
    return SUCCESS;
}

int handle_rm_command(DirectoryTree *dir_tree, ThreadTree *thread_tree, int *thread_count, char *command) {
    char *str = strtok(NULL, " ");
    execute_thread(dir_tree, &thread_tree[*thread_count], 0, command);
    (*thread_count)++;
    if (!command) {
        display_error_rm("invalid_option", NULL);
        return FAIL;
    }
    while (str) {
        execute_thread(dir_tree, &thread_tree[*thread_count], 0, str);
        (*thread_count)++;
        str = strtok(NULL, " ");
    }
    return SUCCESS;
}

int execute_rm(DirectoryTree *dir_tree, char *command) {
    DirectoryNode *current_node = NULL;
    int thread_count = 0;
    pthread_t thread_pool[MAX_THREAD];
    ThreadTree thread_tree[MAX_THREAD];

    if (!command) {
        display_error_rm("invalid_option", NULL);
        return FAIL;
    }

    current_node = dir_tree->current;

    if (command[0] == '-') {
        if (!strcmp(command, "-r")) {
            if (handle_rm_option(dir_tree, thread_tree, &thread_count, command, 1) == FAIL) return FAIL;
        } else if (!strcmp(command, "-f")) {
            if (handle_rm_option(dir_tree, thread_tree, &thread_count, command, 2) == FAIL) return FAIL;
        } else if (!strcmp(command, "-rf") || !strcmp(command, "-fr")) {
            if (handle_rm_option(dir_tree, thread_tree, &thread_count, command, 3) == FAIL) return FAIL;
        } else if (!strcmp(command, "--help")) {
            display_help_rm();
            return FAIL;
        } else {
            char *str = strtok(command, "-");
            if (!str) {
                display_error_rm("invalid_option", NULL);
                return FAIL;
            } else {
                display_error_rm("unrecognized_option", str);
                return FAIL;
            }
        }
    } else {
        if (handle_rm_command(dir_tree, thread_tree, &thread_count, command) == FAIL) return FAIL;
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_create(&thread_pool[i], NULL, rmdir_using_thread, (void *)&thread_tree[i]);
        pthread_join(thread_pool[i], NULL);
    }

    return SUCCESS;
}

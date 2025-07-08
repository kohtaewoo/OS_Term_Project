#include "../include/main.h"


void display_help_mkdir() {
    printf("Usage: mkdir [option]... [directory]...\n");
    printf("  Create the DIRECTORY(ies), if they do not already exists.\n\n");
    printf("  Options:\n");
    printf("    -p, --parents  \t no error if existing, make parent directories as needed\n");
    printf("      --help\t Display this help and exit\n");
}


void display_error_mkdir(const char *error_type, const char *message) {
    if (!strcmp(error_type, "permission_denied")) {
        printf("mkdir: '%s' Can not create directory: Permission denied.\n", message);
    } else if (!strcmp(error_type, "invalid_option")) {
        printf("mkdir: Invalid option\n");
        printf("Try 'mkdir --help' for more information.\n");
    } else if (!strcmp(error_type, "unrecognized_option")) {
        printf("mkdir: Unrecognized option -- '%s'\n", message);
        printf("Try 'mkdir --help' for more information.\n");
    } else if (!strcmp(error_type, "file_exists")) {
        printf("mkdir: '%s' Can not create directory: File exists.\n", message);
    } else if (!strcmp(error_type, "invalid_directory")) {
        printf("mkdir: '%s' Can not create directory.\n", message);
    }
}

int create_new_directory(DirectoryTree* dir_tree, char* dir_name, char type, DirectoryNode* new_node) {
    time_t ltime;
    struct tm* today;
    time(&ltime);
    today = localtime(&ltime);

    new_node->first_child = NULL;
    new_node->next_sibling = NULL;
    strncpy(new_node->name, dir_name, MAX_NAME);
    new_node->type = (type == 'd') ? 'd' : 'f';
    new_node->permission.mode = (type == 'd') ? 755 : 644;
    new_node->size = (type == 'd') ? 4096 : 0;

    mode_to_permission(new_node);
    new_node->id.uid = usr_list->current->id.uid;
    new_node->id.gid = usr_list->current->id.gid;
    new_node->date.month = today->tm_mon + 1;
    new_node->date.day = today->tm_mday;
    new_node->date.hour = today->tm_hour;
    new_node->date.minute = today->tm_min;
    new_node->parent = dir_tree->current;

    if (!dir_tree->current->first_child) {
        dir_tree->current->first_child = new_node;
    } else {
        DirectoryNode* temp_node = dir_tree->current->first_child;
        while (temp_node->next_sibling) temp_node = temp_node->next_sibling;
        temp_node->next_sibling = new_node;
    }
    return SUCCESS;
}

int make_dir(DirectoryTree* dir_tree, char* dir_name, char type) {
    DirectoryNode* new_node = (DirectoryNode*)malloc(sizeof(DirectoryNode));
    if (!new_node) {
        perror("malloc");
        return FAIL;
    }

    if (check_permission(dir_tree->current, 'w')) {
        display_error_mkdir("permission_denied", dir_name);
        free(new_node);
        return FAIL;
    }
    if (!strcmp(dir_name, ".") || !strcmp(dir_name, "..")) {
        display_error_mkdir("invalid_directory", dir_name);
        free(new_node);
        return FAIL;
    }
    DirectoryNode* temp_node = dir_exists(dir_tree, dir_name, type);
    if (temp_node && temp_node->type == 'd') {
        display_error_mkdir("file_exists", dir_name);
        free(new_node);
        return FAIL;
    }

    return create_new_directory(dir_tree, dir_name, type, new_node);
}

int parse_mkdir_options(DirectoryTree* dir_tree, char* command, ThreadTree* thread_tree, int* thread_count) {
    char* str;
    if (!strcmp(command, "-p")) { 
        str = strtok(NULL, " ");
        if (!str) {
            display_error_mkdir("invalid_option", NULL);
            return FAIL;
        }
        while (str) {
            thread_tree[*thread_count].thread_tree = dir_tree;
            thread_tree[*thread_count].option = 1;
            thread_tree[*thread_count].command = str;
            (*thread_count)++;
            str = strtok(NULL, " ");
        }
    } else if (!strcmp(command, "--help")) { 
        display_help_mkdir();
        return FAIL;
    } else { 
        display_error_mkdir("unrecognized_option", command);
        return FAIL;
    }
    return SUCCESS;
}

int execute_mkdir(DirectoryTree* dir_tree, char* command) {
    if (!command) { 
        display_error_mkdir("invalid_option", NULL);
        return FAIL;
    }

    int thread_count = 0;
    pthread_t thread_pool[MAX_THREAD];
    ThreadTree thread_tree[MAX_THREAD];

    if (command[0] == '-') { 
        if (parse_mkdir_options(dir_tree, command, thread_tree, &thread_count) == FAIL) {
            return FAIL;
        }
    } else { 
        thread_tree[thread_count].thread_tree = dir_tree;
        thread_tree[thread_count].option = 0;
        thread_tree[thread_count].command = command;
        thread_count++;

        char* str = strtok(NULL, " ");
        while (str) {
            thread_tree[thread_count].thread_tree = dir_tree;
            thread_tree[thread_count].option = 0;
            thread_tree[thread_count].command = str;
            thread_count++;
            str = strtok(NULL, " ");
        }
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_create(&thread_pool[i], NULL, mkdir_using_thread, (void*)&thread_tree[i]);
    }
    for (int i = 0; i < thread_count; i++) {
        pthread_join(thread_pool[i], NULL);
    }
    return SUCCESS;
}
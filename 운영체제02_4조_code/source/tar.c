#include "../include/main.h"


void display_help_tar() {
    printf("tar(bsdtar): manipulate archive files\n");
    printf("Usage:\n");
    printf("\tExtract: tar -xvf <archive-filename>\n");
    printf("\tCreate:  tar -cvf <archive-filename> [filenames...]\n");
    printf("\tHelp:    tar --help\n");
    printf("      --help\t Display this help and exit\n");
}


void display_error_tar(const char *error_type, const char *message) {
    if (!strcmp(error_type, "invalid_option")) {
        printf("tar: Invalid option\n");
        printf("Try 'tar --help' for more information.\n");
    } else if (!strcmp(error_type, "unrecognized_option")) {
        printf("tar: Unrecognized option -- '%s'\n", message);
        printf("Try 'tar --help' for more information.\n");
    } else if (!strcmp(error_type, "permission_denied")) {
        printf("tar: '%s' Can not create directory: Permission denied.\n", message);
    }
}

int make_tar(DirectoryTree* dir_tree, char* dir_name, char* content, char type) {
    DirectoryNode* new_node = (DirectoryNode*)malloc(sizeof(DirectoryNode));
    if (!new_node) {
        perror("malloc");
        return FAIL;
    }
    DirectoryNode* temp_node = NULL;

    if (check_permission(dir_tree->current, 'w')) {
        display_error_tar("permission_denied", dir_name);
        free(new_node);
        return FAIL;
    }

    time_t ltime;
    struct tm* today;
    time(&ltime);
    today = localtime(&ltime);

    new_node->first_child = NULL;
    new_node->next_sibling = NULL;
    strncpy(new_node->name, dir_name, MAX_NAME);
    new_node->type = (type == 'd') ? 'd' : 'a';
    new_node->permission.mode = 755;
    new_node->size = 0;

    mode_to_permission(new_node);
    new_node->id.uid = usr_list->current->id.uid;
    new_node->id.gid = usr_list->current->id.gid;
    new_node->date.month = today->tm_mon + 1;
    new_node->date.day = today->tm_mday;
    new_node->date.hour = today->tm_hour;
    new_node->date.minute = today->tm_min;
    new_node->parent = dir_tree->current;
    strncpy(new_node->content, content, 20);
    if (!dir_tree->current->first_child) {
        dir_tree->current->first_child = new_node;
    } else {
        temp_node = dir_tree->current->first_child;
        while (temp_node->next_sibling) temp_node = temp_node->next_sibling;
        temp_node->next_sibling = new_node;
    }
    return SUCCESS;
}

char* files;
char safe_content[20];


int tar_options(DirectoryTree* p_directory_tree, char* command, ThreadTree* thread_tree, int* thread_count) {
    char* str;
    DirectoryNode* temp_node;

    if (strcmp(command + 1, "cvf") == 0) {
        str = strtok(NULL, " ");
        if (!str) {
            display_error_tar("invalid_option", NULL);
            return FAIL;
        }

        thread_tree[*thread_count].thread_tree = p_directory_tree;
        thread_tree[*thread_count].option = 1;
        thread_tree[*thread_count].command = str;
        thread_tree[*thread_count].content = str + (strlen(str) + 1);
        (*thread_count)++;
    } else if (strcmp(command + 1, "xvf") == 0) {
        str = strtok(NULL, " ");
        if (!str) {
            display_error_tar("invalid_option", NULL);
            return FAIL;
        }

        if (!p_directory_tree->current->first_child) {
            printf("file not exists\n");
        } else {
            temp_node = p_directory_tree->current->first_child;
            while (temp_node) {
                if (strcmp(temp_node->name, str) == 0) {
                    strncpy(safe_content, temp_node->content, 20);
                    files = strtok(safe_content, " ");

                    while (files) {
                        printf("x %s/\n",files);
                        thread_tree[*thread_count].thread_tree = p_directory_tree;
                        thread_tree[*thread_count].option = 2;
                        thread_tree[*thread_count].command = files;
                        (*thread_count)++;
                        files = strtok(NULL, " ");
                    }
                    break;
                }
                temp_node = temp_node->next_sibling;
            }
        }
    } else if (!strcmp(command, "--help")) {
        display_help_tar();
        return FAIL;
    } else {
        display_error_tar("unrecognized_option", command);
        return FAIL;
    }
    return SUCCESS;
}

int execute_tar(DirectoryTree* p_directory_tree, char* command) {
    if (!command) {
        display_error_tar("invalid_option", NULL);
        return FAIL;
    }

    int thread_count = 0;
    pthread_t thread_pool[MAX_THREAD];
    ThreadTree thread_tree[MAX_THREAD];

    if (command[0] == '-') {
        if (tar_options(p_directory_tree, command, thread_tree, &thread_count) == FAIL) {
            return FAIL;
        }
    } else {
        display_error_tar("invalid_option", NULL);
        return FAIL;
    }

    if (thread_tree[0].option == 2) {
        for (int i = 0; i < thread_count; i++) {
            pthread_create(&thread_pool[i], NULL, mkdir_using_thread, (void*)&thread_tree[i]);
        }
    } else {
        for (int i = 0; i < thread_count; i++) {
            pthread_create(&thread_pool[i], NULL, tar_using_thread, (void*)&thread_tree[i]);
        }
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(thread_pool[i], NULL);
    }
    return SUCCESS;
}


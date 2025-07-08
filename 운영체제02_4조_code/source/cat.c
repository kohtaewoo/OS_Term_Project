#include "../include/main.h"

void display_help() {
    printf("Usage: cat [OPTION]... [FILE]...\n");
    printf("Concatenate FILE(s) to standard output.\n\n");
    printf("With no FILE, or when FILE is -, read standard input.\n\n");
    printf("  -n, --number             number all output lines\n");
    printf("      --help               display this help and exit\n");
}

void print_user_list() {
    UserNode *user = usr_list->head;
    while (user) {
        printf("%s:x:%d:%d:%s:%s\n", user->name, user->id.uid, user->id.gid, user->name, user->dir);
        user = user->next_node;
    }
}

int read_and_print_file(const char *file_name, int number_lines) {
    FILE *file = fopen(file_name, "r");
    if (!file) return FAIL;

    char buffer[MAX_BUFFER];
    int line_number = 1;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (number_lines) {
            if (buffer[strlen(buffer) - 1] == '\n') {
                printf("     %d ", line_number);
                line_number++;
            }
        }
        fputs(buffer, stdout);
    }

    fclose(file);
    return SUCCESS;
}

int concatenate(DirectoryTree *dir_tree, char *file_name, int type) {
    if (type == 4) {
        print_user_list();
        return SUCCESS;
    }

    DirectoryNode *dir_node = dir_exists(dir_tree, file_name, 'f');
    if (!dir_node) return FAIL;

    return read_and_print_file(file_name, (type == 2));
}

int update_directory_node(DirectoryTree *dir_tree, char *file_name, int file_size) {
    DirectoryNode *dir_node = dir_exists(dir_tree, file_name, 'f');
    if (dir_node) {
        time(&ltime);
        today = localtime(&ltime);

        dir_node->date.month = today->tm_mon + 1;
        dir_node->date.day = today->tm_mday;
        dir_node->date.hour = today->tm_hour;
        dir_node->date.minute = today->tm_min;
    } else {
        make_dir(dir_tree, file_name, 'f');
    }

    dir_node = dir_exists(dir_tree, file_name, 'f');
    dir_node->size = file_size;
    return SUCCESS;
}

int create_file(DirectoryTree *dir_tree, char *file_name, char *file_path) {
    FILE *file = fopen(file_name, "w");
    if (!file) return FAIL;

    char buffer[MAX_BUFFER];
    int file_size = 0;

    while (fgets(buffer, sizeof(buffer), stdin)) {
        fputs(buffer, file);
        file_size += strlen(buffer) - 1;
    }

    rewind(stdin);
    fclose(file);

    return update_directory_node(dir_tree, file_name, file_size);
}

int handle_cat_command(DirectoryTree *dir_tree, char *command, ThreadTree *thread_tree, int *thread_count) {
    char *str;
    char temp[MAX_DIR];
    char temp2[MAX_DIR];
    char temp3[MAX_DIR];
    int value;

    if (!command) {
        long size;
        unsigned char c;
        size = read(0, &c, 1);
        while (size) {
            write(1, &c, size);
            size = read(0, &c, 1);
        }
        rewind(stdin);
        return SUCCESS;
    }

    if (strcmp(command, ">") == 0) {
        str = strtok(NULL, " ");
        if (str == NULL) {
            printf("cat: Invalid option\n");
            display_help();
            return FAIL;
        }
        strncpy(temp, str, MAX_DIR);
        if (!strstr(str, "/")) {
            if (check_permission(dir_tree->current, 'w')) {
                printf("cat: Can not create file '%s': No permission.\n", dir_tree->current->name);
                return FAIL;
            }
            DirectoryNode *dir_node = dir_exists(dir_tree, str, 'd');
            if (dir_node) {
                printf("cat: '%s': Is a directory\n", str);
                return FAIL;
            } else {
                create_file(dir_tree, str, NULL);
            }
        } else {
            strncpy(temp2, get_dir(str), MAX_DIR);
            value = change_path(dir_tree, temp2);
            if (value) {
                printf("cat: '%s': No such file or directory\n", temp2);
                return FAIL;
            }
            str = strtok(temp, "/");
            while (str) {
                strncpy(temp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            if (check_permission(dir_tree->current, 'w')) {
                printf("cat: Can not create file '%s': No permission.\n", dir_tree->current->name);
                dir_tree->current = dir_tree->current->parent;  // Corrected line
                return FAIL;
            }
            DirectoryNode *dir_node = dir_exists(dir_tree, temp3, 'd');
            if (dir_node) {
                printf("cat: '%s': Is a directory\n", temp3);
                dir_tree->current = dir_tree->current->parent;  // Corrected line
                return FAIL;
            } else {
                create_file(dir_tree, temp3, NULL);
            }
            dir_tree->current = dir_tree->current->parent;  // Corrected line
        }
        return 0;
    } else if (command[0] == '-') {
        if (!strcmp(command, "-n")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("cat: Invalid option\n");
                display_help();
                return FAIL;
            }
            while (str) {
                thread_tree[*thread_count].thread_tree = dir_tree;
                thread_tree[*thread_count].option = 1;
                thread_tree[(*thread_count)++].command = str;
                str = strtok(NULL, " ");
            }
        } else if (strcmp(command, "--help") == 0) {
            display_help();
            return SUCCESS;
        } else {
            printf("cat: Unrecognized option -- '%s'\n", command + 1);
            display_help();
            return FAIL;
        }
    } else {
        if (!strcmp(command, "/etc/password")) {
            concatenate(dir_tree, "password", 4);
            return SUCCESS;
        }
        if (!command) {
            printf("cat: Invalid option\n");
            display_help();
            return FAIL;
        }
        str = strtok(NULL, " ");
        thread_tree[*thread_count].thread_tree = dir_tree;
        thread_tree[*thread_count].option = 0;
        thread_tree[(*thread_count)++].command = command;
        while (str) {
            thread_tree[*thread_count].thread_tree = dir_tree;
            thread_tree[*thread_count].option = 0;
            thread_tree[(*thread_count)++].command = str;
            str = strtok(NULL, " ");
        }
    }
    return SUCCESS;
}

int execute_cat_threads(ThreadTree *thread_tree, int thread_count) {
    pthread_t thread_pool[MAX_THREAD];
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&thread_pool[i], NULL, cat_using_thread, (void *)&thread_tree[i]);
        pthread_join(thread_pool[i], NULL);
    }
    return SUCCESS;
}

int execute_cat(DirectoryTree *dir_tree, char *command) {
    DirectoryNode *current_node = dir_tree->current;
    ThreadTree thread_tree[MAX_THREAD];
    int thread_count = 0;

    if (handle_cat_command(dir_tree, command, thread_tree, &thread_count) == FAIL) {
        return FAIL;
    }

    return execute_cat_threads(thread_tree, thread_count);
}

#include "../include/main.h"


static void display_help_chown() {
    printf("Usage: chown [OPTION]... [OWNER] FILE...\n");
    printf("Change the owner of each FILE to OWNER.\n\n");
    printf("  Options:\n");
    printf("      --help     display this help and exit\n");
}


void display_error_chown(const char *error_type, const char *message) {
    if (!strcmp(error_type, "permission_denied")) {
        printf("chown: Can not modify file '%s': Permission denied\n", message);
    } else if (!strcmp(error_type, "invalid_user")) {
        printf("chown: Invalid user: '%s'\n", message);
        display_help_chown();
    } else if (!strcmp(error_type, "no_such_file")) {
        printf("chown: cannot access '%s': No such file or directory\n", message);
    } else if (!strcmp(error_type, "invalid_option")) {
        printf("chown: Invalid option\n");
        display_help_chown();
    } else if (!strcmp(error_type, "unrecognized_option")) {
        printf("chown: Unrecognized option -- '%s'\n", message);
        display_help_chown();
    }
}

int update_directory_owner(DirectoryNode *dir_node, UserNode *user, int flag) {
    if (!flag) {
        dir_node->id.uid = user->id.uid;
    } else {
        dir_node->id.gid = user->id.gid;
    }
    return SUCCESS;
}

int update_file_owner(DirectoryNode *file_node, UserNode *user, int flag) {
    if (!flag) {
        file_node->id.uid = user->id.uid;
    } else {
        file_node->id.gid = user->id.gid;
    }
    return SUCCESS;
}

int validate_user_and_update_owner(DirectoryTree *dir_tree, DirectoryNode *node, char *user_name, int flag, char *dir_name) {
    UserNode *temp_user = user_exists(usr_list, user_name);
    if (temp_user) {
        if (node->type == 'd') {
            return update_directory_owner(node, temp_user, flag);
        } else if (node->type == 'f') {
            return update_file_owner(node, temp_user, flag);
        }
    } else {
        display_error_chown("invalid_user", user_name);
        return FAIL;
    }
    return SUCCESS;
}

int change_owner(DirectoryTree *dir_tree, char *user_name, char *dir_name, int flag) {
    DirectoryNode *dir_node = dir_exists(dir_tree, dir_name, 'd');
    DirectoryNode *file_node = dir_exists(dir_tree, dir_name, 'f');

    if (dir_node) {
        if (check_permission(dir_node, 'w')) {
            display_error_chown("permission_denied", dir_name);
            return FAIL;
        }
        return validate_user_and_update_owner(dir_tree, dir_node, user_name, flag, dir_name);
    } else if (file_node) {
        if (check_permission(file_node, 'w')) {
            display_error_chown("permission_denied", dir_name);
            return FAIL;
        }
        return validate_user_and_update_owner(dir_tree, file_node, user_name, flag, dir_name);
    } else {
        display_error_chown("no_such_file", dir_name);
        return FAIL;
    }
}

int handle_chown_command(DirectoryTree *dir_tree, char *command) {
    if (command[0] == '-') {
        if (!strcmp(command, "--help")) {
            display_help_chown();
            return FAIL;
        } else {
            char *str = strtok(command, "-");
            if (!str) {
                display_error_chown("invalid_option", NULL);
                return FAIL;
            } else {
                display_error_chown("unrecognized_option", str);
                return FAIL;
            }
        }
    }

    char temp[MAX_NAME];
    strncpy(temp, command, MAX_NAME);
    char *str = strtok(NULL, " ");
    if (!str) {
        display_error_chown("invalid_option", NULL);
        return FAIL;
    } else {
        ThreadTree thread_tree[MAX_THREAD];
        pthread_t thread_pool[MAX_THREAD];
        int thread_count = 0;

        thread_tree[thread_count].thread_tree = dir_tree;
        thread_tree[thread_count].usr_name = temp;
        thread_tree[thread_count++].command = str;
        while (str) {
            thread_tree[thread_count].thread_tree = dir_tree;
            thread_tree[thread_count].usr_name = temp;
            thread_tree[thread_count++].command = str;
            str = strtok(NULL, " ");
        }

        for (int i = 0; i < thread_count; i++) {
            pthread_create(&thread_pool[i], NULL, chown_using_thread, (void*)&thread_tree[i]);
            pthread_join(thread_pool[i], NULL);
        }
    }
    return SUCCESS;
}

int execute_chown(DirectoryTree *dir_tree, char *command) {
    if (!dir_tree || !command) return FAIL;

    return handle_chown_command(dir_tree, command);
}
#include "../include/main.h"

void *cat_using_thread(void *arg) {
    ThreadTree *thread_tree = (ThreadTree *)arg;
    DirectoryTree *dir_tree = thread_tree->thread_tree;
    DirectoryTree *temp_tree;
    char *command = thread_tree->command;
    DirectoryNode *current_node = dir_tree->current;
    DirectoryNode *temp_node = NULL;
    DirectoryNode *temp_node2 = NULL;
    char temp[MAX_DIR];
    char temp2[MAX_DIR];
    char temp3[MAX_DIR];
    char *str;
    int option = thread_tree->option;
    int value;

    strncpy(temp, command, MAX_DIR);

    if (option == 1) {
        if (!strstr(temp, "/")) {
            if (check_permission(dir_tree->current, 'w')) {
                printf("cat: Can not create file '%s': Permission denied\n", dir_tree->current->name);
                return NULL;
            }
            temp_node = dir_exists(dir_tree, command, 'd');
            temp_node2 = dir_exists(dir_tree, command, 'f');
            if (!temp_node && !temp_node2) {
                printf("cat: '%s': No such file or directory.\n", command);
                return NULL;
            } else if (temp_node && !temp_node2) {
                printf("cat: '%s': Is a directory\n", command);
                return NULL;
            } else if (temp_node2 && check_permission(temp_node2, 'r')) {
                printf("cat: Can not open file '%s': Permission denied\n", temp_node2->name);
                return NULL;
            } else concatenate(dir_tree, command, 2);
        } else {
            strncpy(temp2, get_dir(temp), MAX_DIR);
            value = change_path(dir_tree, temp2);
            if (value) {
                printf("cat: '%s': No such file or directory.\n", temp2);
                return NULL;
            }
            str = strtok(temp, "/");
            while (str) {
                strncpy(temp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            temp_node = dir_exists(dir_tree, temp3, 'd');
            temp_node2 = dir_exists(dir_tree, temp3, 'f');
            if (!temp_node && !temp_node2) {
                printf("cat: '%s': No such file or directory.\n", temp3);
                dir_tree->current = current_node;
                return NULL;
            } else if (temp_node && !temp_node2) {
                printf("cat: '%s': Is a directory\n", temp3);
                dir_tree->current = current_node;
                return NULL;
            } else if (temp_node2 && check_permission(temp_node2, 'r')) {
                printf("cat: Can not open file '%s': Permission denied\n", temp_node2->name);
                dir_tree->current = current_node;
                return NULL;
            } else concatenate(dir_tree, temp3, 2);
            dir_tree->current = current_node;
        }
    } else {
        if (!strstr(temp, "/")) {
            if (check_permission(dir_tree->current, 'w')) {
                printf("cat: Can not create file '%s': Permission denied\n", dir_tree->current->name);
                return NULL;
            }
            temp_node = dir_exists(dir_tree, command, 'd');
            temp_node2 = dir_exists(dir_tree, command, 'f');
            if (!temp_node && !temp_node2) {
                printf("cat: '%s': No such file or directory.\n", command);
                return NULL;
            } else if (temp_node && !temp_node2) {
                printf("cat: '%s': Is a directory\n", command);
                return NULL;
            } else if (temp_node2 && check_permission(temp_node2, 'r')) {
                printf("cat: Can not open file '%s': Permission denied\n", temp_node2->name);
                return NULL;
            } else concatenate(dir_tree, command, 1);
        } else {
            strncpy(temp2, get_dir(temp), MAX_DIR);
            value = change_path(dir_tree, temp2);
            if (value) {
                printf("cat: '%s': No such file or directory.\n", temp2);
                return NULL;
            }
            str = strtok(temp, "/");
            while (str) {
                strncpy(temp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            temp_node = dir_exists(dir_tree, temp3, 'd');
            temp_node2 = dir_exists(dir_tree, temp3, 'f');
            if (!temp_node && !temp_node2) {
                printf("cat: '%s': No such file or directory.\n", temp3);
                dir_tree->current = current_node;
                return NULL;
            } else if (temp_node && !temp_node2) {
                printf("cat: '%s': Is a directory\n", temp3);
                dir_tree->current = current_node;
                return NULL;
            } else if (temp_node2 && check_permission(temp_node2, 'r')) {
                printf("cat: Can not open file '%s': Permission denied\n", temp_node2->name);
                dir_tree->current = current_node;
                return NULL;
            } else concatenate(dir_tree, temp3, 1);
            dir_tree->current = current_node;
        }
    }
    pthread_exit(NULL);
}

void *tar_using_thread(void *arg) {
    ThreadTree *thread_tree = ((ThreadTree *)arg);
    DirectoryTree *dir_tree = thread_tree->thread_tree;
    DirectoryTree *p_pre_tree;
    char *command = thread_tree->command;
    char *content = thread_tree->content;
    DirectoryNode *temp_node = dir_tree->current;
    char temp[MAX_DIR];
    char p_str[MAX_DIR];
    char temp_str[MAX_DIR];
    char directory_name[MAX_DIR];
    int directory_name_length = 0;
    int is_directory_exist;
    boolean flag = TRUE;
    
    char temp_content[20];
    strncpy(temp_content, content, 20);
    char* check_content = strtok(temp_content," ");
    while(check_content){
        printf("a %s\n",check_content);
        if (!dir_tree->current->first_child)
        {
            printf("file not exists\n");
            
        }
        else
        {
            temp_node = dir_tree->current->first_child;
            while(temp_node){
                if(strcmp(temp_node->name,check_content)==0){
                    break;
                }
                temp_node = temp_node->next_sibling;
                if(!temp_node){
                    printf("files not exist\n");
                    return (void*)FAIL;
                }
            }
        }check_content=strtok(NULL, " ");
    }

    strncpy(temp, command, MAX_DIR);

    if (strstr(command, "/") == NULL) {
        make_tar(dir_tree, command, content, 'a');
    } else if (thread_tree->option == 1) {
        int temp_len = strlen(temp), i = 0;
        if (temp[0] == '/') {
            dir_tree->current = dir_tree->root;
            i = 1;
        }
        if (temp[temp_len - 1] == '/') {
            temp_len -= 1;
        }
        for (; i < temp_len; i++) {
            p_str[i] = temp[i];
            p_str[i + 1] = 0;
            directory_name[directory_name_length++] = temp[i];
            if (temp[i] == '/') {
                directory_name[--directory_name_length] = 0;
                strncpy(temp_str, p_str, i - 1);
                is_directory_exist = change_current_directory(dir_tree, directory_name);
                if (is_directory_exist == -1) {
                    make_tar(dir_tree, directory_name, content, 'a');
                    is_directory_exist = change_current_directory(dir_tree, directory_name);
                }
                directory_name_length = 0;
            }
        }
        directory_name[directory_name_length] = 0;
        make_tar(dir_tree, directory_name, content, 'a');
        dir_tree->current = temp_node;
    } else {
        char *p_get_directory = get_dir(command);
        is_directory_exist = change_path(dir_tree, p_get_directory);
        if (is_directory_exist != 0) {
            printf("mkdir: '%s': No such file or directory.\n", p_get_directory);
        } else {
            char *str = strtok(temp, "/");
            char *p_directory_name;
            while (str != NULL) {
                p_directory_name = str;
                str = strtok(NULL, "/");
            }
            make_tar(dir_tree, p_directory_name, content, 'a');
            dir_tree->current = temp_node;
        }
    }
    pthread_exit(NULL);
}

void *mkdir_using_thread(void *arg) {
    ThreadTree *thread_tree = ((ThreadTree *)arg);
    DirectoryTree *dir_tree = thread_tree->thread_tree;
    DirectoryTree *p_pre_tree;
    char *command = thread_tree->command;
    
    DirectoryNode *temp_node = dir_tree->current;
    char temp[MAX_DIR];
    char p_str[MAX_DIR];
    char temp_str[MAX_DIR];
    char directory_name[MAX_DIR];
    int directory_name_length = 0;
    int is_directory_exist;

    strncpy(temp, command, MAX_DIR);

    if (strstr(command, "/") == NULL) {
        make_dir(dir_tree, command, 'd');
    } else if (thread_tree->option == 1) {
        int temp_len = strlen(temp), i = 0;
        if (temp[0] == '/') {
            dir_tree->current = dir_tree->root;
            i = 1;
        }
        if (temp[temp_len - 1] == '/') {
            temp_len -= 1;
        }
        for (; i < temp_len; i++) {
            p_str[i] = temp[i];
            p_str[i + 1] = 0;
            directory_name[directory_name_length++] = temp[i];
            if (temp[i] == '/') {
                directory_name[--directory_name_length] = 0;
                strncpy(temp_str, p_str, i - 1);
                is_directory_exist = change_current_directory(dir_tree, directory_name);
                if (is_directory_exist == -1) {
                    make_dir(dir_tree, directory_name, 'd');
                    is_directory_exist = change_current_directory(dir_tree, directory_name);
                }
                directory_name_length = 0;
            }
        }
        directory_name[directory_name_length] = 0;
        make_dir(dir_tree, directory_name, 'd');
        dir_tree->current = temp_node;
    } else {
        char *p_get_directory = get_dir(command);
        is_directory_exist = change_path(dir_tree, p_get_directory);
        if (is_directory_exist != 0) {
            printf("mkdir: '%s': No such file or directory.\n", p_get_directory);
        } else {
            char *str = strtok(temp, "/");
            char *p_directory_name;
            while (str != NULL) {
                p_directory_name = str;
                str = strtok(NULL, "/");
            }
            make_dir(dir_tree, p_directory_name, 'd');
            dir_tree->current = temp_node;
        }
    }
    pthread_exit(NULL);
}

void *rmdir_using_thread(void *arg) {
    ThreadTree *thread_tree = (ThreadTree *)arg;
    DirectoryTree *dir_tree = thread_tree->thread_tree;

    DirectoryNode *temp_node = NULL;
    DirectoryNode *temp_node2 = NULL;
    DirectoryNode *temp_node3 = NULL;

    char *command = thread_tree->command;
    char temp[MAX_DIR];
    char temp2[MAX_DIR];
    char temp3[MAX_DIR];
    DirectoryNode *current_node = dir_tree->current;

    strncpy(temp, command, MAX_DIR);
    int option = thread_tree->option;

    if (option == 0) {
        if (!strstr(temp, "/")) {
            temp_node = dir_exists(dir_tree, temp, 'f');
            temp_node2 = dir_exists(dir_tree, temp, 'd');
            temp_node3 = dir_exists(dir_tree, temp, 'a');
            if (!temp_node && !temp_node2 && !temp_node3) {
                printf("rm: Can not remove '%s': No such file or directory.\n", temp);
                //printf("DEBUG\n");
                return NULL;
            }
            if (temp_node2) {
                printf("rm: %s: is a directory.\n", command);
                
                return NULL;
            } else {
                //printf("DEBUG\n");
                if (check_permission(dir_tree->current, 'w') || temp_node? check_permission(temp_node, 'w') : check_permission(temp_node3, 'w')) {
                    printf("rm: Can not remove '%s': Permission denied.\n", command);
                    return NULL;
                }
                remove_dir(dir_tree, temp);
            }
        } else {
            strncpy(temp2, get_dir(temp), MAX_DIR);
            int value = change_path(dir_tree, temp2);
            if (value) {
                printf("rm: Can not remove '%s': No such file or directory.\n", temp2);
                return NULL;
            }
            char *str = strtok(command, "/");
            while (str) {
                strncpy(temp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            temp_node = dir_exists(dir_tree, temp3, 'f');
            temp_node2 = dir_exists(dir_tree, temp3, 'd');
            temp_node3 = dir_exists(dir_tree, temp, 'a');
            if (temp_node2) {
                printf("rm: %s: Is a directory.\n", temp3);
                dir_tree->current = current_node;
                return NULL;
            }
            if (!temp_node && !temp_node3) {
                printf("rm: Can not remove '%s' No such file or directory.\n", temp3);
                dir_tree->current = current_node;
                return NULL;
            } else {
                if (check_permission(dir_tree->current, 'w') || check_permission(temp_node, 'w')) {
                    printf("rm: Can not remove '%s' Permission denied.\n", temp3);
                    dir_tree->current = current_node;
                    return NULL;
                }
                remove_dir(dir_tree, temp3);
            }
            dir_tree->current = current_node;
        }
    } else if (option == 1) {
        if (!strstr(temp, "/")) {
            temp_node = dir_exists(dir_tree, temp, 'f');
            temp_node = dir_exists(dir_tree, temp, 'd') == NULL ? temp_node : dir_exists(dir_tree, temp, 'd');
            if (!temp_node) {
                printf("rm: Can not remove '%s': No such file or directory.\n", temp);
                return NULL;
            } else {
                if (check_permission(dir_tree->current, 'w') || check_permission(temp_node, 'w')) {
                    printf("rm: failed to remove '%s'Can not remove directory or file: Permission denied.", temp);
                    return NULL;
                }
                remove_dir(dir_tree, temp);
            }
        } else {
            strncpy(temp2, get_dir(temp), MAX_DIR);
            int value = change_path(dir_tree, temp2);
            if (value) {
                printf("rm: '%s': No such file or directory.\n", temp2);
                return NULL;
            }
            char *str = strtok(command, "/");
            while (str) {
                strncpy(temp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            temp_node = dir_exists(dir_tree, temp3, 'f');
            temp_node = dir_exists(dir_tree, temp3, 'd') == NULL ? temp_node : dir_exists(dir_tree, temp3, 'd');
            if (temp_node) {
                printf("rm: Can not remove '%s': No such file or directory.\n", temp3);
                dir_tree->current = current_node;
                return NULL;
            } else {
                if (check_permission(dir_tree->current, 'w') || check_permission(temp_node, 'w')) {
                    printf("rm: failed to remove '%s' Can not remove directory or file: Permission denied.\n", temp3);
                    dir_tree->current = current_node;
                    return NULL;
                }
                remove_dir(dir_tree, temp3);
            }
            dir_tree->current = current_node;
        }
    } else if (option == 2) {
        if (!strstr(temp, "/")) {
            temp_node = dir_exists(dir_tree, temp, 'f');
            temp_node = dir_exists(dir_tree, temp, 'd') == NULL ? temp_node : dir_exists(dir_tree, temp, 'd');
            if (!temp_node) {
                return NULL;
            } else {
                if (check_permission(dir_tree->current, 'w') || check_permission(temp_node, 'w')) {
                    return NULL;
                }
                remove_dir(dir_tree, temp);
            }
        } else {
            strncpy(temp2, get_dir(temp), MAX_DIR);
            int value = change_path(dir_tree, temp2);
            if (value) return NULL;
            char *str = strtok(command, "/");
            while (str) {
                strncpy(temp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            temp_node = dir_exists(dir_tree, temp3, 'f');
            if (!temp_node) {
                dir_tree->current = current_node;
                return NULL;
            } else {
                if (check_permission(dir_tree->current, 'w') || check_permission(temp_node, 'w')) {
                    dir_tree->current = current_node;
                    return NULL;
                }
                remove_dir(dir_tree, temp3);
            }
            dir_tree->current = current_node;
        }
    } else if (option == 3) {
        if (!strstr(temp, "/")) {
            temp_node = dir_exists(dir_tree, temp, 'f');
            temp_node = dir_exists(dir_tree, temp, 'd') == NULL ? temp_node : dir_exists(dir_tree, temp, 'd');
            if (!temp_node) {
                return NULL;
            } else {
                if (check_permission(dir_tree->current, 'w') || check_permission(temp_node, 'w')) {
                    return NULL;
                }
                remove_dir(dir_tree, temp);
            }
        } else {
            strncpy(temp2, get_dir(temp), MAX_DIR);
            int value = change_path(dir_tree, temp2);
            if (value) return NULL;
            char *str = strtok(command, "/");
            while (str) {
                strncpy(temp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            temp_node = dir_exists(dir_tree, temp3, 'f');
            temp_node = dir_exists(dir_tree, temp3, 'd') == NULL ? temp_node : dir_exists(dir_tree, temp3, 'd');
            if (!temp_node) {
                dir_tree->current = current_node;
                return NULL;
            } else {
                if (check_permission(dir_tree->current, 'w') || check_permission(temp_node, 'w')) {
                    dir_tree->current = current_node;
                    return NULL;
                }
                remove_dir(dir_tree, temp3);
            }
            dir_tree->current = current_node;
        }
    }
    pthread_exit(NULL);
}

void *chown_using_thread(void *arg) {
    ThreadTree *thread_tree = (ThreadTree *)arg;
    DirectoryTree *dir_tree = thread_tree->thread_tree;
    char *command = thread_tree->command;
    char *temp = thread_tree->usr_name;
    char *str;
    char temp3[MAX_NAME];
    
    strcpy(temp3, temp);

    if (!strstr(temp, ":")) change_owner(dir_tree, temp, command, 0);
    else {
        char temp2[MAX_NAME];
        strncpy(temp2, temp3, MAX_NAME);
        char *str2 = strtok(temp3, ":");
        if (str2 && strcmp(temp3, temp2)) {
            change_owner(dir_tree, str2, command, 0);
            str2 = strtok(NULL, " ");
            if (str2) change_owner(dir_tree, str2, command, 1);
        }
        else if (str2 && !strcmp(temp3, temp2)) change_owner(dir_tree, str2, command, 1);
    }
    pthread_exit(NULL);
}

#include "../include/main.h"

#include "../include/main.h"


DirectoryTree *load_directory_tree() {
    DirectoryTree *dir_tree = (DirectoryTree *)malloc(sizeof(DirectoryTree));
    char temp[MAX_LENGTH];

    dir = fopen("file/Directory.txt", "r");
    if (dir == NULL) {
        printf("Cannot open Directory.txt file for reading\n");
        return NULL;
    }
    
    while (fgets(temp, MAX_LENGTH, dir)) {
        if (strcmp(temp, "\n") == 0) {
            continue;
        }
        read_dir_node(dir_tree, temp);
    }
    fclose(dir);
    
    dir_tree->current = dir_tree->root;

    return dir_tree;
}


void save_directory_tree(DirectoryTree *dir_tree, Stack *dir_stack) {
    dir = fopen("file/Directory.txt", "w");
    if (dir == NULL) {
        printf("Cannot open Directory.txt file for writing\n");
        return;
    }
    
    write_dir_node(dir_tree, dir_tree->root, dir_stack);
    
    fclose(dir);
}


DirectoryNode *dir_exists(DirectoryTree *dir_tree, char *dir_name, char type) {
    for (DirectoryNode *current = dir_tree->current->first_child; current; current = current->next_sibling) {
        if (!strcmp(current->name, dir_name) && current->type == type) {
            return current;
        }
    }
    return NULL;
}


char *get_dir(char *dir_path) {
    char *str, *temp_path = (char *)malloc(MAX_DIR);
    char temp[MAX_DIR], temp2[MAX_DIR];

    strncpy(temp, dir_path, MAX_DIR);

    for (str = strtok(dir_path, "/"); str; str = strtok(NULL, "/")) {
        strncpy(temp2, str, MAX_DIR);
    }

    strncpy(temp_path, temp, strlen(temp) - strlen(temp2) - 1);
    temp_path[strlen(temp) - strlen(temp2) - 1] = '\0';
    return temp_path;
}


void get_path(DirectoryTree *dir_tree, DirectoryNode *dir_node, Stack *dir_stack, char *temp) {
    DirectoryNode *temp_node = dir_node->parent;

    if (temp_node == dir_tree->root) {
        strcpy(temp, "/");
    } else {
        while (temp_node->parent) {
            push_stack(dir_stack, temp_node->name);
            temp_node = temp_node->parent;
        }
        while (!is_empty(dir_stack)) {
            strcat(temp, "/");
            strcat(temp, pop_stack(dir_stack));
        }
    }
    fprintf(dir, " %s", temp);
    if (dir_node->type == 'a') {
        fprintf(dir, " %s", dir_node->content);
    }
}


void write_dir_node(DirectoryTree *dir_tree, DirectoryNode *dir_node, Stack *dir_stack) {
    char temp[MAX_DIR] = "";
    fprintf(dir, "%s %c %d ", dir_node->name, dir_node->type, dir_node->permission.mode);
    fprintf(dir, "%d %d %d %d %d %d %d", dir_node->size, dir_node->id.uid, dir_node->id.gid, dir_node->date.month, dir_node->date.day, dir_node->date.hour, dir_node->date.minute);

    if (dir_node != dir_tree->root) {
        get_path(dir_tree, dir_node, dir_stack, temp);
    }
    fprintf(dir, "\n");

    if (dir_node->next_sibling) {
        write_dir_node(dir_tree, dir_node->next_sibling, dir_stack);
    }
    if (dir_node->first_child) {
        write_dir_node(dir_tree, dir_node->first_child, dir_stack);
    }
}


void create_and_attach_node(DirectoryTree *dir_tree, char *str, DirectoryNode *new_node, DirectoryNode *temp_node) {
    if (str) {
        if (new_node->type != 'a') {
            str[strlen(str) - 1] = '\0';
        }
        change_path(dir_tree, str);
        new_node->parent = dir_tree->current;
        if (!dir_tree->current->first_child) {
            dir_tree->current->first_child = new_node;
        } else {
            temp_node = dir_tree->current->first_child;
            while (temp_node->next_sibling) {
                temp_node = temp_node->next_sibling;
            }
            temp_node->next_sibling = new_node;
        }
    } else {
        dir_tree->root = new_node;
        dir_tree->current = dir_tree->root;
    }
}


int read_dir_node(DirectoryTree *dir_tree, char *temp) {
    DirectoryNode *new_node = (DirectoryNode *)malloc(sizeof(DirectoryNode)), *temp_node = NULL;
    char *str;

    new_node->first_child = NULL;
    new_node->next_sibling = NULL;
    new_node->parent = NULL;

    str = strtok(temp, " ");
    strncpy(new_node->name, str, MAX_NAME);
    str = strtok(NULL, " ");
    new_node->type = str[0];
    str = strtok(NULL, " ");
    new_node->permission.mode = atoi(str);
    mode_to_permission(new_node);
    str = strtok(NULL, " ");
    new_node->size = atoi(str);
    str = strtok(NULL, " ");
    new_node->id.uid = atoi(str);
    str = strtok(NULL, " ");
    new_node->id.gid = atoi(str);
    str = strtok(NULL, " ");
    new_node->date.month = atoi(str);
    str = strtok(NULL, " ");
    new_node->date.day = atoi(str);
    str = strtok(NULL, " ");
    new_node->date.hour = atoi(str);
    str = strtok(NULL, " ");
    new_node->date.minute = atoi(str);
    str = strtok(NULL, " ");

    if (new_node->type == 'a') {
        strncpy(new_node->content, str + (strlen(str) + 1), 20);
        new_node->content[strlen(new_node->content) - 1] = '\0';
    }
    create_and_attach_node(dir_tree, str, new_node, temp_node);
    return 0;
}


void save_user_list(UserList *user_list) {
    User = fopen("file/User.txt", "w");
    if (User == NULL) {
        printf("Cannot open User.txt file for writing\n");
        return;
    }
    write_user(user_list, user_list->head);
    fclose(User);
}

UserList *load_user_list() {
    UserList *user_list = (UserList *)malloc(sizeof(UserList));
    char temp[MAX_LENGTH];

    User = fopen("file/User.txt", "r");
    if (User == NULL) {
        printf("Cannot open User.txt file for reading\n");
        free(user_list);
        return NULL;
    }
    
    while (fgets(temp, MAX_LENGTH, User)) {
        read_user(user_list, temp);
    }
    fclose(User);
    user_list->current = NULL;
    return user_list;
}

void write_user(UserList *user_list, UserNode *user_node) {
    time(&ltime);
    today = localtime(&ltime);

    user_list->current->date.year = today->tm_year + 1900;
    user_list->current->date.month = today->tm_mon + 1;
    user_list->current->date.weekday = today->tm_wday;
    user_list->current->date.day = today->tm_mday;
    user_list->current->date.hour = today->tm_hour;
    user_list->current->date.minute = today->tm_min;
    user_list->current->date.second = today->tm_sec;

    fprintf(User, "%s %d %d %d %d %d %d %d %d %d %s\n",
            user_node->name, user_node->id.uid, user_node->id.gid, user_node->date.year,
            user_node->date.month, user_node->date.weekday, user_node->date.day, user_node->date.hour,
            user_node->date.minute, user_node->date.second, user_node->dir);

    if (user_node->next_node) write_user(user_list, user_node->next_node);
}

int read_user(UserList *user_list, char *temp) {
    UserNode *new_node = (UserNode *)malloc(sizeof(UserNode));
    char *str;

    new_node->next_node = NULL;
    str = strtok(temp, " ");
    strncpy(new_node->name, str, MAX_NAME);
    str = strtok(NULL, " ");
    new_node->id.uid = atoi(str);
    str = strtok(NULL, " ");
    new_node->id.gid = atoi(str);
    str = strtok(NULL, " ");
    new_node->date.year = atoi(str);
    str = strtok(NULL, " ");
    new_node->date.month = atoi(str);
    str = strtok(NULL, " ");
    new_node->date.weekday = atoi(str);
    str = strtok(NULL, " ");
    new_node->date.day = atoi(str);
    str = strtok(NULL, " ");
    new_node->date.hour = atoi(str);
    str = strtok(NULL, " ");
    new_node->date.minute = atoi(str);
    str = strtok(NULL, " ");
    new_node->date.second = atoi(str);
    str = strtok(NULL, " ");
    str[strlen(str) - 1] = '\0';
    strncpy(new_node->dir, str, MAX_DIR);

    if (!strcmp(new_node->name, "root")) {
        user_list->head = new_node;
        user_list->tail = new_node;
    } else {
        user_list->tail->next_node = new_node;
        user_list->tail = new_node;
    }
    return SUCCESS;
}

UserNode *user_exists(UserList *user_list, char *user_name) {
    UserNode *return_user = user_list->head;

    while (return_user) {
        if (!strcmp(return_user->name, user_name)) break;
        return_user = return_user->next_node;
    }
    return return_user;
}

char *get_uid(DirectoryNode *dir_node) {
    UserNode *temp_node = usr_list->head;

    while (temp_node) {
        if (temp_node->id.uid == dir_node->id.uid) break;
        temp_node = temp_node->next_node;
    }
    return temp_node->name;
}

char *get_gid(DirectoryNode *dir_node) {
    UserNode *temp_node = usr_list->head;

    while (temp_node) {
        if (temp_node->id.gid == dir_node->id.gid) break;
        temp_node = temp_node->next_node;
    }
    return temp_node->name;
}


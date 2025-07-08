#include "../include/main.h"


#define SUCCESS 0
#define FAIL -1

int mode_to_permission(DirectoryNode *dir_node) {
    if (dir_node == NULL) return FAIL;

    
    for (int i = 0; i < 9; i++) {
        dir_node->permission.permission[i] = 0;
    }

    
    int mode = dir_node->permission.mode;
    for (int i = 2; i >= 0; i--) {
        int temp = mode % 10;
        mode /= 10;
        for (int j = 2; j >= 0; j--) {
            dir_node->permission.permission[3 * i + j] = temp % 2;
            temp /= 2;
        }
    }

    return SUCCESS;
}

void print_permission(DirectoryNode *dir_node) {
    if (dir_node == NULL) return;

    char rwx[4] = "rwx";
    for (int i = 0; i < 9; i++) {
        if (dir_node->permission.permission[i]) {
            printf("%c", rwx[i % 3]);
        } else {
            printf("-");
        }
    }
}

int check_permission(DirectoryNode *dir_node, char mode) {
    if (dir_node == NULL || usr_list == NULL || usr_list->current == NULL) return FAIL;

    
    if (usr_list->current->id.uid == 0) return SUCCESS;

    int *permissions = dir_node->permission.permission;
    int user_check[3] = {0, 1, 2};
    int group_check[3] = {3, 4, 5};
    int others_check[3] = {6, 7, 8};

    int *check_set;
    if (usr_list->current->id.uid == dir_node->id.uid) {
        check_set = user_check;
    } else if (usr_list->current->id.gid == dir_node->id.gid) {
        check_set = group_check;
    } else {
        check_set = others_check;
    }

    switch (mode) {
        case 'r':
            return permissions[check_set[0]] ? SUCCESS : FAIL;
        case 'w':
            return permissions[check_set[1]] ? SUCCESS : FAIL;
        case 'x':
            return permissions[check_set[2]] ? SUCCESS : FAIL;
        default:
            return FAIL;
    }
}
#include "../include/main.h"


void display_help_ls() {
    printf("Usage: ls [OPTION]... [FILE]...\n");
    printf("List information about the FILEs (the current directory by default).\n\n");
    printf("  Options:\n");
    printf("  -a, --all                  do not ignore entries starting with .\n");
    printf("  -l                         use a long listing format\n");
    printf("      --help     display this help and exit\n");
}


void display_error_ls(const char *error_type, const char *message) {
    if (!strcmp(error_type, "cannot_access")) {
        printf("ls: cannot access '%s': No such file or directory\n", message);
    } else if (!strcmp(error_type, "invalid_option")) {
        printf("ls: invalid option -- '%s'\n", message);
        printf("Try 'ls --help' for more information.\n");
    }
}


void print_dir_details(DirectoryNode *dir_node, int cnt) {
    printf("%c", dir_node->type=='d' ? 'd':'-');
    print_permission(dir_node);
    printf("%3d   ", cnt);
    printf("%-5s%-5s", get_uid(dir_node), get_gid(dir_node));
    printf("%5d ", dir_node->size); 
    get_month(dir_node->date.month);
    printf("%2d %02d:%02d ", dir_node->date.day, dir_node->date.hour, dir_node->date.minute);
}


void print_dir_name_with_color(DirectoryNode *dir_node) {
    if (dir_node->type == 'd') {
        BOLD; BLUE;
        printf("%s\t", dir_node->name);
        DEFAULT;
    } else {
        printf("%s\t", dir_node->name);
    }
}


void print_child_dirs(DirectoryNode *dir_node, int option_a) {
    DirectoryNode *temp_node = dir_node->first_child;
    while (temp_node) {
        if (!option_a && !strncmp(temp_node->name, ".", 1)) {
            temp_node = temp_node->next_sibling;
            continue;
        }
        print_dir_name_with_color(temp_node);
        temp_node = temp_node->next_sibling;
    }
    printf("\n");
}


int get_child_count(DirectoryNode *dir_node) {
    int cnt = 0;
    DirectoryNode *temp_node = dir_node->first_child;
    if (!temp_node) {
        cnt = (dir_node->type == 'd') ? 2 : 1;
    } else {
        cnt = (temp_node->type == 'd') ? 3 : 2;
        while (temp_node->next_sibling) {
            temp_node = temp_node->next_sibling;
            if (temp_node->type == 'd') cnt++;
        }
    }
    return cnt;
}


int list_dir_simple(DirectoryTree *dir_tree, int option_a) {
    DirectoryNode *temp_node = dir_tree->current->first_child;
    if (!option_a && !temp_node) return FAIL;
    if (option_a) {
        BOLD; BLUE;
        printf(".\t");
        DEFAULT;
        if (dir_tree->current != dir_tree->root) {
            BOLD; BLUE;
            printf("..\t");
            DEFAULT;
        }
    }
    print_child_dirs(dir_tree->current, option_a);
    return SUCCESS;
}


int list_dir_detailed(DirectoryTree *dir_tree, int option_a) {
    DirectoryNode *temp_node = dir_tree->current->first_child;
    if (!option_a && !temp_node) {
        return FAIL;
    }
    if (option_a) {
        int cnt = get_child_count(dir_tree->current);
        print_dir_details(dir_tree->current, cnt);
        BOLD; BLUE;
        printf(".\n");
        DEFAULT;
        if (dir_tree->current != dir_tree->root) {
            cnt = get_child_count(dir_tree->current->parent);
            print_dir_details(dir_tree->current->parent, cnt);
            BOLD; BLUE;
            printf("..\n");
            DEFAULT;
        }
    }
    while (temp_node) {
        if (!option_a && !strncmp(temp_node->name, ".", 1)) {
            temp_node = temp_node->next_sibling;
            continue;
        }
        int cnt = get_child_count(temp_node);
        print_dir_details(temp_node, cnt);
        print_dir_name_with_color(temp_node);
        printf("\n");
        temp_node = temp_node->next_sibling;
    }
    return SUCCESS;
}

int print_help() {
    display_help_ls();
    return FAIL;
}

int handle_ls_options(DirectoryTree *dir_tree, char *command, DirectoryNode *current_node) {
    char *str = strtok(NULL, " ");
    int value;

    if (str) {
        value = change_path(dir_tree, str);
        if (value) return FAIL;
    }
    if (!strcmp(command, "-al") || !strcmp(command, "-la")) {
        list_dir_detailed(dir_tree, 1);
        dir_tree->current = current_node;
        return SUCCESS;
    } else if (!strcmp(command, "-l")) {
        list_dir_detailed(dir_tree, 0);
        dir_tree->current = current_node;
        return SUCCESS;
    } else if (!strcmp(command, "-a")) {
        list_dir_simple(dir_tree, 1);
        dir_tree->current = current_node;
        return SUCCESS;
    } else if (!strcmp(command, "--help")) {
        return print_help();
    } else {
        str = strtok(command, "-");
        if (!str) {
            display_error_ls("cannot_access", "-");
        } else {
            display_error_ls("invalid_option", str);
        }
        dir_tree->current = current_node;
        return FAIL;
    }
}

int handle_ls_command(DirectoryTree *dir_tree, char *command, DirectoryNode *current_node) {
    int value = change_path(dir_tree, command);
    if (value) return FAIL;
    int result = list_dir_simple(dir_tree, 0);
    dir_tree->current = current_node;
    return result;
}


int execute_ls(DirectoryTree *dir_tree, char *command) {
    DirectoryNode *current_node = dir_tree->current;

    if (!command) {
        return list_dir_simple(dir_tree, 0);
    }
    if (command[0] == '-') {
        return handle_ls_options(dir_tree, command, current_node);
    } else {
        return handle_ls_command(dir_tree, command, current_node);
    }
}
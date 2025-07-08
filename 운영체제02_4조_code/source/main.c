#include "../include/main.h"


DirectoryTree *linux_dir_tree;
UserList *usr_list;
Stack *d_stack;
FILE *dir;
FILE *User;
time_t ltime;
struct tm *today;

int main() {
    char command[50];

    linux_dir_tree = load_directory_tree(); 
    usr_list = load_user_list(); 
    d_stack = init_stack(); 

    login(usr_list, linux_dir_tree); 
    print_start(); 
    save_user_list(usr_list); 

    while (TRUE) {
        print_prompt(linux_dir_tree, d_stack); 
        fgets(command, sizeof(command), stdin); 
        command[strlen(command) - 1] = '\0'; 
        execute_command(linux_dir_tree, command); 
    }
    return 0;
}


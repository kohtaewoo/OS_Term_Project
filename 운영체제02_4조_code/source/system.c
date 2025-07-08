#include "../include/main.h"

void execute_command(DirectoryTree *dir_tree, char *command) {
    char *str;
    Deque dir_deque; 
    int value;

    if (!strcmp(command, "") || command[0] == ' ') return;

    init_deque(&dir_deque); 
    str = strtok(command, " ");

    if (!strcmp(str, "clear")) {
        printf("\033[H\033[2J");
    } else if (!strcmp(str, "mkdir")) {
        str = strtok(NULL, " ");
        value = execute_mkdir(dir_tree, str);
        if (!value) save_directory_tree(dir_tree, d_stack);
    } else if (!strcmp(str, "rm")) {
        str = strtok(NULL, " ");
        value = execute_rm(dir_tree, str);
        if (!value) save_directory_tree(dir_tree, d_stack);
    } else if (!strcmp(str, "cd")) {
        str = strtok(NULL, " ");
        execute_cd(dir_tree, str);
    } else if (!strcmp(str, "ls")) {
        str = strtok(NULL, " ");
        execute_ls(dir_tree, str);
    } else if (!strcmp(str, "cat")) {
        str = strtok(NULL, " ");
        value = execute_cat(dir_tree, str);
        if (!value) save_directory_tree(dir_tree, d_stack);
    } else if(!strcmp(str, "chown")) {
        str = strtok(NULL, " ");
        value = execute_chown(dir_tree, str);
        if (!value) save_directory_tree(dir_tree, d_stack);
    } else if(!strcmp(str, "tar")) {
        str = strtok(NULL, " ");
        value = execute_tar(dir_tree, str);
        if (!value) save_directory_tree(dir_tree, d_stack);
    } else if (!strcmp(str, "find")) {
        str = strtok(NULL, " ");
        execute_find(dir_tree, str);
    } else if (strcmp(str, "sl_dongguk") == 0) {
        system("./art_animation"); 
    } else if (!strcmp(str, "pwd")) {
        execute_pwd(dir_tree, &dir_deque, NULL); 
    } else if (!strcmp(str, "help")) {
        print_help_msg();
    } else if (!strcmp(str, "exit")) {
        printf("logout\n");
        exit(0);
    } else {
        printf("Command '%s' not found\n", str);
    }
    return;
}



void get_month(int type) {
    const char *months[] = {
        "Invalid", "Jan ", "Feb ", "Mar ", "Apr ", "May ", "Jun ",
        "Jul ", "Aug ", "Sep ", "Oct ", "Nov ", "Dec "
    };
    if (type >= 1 && type <= 12) {
        printf("%s", months[type]);
    } else {
        printf("Invalid ");
    }
}


void get_weekday(int type) {
    const char *weekdays[] = {
        "Sun ", "Mon ", "Tue ", "Wed ", "Thu ", "Fri ", "Sat "
    };
    if (type >= 0 && type <= 6) {
        printf("%s", weekdays[type]);
    } else {
        printf("Invalid ");
    }
}

void print_help_msg() {
    printf("You can use the command displayed below.\n");
    printf("\t- ls        -l, -a, -al\n");
    printf("\t- cat       >, -n\n");
    printf("\t- cd\n");
    printf("\t- mkdir     -p\n");
    printf("\t- clear\n");
    printf("\t- rm        -r, -f, -rf\n");
    printf("\t- chown\n");
    printf("\t- find      -name\n");
    printf("\t- pwd\n");
    printf("\t- sl_dongguk\n");
    printf("\t- tar       -cvf, -xvf\n\n");
    printf("The 'exit' command allows you to exit the program.\n\n");
}


void get_today(Date *date) {
    time(&ltime);
    today = localtime(&ltime);

    date->weekday = today->tm_wday;
    date->month = today->tm_mon + 1;
    date->day = today->tm_mday;
    date->hour = today->tm_hour;
    date->minute = today->tm_min;
    date->second = today->tm_sec;
    date->year = today->tm_year + 1900;
}


void print_start() {
    Date temp_date;
    get_today(&temp_date);
    printf("Welcome to OS_Team4 LINUXSHELL\n\n");
    printf("System information as of ");
    get_weekday(temp_date.weekday);
    get_month(temp_date.month);
    printf("%d %02d:%02d:%02d UTC %d\n\n", temp_date.day, temp_date.hour, temp_date.minute, temp_date.second, temp_date.year);
    printf("You can use the command displayed below.\n");
    printf("\t- ls        -l, -a, -al\n");
    printf("\t- cat       >, -n\n");
    printf("\t- cd\n");
    printf("\t- mkdir     -p\n");
    printf("\t- clear\n");
    printf("\t- rm        -r, -f, -rf\n");
    printf("\t- chown\n");
    printf("\t- find      -name\n");
    printf("\t- pwd\n");
    printf("\t- sl_dongguk\n");
    printf("\t- tar       -cvf, -xvf\n\n");
    printf("The 'exit' command allows you to exit the program.\n\n");
    printf("Last login: ");
    get_weekday(usr_list->current->date.weekday);
    get_month(usr_list->current->date.month);
    printf("%d %02d:%02d:%02d %d\n", usr_list->current->date.day, usr_list->current->date.hour, usr_list->current->date.minute, usr_list->current->date.second, usr_list->current->date.year);
}


void print_prompt(DirectoryTree *dir_tree, Stack *dir_stack) {
    DirectoryNode *temp_node = NULL;
    char temp[MAX_DIR] = "";
    char temp2[MAX_DIR] = "";
    char usr;

    if (usr_list->current == usr_list->head) {
        usr = '#'; 
        BOLD;
        printf("%s@OS_Project_bash_Team_4", usr_list->current->name);
        DEFAULT;
        printf(":");
    }
    else {
        usr = '$'; 
        BOLD;GREEN;
        printf("%s@OS_Project_bash_Team_4", usr_list->current->name);
        DEFAULT;
        printf(":");
    }
    temp_node = dir_tree->current;

    if(temp_node == dir_tree->root){
        strcpy(temp, "/");
    } else {
        while (temp_node->parent) {
            push_stack(dir_stack, temp_node->name);
            temp_node = temp_node->parent;
        }
        while (is_empty(dir_stack) == FALSE) {
            strcat(temp, "/");
            strcat(temp, pop_stack(dir_stack));
        }
    }
    strncpy(temp2, temp, strlen(usr_list->current->dir));
    if (usr_list->current == usr_list->head){
        BOLD;BLUE;
        printf("%s", temp);
    } else if (strcmp(usr_list->current->dir, temp2)) {
	    BOLD;BLUE;
        printf("%s", temp);
    } else {
        temp_node = dir_tree->current;
        while (temp_node->parent) {
            push_stack(dir_stack, temp_node->name);
            temp_node = temp_node->parent;
        }
        pop_stack(dir_stack);
        pop_stack(dir_stack);
	    BOLD;BLUE;
        printf("~");
        while (is_empty(dir_stack) == FALSE) {
            printf("/");
            printf("%s", pop_stack(dir_stack));
        }
    }
    DEFAULT;
    printf("%c ", usr);
}

void login(UserList *user_list, DirectoryTree *dir_tree) {
    UserNode *temp_user = NULL;
    char user_name[MAX_NAME];
    char temp[MAX_DIR];

    temp_user = user_list->head;

    printf("Users: ");
    while (temp_user) {
        printf("%s ", temp_user->name);
        temp_user = temp_user->next_node;
    }
    printf("\n");
    while (TRUE) {
        printf("Login as: ");
        fgets(user_name, sizeof(user_name), stdin);
        user_name[strlen(user_name) - 1] = '\0';
        if (!strcmp(user_name, "exit")) {
            exit(0);
        }
        temp_user = user_exists(user_list, user_name);
        if (temp_user) {
            user_list->current = temp_user;
            break;
        }
        printf("'%s' User not found\n", user_name);
    }
    strcpy(temp, user_list->current->dir);
    change_path(dir_tree, temp);
}

int print_content(DirectoryTree *dir_tree, char *search, char *file_name, int option) {
    DirectoryNode *temp_node = NULL;
    FILE *fp;
    char buf[MAX_BUFFER];
    int count = 1;

    temp_node = dir_exists(dir_tree, file_name, 'f');
    if (!temp_node) return FAIL;
    fp = fopen(file_name, "r");
    while (!feof(fp)) {
        fgets(buf, sizeof(buf), fp);
        if (feof(fp)) break;
        else if (option == 0) {
            if (strstr(buf, search)) printf("%s", buf);
        } else if (option == 1) {
            if (strstr(buf, search)) printf("%d:%s", count, buf);
        } else if (option == 2) {
            if (!strstr(buf, search)) printf("%s", buf);
        } else if (option == 3) {
            if (strcasestr(buf, search)) printf("%s", buf);
        } else if (option == 4) {
            if (!strstr(buf, search)) printf("%d:%s", count, buf);
        } else if (option == 5) {
            if (strcasestr(buf, search)) printf("%d:%s", count, buf);
        } else if (option == 6) {
            if (!strcasestr(buf, search)) printf("%s", buf);
        } else if (option == 7) {
            if (!strcasestr(buf, search)) printf("%d:%s", count, buf);
        }
        count++;
    }
    fclose(fp);
    return SUCCESS;
}


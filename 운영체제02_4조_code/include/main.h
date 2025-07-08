#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <ncurses.h>


#define DEFAULT printf("%c[%dm", 0x1B, 0)
#define BOLD printf("%c[%dm", 0x1B, 1)
#define WHITE printf("\x1b[37m")
#define BLUE printf("\x1b[34m")
#define GREEN printf("\x1b[32m")

#define MAX_BUFFER 512
#define MAX_LENGTH 200
#define MAX_DIR 50
#define MAX_NAME 20
#define MAX_THREAD 50
#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAIL -1

typedef int boolean;


typedef struct date {
    int year;
    int month;
    int weekday;
    int day;
    int hour;
    int minute;
    int second;
} Date;

extern time_t ltime;
extern struct tm *today;


typedef struct id {
    int uid;
    int gid;
} ID;

typedef struct user_node {
    char name[MAX_NAME];
    char dir[MAX_DIR];
    ID id;
    Date date;
    struct user_node *next_node;
} UserNode;

typedef struct user_list {
    ID top_id;
    UserNode *head;
    UserNode *tail;
    UserNode *current;
} UserList;


typedef struct permission {
    int mode;
    int permission[9];
} Permission;

typedef struct directory_node {
    char name[MAX_NAME];
    char type;
    int size;
    char content[20];
    Permission permission;
    ID id;
    Date date;
    struct directory_node *parent;
    struct directory_node *first_child;
    struct directory_node *next_sibling;
} DirectoryNode;

typedef struct directory_tree {
    DirectoryNode* root;
    DirectoryNode* current;
} DirectoryTree;

typedef struct stack_node {
    char name[MAX_NAME];
    struct stack_node *next_node;
} StackNode;

typedef struct stack {
    StackNode* top_node;
    int cnt;
} Stack;


typedef struct deque_node {
    char *data;
    struct deque_node *next;
    struct deque_node *prev;
} DequeNode;

typedef struct deque {
    DequeNode *front;
    DequeNode *rear;
} Deque;


typedef struct thread_tree {
    DirectoryTree *thread_tree;
    DirectoryTree *file_name;
    char *content;
    char *command;
    char *usr_name;
    int mode;
    int option;
} ThreadTree;

typedef struct {
    DirectoryTree* dir_tree;
    char* archive_name;
    char* file_path;
    int option; 
} TarThreadArgs;


void init_deque(Deque *dq);
int is_deque_empty(Deque *dq);
void push_front(Deque *dq, char *data);
void push_rear(Deque *dq, char *data);
char *pop_front(Deque *dq);
char *pop_rear(Deque *dq);


void write_user(UserList *user_list, UserNode *user_node);
int read_user(UserList *user_list, char *temp);
UserNode *user_exists(UserList *user_list, char *user_name);
char *get_uid(DirectoryNode *dir_node);
char *get_gid(DirectoryNode *dir_node);


UserList *load_user_list();
void save_user_list(UserList *user_list);


char *get_dir(char *dir_path);
void get_path(DirectoryTree *dir_tree, DirectoryNode *dir_node, Stack *dir_stack, char *temp);
void write_dir_node(DirectoryTree *dir_tree, DirectoryNode *dir_node, Stack *dir_stack);
int read_dir_node(DirectoryTree *dir_tree, char *temp);
DirectoryNode *dir_exists(DirectoryTree *dir_tree, char *dir_name, char type);


void save_directory_tree(DirectoryTree *dir_tree, Stack *dir_stack);
DirectoryTree *load_directory_tree();


int concatenate(DirectoryTree* dir_tree, char* f_name, int type);
int execute_cat(DirectoryTree* dir_tree, char* cmd);


int change_path(DirectoryTree* dir_tree, char* dir_path);
int change_current_directory(DirectoryTree* dir_tree, char* dir_path);
int execute_cd(DirectoryTree* dir_tree, char* cmd);
void display_help();


int change_owner(DirectoryTree *dir_tree, char *user_name, char *dir_name, int flag);
int execute_chown(DirectoryTree* dir_tree, char* cmd);


int read_dir(DirectoryTree *dir_tree, char *temp, char *dir_name, int o);
void find_dir(DirectoryTree *dir_tree, char *dir_name, int o);
int execute_find(DirectoryTree *dir_tree, char *cmd);


int execute_pwd(DirectoryTree *dir_tree, Deque *dir_deque, char *command);



int list_dir(DirectoryTree *dir_tree, int a, int l);
int execute_ls(DirectoryTree *dir_tree, char *cmd);


int make_dir(DirectoryTree *dir_tree, char *dir_name, char type);
int execute_mkdir(DirectoryTree *dir_tree, char *cmd);


void destroy_node(DirectoryNode *dir_node);
void destroy_dir(DirectoryNode *dir_node);
int remove_dir(DirectoryTree *dir_tree, char *dir_name);
int execute_rm(DirectoryTree *dir_tree, char *cmd);


int make_tar(DirectoryTree* dir_tree, char* dir_name, char* content, char type);
int execute_tar(DirectoryTree* dir_tree, char* command);


void execute_command(DirectoryTree* dir_tree, char* cmd);


void print_help_msg();
void get_today(Date *date);
void print_start();
void print_prompt(DirectoryTree *dir_tree, Stack *dir_stack);
void login(UserList *user_list, DirectoryTree *dir_tree);


int mode_to_permission(DirectoryNode *dir_node);
void print_permission(DirectoryNode *dir_node);
int check_permission(DirectoryNode *dir_node, char o);


void *mkdir_using_thread(void *arg);
void *rmdir_using_thread(void *arg);
void *touch_using_thread(void *arg);
void *cat_using_thread(void *arg);

void *tar_using_thread(void *arg);
void *chown_using_thread(void *arg);
void *grep_used_thread(void *arg);


boolean is_empty(Stack *stack);
Stack* init_stack();
int push_stack(Stack* dir_stack, char* dir_name);
char *pop_stack(Stack* dir_stack);


void get_month(int type);
void get_weekday(int type);


extern DirectoryTree *linux_dir_tree;
extern UserList *usr_list;
extern Stack *d_stack;
extern FILE *dir;
extern FILE *User;
extern time_t ltime;
extern struct tm *today;

#endif 


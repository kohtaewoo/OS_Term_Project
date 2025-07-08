#include "../include/main.h"

boolean is_empty(Stack* stack) {
    if(stack->top_node == NULL){
        return TRUE;
    }
    return FALSE;
}

void init_deque(Deque *dq) {
    dq->front = dq->rear = NULL;
}

int is_deque_empty(Deque *dq) {
    return dq->front == NULL;
}

void push_front(Deque *dq, char *data) {
    DequeNode *new_node = (DequeNode *)malloc(sizeof(DequeNode));
    new_node->data = strdup(data);
    new_node->next = dq->front;
    new_node->prev = NULL;
    if (is_deque_empty(dq)) {
        dq->front = dq->rear = new_node;
    } else {
        dq->front->prev = new_node;
        dq->front = new_node;
    }
}

void push_rear(Deque *dq, char *data) {
    DequeNode *new_node = (DequeNode *)malloc(sizeof(DequeNode));
    new_node->data = strdup(data);
    new_node->next = NULL;
    new_node->prev = dq->rear;
    if (is_deque_empty(dq)) {
        dq->front = dq->rear = new_node;
    } else {
        dq->rear->next = new_node;
        dq->rear = new_node;
    }
}

char *pop_front(Deque *dq) {
    if (is_deque_empty(dq)) return NULL;
    DequeNode *temp = dq->front;
    char *data = temp->data;
    dq->front = dq->front->next;
    if (dq->front == NULL) {
        dq->rear = NULL;
    } else {
        dq->front->prev = NULL;
    }
    free(temp);
    return data;
}

char *pop_rear(Deque *dq) {
    if (is_deque_empty(dq)) return NULL;
    DequeNode *temp = dq->rear;
    char *data = temp->data;
    dq->rear = dq->rear->prev;
    if (dq->rear == NULL) {
        dq->front = NULL;
    } else {
        dq->rear->next = NULL;
    }
    free(temp);
    return data;
}

Stack *init_stack() {
    Stack *new_stack = (Stack *)malloc(sizeof(Stack));

    if (!new_stack) {
        return NULL;
    }
    new_stack->top_node = NULL;
    return new_stack;
}

int push_stack(Stack *dir_stack, char *dir_name) {
    StackNode *dir_node = (StackNode *)malloc(sizeof(StackNode));

    if (!dir_stack || !dir_node) {
        return FALSE;
    }
    strncpy(dir_node->name, dir_name, MAX_NAME);
    dir_node->next_node = dir_stack->top_node;
    dir_stack->top_node = dir_node;
    return TRUE;
}

char *pop_stack(Stack *dir_stack) {
    StackNode *return_node = NULL;
    if (!dir_stack || (is_empty(dir_stack) == TRUE)) {
        return NULL;
    }
    return_node = dir_stack->top_node;
    dir_stack->top_node = return_node->next_node;
    return return_node->name;
}


#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct IntList {
    int *items;
    size_t count;
    size_t capacity;
} IntList;

void init_int_list(struct IntList *list);
void seed_int_list(struct IntList *list);
void append_int_list(struct IntList *list, int value);
void insert_int_list(struct IntList *list, int value, int index);
void remove_int_list_head(struct IntList *list);
void remove_int_list_by_index(struct IntList *list, int index);
int get_int_list(struct IntList *list, int index);
void set_int_list(struct IntList *list, int index, int value);
bool contains_int_list(struct IntList *list, int value);
int find_int_list(struct IntList *list, int value);
void clear_int_list(struct IntList *list);
void free_int_list(struct IntList *list);

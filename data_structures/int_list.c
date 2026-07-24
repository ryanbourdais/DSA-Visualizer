#include "int_list.h"

#include <stdlib.h>
#include <stdio.h>

void init_int_list(struct IntList *list)
{
    list->items = malloc(4 * sizeof(int));
    if (list->items == NULL) {
        fprintf(stderr, "Memory allocation failed");
        return;
    }
    list->count = 0;
    list->capacity = 4;
}

void seed_int_list(struct IntList *list)
{
    clear_int_list(list);

    for(int value = 1; value <= 5; value++)
    {
        append_int_list(list,value);
    }
}

void append_int_list(struct IntList *list, int value)
{
    if (list->count == list->capacity)
    {
        list->capacity += 2;
        int *new_items = realloc(list->items, list->capacity * sizeof(int));
        if (new_items == NULL)
        {
            fprintf(stderr, "Memory re-allocation failed, exiting insert");
            return;
        }
        list->items = new_items;
        if (list->capacity == 0)
        {
            list->capacity = 4;
            list->items = malloc(list->capacity * sizeof(int));
            if (list->items == NULL)
            {
                fprintf(stderr, "Memory allocation failed");
                return;
            }
        }
    }
    list->items[list->count] = value;
    list->count++;
}

void insert_int_list(struct IntList *list, int value, int index)
{
    if (index < 0 || index > list->count)
    {
        return;
    }
    if (list->count == list->capacity)
    {
        list->capacity += 2;
        int *new_items = realloc(list->items, list->capacity * sizeof(int));
        if (new_items == NULL)
        {
            fprintf(stderr, "Memory re-allocation failed, exiting insert");
            return;
        }
        list->items = new_items;
        if (list->items == NULL)
        {
            fprintf(stderr, "Memory re-allocation failed");
            return;
        }
        if (list->capacity == 0)
        {
            list->capacity = 4;
            list->items = malloc(list->capacity * sizeof(int));
            if (list->items == NULL)
            {
                fprintf(stderr, "Memory allocation failed");
                return;
            }
        }
    }
    for(size_t i = list->count; i > index; i--)
    {
        list->items[i] = list->items[i - 1];
    }
    list->items[index] = value;
    list->count++;
}

void remove_int_list_head(struct IntList *list)
{
    remove_int_list_by_index(list, 0);
}

void remove_int_list_by_index(struct IntList *list, int index)
{
    if(list->count == 0)
    {
        fprintf(stderr, "List already empty, exiting");
        return;
    }
    if (index < 0 || index >= list->count)
    {
        fprintf(stderr, "Index out of bounds, exiting");
        return;
    }
    for (size_t i = index; i < list->count - 1; i++)
    {
        list->items[i] = list->items[i + 1];
    }
    list->count--;
}

int get_int_list(struct IntList *list, int index)
{
    if (index < 0 || index >= list->count)
    {
        fprintf(stderr, "Index out of bounds, exiting");
        return -1;
    }
    return list->items[index];
}

void set_int_list(struct IntList *list, int index, int value)
{
    if (index < 0 || index >= list->count)
    {
        fprintf(stderr, "Index out of bounds, exiting");
        return;
    }
    list->items[index] = value;
}

bool contains_int_list(struct IntList *list, int value)
{
    for(size_t i = 0; i < list->count; i++)
    {
        if(list->items[i] == value)
        {
            return true;
        }
    }
    return false;
}

int find_int_list(struct IntList *list, int value)
{
    for(int i = 0; i < list->count; i++)
    {
        if(list->items[i] == value)
        {
            return i;
        }
    }
    return -1;
}

void clear_int_list(struct IntList *list)
{
    for (size_t i = 0; i < list->count; i++)
    {
        list->items[i] = 0;
    }
    list->count = 0;
}

void free_int_list(struct IntList *list)
{
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}
// #include "test.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void memory_realloc();

// TODO: Add automatic reallocation of array when full
int ITEMS_LENGTH = 100;
int tail = -1;
int head = -1;
int **items;
int length = 0;

typedef struct
{
    void (*allocate_memory)();
    void (*deallocate_memory)();
    void (*enqueue)(int x, int y);
    void (*dequeue)();
    void (*print)();
    int (*size)();
    int (*contains)(int x, int y);
} Queue;

void enqueue(int x, int y)
{
    if (tail == ITEMS_LENGTH - 1)
    {
        memory_realloc();
    }

    if (head == -1)
    {
        head = 0;
    }

    tail++;
    items[tail][0] = x;
    items[tail][1] = y;
    length++;
}

void dequeue()
{
    if (head == -1)
    {
        // printf("Coda vuota!");
    }
    else
    {
        head++;

        if (head > tail)
        {
            head = tail - 1;
        }

        length--;
    }
}

int contains(int x, int y)
{
    if(tail == -1) return 0;
    for (int i = head; i <= tail; i++)
        if (items[i][0] == x && items[i][1] == y)
            return 1;
    return 0;
}

void print()
{
    if (tail == -1)
    {
        printf("Coda vuota!\n");
    }
    else
    {
        for (int i = head; i <= tail; i++)
        {
            if (i != tail)
                printf("(%i,%i), ", items[i][0], items[i][1]);
            else
                printf("(%i,%i)\n", items[i][0], items[i][1]);
        }
    }
}

int size()
{
    int c = 0;
    for (int i = head; i <= tail; i++)
        c++;
    return c;
}

void allocate_memory()
{
    items = (int **)malloc(ITEMS_LENGTH * sizeof(int *));
    if (items == NULL)
    {
        printf("Errore nell'allocazione della memoria");
        exit(-1);
    }

    for (int i = 0; i < ITEMS_LENGTH; i++)
    {
        items[i] = (int *)malloc(sizeof(int) * 2);
        if (items[i] == NULL)
        {
            printf("Errore nell'allocazione della memoria");
            exit(-1);
        }
    }
}

void deallocate_memory()
{
    for (int i = 0; i < ITEMS_LENGTH; i++)
        free(items[i]);
    free(items);
}

void memory_realloc()
{
    items = (int **)realloc(items, sizeof(int *) * (ITEMS_LENGTH * ITEMS_LENGTH));
    ITEMS_LENGTH *= ITEMS_LENGTH;

    if(items == NULL){
        printf("Memory realloc error!");
        exit(-1);
    }

    for (int i = tail+1; i < ITEMS_LENGTH; i++)
    {
        items[i] = (int*) malloc(sizeof(int) * 2);
        if(items[i] == NULL){
            printf("Memory realloc error!");
            exit(-1);
        }
    }    
}

/*
int main(int argc, char const *argv[])
{

    Queue q;
    q.size = size;
    q.allocate_memory = allocate_memory;
    q.deallocate_memory = deallocate_memory;
    q.dequeue = dequeue;
    q.enqueue = enqueue;
    q.print = print;

    q.allocate_memory();

    q.enqueue(2,2);
    q.enqueue(2,3);
    q.enqueue(1,1);
    q.print();
    q.dequeue();
    q.enqueue(1,2);
    q.enqueue(3,1);
    q.print();

    q.deallocate_memory();
    return 0;

}
*/
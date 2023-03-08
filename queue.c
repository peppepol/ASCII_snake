//#include "test.c"
#include <stdio.h>
#include <stdlib.h>

//TODO: Add automatic reallocation of array when full
#define SIZE 100

int tail = -1;
int head = -1;
int items[SIZE];
int length = 0;


typedef struct{
    void (*initialize)();
    void (*enqueue)(int val);
    void (*dequeue)();
    void (*print)();
    int (*size)();
    int (*contains)(int val);
} Queue;


void enqueue(int val){
    if(tail == SIZE-1){
        //printf("Coda piena!");
    }else{
        if(head == -1){    
            head=0;
        }

        tail++;
        items[tail] = val;
        length++;
    }
}

void dequeue(){
    if(head == -1){
        //printf("Coda vuota!");
    }else{
        head++;

        if(head > tail){
            head = tail-1;
        }

        length--;
    }
}

int contains(int val){
    for (int i = head; i <= tail; i++)
        if(items[i] == val) return 1;
    return 0;
}

void print(){
    if(tail == -1){
        //printf("Coda vuota!");
    }else{
        for (int i = head; i <= tail; i++)
        {
            if(i != tail)
                printf("%i, ", items[i]);
            else
                printf("%i\n", items[i]);
        }   
    }
}

int size(){
    int c=0;
    for (int i = head; i <= tail; i++)
        c++;
    return c;
}

/*
int main(int argc, char const *argv[])
{
    struct Queue q;
    q.size = SIZE;
    q.dequeue = dequeue;
    q.enqueue = enqueue;
    q.print = print;
    
    q.enqueue(24);
    q.enqueue(12);
    q.enqueue(13);
    q.print();
    q.dequeue();
    q.enqueue(22);
    q.enqueue(77);
    q.print();
    return 0;
}
*/
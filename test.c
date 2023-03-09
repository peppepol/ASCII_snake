#include "queue.c"
#include <stdio.h>
#include <stdlib.h>

// define queue for x
Queue q;

Queue initialize_queue()
{
    ITEMS_LENGTH=5;
    Queue q;
    q.size = size;
    q.allocate_memory = allocate_memory;
    q.deallocate_memory = deallocate_memory;
    q.dequeue = dequeue;
    q.enqueue = enqueue;
    q.print = print;
    q.contains = contains;

    return q;
}


int main(int argc, char const *argv[])
{  
    q=initialize_queue(q);

    q.allocate_memory();
    q.print();
    q.enqueue(1,1);
    q.print();
    q.enqueue(2,2);
    q.print();
    q.enqueue(3,3);
    q.print();
    q.enqueue(4,4);
    q.print();
    q.enqueue(5,5);
    q.print();
    q.enqueue(6,6);
    q.print();
    q.enqueue(7,7);
    q.print();
    
    printf("----\n");
    printf("%i\n", q.contains(3,3));

    //q.dequeue();
    q.print();

    q.dequeue();
    q.print();

    printf("%i\n", q.contains(1,1));

    q.dequeue();
    q.print();

    q.deallocate_memory();
    
    return 0;
}


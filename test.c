#include "queue.c"

// define queue for x
Queue q;

Queue initialize_queue()
{
    Queue q;
    q.size = SIZE;
    q.dequeue = dequeue;
    q.enqueue = enqueue;
    q.print = print;

    return q;
}


int main(int argc, char const *argv[])
{    
    q=initialize_queue(q);

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
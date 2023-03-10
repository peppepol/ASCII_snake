//TODO: Add handling for game over
//TODO: Fix snake bug on first row and last column

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// PRIVATE
#include "queue.h"

// KEYBOARD EVENTS LIBRARY
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
#endif

// GLOBAL VARIABLES
#define WIDTH 42
#define HEIGHT 22
struct timespec ts;

#ifdef __unix__
    struct termios oldt, newt;
#endif

enum Direction
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

int xOffset = 1;
int yOffset = 1;

int snake_x = 1;
int snake_y = 1;
int snake_length = 1;

int *food;
int food_spawned = 0;
int food_taked = 0;
int quit = 0;
enum Direction dir = RIGHT;

// Queue for X,Y
Queue q;

Queue initialize_queue()
{
    ITEMS_LENGTH = 5;
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

// PRINT GAME BOX
void print_schema()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        int blocks = 0;

        for (int j = 0; j < WIDTH; j++)
        {
            // FIRST ROW
            if (i==0)
            {   
                if(j==WIDTH - xOffset){
                    putchar('+');
                    putchar('\n');
                }else if(j==0){
                    putchar('+');
                }else{
                    putchar('-');
                }

            // LAST ROW
            }else if(i == HEIGHT - yOffset){
                if(j==WIDTH - xOffset){
                    putchar('+');
                    putchar('\n');
                }else if(j==0){
                    putchar('+');
                }else{
                    putchar('-');
                }

            // MIDDLE ROW
            }else{
                if(j == 0){
                    putchar('|');
                }else if(j==WIDTH-xOffset){
                    putchar('|');
                    putchar('\n');
                }else{
                    //ALL INTERNAL BOX
                    if(q.contains(j, i) == 1){
                        printf("\e[32m");
                        putchar('#');
                        printf("\e[0m");

                        if(j == food[0] && i == food[1]){
                            food_taked = 1;
                            food_spawned = 0;
                            snake_length++;
                        }
                    }else if(j == food[0] && i == food[1]){
                        printf("\e[38;5;220m");
                        putchar('o');     
                        printf("\e[0m");

                        food_spawned = 1;                   
                    }else{
                        putchar(' ');
                    }
                }
            }
        }
    }
}

// MOVE CURSOR TO X,Y
void move_cursor(int x, int y)
{
    if (x > 0)
        printf("\e[%iC", x);
    if (y > 0)
        printf("\e[%iB", y);
}

// MOVE CURSOR TO START
void up_cursor()
{
    printf("\e[%iA", HEIGHT+1);
    printf("\e[%iG", 0);
}

// MOVE CURSOR TO END
void end_cursor()
{
    printf("\e[%iB", HEIGHT);
    printf("\e[%iG", 0);
}

// READ INPUT FROM KEYBOARD
void read_keyboard()
{    
    #ifdef __unix
        struct timeval tv;
        fd_set fds;
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
        if (FD_ISSET(STDIN_FILENO, &fds)) {
            char ch = getchar();
    #else

        if (kbhit())
        {
            char ch = getch();

    #endif
        
        if (ch == 27 || ch == 'q')
        {
            quit = 1;
            end_cursor();
            q.deallocate_memory();

            printf("\n\nGAME EXIT");
        }
        else if (ch == 'w')
        {
            dir = UP;
        }
        else if (ch == 'a')
        {
            dir = LEFT;
        }
        else if (ch == 's')
        {
            dir = DOWN;
        }
        else if (ch == 'd')
        {
            dir = RIGHT;
        }
    }
    
}

// CHECK DIRECTION PRESSED
void check_direction()
{
    if (dir == RIGHT)
    {
        snake_x += 1;
    }
    else if (dir == LEFT)
    {
        snake_x -= 1;
    }
    else if (dir == UP)
    {
        snake_y -= 1;
    }
    else if (dir == DOWN)
    {
        snake_y += 1;
    }

    q.enqueue(snake_x, snake_y);
}

// CHECK IF SNAKE IS OVER BOUNDS
void check_bounds()
{
    if (snake_x > WIDTH - xOffset)
    {
        snake_x = 0;
    }
    else if (snake_x <= 0)
    {
        snake_x = WIDTH - xOffset;
    }
    else if (snake_y > HEIGHT - yOffset - 1)
    {
        snake_y = 0;
    }
    else if (snake_y <= 0)
    {
        snake_y = HEIGHT - yOffset;
    }
    
}

// GET RANDOM INT
int random_number(int min_num, int max_num)
{
    int result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1; // include max_num in output
    }
    else
    {
        low_num = max_num + 1; // include max_num in output
        hi_num = min_num;
    }

    srand(time(NULL));
    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}

// GET RANDOM X,Y IN BOX
int *get_random_position()
{
    int *array = malloc(sizeof(int) * 2);
    array[0] = random_number(xOffset, WIDTH - xOffset - 1);
    array[1] = random_number(0, HEIGHT - yOffset -1);

    return array;
}

int main(int argc, char const *argv[])
{
    // [LINUX] Switch to canonical mode, disable echo
    #ifdef __unix__
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    #endif
    
    // Initialize queue
    q = initialize_queue();
    q.allocate_memory();

    //  Hide cursor
    printf("\e[?25l");

    // Print schema
    food = get_random_position();
    print_schema();
    printf("\nPUNTI: %i", q.size());

    while (!quit)
    {
        if (food_spawned == 0)
        {
            food = get_random_position();
            food_taked = 0;
        }

        up_cursor();
        print_schema();
        printf("\nPUNTI: %i", snake_length);

        check_direction();

        //CHECK QUEUE
        for (int i = 0; i <= snake_length; i++)
        {
            if (q.size() > snake_length)
                q.dequeue();
        }

        
        check_bounds();

        read_keyboard();
	
        // SLEEP TIME
        int milliseconds=300;
        ts.tv_sec = milliseconds / 1000;
        ts.tv_nsec = (milliseconds % 1000) * 1000000;
        nanosleep(&ts, NULL);
    }
    
    q.deallocate_memory();
    
    // RESET LINUX TERMINAL STDIN
    #ifdef __unix__
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    #endif

    return 0;
}

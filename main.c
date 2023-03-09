#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

// PRIVATE
#include "queue.c"

#define WIDTH 42
#define HEIGHT 22

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

// define queue for x
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

/*
void print_schema()
{
    //TODO: Refactoring and fix snake print
    for (int i = 0; i < HEIGHT; i++)
    {
        int blocks = 0;

        for (int j = 0; j < WIDTH; j++)
        {
            int blocks = 0;

            if (i == snake_y && j == snake_x && j == food[0] && i == food[1])
            {
                food_taked = 1;
                food_spawned = 0;
                snake_length += 1;
            }

            if ((j == 0 && i == 0) || (j == 0 && i == HEIGHT - 1))
            {
                putchar('+');
            }
            else if ((j == WIDTH - blocks - 1 && i == 0) || (i == HEIGHT - 1 && j == WIDTH - blocks - 1))
            {
                putchar('+');
                putchar('\n');
            }
            else if (j != 0 && j != WIDTH - blocks - 1 && (i == 0 || i == HEIGHT - 1))
            {
                putchar('-');
            }
            else if (j != 0 && j != WIDTH - blocks - 1)
            {
                if (i == snake_y && q.contains(i, j) == 1)
                {
                    printf("si");
                    putchar('#');
                    blocks += 1;
                    continue;
                }

                if (j == food[0] && i == food[1]){
                    putchar('o');
                    blocks += 1;
                    food_spawned = 1;
                    continue;

                }

                putchar(' ');
            }
            else if (j == WIDTH - blocks - 1)
            {
                putchar('|');
                putchar('\n');
            }
            else if (j == 0)
            {
                putchar('|');
            } // else if( i==snakk)
        }
    }
}
*/

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

void move_cursor(int x, int y)
{
    if (x > 0)
        printf("\e[%iC", x);
    if (y > 0)
        printf("\e[%iB", y);
}

void up_cursor()
{
    printf("\e[%iA", HEIGHT+1);
    printf("\e[%iG", 0);
}

void end_cursor()
{
    printf("\e[%iB", HEIGHT);
    printf("\e[%iG", 0);
}

void read_keyboard()
{
    // Read keyboard
    if (kbhit())
    {
        char ch = getch();
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

int *get_random_position()
{
    int *array = malloc(sizeof(int) * 2);
    array[0] = random_number(xOffset, WIDTH - xOffset - 1);
    array[1] = random_number(0, HEIGHT - yOffset -1);

    return array;
}

int main(int argc, char const *argv[])
{
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
        // printf("%i", food[1]);

        up_cursor();
        print_schema();
        printf("\nPUNTI: %i", q.size());

        //CHECK QUEUE
        for (int i = 0; i < snake_length; i++)
        {
            if (q.size() > snake_length)
                q.dequeue();
        }

        check_direction();
        check_bounds();

        read_keyboard();

        Sleep(500);
    }
    
    q.deallocate_memory();
    return 0;
}

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
    Queue q;
    q.dequeue = dequeue;
    q.enqueue = enqueue;
    q.print = print;
    q.size = size;
    q.contains = contains;

    return q;
}

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
                
                if (i == snake_y && q.contains(j) == 1)
                {
                    putchar('#');
                    blocks += 1;                
                }else if (j == food[0] && i == food[1]){
                    putchar('o');
                    blocks += 1;
                    food_spawned = 1;                
                }else{
                    putchar(' ');
                }
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

void move_cursor(int x, int y)
{
    if (x > 0)
        printf("\e[%iC", x);
    if (y > 0)
        printf("\e[%iB", y);
}

void up_cursor()
{
    printf("\e[%iA", HEIGHT + 2);
    printf("\e[%iG", 0);
}

void end_cursor()
{
    printf("\e[%iB", HEIGHT - 1);
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
            printf("GAME EXIT");
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
        q.enqueue(snake_x);
    }
    else if (dir == LEFT)
    {
        snake_x -= 1;
        q.enqueue(snake_x);
    }
    else if (dir == UP)
    {
        snake_y -= 1;
    }
    else if (dir == DOWN)
    {
        snake_y += 1;
    }
}

void check_bounds()
{
    if (snake_x == WIDTH - xOffset)
    {
        snake_x = xOffset;
    }
    else if (snake_x == 0)
    {
        snake_x = WIDTH - xOffset - 1;
    }
    else if (snake_y == 0)
    {
        snake_y = HEIGHT - yOffset - 1;
    }
    else if (snake_y == HEIGHT - yOffset)
    {
        snake_y = yOffset;
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
    array[1] = random_number(0, 1 /*HEIGHT - yOffset -1*/);

    return array;
}

int main(int argc, char const *argv[])
{
    // Initialize queues
    q = initialize_queue();

    // Hide cursor
    printf("\e[?25l");

    // Print schema
    food = get_random_position();
    food[1] = 1;
    print_schema();

    while (!quit)
    {
        if (food_spawned == 0)
        {
            food = get_random_position();
            food[1] = 1;
            food_taked = 0;
        }
        // printf("%i", food[1]);

        up_cursor();
        print_schema();

        for (int i = 0; i < snake_length; i++)
        {
            if (q.size() > snake_length)
                q.dequeue();
            // if(dir == RIGHT || dir == LEFT)
            // q.dequeue();
        }
        q.print();
        printf("SIZE: %i", q.size());

        up_cursor();

        check_direction();
        check_bounds();

        read_keyboard();

        // quit++;

        Sleep(500);
    }

    return 0;
}

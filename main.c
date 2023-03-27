#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//Private libraries
#include "queue.h"

//Keyboard events libraries
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
#endif

//Global variables
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
int game_over=0;

// Queue for X,Y snake position
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

//Print game schema
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
                if(j==WIDTH-xOffset){
                    printf("+\e[0m\n");
                    //putchar('\n');
                }else if(j==0){
                    printf("\e[0;31m+");
                }else{
                    printf("-");
                }

            // LAST ROW
            }else if(i == HEIGHT - yOffset){
                if(j==WIDTH - xOffset){
                    printf("+\e[0m\n");
                }else if(j==0){
                    printf("\e[0;31m+");
                }else{
                    putchar('-');
                }

            // MIDDLE ROW
            }else{
                if(j == 0){
                    printf("\e[0;31m|\e[0m");
                }else if(j==WIDTH-xOffset){
                    printf("\e[0;31m|\e[0m\n");
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

//Move cursor to x,y
void move_cursor(int x, int y)
{
    if (x > 0)
        printf("\e[%iC", x);
    if (y > 0)
        printf("\e[%iB", y);
}

//Move cursor to start
void up_cursor()
{
    printf("\e[%iA", HEIGHT+1);
    printf("\e[%iG", 0);
}

//Move cursor to end
void end_cursor()
{
    printf("\e[%iB", HEIGHT-snake_y+2);
    printf("\e[%iG", 0);
}

//Read input from keyboard
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
        
        int charCode = ch - '0';
        if (ch == 27 || ch == 'q')
        {
           game_over=1;
        }
        else if ((ch == 'w' || charCode == 24) && (snake_x>0 && snake_x<WIDTH-yOffset))
        {
            if(dir != DOWN && snake_length > 1)
                dir = UP;
            else if(snake_length == 1)
                dir = UP;
        }
        else if ((ch == 'a' || charCode == 27) && (snake_y>0 && snake_y<HEIGHT-xOffset))
        {
            if(dir != RIGHT && snake_length > 1)
                dir = LEFT;
            else if(snake_length == 1)
                dir = LEFT;
        }
        else if ((ch == 's' || charCode == 32) && (snake_x>0 && snake_x<WIDTH-yOffset))
        {
            if(dir != UP && snake_length > 1)
                dir = DOWN;
            else if(snake_length == 1)
                dir = DOWN;
        }
        else if ((ch == 'd' || charCode == 29)  && (snake_y>0 && snake_y<HEIGHT-xOffset))
        {
            if(dir != LEFT && snake_length > 1)
                dir = RIGHT;
            else if(snake_length == 1)
                dir = RIGHT;
        }
    }
    
}

//Check direction pressed
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

    if(q.contains(snake_x, snake_y)){
        game_over=1;
    }else{
        q.enqueue(snake_x, snake_y);
    }
}

//Check if snake is over bounds
void check_bounds()
{
    if (snake_x >= WIDTH - xOffset)
    {
        snake_x = 0;
    }
    else if (snake_x <= 0)
    {
        snake_x = WIDTH - xOffset;
    }

    else if (snake_y >= HEIGHT-yOffset )
    {
        snake_y = 0;
    }
    
    else if (snake_y <= 0)
    {
        snake_y = HEIGHT - yOffset;
    }
    
}

//Get random integer
int random_number(int min_num, int max_num)
{
    int result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1; 
    }
    else
    {
        low_num = max_num + 1; 
        hi_num = min_num;
    }

    srand(time(NULL));
    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}

//Get random x,y in box
int *get_random_position()
{
    int *array = malloc(sizeof(int) * 2);
    array[0] = random_number(xOffset, WIDTH - xOffset - 1);
    array[1] = random_number(0, HEIGHT - yOffset -1);

    return array;
}

//Print game title
void print_title(){
    printf("\n\e[1;35;40m");
    printf("   _   __  __         __        _       __  \n");
    printf("  | | |   |   | |    |   ||  | | | | | |    \n");
    printf("  |_| |_  |   | |    |_  | | | |_| | | |_   \n");
    printf("  | |   | |   | |      | |  || | | ||  |    \n");
    printf("  | | __| |__ | |    __| |   | | | | | |__  \n");
    printf("\e[0m\n");
}

//Print game over
void print_game_over(){
    for (int i = 0; i < HEIGHT+2; i++)
    {   
        if (i == (HEIGHT+2)/2){
            for (int j = 0; j < WIDTH+20; j++)
            {
                if(j == (WIDTH/2) - 5){
                    printf("\e[1;35mGAME OVER!");
                }else if(j == WIDTH + 20 -1){
                    putchar(' ');
                    putchar('\n');
                }else{
                    putchar(' ');
                }
            }
        }else if(i == (HEIGHT+2)/2 +1){
            for (int j = 0; j < WIDTH+20; j++)
            {
                if(j == (WIDTH/2) - 5){
                    printf("\e[0;35mPOINTS: %i\e[0m", length);
                }else if(j == WIDTH + 20 -1){
                    putchar(' ');
                    putchar('\n');
                }else{
                    putchar(' ');
                }
            }
        }else{
            for (int j = 0; j < WIDTH; j++)
            {
                putchar(' '); 
                    
                if(j==WIDTH-1) {
                    putchar('\n');
                }
            }
        }
    }
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

    // Hide cursor
    printf("\e[?25l");

    // Print schema
    food = get_random_position();
    print_title();
    print_schema();
    printf("\n\e[0;35mPOINTS: %i\e[0m", snake_length);

    while (!quit)
    {
        if(game_over){
            end_cursor();
            up_cursor();
            print_game_over();
            quit=1;
            break;
        }

        if (food_spawned == 0)
        {
            food = get_random_position();
            food_taked = 0;
        }        
        
        up_cursor();
        print_schema();
        printf("\n\e[0;35mPOINTS: %i   \e[3;35m[WASD to move and Q to exit]\e[0m", snake_length);
        
        check_direction();
        check_bounds();        

        //CHECK QUEUE
        for (int i = 0; i <= snake_length; i++)
        {
            if (q.size() > snake_length)
                q.dequeue();
        }      

        read_keyboard();       
	
        //SLEEP TIME
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

#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <time.h>

using namespace std;

#define length 600
#define width 700
#define speed 10


void draw_circle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}


int rand(int n)
{
    int result = (rand() % n);
    return result;
}

void draw_balls(int start_point , int coordinates[20][7][2] , SDL_Renderer * renderer)
{
    SDL_Color col_red = {255 , 0 , 0};
    SDL_Color col_blue = {0 , 0 , 255};
    SDL_Color col_green = {0 , 255 , 0};
    SDL_Color col_yellow = {255 , 255 , 0};
    SDL_Color col_black = {0 , 0 , 0};
    SDL_Color col_orange = {255 , 165 , 0};

    int i = 0;
    int Rx = 15;
    while (i < 20)
    {
        int j = 0;
        int Ry = start_point;
        while (j < 7)
        {
            if (coordinates[i][j][0] == 0)
            {
                draw_circle(renderer , Rx , Ry  , 15 , col_red);
            }
            if (coordinates[i][j][0] == 1)
            {
                draw_circle(renderer , Rx , Ry  , 15 , col_blue);
            }
            if (coordinates[i][j][0] == 2)
            {
                draw_circle(renderer , Rx , Ry  , 15 , col_green);
            }
            if (coordinates[i][j][0] == 3)
            {
                draw_circle(renderer , Rx , Ry  , 15 , col_yellow);
            }
            if (coordinates[i][j][0] == 4)
            {
                draw_circle(renderer , Rx , Ry  , 15 , col_black);
            }
            if (coordinates[i][j][0] == 5)
            {
                draw_circle(renderer , Rx , Ry  , 15 , col_orange);
            }
            j += 1;
            Ry += 30;
        }
        i += 1;
        Rx +=30;
    }
    SDL_SetRenderDrawColor(renderer, 0 , 0 , 0 , 255);
    SDL_RenderDrawLine(renderer, 0 , 600 , 600 , 600);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Delay(speed);
    SDL_RenderClear(renderer);


}

int main(int argc, char ** argv)
{
    srand(time(0));
    int coordinates[20][7][2];
    int i = 0;
    while (i < 20)
    {
        int j = 0;
        while (j < 7)
        {
            coordinates[i][j][0] = rand(6);
            int e = rand(9);
            if (e = 1)
            {
                int r = rand(2);
                if (r = 1)
                {
                    coordinates[i][j][1] = -1;
                }
                else
                {
                    coordinates[i][j][1] = rand(6);
                }
            }
            j += 1;
        }
        i += 1;
    }


/**<
for(int i = 0;i <20;i++)
    {
        for(int j = 0 ; j<7 ; j++)
        {
            cout<<coordinates[i][j][0]<< " ";
        }
        cout << '\n';
    } */

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window = SDL_CreateWindow("TALES",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);


    i = 15;
    while (i <= 400)
    {
        Sleep(10);
        draw_balls(i , coordinates , renderer);
        i +=1;
    }

    SDL_Delay(200000);

    return 0;
}

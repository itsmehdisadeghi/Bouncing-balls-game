#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <time.h>
#include <vector>
#include <windows.h>
#include <cmath>

using namespace std;

#define length 600
#define width 700
#define speed 100


int cannon_ball[3] = {0};//color -ballx - bally -
int ballx = 300;
int bally = 600;
int ballc = 0;
int color_temp1[3]; // the ball that is ready to lounch
int color_temp2[3]; // the ball wait for replace
int  xMouse = -1;
int  yMouse;
float m;
int dx = 10;
int dy = 10;

bool press = false;
bool ball = false;
bool first = true;

SDL_Color col_red = {255 , 0 , 0};
SDL_Color col_blue = {0 , 0 , 255};
SDL_Color col_green = {0 , 255 , 0};
SDL_Color col_yellow = {255 , 255 , 0};
SDL_Color col_black = {0 , 0 , 0};
SDL_Color col_orange = {255 , 165 , 0};

// coordinates[20][20][5] = {show or not(0 = dont show , 1 = show)
//                         , type(-2 = normal ball , -1 = locked ball , a color between 0 to 5 = double color(siah nadarim))
//                         , color(0 = red , 1 = blue , 2 = green , 3 = yellow , 4 = black , 5 = orange)
//                         , x , y}


void start_counter() {
	Sleep(1000);
	cout << "game will start in \n";
	cout << "\t3 \n";
	Sleep(1000);
	cout << "\t2 \n";
	Sleep(1000);
	cout << "\t1 \n";
	Sleep(1000);
}

// void destroy_ball(int coordinates[20][20][5])
// {
//     for (int i = 0;i<20;i++)
//     {
//         for(int j = 0;j<20;j++)
//         {
//             if (coordintes[i][j][0] == 1)&&(coordinates[i][j+1][0] == 1)&&(coordinates[i][j+2] == 1)
//             {
//                 if(coordinates[i][j][1] == coordinates[i][j+1][1])
//                 {
//                     if(coordinates[i][j+1][1] == coordinates[i][j+1][1])
//                     {
//                         coordinates[i][j][0] = 0 ;
//                         coordinates[i][j+1][0] = 0 ;
//                         coordinates[i][j+2][0] = 0 ;
//                     }
//                 }
//             }
//         }
//     }
// }

int rand(int n)
{
    int result = (rand() % n);
    return result;
}

void draw_circle(SDL_Renderer *renderer, int type , int x, int y, int radius, int coler , int coler1)
{
    SDL_Color color;
    SDL_Color color1;

    if (coler == 0) color = col_red;
    if (coler == 1) color = col_blue;
    if (coler == 2) color = col_green;
    if (coler == 3) color = col_yellow;
    if (coler == 4) color = col_black;
    if (coler == 5) color = col_orange;

    if (coler1 == 0) color1 = col_red;
    if (coler1 == 1) color1 = col_blue;
    if (coler1 == 2) color1 = col_green;
    if (coler1 == 3) color1 = col_yellow;
    if (coler1 == 4) color1 = col_black;
    if (coler1 == 5) color1 = col_orange;

    if(type == 1) // normal ball
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
    if (type == 2) // locked ball
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
        SDL_SetRenderDrawColor(renderer, 0 , 0, 0 , 255);
        SDL_RenderDrawLine(renderer, x , (y - radius) , x , (y + radius));
        SDL_RenderDrawLine(renderer, (x - radius), y , (x + radius) , y );
    }
    if (type == 3) // double color ball
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        for (int w = 0; w < radius * 2; w++)
        {
            for (int h = 0; h < radius * 2; h++)
            {
                int dx = radius - w; // horizontal offset
                int dy = radius - h; // vertical offset
                if (((dx*dx + dy*dy) <= (radius * radius)) && ((dx + x) > x))
                {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, color1.r, color1.g, color1.b, color1.a);
        for (int w = 0; w < radius * 2; w++)
        {
            for (int h = 0; h < radius * 2; h++)
            {
                int dx = radius - w; // horizontal offset
                int dy = radius - h; // vertical offset
                if (((dx*dx + dy*dy) <= (radius * radius)) && ((dx + x) <= x))
                {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
    }
    if (type == 4) // multiple color
    {
        SDL_SetRenderDrawColor(renderer, col_red.r, col_red.g, col_red.b, col_red.a);
        for (int w = 0; w < 30; w++)
        {
            for (int h = 0; h < 30; h++)
            {
                int dx = 15 - w; // horizontal offset
                int dy = 15 - h; // vertical offset
                if ((dx*dx + dy*dy) <= (225))
                {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, col_blue.r, col_blue.g, col_blue.b, col_blue.a);
        for (int w = 0; w < 24; w++)
        {
            for (int h = 0; h < 24; h++)
            {
                int dx = 12 - w; // horizontal offset
                int dy = 12 - h; // vertical offset
                if ((dx*dx + dy*dy) <= (144))
                {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, col_yellow.r, col_yellow.g, col_yellow.b, col_yellow.a);
        for (int w = 0; w < 18; w++)
        {
            for (int h = 0; h < 18; h++)
            {
                int dx = 9 - w; // horizontal offset
                int dy = 9 - h; // vertical offset
                if ((dx*dx + dy*dy) <= (81))
                {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, col_green.r, col_green.g, col_green.b, col_green.a);
        for (int w = 0; w < 12; w++)
        {
            for (int h = 0; h < 12; h++)
            {
                int dx = 6 - w; // horizontal offset
                int dy = 6 - h; // vertical offset
                if ((dx*dx + dy*dy) <= (25))
                {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, col_orange.r, col_orange.g, col_orange.b, col_orange.a);
        for (int w = 0; w < 6; w++)
        {
            for (int h = 0; h < 6; h++)
            {
                int dx = 3 - w; // horizontal offset
                int dy = 3 - h; // vertical offset
                if ((dx*dx + dy*dy) <= (9))
                {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
    }

}

void cord(int coordinates[20][20][5] , int coordin[3]) // this function just look at the coordiantes and than give the cannon ball a colored ball too shoot
{
    srand(time(0));
    bool colored = false;
    bool coloblue = false;
    bool cologreen = false;
    bool coloyellow = false;
    bool coloorange = false;
    for(int i = 0; i< 20;i++) // for finding what color do we have in our map.
    {
        for(int j = 0; j <20; j++)
        {
            if (coordinates[i][j][0] == 1)
            {
                if (coordinates[i][j][2] == 0) colored = true;
                if (coordinates[i][j][2] == 1) coloblue = true;
                if (coordinates[i][j][2] == 2) cologreen = true;
                if (coordinates[i][j][2] == 3) coloyellow = true;
                if (coordinates[i][j][2] == 5) coloorange = true;
            }
        }
    }
    // in this scope , we give tha cannon a ball that depend on that must be in the map .
    thisline:
    int colo = rand(6);
    if ((colo == 0)&&(colored)) coordin[0]= 0;
    else if ((colo == 1)&&(coloblue)) coordin[0]= 1;
    else if ((colo == 2)&&(cologreen)) coordin[0]= 2;
    else if ((colo == 3)&&(coloyellow)) coordin[0]= 3;
    else if ((colo == 4)&&(coloorange)) coordin[0]= 5;
    else if ((colo == 5)&&(coloorange)) coordin[0]= 5;
    else goto thisline;
}


void launch_balls(SDL_Renderer * renderer , int coordinates[20][20][5])
{
    i:
    if (first)
    {
        h:
        int s = rand(6);
        int h = rand(6);
        if ((s==4)||(h==4)) goto h;
        color_temp1[0] = s;
        ballc = color_temp1[0];
        color_temp2[0] = h;
        first = false;
    }

    draw_circle(renderer , 1 ,  300 , 600 , 15 , color_temp1[0] , color_temp1[0]); // the ball that is ready to lounch
    draw_circle(renderer , 1 ,  200 , 600 , 15 , color_temp2[0] , color_temp2[0]); // the ball wait for replace

    int ex , ey;
    SDL_SetRenderDrawColor(renderer, 0 , 0 , 0 , 255);
    SDL_GetGlobalMouseState(&ex,&ey);
    if ((ey <= 700) && (ex <= 600))
    {
        SDL_RenderDrawLine(renderer, 300 , 600 , ex , ey);
    }
    if (!press)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if(SDLK_SPACE == event.key.keysym.sym)
            {
                SDL_GetGlobalMouseState(&xMouse,&yMouse);
                press = true;
                cord(coordinates , cannon_ball);
                color_temp1[0] = color_temp2[0];
                color_temp2[0] = cannon_ball[0];
            }
        }
    }
    if (press)
    {
        m = (float(600 - yMouse)/float(xMouse - 300));
        if (xMouse != -1)
        {
            if ((xMouse - 300)<0)
            {
                dx = -10;
                m *= -1;
            }
        }
        if ((bally > 686) || (bally < 14)) m *= -1;
        if ((ballx > 586) || (ballx < 14)) dx *=-1;
        ballx += dx;
        bally -= (10 * m);
        draw_circle(renderer , 1 ,  ballx , bally , 15 , ballc , ballc);
        for(int i=0;i<20;i++)
        {
            for(int j=0;j<20;j++)
            {
                if (coordinates[i][j][0])
                {
                    if(abs(coordinates[i][j][3] - ballx) <= 15)
                    {
                        if(abs(bally - coordinates[i][j][4]) <= 15)
                        {
                            coordinates[i][j+1][0] = 1;
                            coordinates[i][j+1][1] = -2;
                            coordinates[i][j+1][2] = ballc;
                            if (coordinates[i][j][0]&&coordinates[i][j+1][0]&&coordinates[i][j-1])
                            {
                                if((coordinates[i][j][1] == coordinates[i][j+1][1]) && (coordinates[i][j+1][1] == coordinates[i][j-1][1]))
                                {
                                    coordinates[i][j][0] = 0 ;
                                    coordinates[i][j+1][0] = 0 ;
                                    coordinates[i][j+2][0] = 0 ;
                                }
                            }
                            ballx = 300;
                            bally = 600;
                            press = false;
                        }
                    }
                }
            }
        }
    }
}

void coord(int type , int till , int coordinates[20][20][5])
{
    srand(time(0));
    for (int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
        {
            coordinates[i][j][0] = 0;
        }
    }
    if (type == 0) // random mode
    {
        int i = 0;
        while (i < 20)
        {
            int j = 0;
            while (j < 7)
            {
                coordinates[i][j][0] = 1;//show
                here1:
                int colo = rand(6);
                if (colo == 4)
                {
                    int s = rand(2);
                    if (s == 0)
                    {
                        goto here1;
                    }
                    else
                    {
                        coordinates[i][j][2] = 4;
                    }
                }
                else
                {
                    coordinates[i][j][2] = colo;
                }
                coordinates[i][j][3] = i*15;
                coordinates[i][j][4] = j*15;
                int e = rand(9);
                if (e == 1)
                {
                    int r = rand(2);
                    if (r == 1)
                    {
                        if (colo != 4)
                        {
                            coordinates[i][j][1] = -1;
                        }
                        else
                        {
                            coordinates[i][j][1] = -2;
                        }
                    }
                    else
                    {
                        coordinates[i][j][1] = rand(5);
                    }
                }
                else
                {
                    coordinates[i][j][1] = -2;
                }
                int s = rand(3);
                if (s == 0)
                {
                    int x = rand(17);
                    int y = rand(17);
                    coordinates[x+1][y][2] =  coordinates[x][y][2];
                    coordinates[x-1][y][2] =  coordinates[x][y][2];
                }
                j += 1;
            }
            i += 1;
        }
    }
    if (type == 1) // triangle
    {
        int k = 10;
        int m = 10;
        int j = 0;
        while (j < till)
        {
            for (int f = k ; f <= m ; f++)
            {
                coordinates[f][j][0] = 1;
                here2:
                int colo = rand(6);
                if (colo == 4)
                {
                    int s = rand(2);
                    if (s == 0)
                    {
                        goto here2;
                    }
                    else
                    {
                        coordinates[f][j][2] = 4;
                    }
                }
                else
                {
                    coordinates[f][j][2] = colo;
                }
                coordinates[f][j][3] = f*15;
                coordinates[f][j][4] = j*15;
                int e = rand(9);
                if (e == 1)
                {
                    int r = rand(2);
                    if (r == 1)
                    {
                        if (colo != 4)
                        {
                            coordinates[f][j][1] = -1;
                        }
                        else
                        {
                            coordinates[f][j][1] = -2;
                        }
                    }
                    else
                    {
                        coordinates[f][j][1] = rand(5);
                    }
                }
                else
                {
                    coordinates[f][j][1] = -2;
                }
                int s = rand(3);
                if (s == 0)
                {
                    int x = rand(17);
                    int y = rand(17);
                    coordinates[x+1][y][2] =  coordinates[x][y][2];
                    coordinates[x-1][y][2] =  coordinates[x][y][2];
                }
            }
            m +=1;
            k -=1;
            if((m-k) == 12)
            {
                break;
            }
            j += 1;
        }
    }
    if (type == 2) // loozi
    {
        int k = 10;
        int m = 10;
        int a = 1;
        int j = 0;
        while (j < 20)
        {
            thisline:
            for (int f = k ; f <= m ; f++)
            {
                coordinates[f][j][0] = 1;
                 if (j>10)
                {
                    coordinates[f][j][0] = 0;
                }
                here3:
                int colo = rand(6);
                if (colo == 4)
                {
                    int s = rand(2);
                    if (s == 0)
                    {
                        goto here3;
                    }
                    else
                    {
                        coordinates[f][j][2] = 4;
                    }
                }
                else
                {
                    coordinates[f][j][2] = colo;
                }
                coordinates[f][j][3] = f*15;
                coordinates[f][j][4] = j*15;
                int e = rand(9);
                if (e == 1)
                {
                    int r = rand(2);
                    if (r == 1)
                    {
                        if (colo != 4)
                        {
                            coordinates[f][j][1] = -1;
                        }
                        else
                        {
                            coordinates[f][j][1] = -2;
                        }
                    }
                    else
                    {
                        coordinates[f][j][1] = rand(5);
                    }
                }
                else
                {
                    coordinates[f][j][1] = -2;
                }
                int s = rand(3);
                if (s == 0)
                {
                    int x = rand(17);
                    int y = rand(17);
                    coordinates[x+1][y][2] =  coordinates[x][y][2];
                    coordinates[x-1][y][2] =  coordinates[x][y][2];
                }
            }
            m += a;
            k -=a;
            if ((m-k) == 12)
            {
                a = -1;
                goto thisline;
            }
            if ((m-k) == 0)
            {
                break;
            }
            j += 1;
        }
    }
    if (type == 3) // diamond
    {
        int k = 10;
        int m = 10;
        int a = 1;
        thislinee:
        int j = 0;
        while (j < 20)
        {
            for (int f = k ; f <= m ; f++)
            {
                for(int i = 0 ; i<k;i++)
                {
                    coordinates[i][j][0] = 0;
                }
                 for(int i = m+1 ; i<20;i++)
                {
                    coordinates[i][j][0] = 0;
                }
                coordinates[f][j][0] = 1;
                if (j>7)
                {
                    coordinates[f][j][0] = 0;
                }
                here4:
                int colo = rand(6);
                if (colo == 4)
                {
                    int s = rand(2);
                    if (s == 0)
                    {
                        goto here4;
                    }
                    else
                    {
                        coordinates[f][j][2] = 4;
                    }
                }
                else
                {
                    coordinates[f][j][2] = colo;
                }
                coordinates[f][j][3] = f*15;
                coordinates[f][j][4] = j*15;
                int e = rand(9);
                if (e == 1)
                {
                    int r = rand(2);
                    if (r == 1)
                    {
                        if (colo != 4)
                        {
                            coordinates[f][j][1] = -1;
                        }
                        else
                        {
                            coordinates[f][j][1] = -2;
                        }
                    }
                    else
                    {
                        coordinates[f][j][1] = rand(5);
                    }
                }
                else
                {
                    coordinates[f][j][1] = -2;
                }
                int s = rand(3);
                if (s == 0)
                {
                    int x = rand(17);
                    int y = rand(17);
                    coordinates[x+1][y][2] =  coordinates[x][y][2];
                    coordinates[x-1][y][2] =  coordinates[x][y][2];
                }
            }
            m += a;
            k -=a;
            if ((m-k) == 12)
            {
                a = -1;
                goto thislinee;
            }
            if ((m-k) == 0)
            {
                break;
            }
            j += 1;
        }
    }
    if (type == 4) // diamond
    {
        int k = 10;
        int m = 10;
        int a = 1;
        thislineee:
        int j = 0;
        while (j < 20)
        {
            for (int f = k ; f <= m ; f++)
            {
                coordinates[f][j][0] = 1;
                if (j>7)
                {
                    coordinates[f][j][0] = 0;
                }
                here5:
                int colo = rand(6);
                if (colo == 4)
                {
                    int s = rand(2);
                    if (s == 0)
                    {
                        goto here5;
                    }
                    else
                    {
                        coordinates[f][j][2] = 4;
                    }
                }
                else
                {
                    coordinates[f][j][2] = colo;
                }
                coordinates[f][j][3] = f*15;
                coordinates[f][j][4] = j*15;
                int e = rand(9);
                if (e == 1)
                {
                    int r = rand(2);
                    if (r == 1)
                    {
                        if (colo != 4)
                        {
                            coordinates[f][j][1] = -1;
                        }
                        else
                        {
                            coordinates[f][j][1] = -2;
                        }
                    }
                    else
                    {
                        coordinates[f][j][1] = rand(5);
                    }
                }
                else
                {
                    coordinates[f][j][1] = -2;
                }
                int x = rand(17);
                int y = rand(6);
                coordinates[x+1][y][2] =  coordinates[x][y][2];
                coordinates[x-1][y][2] =  coordinates[x][y][2];
            }
            m += a;
            k -=a;
            if ((m-k) == 12)
            {
                a = -1;
                goto thislineee;
            }
            if ((m-k) == 0)
            {
                break;
            }
            j += 1;
        }
    }
}

void show(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0 , 0 , 0 , 255);
    SDL_RenderDrawLine(renderer, 0 , 600 , 180 , 600);
    SDL_RenderDrawLine(renderer, 220 , 600 , 280 , 600);
    SDL_RenderDrawLine(renderer, 320 , 600 , 600 , 600);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Delay(speed);
    SDL_RenderClear(renderer);
}

void draw_balls(int start_point , int coordinates[20][20][5] , SDL_Renderer * renderer)
{
    int i = 0;
    int Rx = 15;
    while (i < 20)
    {
        int j = 0;
        int Ry = start_point;
        while (j < 20)
        {
            if (coordinates[i][j][0] == 1) // show ball or not ?
            {
                if (coordinates[i][j][1] == -1) // for locked balls
                {
                    draw_circle(renderer , 2 , Rx , Ry  , 15 , coordinates[i][j][2] , coordinates[i][j][2]);
                }
                if (coordinates[i][j][1] == -2) // for normall balls
                {
                    draw_circle(renderer , 1 , Rx , Ry  , 15 , coordinates[i][j][2] , coordinates[i][j][2]);
                }
                else
                {
                    draw_circle(renderer , 3 , Rx , Ry  , 15 , coordinates[i][j][2] , coordinates[i][j][2]);
                }
            }
            coordinates[i][j][3] = Rx;
            coordinates[i][j][4] = Ry;
            j += 1;
            Ry += 30;
        }
        i += 1;
        Rx +=30;
    }
}

int loos(int coordinates[20][20][5])
{
    int result = 1;
    for (int i = 19;i>=0;i--)
    {
        for (int j = 19; j >=0; j--)
        {
            if (coordinates[i][j][0] == 1)
            {
                if (coordinates[i][j][4] >= 600)
                {
                    result = 0;
                }
            }
        }
    }
    return result;
}

void random_mode()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window = SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
    SDL_SetWindowPosition(window ,0 , 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    int coordinates[20][20][5];
    coord(0 , 7 , coordinates);
    int i = 15;
    while (loos(coordinates))
    {
        draw_balls(i , coordinates , renderer); // show ball in the window
        launch_balls(renderer , coordinates);
        show(renderer);
        i +=1;
    }
    SDL_DestroyWindow(window);
}

void triangle_mode()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window = SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
    SDL_SetWindowPosition(window ,0 , 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    int coordinates[20][20][5];
    coord(1 , 7 , coordinates);
    int i = 15;
    while (loos(coordinates))
    {
        draw_balls(i , coordinates , renderer); // show ball in the window
        launch_balls(renderer , coordinates);
        show(renderer);
        i +=1;
    }
    SDL_DestroyWindow(window);
}

void inverted_triangle_mode()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window = SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
    SDL_SetWindowPosition(window ,0 , 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    int coordinates[20][20][5];
    coord(3 , 7 , coordinates);
    int i = 15;
    while (loos(coordinates))
    {
        draw_balls(i , coordinates , renderer); // show ball in the window
        launch_balls(renderer , coordinates);
        show(renderer);
        i +=1;
    }
    SDL_DestroyWindow(window);
}

void diamond_mode()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window = SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
    SDL_SetWindowPosition(window ,0 , 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    int coordinates[20][20][5];
    coord(2 , 7 , coordinates);
    int i = 15;
    while (loos(coordinates))
    {
        draw_balls(i , coordinates , renderer); // show ball in the window
        launch_balls(renderer , coordinates);
        show(renderer);
        i +=1;
    }
    SDL_DestroyWindow(window);
}

void menu()
{
    here:
    system("CLS");
    int inp;
    Sleep(500);
    cout << "enter your choose : \n";
    Sleep(500);
    cout << "  [1] random mode \n";

    Sleep(500);

    cout << "  [2] triangle mode \n";
    Sleep(500);
    cout << "  [3] inverted triangle mode \n";
    Sleep(500);
    cout << "  [4] diamond mode \n";
    cin >> inp;
    switch (inp)
    {
        case 1 : random_mode();break;
        case 2 : triangle_mode();break;
        case 3 : inverted_triangle_mode();break;
        case 4 : diamond_mode();break;
        default : cout << "wrong input !";Sleep(1000);goto here;break;
    }
}

int main(int argc, char ** argv)
{
    while(true)
    {
        menu();
    }
}

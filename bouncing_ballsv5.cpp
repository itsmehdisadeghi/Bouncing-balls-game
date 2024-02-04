#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <time.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <SDL2/SDL_mixer.h>

using namespace std;

#define length 600
#define width 700
#define speed 100

bool help = false;

int local_score = 0;
string local_username;
bool quit = false;
bool stop = false;
int xmouse , ymouse;
bool resuem_menu = false;
const char* background = "C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\background_photo4.bmp";
int destroy_ball[20][20][1];
int color_find_chek[20][20][1];
int cannon_ball[3] = {0};//color -ballx - bally
int ballx = 300;
int bally = 600;
int ballc = 0;
int color_temp1[3]; // the ball that is ready to lounch
int color_temp2[3]; // the ball wait for replace
int  xMouse = -1;
int  yMouse;
float m;
float k;
int d = 16;
SDL_Event e;
SDL_Event q;
bool press = false;
bool ball = false;
bool first = true;
bool mute = 0;

int statusx = 1;
int statusy = 1;

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

void player_list(string username)
{
  fstream file("members.txt" , ios::in);
  string line1;
  cout<<"name  | high score" << endl;
  while (getline(file , line))
  {
    cout << line1;
    getline(file,line);
    getline(file,line);
    cout << " " << line<<endl;
  }
}

int set_score()
{ int result = 0;
  fstream file("members.txt" , ios::in);
  string line1;
  string temp;
  cout<<"name  | high score" << endl;
  while (getline(file , line))
  {
    if(line == local_username)
    {
      temp += (line + "\n");
      getline(file,line);
      temp += line;
      getline(file,line);
      int l = stoi(line);
      if (l<local_score)
      {
        string sc = to_string(local_score);
        temp += (sc + "\n");
        result = 1;
      }
      else
      {
        temp += (line + "\n");
      }
    }
    else
    {
      temp += (line + "\n");
    }
    cout << " " << line<<endl;
  }
  file.close();
  fstream wfile("members.txt" , ios::out);
  wfile<<temp;
  wfile.close();
  return result;
}

void playmusic(const char* musicFile) {
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Music* music = Mix_LoadMUS(musicFile);
    Mix_PlayMusic(music, -1);
    Mix_PlayingMusic();
//    while () {
//        SDL_Delay(100);
//    }
//    Mix_FreeMusic(music);
//    Mix_CloseAudio();
}
void destroy_balls(int coordinates[20][20][5])
{
    for (int i = 0;i<20;i++)
    {
        for(int j = 0;j<20;j++)
        {
            if (destroy_ball[i][j][0])
            {
              coordinates[i][j][0] = 0 ;
              local_score += 1;
            }
        }
    }
}

void reset_destroy_ball()
{
    for(int i = 0; i < 20 ; i++)
    {
        for ( int j = 0; j < 20 ; j++)
        {
            destroy_ball[i][j][0] = 0;
        }
    }
}
void reset_color_find_check()
{
    for(int i = 0; i < 20 ; i++)
    {
        for ( int j = 0; j < 20 ; j++)
        {
            color_find_chek[i][j][0] = 0;
        }
    }
}
void findcolor(int coordinates[20][20][5] , int i , int j)
{
    if (color_find_chek[i][j][0] == 0)
    {
        color_find_chek[i][j][0] = 1;
    }
    if (i==0)
    {
        if (((coordinates[i+1][j][2] == coordinates[i][j][2]) && (coordinates[i+1][j][0]))&& (color_find_chek[i+1][j][0] == 0))
        {
            destroy_ball[i+1][j][0] = 1;
            findcolor(coordinates, i+1 , j);
        }
        if (((coordinates[i+1][j-1][2] == coordinates[i][j][2]) && (coordinates[i+1][j-1][0]))&& (color_find_chek[i+1][j-1][0] == 0))
        {
            destroy_ball[i+1][j-1][0] = 1;
            findcolor(coordinates, i+1 , j-1);
        }
        if (((coordinates[i][j-1][2] == coordinates[i][j][2]) && (coordinates[i][j-1][0]))&& (color_find_chek[i][j-1][0] == 0))
        {
            destroy_ball[i][j-1][0] = 1;
            findcolor(coordinates, i , j-1);
        }
        return;
    }
    if (i==19)
    {
        if (((coordinates[i-1][j][2] == coordinates[i][j][2]) && (coordinates[i-1][j][0]))&& (color_find_chek[i-1][j][0] == 0))
        {
            destroy_ball[i-1][j][0] = 1;
            findcolor(coordinates, i-1 , j);
        }
        if (((coordinates[i-1][j-1][2] == coordinates[i][j][2]) && (coordinates[i-1][j-1][0]))&& (color_find_chek[i-1][j-1][0] == 0))
        {
            destroy_ball[i-1][j-1][0] = 1;
            findcolor(coordinates, i-1 , j-1);
        }
        if (((coordinates[i][j-1][2] == coordinates[i][j][2]) && (coordinates[i][j-1][0]))&& (color_find_chek[i][j-1][0] == 0))
        {
            destroy_ball[i][j-1][0] = 1;
            findcolor(coordinates, i , j-1);
        }
        return;
    }
    if (((coordinates[i-1][j][2] == coordinates[i][j][2]) && (coordinates[i-1][j][0])) && (color_find_chek[i-1][j][0] == 0))
    {
        destroy_ball[i-1][j][0] = 1;
        findcolor(coordinates , i-1 , j);
    }
    if (((coordinates[i-1][j-1][2] == coordinates[i][j][2]) && (coordinates[i-1][j-1][0])) && (color_find_chek[i-1][j-1][0] == 0))
    {
        destroy_ball[i-1][j-1][0] = 1;
        findcolor(coordinates, i-1 , j-1);
    }
    if (((coordinates[i][j-1][2] == coordinates[i][j][2]) && (coordinates[i][j-1][0]))&& (color_find_chek[i][j-1][0] == 0))
    {
        destroy_ball[i][j-1][0] = 1;
        findcolor(coordinates, i , j-1);
    }
    if (((coordinates[i+1][j-1][2] == coordinates[i][j][2]) && (coordinates[i+1][j-1][0]))&& (color_find_chek[i+1][j-1][0] == 0))
    {
        destroy_ball[i+1][j-1][0] = 1;
        findcolor(coordinates, i+1 , j-1);
    }
    if (((coordinates[i+1][j][2] == coordinates[i][j][2]) && (coordinates[i+1][j][0]))&& (color_find_chek[i+1][j][0] == 0))
    {
        destroy_ball[j+1][i][0] = 1;
        findcolor(coordinates, i+1 , j);
    }
    return;
}
int xcounter(int m, int d)
{
    int result;
    result = sqrt((d*d)/((m*m)+1));
    return result;
}
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
void launch_balls(SDL_Renderer * renderer , int coordinates[20][20][5]) {
    i:
    if (first) {
        h:
        int s = rand(6);
        int h = rand(6);
        if ((s == 4) || (h == 4)) goto h;
        color_temp1[0] = s;
        ballc = color_temp1[0];
        color_temp2[0] = h;
        first = false;
    }

    draw_circle(renderer, 1, 300, 600, 15, color_temp1[0], color_temp1[0]); // the ball that is ready to lounch
    draw_circle(renderer, 1, 200, 600, 15, color_temp2[0], color_temp2[0]); // the ball wait for replace

    int ex, ey;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_GetGlobalMouseState(&ex, &ey);
    k = float(600 - ey) / float(ex - 300);
    if ((ey <= 600) && (ex <= 600)) {
        SDL_RenderDrawLine(renderer, 300, 600, ex, ey);
    }
    SDL_PollEvent(&q);
    SDL_GetGlobalMouseState(&xmouse, &ymouse);
    if (((xmouse > 190) && (xmouse < 270) && (ymouse > 627) && (ymouse < 688)) && (q.type == SDL_MOUSEBUTTONDOWN) &&
        (q.button.button = SDL_BUTTON_LEFT)) {


    }
    if (((xmouse > 26) && (xmouse < 90) && (ymouse > 600) && (ymouse < 665)) && (q.type == SDL_MOUSEBUTTONDOWN) &&
        (q.button.button = SDL_BUTTON_LEFT)) {
        if(resuem_menu)
        {
            resuem_menu = false;
        }
        else resuem_menu = true;
        if(help)
        {
            help = false;
        }
    }
    if (((xmouse > 410) && (xmouse < 495) && (ymouse > 607) && (ymouse < 644)) && (q.type == SDL_MOUSEBUTTONDOWN) &&
        (q.button.button = SDL_BUTTON_LEFT)) {
        resuem_menu = false;
    }
    if (((xmouse > 507) && (xmouse < 590) && (ymouse > 607) && (ymouse < 644)) && (q.type == SDL_MOUSEBUTTONDOWN) &&
        (q.button.button = SDL_BUTTON_LEFT)) {
        quit = true;
    }

    if (((xmouse > 410) && (xmouse < 495) && (ymouse > 650) && (ymouse < 690)) && (q.type == SDL_MOUSEBUTTONDOWN) &&
        (q.button.button = SDL_BUTTON_LEFT)) {
        if(!mute)
        {
            Mix_PauseMusic();
            SDL_Delay(100);
            mute = 1;
        }
        else
        {
            Mix_ResumeMusic();
            SDL_Delay(100);
            mute = 0;
        }
    }
    if (((xmouse > 507) && (xmouse < 590) && (ymouse > 650) && (ymouse < 690)) && (q.type == SDL_MOUSEBUTTONDOWN) &&
        (q.button.button = SDL_BUTTON_LEFT)) {
        help = true;
    }
    if (!press) {
            if (((q.type == SDL_MOUSEBUTTONDOWN) && (q.button.button = SDL_BUTTON_LEFT)) && (ymouse < 600)) {
                SDL_GetGlobalMouseState(&xMouse, &yMouse);
                cord(coordinates, cannon_ball);
                color_temp1[0] = color_temp2[0];
                color_temp2[0] = cannon_ball[0];
                statusx = 1;
                statusy = 1;
                if (xMouse < 300) statusx *= -1;
                if (yMouse > 600) statusy *= -1;
                if ((yMouse < 600) && (xMouse > 300)) {
                    statusy = 1;
                    statusx = 1;
                }
                if ((yMouse < 600) && (xMouse < 300)) {
                    statusy = 1;
                    statusx = -1;
                }
                if ((yMouse > 600) && (xMouse < 300)) {
                    statusy = -1;
                    statusx = -1;
                }
                if ((yMouse > 600) && (xMouse > 300)) {
                    statusy = -1;
                    statusx = 1;
                }

                press = true;
            }
        }
    if (press) {
            if ((bally > 686) || (bally < 14)) statusy *= -1;
            if ((ballx > 586) || (ballx < 14)) statusx *= -1;
            m = abs((float(600 - yMouse) / float(xMouse - 300)));
            int x = xcounter(m, d);
            int y = m * x;
            ballx += (statusx * x);
            bally -= (statusy * y);
            ballc = color_temp1[0];
            draw_circle(renderer, 1, ballx, bally, 15, ballc, ballc);
            for (int i = 0; i < 20; i++) {
                for (int j = 0; j < 20; j++) {
                    if (coordinates[i][j][0]) {
                        if (abs(coordinates[i][j][3] - ballx) <= 15) {
                            if (abs(bally - coordinates[i][j][4]) <= 15) {
                                reset_destroy_ball();
                                reset_color_find_check();
                                destroy_ball[i][j][0] = 1;
                                color_find_chek[i][j][0] = 1;
                                findcolor(coordinates, i, j);
                                destroy_balls(coordinates);
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
     SDL_Surface* cannon = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\cannon.bmp");
     SDL_Texture* texture_cannon = SDL_CreateTextureFromSurface(renderer, cannon);
     SDL_Rect destcannon;
     destcannon.x = 225;
     destcannon.y = 490;
     destcannon.w = 155;
     destcannon.h = 155;
     SDL_RenderCopy(renderer, texture_cannon, NULL,  &destcannon);

    SDL_Surface *replace = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\replace.bmp");
    SDL_Texture *texture_rep = SDL_CreateTextureFromSurface(renderer, replace);
    SDL_Rect destrep;
    destrep.x = 195;
    destrep.y = 630;
    destrep.w = 60;
    destrep.h = 60;
    SDL_RenderCopy(renderer, texture_rep, NULL, &destrep);
    if(resuem_menu)
    {
        SDL_Surface *men = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\resume_menu.bmp");
        SDL_Texture *texture_men = SDL_CreateTextureFromSurface(renderer, men);
        SDL_Rect destmen;
        destmen.x = 400;
        destmen.y = 600;
        destmen.w = 200;
        destmen.h = 100;
        SDL_RenderCopy(renderer, texture_men, NULL, &destmen);
    }
    if(help)
    {
        SDL_Surface *menn = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\help_me.bmp");
        SDL_Texture *texture_menn = SDL_CreateTextureFromSurface(renderer, menn);
        SDL_Rect destmenn;
        destmenn.x = 400;
        destmenn.y = 600;
        destmenn.w = 200;
        destmenn.h = 100;
        SDL_RenderCopy(renderer, texture_menn, NULL, &destmenn);
    }

    SDL_Surface* menu = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\in_game_menu.bmp");
    SDL_Texture* texture_menu = SDL_CreateTextureFromSurface(renderer, menu);
    SDL_Rect destmenu;
    destmenu.x = 30;
    destmenu.y = 600;
    destmenu.w = 60;
    destmenu.h = 90;
    SDL_RenderCopy(renderer, texture_menu, NULL, &destmenu);
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
    SDL_Surface* back_img = SDL_LoadBMP(background);
    SDL_Texture* texture_img = SDL_CreateTextureFromSurface(renderer, back_img);
    SDL_Rect destimg;
    destimg.x = 0;
    destimg.y = 0;
    destimg.w = 600;
    destimg.h = 700;
    SDL_RenderCopy(renderer, texture_img, NULL, &destimg);
    int i = 0;
    int Rx = 15;
    while (i < 20) {
        int j = 0;
        int Ry = start_point;
        while (j < 20) {
            if (coordinates[i][j][0] == 1) // show ball or not ?
            {
                if (coordinates[i][j][1] == -1) // for locked balls
                {
                    draw_circle(renderer, 2, Rx, Ry, 15, coordinates[i][j][2], coordinates[i][j][2]);
                }
                if (coordinates[i][j][1] == -2) // for normall balls
                {
                    draw_circle(renderer, 1, Rx, Ry, 15, coordinates[i][j][2], coordinates[i][j][2]);
                } else {
                    draw_circle(renderer, 3, Rx, Ry, 15, coordinates[i][j][2], coordinates[i][j][2]);
                }
            }
            coordinates[i][j][3] = Rx;
            coordinates[i][j][4] = Ry;
            j += 1;
            Ry += 30;
        }
        i += 1;
        Rx += 30;
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
int win(int coordinates[20][20][5])
{
    int result = 1;
    for (int i = 19;i>=0;i--)
    {
        for (int j = 19; j >=0; j--)
        {
            if (coordinates[i][j][0] == 1)
            {
              result = 0
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
        if(win(coordiantes))
        {
          if(set_score())
          {
            SDL_DestroyWindow(window);
            SDL_Init(SDL_INIT_EVERYTHING);
            SDL_Window * window3= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
            SDL_SetWindowPosition(window3 ,0 , 0);
            SDL_Renderer * renderer3 = SDL_CreateRenderer(window3, -1, 0);
            SDL_SetRenderDrawColor(renderer3, 255, 255, 255, 255);
            SDL_RenderClear(renderer3);
            SDL_Surface* game_win = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\you_win.bmp");
            SDL_Texture* txt_win = SDL_CreateTextureFromSurface(renderer3, game_win);
            SDL_Rect destRect1;
            destRect1.x = 0;
            destRect1.y = 0;
            destRect1.w = 600;
            destRect1.h = 700;
            SDL_RenderClear(renderer3);
            SDL_RenderCopy(renderer3, txt_game_win, NULL, &destRect1);
            SDL_RenderPresent(renderer3);
            SDL_Delay(3000);
            SDL_DestroyWindow(window3);
          }
          else
          {
            SDL_DestroyWindow(window);
            SDL_Init(SDL_INIT_EVERYTHING);
            SDL_Window * window3= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
            SDL_SetWindowPosition(window3 ,0 , 0);
            SDL_Renderer * renderer3 = SDL_CreateRenderer(window3, -1, 0);
            SDL_SetRenderDrawColor(renderer3, 255, 255, 255, 255);
            SDL_RenderClear(renderer3);
            SDL_Surface* game_win = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\you_win_high.bmp");
            SDL_Texture* txt_win = SDL_CreateTextureFromSurface(renderer3, game_win);
            SDL_Rect destRect1;
            destRect1.x = 0;
            destRect1.y = 0;
            destRect1.w = 600;
            destRect1.h = 700;
            SDL_RenderClear(renderer3);
            SDL_RenderCopy(renderer3, txt_game_win, NULL, &destRect1);
            SDL_RenderPresent(renderer3);
            SDL_Delay(3000);
            SDL_DestroyWindow(window3);
          }
        }
        if(quit)
        {
            SDL_DestroyWindow(window);
            quit= false;
            return;
        }
        draw_balls(i , coordinates , renderer); // show ball in the window
        launch_balls(renderer , coordinates);
        show(renderer);
        if((resuem_menu) ||(stop))
        {
            continue;
        }
        else
        {
            i +=1;
        }
    }
    SDL_DestroyWindow(window);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window1= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
    SDL_SetWindowPosition(window ,0 , 0);
    SDL_Renderer * renderer1 = SDL_CreateRenderer(window1, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_Surface* game_over = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\Game_over.bmp");
    SDL_Texture* txt_game_over = SDL_CreateTextureFromSurface(renderer1, game_over);
    SDL_Rect destRect;
    destRect.x = 0;
    destRect.y = 0;
    destRect.w = 600;
    destRect.h = 700;
    SDL_RenderClear(renderer1);
    SDL_RenderCopy(renderer1, txt_game_over, NULL, &destRect);
    SDL_RenderPresent(renderer1);
    SDL_Delay(3000);
    SDL_DestroyWindow(window1);
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
      if(win(coordiantes))
      {
        if(set_score())
        {
          SDL_DestroyWindow(window);
          SDL_Init(SDL_INIT_EVERYTHING);
          SDL_Window * window3= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
          SDL_SetWindowPosition(window3 ,0 , 0);
          SDL_Renderer * renderer3 = SDL_CreateRenderer(window3, -1, 0);
          SDL_SetRenderDrawColor(renderer3, 255, 255, 255, 255);
          SDL_RenderClear(renderer3);
          SDL_Surface* game_win = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\you_win.bmp");
          SDL_Texture* txt_win = SDL_CreateTextureFromSurface(renderer3, game_win);
          SDL_Rect destRect1;
          destRect1.x = 0;
          destRect1.y = 0;
          destRect1.w = 600;
          destRect1.h = 700;
          SDL_RenderClear(renderer3);
          SDL_RenderCopy(renderer3, txt_game_win, NULL, &destRect1);
          SDL_RenderPresent(renderer3);
          SDL_Delay(3000);
          SDL_DestroyWindow(window3);
        }
        else
        {
          SDL_DestroyWindow(window);
          SDL_Init(SDL_INIT_EVERYTHING);
          SDL_Window * window3= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
          SDL_SetWindowPosition(window3 ,0 , 0);
          SDL_Renderer * renderer3 = SDL_CreateRenderer(window3, -1, 0);
          SDL_SetRenderDrawColor(renderer3, 255, 255, 255, 255);
          SDL_RenderClear(renderer3);
          SDL_Surface* game_win = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\you_win_high.bmp");
          SDL_Texture* txt_win = SDL_CreateTextureFromSurface(renderer3, game_win);
          SDL_Rect destRect1;
          destRect1.x = 0;
          destRect1.y = 0;
          destRect1.w = 600;
          destRect1.h = 700;
          SDL_RenderClear(renderer3);
          SDL_RenderCopy(renderer3, txt_game_win, NULL, &destRect1);
          SDL_RenderPresent(renderer3);
          SDL_Delay(3000);
          SDL_DestroyWindow(window3);
        }
      }
        if(quit)
        {
            SDL_DestroyWindow(window);
            quit= false;
            return;
        }
        draw_balls(i , coordinates , renderer); // show ball in the window
        launch_balls(renderer , coordinates);
        show(renderer);
        if((resuem_menu) ||(stop))
        {
            continue;
        }
        else
        {
            i +=1;
        }
    }
    SDL_DestroyWindow(window);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window1= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
    SDL_SetWindowPosition(window ,0 , 0);
    SDL_Renderer * renderer1 = SDL_CreateRenderer(window1, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_Surface* game_over = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\Game_over.bmp");
    SDL_Texture* txt_game_over = SDL_CreateTextureFromSurface(renderer1, game_over);
    SDL_Rect destRect;
    destRect.x = 0;
    destRect.y = 0;
    destRect.w = 600;
    destRect.h = 700;
    SDL_RenderClear(renderer1);
    SDL_RenderCopy(renderer1, txt_game_over, NULL, &destRect);
    SDL_RenderPresent(renderer1);
    SDL_Delay(3000);
    SDL_DestroyWindow(window1);
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
      if(win(coordiantes))
      {
        if(set_score())
        {
          SDL_DestroyWindow(window);
          SDL_Init(SDL_INIT_EVERYTHING);
          SDL_Window * window3= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
          SDL_SetWindowPosition(window3 ,0 , 0);
          SDL_Renderer * renderer3 = SDL_CreateRenderer(window3, -1, 0);
          SDL_SetRenderDrawColor(renderer3, 255, 255, 255, 255);
          SDL_RenderClear(renderer3);
          SDL_Surface* game_win = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\you_win.bmp");
          SDL_Texture* txt_win = SDL_CreateTextureFromSurface(renderer3, game_win);
          SDL_Rect destRect1;
          destRect1.x = 0;
          destRect1.y = 0;
          destRect1.w = 600;
          destRect1.h = 700;
          SDL_RenderClear(renderer3);
          SDL_RenderCopy(renderer3, txt_game_win, NULL, &destRect1);
          SDL_RenderPresent(renderer3);
          SDL_Delay(3000);
          SDL_DestroyWindow(window3);
        }
        else
        {
          SDL_DestroyWindow(window);
          SDL_Init(SDL_INIT_EVERYTHING);
          SDL_Window * window3= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
          SDL_SetWindowPosition(window3 ,0 , 0);
          SDL_Renderer * renderer3 = SDL_CreateRenderer(window3, -1, 0);
          SDL_SetRenderDrawColor(renderer3, 255, 255, 255, 255);
          SDL_RenderClear(renderer3);
          SDL_Surface* game_win = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\you_win_high.bmp");
          SDL_Texture* txt_win = SDL_CreateTextureFromSurface(renderer3, game_win);
          SDL_Rect destRect1;
          destRect1.x = 0;
          destRect1.y = 0;
          destRect1.w = 600;
          destRect1.h = 700;
          SDL_RenderClear(renderer3);
          SDL_RenderCopy(renderer3, txt_game_win, NULL, &destRect1);
          SDL_RenderPresent(renderer3);
          SDL_Delay(3000);
          SDL_DestroyWindow(window3);
        }
      }
        if(quit)
        {
            SDL_DestroyWindow(window);
            quit= false;
            return;
        }
        draw_balls(i , coordinates , renderer); // show ball in the window
        launch_balls(renderer , coordinates);
        show(renderer);
        if((resuem_menu) ||(stop))
        {
            continue;
        }
        else
        {
            i +=1;
        }
    }
    SDL_DestroyWindow(window);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window1= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
    SDL_SetWindowPosition(window ,0 , 0);
    SDL_Renderer * renderer1 = SDL_CreateRenderer(window1, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_Surface* game_over = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\Game_over.bmp");
    SDL_Texture* txt_game_over = SDL_CreateTextureFromSurface(renderer1, game_over);
    SDL_Rect destRect;
    destRect.x = 0;
    destRect.y = 0;
    destRect.w = 600;
    destRect.h = 700;
    SDL_RenderClear(renderer1);
    SDL_RenderCopy(renderer1, txt_game_over, NULL, &destRect);
    SDL_RenderPresent(renderer1);
    SDL_Delay(3000);
    SDL_DestroyWindow(window1);
}
void hourglassed_mode()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window = SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
    SDL_SetWindowPosition(window ,0 , 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    int coordinates[20][20][5];
    coord(4 , 7 , coordinates);

    int i = 15;
    while (loos(coordinates))
    {
      if(win(coordiantes))
      {
        if(set_score())
        {
          SDL_DestroyWindow(window);
          SDL_Init(SDL_INIT_EVERYTHING);
          SDL_Window * window3= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
          SDL_SetWindowPosition(window3 ,0 , 0);
          SDL_Renderer * renderer3 = SDL_CreateRenderer(window3, -1, 0);
          SDL_SetRenderDrawColor(renderer3, 255, 255, 255, 255);
          SDL_RenderClear(renderer3);
          SDL_Surface* game_win = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\you_win.bmp");
          SDL_Texture* txt_win = SDL_CreateTextureFromSurface(renderer3, game_win);
          SDL_Rect destRect1;
          destRect1.x = 0;
          destRect1.y = 0;
          destRect1.w = 600;
          destRect1.h = 700;
          SDL_RenderClear(renderer3);
          SDL_RenderCopy(renderer3, txt_game_win, NULL, &destRect1);
          SDL_RenderPresent(renderer3);
          SDL_Delay(3000);
          SDL_DestroyWindow(window3);
        }
        else
        {
          SDL_DestroyWindow(window);
          SDL_Init(SDL_INIT_EVERYTHING);
          SDL_Window * window3= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
          SDL_SetWindowPosition(window3 ,0 , 0);
          SDL_Renderer * renderer3 = SDL_CreateRenderer(window3, -1, 0);
          SDL_SetRenderDrawColor(renderer3, 255, 255, 255, 255);
          SDL_RenderClear(renderer3);
          SDL_Surface* game_win = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\you_win_high.bmp");
          SDL_Texture* txt_win = SDL_CreateTextureFromSurface(renderer3, game_win);
          SDL_Rect destRect1;
          destRect1.x = 0;
          destRect1.y = 0;
          destRect1.w = 600;
          destRect1.h = 700;
          SDL_RenderClear(renderer3);
          SDL_RenderCopy(renderer3, txt_game_win, NULL, &destRect1);
          SDL_RenderPresent(renderer3);
          SDL_Delay(3000);
          SDL_DestroyWindow(window3);
        }
      }
        if(quit)
        {
            SDL_DestroyWindow(window);
            quit= false;
            return;
        }
        draw_balls(i , coordinates , renderer); // show ball in the window
        launch_balls(renderer , coordinates);
        show(renderer);
        if((resuem_menu) ||(stop))
        {
            continue;
        }
        else
        {
            i +=1;
        }
    }
    SDL_DestroyWindow(window);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window1= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
    SDL_SetWindowPosition(window ,0 , 0);
    SDL_Renderer * renderer1 = SDL_CreateRenderer(window1, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_Surface* game_over = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\Game_over.bmp");
    SDL_Texture* txt_game_over = SDL_CreateTextureFromSurface(renderer1, game_over);
    SDL_Rect destRect;
    destRect.x = 0;
    destRect.y = 0;
    destRect.w = 600;
    destRect.h = 700;
    SDL_RenderClear(renderer1);
    SDL_RenderCopy(renderer1, txt_game_over, NULL, &destRect);
    SDL_RenderPresent(renderer1);
    SDL_Delay(3000);
    SDL_DestroyWindow(window1);
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
      if(win(coordiantes))
      {
        if(set_score())
        {
          SDL_DestroyWindow(window);
          SDL_Init(SDL_INIT_EVERYTHING);
          SDL_Window * window3= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
          SDL_SetWindowPosition(window3 ,0 , 0);
          SDL_Renderer * renderer3 = SDL_CreateRenderer(window3, -1, 0);
          SDL_SetRenderDrawColor(renderer3, 255, 255, 255, 255);
          SDL_RenderClear(renderer3);
          SDL_Surface* game_win = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\you_win.bmp");
          SDL_Texture* txt_win = SDL_CreateTextureFromSurface(renderer3, game_win);
          SDL_Rect destRect1;
          destRect1.x = 0;
          destRect1.y = 0;
          destRect1.w = 600;
          destRect1.h = 700;
          SDL_RenderClear(renderer3);
          SDL_RenderCopy(renderer3, txt_game_win, NULL, &destRect1);
          SDL_RenderPresent(renderer3);
          SDL_Delay(3000);
          SDL_DestroyWindow(window3);
        }
        else
        {
          SDL_DestroyWindow(window);
          SDL_Init(SDL_INIT_EVERYTHING);
          SDL_Window * window3= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
          SDL_SetWindowPosition(window3 ,0 , 0);
          SDL_Renderer * renderer3 = SDL_CreateRenderer(window3, -1, 0);
          SDL_SetRenderDrawColor(renderer3, 255, 255, 255, 255);
          SDL_RenderClear(renderer3);
          SDL_Surface* game_win = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\you_win_high.bmp");
          SDL_Texture* txt_win = SDL_CreateTextureFromSurface(renderer3, game_win);
          SDL_Rect destRect1;
          destRect1.x = 0;
          destRect1.y = 0;
          destRect1.w = 600;
          destRect1.h = 700;
          SDL_RenderClear(renderer3);
          SDL_RenderCopy(renderer3, txt_game_win, NULL, &destRect1);
          SDL_RenderPresent(renderer3);
          SDL_Delay(3000);
          SDL_DestroyWindow(window3);
        }
      }
        if(quit)
        {
            SDL_DestroyWindow(window);
            quit= false;
            return;
        }
        draw_balls(i , coordinates , renderer); // show ball in the window
        launch_balls(renderer , coordinates);
        show(renderer);
        if((resuem_menu) ||(stop))
        {
            continue;
        }
        else
        {
            i +=1;
        }
    }
    SDL_DestroyWindow(window);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window1= SDL_CreateWindow("bouncing balls",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, length, width, SDL_WINDOW_SHOWN);
    SDL_SetWindowPosition(window ,0 , 0);
    SDL_Renderer * renderer1 = SDL_CreateRenderer(window1, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_Surface* game_over = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\Game_over.bmp");
    SDL_Texture* txt_game_over = SDL_CreateTextureFromSurface(renderer1, game_over);
    SDL_Rect destRect;
    destRect.x = 0;
    destRect.y = 0;
    destRect.w = 600;
    destRect.h = 700;
    SDL_RenderClear(renderer1);
    SDL_RenderCopy(renderer1, txt_game_over, NULL, &destRect);
    SDL_RenderPresent(renderer1);
    SDL_Delay(3000);
    SDL_DestroyWindow(window1);
}
void menu() {
    quit = false;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Image Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 700, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetWindowPosition(window ,0 , 0);

    SDL_Surface* background1 = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\background1.bmp");
    SDL_Surface* unmuted = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\unmuted.bmp");
    SDL_Surface* help = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\help.bmp");
    SDL_Surface* tick = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\tick.bmp");
    SDL_Surface* modees = SDL_LoadBMP("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\modes.bmp");

    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, background1);
    SDL_Texture* txunmuted = SDL_CreateTextureFromSurface(renderer, unmuted);
    SDL_Texture* texture_help = SDL_CreateTextureFromSurface(renderer, help);
    SDL_Texture* texr = SDL_CreateTextureFromSurface(renderer, modees);
    SDL_Texture* texture_tick1 = SDL_CreateTextureFromSurface(renderer, tick);
    SDL_Texture* texture_tick2 = SDL_CreateTextureFromSurface(renderer, tick);


    SDL_Rect destRect;
    destRect.x = 0;
    destRect.y = 0;
    destRect.w = 600;
    destRect.h = 700;

    SDL_Rect desttick;
    desttick.x = 90;
    desttick.y = 250;
    desttick.w = 50;
    desttick.h = 50;

    SDL_Rect desttick2;
    desttick2.x = 90;
    desttick2.y = 350;
    desttick2.w = 50;
    desttick2.h = 50;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture1, NULL, &destRect);
    SDL_RenderPresent(renderer);
    playmusic("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\ddaisy.mp3");
    bool setting =0 ;
    bool menu = 0;

    while(true)
    {
        SDL_PollEvent(&e);
        SDL_GetGlobalMouseState(&xMouse,&yMouse);
        // setting
        if ((!menu)&&(!setting)) // out of start menu and setting
        {
            // setting menu
            if (((xMouse > 470) && (xMouse < 570) && (yMouse > 530) && (yMouse <610)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, txunmuted, NULL, &destRect);
                SDL_RenderPresent(renderer);
                setting = 1;
            }
            // exite
            if (((xMouse > 30) && (xMouse < 140) && (yMouse > 530) && (yMouse <610)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                SDL_DestroyWindow(window);
                return;
            }
            // start ( go to menu )
            if (((xMouse > 240) && (xMouse < 360) && (yMouse >320 ) && (yMouse <380)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texr, NULL, &destRect);
                SDL_RenderPresent(renderer);
                SDL_Delay(1000);
                xMouse = 0;
                menu = 1;
            }
        }
        if(setting) // in setting
        {
            // go back
            if (((xMouse > 40) && (xMouse < 180) && (yMouse > 620) && (yMouse <690)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture1, NULL, &destRect);
                SDL_RenderPresent(renderer);
                setting = 0;
            }
            // help
            if (((xMouse > 298) && (xMouse < 417) && (yMouse > 8) && (yMouse <145)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture_help, NULL, &destRect);
                SDL_RenderPresent(renderer);
            }
            //music
            if (((xMouse > 450) && (xMouse < 580) && (yMouse > 8) && (yMouse <145)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                if(!mute)
                {
                    Mix_PauseMusic();
                    SDL_Delay(100);
                    mute = 1;
                }
                else
                {
                    Mix_ResumeMusic();
                    SDL_Delay(100);
                    mute = 0;
                }
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, txunmuted, NULL, &destRect);
                desttick.x = 490;
                desttick.y = 50;
                desttick.w = 50;
                desttick.h = 50;
                SDL_RenderCopy(renderer, texture_tick1, NULL, &desttick);
                SDL_RenderPresent(renderer);
            }


            if (((xMouse > 50) && (xMouse < 150) && (yMouse > 214) && (yMouse <351)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                background = "C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\background_photo1.bmp";
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, txunmuted, NULL, &destRect);
                desttick.x = 90;
                desttick.y = 250;
                desttick.w = 50;
                desttick.h = 50;
                SDL_RenderCopy(renderer, texture_tick1, NULL, &desttick);
                SDL_RenderPresent(renderer);
            }
            if (((xMouse > 184) && (xMouse < 306) && (yMouse > 214) && (yMouse <351)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                background = "C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\background_photo2.bmp";
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, txunmuted, NULL, &destRect);
                desttick.x = 235;
                desttick.y = 250;
                desttick.w = 50;
                desttick.h = 50;
                SDL_RenderCopy(renderer, texture_tick1, NULL, &desttick);
                SDL_RenderPresent(renderer);
            }
            if (((xMouse > 314) && (xMouse < 437) && (yMouse > 214) && (yMouse <351)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                background = "C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\background_photo3.bmp";
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, txunmuted, NULL, &destRect);
                desttick.x = 340;
                desttick.y = 250;
                desttick.w = 50;
                desttick.h = 50;
                SDL_RenderCopy(renderer, texture_tick1, NULL, &desttick);
                SDL_RenderPresent(renderer);
            }
            if (((xMouse > 450) && (xMouse < 570) && (yMouse > 214) && (yMouse <351)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                background = "C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\background_photo4.bmp";
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, txunmuted, NULL, &destRect);
                desttick.x = 490;
                desttick.y = 250;
                desttick.w = 50;
                desttick.h = 50;
                SDL_RenderCopy(renderer, texture_tick1, NULL, &desttick);
                SDL_RenderPresent(renderer);
            }



            if (((xMouse > 50) && (xMouse < 150) && (yMouse > 460) && (yMouse <600)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                Mix_CloseAudio();
                playmusic("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\ordak_tak_tak.mp3");
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture_tick1, NULL, &desttick);
                SDL_RenderCopy(renderer, txunmuted, NULL, &destRect);
                desttick2.x = 90;
                desttick2.y = 500;
                desttick2.w = 50;
                desttick2.h = 50;
                SDL_RenderCopy(renderer, texture_tick2, NULL, &desttick2);
                SDL_RenderPresent(renderer);
            }
            if (((xMouse > 184) && (xMouse < 306) && (yMouse > 460) && (yMouse <600)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                Mix_CloseAudio();
                playmusic("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\Fluffing-a-Duck.mp3");
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture_tick1, NULL, &desttick);
                SDL_RenderCopy(renderer, txunmuted, NULL, &destRect);
                desttick2.x = 235;
                desttick2.y = 500;
                desttick2.w = 50;
                desttick2.h = 50;
                SDL_RenderCopy(renderer, texture_tick2, NULL, &desttick2);
                SDL_RenderPresent(renderer);
            }
            if (((xMouse > 314) && (xMouse < 437) && (yMouse > 460) && (yMouse <600)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                Mix_CloseAudio();
                playmusic("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\run_amok.mp3");
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture_tick1, NULL, &desttick);
                SDL_RenderCopy(renderer, txunmuted, NULL, &destRect);
                desttick2.x = 340;
                desttick2.y = 500;
                desttick2.w = 50;
                desttick2.h = 50;
                SDL_RenderCopy(renderer, texture_tick2, NULL, &desttick2);
                SDL_RenderPresent(renderer);
            }
            if (((xMouse > 450) && (xMouse < 570) && (yMouse > 460) && (yMouse <600)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                Mix_CloseAudio();
                playmusic("C:\\Users\\HP NOAVAR\\CLionProjects\\cp1\\ddaisy.mp3");
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture_tick1, NULL, &desttick);
                SDL_RenderCopy(renderer, txunmuted, NULL, &destRect);
                desttick2.x = 490;
                desttick2.y = 500;
                desttick2.w = 50;
                desttick2.h = 50;
                SDL_RenderCopy(renderer, texture_tick2, NULL, &desttick2);
                SDL_RenderPresent(renderer);
            }
        }
        if(menu) // in menu
        {
            // go back in menu
            if (((xMouse > 60) && (xMouse < 200) && (yMouse >520 ) && (yMouse <580)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture1, NULL, &destRect);
                SDL_RenderPresent(renderer);
                menu = 0;
            }
            // random button
            if (((xMouse > 230) && (xMouse < 360) && (yMouse >300 ) && (yMouse <400)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture1, NULL, &destRect);
                SDL_RenderPresent(renderer);
                random_mode();
                menu = 0;
            }
            // diamond button
            if (((xMouse > 400) && (xMouse < 530) && (yMouse >370 ) && (yMouse <470)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture1, NULL, &destRect);
                SDL_RenderPresent(renderer);
                diamond_mode();
                menu = 0;
            }
            // triangle
            if (((xMouse > 70) && (xMouse < 200) && (yMouse >370 ) && (yMouse <470)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture1, NULL, &destRect);
                SDL_RenderPresent(renderer);
                triangle_mode();
                menu = 0;
            }
            // inveted triangle
            if (((xMouse > 70) && (xMouse < 200) && (yMouse >260 ) && (yMouse <350)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture1, NULL, &destRect);
                SDL_RenderPresent(renderer);
                inverted_triangle_mode();
                menu = 0;
            }
            // hourglass
            if (((xMouse > 400) && (xMouse < 530) && (yMouse >260 ) && (yMouse <360)) && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button = SDL_BUTTON_LEFT))
            {
                SDL_RenderClear(renderer);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture1, NULL, &destRect);
                SDL_RenderPresent(renderer);
                hourglassed_mode();
                menu = 0;
            }
        }
    }
}
bool username_password_validation(string username , string password)
{
    fstream file("members.txt" , ios::in);
    string line;
    while(getline(file , line))
    {
        if (line == username)
        {
            getline(file , line);
            if(line == password)
            {
                file.close();
                return true;
            }
            else
            {
                file.close();
                return false;
            }
        }
    }
    file.close();
    return false;
}
bool user_name_check(string username)
{
    fstream file("members.txt" , ios::in);
    string line;
    while(getline(file , line))
    {
        if (line == username)
        {
            file.close();
            return false;
        }
    }
    file.close();
    return false;
}
void console_menu()
{
    string username;
    string password;
    int user_choice;
    cout << "[1] sign in" << endl << "[2] login" << endl << "[3] members list" <<endl << "[4] exit"<<endl;
    cin >> user_choice;
    switch(user_choice)
    {
        case 1 :
        {
            string username , password;
            cout << "username : ";
            cin >> username;
            cout << "password : ";
            cin >> password;
            if (user_name_check(username))
            {
                cout << "this username already token" << endl;
                break;
            }
            ofstream file("members.txt" , ios::app);
            file<<username<<endl<<password<<endl<<"0"<<endl;
            file.close();
            cout << "your account has been created successfully" << endl;
            break;
        }
        case 2 :
        {
            string username , password;
            cout << "username : ";
            cin >> username;
            cout << "password : ";
            cin >> password;
            if(username_password_validation(username ,password))
            {
              cout << "Welcome "  << username << endl;
              local_username = username;
                menu();

            }
            else
            {
                cout << "invalid password or username" <<endl;
                break;
            }
        }
        case 3:
        {
            cout << "Player List : " <<endl;
            break;
        }
        case 4: exit(0);
        default :
        {
            cout << "invalid input";
            break;
        }
    }
}
int main(int argc, char ** argv)
{
    while(true)
    {
        console_menu();
    }
}

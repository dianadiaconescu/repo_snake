
#include "raylib.h"
#include <assert.h>
#include <stddef.h>
#include <time.h>
#define D 10//Dimensions of the grid
typedef struct{
    int x;
    int y;
}Point;//coord snake
typedef enum
{
    Grid_Empty=0,
    Grid_Food,
    Grid_North,
    Grid_South,
    Grid_East,
    Grid_West   
}Grid_Type;

static inline Point point_plus_direction(Point p,Grid_Type t)
{
    Point new_p=p;
    switch(t)
    {
        case Grid_North:
            --new_p.y;
            break;
        case Grid_South:
            ++new_p.y;
            break;
        case Grid_East:
            ++new_p.x;
            break;
        case Grid_West:
            --new_p.x;
            break;
        case Grid_Empty:
        case Grid_Food:
        default:
            return p;//This should never happen, as the snake should only move in one of the four directions
            break;
        
    }
    return new_p;
}
typedef enum{
    Scene_Menu=0,
    Scene_Game,
    Scene_Win,
    Scene_Lost
}Scene;

static inline void place_food( Grid_Type grid[D][D], const int snake_size)
{   
    Point p;
    if(D*D/2 < snake_size)//snake e foarte mare, e ineficient a ghicesti o pozitie pentru mancare
    {
        Point points[D*D];
        int len=0;
        for(size_t y=0;y<D;++y){
            for(size_t x=0; x<D; ++x)
            {
                if(grid[y][x]=Grid_Empty){
                    points[len].x=x;
                    points[len].y=y;
                    ++len;
                }
            }
        }
        p=points[GetRandomValue(0,len-1)];
    }
    else//practic pune random mancarea
    {
        
        do{
            p.x= GetRandomValue(0,D-1);
            p.y= GetRandomValue(0,D-1);
            
        }while(grid[p.y][p.x]!=Grid_Empty);
        
    }
    grid[p.y][p.x]=Grid_Food;
    
    
} 
int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;

    const int start_d=(int)(screenWidth*0.1f);//The distance from the edge of the screen to the start of the playable area
    const int play_d=(int)(screenWidth*0.8f);//The size of the playable area, which is a square
    const int grid_d=play_d/D;//The size of each cell in the grid
    unsigned int frame_count = 0;
    int snake_size;//size of the snake
    Point snake_head;//capul snake
    Point snake_tail;//coada snake
    Grid_Type snake_direction;//direcția în care se mișcă șarpele
    Grid_Type grid[D][D]={0};//The grid of the game, initially empty
    

    bool show_menu_instructions=true;//Whether to show the instructions in the menu scene or not

    Scene current_scene = Scene_Menu;//The game starts in the menu scene

    InitWindow(screenWidth, screenHeight, "Snake");

    SetTargetFPS(60);

    SetRandomSeed(time(NULL));

    while (!WindowShouldClose())
    {
        ++frame_count;
        ////////////////////////////////////////////////////////////////////////
        ////Update the game
            switch(current_scene)
            {
                case Scene_Menu:
                {
                   if(IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                   {
                        current_scene = Scene_Game;
                        snake_size=1;
                        snake_head.x=D/2;
                        snake_head.y=D/2;
                        snake_tail=snake_head;
                        snake_direction=Grid_North;

                        for(size_t y=0;y<D;++y)
                        {
                            for(size_t x=0;x<D;++x)
                            {
                                grid[y][x]=Grid_Empty;
                            }
                        }

                        grid[snake_head.y][snake_head.x]=snake_direction;
                        place_food(grid, snake_size);
                        

                        frame_count=1;//Reset the frame count when starting the game, so that the instructions toggle in the menu doesn't affect the game
                   }
                   if(frame_count%30==0)
                   {
                        show_menu_instructions=!show_menu_instructions;//Toggle the instructions every 30 frames,practic se misca
                   }
                    break;
                }
                case Scene_Game:
                {
                    if(IsKeyPressed(KEY_Q))
                    {
                        current_scene = Scene_Menu;
                    }
                    if((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && grid[snake_head.y][snake_head.x]!=Grid_South)
                    {
                        ///Q:e bine sa exista "a collision with the self?"-aici nu exista ---> nu poate deveni ---<
                        snake_direction=Grid_North;
                    }
                    else if((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && grid[snake_head.y][snake_head.x]!=Grid_North)
                    {
                        snake_direction=Grid_South;
                    }
                    else if((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && grid[snake_head.y][snake_head.x]!=Grid_West)
                    {
                        snake_direction=Grid_East;
                    }
                    else if((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && grid[snake_head.y][snake_head.x]!=Grid_East)
                    {
                        snake_direction=Grid_West;
                    }
                    if(frame_count % 30 == 0)
                    {   //ii plimba capul
                        grid[snake_head.y][snake_head.x]=snake_direction;//Update the grid with the new direction of the snake head
                        snake_head=point_plus_direction(snake_head, snake_direction);//Move the snake head in the direction it's moving
                        
                        if(snake_head.x<0 || snake_head.x >=D || snake_head.y < 0 || snake_head.y >= D)
                        {
                            current_scene=Scene_Lost;
                            frame_count=1;
                        }
                        else
                        {
                            switch(grid[snake_head.y][snake_head.x])
                            {

                            case Grid_Food:
                            {
                                ++snake_size;
                                //cand daam de mancare nu se goleste patratul de coada, aparent snakeul creste
                                if(snake_size==D*D)
                                {
                                    current_scene=Scene_Win;
                                    frame_count=1;
                                }
                                else{
                                    place_food(grid, snake_size);
                                }
                                


                            break;
                            }
                            case Grid_North:
                            case Grid_East:
                            case Grid_South:
                            case Grid_West:
                            {
                                current_scene=Scene_Lost;
                                frame_count=1;
                                break;
                            }
                            ////
                            
                            //dc nu e nimic acolo nu ne pasa 
                            case Grid_Empty:{
                                //ii trage coada dupa el
                                Grid_Type temp_dir=grid[snake_tail.y][snake_tail.x];//Get the direction of the snake tail, which is the direction of the next cell in the snake body
                                grid[snake_tail.y][snake_tail.x]=Grid_Empty;//Clear the cell of the snake tail, as it will move forward
                                snake_tail=point_plus_direction(snake_tail, temp_dir);//Move the snake tail in the direction of the next cell in the snake body
                    
                            }
                            default:
                            break;
                            }
                            grid[snake_head.y][snake_head.x]=snake_direction;

                        }





                        
                        }
                    break;
                }
                case Scene_Lost:
                case Scene_Win:
                {
                    if(frame_count%90==0)
                    {
                        current_scene=Scene_Menu;
                    }
                    break;
                }
                
                {
                    break;
                }
                default:
                    break;
            } 
        ////////////////////////////////////////////////////////////////////////
        ////Render the game 

        BeginDrawing();
        ClearBackground(BLACK);
        switch(current_scene)
        {
            case Scene_Menu:
            {
                const char* title = "SNAKE";
                const int t_size = 40;
                const int t_w=MeasureText(title, t_size);
                DrawText(title, (screenWidth - t_w) / 2, screenHeight/4, t_size, WHITE);
                
                if(show_menu_instructions)
                {
                    const char* instructions = "Press 'space' to play";
                    const int i_size = 30;
                    const int i_w=MeasureText(instructions, i_size);
                
                    DrawText(instructions, (screenWidth - i_w) / 2, screenHeight/2, i_size, WHITE);
                }
                break;
            }
            case Scene_Game:
            {
                for(int y_i=0;y_i<D;++y_i)
                {
                    const int y_r=start_d+(y_i*grid_d);
                    for(int x_i=0;x_i<D;++x_i)
                    {
                        const int x_r=start_d+(x_i*grid_d);
                        switch (grid[y_i][x_i])
                        {
                                break;
                            case Grid_Food:
                                DrawRectangle(x_r, y_r, grid_d-1, grid_d-1, GREEN);
                                break;
                            case Grid_North:
                            case Grid_South:
                            case Grid_East:
                            case Grid_West:
                                DrawRectangle(x_r, y_r, grid_d-1, grid_d-1, RED);
                                break;
                            case Grid_Empty:
                                DrawRectangle(x_r, y_r, grid_d-1, grid_d-1, BLUE);
                                break;
                            default:
                                break;
                            }
                    }
                 
                }
                
                break;
            }
            case Scene_Lost:
            {
                const char* title = "You Lost :(";
                const int t_size = 40;
                const int t_w=MeasureText(title, t_size);
                DrawText(title, (screenWidth - t_w) / 2, screenHeight/4, t_size, RED);
                break;
            }
            case Scene_Win:
            {
                const char* title = "You Won!!!";
                const int t_size = 40;
                const int t_w=MeasureText(title, t_size);
                DrawText(title, (screenWidth - t_w) / 2, screenHeight/4, t_size, GREEN);
                break;
            }
            default:
                break;
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
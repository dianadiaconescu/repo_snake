//ai ramas la minutul 30
#include "raylib.h"
#include <assert.h>
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
            assert(false);//This should never happen, as the snake should only move in one of the four directions
            break;
        
    }
    return new_p;
}
typedef enum{
    Scene_Menu=0,
    Scene_Game,
}Scene;

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

                        grid[snake_head.y][snake_head.x]=snake_direction;
                        grid[6][1]=Grid_Food;

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
                    if((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && snake_direction!=Grid_South)
                    {
                        ///Q:e bine sa exista "a collision with the self?"-aici nu exista ---> nu poate deveni ---<
                        snake_direction=Grid_North;
                    }
                    else if((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && snake_direction!=Grid_North)
                    {
                        snake_direction=Grid_South;
                    }
                    else if((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && snake_direction!=Grid_West)
                    {
                        snake_direction=Grid_East;
                    }
                    else if((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && snake_direction!=Grid_East)
                    {
                        snake_direction=Grid_West;
                    }
                    if(frame_count % 30 == 0)
                    {
                        grid[snake_head.y][snake_head.x]=snake_direction;//Update the grid with the new direction of the snake head
                        snake_head=point_plus_direction(snake_head, snake_direction);//Move the snake head in the direction it's moving
                        grid[snake_head.y][snake_head.x]=snake_direction;//Update the grid with the new position of the snake head
                    
                        //Grid_Type temp_dir=grid[snake_tail.y][snake_tail.x];//Get the direction of the snake tail, which is the direction of the next cell in the snake body
                        //grid[snake_tail.y][snake_tail.x]=Grid_Empty;//Clear the cell of the snake tail, as it will move forward
                        //snake_tail=point_plus_direction(snake_tail, temp_dir);//Move the snake tail in the direction of the next cell in the snake body
                    }
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
            default:
                break;
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
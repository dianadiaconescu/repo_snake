#include "raylib.h"

typedef enum{
    Scene_Menu=0,
    Scene_Game,
}Scene;
int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;

    Scene current_scene = Scene_Menu;

    InitWindow(screenWidth, screenHeight, "Snake");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        ////////////////////////////////////////////////////////////////////////
        ////Update the game
            switch(current_scene)
            {
                case Scene_Menu:
                {
                   if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ENTER))
                   {
                        current_scene = Scene_Game;
                   }
                    break;
                }
                case Scene_Game:
                {
                    if(IsKeyPressed(KEY_Q))
                    {
                        current_scene = Scene_Menu;
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
                DrawText("Menu Scene", 450, 450, 40, WHITE);
                
                break;
            }
            case Scene_Game:
            {
                DrawText(title, (screenWidth - t_w) / 2, screenHeight/3, t_size, WHITE);
                
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
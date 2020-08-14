#include "math.h"
#include "raylib.h"
#include "stdio.h"
#include "game.c"

// CONSTANTS
#define UPDATE_STEPS 4
const float dt = 1 / 60.0;



int main(void)
{

    InitWindow(WIDTH, HEIGHT, "Particulatoriburatiburatinus");

    SetTargetFPS(60);


    init();


    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        for (int i = 0; i < UPDATE_STEPS; i++)
        {
            update(dt);
        }


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            // DRAW LOOP
            draw();


        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}

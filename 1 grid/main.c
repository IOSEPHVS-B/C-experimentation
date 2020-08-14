#include <raylib.h>
#include <math.h>


// CONSTANTS
#define GAME_WIDTH  600
#define GAME_HEIGHT 400

#define TARGET_FPS 60
#define STEPS_PER_FRAME 2


// FUNCTIONS
void update(float dt);
void draw(void);


// VARIABLES
#define CELL_SIZE 20
#define GRID_W (GAME_WIDTH / CELL_SIZE)
#define GRID_H (GAME_HEIGHT / CELL_SIZE)
Color grid[GRID_W][GRID_H];

Vector2 currentCell;

float time = 0.5;


int main(void)
{
    // INITIALISATION
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "PARTICLEZ!!!");
    
    
    SetTargetFPS(TARGET_FPS);

    for (int i = 0; i < GRID_H; i++)
    {
        for (int j = 0; j < GRID_W; j++)
        {
            grid[j][i] = (Color){j * (255/GRID_W), i * (255/GRID_W), 127, 255};   
        }
    }
    

    // GAME LOOP
    while (!WindowShouldClose())
    {
        // UPDATE
        for (int i = 0; i < STEPS_PER_FRAME; i++)
        {
            update(1.0 / TARGET_FPS);
        }
        

        // DRAW
        BeginDrawing();
        
            draw();
            
        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}



void update(float dt)
{
    time = (fmod((time + dt/1), 2.0));
    float colTime = fabs(time - 1);

    for (int i = 0; i < GRID_H; i++)
    {
        for (int j = 0; j < GRID_W; j++)
        {
            grid[j][i].g = (int) (colTime * 255.0);   
        }
    }
    
    currentCell.x = (int) GetMousePosition().x / CELL_SIZE * CELL_SIZE;
    currentCell.y = (int) GetMousePosition().y / CELL_SIZE * CELL_SIZE;
}


void draw(void)
{
    ClearBackground(RAYWHITE);
    DrawFPS(8, 8);

    for (int i = 0; i < GRID_H; i++)
    {
        for (int j = 0; j < GRID_W; j++)
        {
            //DrawRectangleLines(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE, LIGHTGRAY);
            DrawRectangle(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE, grid[j][i]);
        }
        
    }
    DrawRectangle(currentCell.x, currentCell.y, CELL_SIZE, CELL_SIZE, LIGHTGRAY);
}
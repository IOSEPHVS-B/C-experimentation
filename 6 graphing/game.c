#include "math.h"
#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"

const int WIDTH  = 600;
const int HEIGHT = 600;

// FUNCTIONS
void init();
void update(float dt);
void draw();



// CONSTANTS
const float SCOPE = 10.0;
const int PRECISION = 3000;


// POINTS
typedef struct Vec2 {
    double x; 
    double y;
} Vec2;
// ARRAY OF THE POINTS (MALLOCED)
Vec2 *points;
#define POINT_NUM (PRECISION)
double step;


// THE ACTUAL FUNCTION
void initPoints();
double function(double x);



float anim = 0.0;




void init()
{
    // ALLOCATE POINTS
    points = malloc( POINT_NUM * sizeof(Vec2) );
    step = SCOPE / (PRECISION / 2.0);
    initPoints();
}



void update(float dt)
{
    if (!IsKeyDown(KEY_SPACE)) return;
    anim += dt;
    initPoints();
}



void draw()
{
    ClearBackground(BLACK);
    
    // Draw the grid
    for (int i = -SCOPE; i < SCOPE; i++)
    {
        float y = HEIGHT / (SCOPE * 2.0) * i + HEIGHT/2.0;
        DrawLine(0.0, y, WIDTH, y, DARKGREEN);

        float x = WIDTH / (SCOPE * 2.0) * i + WIDTH/2.0;
        DrawLine(x, 0.0, x, HEIGHT, DARKGREEN);
    }

    // Draw axes, first X, then Y
    DrawLineEx((Vector2){0.0, HEIGHT/2.0}, (Vector2){WIDTH, HEIGHT/2.0}, 2.0, LIME);
    DrawLineEx((Vector2){WIDTH/2.0, 0.0}, (Vector2){WIDTH/2.0, HEIGHT}, 2.0, LIME);
    
    // Draw the points
    for (int i = 1; i < POINT_NUM; i++)
    {
        Vec2 p1 = points[i];
        Vec2 p2 = points[i-1];
        DrawLine(
            p1.x * (WIDTH  / (SCOPE*2)) + WIDTH /2.0, 
            p1.y * (HEIGHT / (SCOPE*2)) + HEIGHT/2.0, 
            p2.x * (WIDTH  / (SCOPE*2)) + WIDTH /2.0, 
            p2.y * (HEIGHT / (SCOPE*2)) + HEIGHT/2.0, 
            GREEN
        );
    }
    
    
}



void initPoints()
{
    // Starts at -SCOPE, ends at SCOPE
    for (int i = 0; i < POINT_NUM; i++)
    {
        double x = i * step - SCOPE;
        double y = function(x) * -1;
        points[i] = (Vec2){x, y};
        //printf("\nx:%f, y:%f", x, y);
    }
}



double function(double x)
{
    return pow(x, anim);
}

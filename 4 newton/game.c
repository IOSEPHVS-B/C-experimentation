#include "math.h"
#include "raylib.h"
#include "stdio.h"

const int WIDTH  = 600;
const int HEIGHT = 600;

// FUNCTIONS
void init();
void update(float dt);
void draw();


// COLORS
Color blue  = {62, 77, 180, 255};
Color white = {236, 221, 190, 255};


// CONSTANTS
const double G = 5.0;
const float BODY_SCALE = 0.1;


// STRUCTS
typedef struct vec2
{
    double x;
    double y;
} vec2;

typedef struct Body
{
    vec2 pos;
    vec2 vel;
    vec2 acc;
    double m;
} Body;


// ARRAY OF BODIES
#define MAX_BODIES 16
Body bodies[MAX_BODIES];
int bodiesN = 0;


// PHYS FUNCTIONS
vec2 getAcceleration(Body b1, Body b2);
double getDistance(vec2 p1, vec2 p2);




void init()
{
    for (int i = 0; i < 2; i++)
    {
        bodies[i] = (Body){
            (vec2){GetRandomValue(100, 500), GetRandomValue(100, 500)},
            (vec2){
                GetRandomValue(-600, 600) / 1200.0,
                GetRandomValue(-600, 600) / 1200.0
            },
            (vec2){0.0, 0.0},
            GetRandomValue(50, 500)/2.0
        };
        bodiesN++;
    }
    // bodiesN = 2;
    // bodies[0] = (Body){
    //     (vec2) {200.0, 200.0},
    //     (vec2) {0.2, -0.2},
    //     (vec2) {0.0, 0.0},
    //     300.0
    // };
    // bodies[1] = (Body){
    //     (vec2) {300.0, 300.0},
    //     (vec2) {-0.2, 0.2},
    //     (vec2) {0.0, 0.0},
    //     900.0
    // };

}



void update(float dt)
{
    // FORCE
    for (int i = 0; i < bodiesN; i++)
    {
        Body *b = &bodies[i];
        b->acc.x = 0.0;
        b->acc.y = 0.0;

        for (int j = 0; j < bodiesN; j++)
        {
            if (i == j) continue;
            if (getDistance(b->pos, bodies[j].pos) < 5.0) continue;
            vec2 acc = getAcceleration(*b, bodies[j]);
            b->acc.x += acc.x;
            b->acc.y += acc.y;
        }
    }

    // MOVE
    for (int i = 0; i < bodiesN; i++)
    {
        Body *b = &bodies[i];
        
        b->vel.x += b->acc.x * dt;
        b->vel.y += b->acc.y * dt;

        b->pos.x += b->vel.x;
        b->pos.y += b->vel.y;

        b->pos.x = fmod(3 * WIDTH  + b->pos.x, WIDTH);
        b->pos.y = fmod(3 * HEIGHT + b->pos.y, HEIGHT);
    }
    
}



void draw()
{
    ClearBackground(blue);
    for (int i = 0; i < bodiesN; i++)
    {
        Body b = bodies[i];
        DrawCircleLines(b.pos.x, b.pos.y, b.m * BODY_SCALE, white);
    }
    //DrawFPS(16, 16);
}




vec2 getAcceleration(Body b1, Body b2)
{
    double dist = getDistance(b1.pos, b2.pos);
    //printf("\n%f", dist);
    vec2 distVector = (vec2){ b2.pos.x - b1.pos.x, b2.pos.y - b1.pos.y };
    vec2 distUnitVector = (vec2){ distVector.x / dist, distVector.y / dist };

    //double acc = (G * pow(b2.m, 2) * b1.m) / pow(dist, 2);
    double acc = (G * b2.m) / (dist * dist);
    vec2 accVector = (vec2){ distUnitVector.x * acc, distUnitVector.y * acc };
    
    return (accVector);
}


double getDistance(vec2 p1, vec2 p2)
{
    return sqrt( pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) );
}

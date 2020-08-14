#include "math.h"
#include "raylib.h"
#include "vectors.h"

float sign(float x) {
    return (x > 0.0) - (x < 0.0);
}


// CONSTANTS
const int screenWidth = 800;
const int screenHeight = 640;


// PARTICLE STUFF

#define P_NUM 256
int activeParticles = 0;

typedef enum {
    P_RED = 0,
    P_BLU = 1,
    P_YLW = 2
} P_COLS;

Color P_RENDER_COLORS[] = {RED, BLUE, ORANGE};

float relationships[P_NUM][P_NUM][4] = {
    // RED
    {
        {100, 0, 8, 1},// RED
        {47, 50, 16, 200},// BLU
        {55, 23, 6, 1}// YLW
    },
    // BLU
    {
        {47, 50, 16, 200},// RED
        {100, 0, 8, 1},// BLU
        {140, 6, 6, 1}// YLW
    },
    // YLW
    {
        {55, 23, 6, 1},// RED
        {140, 6, 6, 1},// BLU
        {65, 6, 6, 1}// YLW
    },
};

typedef struct
{
    Vector2 pos;
    Vector2 vel;
    P_COLS col;
} Particle;


// FUNCTIONS
Particle newParticle(float x, float y, float vx, float vy, P_COLS col);
void updateParticleVel(Particle *p1, Particle *p2, float dt);
void updateParticle(Particle *p, float dt);
float forceFunction(float x, float a, float b, float c, float d);




int main(void)
{

    InitWindow(screenWidth, screenHeight, "Particulatoriburatiburatinus");

    SetTargetFPS(120);

    // Load
    //----------------------------------------------------------------------------------
    Particle particles[P_NUM];
    /*
    for (int i = 0; i < P_NUM / 2; i++)
    {
        particles[i] = newParticle(
            GetRandomValue(0, screenWidth), 
            GetRandomValue(0, screenHeight),
            0.0, 0.0, P_BLU
        );
        activeParticles++;
    }
    */
    /*
    particles[0] = newParticle(screenWidth/2, screenHeight/2 + 64, 0.1, 0.0);
    particles[1] = newParticle(screenWidth/2, screenHeight/2 - 64, -0.1, 0.0);
    */
    

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        float dt = 1/120.0;

        if (!IsKeyDown(KEY_SPACE)) dt = 0.0;

        for (int i = 0; i < 3; i++)
        {
            if (IsMouseButtonPressed(i))
            {
                if (activeParticles < P_NUM)
                {
                    particles[activeParticles] = newParticle(GetMouseX(), GetMouseY(), 0.0, 0.0, i);
                    activeParticles++;
                }
                
            }
        }
        

        for (int i = 0; i < activeParticles; i++)
        {
            for (int j = 0; j < activeParticles; j++)
            {
                if (i == j) continue;
                updateParticleVel(&particles[i], &particles[j], dt);
                updateParticle(&particles[i], dt);
            }
            
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(WHITE);

            for (int i = 0; i < activeParticles; i++)
            {
                Particle p = particles[i];
                DrawCircleV(p.pos, 2, P_RENDER_COLORS[p.col]);
            }

            DrawFPS(16, 16);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}


Particle newParticle(float x, float y, float vx, float vy, P_COLS col)
{
    return (Particle){
        (Vector2){x, y},
        (Vector2){vx, vy},
        col
    };
}


void updateParticleVel(Particle *p1, Particle *p2, float dt)
{
    float dx = p1->pos.x - p2->pos.x;
    float dy = p1->pos.y - p2->pos.y;
    float d  = sqrt( pow(dx, 2) + pow(dy, 2) );
    Vector2 unitV = (Vector2){dx / d, dy / d};

    float force = forceFunction(
        d, 
        relationships[p1->col][p2->col][0], 
        relationships[p1->col][p2->col][1], 
        relationships[p1->col][p2->col][2],
        relationships[p1->col][p2->col][3]
    );
    
    p1->vel.x += unitV.x * -force * dt;
    p1->vel.y += unitV.y * -force * dt;
}


void updateParticle(Particle *p, float dt)
{
    p->vel.x *= 0.999;
    p->vel.y *= 0.999;
    p->pos.x += p->vel.x * dt * 0.1;
    p->pos.y += p->vel.y * dt * 0.1;

    p->pos.x = fmod(p->pos.x + screenWidth, screenWidth);
    p->pos.y = fmod(p->pos.y + screenHeight, screenHeight);
}


float forceFunction(float x, float a, float b, float c, float d)
{
    //float a = 200;
    //float b = 9;
    //float c = 8;

    float attractive = b / pow(x / (2 * a) + sign(x), c); 
    float repulsive = -1 / pow( (x + 35 * sign(x)) / a, c);
    float force = d * (attractive + repulsive);
    return force;
}
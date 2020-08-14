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
Color blue  = { 43,  15,  84, 255};
Color white = {255, 218,  69, 255};


// 3D STUFF
Camera3D camera = {};
float cameraRotation = 0.0;
float cameraDistance = 45.0;
float cameraRotationSpeed = 0.0;


// CONSTANTS
const double G = 0.3;


// STRUCTS
typedef struct {
    double x;
    double y;
    double z;
} Vec3;

typedef struct Body {
    Vec3 pos;
    Vec3 vel;
    Vec3 acc;
    double mass;
    float radius;
    Color col;
} Body;


// BODIES
#define MAX_BODIES 16
Body bodies[MAX_BODIES];
int bodiesN = 0;


// FUNCTIONS
double getDistance(Vec3 p1, Vec3 p2);
void drawConnection(Vec3 p1, Vec3 p2);
Vec3 getForce(Body b1, Body b2);
Vec3 forceToAcc(Vec3 force, double mass);




void init()
{
    // INIT CAMERA
    camera.position = (Vector3){0.0, 10.0, 10.0};
    camera.target   = (Vector3){0.0,  0.0,  0.0};
    camera.up       = (Vector3){0.0,  1.0,  0.0};
    camera.fovy     = 45.0;
    camera.type     = CAMERA_PERSPECTIVE;

    // INIT BODIES
    // Color colors[] = {
    //     BLUE, RED, YELLOW, GREEN, VIOLET, GOLD, PINK, BEIGE
    // };
    // for (int i = 0; i < 4; i++)
    // {
    //     Body b = {
    //         (Vec3){
    //             GetRandomValue(-100, 100) / 10.0,
    //             GetRandomValue(-100, 100) / 10.0,
    //             GetRandomValue(-100, 100) / 10.0
    //         },
    //         (Vec3){
    //             GetRandomValue(-100, 100) / 100.0,
    //             GetRandomValue(-100, 100) / 100.0,
    //             GetRandomValue(-100, 100) / 100.0
    //         },
    //         (Vec3){0.0, 0.0, 0.0},
    //         32.0, 10.0,
    //         colors[GetRandomValue(0, sizeof(colors)/sizeof(Color) - 1)]
    //     };
    //     bodies[i] = b;
    //     bodiesN++;
    // }
    bodiesN = 2;
    bodies[0] = (Body){
        (Vec3){ // POS
            0.0, 0.0, 0.0
        },
        (Vec3){ // VEL
            -0.2, 0.0, 0.0
        },
        (Vec3){0.0, 0.0, 0.0},
        200.0, 20.0, GOLD
    };
    bodies[1] = (Body){
        (Vec3){ // POS
            6.0, 0.0, 0.0
        },
        (Vec3){ // VEL
            2.0, 1.0, 2.5
        },
        (Vec3){0.0, 0.0, 0.0},
        10.0, 10.0, BLUE
    };
    
}



void update(float dt)
{
    if (!IsKeyDown(KEY_SPACE))
    {
        return;
    }
    
    // ROTATE CAMERA
    cameraRotation += dt * cameraRotationSpeed;
    camera.position.x = sin(cameraRotation) * cameraDistance;
    camera.position.z = cos(cameraRotation) * cameraDistance;

    // LOOP THROUGH ALL BODIES
    for (int i = 0; i < bodiesN; i++)
    {
        Body *b1 = &bodies[i];
        Vec3 totalForce = {0.0, 0.0, 0.0};

        for (int j = 0; j < bodiesN; j++)
        {
            if (j == i) continue;

            Body b2 = bodies[j];

            Vec3 force = getForce(*b1, b2);
            totalForce.x += force.x;
            totalForce.y += force.y;
            totalForce.z += force.z;
        }
        Vec3 accVec = forceToAcc(totalForce, b1->mass);
        b1->acc = accVec;

        b1->vel.x += b1->acc.x * dt;
        b1->vel.y += b1->acc.y * dt;
        b1->vel.z += b1->acc.z * dt;

        b1->pos.x += b1->vel.x * dt;
        b1->pos.y += b1->vel.y * dt;
        b1->pos.z += b1->vel.z * dt;
    }


    // CAMERA LOOKAT
    Vec3 lookat = {0.0, 0.0, 0.0};

    for (int i = 0; i < bodiesN; i++)
    {
        lookat.x += bodies[i].pos.x;
        lookat.y += bodies[i].pos.y;
        lookat.z += bodies[i].pos.z;
    }
    camera.target.x = lookat.x /= bodiesN;
    camera.target.y = lookat.y /= bodiesN;
    camera.target.z = lookat.z /= bodiesN;
    
}



void draw()
{
    ClearBackground(blue);

    BeginMode3D(camera);


        // DRAW SPHERES
        for (int i = 0; i < bodiesN; i++)
        {
            Body b = bodies[i];
            
            DrawSphereWires(
                (Vector3){
                    b.pos.x,
                    b.pos.y,
                    b.pos.z
                }, 
                b.radius / 10.0, 6, 6, b.col
            );
        }

        // LOOP THROUGH ALL BODIES
        for (int i = 0; i < bodiesN; i++)
        {
            Body b1 = bodies[i];

            for (int j = 0; j < bodiesN; j++)
            {
                if (j == i) continue;
                Body b2 = bodies[j];
                drawConnection(b1.pos, b2.pos);
            }
            
        }

        DrawGrid(10, 2.0);
        

    EndMode3D();
}







double getDistance(Vec3 p1, Vec3 p2)
{
    return sqrt(  pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2)  );
}


Vec3 getForce(Body b1, Body b2)
{
    double dist = getDistance(b1.pos, b2.pos);

    if (dist == 0.0) return (Vec3){0.0, 0.0, 0.0}; // Return 0 vector if dist is 0
    
    Vec3 distUnitVec = {
        (b2.pos.x - b1.pos.x) / dist,
        (b2.pos.y - b1.pos.y) / dist,
        (b2.pos.z - b1.pos.z) / dist
    };

    double force = ( G * b1.mass * b2.mass ) / (dist * dist);
    Vec3 forceVector = {
        distUnitVec.x * force,
        distUnitVec.y * force,
        distUnitVec.z * force
    };

    return forceVector;
}



Vec3 forceToAcc(Vec3 force, double mass)
{
    double forceMagnitude = sqrt( pow(force.x, 2) + pow(force.y, 2) + pow(force.z, 2) );

    Vec3 forceUnitVector = {
        force.x / forceMagnitude,
        force.y / forceMagnitude,
        force.z / forceMagnitude
    };

    double acceleration = forceMagnitude / mass;

    Vec3 accelerationVector = {
        forceUnitVector.x * acceleration,
        forceUnitVector.y * acceleration,
        forceUnitVector.z * acceleration
    };
    return accelerationVector;
}









void drawConnection(Vec3 p1, Vec3 p2)
{
    static Color col = { 255, 255, 255, 32 };

    double dist = getDistance(p1, p2);
    col.a = (255.0 / (dist*2)) * 3.0;

    Vec3 distVector = (Vec3){ p2.x - p1.x, p2.y - p1.y, p2.z - p1.z };
    Vec3 distUnitVector = (Vec3){ distVector.x/dist, distVector.y/dist, distVector.z/dist };

    Vector3 point1 = { p1.x, p1.y, p1.z };
    Vector3 point2 = { p1.x + distUnitVector.x * dist, p1.y + distUnitVector.y * dist, p1.z + distUnitVector.z * dist };
    
    // point2.x *= dist;
    // point2.y *= dist;
    // point2.z *= dist;
    DrawLine3D(point1, point2, col);
}
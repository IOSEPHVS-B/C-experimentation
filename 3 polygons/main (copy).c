#include "math.h"
#include "raylib.h"
#include "stdio.h"

// CONSTANTS
const int screenWidth = 600;
const int screenHeight = 600;


#define POL_N 4
Vector2 polygon[POL_N] = {
    {3, 2},
    {2, 5},
    {5, 7},
    {9, 5}//,
    //{9, 3}
};


Vector2 point = {0.0, 0.0};
Color   pointColour = GREEN;


int isPointInsidePolygon();
int isPointInsideAngle(Vector2 A, Vector2 B, Vector2 C, Vector2 X);
double getAngle(Vector2 A, Vector2 B, Vector2 C);
double getDistance(Vector2 A, Vector2 B);



int main(void)
{

    InitWindow(screenWidth, screenHeight, "Particulatoriburatiburatinus");

    SetTargetFPS(30);




    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------

        point = (Vector2){
            GetMouseX()/60.0,
            GetMouseY()/60.0
        };

        pointColour = isPointInsidePolygon() ? BLUE : RED;


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(WHITE);

            // DRAW THE POLYGON
            for (int i = 0; i < POL_N; i++)
            {
                Vector2 startP = polygon[i];
                Vector2 endP   = polygon[(i+1)%POL_N];
                DrawLineEx(
                    (Vector2) {startP.x * 60.0, startP.y * 60.0}, 
                    (Vector2) {endP.x * 60.0, endP.y * 60.0}, 
                    3.0, 
                    BLUE
                );
            }

            // DRAW THE MOUSE POINT
            DrawCircle(
                point.x * 60.0,
                point.y * 60.0,
                4.0, pointColour
            );
            

            DrawFPS(16, 16);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}



int isPointInsidePolygon()
{
    // Check if point is within all angles
    for (int i = 0; i < POL_N; i+=1)
    {
        // Get all the points
        Vector2 A = polygon[ i ];
        Vector2 B = polygon[ (i + POL_N-1) % (POL_N) ];
        Vector2 C = polygon[ (i+1) % POL_N ];
        Vector2 X = point;
        //printf("i: %d, res: %d\n", i, isPointInsideAngle(A, B, C, X));
        if (!isPointInsideAngle(A, B, C, X))
        {
            //printf("%d\n", i);
            return 0;
        }
    }

    return 1;
}


int isPointInsideAngle(Vector2 A, Vector2 B, Vector2 C, Vector2 X)
{
    // Get all the angles
    double BAX = fabs(getAngle(B, A, X));
    double XAC = fabs(getAngle(X, A, C));
    double BAC = fabs(getAngle(B, A, C));

    // If shit doesnt add up, then return 0
    if ( (BAX + XAC) - BAC < 0.01 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


// double getAngle(Vector2 A, Vector2 B, Vector2 C)
// {
//     double AB = getDistance(A, B);
//     double BC = getDistance(B, C);
//     //double CB = getDistance(C, B);

//     double h = (AB * BC) / BC;
//     double angle = asin( h / AB );
//     return angle;
// }

double getAngle(Vector2 A, Vector2 B, Vector2 C) {

    double atanA = atan2(A.x - B.x, A.y - B.y);
    double atanC = atan2(C.x - B.x, C.y - B.y);
    double diff = atanC - atanA;

    //if (diff > PI) diff -= PI;
    //else if (diff < -PI) diff += PI;
    if (diff > PI)
        diff = 2*PI - diff;

    return diff;
} 


double getDistance(Vector2 A, Vector2 B)
{
    return sqrt( pow(B.x - A.x, 2) + pow(B.y - A.y, 2) );
}
#include "../include/MasterHeader.h"

// functional programming attempt
// Matrix wallPlane_a = MatrixIdentity();
// Color yoWHITE = {255,255,255,255};
// Vector3 wallPlane_aV;
// float wave_y = 0.0f;

class panel;

vector<panel> panelList; 

class panel
{
    public:
    Matrix  myPlane = MatrixIdentity(); // matrix of draw context
    Color   myColor = {255,255,255,255};
    Vector3 myPosition; // passed position from invoker
    Vector3 planesPos; // actual plane position within matrix
    bool    wasHit = false;
    bool    animActive = false;
    float   amplitude = 2.0f;
    float wave_y = 0.0f;

    panel(Vector3 passedPosition) 
    : myPosition(passedPosition)
    {
        myPlane.m12 = myPosition.x;
        myPlane.m13 = myPosition.y;
        myPlane.m14 = myPosition.z;
        planesPos.x = 0.0f;
        planesPos.y = 0.0f;
        planesPos.z = 0.0f;
    }
    
    void draw()
    {
        rlPushMatrix();

        DrawSphereWires(myPosition, 2.0f, 8, 8, GREEN);

        rlRotatef(90, 1.0f, 0.0f, 0.0f);

        rlMultMatrixf(MatrixToFloat(myPlane));

        DrawPlane(planesPos, {4.0f,4.0f}, myColor);

        detectCollisions();

        hitAnimation();

        rlPopMatrix();
    }

    void detectCollisions()
    {
        for (auto& laser : lasersList)
        {
            float distance = std::sqrt
            (
                (laser.currentPos.x - myPosition.x) * (laser.currentPos.x - myPosition.x) +
                (laser.currentPos.y - myPosition.y) * (laser.currentPos.y - myPosition.y) +
                (laser.currentPos.z - myPosition.z) * (laser.currentPos.z - myPosition.z)
            );
            
            if (laser.currentPos.z < -19.0f && laser.currentPos.z > -21.0f) // this is a slice of  3d world space
            {
                if (distance < 2.0f)
                {
                    myColor = {255,0,0,255}; // change red
                    wasHit = true;
                }
            }
        }
    }

    void hitAnimation()
    {
        if (wasHit)
        {
            // wave_y was declared in the class as (float wave_y = 0.0f;)
            wave_y -= 0.10f;
            planesPos.y = amplitude * sinf(wave_y);

            // tune the animation down and reset all inputs
            amplitude -= 0.01f;
            if (amplitude <= 0.0f)
            {
                wasHit = false;
                wave_y = 0.0f;
                myColor = {255,225,225,255};
                amplitude = 2.0f;
            }
        }
    }

};

//planesPos.y -= 1.0;


// // basic plane
// void newPlane()
// {
//     rlPushMatrix();

//     // move to a new x y z
//     wallPlane_a.m12 = 20.0f;
//     wallPlane_a.m13 = 0.0f;
//     wallPlane_a.m14 = -20.0f;
//     wallPlane_aV.x  = wallPlane_a.m12;
//     wallPlane_aV.y  = wallPlane_a.m13;
//     wallPlane_aV.z  = wallPlane_a.m14;
//     DrawSphereWires(wallPlane_aV, 2.0f, 8, 8, GREEN);

//     // make flat on the y plane rotate to y plane
//     rlRotatef(90, 1.0f, 0.0f, 0.0f);

//     // apply wallPlane to current matrix
//     rlMultMatrixf(MatrixToFloat(wallPlane_a));

//     // draw plane within plane
//     //DrawGrid(10, 1.0f);
//     DrawPlane({0.0f, 0.0f, 0.0f}, {4.0f,4.0f}, yoWHITE);

//     // detect laser hit
//     for (auto& laser : lasersList)
//     {

//         float distance = std::sqrt
//         (
//             (laser.currentPos.x - wallPlane_aV.x) * (laser.currentPos.x - wallPlane_aV.x) + 
//             (laser.currentPos.y - wallPlane_aV.y) * (laser.currentPos.y - wallPlane_aV.y) + 
//             (laser.currentPos.z - wallPlane_aV.z) * (laser.currentPos.z - wallPlane_aV.z)
//         );

//         if (laser.currentPos.z < -20.0f && distance < 2.0f)
//         {
//             yoWHITE = {255,0,0,255};
//         }
//     }

//     rlPopMatrix();
// }









// A point in space like any other. Plus x and plus y
// void DrawPlane(Vector3 centerPos, Vector2 size, Color color);                                      // Draw a plane XZ
//vector<lasers> lasersList;
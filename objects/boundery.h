#include "../include/MasterHeader.h"

Matrix wallPlane_a =  MatrixIdentity();
Color yoWHITE = {255,255,255,255};
Vector3 wallPlane_aV;

void newPlane()
{
    rlPushMatrix();

    // move to a new x y z
    wallPlane_a.m12 = 20.0f;
    wallPlane_a.m13 = 0.0f;
    wallPlane_a.m14 = -20.0f;
    wallPlane_aV.x  = wallPlane_a.m12;
    wallPlane_aV.y  = wallPlane_a.m13;
    wallPlane_aV.z  = wallPlane_a.m14;
    DrawSphereWires(wallPlane_aV, 2.0f, 8, 8, GREEN);

    // make flat on the y plane rotate to y plane
    rlRotatef(90, 1.0f, 0.0f, 0.0f);

    // apply wallPlane to current matrix
    rlMultMatrixf(MatrixToFloat(wallPlane_a));

    // draw plane within plane
    //DrawGrid(10, 1.0f);
    DrawPlane({0.0f, 0.0f, 0.0f}, {4.0f,4.0f}, yoWHITE);

    // detect laser hit
    for (auto& laser : lasersList)
    {

        float distance = std::sqrt
        (
            (laser.currentPos.x - wallPlane_aV.x) * (laser.currentPos.x - wallPlane_aV.x) + 
            (laser.currentPos.y - wallPlane_aV.y) * (laser.currentPos.y - wallPlane_aV.y) + 
            (laser.currentPos.z - wallPlane_aV.z) * (laser.currentPos.z - wallPlane_aV.z)
        );

        if (laser.currentPos.z < -20.0f && distance < 2.0f)
        {
            yoWHITE = {255,0,0,255};
        }
    }


    rlPopMatrix();
}



// A point in space like any other. Plus x and plus y
// void DrawPlane(Vector3 centerPos, Vector2 size, Color color);                                      // Draw a plane XZ
//vector<lasers> lasersList;
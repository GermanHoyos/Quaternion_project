////////////////////////////////////////////////////////////////////////////////////
// OPENGL 4X4 IDENTITY MATRIX
// | 1  0  0  0 |  ->  | m0 m4 m8 m12 |   // m0, m1, m2: First column, representing the x-axis direction vector.
// | 0  1  0  0 |  ->  | m1 m5 m9 m13 |   // m4, m5, m6: Second column, representing the y-axis direction vector.
// | 0  0  1  0 |  ->  | m2 m6 m10 m14|   // m8, m9, m10: Third column, representing the z-axis direction vector.
// | 0  0  0  1 |  ->  | m3 m7 m11 m15|   // m12, m13, m14: Translation components along the x, y, and z axes, respectively.
////////////////////////////////////////////////////////////////////////////////////
// OPENGL + RAYLIB "MATRIX" STRUCT
// | 1  0  0  10 |  ->  | m0  m4  m8  m12 |    // [12] = x-axis
// | 0  1  0  0  |  ->  | m1  m5  m9  m13 |    // [13] = y-axis
// | 0  0  1  0  |  ->  | m2  m6  m10 m14 |    // [14] = z-axis
// | 0  0  0  1  |  ->  | m3  m7  m11 m15 |    // [15] = always 1, required for matrix multiplication
////////////////////////////////////////////////////////////////////////////////////
// AXIS IN RELATION TO RAYLIB DEFAULT CAMERA POSITION
// [13][Y] = (+)up     (-)down
// [12][X] = (+)right  (-)left
// [14][Z] = (-)foward (+)back
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../include/MasterHeader.h"

void makeAsteroids()
{
    // DrawSphere((Vector3){0.0f, 0.0f, 0.0f}, 1.0f, GREEN);
    // DrawSphereWires((Vector3){0.0f, 0.0f, 0.0f}, 2.0f, 6, 6, LIME);
};

class asteroids 
{
    public:
    Vector3 myPosition;
    Color   myRGB = { 0, 228, 48, 255 };
    Color   myLime = { 0, 158, 47, 255 };
    float   sphereRad = 1.0f;
    float   wiresRad = 2.0f;
    bool    hitAnim = false;
    int     hitTime;

    asteroids(Vector3 position) 
    : myPosition(position)
    {
        // some init code
    }

    void draw()
    {
        DrawSphere((Vector3){myPosition.x, myPosition.y, myPosition.z}, sphereRad, myRGB);
        DrawSphereWires((Vector3){myPosition.x, myPosition.y, myPosition.z}, wiresRad, 6, 6, myLime);
        if (hitAnim == true)
        {
            hitAnimation();
        }
    }

    void detectCollisions(lasers& laser)
    {
        // Get distance between laser and asteroid
        // if distance is less than radius of asteroid, then
        // change color
        // Calculate distance between laser and asteroid
        float distance = std::sqrt(
            //(x2 - x1)^2 = x axis distance
            (laser.currentPos.x - myPosition.x) * (laser.currentPos.x - myPosition.x) +
            //(y2 - y1)^2 = y axis distance
            (laser.currentPos.y - myPosition.y) * (laser.currentPos.y - myPosition.y) +
            //(z2 - z1)^2 = z axis distance
            (laser.currentPos.z - myPosition.z) * (laser.currentPos.z - myPosition.z)
        );

        // Check if distance is less than radius of asteroid (assuming radius = 1.0f)
        if (distance < 2.0f)
        {
            hitAnimationDetect();
        }
    }

    void hitAnimationDetect()
    {
        // Develop a timer that increases radius for X amount of time 
        if (hitAnim == false)
        {
            hitTime = GetTime();
            hitAnim = true;
        } 
    }

    void hitAnimation()
    {   
        int latestTime = GetTime();

        // If this conditional returns true, then the scopes animation will run
        if(latestTime < hitTime + 1)
        {
            myRGB = { 255, 255, 255, 255 };
            sphereRad += 0.01;
        } 
        if (latestTime >= hitTime + 1) 
        {
            hitAnim = false;
        }
    }


};
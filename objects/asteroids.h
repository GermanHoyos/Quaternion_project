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
    Vector3 myPositionSp;
    float xShake;
    float yShake;
    float zShake;
    Vector3 myPositionWr;
    int     myAlpha = 255;
    Color   myRGB = { 0, 228, 48, static_cast<unsigned char>(myAlpha) };
    Color   myLime = { 0, 158, 47, static_cast<unsigned char>(myAlpha) };
    float   sphereRad = 1.0f;
    float   wiresRad = 2.0f;
    bool    hitAnim = false;
    int     hitTime;

    asteroids(Vector3 position) 
    : myPositionSp(position)
    {
        myPositionWr = myPositionSp;
    }

    void draw()
    {
        DrawSphere((Vector3){myPositionSp.x, myPositionSp.y, myPositionSp.z}, sphereRad, myRGB);
        DrawSphereWires((Vector3){myPositionWr.x, myPositionWr.y, myPositionWr.z}, wiresRad, 6, 6, DARKGREEN);
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
            (laser.currentPos.x - myPositionSp.x) * (laser.currentPos.x - myPositionSp.x) +
            //(y2 - y1)^2 = y axis distance
            (laser.currentPos.y - myPositionSp.y) * (laser.currentPos.y - myPositionSp.y) +
            //(z2 - z1)^2 = z axis distance
            (laser.currentPos.z - myPositionSp.z) * (laser.currentPos.z - myPositionSp.z)
        );

        // Check if distance is less than radius of asteroid (assuming radius = 1.0f)
        if (distance < 2.0f && hitAnim == false)
        {
            hitAnimationDetect();
        }
    }

    void hitAnimationDetect()
    {
        // Develop a timer that increases radius for X amount of time 
        if (!hitAnim)
        {
            hitTime = GetTime();
            hitAnim = true;
        } 
    }

    void hitAnimation()
    {   
        int latestTime = GetTime();

        if (hitAnim)
        {
            // If this conditional returns true, then this scopes animation will run
            if(latestTime <= hitTime + 2)
            {
                // Animate hit here:
                //if (sphereRad < 1.60f) sphereRad += 0.05f;
                myRGB = { 255, 155, 155, static_cast<unsigned char>(myAlpha) };

                xShake = 0.0f;
                yShake = 0.0f;
                zShake = 0.0f;
                myPositionSp.x = myPositionWr.x;
                myPositionSp.y = myPositionWr.y;
                myPositionSp.z = myPositionWr.z;

                // RANDOM NUMS
                ///*sphere color*/ random_device rd_1; mt19937 gen_1(rd_1()); uniform_int_distribution<int>    dis_1(1, 255);      cG = dis_1(gen_1); cB = dis_1(gen_1); cAlpha = dis_1(gen_1);
                random_device rd_2; mt19937 gen_2(rd_2()); uniform_real_distribution<float> dis_2(-0.6f, 0.6f); xShake = xShake + dis_2(gen_2);//thrus1 = thrus1 + dis_2(gen_2);
                random_device rd_3; mt19937 gen_3(rd_3()); uniform_real_distribution<float> dis_3(-0.6f, 0.6f); yShake = yShake + dis_3(gen_3);//thrus2 = thrus2 + dis_3(gen_2);
                random_device rd_4; mt19937 gen_4(rd_4()); uniform_real_distribution<float> dis_4(-0.6f, 0.6f); zShake = zShake + dis_4(gen_4);

                myPositionSp.x = myPositionSp.x + xShake;
                myPositionSp.y = myPositionSp.y + yShake;
                myPositionSp.z = myPositionSp.z + zShake;
                
            }

            if (latestTime >= hitTime + 2) 
            {
                sphereRad = 1.0f;
                myAlpha = 255;
                myRGB = { 0, 255, 0, static_cast<unsigned char>(myAlpha) };
                hitAnim = false;
                myPositionSp.x = myPositionWr.x;
                myPositionSp.y = myPositionWr.y;
                myPositionSp.z = myPositionWr.z;
                xShake = 0.0f;
                yShake = 0.0f;
                zShake = 0.0f;
            }
        }
    }

};
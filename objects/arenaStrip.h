#include "../include/MasterHeader.h"

float zPositionBlueLine = 800.0f;
float troidX;
float troidY;


void drawStrip()
{
    // Draw the lines
    DrawLine3D({80.0f, 0.0f, -20.0f}, {80.0f, 0.0f, 800.0f}, WHITE);
    DrawLine3D({180.0f, 0.0f, -20.0f}, {180.0f, 0.0f, 800.0f}, WHITE);

    DrawLine3D({80.0f, 0.0f, -20.0f}, {80.0f, 40.0f, -20.0f}, WHITE);
    DrawLine3D({180.0f, 0.0f, -20.0f}, {180.0f, 40.0f, -20.0f}, WHITE);

    DrawLine3D({80.0f, 40.0f, -20.0f}, {80.0f, 40.0f, 800.0f}, WHITE);
    DrawLine3D({180.0f, 40.0f, -20.0f}, {180.0f, 40.0f, 800.0f}, WHITE);

    DrawLine3D({80.0f, 0.0f, 800.0f}, {80.0f, 40.0f, 800.0f}, WHITE);
    DrawLine3D({180.0f, 0.0f, 800.0f}, {180.0f, 40.0f, 800.0f}, WHITE);

    //DrawLine3D({80.0f, 0.0f, -20.0f}, {180.0f, 0.0f, -20.0f}, WHITE);
    //DrawLine3D({80.0f, 0.0f, 800.0f}, {180.0f, 0.0f, 800.0f}, WHITE);

    // Draw the interpolated line
    //DrawLine3D({80.0f, 0.0f, zPosition}, {180.0f, 0.0f, zPosition}, WHITE);
    DrawLine3D({80.0f, 0.0f, zPositionBlueLine}, {80.0f, 40.0f, zPositionBlueLine}, BLUE);
    DrawLine3D({180.0f, 0.0f,zPositionBlueLine}, {180.0f, 40.0f, zPositionBlueLine}, BLUE);

    // Check if zPosition reached the destination, then reset it
    zPositionBlueLine -= 4.0f;
    if (zPositionBlueLine <= -20.0f) 
    {

        // if the blue line is reset then push an asteroid to the list 
        // Randomize position
        /*[x]*/ random_device rd_1; mt19937 gen_1(rd_1()); uniform_real_distribution<float> dis_1(90.0f, 170.0f); 
        /*[y]*/ random_device rd_2; mt19937 gen_2(rd_2()); uniform_real_distribution<float> dis_2(0.0f, 40.0f); 
        ///*[z]*/ random_device rd_3; mt19937 gen_3(rd_3()); uniform_real_distribution<float> dis_3(-1000.0f, 1000.0f); 

        zPositionBlueLine = 800.0f;
        //reflectiveCube refCube = reflectiveCube({dis_1(gen_1), dis_2(gen_2), 800.0f}, true, false, false, false);
        reflectiveCube refCube = reflectiveCube({dis_1(gen_1), 20.0f, 800.0f}, true, false, false, false);

        cubeList.push_back(refCube);
    }






















}

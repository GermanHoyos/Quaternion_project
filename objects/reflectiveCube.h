#include "../include/MasterHeader.h"

class reflectiveCube
{
    public:
    Vector3 myPosition;
    Vector3 mySize;
    Vector3 topFaceCenter, topFaceNormal;
    Vector3 botFaceCenter, botFaceNormal;
    Vector3 lefFaceCenter, lefFaceNormal;
    Vector3 ritFaceCenter, ritFaceNormal; 
    Vector3 fwdFaceCenter, fwdFaceNormal;
    Vector3 bckFaceCenter, bckFaceNormal;
    float   rotateX = 0.0f;
    float   rotateY = 0.0f;
    float   rotateZ = 0.0f;
    Color   myColor = {255,255,255,255};
    Matrix  cubeMatrix = MatrixIdentity();
 
    reflectiveCube(Vector3 passedPosition)
    : myPosition(passedPosition)
    {
        
        cubeMatrix.m12 = myPosition.x;
        cubeMatrix.m13 = myPosition.y;
        cubeMatrix.m14 = myPosition.z;
        topFaceCenter  = { 00.0f, 05.0f, 00.0f}; topFaceNormal  = { 00.0f, 10.0f, 00.0f};
        botFaceCenter  = { 00.0f,-05.0f, 00.0f}; botFaceNormal  = { 00.0f,-10.0f, 00.0f};
        lefFaceCenter  = {-05.0f, 00.0f, 00.0f}; lefFaceNormal  = {-10.0f, 00.0f, 00.0f};
        ritFaceCenter  = { 05.0f, 00.0f, 00.0f}; ritFaceNormal  = { 10.0f, 00.0f, 00.0f};
        fwdFaceCenter  = { 00.0f, 00.0f,-05.0f}; fwdFaceNormal  = { 00.0f, 00.0f,-10.0f};
        bckFaceCenter  = { 00.0f, 00.0f, 05.0f}; bckFaceNormal  = { 00.0f, 00.0f, 10.0f};
    }

    void drawNormals()
    {
        DrawLine3D(topFaceCenter,topFaceNormal,GREEN);
        DrawLine3D(botFaceCenter,botFaceNormal,GREEN);
        DrawLine3D(lefFaceCenter,lefFaceNormal,RED);
        DrawLine3D(ritFaceCenter,ritFaceNormal,RED);
        DrawLine3D(fwdFaceCenter,fwdFaceNormal,BLUE);
        DrawLine3D(bckFaceCenter,bckFaceNormal,BLUE);
    }

    void calcRotations()
    {
        rotateX += 0.1f;
        rotateY += 0.1f;
        rotateZ += 0.1f;
        rlRotatef(rotateX, 1.0f, 0.0f, 0.0f);
        rlRotatef(rotateY, 0.0f, 1.0f, 0.0f);
        rlRotatef(rotateZ, 0.0f, 0.0f, 1.0f);
    }

    void draw()
    {
        rlPushMatrix();
        calcRotations();
        rlMultMatrixf(MatrixToFloat(cubeMatrix));
        DrawCubeV({0.0f,0.0f,0.0f},{10.0f,10.0f,10.0f},WHITE);
        DrawCubeWiresV({0.0f,0.0f,0.0f}, {10.0f, 10.0f, 10.0f}, RED);
        drawNormals();
        rlPopMatrix();
    }
};






// class reflectiveCube
// {
// public:
//     Vector3 myPosition;
//     Vector3 mySize;
//     Vector3 topFaceCenter, topFaceNormal;
//     Vector3 botFaceCenter, botFaceNormal;
//     Vector3 lefFaceCenter, lefFaceNormal;
//     Vector3 ritFaceCenter, ritFaceNormal; 
//     Vector3 fwdFaceCenter, fwdFaceNormal;
//     Vector3 bckFaceCenter, bckFaceNormal;
//     float   rotateX = 0.0f;
//     float   rotateY = 0.0f;
//     float   rotateZ = 0.0f;
//     Color   myColor = {255,255,255,255};
//     Matrix  cubeMatrix;

//     reflectiveCube(Vector3 passedPosition)
//     : myPosition(passedPosition)
//     {
//         // Initialize cubeMatrix with translation
//         cubeMatrix = MatrixIdentity();
//         cubeMatrix.m12 = myPosition.x;
//         cubeMatrix.m13 = myPosition.y;
//         cubeMatrix.m14 = myPosition.z;

//         // Set face centers and normals
//         topFaceCenter  = { 00.0f, 05.0f, 00.0f}; topFaceNormal  = { 00.0f, 10.0f, 00.0f};
//         botFaceCenter  = { 00.0f,-05.0f, 00.0f}; botFaceNormal  = { 00.0f,-10.0f, 00.0f};
//         lefFaceCenter  = {-05.0f, 00.0f, 00.0f}; lefFaceNormal  = {-10.0f, 00.0f, 00.0f};
//         ritFaceCenter  = { 05.0f, 00.0f, 00.0f}; ritFaceNormal  = { 10.0f, 00.0f, 00.0f};
//         fwdFaceCenter  = { 00.0f, 00.0f,-05.0f}; fwdFaceNormal  = { 00.0f, 00.0f,-10.0f};
//         bckFaceCenter  = { 00.0f, 00.0f, 05.0f}; bckFaceNormal  = { 00.0f, 00.0f, 10.0f};
//     }

//     void drawNormals()
//     {
//         DrawLine3D(topFaceCenter, topFaceCenter + topFaceNormal, GREEN);
//         DrawLine3D(botFaceCenter, botFaceCenter + botFaceNormal, GREEN);
//         DrawLine3D(lefFaceCenter, lefFaceCenter + lefFaceNormal, RED);
//         DrawLine3D(ritFaceCenter, ritFaceCenter + ritFaceNormal, RED);
//         DrawLine3D(fwdFaceCenter, fwdFaceCenter + fwdFaceNormal, BLUE);
//         DrawLine3D(bckFaceCenter, bckFaceCenter + bckFaceNormal, BLUE);
//     }

//     void calcRotations()
//     {
//         rotateX += 0.1f;
//         rotateY += 0.1f;
//         rotateZ += 0.1f;

//         Matrix rotationX = MatrixRotateX(rotateX);
//         Matrix rotationY = MatrixRotateY(rotateY);
//         Matrix rotationZ = MatrixRotateZ(rotateZ);
        
//         // Apply rotations to the cubeMatrix
//         cubeMatrix = MatrixMultiply(rotationX, cubeMatrix);
//         cubeMatrix = MatrixMultiply(rotationY, cubeMatrix);
//         cubeMatrix = MatrixMultiply(rotationZ, cubeMatrix);
//     }

//     void draw()
//     {
//         rlPushMatrix();
//         calcRotations();
//         rlMultMatrixf(MatrixToFloat(cubeMatrix));
//         DrawCubeV({0.0f,0.0f,0.0f},{10.0f,10.0f,10.0f}, WHITE);
//         DrawCubeWiresV({0.0f,0.0f,0.0f}, {10.0f, 10.0f, 10.0f}, RED);
//         drawNormals();
//         rlPopMatrix();
//     }
// };
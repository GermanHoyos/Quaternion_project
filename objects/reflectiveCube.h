#include "../include/MasterHeader.h"

class reflectiveCube
{
    public:
    Vector3 myPosition;    Vector3 mySize;
    Vector3 topFaceCenter, topFaceNormal; Vector3 getTopFaceCenter; Vector3 getTopFaceNormal; Vector3 topFaceCenterInWorldSpace; Vector3 topFaceNormalInWorldSpace;
    Vector3 botFaceCenter, botFaceNormal; Vector3 getBotFaceCenter; Vector3 getBotFaceNormal; Vector3 botFaceCenterInWorldSpace; Vector3 botFaceNormalInWorldSpace;
    Vector3 lefFaceCenter, lefFaceNormal; Vector3 getLefFaceCenter; Vector3 getLefFaceNormal; Vector3 lefFaceCenterInWorldSpace; Vector3 lefFaceNormalInWorldSpace; 
    Vector3 ritFaceCenter, ritFaceNormal; Vector3 getRitFaceCenter; Vector3 getRitFaceNormal; Vector3 ritFaceCenterInWorldSpace; Vector3 ritFaceNormalInWorldSpace;
    Vector3 fwdFaceCenter, fwdFaceNormal; Vector3 getFwdFaceCenter; Vector3 getFwdFaceNormal; Vector3 fwdFaceCenterInWorldSpace; Vector3 fwdFaceNornalInWorldSpace;
    Vector3 bckFaceCenter, bckFaceNormal; Vector3 getBckFaceCenter; Vector3 getBckFaceNormal; Vector3 bckFaceCenterInWorldSpace; Vector3 bckFaceNornalInWorldSpace;
    float   rotateX    = 0.0f; float   rotateY = 0.0f; float   rotateZ = 0.0f;
    Color   myColor    = {255,255,255,255};
    Matrix  cubeMatrix = MatrixIdentity();
    Matrix  worldMatrix;
 
    reflectiveCube(Vector3 passedPosition)
    : myPosition(passedPosition)
    {
        cubeMatrix.m12 = myPosition.x; cubeMatrix.m13 = myPosition.y; cubeMatrix.m14 = myPosition.z;
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
        rotateX += 0.1f;  rlRotatef(rotateX, 1.0f, 0.0f, 0.0f);
        rotateY += 0.1f;  rlRotatef(rotateY, 0.0f, 1.0f, 0.0f);
        rotateZ += 0.1f;  rlRotatef(rotateZ, 0.0f, 0.0f, 1.0f);

        // Apply rotations to topFaceCenter to get the rotated position
        Matrix rotationMatrix = MatrixRotateXYZ({DEG2RAD * rotateX, DEG2RAD * rotateY, DEG2RAD * rotateZ});
        getTopFaceCenter = Vector3Transform(topFaceCenter, rotationMatrix);
        //next
    }

    void draw()
    {
        rlPushMatrix();
        calcRotations();
        rlMultMatrixf(MatrixToFloat(cubeMatrix));
        DrawCubeV({0.0f,0.0f,0.0f},{10.0f,10.0f,10.0f},WHITE);
        DrawCubeWiresV({0.0f,0.0f,0.0f}, {10.0f, 10.0f, 10.0f}, RED);
        drawNormals();
        topFaceCenterInWorldSpace = Vector3Transform(getTopFaceCenter, cubeMatrix);
        rlPopMatrix();
        DrawSphere(topFaceCenterInWorldSpace,1.0f,RED);
    }
};

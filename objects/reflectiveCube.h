#include "../include/MasterHeader.h"

class reflectiveCube
{
    public:
    Vector3 myPosition;
    Vector3 mySize;
    Vector3 topFaceCenter;
    Vector3 botFaceCenter;
    Vector3 lefFaceCenter;
    Vector3 ritFaceCenter;
    Vector3 fwdFaceCenter;
    Vector3 bckFaceCenter;
    Vector3 topFaceNormal;
    Vector3 botFaceNormal;
    Vector3 lefFaceNormal;
    Vector3 ritFaceNormal;
    Vector3 fwdFaceNormal;
    Vector3 bckFaceNormal;
    float   rotateX = 0.0f;
    float   rotateY = 0.0f;
    float   rotateZ = 0.0f;
    Color   myColor = {255,255,255,255};

    reflectiveCube(Vector3 passedPosition)
    : myPosition(passedPosition)
    {
        topFaceCenter = {myPosition.x +  00.0f, myPosition.y +  05.0f, myPosition.z +  00.0f};
        botFaceCenter = {myPosition.x +  00.0f, myPosition.y + -05.0f, myPosition.z +  00.0f};
        lefFaceCenter = {myPosition.x + -05.0f, myPosition.y +  00.0f, myPosition.z +  00.0f};
        ritFaceCenter = {myPosition.x +  05.0f, myPosition.y +  00.0f, myPosition.z +  00.0f};
        fwdFaceCenter = {myPosition.x +  00.0f, myPosition.y +  00.0f, myPosition.z + -05.0f};
        bckFaceCenter = {myPosition.x +  00.0f, myPosition.y +  00.0f, myPosition.z +  05.0f};
        topFaceNormal = Vector3Add(topFaceCenter, Vector3Scale({ 00.0f, 10.0f, 00.0f}, 1.0f));
        botFaceNormal = Vector3Add(botFaceCenter, Vector3Scale({ 00.0f,-10.0f, 00.0f}, 1.0f));
        lefFaceNormal = Vector3Add(lefFaceCenter, Vector3Scale({-10.0f, 00.0f, 00.0f}, 1.0f));
        ritFaceNormal = Vector3Add(ritFaceCenter, Vector3Scale({ 10.0f, 00.0f, 00.0f}, 1.0f));
        fwdFaceNormal = Vector3Add(fwdFaceCenter, Vector3Scale({ 00.0f, 00.0f,-10.0f}, 1.0f));
        bckFaceNormal = Vector3Add(bckFaceCenter, Vector3Scale({ 00.0f, 00.0f, 10.0f}, 1.0f));

    }

    void drawNormals()
    {
        DrawLine3D(topFaceCenter,topFaceNormal,RED);
        DrawLine3D(botFaceCenter,botFaceNormal,RED);
        DrawLine3D(lefFaceCenter,lefFaceNormal,RED);
        DrawLine3D(ritFaceCenter,ritFaceNormal,RED);
        DrawLine3D(fwdFaceCenter,fwdFaceNormal,RED);
        DrawLine3D(bckFaceCenter,bckFaceNormal,RED);

    }

    void calculateRotation()
    {

    }

    void draw()
    {
        DrawCubeV(myPosition, {10.0f, 10.0f, 10.0f}, GREEN);
        DrawCubeWiresV(myPosition, {10.0f, 10.0f, 10.0f}, RED);
        drawNormals();
    }
};






/*
class ReflectiveCubeExample
{
public:
    Vector3 myPosition;
    Vector3 mySize;
    Vector3 topFaceCenter;
    Vector3 botFaceCenter;
    Vector3 lefFaceCenter;
    Vector3 ritFaceCenter;
    Vector3 fwdFaceCenter;
    Vector3 bckFaceCenter;
    Vector3 topFaceNormal;
    Vector3 botFaceNormal;
    Vector3 lefFaceNormal;
    Vector3 ritFaceNormal;
    Vector3 fwdFaceNormal;
    Vector3 bckFaceNormal;
    Quaternion rotation;
    Color myColor = {255, 255, 255, 255};

    ReflectiveCube(Vector3 passedPosition)
    : myPosition(passedPosition), rotation(QuaternionIdentity())
    {
        mySize = {10.0f, 10.0f, 10.0f};

        updateFaceCenters();
        updateFaceNormals();
    }

    void updateFaceCenters()
    {
        topFaceCenter = Vector3Add(myPosition, {0.0f, 5.0f, 0.0f});
        botFaceCenter = Vector3Add(myPosition, {0.0f, -5.0f, 0.0f});
        lefFaceCenter = Vector3Add(myPosition, {-5.0f, 0.0f, 0.0f});
        ritFaceCenter = Vector3Add(myPosition, {5.0f, 0.0f, 0.0f});
        fwdFaceCenter = Vector3Add(myPosition, {0.0f, 0.0f, -5.0f});
        bckFaceCenter = Vector3Add(myPosition, {0.0f, 0.0f, 5.0f});
    }

    void updateFaceNormals()
    {
        topFaceNormal = Vector3Transform((Vector3){0.0f, 1.0f, 0.0f}, QuaternionToMatrix(rotation));
        botFaceNormal = Vector3Transform((Vector3){0.0f, -1.0f, 0.0f}, QuaternionToMatrix(rotation));
        lefFaceNormal = Vector3Transform((Vector3){-1.0f, 0.0f, 0.0f}, QuaternionToMatrix(rotation));
        ritFaceNormal = Vector3Transform((Vector3){1.0f, 0.0f, 0.0f}, QuaternionToMatrix(rotation));
        fwdFaceNormal = Vector3Transform((Vector3){0.0f, 0.0f, -1.0f}, QuaternionToMatrix(rotation));
        bckFaceNormal = Vector3Transform((Vector3){0.0f, 0.0f, 1.0f}, QuaternionToMatrix(rotation));
    }

    void rotate(Quaternion q)
    {
        rotation = QuaternionMultiply(rotation, q);
        updateFaceCenters();
        updateFaceNormals();
    }

    void drawNormals()
    {
        Vector3 topNormalEnd = Vector3Add(topFaceCenter, Vector3Scale(topFaceNormal, 5.0f));
        Vector3 botNormalEnd = Vector3Add(botFaceCenter, Vector3Scale(botFaceNormal, 5.0f));
        Vector3 lefNormalEnd = Vector3Add(lefFaceCenter, Vector3Scale(lefFaceNormal, 5.0f));
        Vector3 ritNormalEnd = Vector3Add(ritFaceCenter, Vector3Scale(ritFaceNormal, 5.0f));
        Vector3 fwdNormalEnd = Vector3Add(fwdFaceCenter, Vector3Scale(fwdFaceNormal, 5.0f));
        Vector3 bckNormalEnd = Vector3Add(bckFaceCenter, Vector3Scale(bckFaceNormal, 5.0f));

        DrawLine3D(topFaceCenter, topNormalEnd, RED);
        DrawLine3D(botFaceCenter, botNormalEnd, RED);
        DrawLine3D(lefFaceCenter, lefNormalEnd, RED);
        DrawLine3D(ritFaceCenter, ritNormalEnd, RED);
        DrawLine3D(fwdFaceCenter, fwdNormalEnd, RED);
        DrawLine3D(bckFaceCenter, bckNormalEnd, RED);
    }

    void draw()
    {
        Matrix rotationMatrix = QuaternionToMatrix(rotation);
        DrawCubeV(myPosition, mySize, rotationMatrix, GREEN);
        DrawCubeWiresV(myPosition, mySize, rotationMatrix, RED);
        drawNormals();
    }
};
*/
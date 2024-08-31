#include "../include/MasterHeader.h"

Vector3 globalTranlsatedPosition = {-300.0f, 0.0f, 0.0f};

float distFromCenter = -45.0f;

class terrainVerts;

vector<terrainVerts> tField;

class terrainVerts
{
    public:
    Vector3 myPosition;
    Vector3 translatedPosition;
    Color   myRGB;

    terrainVerts(Vector3 position)
    : myPosition(position)
    {
        translatedPosition = globalTranlsatedPosition;
        myPosition = Vector3Add(myPosition, translatedPosition);
    }

    void draw()
    {   
        DrawPoint3D(myPosition, myRGB);
        DrawSphere(myPosition,1.0f,GREEN);
        DrawSphereWires(globalTranlsatedPosition,2.0f,8,8,WHITE);      
    }    
};

// Initial call
static void createTerrain()
{
    for (int i = 0; i < 10; i++)
    {
        terrainVerts myTerrain = terrainVerts({distFromCenter, 0.0f, 0.0f});
        tField.push_back(myTerrain);
        distFromCenter += 10.0f;
    } 
}; 

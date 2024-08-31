#include "../include/MasterHeader.h"

Vector3 globalTranlsatedPosition = {-300.0f, 0.0f, 0.0f};
float distFromCenter = -45.0f;
float zTracker; 

class terrainVerts;
vector<terrainVerts> tField;

// Function declaration (if the definition is later in the file)
void createTerrain();
void moveTerrain();

class terrainVerts
{
public:
    Vector3 myPosition;
    Vector3 translatedPosition;
    Color   myRGB;
    bool    nextRowCalled = false;

    terrainVerts(Vector3 position)
    : myPosition(position)
    {
        translatedPosition = globalTranlsatedPosition;
        myPosition = Vector3Add(myPosition, translatedPosition);
    }

    void draw()
    {   
        DrawPoint3D(myPosition, myRGB);
        DrawSphere(myPosition,0.1f,GREEN);
        DrawSphereWires(globalTranlsatedPosition, 2.0f, 8, 8, WHITE);
        myPosition.z += 0.1f;

    }  
};

// Function definition
void createTerrain()
{
    for (int i = 0; i < 10; i++)
    {
        terrainVerts myTerrain = terrainVerts({distFromCenter, 0.0f, 0.0f});
        tField.push_back(myTerrain);
        distFromCenter += 10.0f;
    }
    distFromCenter = -45.0f; 
} 

void moveTerrain()
{
    zTracker += 0.1f; // This should be done once per frame, not in each draw call
    if (zTracker >= 10.0f) // Make sure the condition is correct
    {
        createTerrain(); // Create new terrain when zTracker hits the threshold
        zTracker = 0.0f; // Reset after creating new terrain
    }
}


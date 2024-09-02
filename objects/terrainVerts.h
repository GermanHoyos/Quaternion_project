#include "../include/MasterHeader.h"

Vector3 globalTranlsatedPosition = {-300.0f, 0.0f, 0.0f};
float distFromCenter = -45.0f;
float zTracker;
float zSpeed = 0.1f;
static int vertIds = 0;
bool  listIsChanging = false; 
int   countMe = 0;
int   destructDistValue = 100;

class terrainVerts;
vector<terrainVerts> tField;
vector<terrainVerts> vertIdList;

// Function declaration (if the definition is later in the file)
void createTerrain();
void moveTerrain();
void drawSurfaces();

class terrainVerts
{
public:
    int myVertId;
    Vector3 myPosition;
    Vector3 translatedPosition;
    Color   myRGB;
    bool    nextRowCalled = false;


    terrainVerts(Vector3 position)
    : myPosition(position)
    {
        myVertId = vertIds++;
        translatedPosition = globalTranlsatedPosition;
        myPosition = Vector3Add(myPosition, translatedPosition);
    }

    void draw()
    {   
        DrawSphere(myPosition,0.1f,GREEN);
        DrawSphereWires(globalTranlsatedPosition, 2.0f, 8, 8, WHITE);
        myPosition.z += zSpeed;
        
        if (myPosition.z > destructDistValue)
        {
            selfDestruct();
        }

        if (countMe == 0)
        {
            drawSurfaces();
        }

        if (countMe == 1055)
        {
            countMe = 0;
        }
    }  

    void selfDestruct()
    {
        //columnToFrames = 50;
        for (auto vertice = tField.begin(); vertice != tField.end();)
        {
            if (vertice->myPosition.z > destructDistValue)
            {
                countMe++;
                vertice = tField.erase(vertice);
            }
            else
            {
                countMe++;
                ++vertice;
            }
        }
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
    zTracker += zSpeed; // This should be done once per frame, not in each draw call
    if (zTracker >= 10.0f) // Make sure the condition is correct
    {
        createTerrain(); // Create new terrain when zTracker hits the threshold
        zTracker = 0.0f; // Reset after creating new terrain
    }
}

void drawSurfaces()
{
    DrawTriangle3D(tField[0].myPosition, tField[1].myPosition, tField[10].myPosition, RED );
}


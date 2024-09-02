#include "../include/MasterHeader.h"

class terrainMesh;

vector<terrainMesh> meshList;

static int meshIds = 0;

Vector3 terrainMeshTranslate = {0.0f, 0.0f, -200.0f};

class terrainMesh
{

    public:
    Vector3 myPosition;
    vector<Vector3> myGridVerts;
    bool gridCreated = false;
    int myMeshId;
    int rows = 20;
    int cols = 11;
    float spacing = 4.5f;

    terrainMesh(Vector3 position)
    : myPosition(position)
    {
        myMeshId = meshIds++;
        myPosition = Vector3Add(myPosition, terrainMeshTranslate);
    }

    void draw()
    {
        if (!gridCreated) { buildInternalGrid(); gridCreated = true; }
        DrawSphere(myPosition, 1.0f, GREEN);

        for(auto& eachVert : myGridVerts)
        {
            DrawSphere(eachVert, 0.2f, GREEN);
        }

        DrawTriangle3D(
            {myGridVerts[0].x,myGridVerts[0].y,myGridVerts[0].z},
            {myGridVerts[1].x,myGridVerts[1].y,myGridVerts[1].z},
            {myGridVerts[11].x,myGridVerts[11].y,myGridVerts[11].z},
        WHITE);
        DrawLine3D(
            {myGridVerts[0].x,myGridVerts[0].y,myGridVerts[0].z},
            {myGridVerts[1].x,myGridVerts[1].y,myGridVerts[1].z},
        RED);
        DrawLine3D(
            {myGridVerts[1].x,myGridVerts[1].y,myGridVerts[1].z},
            {myGridVerts[11].x,myGridVerts[11].y,myGridVerts[11].z},
        RED);
        DrawLine3D(
            {myGridVerts[11].x,myGridVerts[11].y,myGridVerts[11].z},
            {myGridVerts[0].x,myGridVerts[0].y,myGridVerts[0].z},
        RED);
        // DrawTriangle3D(
        //     {myGridVerts[1].x,myGridVerts[1].y,myGridVerts[1].z},
        //     {myGridVerts[12].x,myGridVerts[12].y,myGridVerts[12].z},
        //     {myGridVerts[11].x,myGridVerts[11].y,myGridVerts[11].z},
        // WHITE);
        // DrawLine3D(
        //     {myGridVerts[1].x,myGridVerts[1].y,myGridVerts[1].z},
        //     {myGridVerts[12].x,myGridVerts[12].y,myGridVerts[12].z},
        // RED);
        // DrawLine3D(
        //     {myGridVerts[12].x,myGridVerts[12].y,myGridVerts[12].z},
        //     {myGridVerts[11].x,myGridVerts[11].y,myGridVerts[11].z},
        // RED);
        // DrawLine3D(
        //     {myGridVerts[11].x,myGridVerts[11].y,myGridVerts[11].z},
        //     {myGridVerts[1].x,myGridVerts[1].y,myGridVerts[1].z},
        // RED);














    }

    void buildInternalGrid()
    {

        for (int row = 0; row < rows; ++row)
        {
            for(int col = 0; col < cols; ++col)
            {
                float x = col * spacing;
                float z = row * spacing;
                Vector3 vec3 = Vector3({myPosition.x + x, myPosition.y, myPosition.z - z});
                myGridVerts.push_back(vec3);
            }
        }
    }

    void moveGrid()
    {

    }

};

void createMeshGrid()
{

    terrainMesh meshGrid = terrainMesh({0.0f,0.0f,0.0f});
    meshList.push_back(meshGrid);
}
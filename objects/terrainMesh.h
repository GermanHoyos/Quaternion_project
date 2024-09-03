#include "../include/MasterHeader.h"

class terrainMesh;

vector<terrainMesh> meshList;

static int meshIds = 0;

Vector3 terrainMeshTranslate = {400.0f, 0.0f, 0.0f};

vector<Vector3> globalGridVerts;

float mesh_0_z = 0.0f;
string display_0;

float mesh_1_z = 0.0f;
string display_1;

class terrainMesh
{

    public:
    Vector3 myPosition;
    vector<Vector3> myGridVerts;
    bool gridCreated = false;
    int myMeshId;
    int rows = 20;
    int cols = 11;
    float spacing = 100.0f;
    float rand_y;
    float zSpeed = 3.0f;
    float trackMyFirstVertexZ;
    Matrix myPlane_mesh0 = MatrixIdentity();
    Matrix myPlane_mesh1 = MatrixIdentity();

    float moveZ_mesh0 = 0.0f;
    float moveZ_mesh1 = 0.0f;

    Color myColor = {255,255,255,255};

    bool specialCase = true;
    bool mesh_0_switch = false;
    bool mesh_1_switch = false;

    terrainMesh(Vector3 position)
    : myPosition(position)
    {
        myMeshId = meshIds++; // only 1 after its assigned to 0
        myPosition = Vector3Add(myPosition, terrainMeshTranslate);
    }

    void draw()
    {
        if (!gridCreated) { buildInternalGrid(); gridCreated = true; }

        // Start sphere 1 VERT
        trackMyFirstVertexZ = myPosition.z + myGridVerts[0].z;
        //DrawSphere({myPosition.x, myPosition.y, trackMyFirstVertexZ}, 5.0f, RED);



        //Loop in a loop....
        for (int i = 0; i < 208; i++)
        {
            if
            (
                i ==  10 || i ==  21 || i ==  32 || i ==  43 || i ==  54 || i ==  65 ||
                i ==  76 || i ==  87 || i ==  98 || i == 109 || i == 120 || i == 131 ||
                i == 142 || i == 153 || i == 164 || i == 175 || i == 186 || i == 197 
            ) continue;

            rlPushMatrix();

            //rlTranslatef(0.0f,0.0f, moveZ);
            if(myMeshId == 0){rlTranslatef(0.0f,0.0f, moveZ_mesh0);}

            if(myMeshId == 1){rlTranslatef(0.0f,0.0f, moveZ_mesh1);}


            
            if(myMeshId == 0) {rlMultMatrixf(MatrixToFloat(myPlane_mesh0));}
            
            if(myMeshId == 1) {rlMultMatrixf(MatrixToFloat(myPlane_mesh1));}


            if (myMeshId == 0) {DrawSphere({myPosition.x, myPosition.y, myPosition.z}, 5.0f, RED);}
            if (myMeshId == 1) {DrawSphere({myPosition.x - 1.0f, myPosition.y, myPosition.z}, 6.0f, GREEN);}
            
            






                DrawTriangle3D(
                    {myGridVerts[i].x, myGridVerts[i].y, myGridVerts[i].z},
                    {myGridVerts[i + 1].x, myGridVerts[i + 1].y, myGridVerts[i + 1].z},
                    {myGridVerts[i + 11].x, myGridVerts[i + 11].y, myGridVerts[i+ 11].z},
                DARKGRAY);
                DrawTriangle3D(
                    {myGridVerts[i + 1].x, myGridVerts[i + 1].y,myGridVerts[i + 1].z},
                    {myGridVerts[i + 12].x, myGridVerts[i + 12].y,myGridVerts[i + 12].z},
                    {myGridVerts[i + 11].x, myGridVerts[i + 11].y,myGridVerts[i+ 11].z},
                DARKGRAY);
                DrawLine3D(
                    {myGridVerts[i].x, myGridVerts[i].y + 0.1f,myGridVerts[i].z},
                    {myGridVerts[i + 1].x, myGridVerts[i + 1].y + 0.1f,myGridVerts[i + 1].z},
                BLUE);
                DrawLine3D(
                    {myGridVerts[i + 1].x, myGridVerts[i + 1].y + 0.1f,myGridVerts[i + 1].z},
                    {myGridVerts[i + 12].x, myGridVerts[i + 12].y + 0.1f,myGridVerts[i + 12].z},
                BLUE);
                DrawLine3D(
                    {myGridVerts[i + 1].x, myGridVerts[i + 1].y, myGridVerts[i + 1].z},
                    {myGridVerts[i + 11].x, myGridVerts[i + 11].y, myGridVerts[i + 11].z},
                BLUE);

            rlPopMatrix();

        }

        moveGrid();

    }

    void buildInternalGrid()
    {
        //myGridVerts.clear();
        
        random_device rd_1;
        mt19937 gen_1(rd_1());
        uniform_real_distribution<float> dis_1(myPosition.y, myPosition.y + 60.0f);

        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                float x = col * spacing;
                float z = row * spacing;
                rand_y = dis_1(gen_1);
                if (row == 0 || row == 19){ rand_y = 0.0f;};
                Vector3 vec3 = Vector3({myPosition.x + x, rand_y, myPosition.z - z});
                myGridVerts.push_back(vec3);
            }
        }
    }

    void moveGrid()
    {
        //moveZ += 4.0f;
        if(myMeshId == 0) {moveZ_mesh0 += 4.0;}
        if(myMeshId == 1) {moveZ_mesh1 += 4.0;}

        if (myMeshId == 0) 
        {
            // Reset moveZ when it exceeds the length of one mesh
            if (moveZ_mesh0 >= 1900)
            {
                moveZ_mesh0 = -1900;
            }
        }

        if (myMeshId == 1)
        {
            if (moveZ_mesh1 >= 1900 * 2)
            {
                moveZ_mesh1 = 0;
            }
        }

        // print locations
        mesh_0_z = moveZ_mesh0;
        if(myMeshId == 0)
        {
            display_0 = "RED " + to_string(mesh_0_z);
        }

        mesh_1_z = moveZ_mesh1;
        if(myMeshId == 1)
        {
            display_1 = "GREEN " + to_string(mesh_1_z);
        }

    }

};





void createMeshGrid()
{
    // terrainMesh meshGrid = terrainMesh({0.0f,0.0f,0.0f});
    // meshList.push_back(meshGrid);

    float zOffset = 0.0f;
    for(int i = 0; i < 2; i++)
    {
        terrainMesh meshGrid = terrainMesh({0.0f,0.0f,zOffset});
        meshList.push_back(meshGrid);
        zOffset += -1900;
    }
}

#include "../include/MasterHeader.h"
#define DARKRED    CLITERAL(Color){ 160, 41, 55, 255 } 



class reflectiveCube
{
    public:
    Vector3 myPosition;    Vector3 mySize;
    Vector3 topFaceCenter, topFaceNormal; Vector3 getTopFaceCenter; Vector3 getTopFaceNormal; Vector3 topFaceCenterInWorldSpace; Vector3 topFaceNormalInWorldSpace;
    Vector3 botFaceCenter, botFaceNormal; Vector3 getBotFaceCenter; Vector3 getBotFaceNormal; Vector3 botFaceCenterInWorldSpace; Vector3 botFaceNormalInWorldSpace;
    Vector3 lefFaceCenter, lefFaceNormal; Vector3 getLefFaceCenter; Vector3 getLefFaceNormal; Vector3 lefFaceCenterInWorldSpace; Vector3 lefFaceNormalInWorldSpace; 
    Vector3 ritFaceCenter, ritFaceNormal; Vector3 getRitFaceCenter; Vector3 getRitFaceNormal; Vector3 ritFaceCenterInWorldSpace; Vector3 ritFaceNormalInWorldSpace;
    Vector3 fwdFaceCenter, fwdFaceNormal; Vector3 getFwdFaceCenter; Vector3 getFwdFaceNormal; Vector3 fwdFaceCenterInWorldSpace; Vector3 fwdFaceNormalInWorldSpace;
    Vector3 bckFaceCenter, bckFaceNormal; Vector3 getBckFaceCenter; Vector3 getBckFaceNormal; Vector3 bckFaceCenterInWorldSpace; Vector3 bckFaceNormalInWorldSpace;
    float   rotateX    = 0.0f; float   rotateY = 0.0f; float   rotateZ = 0.0f;
    float   hitSphereRadius = 8.5f;
    Color   myColor    = {255,255,255,255};
    Matrix  cubeMatrix = MatrixIdentity();
    Matrix  worldMatrix;
    bool    toggleMySkin = false;
    bool    textureLoaded = false;
    bool    hit = false;

    Model rock = LoadModel("C:\\Users\\Hoyos\\OneDrive\\Desktop\\C++ Runner\\raylib_quaternion_example\\home\\src\\assets\\rock_1.obj");
    Texture2D rockTexture = LoadTexture("C:\\Users\\Hoyos\\OneDrive\\Desktop\\C++ Runner\\raylib_quaternion_example\\home\\src\\assets\\t2.png");


 
    reflectiveCube(Vector3 passedPosition, bool skinToggle)
    : myPosition(passedPosition), toggleMySkin(skinToggle)
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
        /*******************
        **                **
        ** Visualizations **
        ** for reflect    **
        ** math           **
        **                **
        **                **
        *******************/
        // DrawLine3D(topFaceCenter,topFaceNormal,GREEN);
        // DrawLine3D(botFaceCenter,botFaceNormal,PURPLE);
        // DrawLine3D(lefFaceCenter,lefFaceNormal,RED);
        // DrawLine3D(ritFaceCenter,ritFaceNormal,RED);
        // DrawLine3D(fwdFaceCenter,fwdFaceNormal,BLUE);
        // DrawLine3D(bckFaceCenter,bckFaceNormal,BLUE);
    }

    void calcRotations()
    {
        rotateX += 1.0f;  rlRotatef(rotateX, 1.0f, 0.0f, 0.0f);
        rotateY += 1.0;  rlRotatef(rotateY, 0.0f, 1.0f, 0.0f);
        rotateZ += 1.0f;  rlRotatef(rotateZ, 0.0f, 0.0f, 1.0f);
        Matrix rotationMatrix = MatrixRotateXYZ({DEG2RAD * rotateX, DEG2RAD * rotateY, DEG2RAD * rotateZ});
        getTopFaceCenter = Vector3Transform(topFaceCenter, rotationMatrix); getTopFaceNormal = Vector3Transform(topFaceNormal, rotationMatrix);
        getBotFaceCenter = Vector3Transform(botFaceCenter, rotationMatrix); getBotFaceNormal = Vector3Transform(botFaceNormal, rotationMatrix);
        getLefFaceCenter = Vector3Transform(lefFaceCenter, rotationMatrix); getLefFaceNormal = Vector3Transform(lefFaceNormal, rotationMatrix);
        getRitFaceCenter = Vector3Transform(ritFaceCenter, rotationMatrix); getRitFaceNormal = Vector3Transform(ritFaceNormal, rotationMatrix);
        getFwdFaceCenter = Vector3Transform(fwdFaceCenter, rotationMatrix); getFwdFaceNormal = Vector3Transform(fwdFaceNormal, rotationMatrix);
        getBckFaceCenter = Vector3Transform(bckFaceCenter, rotationMatrix); getBckFaceNormal = Vector3Transform(bckFaceNormal, rotationMatrix);
    }

    void draw()
    {
        loadTexture();
        rlPushMatrix();
        calcRotations();
        rlMultMatrixf(MatrixToFloat(cubeMatrix));
        //DrawCubeV({0.0f,0.0f,0.0f},{10.0f,10.0f,10.0f},WHITE);
        //DrawCubeWiresV({0.0f,0.0f,0.0f}, {10.0f, 10.0f, 10.0f}, RED);
        drawNormals();
        topFaceCenterInWorldSpace = Vector3Transform(getTopFaceCenter, cubeMatrix); topFaceNormalInWorldSpace = Vector3Transform(getTopFaceNormal, cubeMatrix);
        botFaceCenterInWorldSpace = Vector3Transform(getBotFaceCenter, cubeMatrix); botFaceNormalInWorldSpace = Vector3Transform(getBotFaceNormal, cubeMatrix);
        lefFaceCenterInWorldSpace = Vector3Transform(getLefFaceCenter, cubeMatrix); lefFaceNormalInWorldSpace = Vector3Transform(getLefFaceNormal, cubeMatrix);
        ritFaceCenterInWorldSpace = Vector3Transform(getRitFaceCenter, cubeMatrix); ritFaceNormalInWorldSpace = Vector3Transform(getRitFaceNormal, cubeMatrix);
        fwdFaceCenterInWorldSpace = Vector3Transform(getFwdFaceCenter, cubeMatrix); fwdFaceNormalInWorldSpace = Vector3Transform(getFwdFaceNormal, cubeMatrix);
        bckFaceCenterInWorldSpace = Vector3Transform(getBckFaceCenter, cubeMatrix); bckFaceNormalInWorldSpace = Vector3Transform(getBckFaceNormal, cubeMatrix);
        toggleSkin();                                                                                                                                // Determine which face was hit
        hitAnimation();
        rlPopMatrix();
        /*******************
        **                **
        ** Visualizations **
        ** for reflect    **
        ** math           **
        **                **
        **                **
        *******************/
        DrawSphere(topFaceCenterInWorldSpace,1.0f,GREEN);   //DrawSphere(topFaceNormalInWorldSpace,1.0f,PURPLE);
        DrawSphere(botFaceCenterInWorldSpace,1.0f,DARKGREEN);    //DrawSphere(botFaceNormalInWorldSpace,1.0f,GREEN);
        DrawSphere(lefFaceCenterInWorldSpace,1.0f,DARKRED);      //DrawSphere(lefFaceNormalInWorldSpace,1.0f,RED);
        DrawSphere(ritFaceCenterInWorldSpace,1.0f,RED);      //DrawSphere(ritFaceNormalInWorldSpace,1.0f,RED);
        DrawSphere(fwdFaceCenterInWorldSpace,1.0f,DARKBLUE);     //DrawSphere(fwdFaceNormalInWorldSpace,1.0f,BLUE);
        DrawSphere(bckFaceCenterInWorldSpace,1.0f,BLUE);     //DrawSphere(bckFaceNormalInWorldSpace,1.0f,BLUE);
        cubeNormals[0].x  = topFaceCenterInWorldSpace.x; cubeNormals[0].y  = topFaceCenterInWorldSpace.y; cubeNormals[0].z  = topFaceCenterInWorldSpace.z; // Top       face normal start
        cubeNormals[1].x  = topFaceNormalInWorldSpace.x; cubeNormals[1].y  = topFaceNormalInWorldSpace.y; cubeNormals[1].z  = topFaceNormalInWorldSpace.z; // Top       face normal end
        cubeNormals[2].x  = botFaceCenterInWorldSpace.x; cubeNormals[2].y  = botFaceCenterInWorldSpace.y; cubeNormals[2].z  = botFaceCenterInWorldSpace.z; // Bottom    face normal start
        cubeNormals[3].x  = botFaceNormalInWorldSpace.x; cubeNormals[3].y  = botFaceNormalInWorldSpace.y; cubeNormals[3].z  = botFaceNormalInWorldSpace.z; // Bottom    face normal end
        cubeNormals[4].x  = lefFaceCenterInWorldSpace.x; cubeNormals[4].y  = lefFaceCenterInWorldSpace.y; cubeNormals[4].z  = lefFaceCenterInWorldSpace.z; // Left      face normal start
        cubeNormals[5].x  = lefFaceNormalInWorldSpace.x; cubeNormals[5].y  = lefFaceNormalInWorldSpace.y; cubeNormals[5].z  = lefFaceNormalInWorldSpace.z; // Left      face normal end
        cubeNormals[6].x  = ritFaceCenterInWorldSpace.x; cubeNormals[6].y  = ritFaceCenterInWorldSpace.y; cubeNormals[6].z  = ritFaceCenterInWorldSpace.z; // Right     face normal start
        cubeNormals[7].x  = ritFaceNormalInWorldSpace.x; cubeNormals[7].y  = ritFaceNormalInWorldSpace.y; cubeNormals[7].z  = ritFaceNormalInWorldSpace.z; // Right     face normal end
        cubeNormals[8].x  = fwdFaceCenterInWorldSpace.x; cubeNormals[8].y  = fwdFaceCenterInWorldSpace.y; cubeNormals[8].z  = fwdFaceCenterInWorldSpace.z; // Foward    face normal start
        cubeNormals[9].x  = fwdFaceNormalInWorldSpace.x; cubeNormals[9].y  = fwdFaceNormalInWorldSpace.y; cubeNormals[9].z  = fwdFaceNormalInWorldSpace.z; // Foward    face normal end
        cubeNormals[10].x = bckFaceCenterInWorldSpace.x; cubeNormals[10].y = bckFaceCenterInWorldSpace.y; cubeNormals[10].z = bckFaceCenterInWorldSpace.z; // Back      face normal start
        cubeNormals[11].x = bckFaceNormalInWorldSpace.x; cubeNormals[11].y = bckFaceNormalInWorldSpace.y; cubeNormals[11].z = bckFaceNormalInWorldSpace.z; // Back      face normal end
        /*******************
        **                **
        ** Visualizations **
        ** for reflect    **
        ** math           **
        **                **
        **                **
        *******************/
        //DrawSphereWires(myPosition, hitSphereRadius, 10, 10, DARKGREEN);                                                                                // Sphere representing hit box
        detectCollisions();
        //toggleSkin();                                                                                                                                // Determine which face was hit
    }

    void detectCollisions()
    {
        for (auto& laser : lasersList)
        {
            float distanceOfLaser = std::sqrt
            (
                (laser.currentPos.x - myPosition.x) * (laser.currentPos.x - myPosition.x) +
                (laser.currentPos.y - myPosition.y) * (laser.currentPos.y - myPosition.y) +
                (laser.currentPos.z - myPosition.z) * (laser.currentPos.z - myPosition.z)
            );

            if (distanceOfLaser < hitSphereRadius)
            {
                float shortestDistance = 100.0f;
                int   indexOfShortestD;
                for (int i = 0; i < 11; i += 2) 
                {
                    
                    float distanceFromNormalStart = std::sqrt
                    (
                        (laser.currentPos.x - cubeNormals[i].x) * (laser.currentPos.x - cubeNormals[i].x) +
                        (laser.currentPos.y - cubeNormals[i].y) * (laser.currentPos.y - cubeNormals[i].y) +
                        (laser.currentPos.z - cubeNormals[i].z) * (laser.currentPos.z - cubeNormals[i].z)
                    );

                    if (distanceFromNormalStart < shortestDistance)
                    {
                        shortestDistance = distanceFromNormalStart;
                        indexOfShortestD = i;
                    }
                }
                // The vector passed here will be the travel vector of the laser
                laser.changeDirection(cubeNormals[indexOfShortestD], cubeNormals[indexOfShortestD + 1]);
                hit = true;
            }
        }
    }

    void toggleSkin()
    {
        if (IsKeyDown(KEY_T))
        {
            if (toggleMySkin)
            {
                toggleMySkin = false;
            } else {
                toggleMySkin = true;
            }
        }

        if (toggleMySkin)
        {
            // void DrawModel(Model model, Vector3 position, float scale, Color tint);               // Draw a model (with texture if set)
            // void DrawModelEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint); // Draw a model with extended parameters
            // void DrawModelWires(Model model, Vector3 position, float scale, Color tint);          // Draw a model wires (with texture if set)
            // void DrawModelWiresEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint); // Draw a model wires (with texture if set) with extended parameters
            //DrawSphereWires({0.0f,0.0f,0.0f}, hitSphereRadius, 10, 10, DARKGREEN);
            DrawModel(rock,{0.0f,0.0f,0.0f}, 8.0f, WHITE);
        }
    }

    void loadTexture()
    {
        if(!textureLoaded){ rock.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = rockTexture; textureLoaded = true;};
    }

    void hitAnimation()
    {
        if (hit)
        {
            DrawSphere({0.0f,0.0f,0.0f},8.3f,WHITE);
        }
        hit = false;
    }

};

// Vector3 cubeNormals[12] = {
//     { 00.0f, 00.0f, 00.0f},  //0  Top       face normal start [distance checked]
//     { 00.0f, 00.0f, 00.0f},  //1  Top       face normal end
//     { 00.0f, 00.0f, 00.0f},  //2  Bottom    face normal start [distance checked]
//     { 00.0f, 00.0f, 00.0f},  //3  Bottom    face normal end
//     { 00.0f, 00.0f, 00.0f},  //4  Left      face normal start [distance checked]
//     { 00.0f, 00.0f, 00.0f},  //5  Left      face normal end
//     { 00.0f, 00.0f, 00.0f},  //6  Right     face normal start [distance checked]
//     { 00.0f, 00.0f, 00.0f},  //7  Right     face normal end
//     { 00.0f, 00.0f, 00.0f},  //8  Forward   face normal start [distance checked]
//     { 00.0f, 00.0f, 00.0f},  //9  Forward   face normal end
//     { 00.0f, 00.0f, 00.0f},  //10 Back      face normal start [distance checked]
//     { 00.0f, 00.0f, 00.0f}   //11 Back      face normal end
// };


/*
#include "raylib.h"

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Raylib - Texture on Sphere Example");

    // Load texture
    Texture2D texture = LoadTexture("resources/earth_texture.png");

    // Define the camera to look into our 3D world
    Camera camera = { 0 };
    camera.position = (Vector3){ 4.0f, 4.0f, 4.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };    // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };        // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                              // Camera field-of-view Y
    camera.type = CAMERA_PERSPECTIVE;                 // Camera mode type

    SetCameraMode(camera, CAMERA_FREE);               // Set a free camera mode

    SetTargetFPS(60);                                 // Set the game to run at 60 frames-per-second

    while (!WindowShouldClose())                      // Detect window close button or ESC key
    {
        // Update
        UpdateCamera(&camera);                        // Update camera

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Draw sphere with texture
                DrawSphereEx((Vector3){ 0.0f, 0.0f, 0.0f }, 2.0f, 32, 32, WHITE);
                DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, 2.0f, 32, 32, BLACK);

                // Apply the texture
                rlPushMatrix();
                rlTranslatef(0.0f, 0.0f, 0.0f);        // Translate to the sphere's position
                rlRotatef(0, 0, 1, 0);                 // Rotate sphere if needed
                rlScalef(2.0f, 2.0f, 2.0f);            // Scale sphere to the right size
                rlEnableTexture(texture.id);           // Enable texture
                DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, WHITE); // Draw the sphere with texture
                rlDisableTexture();
                rlPopMatrix();

            EndMode3D();

            DrawText("Texture mapped to a sphere", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(texture);      // Unload texture
    CloseWindow();               // Close window and OpenGL context

    return 0;
}



*/
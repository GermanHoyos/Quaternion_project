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
    float   rotateX         = 0.0f; float   rotateY = 0.0f; float   rotateZ = 0.0f;
    float   axisRotX = 0.0f; float axisRotY = 0.0f; float axisRotZ = 0.0f;
    float   hitSphereRadius = 8.5f;
    Color   myColor         = {255,255,255,255};
    Matrix  cubeMatrix      = MatrixIdentity();
    Matrix  worldMatrix;
    bool    toggleMySkin    = false;
    bool    textureLoaded   = false;
    bool    hit             = false;
    bool    planeCube       = false;
    bool    drwNormls       = false;
    bool    skin_1          = false;
    bool    skin_2          = false;
    bool    skin_3          = false;
    bool    stationary      = false;
    bool    timeToDie       = false;
    bool    rotationChose   = false;
    int     hitCount        = 0;
    Color   rockColor       = {255,255,255,255};


    Model     rock = LoadModel("C:\\Users\\Hoyos\\OneDrive\\Desktop\\C++ Runner\\raylib_quaternion_example\\home\\src\\assets\\rock_2.obj");
    Texture2D rockTexture = LoadTexture("C:\\Users\\Hoyos\\OneDrive\\Desktop\\C++ Runner\\raylib_quaternion_example\\home\\src\\assets\\rock_2.png");
    Texture2D rockTexture_hit = LoadTexture("C:\\Users\\Hoyos\\OneDrive\\Desktop\\C++ Runner\\raylib_quaternion_example\\home\\src\\assets\\rock_2_hit.png");

    // //Randomaize rotations
    // /*[x]*/ random_device rd_1; mt19937 gen_1(rd_1()); uniform_real_distribution<float> dis_1(-1.0f, 1.0f); 
    // /*[y]*/ random_device rd_2; mt19937 gen_2(rd_2()); uniform_real_distribution<float> dis_2(-1.0f, 1.0f); 
    // /*[z]*/ random_device rd_3; mt19937 gen_3(rd_3()); uniform_real_distribution<float> dis_3(-1.0f, 1.0f); 

    reflectiveCube(Vector3 passedPosition, bool skinToggle, bool isPlaneCube, bool showNormals, bool isStationary)
    : myPosition(passedPosition), toggleMySkin(skinToggle), planeCube(isPlaneCube), drwNormls(showNormals), stationary(isStationary)
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
        if(drwNormls)
        {
            DrawLine3D(topFaceCenter,topFaceNormal,GREEN);
            DrawLine3D(botFaceCenter,botFaceNormal,PURPLE);
            DrawLine3D(lefFaceCenter,lefFaceNormal,RED);
            DrawLine3D(ritFaceCenter,ritFaceNormal,RED);
            DrawLine3D(fwdFaceCenter,fwdFaceNormal,BLUE);
            DrawLine3D(bckFaceCenter,bckFaceNormal,BLUE);
        }
    }

    void calcRotations()
    {

        rotateX += axisRotX;  rlRotatef(rotateX, 1.0f, 0.0f, 0.0f);  if (rotateX <= -360 || rotateX >= 360) { rotateX = 0; }
        rotateY += axisRotY;  rlRotatef(rotateY, 0.0f, 1.0f, 0.0f);  if (rotateY <= -360 || rotateX >= 360) { rotateY = 0; }
        rotateZ += axisRotZ;  rlRotatef(rotateZ, 0.0f, 0.0f, 1.0f);  if (rotateZ <= -360 || rotateX >= 360) { rotateZ = 0; }
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
        chooseRotation();
        loadTexture();
        rlPushMatrix();
        calcRotations();
        rlMultMatrixf(MatrixToFloat(cubeMatrix));
        if (planeCube)
        {
            DrawCubeV({0.0f,0.0f,0.0f},{10.0f,10.0f,10.0f},WHITE);
            DrawCubeWiresV({0.0f,0.0f,0.0f}, {10.0f, 10.0f, 10.0f}, RED);
        }
        drawNormals();
        topFaceCenterInWorldSpace = Vector3Transform(getTopFaceCenter, cubeMatrix); topFaceNormalInWorldSpace = Vector3Transform(getTopFaceNormal, cubeMatrix);
        botFaceCenterInWorldSpace = Vector3Transform(getBotFaceCenter, cubeMatrix); botFaceNormalInWorldSpace = Vector3Transform(getBotFaceNormal, cubeMatrix);
        lefFaceCenterInWorldSpace = Vector3Transform(getLefFaceCenter, cubeMatrix); lefFaceNormalInWorldSpace = Vector3Transform(getLefFaceNormal, cubeMatrix);
        ritFaceCenterInWorldSpace = Vector3Transform(getRitFaceCenter, cubeMatrix); ritFaceNormalInWorldSpace = Vector3Transform(getRitFaceNormal, cubeMatrix);
        fwdFaceCenterInWorldSpace = Vector3Transform(getFwdFaceCenter, cubeMatrix); fwdFaceNormalInWorldSpace = Vector3Transform(getFwdFaceNormal, cubeMatrix);
        bckFaceCenterInWorldSpace = Vector3Transform(getBckFaceCenter, cubeMatrix); bckFaceNormalInWorldSpace = Vector3Transform(getBckFaceNormal, cubeMatrix);
        hitAnimation();
        if (!stationary)
        {
            cubeMatrix.m14 -= 0.9f;
        }

        myPosition.z = cubeMatrix.m14;
        rlPopMatrix();
        if (drwNormls)
        {
            DrawSphere(topFaceCenterInWorldSpace,1.0f,GREEN);       DrawSphere(topFaceNormalInWorldSpace,1.0f,GREEN);
            DrawSphere(botFaceCenterInWorldSpace,1.0f,DARKGREEN);   DrawSphere(botFaceNormalInWorldSpace,1.0f,DARKGREEN);
            DrawSphere(lefFaceCenterInWorldSpace,1.0f,DARKRED);     DrawSphere(lefFaceNormalInWorldSpace,1.0f,DARKRED);
            DrawSphere(ritFaceCenterInWorldSpace,1.0f,RED);         DrawSphere(ritFaceNormalInWorldSpace,1.0f,RED);
            DrawSphere(fwdFaceCenterInWorldSpace,1.0f,DARKBLUE);    DrawSphere(fwdFaceNormalInWorldSpace,1.0f,DARKBLUE);
            DrawSphere(bckFaceCenterInWorldSpace,1.0f,BLUE);        DrawSphere(bckFaceNormalInWorldSpace,1.0f,BLUE);
            DrawSphereWires(myPosition, hitSphereRadius, 10, 10, DARKGREEN);  
        }
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
        detectCollisions();
        selfDestruct();                                                                                                                   // Determine which face was hit
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
                explosions explosion = explosions(laser.currentPos);
                explosionsList.push_back(explosion);
            }
        }
    }

    void loadTexture()
    {
        if(!textureLoaded)
        {
            rock.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = rockTexture;
            textureLoaded = true;
        }
    }

    void hitAnimation()
    {
        if (!hit || !textureLoaded)
        {
            rock.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = rockTexture;
        }

        if (hit)
        {
            rock.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = rockTexture_hit;
            hitCount++;
        }

        hit = false;

        if (hitCount > 10)
        {
            timeToDie = true;
        }

        if (myPosition.z  <= -20.0f)
        {
            timeToDie = true;
        }

        if(!planeCube)
        {
            DrawModel(rock, {0.0f,0.0f,0.0f}, 8.0f, rockColor);
        }
    }

    void selfDestruct()
    {
        for (auto cube = cubeList.begin(); cube != cubeList.end(); ++cube)
        {
            if (cube->timeToDie == true)
            {
                cubeList.erase(cube);
                break;
            }
        }
    }

    void chooseRotation()
    {
        if(!rotationChose)
        {
            //Randomaize rotations
            /*[x]*/ random_device rd_1; mt19937 gen_1(rd_1()); uniform_real_distribution<float> dis_1(-2.0f, 2.0f); axisRotX = dis_1(gen_1); 
            /*[y]*/ random_device rd_2; mt19937 gen_2(rd_2()); uniform_real_distribution<float> dis_2(-2.0f, 2.0f); axisRotY = dis_2(gen_2);
            /*[z]*/ random_device rd_3; mt19937 gen_3(rd_3()); uniform_real_distribution<float> dis_3(-2.0f, 2.0f); axisRotZ = dis_3(gen_3);
            rotationChose = true; 
        }
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

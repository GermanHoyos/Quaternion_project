#include "../include/MasterHeader.h"



//class lasers;

//vector<lasers> lasersList;

static int laserIds = 0;

class lasers 
{
public:

   Quaternion collectRotations = QuaternionIdentity();
   Quaternion rotationDelta    = QuaternionIdentity();
   Quaternion reflectionVector = QuaternionIdentity();
   Vector3    currentPos; 
   Vector3    startPos; 
   Vector3    endPos;
   Vector3    velocity; 
   Vector3    newForwardDirection;
   Vector3    distanceFromShotStart;
   Vector3    laserVisual;
   Color      myColor     = GREEN;
   Color      changed     = GREEN;
   int        myLaserId;
   float      laserLength = 20.0f;
   bool       collidedWithObject = false;

   //LIGHT BEGIN

   //LIGHT END


   // Arugments passed at invokation
   lasers(Vector3 currentPos, Vector3 forwardDirection, float speed)
   : 
   currentPos(currentPos),
   endPos(Vector3Add(currentPos, Vector3Scale(forwardDirection, 4.0f))),
   velocity(Vector3Scale(forwardDirection, speed)) 
   {
      myLaserId = laserIds++;
      startPos = currentPos;
   }

   void selfDestruct()
   {
      // Calculate distance from shot start
      distanceFromShotStart = Vector3Subtract(currentPos, startPos);
      float distance = Vector3Length(distanceFromShotStart);
  
      if (distance > 600)
      {
         // Find the laser in the list by its ID and remove it
         for (auto laser = lasersList.begin(); laser != lasersList.end(); ++laser)
         {
            if (laser->myLaserId == myLaserId)
            {
               lasersList.erase(laser);
               break; // Exit the loop after erasing the laser
            }
         }
      }
   }

   // THE BELOW ONLY SENDS THE BULLET IN THE SURFACE NORMALS DIRECTION AND DOES NO HAVE A REFLECTION VECTOR
   // void changeDirection(Vector3 startOfLine, Vector3 endOfLine)
   // {
   //    if (!collidedWithObject)
   //    {
   //       // Calculate the new forward direction vector
   //       Vector3 newForwardDirection = Vector3Subtract(endOfLine, startOfLine);
         
   //       // Normalize the new forward direction vector
   //       newForwardDirection = Vector3Normalize(newForwardDirection);
         
   //       // Update the laser's velocity with the new forward direction
   //       velocity = Vector3Scale(newForwardDirection, Vector3Length(velocity));
         
   //       // Update the end position based on the new forward direction
   //       endPos = Vector3Add(currentPos, Vector3Scale(newForwardDirection, laserLength));

   //       //Stop listening for collisions
   //       collidedWithObject = true;
   //    }
   // }

   // THE BELOW TAKES INTO ACCOUNT REFLECTION VECTORS
   void changeDirection(Vector3 startOfLine, Vector3 endOfLine)
   {
      if (!collidedWithObject)
      {
         // My current vector of travel, otherwise known as (a)
         // Velocity is the direction vector, line 115
         Vector3 a = velocity; 

         // The surface normal of the vector the bullet collided with, otherwise known as (b)
         Vector3 b = Vector3Normalize(Vector3Subtract(endOfLine, startOfLine));

         // Calculate the dot product a * b
         float dotProduct = Vector3DotProduct(a, b);

         // Calculate the reflection vector r = a - 2(a * b)b
         // "Scale" can be interpreted as "multiply by"
         Vector3 scaledNormal = Vector3Scale(b, 2.0f * dotProduct);
         Vector3 r = Vector3Subtract(a, scaledNormal);

         // Normalize the reflection vector
         newForwardDirection = Vector3Normalize(r);

         // Update the laser's velocity with the new forward direction
         velocity = Vector3Scale(newForwardDirection, Vector3Length(velocity));

         // Stop listening for collisions. (if this is left false then the ball will be trapped)
         collidedWithObject = true;
      }
   }

   void draw() 
   {

      // Get 3D line points
      currentPos = Vector3Add(currentPos, velocity);   // Ultimatly velocity is the direction vector
      //endPos = Vector3Add(endPos, velocity);         // Get the end point of the 3d line

      // Check for direction change
      //changeDirection(); 

      // Draw a 3D line
      //DrawLine3D(currentPos, endPos, WHITE);

      // Draw a 3D sphere at the start point
      DrawSphere(currentPos, 0.4f, myColor);

      // Check for self destruct
      selfDestruct();

   }

};

/*


#include "raylib.h"
#include "raymath.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)
    InitWindow(screenWidth, screenHeight, "raylib [shaders] example - single blue light");

    Camera camera = { 0 };
    camera.position = (Vector3){ 2.0f, 4.0f, 6.0f };
    camera.target = (Vector3){ 0.0f, 0.5f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Shader shader = LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
                               TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

    // Create a single blue light
    Light blueLight = CreateLight(LIGHT_POINT, (Vector3){ 2, 1, -2 }, Vector3Zero(), BLUE, shader);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

        // Update light values (in case of any changes)
        UpdateLightValues(shader, blueLight);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                BeginShaderMode(shader);

                    DrawPlane(Vector3Zero(), (Vector2){ 10.0, 10.0 }, WHITE);
                    DrawCube(Vector3Zero(), 2.0, 4.0, 2.0, WHITE);

                EndShaderMode();

                // Draw the blue sphere where the light is
                DrawSphereEx(blueLight.position, 0.2f, 8, 8, blueLight.color);

                DrawGrid(10, 1.0f);

            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadShader(shader);
    CloseWindow();

    return 0;
}


*/
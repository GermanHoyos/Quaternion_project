//https://x.com/EvangelizerH
/********************
**                 **
** Experiment to   **
** learn OpenGL    **
** and 6dof maths  **
** German A Hoyos  **
**                 **
********************/
#include "include/MasterHeader.h"

int main()
{

// INIT - [WINDOW] + [WORLD] + [CAMERA]
const int screenWidth = 1200; const int screenHeight = 600;
InitWindow(screenWidth, screenHeight, ""); SetTargetFPS(60);
Camera3D camera = { 0 };
camera.position = (Vector3){ 0.0f, 10.0f, 20.0f };  // Camera position
camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
camera.fovy = 45.0f;                                // Camera field-of-view Y
camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

class myPlayer 
   {
   public:
      float x, y, z, dx, dy, dz, rx, ry, rz;
      float speed = 1.0f;
      Vector3 pw1 = {x,y,z};
      Vector3 pw2 = {x,y,z};
      Vector3 pw3 = {x,y,z};
      Quaternion rotation         = QuaternionIdentity();
      Quaternion rotationDelta    = QuaternionIdentity();
      Quaternion collectRotations = QuaternionIdentity();
      Matrix     cubeSpace        = MatrixIdentity();
      myPlayer(float x, float y, float z, float dx = 0.0f, float dy = 0.0f, float dz = 0.0f, float rx = 0.0f, float ry = 0.0f, float rz = 0.0f)
      : x(x), y(y), z(z), dx(dx), dy(dy), dz(dz), rx(rx), ry(ry), rz(rz) {}

      // OPENGL 4X4 IDENTITY MATRIX
      // | 1  0  0  0 |  ->  | m0 m4 m8 m12 |   // m0, m1, m2: First column, representing the x-axis direction vector.
      // | 0  1  0  0 |  ->  | m1 m5 m9 m13 |   // m4, m5, m6: Second column, representing the y-axis direction vector.
      // | 0  0  1  0 |  ->  | m2 m6 m10 m14|   // m8, m9, m10: Third column, representing the z-axis direction vector.
      // | 0  0  0  1 |  ->  | m3 m7 m11 m15|   // m12, m13, m14: Translation components along the x, y, and z axes, respectively.

      // OPENGL + RAYLIB "MATRIX" STRUCT
      // | 1  0  0  10 |  ->  | m0  m4  m8  m12 |    // [12] = x-axis
      // | 0  1  0  0  |  ->  | m1  m5  m9  m13 |    // [13] = y-axis
      // | 0  0  1  0  |  ->  | m2  m6  m10 m14 |    // [14] = z-axis
      // | 0  0  0  1  |  ->  | m3  m7  m11 m15 |    // [15] = always 1, required for matrix multiplication

      // AXIS IN RELATION TO RAYLIB DEFAULT CAMERA POSITION
      // [13][Y] = (+)up     (-)down 
      // [12][X] = (+)right  (-)left 
      // [14][Z] = (-)foward (+)back

      void draw()
      { 
         rlPushMatrix();                                                               // save current state of matrix untill [pop]
         apply_rotation_axis();                                                        // apply quaternion math to [rx] [ry] [rz] based on start point of 0 0 0
         accelerate_foward();                                                          // apply acceleration maths to directional variable [dx] [dy] [dz]
         Quaternion resultRotation = QuaternionMultiply(rotation, collectRotations);   // store above quaternion in a variable
         cubeSpace = QuaternionToMatrix(resultRotation);                               // calculate how to apply quaternion to this current matrix
         cubeSpace.m12 += dx;                                                          // [x] axis acceleration
         cubeSpace.m13 += dy;                                                          // [y] axis acceleration
         cubeSpace.m14 += dz;                                                          // [z] axis acceleration
         rlMultMatrixf(MatrixToFloat(cubeSpace));                                      // apply all above culculations to this current matrix
         DrawCube({0,0,0}, 5.0f, 5.0f, 5.0f, RED); DrawGrid(10, 1.0f);                 // draw object on current matrix
         rlPopMatrix();                                                                // revert back to state before [push]
         collectRotations = QuaternionIdentity();                                      // reset the collectRotations quaternion to 0 0 0
      }
      void rotate_x_axis()
      {
         if (IsKeyDown(KEY_W)) rx += 0.5f;
         if (IsKeyDown(KEY_S)) rx -= 0.5f;
         rotationDelta = QuaternionFromAxisAngle({ 1.0f, 0.0f, 0.0f }, DEG2RAD * rx );
         collectRotations = QuaternionMultiply(collectRotations, rotationDelta);
      }
      void rotate_y_axis()
      {
         if (IsKeyDown(KEY_Q)) ry += 0.5f;
         if (IsKeyDown(KEY_E)) ry -= 0.5f;
         rotationDelta = QuaternionFromAxisAngle({ 0.0f, 1.0f, 0.0f }, DEG2RAD * ry );
         collectRotations = QuaternionMultiply(collectRotations, rotationDelta);
      }
      void rotate_z_axis()
      {
         if (IsKeyDown(KEY_A)) rz += 0.5f;
         if (IsKeyDown(KEY_D)) rz -= 0.5f;
         rotationDelta = QuaternionFromAxisAngle({ 0.0f, 0.0f, 1.0f }, DEG2RAD * rz );
         collectRotations = QuaternionMultiply(collectRotations, rotationDelta);
      }
      void apply_rotation_axis()
      {
         rotate_x_axis(); rotate_y_axis(); rotate_z_axis();
      }
      void accelerate_foward()
      {
         if (IsKeyDown(KEY_SPACE))
         {
            Vector3 fowardDirection = Vector3Transform({0.0f, 0.0f, -1.0f}, QuaternionToMatrix(collectRotations));
            dx += fowardDirection.x;
            dy += fowardDirection.y;
            dz += fowardDirection.z;
         }
      }
      void display_coord_data()
      {
         // print to screen [x] [y] [z] world space coords
      }
   };

   myPlayer player = myPlayer(0.0f,0.0f,0.0f);

   // Main game loop
   while (!WindowShouldClose())    // Detect window close button or ESC key
   {
      BeginDrawing();
      ClearBackground(BLACK);
      BeginMode3D(camera);
      /////////////////////////////////////////////////////////////////////////

      player.draw();

      ////////////////////////////////////////////////////////////////////////
      DrawGrid(10, 1.0f); // world matrix
      EndMode3D();
      DrawFPS(10, 10);
      EndDrawing();
   }

   CloseWindow();  

   return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NOTES
/*

// ROTATE ABOUT 0 0 0 WORLD SPACE DESPITE INREMENTING X POSITION:
void draw() 
{
   rlPushMatrix();
   rlTranslatef(position.x, position.y, position.z);
   Quaternion resultRotation = QuaternionMultiply(rotation, rotationDelta);
   Matrix rotationMatrix = QuaternionToMatrix(resultRotation);
   rlMultMatrixf(MatrixToFloat(rotationMatrix));
   DrawCube({0, 0, 0}, 10.0f, 10.0f, 10.0f, RED);
   rlPopMatrix();
}

// ROTATE ABOUT LOCAL SPACE VIA DIRECT MATRIX MANIPULATE AND DESCRIBING LOCAL SPACE
void draw()
{ 
   pw1.x+=0.01f;
   rlPushMatrix();                                                               // save current state of matrix untill ["pop"]
   apply_rotation_axis();                                                        // apply quaternion math to rx,ry,rz; based on start point of 0 0 0
   Quaternion resultRotation = QuaternionMultiply(rotation, collectRotations);   // store above quaternion in a variable
   cubeSpace = QuaternionToMatrix(resultRotation);                               // calculate how to apply quaternion to this current matrix
   //cubeSpace.m12 += pw1.x;                                                     // calculate how to apply movement to this current matrix [x]
   //cubeSpace.m13 += pw1.x;                                                     // calculate how to apply movement to this current matrix [y]
   //cubeSpace.m14 += pw1.x;                                                     // calculate how to apply movement to this current matrix [z]
   rlMultMatrixf(MatrixToFloat(cubeSpace));                                      // apply all above culculations to this current matrix
   DrawCube({0,0,0}, 5.0f, 5.0f, 5.0f, RED); DrawGrid(10, 1.0f);                 // draw object on current matrix
   rlPopMatrix();                                                                // revert back to state before ["push"]
   collectRotations = QuaternionIdentity();                                      // reset the collectRotations quaternion to 0 0 0
}

























































*/
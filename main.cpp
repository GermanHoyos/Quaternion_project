
//THE SOURCE / WHO WAS, WHO IS, WHO IS COMING: YHWH
//German Adrian Hoyos // "Imagination + Willpower" -> the only two skills that matter.
//https://x.com/EvangelizerH
/********************
**                 **
** Experiment to   **
** learn OpenGL    **
** and 6dof Maths  **
**                 **
********************/
// GAME START: [X][Y][Z] FROM THE PERSPECTIVE OF THE INIT SCREEN START, ZOOM AND ROTATE WITH "P" KEY 
//                                [Y]              [Z] 
//                                **              **
//                                **            **
//                                **          **
//                                **        **
//                                **      **
//                                **    **
//                                **  **
//                                ****
//      ******************************************************[X]
//                              ****
//                            **  **
//                          **    **
//                        **      **
//                      **        **
//                    **          **
//                  **            **
//                **              **
#include "include/MasterHeader.h" // <- collection of standard c++ and custom headers // "pragma once"
int main()
{
// INIT - [WINDOW] + [WORLD] + [CAMERA]
const int screenWidth = 1200; const int screenHeight = 600;
InitWindow(screenWidth, screenHeight, ""); SetTargetFPS(60);
Camera3D camera = { 0 };
Vector3  cam_track = {0.0f, 0.0f, 0.0};
camera.position = (Vector3){ 0.0f, 12.0f, 20.0f };                     // Camera position
camera.target = (Vector3){cam_track.x, cam_track.y, cam_track.z};      // Camera looking at point
camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };                             // Camera up vector (rotation towards target)
camera.fovy = 45.0f;                                                   // Camera field-of-view Y
camera.projection = CAMERA_PERSPECTIVE;                                // Camera mode type

// ACTOR
class myPlayer 
   {
   public:
   // MODEL + BLENDER BAKE
   Model ship = LoadModel("C:\\Users\\Hoyos\\Desktop\\C++ Runner\\raylib_quaternion_example\\home\\src\\include\\ship.obj");
   Texture2D texture = LoadTexture("C:\\Users\\Hoyos\\Desktop\\C++ Runner\\raylib_quaternion_example\\home\\src\\include\\t1.png");
   Vector3 ship_init_pos = {0.0f, 0.0f, 0.0f};
   float    x, y, z, dx, dy, dz, rx, ry, rz;
   float speed1 = 1.0f; float speed2 = 0.5f;
   float speed3 = 0.1f; float thrus1 = 0.0f;
   float thrus2 = 0.2f; float thrus3 = 3.4f;
   float radis1 = 0.3f; float radis2 = 0.0f;
   int cR = 255; int cG     = 255; 
   int cB = 255; int cAlpha = 255; 
   Color      myRGB = {255,255,255,255};
   Quaternion rotation         = QuaternionIdentity();
   Quaternion rotationDelta    = QuaternionIdentity();
   Quaternion collectRotations = QuaternionIdentity();
   Matrix     cubeSpace        = MatrixIdentity();
   bool       textureLoaded    = false;
   myPlayer(float x, float y, float z, float dx = 0.0f, float dy = 0.0f, float dz = 0.0f, float rx = 0.0f, float ry = 0.0f, float rz = 0.0f)
   : x(x), y(y), z(z), dx(dx), dy(dy), dz(dz), rx(rx), ry(ry), rz(rz) {}
   ////////////////////////////////////////////////////////////////////////////////////
   // OPENGL 4X4 IDENTITY MATRIX
   // | 1  0  0  0 |  ->  | m0 m4 m8 m12 |   // m0, m1, m2: First column, representing the x-axis direction vector.
   // | 0  1  0  0 |  ->  | m1 m5 m9 m13 |   // m4, m5, m6: Second column, representing the y-axis direction vector.
   // | 0  0  1  0 |  ->  | m2 m6 m10 m14|   // m8, m9, m10: Third column, representing the z-axis direction vector.
   // | 0  0  0  1 |  ->  | m3 m7 m11 m15|   // m12, m13, m14: Translation components along the x, y, and z axes, respectively.
   ////////////////////////////////////////////////////////////////////////////////////
   // OPENGL + RAYLIB "MATRIX" STRUCT
   // | 1  0  0  10 |  ->  | m0  m4  m8  m12 |    // [12] = x-axis
   // | 0  1  0  0  |  ->  | m1  m5  m9  m13 |    // [13] = y-axis
   // | 0  0  1  0  |  ->  | m2  m6  m10 m14 |    // [14] = z-axis
   // | 0  0  0  1  |  ->  | m3  m7  m11 m15 |    // [15] = always 1, required for matrix multiplication
   ////////////////////////////////////////////////////////////////////////////////////
   // AXIS IN RELATION TO RAYLIB DEFAULT CAMERA POSITION
   // [13][Y] = (+)up     (-)down
   // [12][X] = (+)right  (-)left
   // [14][Z] = (-)foward (+)back
   ////////////////////////////////////////////////////////////////////////////////////
      void draw()
      { 
        // RANDOM NUMS
        /*sphere color*/random_device rd_1; mt19937 gen_1(rd_1()); uniform_int_distribution<int>    dis_1(1, 255);      cG = dis_1(gen_1); cB = dis_1(gen_1); cAlpha = dis_1(gen_1);
        /*sphere xy[Z]*/random_device rd_2; mt19937 gen_2(rd_2()); uniform_real_distribution<float> dis_2(-0.1f, 0.1f); thrus1 = thrus1 + dis_2(gen_2);
        /*sphere x[Y]z*/random_device rd_3; mt19937 gen_3(rd_3()); uniform_real_distribution<float> dis_3(-0.1f, 0.2f); thrus2 = thrus2 + dis_3(gen_2);
        /*sphere [X]yz*/random_device rd_4; mt19937 gen_4(rd_4()); uniform_real_distribution<float> dis_4(-1.0f, 1.0f);
        /*sphere radus*/if (IsKeyDown(KEY_SPACE)) { if(radis1 < 0.5f){radis1 += 0.009f;}} else {radis1 = 0.01f;} 

         // PUSH MATRIX // GEN LOCAL SPACE MATRIX
            loadTexture();
            rlPushMatrix();                                                               // save current state of matrix untill [pop]
            apply_rotation_axis();                                                        // apply quaternion math to [rx] [ry] [rz] based on start point of 0 0 0
            accelerate_foward();                                                          // apply acceleration maths to directional variable [dx] [dy] [dz]
            Quaternion resultRotation = QuaternionMultiply(rotation, collectRotations);   // store above quaternion in a variable
            cubeSpace = QuaternionToMatrix(resultRotation);                               // calculate how to apply quaternion to this current matrix
            cubeSpace.m12 += dx;                                                          // [x] axis acceleration
            cubeSpace.m13 += dy;                                                          // [y] axis acceleration
            cubeSpace.m14 += dz;                                                          // [z] axis acceleration
            rlMultMatrixf(MatrixToFloat(cubeSpace));                                      // apply all above culculations to this current matrix
            DrawModel(ship, ship_init_pos, 3.0f,  WHITE);                                 // blender made model
            DrawSphere({thrus1,thrus2,thrus3}, radis1, {cR,cG,cB,cAlpha});
            DrawGrid(5, 1.0f); // shifted matrix

            rlPopMatrix();                                                                // revert back to state before [push]
         // POP MATRIX // REVERT BACK TO WORLD SPACE MATRIX

         // RESETS
         rotationDelta    = QuaternionIdentity();
         collectRotations = QuaternionIdentity();                                      // reset the collectRotations quaternion to 0 0 0
         rotation         = QuaternionIdentity();
         thrus1 = 0.0f; thrus2 = 0.0f;

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
         // if (IsKeyDown(KEY_Q)) ry += 0.5f;
         // if (IsKeyDown(KEY_E)) ry -= 0.5f;
         // rotationDelta = QuaternionFromAxisAngle({ 0.0f, 1.0f, 0.0f }, DEG2RAD * ry );
         // collectRotations = QuaternionMultiply(collectRotations, rotationDelta);
      }
      void rotate_z_axis()
      {
         if (IsKeyDown(KEY_A)) rz -= 0.5f;
         if (IsKeyDown(KEY_D)) rz += 0.5f;
         rotationDelta = QuaternionFromAxisAngle({ 0.0f, -1.0f, 0.0f }, DEG2RAD * rz );
         collectRotations = QuaternionMultiply(collectRotations, rotationDelta);
      }
      void apply_rotation_axis()
      {
         rotate_z_axis(); rotate_x_axis(); rotate_y_axis(); 
      }
      void accelerate_foward()
      {
         if (IsKeyDown(KEY_SPACE))
         {
            Vector3 fowardDirection = Vector3Transform({0.0f, 0.0f, -1.0f}, QuaternionToMatrix(collectRotations));
            dx += fowardDirection.x * speed3; 
            dy += fowardDirection.y * speed3; 
            dz += fowardDirection.z * speed3; 
         }
      }
      void display_coord_data()
      {
         // print to screen [x] [y] [z] world space coords
      }
      void loadTexture()
      {
         if(!textureLoaded){ ship.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; textureLoaded = true;};
      }
   };

   //INSTANTIATE ACTOR
   myPlayer player = myPlayer(0.0f,0.0f,0.0f);

   //OSCILLATER INITS
   float hover = 0.0f;
   float time = 0.0f;
   float amplitude = 2.0f;
   float frequency = 1.0f;

   // Main game loop
   while (!WindowShouldClose())    // Detect window close button or ESC key
   {
      if (IsKeyDown(KEY_P)) UpdateCamera(&camera, CAMERA_THIRD_PERSON);
      BeginDrawing();
      ClearBackground({ 0, 37, 51, 255 });
      BeginMode3D(camera);
      rlDisableBackfaceCulling();
      rlPushMatrix();
        // Calculate the camera's position relative to the ship's transformation
        Vector3 cameraOffset = {0.0f, 0.0f, 20.0f};  // 20 units behind the ship
        Vector3 transformedCameraOffset = Vector3Transform(cameraOffset, player.cubeSpace);
        camera.position = transformedCameraOffset;
      rlPopMatrix();
      camera.target = (Vector3){player.cubeSpace.m12, player.cubeSpace.m13, player.cubeSpace.m14};
      /////////////////////////////JUICY////////////////////////////////////////(+)

      player.draw();

      /////////////////////////////BITS/////////////////////////////////////////(-)
      rlPushMatrix();
      rlTranslatef(0.0f,-20.0f,20.0f);
      DrawGrid(100, 1.0f); // shifted matrix
      rlPopMatrix();
      
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

// SHAPES
   DrawCube((Vector3){-4.0f, 0.0f, 2.0f}, 2.0f, 5.0f, 2.0f, RED);
   DrawCubeWires((Vector3){-4.0f, 0.0f, 2.0f}, 2.0f, 5.0f, 2.0f, GOLD);
   DrawCubeWires((Vector3){-4.0f, 0.0f, -2.0f}, 3.0f, 6.0f, 2.0f, MAROON);

   DrawSphere((Vector3){-1.0f, 0.0f, -2.0f}, 1.0f, GREEN);
   DrawSphereWires((Vector3){1.0f, 0.0f, 2.0f}, 2.0f, 16, 16, LIME);

   DrawCylinder((Vector3){4.0f, 0.0f, -2.0f}, 1.0f, 2.0f, 3.0f, 4, SKYBLUE);
   DrawCylinderWires((Vector3){4.0f, 0.0f, -2.0f}, 1.0f, 2.0f, 3.0f, 4, DARKBLUE);
   DrawCylinderWires((Vector3){4.5f, -1.0f, 2.0f}, 1.0f, 1.0f, 2.0f, 6, BROWN);

   DrawCylinder((Vector3){1.0f, 0.0f, -4.0f}, 0.0f, 1.5f, 3.0f, 8, GOLD);
   DrawCylinderWires((Vector3){1.0f, 0.0f, -4.0f}, 0.0f, 1.5f, 3.0f, 8, PINK);

   DrawCapsule     ((Vector3){-3.0f, 1.5f, -4.0f}, (Vector3){-4.0f, -1.0f, -4.0f}, 1.2f, 8, 8, VIOLET);
   DrawCapsuleWires((Vector3){-3.0f, 1.5f, -4.0f}, (Vector3){-4.0f, -1.0f, -4.0f}, 1.2f, 8, 8, PURPLE);


*/
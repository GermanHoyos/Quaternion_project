
// THE SOURCE / WHO WAS, WHO IS, WHO IS COMING: YHWH
// German Adrian Hoyos // "Imagination + Willpower" -> the only two skills that matter. //https://x.com/EvangelizerH
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


#include "include/masterHeader.h" // <- collection of standard c++ and custom headers // "pragma once"


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
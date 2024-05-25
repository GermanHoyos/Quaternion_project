
// THE SOURCE - WHO WAS, WHO IS, WHO IS COMING: YHWH
// GERMAN HOYOS - IMAGINATION AND WILLPOWER ARE THE ONLY TWO CODING SKILLS THAT MATTER. TWITTER||X: //https://x.com/EvangelizerH

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
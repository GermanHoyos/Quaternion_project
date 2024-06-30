
// THE  SOURCE   - WHO WAS, WHO IS, WHO IS COMING: YHWH
//
// GERMAN HOYOS - IMAGINATION AND WILLPOWER ARE THE ONLY TWO CODING SKILLS THAT MATTER. TWITTER||X: //https://x.com/EvangelizerH
//
// GAME START   - [X][Y][Z]: FROM THE PERSPECTIVE OF THE INIT SCREEN START, ZOOM AND ROTATE WITH "P" KEY 
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
//
// OPENGL 4X4 IDENTITY MATRIX
// | 1  0  0  0 |  ->  | m0 m4 m8 m12 |   // m0, m1, m2: First column, representing the x-axis direction vector.
// | 0  1  0  0 |  ->  | m1 m5 m9 m13 |   // m4, m5, m6: Second column, representing the y-axis direction vector.
// | 0  0  1  0 |  ->  | m2 m6 m10 m14|   // m8, m9, m10: Third column, representing the z-axis direction vector.
// | 0  0  0  1 |  ->  | m3 m7 m11 m15|   // m12, m13, m14: Translation components along the x, y, and z axes, respectively.
//
// OPENGL + RAYLIB "MATRIX" STRUCT
// | 1  0  0  10 |  ->  | m0  m4  m8  m12 |    // [12] = x-axis
// | 0  1  0  0  |  ->  | m1  m5  m9  m13 |    // [13] = y-axis
// | 0  0  1  0  |  ->  | m2  m6  m10 m14 |    // [14] = z-axis
// | 0  0  0  1  |  ->  | m3  m7  m11 m15 |    // [15] = always 1, required for matrix multiplication
//
// AXIS IN RELATION TO RAYLIB DEFAULT CAMERA POSITION
// [13][Y] = (+)up     (-)down
// [12][X] = (+)right  (-)left
// [14][Z] = (-)foward (+)back

#include "include/masterHeader.h" // <- collection of standard c++ and custom headers // "pragma once"
#define XBOX360_LEGACY_NAME_ID "Xbox Controller"
#define XBOX360_NAME_ID "Xbox 360 Controller"
#define PS3_NAME_ID "PLAYSTATION(R)3 Controller"

// struct gameState 
// {
//    // INIT - [WINDOW] + [WORLD] + [CAMERA]
//    // const int screenWidth = 1200; const int screenHeight = 600;
//    // InitWindow(screenWidth, screenHeight, ""); SetTargetFPS(60);
//    // Camera3D camera = { 0 };
//    // Vector3  cam_track = {0.0f, 0.0f, 0.0};
//    // camera.position = (Vector3){ 0.0f, 12.0f, 20.0f };                     // Camera position
//    // camera.target = (Vector3){cam_track.x, cam_track.y, cam_track.z};      // Camera looking at point
//    // camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };                             // Camera up vector (rotation towards target)
//    // camera.fovy = 45.0f;                                                   // Camera field-of-view Y
//    // camera.projection = CAMERA_PERSPECTIVE;                                // Camera mode type

// };
// gameState g; //Access with "g."

// void idle(float dt) // Almost always has a time param since the last call
// {
//    // Seconds
//    const float dt = 1.0f / 60.0f;

//    // Input devices - handle all player movement
//    player.idle(); // <- pass the dt to the player
// };

// void draw()
// {
//    // all draw code goes in here:
//    player.draw();
//    // and any other objects instantiated

// };


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

   // LOAD GAME OBJECTS
   myPlayer player = myPlayer(0.0f,0.0f,0.0f); // Create user controlled actor
   createField();                              // Create space particle field
   asteroids myAsteroids = asteroids({-10.0f,0.0f,-20.0f});
   asteroids myAsteroids_2 = asteroids({0.0f,0.0f,-20.0f});
   asteroids myAsteroids_3 = asteroids({10.0f,0.0f,-20.0f});



   //OSCILLATER INITS
   float hover = 0.0f;
   float time = 0.0f;
   float amplitude = 2.0f;
   float frequency = 1.0f;

   // Main game loop
   while (!WindowShouldClose())    // Detect window close button or ESC key
   {
      // remove test camera when ready
      //if (IsKeyDown(KEY_P)) UpdateCamera(&camera, CAMERA_THIRD_PERSON);
      // idle();
      // draw();

      
      BeginDrawing();
      //ClearBackground({ 0, 37, 51, 255 });
      ClearBackground(BLACK);

      BeginMode3D(camera);
      rlDisableBackfaceCulling();
      rlPushMatrix();
      // Calculate the camera's position relative to the ship's transformation
      Vector3 cameraOffset = {0.0f, 5.0f, 20.0f};  // 20 units behind the ship
      Vector3 transformedCameraOffset = Vector3Transform(cameraOffset, player.cubeSpace);
      camera.position = transformedCameraOffset;
      rlPopMatrix();
      camera.target = (Vector3){player.cubeSpace.m12, player.cubeSpace.m13, player.cubeSpace.m14};
      /////////////////////////////JUICY////////////////////////////////////////(+)

      player.draw();
      myAsteroids.draw();
      myAsteroids_2.draw();
      myAsteroids_3.draw();


      // Draw all lasers && particles
      for (auto& laser : lasersList)
      {
         laser.draw();
         myAsteroids.detectCollisions(laser);
         myAsteroids_2.detectCollisions(laser);
         myAsteroids_3.detectCollisions(laser);

      }
      //for (auto& particle : pField){particle.draw();}

      // Test game area
      // big white cube
      //DrawCube({0,0,-50}, 20, 20, 20, {255, 255, 255, 255});
      makeAsteroids();
      // Instantiate asteroids


      /////////////////////////////BITS/////////////////////////////////////////(-)
      rlPushMatrix();
      rlTranslatef(0.0f,-20.0f,0.0f);
      DrawGrid(100, 1.0f); // shifted matrix
      rlPopMatrix();
      
      EndMode3D();
      // Debugs
      //DrawFPS(10, 10);
      string player_xyz = "Ship position in realation to world space:\nX = " + to_string(playerPosition.x) + "\nY = " +  to_string(playerPosition.y) + "\nZ = " + to_string(playerPosition.z);
      DrawText (player_xyz.c_str(), 2, 2, 10, GREEN);
      string player_rot = "Ship quaternion derived vector:\nX = " + to_string(playerRotation.x) + "\nY = " + to_string(playerRotation.y) + "\nZ = " + to_string(playerRotation.z);
      DrawText (player_rot.c_str(), 2, 60, 10, GREEN);


      EndDrawing();
   }

   CloseWindow();  

   return 0;
}
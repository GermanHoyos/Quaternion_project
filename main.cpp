
// THE  SOURCE   - WHO WAS, WHO IS, WHO IS COMING: YHWH
//
// Run via vscode: ./runme.bat
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

// Propogation waves across tile sheet
// Particle noise using 
   // fluid wave mechanics which for games 
   // fluid dynamics

// #define RLGL_IMPLEMENTATION
// #include <rlgl.h>

#include "include/masterHeader.h" // <- collection of standard c++ and custom headers // "pragma once"
#define XBOX360_LEGACY_NAME_ID "Xbox Controller"
#define XBOX360_NAME_ID "Xbox 360 Controller"
#define PS3_NAME_ID "PLAYSTATION(R)3 Controller"

int main()
{
   // Raylibs camera and 3d world inits
   //const int screenWidth = 1600; const int screenHeight = 800;
   const int screenWidth = 1200; const int screenHeight = 600;
   Quaternion myQuat = QuaternionIdentity();
   InitWindow(screenWidth, screenHeight, ""); SetTargetFPS(60);

   //Camera3D camera = { 0 }; // Declared in master header
   Vector3  cam_track = {0.0f, 0.0f, 0.0};
   camera.position = (Vector3){ 0.0f, 12.0f, 20.0f };                     // Camera position
   camera.target = (Vector3){cam_track.x, cam_track.y, cam_track.z};      // Camera looking at point
   camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };                             // Camera up vector (rotation towards target)
   camera.fovy = 45.0f;                                                   // Camera field-of-view Y
   camera.projection = CAMERA_PERSPECTIVE;                                // Camera mode type

   // Instantiate actors
   myPlayer  player = myPlayer(0.0f,0.0f,0.0f); // Create user controlled actor
   asteroids myAsteroids = asteroids({-10.0f,0.0f,-20.0f});
   asteroids myAsteroids_2 = asteroids({0.0f,0.0f,-20.0f});
   asteroids myAsteroids_3 = asteroids({10.0f,0.0f,-20.0f});

   const int rows = 100;
   const int cols = 100;
   const float spacing = 4.5f;
   float startX = 22.0f;
   float startY = -22.0f;
   float startZ = -20.0f;
   // Vector to hold the panels
   std::vector<panel> panelList;

   for (int row = 0; row < rows; ++row)
   {
      for (int col = 0; col < cols; ++col)
      {
         float x = col * spacing;
         float y = row * spacing;

         // Create a new panel at (x, y, 0.0f) and add to the list
         panel newPanel = panel({x + startX, y + startY, 0.0f + startZ});
         panelList.push_back(newPanel);
      }
   }

   // Oscillator inits
   float hover = 0.0f;
   float time = 0.0f;
   float amplitude = 2.0f;
   float frequency = 1.0f;

   // Special effects inits
   Vector3 positionSnapshot = playerPosition;
   bool gameStarted = false;

   // Axis visualization inits
   Texture2D texture_x = LoadTexture("./src/assets/x_axis.png");
   Texture2D texture_y = LoadTexture("./src/assets/y_axis.png");
   Texture2D texture_z = LoadTexture("./src/assets/z_axis.png");

   //test ship colors
   Color      leftWingLines    = {0,0,255,255};
   Color      rightWingLines   = {0,0,255,255};
   Color      leftEngLines     = {0,0,255,255};
   Color      rightEngLines    = {0,0,255,255};
   Color      centerLines      = {0,0,255,255};
   Model      _001             = LoadModel("./src/assets/001.obj");
   Model      _002             = LoadModel("./src/assets/002.obj");
   Model      _003             = LoadModel("./src/assets/003.obj");
   Model      _004             = LoadModel("./src/assets/004.obj");
   Model      _005             = LoadModel("./src/assets/005.obj");

   // Reflective cube inits
   reflectiveCube refCube_plane = reflectiveCube({-25.0f, 20.0f, -20.0f}, false, true, false, true);
   reflectiveCube refCube_all_Normals = reflectiveCube({-5.0f, 20.0f, -20.0f}, false, true, true, true);
   reflectiveCube refCube_all_skin = reflectiveCube({-15.0f, 40.0f, -40.0f}, true, false, false, true);

   // terrainVert TEST
   createTerrain();

   // terrainMesh TEST
   createMeshGrid();

   /*****************
   **              **
   **              **
   ** MAIN LOOP    **
   **              **
   **              **
   *****************/
   while (!WindowShouldClose())    // Detect window close button or ESC key
   {

      // Call raylibs drawing context      
      BeginDrawing();

      // ClearBackground({ 0, 37, 51, 255 });
      ClearBackground(BLACK);

      BeginMode3D(camera);
      rlDisableBackfaceCulling();
      rlPushMatrix();

      // Calculate the camera's position relative to the ship's transformation
      Vector3 cameraOffset = {0.0f, 5.0f, 20.0f};  // 20 units behind the ship
      Vector3 transformedCameraOffset = Vector3Transform(cameraOffset, player.cubeSpace);
      camera.position = transformedCameraOffset;
      camera.target = (Vector3){player.cubeSpace.m12, player.cubeSpace.m13, player.cubeSpace.m14};
      rlPopMatrix();

      /////////////////////////////JUICY////////////////////////////////////////(+)
      // Instantiate actors
      player.draw();
      myAsteroids.draw();
      myAsteroids_2.draw();
      myAsteroids_3.draw();

      // Instantiate planes with wave functions
      for (auto& panel : panelList)
      {
         panel.draw();
      }
      
      // Create initial particle field only once
      if (!gameStarted)
      {
         createFieldOne();
         positionSnapshot = playerPosition;
         gameStarted = true;
      }

      // Get the distance of the ship from a snapshot of its vector
      // if the distance is > than X then delete the particle field
      // and create a new one
      Vector3 distanceFromSnapshot = Vector3Subtract(positionSnapshot, playerPosition);
      float derivedDistance = Vector3Length(distanceFromSnapshot);
      if (derivedDistance > 1000)
      {
         createFieldOne();
         positionSnapshot = playerPosition;
      }

      // Draw all lasers && particles
      for (auto& laser : lasersList)
      {
         laser.draw();
         myAsteroids.detectCollisions(laser);
         myAsteroids_2.detectCollisions(laser);
         myAsteroids_3.detectCollisions(laser);
      }

      // If a particle exists, draw it
      if (!pField.empty()) 
      {
         for (auto& particle : pField)
         {
            particle.draw();
         }
      }

      // If wave exists draw it
      for (auto& wave : waveList)
      {
         wave.draw();
      }

      // Asteroids (currently represented by GREEN 3d wire spheres)
      makeAsteroids();

      // Axis visualizations
      DrawLine3D({-30.0f,0.0f,-20.0f},{-20.0f,0.0f,-20.0f},RED);
      DrawBillboard(camera, texture_x, {-18.0f,0.0f,-20.0f}, 2, {255, 255, 255, 255});
      DrawLine3D({-30.0f,0.0f,-20.0f},{-30.0f,10.0f,-20.0f},GREEN);
      DrawBillboard(camera, texture_y, {-30.0f,12.0f,-20.0f}, 2, {255, 255, 255, 255});
      DrawLine3D({-30.0f,0.0f,-20.0f},{-30.0f,0.0f,-30.0f},BLUE);
      DrawBillboard(camera, texture_z, {-30.0f,0.0f,-32.0f}, 2, {255, 255, 255, 255});

      // Draw reflective cube
      for(auto& cube : cubeList)
      {
         cube.draw();
      }
      refCube_plane.draw();
      refCube_all_Normals.draw();
      refCube_all_skin.draw();

      // Draw arenas
      drawStrip();

      // Draw explosions
      for(auto& xplode : explosionsList)
      {
         xplode.draw();
      }

      // Draw "terrainVerts"
      for(auto& terrainVert : tField)
      {
         terrainVert.draw();
      }
      //Center Sphere
      moveTerrain();

      // Draw "terrainMesh"
      for(auto& terrainMesh : meshList)
      {
         terrainMesh.draw();
      }
      DrawSphere({380.0f, 0.0f, 0.0f }, 2, WHITE);
      DrawSphere({380.0f, 0.0f, -1900.0f }, 2, WHITE);



      // /***************
      // **            **
      // ** CONCEPT 1  **
      // **            **
      // ***************/
      DrawTriangle3D( // Counter Clockwise
         (Vector3){10.0f, 0.0f, 0.0f},   // RED   
         (Vector3){20.0f, 0.0f, 0.0f}, // GREEN
         (Vector3){15.0f, 0.0f, -10.0f},  // BLUE
         WHITE
      );
      DrawSphere((Vector3){10.0f,0.0f,0.0f},  0.2f,RED);
      DrawSphere((Vector3){20.0f,0.0f,0.0f},  0.2f,GREEN);
      DrawSphere((Vector3){15.0f,0.0f,-10.0f},0.2f,BLUE);

      // /***************
      // **            **
      // ** CONCEPT 2  **
      // **            **
      // ***************/
      Vector3 vRed_2_a   = {-20.0f,0.0f,0.0f};
      Vector3 vGreen_2_a = {-10.0f,0.0f,0.0f};
      Vector3 vBlue_2_a  = {-10.0f,0.0f,-10.0f};
      DrawTriangle3D( // Counter Clockwise
         vRed_2_a,    // RED   
         vGreen_2_a,  // GREEN
         vBlue_2_a,   // BLUE
         WHITE
      );
      DrawSphere(vRed_2_a,   0.2f, RED);
      DrawSphere(vGreen_2_a, 0.2f, GREEN);
      DrawSphere(vBlue_2_a,  0.2f, BLUE);


      Vector3 vRed_2_b   = {-21.0f,0.0f,0.0f};
      Vector3 vGreen_2_b = {-11.0f,0.0f,-10.0f};
      Vector3 vBlue_2_b  = {-21.0f,0.0f,-10.0f};
      DrawTriangle3D( // Counter Clockwise
         vRed_2_b,    // RED   
         vGreen_2_b,  // GREEN
         vBlue_2_b,   // BLUE
         WHITE
      );
      DrawSphere(vRed_2_b,   0.2f, RED);
      DrawSphere(vGreen_2_b, 0.2f, GREEN);
      DrawSphere(vBlue_2_b,  0.2f, BLUE);

      // /***************
      // **            **
      // ** CONCEPT 3  **
      // **            **
      // ***************/
      Vector3 vRed_3_a   = {-120.0f,0.0f,0.0f};
      Vector3 vGreen_3_a = {-60.0f,0.0f,0.0f};
      Vector3 vBlue_3_a  = {-60.0f,0.0f,-30.0f};
      DrawTriangle3D( // Counter Clockwise
         vRed_3_a,    // RED   
         vGreen_3_a,  // GREEN
         vBlue_3_a,   // BLUE
         GRAY
      );
      DrawSphere(vRed_3_a,   0.2f, RED);
      DrawSphere(vGreen_3_a, 0.2f, RED);
      DrawSphere(vBlue_3_a,  0.2f, RED);

      Vector3 vRed_3_b   = {-120.0f,0.0f,0.0f};
      Vector3 vGreen_3_b = {-60.0f,0.0f,-30.0f};
      Vector3 vBlue_3_b  =  {-120.0f,0.0f,-30.0f};
      DrawTriangle3D( // Counter Clockwise
         vRed_3_b,    // RED   
         vGreen_3_b,  // GREEN
         vBlue_3_b,   // BLUE
         WHITE
      );
      DrawSphere(vRed_3_b,   0.2f, RED);
      DrawSphere(vGreen_3_b, 0.2f, RED);
      DrawSphere(vBlue_3_b,  0.2f, RED);


      rlPushMatrix();
      rlTranslatef(0.0f,-20.0f,0.0f);
      //DrawGrid(100, 1.0f); // shifted matrix
      rlPopMatrix();
      
      // /***************
      // **            **
      // ** TERRAIN 1  ** // 5 faces _/-\_
      // **            **
      // ***************/
      //
      //        [B]
      //         ^
      //         |
      // [R] -> [G] 
      //
      // Face1 (A,B)*******************************
      // [CENTER] [A]
      Vector3 mT_a = {-200.0f,  000.00f,  000.0f};  // my translate center position
      DrawSphere(mT_a, 1.0f, PURPLE); 
      // [A]
      Vector3 tR_a = {-10.00f, 00.00f, 10.00f}; // terrain shape red vector
      Vector3 tG_a = { 10.00f, 00.00f, 10.00f}; // terrain shape green vector
      Vector3 tB_a = { 10.00f, 00.00f,-10.00f}; // terrain shape blue vector
      Vector3 fR_a = Vector3Add(mT_a, tR_a);    // final red in world space
      Vector3 fG_a = Vector3Add(mT_a, tG_a);    // final green in world space
      Vector3 fB_a = Vector3Add(mT_a, tB_a);    // final blue in world space
      DrawTriangle3D(fR_a, fG_a, fB_a, DARKGRAY);
      DrawSphere(fR_a, 0.2f, RED);
      DrawSphere(fG_a, 0.2f, RED);
      DrawSphere(fB_a, 0.2f, RED);
      // [B]
      Vector3 tR_b = {-10.00f, 00.00f, 10.00f}; // terrain shape red vector
      Vector3 tG_b = { 10.00f, 00.00f,-10.00f}; // terrain shape green vector
      Vector3 tB_b = {-10.00f, 00.00f,-10.00f}; // terrain shape blue vector
      Vector3 fR_b = Vector3Add(mT_a, tR_b);    // final red in world space
      Vector3 fG_b = Vector3Add(mT_a, tG_b);    // final green in world space
      Vector3 fB_b = Vector3Add(mT_a, tB_b);    // final blue in world space
      DrawTriangle3D(fR_b, fG_b, fB_b, WHITE);
      DrawSphere(fR_b, 0.2f, RED);
      DrawSphere(fG_b, 0.2f, RED);
      DrawSphere(fB_b, 0.2f, RED);
      // Face2 (C,D)*******************************
      // [CENTER] [B]
      Vector3 mT_b   = {-200.0f, 000.00f, -020.00f};  // my translate center position
      Vector3 mT_b_v = {-200.0f, 005.00f, -020.00f};  // my translate visual center
      DrawSphere(mT_b, 1.0f, PURPLE);
      DrawSphere(mT_b_v, 1.0f, RED); 
      // [A]
      Vector3 tR_c = {-10.00f, 00.00f, 10.00f}; // terrain shape red vector
      Vector3 tG_c = { 10.00f, 00.00f, 10.00f}; // terrain shape green vector
      Vector3 tB_c = { 10.00f, 10.00f,-10.00f}; // terrain shape blue vector
      Vector3 fR_c = Vector3Add(mT_b, tR_c);    // final red in world space
      Vector3 fG_c = Vector3Add(mT_b, tG_c);    // final green in world space
      Vector3 fB_c = Vector3Add(mT_b, tB_c);    // final blue in world space
      DrawTriangle3D(fR_c, fG_c, fB_c, DARKGRAY);
      DrawSphere(fR_c, 0.2f, RED);
      DrawSphere(fG_c, 0.2f, RED);
      DrawSphere(fB_c, 0.2f, RED);
      // [B]
      Vector3 tR_d = {-10.00f, 00.00f, 10.00f}; // terrain shape red vector
      Vector3 tG_d = { 10.00f, 10.00f,-10.00f}; // terrain shape green vector
      Vector3 tB_d = {-10.00f, 10.00f,-10.00f}; // terrain shape blue vector
      Vector3 fR_d = Vector3Add(mT_b, tR_d);    // final red in world space
      Vector3 fG_d = Vector3Add(mT_b, tG_d);    // final green in world space
      Vector3 fB_d = Vector3Add(mT_b, tB_d);    // final blue in world space
      DrawTriangle3D(fR_d, fG_d, fB_d, WHITE);
      DrawSphere(fR_d, 0.2f, RED);
      DrawSphere(fG_d, 0.2f, RED);
      DrawSphere(fB_d, 0.2f, RED);
      // Face3(E,F)*******************************
      // [CENTER] [C]
      Vector3 mT_c   = {-200.0f, 000.00f, -040.00f};  // my translate center position
      Vector3 mT_c_v = {-200.0f, 010.00f, -040.00f};  // my translate visual center
      DrawSphere(mT_c, 1.0f, PURPLE);
      DrawSphere(mT_c_v, 1.0f, GREEN); 
      // [A]
      Vector3 tR_e = {-10.00f, 10.00f, 10.00f}; // terrain shape red vector
      Vector3 tG_e = { 10.00f, 10.00f, 10.00f}; // terrain shape green vector
      Vector3 tB_e = { 10.00f, 10.00f,-10.00f}; // terrain shape blue vector
      Vector3 fR_e = Vector3Add(mT_c, tR_e);    // final red in world space
      Vector3 fG_e = Vector3Add(mT_c, tG_e);    // final green in world space
      Vector3 fB_e = Vector3Add(mT_c, tB_e);    // final blue in world space
      DrawTriangle3D(fR_e, fG_e, fB_e, DARKGRAY);
      DrawSphere(fR_e, 0.2f, RED);
      DrawSphere(fG_e, 0.2f, RED);
      DrawSphere(fB_e, 0.2f, RED);
      // [B]
      Vector3 tR_f = {-10.00f, 10.00f, 10.00f}; // terrain shape red vector
      Vector3 tG_f = { 10.00f, 10.00f,-10.00f}; // terrain shape green vector
      Vector3 tB_f = {-10.00f, 10.00f,-10.00f}; // terrain shape blue vector
      Vector3 fR_f = Vector3Add(mT_c, tR_f);    // final red in world space
      Vector3 fG_f = Vector3Add(mT_c, tG_f);    // final green in world space
      Vector3 fB_f = Vector3Add(mT_c, tB_f);    // final blue in world space
      DrawTriangle3D(fR_f, fG_f, fB_f, WHITE);
      DrawSphere(fR_f, 0.2f, RED);
      DrawSphere(fG_f, 0.2f, RED);
      DrawSphere(fB_f, 0.2f, RED);
      // Face4(G,H)*******************************
      // [CENTER] [D]
      Vector3 mT_d   = {-200.0f, 000.00f, -060.00f};  // my translate center position
      Vector3 mT_d_v = {-200.0f, 005.00f, -060.00f};  // my translate visual center
      DrawSphere(mT_d, 1.0f, PURPLE);
      DrawSphere(mT_d_v, 1.0f, BLUE); 
      // [A]
      Vector3 tR_g = {-10.00f, 10.00f, 10.00f}; // terrain shape red vector
      Vector3 tG_g = { 10.00f, 10.00f, 10.00f}; // terrain shape green vector
      Vector3 tB_g = { 10.00f, 00.00f,-10.00f}; // terrain shape blue vector
      Vector3 fR_g = Vector3Add(mT_d, tR_g);    // final red in world space
      Vector3 fG_g = Vector3Add(mT_d, tG_g);    // final green in world space
      Vector3 fB_g = Vector3Add(mT_d, tB_g);    // final blue in world space
      DrawTriangle3D(fR_g, fG_g, fB_g, DARKGRAY);
      DrawSphere(fR_g, 0.2f, RED);
      DrawSphere(fG_g, 0.2f, RED);
      DrawSphere(fB_g, 0.2f, RED);
      // [B]
      Vector3 tR_h = {-10.00f, 10.00f, 10.00f}; // terrain shape red vector
      Vector3 tG_h = { 10.00f, 00.00f,-10.00f}; // terrain shape green vector
      Vector3 tB_h = {-10.00f, 00.00f,-10.00f}; // terrain shape blue vector
      Vector3 fR_h = Vector3Add(mT_d, tR_h);    // final red in world space
      Vector3 fG_h = Vector3Add(mT_d, tG_h);    // final green in world space
      Vector3 fB_h = Vector3Add(mT_d, tB_h);    // final blue in world space
      DrawTriangle3D(fR_h, fG_h, fB_h, WHITE);
      DrawSphere(fR_h, 0.2f, RED);
      DrawSphere(fG_h, 0.2f, RED);
      DrawSphere(fB_h, 0.2f, RED);
      // Face5(I,J)*******************************
      // [CENTER] [E]
      // [A]
      Vector3 mT_e   = {-200.0f, 000.00f, -080.00f};  // my translate center position
      DrawSphere(mT_e, 1.0f, PURPLE);
      Vector3 tR_i = {-10.00f, 00.00f, 10.00f}; // terrain shape red vector
      Vector3 tG_i = { 10.00f, 00.00f, 10.00f}; // terrain shape green vector
      Vector3 tB_i = { 10.00f, 00.00f,-10.00f}; // terrain shape blue vector
      Vector3 fR_i = Vector3Add(mT_e, tR_i);    // final red in world space
      Vector3 fG_i = Vector3Add(mT_e, tG_i);    // final green in world space
      Vector3 fB_i = Vector3Add(mT_e, tB_i);    // final blue in world space
      DrawTriangle3D(fR_i, fG_i, fB_i, DARKGRAY);
      DrawSphere(fR_i, 0.2f, RED);
      DrawSphere(fG_i, 0.2f, RED);
      DrawSphere(fB_i, 0.2f, RED);
      // [B]
      Vector3 tR_j = {-10.00f, 00.00f, 10.00f}; // terrain shape red vector
      Vector3 tG_j = { 10.00f, 00.00f,-10.00f}; // terrain shape green vector
      Vector3 tB_j = {-10.00f, 00.00f,-10.00f}; // terrain shape blue vector
      Vector3 fR_j = Vector3Add(mT_e, tR_j);    // final red in world space
      Vector3 fG_j = Vector3Add(mT_e, tG_j);    // final green in world space
      Vector3 fB_j = Vector3Add(mT_e, tB_j);    // final blue in world space
      DrawTriangle3D(fR_j, fG_j, fB_j, WHITE);
      DrawSphere(fR_j, 0.2f, RED);
      DrawSphere(fG_j, 0.2f, RED);
      DrawSphere(fB_j, 0.2f, RED);






      // ship experiment
      Vector3 ship_init_pos = {0.0f,0.0f,0.0f};

      // /***************
      // **            **
      // ** Left Wing  **
      // **            **
      // ***************/
       DrawModel(_001, ship_init_pos, 3.0f,  WHITE);
      //                     >RL  ^UD   \/BF 
      DrawSphere((Vector3){-3.9f, -0.31f, 4.0f}, 0.01f, BLACK); 
      DrawSphere((Vector3){-1.87f,  0.09f, -0.7f}, 0.01f, BLACK);
      DrawSphere((Vector3){-2.01f,  0.1f, 1.0f}, 0.01f, BLACK);
      DrawSphere((Vector3){-2.0f,  -0.2f, 0.85f}, 0.01f, BLACK);
      DrawLine3D((Vector3){-3.9f, -0.31f, 4.0f}, (Vector3){-1.87f,  0.09f, -0.7f}, leftWingLines);
      DrawLine3D((Vector3){-1.87f,  0.09f, -0.7f},(Vector3){-2.01f,  0.1f, 1.0f}, leftWingLines);
      DrawLine3D((Vector3){-1.87f,  0.09f, -0.7f},(Vector3){-2.01f,  0.1f, 1.0f}, leftWingLines);
      DrawLine3D((Vector3){-1.87f,  0.09f, -0.7f},(Vector3){-2.0f,  -0.2f, 0.85f}, leftWingLines);
      DrawLine3D((Vector3){-2.01f,  0.1f, 1.0f},(Vector3){-2.0f,  -0.2f, 0.85f}, leftWingLines);
      DrawLine3D((Vector3){-2.01f,  0.1f, 1.0f},(Vector3){-3.9f, -0.31f, 4.0f}, leftWingLines);
      DrawLine3D((Vector3){-2.0f,  -0.2f, 0.85f},(Vector3){-3.9f, -0.31f, 4.0f}, leftWingLines);

      // /***************
      // **            **
      // ** Left Eng   **
      // **            **
      // ***************/
      DrawModel(_002, ship_init_pos, 3.0f,  DARKBLUE);
      //                   >RL  ^UD   \/BF 
      DrawSphere((Vector3){-1.95f, 1.5f, 1.3f}, 0.05f, GREEN);
      DrawSphere((Vector3){-1.77f, 0.19f, -1.72f}, 0.05f, GREEN);
      DrawSphere((Vector3){-1.34f, -0.076f, 0.85f}, 0.05f, GREEN);
      DrawSphere((Vector3){-2.01f, -0.54f, 0.97f}, 0.05f, GREEN);
      DrawLine3D((Vector3){-1.95f, 1.5f, 1.3f}, (Vector3){-1.77f, 0.19f, -1.72f}, leftEngLines);
      DrawLine3D((Vector3){-1.95f, 1.5f, 1.3f}, (Vector3){-1.34f, -0.076f, 0.85f}, leftEngLines);
      DrawLine3D((Vector3){-1.95f, 1.5f, 1.3f}, (Vector3){-2.01f, -0.54f, 0.97f}, leftEngLines);
      DrawLine3D((Vector3){-2.01f, -0.54f, 0.97f},(Vector3){-1.34f, -0.076f, 0.85f}, leftEngLines);
      DrawLine3D((Vector3){-1.34f, -0.076f, 0.85f},(Vector3){-1.77f, 0.19f, -1.72f}, leftEngLines);
      DrawLine3D((Vector3){-1.77f, 0.19f, -1.72f},(Vector3){-2.01f, -0.54f, 0.97f}, leftEngLines);


      /***************
      **            **
      ** Center     **
      **            **
      ***************/
      DrawModel(_003, ship_init_pos, 3.0f,  WHITE);
      //                     >RL  ^UD   \/BF 
      DrawSphere((Vector3){0.0f,0.02f,-5.81f}, 0.05f, GREEN); // front tip
      DrawSphere((Vector3){0.0f,0.93f,-0.42f}, 0.05f, GREEN); // top
      DrawSphere((Vector3){0.0f,0.30f, 0.19f}, 0.05f, GREEN); //eng top
      DrawSphere((Vector3){-1.54f,0.02f,0.18f}, 0.05f, GREEN); // left
      DrawSphere((Vector3){1.54f,0.02f,0.18f}, 0.05f, GREEN);  // right
      DrawLine3D((Vector3){0.0f,0.02f,-5.81f}, (Vector3){0.0f,0.93f,-0.42f}, centerLines);
      DrawLine3D((Vector3){0.0f,0.02f,-5.81f},(Vector3){-1.54f,0.02f,0.18f}, centerLines);
      DrawLine3D((Vector3){0.0f,0.02f,-5.81f},(Vector3){1.54f,0.02f,0.18f}, centerLines);
      DrawLine3D((Vector3){0.0f,0.93f,-0.42f},(Vector3){0.0f,0.30f, 0.19f}, centerLines);
      DrawLine3D((Vector3){0.0f,0.30f, 0.19f},(Vector3){-1.54f,0.02f,0.18f}, centerLines);
      DrawLine3D((Vector3){0.0f,0.30f, 0.19f},(Vector3){1.54f,0.02f,0.18f}, centerLines);
      DrawLine3D((Vector3){-1.54f,0.02f,0.18f},(Vector3){1.54f,0.02f,0.18f}, centerLines);
      DrawLine3D((Vector3){0.0f,0.93f,-0.42f},(Vector3){-1.54f,0.02f,0.18f}, centerLines);
      DrawLine3D((Vector3){0.0f,0.93f,-0.42f},(Vector3){1.54f,0.02f,0.18f}, centerLines);


      // /***************
      // **            **
      // ** Right Eng  **
      // **            **
      // ***************/
      DrawModel(_004, ship_init_pos, 3.0f,  DARKBLUE);
      //                   >RL  ^UD   \/BF 
      DrawSphere((Vector3){1.95f, 1.5f, 1.3f}, 0.05f, GREEN);
      DrawSphere((Vector3){1.77f, 0.19f, -1.72f}, 0.05f, GREEN);
      DrawSphere((Vector3){1.34f, -0.076f, 0.85f}, 0.05f, GREEN);
      DrawSphere((Vector3){2.01f, -0.54f, 0.97f}, 0.05f, GREEN);
      DrawLine3D((Vector3){1.95f, 1.5f, 1.3f}, (Vector3){1.77f, 0.19f, -1.72f}, rightEngLines);
      DrawLine3D((Vector3){1.95f, 1.5f, 1.3f}, (Vector3){1.34f, -0.076f, 0.85f}, rightEngLines);
      DrawLine3D((Vector3){1.95f, 1.5f, 1.3f}, (Vector3){2.01f, -0.54f, 0.97f}, rightEngLines);
      DrawLine3D((Vector3){2.01f, -0.54f, 0.97f},(Vector3){1.34f, -0.076f, 0.85f}, rightEngLines);
      DrawLine3D((Vector3){1.34f, -0.076f, 0.85f},(Vector3){1.77f, 0.19f, -1.72f}, rightEngLines);
      DrawLine3D((Vector3){1.77f, 0.19f, -1.72f},(Vector3){2.01f, -0.54f, 0.97f}, rightEngLines);

      // /***************
      // **            **
      // ** Right Wing **
      // **            **
      // ***************/
      DrawModel(_005, ship_init_pos, 3.0f,  WHITE);
      //                     >RL  ^UD   \/BF 
      DrawSphere((Vector3){3.9f, -0.31f, 4.0f}, 0.05f, GREEN);
      DrawSphere((Vector3){1.87f,  0.09f, -0.7f}, 0.05f, GREEN);
      DrawSphere((Vector3){2.01f,  0.1f, 1.0f}, 0.05f, GREEN);
      DrawSphere((Vector3){2.0f,  -0.2f, 0.85f}, 0.05f, GREEN);
      DrawLine3D((Vector3){3.9f, -0.31f, 4.0f}, (Vector3){1.87f,  0.09f, -0.7f}, rightWingLines);
      DrawLine3D((Vector3){1.87f,  0.09f, -0.7f},(Vector3){2.01f,  0.1f, 1.0f}, rightWingLines);
      DrawLine3D((Vector3){1.87f,  0.09f, -0.7f},(Vector3){2.01f,  0.1f, 1.0f}, rightWingLines);
      DrawLine3D((Vector3){1.87f,  0.09f, -0.7f},(Vector3){2.0f,  -0.2f, 0.85f}, rightWingLines);
      DrawLine3D((Vector3){2.01f,  0.1f, 1.0f},(Vector3){2.0f,  -0.2f, 0.85f}, rightWingLines);
      DrawLine3D((Vector3){2.01f,  0.1f, 1.0f},(Vector3){3.9f, -0.31f, 4.0f}, rightWingLines);
      DrawLine3D((Vector3){2.0f,  -0.2f, 0.85f},(Vector3){3.9f, -0.31f, 4.0f}, rightWingLines);




      EndMode3D();
      /////////////////////////////BITS/////////////////////////////////////////(-)
            
      // Debugs
      // DrawFPS(10, 10);
      string player_xyz = "Ship position in realation to world space:\nX = " + to_string(playerPosition.x) + "\nY = " +  to_string(playerPosition.y) + "\nZ = " + to_string(playerPosition.z);
      DrawText (player_xyz.c_str(), 2, 2, 10, GREEN);
      string player_rot = "Ship quaternion derived vector:\nX = " + to_string(playerRotation.x) + "\nY = " + to_string(playerRotation.y) + "\nZ = " + to_string(playerRotation.z);
      DrawText (player_rot.c_str(), 2, 60, 10, GREEN);
      string distString = "Distance from snapshot:\nVector3 dist = " + to_string(derivedDistance); 
      DrawText (distString.c_str(), 2, 118, 10, GREEN);
      //player.printValues();
      string cm_str = to_string(countMe);
      //DrawText(cm_str.c_str(), 2, 220, 20, GREEN);
      DrawText(display_0.c_str(), 2, 260, 20, GREEN);
      DrawText(display_1.c_str(), 2, 290, 20, GREEN);




      EndDrawing();
   }

   CloseWindow();  

   return 0;
}
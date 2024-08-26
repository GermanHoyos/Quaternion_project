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
#include "../include/MasterHeader.h"
 
class myPlayer
{
   public:
   Quaternion rotation         = QuaternionIdentity();
   Quaternion rotationDelta    = QuaternionIdentity();
   Quaternion strafeDelta      = QuaternionIdentity();
   Quaternion collectRotations = QuaternionIdentity();
   Matrix     cubeSpace        = MatrixIdentity();
   Matrix     animMatrx        = MatrixIdentity();
   Matrix     tiltMatrix       = MatrixIdentity();
   Matrix     aimMatrix1       = MatrixIdentity();
   Matrix     aimMatrix2       = MatrixIdentity();
   Color      myRGB            = {255,255,255,255};
   Color      leftWingLines    = {0,0,255,255};
   Color      rightWingLines   = {0,0,255,255};
   Color      leftEngLines     = {0,0,255,255};
   Color      rightEngLines    = {0,0,255,255};
   Color      centerLines      = {0,0,255,255};
   Model      ship             = LoadModel("./src/assets/ship.obj");
   Model      ship2            = LoadModel("./src/assets/rwing.obj");
   Texture2D  texture          = LoadTexture("./src/assets/t1.png");
   Texture2D  texture2         = LoadTexture("./src/assets/rwing.1001.png");
   Texture2D  rockTexture      = LoadTexture("./src/assets/rock_2.png");
   Model      myGlb            = LoadModel("./src/assets/rwing.glb");
   //Piece mealed obj
   Model      _001             = LoadModel("./src/assets/001.obj");
   Model      _002             = LoadModel("./src/assets/002.obj");
   Model      _003             = LoadModel("./src/assets/003.obj");
   Model      _004             = LoadModel("./src/assets/004.obj");
   Model      _005             = LoadModel("./src/assets/005.obj");
   Model      fnlModel         = LoadModel("./src/assets/finalAttempt.obj");
   Texture2D  fnlTexture       = LoadTexture("./src/assets/finalAttempt.png");
   Vector3    ship_init_pos    = {0.0f, 0.0f, 0.0f};
   Vector3    rotationAxis     = {0.0f, 0.0f, 1.0f}; // Z axis rot
   Vector3    shipWorldPos;
   Vector3    forwardDirection;
   bool       shot = false;
   bool       textureLoaded = false;
   double     shotClock = 0;
   double     shotTimer = 0;
   int        cR = 255;
   int        cG = 255;
   int        cB = 255;
   int        cAlpha = 255;
   float      x, y, z, dx, dy, dz, rx, ry, rz;
   float      animMatrixDeg  = 0.0f;
   float      animMatrixAlpa = 0.0f;
   float      offSet = 0.0f;
   float      speed1 = 1.0f;
   float      speed2 = 0.5f;
   float      speed3 = 0.0f;
   float      thrus1 = 0.0f;
   float      thrus2 = 0.3f;
   float      thrus3 = 3.4f;
   float      radis1 = 0.3f;
   float      radis2 = 0.0f;
   float      negPos =-1.0f;
   float      strfSp = 0.0f;
   float      strafe = 0.0f;
   float      strafeUpSpeed = 0.0f;
   float      strafeDownSpeed = 0.0f;
   float      rotAng = 0.0f * (3.1415f / 180.0f);
   float      rotationIncrementer = 0.0f;
   float      turnSpeedX = 0.5f;
   float      turnSpeedY = 0.5f;
 
   myPlayer(float x, float y, float z, float dx = 0.0f, float dy = 0.0f, float dz = 0.0f, float rx = 0.0f, float ry = 0.0f, float rz = 0.0f)
   : x(x), y(y), z(z), dx(dx), dy(dy), dz(dz), rx(rx), ry(ry), rz(rz) {}
 
   void draw()
   {
      // RANDOM NUMS
      /*sphere color*/ random_device rd_1; mt19937 gen_1(rd_1()); uniform_int_distribution<int>    dis_1(1, 255);       cG = dis_1(gen_1); cB = dis_1(gen_1); /*cAlpha = dis_1(gen_1);*/
      /*sphere xy[Z]*/ random_device rd_2; mt19937 gen_2(rd_2()); uniform_real_distribution<float> dis_2(-1.6f, -1.3f); thrus1 = thrus1 + dis_2(gen_2);
      /*sphere x[Y]z*/ random_device rd_3; mt19937 gen_3(rd_3()); uniform_real_distribution<float> dis_3(0.3f, 0.7f);   thrus2 = thrus2 + dis_3(gen_2);
      /*sphere [X]yz*/ random_device rd_4; mt19937 gen_4(rd_4()); uniform_real_distribution<float> dis_4(-1.0f, 1.0f);
      /*sphere radus*/ if(radis1 < 0.2f){radis1 += 0.009f;} else {radis1 = 0.01f;}
 
      // PUSH MATRIX // GEN LOCAL SPACE MATRIX
      loadTexture();
      tiltMatrix = MatrixRotate(rotationAxis, rotAng);                              // Calculate a tilt that will be applied to the matrix that quaternions are applied to.
      rlPushMatrix();                                                               // save current state of matrix untill [pop]
      apply_rotation_axis();                                                        // apply quaternion math to [rx] [ry] [rz] based on start point of 0 0 0
      accelerate_foward();                                                          // apply acceleration maths to directional variable [dx] [dy] [dz]
      Quaternion resultRotation = QuaternionMultiply(rotation, collectRotations);   // store above quaternion in a variable
      cubeSpace = QuaternionToMatrix(resultRotation);                               // calculate how to apply quaternion to this current matrix
 
      // 1) apply rotation calculations 2) apply acceleration calculations 3) apply quaternion rotation applications
      cubeSpace.m12 += dx;                                                          // [x] axis acceleration
      cubeSpace.m13 += dy;                                                          // [y] axis acceleration
      cubeSpace.m14 += dz;                                                          // [z] axis acceleration
 
      // apply strafing
      strafe_x_axis();
      strafe_y_axis();
 
      // apply tilts
      cubeSpace = MatrixMultiply(tiltMatrix, cubeSpace);
     
      // take all above calculations and finally apply to matrix
      rlMultMatrixf(MatrixToFloat(cubeSpace));                                      // apply all above culculations to this current matrix
     
      // draw models and thruster effects
      // DrawModel(ship2, ship_init_pos, 3.0f,  WHITE);                                 // blender made model
      // DrawModel(myGlb, ship_init_pos, 2.0f, WHITE);
      // DrawModel(_001, ship_init_pos, 3.0f,  WHITE);
      // DrawModel(_002, ship_init_pos, 3.0f,  BLUE);
      // DrawModel(_003, ship_init_pos, 3.0f,  WHITE);
      // DrawModel(_004, ship_init_pos, 3.0f,  BLUE);
      // DrawModel(_005, ship_init_pos, 3.0f,  WHITE);
      //DrawModel(fnlModel, ship_init_pos, 3.0f,  WHITE);

      DrawSphere({thrus1, thrus2, thrus3}, radis1, {static_cast<unsigned char>(cR), static_cast<unsigned char>(cG), static_cast<unsigned char>(cB), static_cast<unsigned char>(cAlpha)});
      DrawSphere({-thrus1, thrus2, thrus3}, radis1, {static_cast<unsigned char>(cR), static_cast<unsigned char>(cG), static_cast<unsigned char>(cB), static_cast<unsigned char>(cAlpha)});

      drawThisShip();
      drawAimBox();
 
      // pop matrix
      rlPopMatrix();                                                                // revert back to state before [push]
 
      // reset to default values [0.00f]
      rotationDelta    = QuaternionIdentity();
      collectRotations = QuaternionIdentity();                                     // reset the collectRotations quaternion to 0 0 0
      rotation         = QuaternionIdentity();
      strafeDelta      = QuaternionIdentity();
      thrus1 = 0.0f; thrus2 = 0.3f;
      makeGlobal();
   }
 
   void rotate_y_axis() /* [Y] */
   {
     float axisMovementY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_Y);
 
      // If a joystick max value is needed then convert it to int and check against a value of "1"
      if (IsKeyDown(KEY_Q) || axisMovementY < 0.0f)
      {
         if (playerRotation.y < 0.98f)
         {
            ry += axisMovementY; // Add directly (axisMovementX will be negative)
         }
      }
      else if (IsKeyDown(KEY_E) || axisMovementY > 0.0f)
      {
         if (playerRotation.y > -0.95f)
         {
            ry += axisMovementY; // Add directly (axisMovementX will be positive)
         }
      }
      // Convert the X-axis rotation to a quaternion and apply it
      rotationDelta = QuaternionFromAxisAngle({ -1.0f, 0.0f, 0.0f }, DEG2RAD * ry);
      collectRotations = QuaternionMultiply(collectRotations, rotationDelta);
   }
 
   void rotate_x_axis() /* [X] */
   {
      float axisMovementX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_X);
 
      // If a joystick max value is needed then convert it to int and check against a value of "1"
      if (IsKeyDown(KEY_Q) || axisMovementX < 0.0f)
      {
         rx += axisMovementX; // Add directly (axisMovementX will be negative)
      }
      else if (IsKeyDown(KEY_E) || axisMovementX > 0.0f)
      {
         rx += axisMovementX; // Add directly (axisMovementX will be positive)
      }
 
      // Convert the X-axis rotation to a quaternion and apply it
      rotationDelta = QuaternionFromAxisAngle({ 0.0f, -1.0f, 0.0f }, DEG2RAD * rx);
      collectRotations = QuaternionMultiply(collectRotations, rotationDelta);
   }
 
   void apply_tilt_animation()
   {
   // Tilt conditional
      if ((float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X)) > 0.2f )
      {
         if (rotationIncrementer > -15.0f)
         {
            rotationIncrementer -= 1.0f;
            rotAng = rotationIncrementer * (3.1415f / 180.0f);
         }
      }
 
      // Tilt conditional
      if ((float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X)) < -0.2f )
      {
         if (rotationIncrementer < 15.0f)
         {
            rotationIncrementer += 1.0f;
            rotAng = rotationIncrementer * (3.1415f / 180.0f);
         }
      }
 
      // friction / brakes
      if ((float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X)) == 0.0f)
      {
         if (rotationIncrementer < 0.0f)
         {
            rotationIncrementer += 0.1f;
            rotAng = rotationIncrementer * (3.1415f / 180.0f);
         }
         if (rotationIncrementer > 0.0f)
         {
            rotationIncrementer += -0.1f;
            rotAng = rotationIncrementer * (3.1415f / 180.0f);
         }
      }
   }
 
   void strafe_x_axis() // slide left and right but maintain quaternion rotation
   {
      if (IsKeyDown(KEY_A) ||(float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X)) < 0.0f )
      {
         if (strafe < 0.9f)  strafe += 0.01f;
      }
      if (IsKeyDown(KEY_D) || (float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X)) > 0.0f )
      {
         if (strafe > -0.9f) strafe -= 0.01f;
      }
      
      //Brakes for strafing on x axis
      if
      (
         (float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X)) == 0.0f &&
         !IsKeyDown(KEY_A) &&
         !IsKeyDown(KEY_D)
      )
      {
         if (strafe > 0.01f) {
            strafe -= 0.01f;
         } else if (strafe < -0.01f) {
            strafe += 0.01f;
         } else {
            strafe = 0.0f;
         }
      }
 
      // Calculate strafe offset based on ship's facing direction
      Vector3 forwardDirection = Vector3Transform({0.0f, 0.0f, -1.0f}, QuaternionToMatrix(collectRotations));
      forwardDirection = Vector3Normalize(forwardDirection);
      Vector3 strafeOffset = {forwardDirection.z * strafe, 0.0f, -forwardDirection.x * strafe}; // This is what calculates the strafe
 
      // Apply strafe offset to ship's position
      // By adjusting both the dx (x-coordinate) and dz (z-coordinate), we ensure that the ship moves correctly in the desired direction.
      // This approach allows for fluid movement across the 3D space while maintaining the ship's orientation.
      dx += strafeOffset.x;
      dz += strafeOffset.z;
   }
 
   void strafe_y_axis() // slide up and down but mainain quaternion rotation
   {
      // if (IsKeyDown(KEY_UP)   || IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1) > 0) dy += 0.3f;
      // if (IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1) > 0)  dy -= 0.3f;
 
      if (IsKeyDown(KEY_UP)   || IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1) > 0)
      {
         strafeUpSpeed += 0.005;
      }
 
      if (IsKeyDown(KEY_UP)   || IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_2) > 0)
      {
         strafeUpSpeed -= 0.005;
      }
 
      if
      (
         IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1) == 0 &&
         IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_2) == 0
      )
      {
         if (strafeUpSpeed > 0.01f) {
            strafeUpSpeed -= 0.01f;
         } else if (strafeUpSpeed < -0.01f) {
            strafeUpSpeed += 0.01f;
         } else {
            strafeUpSpeed = 0.0f;
         }
      }
 
      dy += strafeUpSpeed;
 
 
   }
 
   void accelerate_foward()
   {
     
      Vector3 fowardDirection = Vector3Transform({0.0f, 0.0f, negPos}, QuaternionToMatrix(collectRotations));
 
      // foward
      if ((float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y)) < 0.0f)
      {
         if (speed3 < 1.0f)
         {
            speed3 += 0.01f;
         }
      }
 
      // reverse
      if ((float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y)) > 0.0f)
      {
         if (speed3 > -1.0f )
         {
            speed3 -= 0.01f;
         }
      }
 
      // friction / brakes
      if ((float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y)) == 0.0f)
      {
         if (speed3 > 0.05f) speed3 -= 0.01f;
         else if (speed3 < -0.05f) speed3 += 0.01f;
         else speed3 = 0.0f; // stop the vehicle if speed is within a small range around 0
      }
 
      dx += fowardDirection.x * speed3;
      dy += fowardDirection.y * speed3;
      dz += fowardDirection.z * speed3;
   }
 
   void apply_rotation_axis()
   {
      rotate_x_axis(); rotate_y_axis(); apply_tilt_animation(); shootLasers(); //drawAimBox();
   }
 
void shootLasers()
{
    forwardDirection = Vector3Transform({0.0f, 0.0f, -1.0f}, QuaternionToMatrix(collectRotations));
    forwardDirection = Vector3Normalize(forwardDirection);

    //START shots
    if (IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_2) > 0)
    {
        // Define the local left position (relative to the ship's origin)
        Vector3 localLeftPos = {-2.0f, 0.0f, 0.0f};
        Vector3 localRightPos = {2.0f, 0.0f, 0.0f};


        // Rotate the local position by the ship's rotation quaternion
        Vector3 rotatedLeftPos = Vector3Transform(localLeftPos, QuaternionToMatrix(collectRotations));
        Vector3 rotatedRightPos = Vector3Transform(localRightPos, QuaternionToMatrix(collectRotations));

        // Get the ship's world position
        Vector3 shipWorldPos = {cubeSpace.m12, cubeSpace.m13, cubeSpace.m14};

        // Calculate the final world position of the left laser
        Vector3 shipWorldPos_left = Vector3Add(shipWorldPos, rotatedLeftPos);
        Vector3 shipWorldPos_right = Vector3Add(shipWorldPos, rotatedRightPos);


        // Create and store the laser with an initial speed
        float laserSpeed = 3.5f;
        lasers laser_left = lasers(shipWorldPos_left, forwardDirection, laserSpeed);
        lasersList.push_back(laser_left);
        lasers laser_right = lasers(shipWorldPos_right, forwardDirection, laserSpeed);
        lasersList.push_back(laser_right);


    }
}

 
   void drawAimBox()
   {
      animMatrixDeg    += 2.5f;
      animMatrixAlpa   += 0.01f;  // Adjust increment rate as needed
      offSet           += 0.01f;
 
      // Calculate alpha value using sin and map it to a range 20 to 255
      float myAlpha = animMatrixAlpa + offSet;
      float alphaSin = sin(myAlpha);             // Sin value oscillates between -1 and 1
      float alphaNormalized = (alphaSin + 1.0f) * 0.5f;  // Map to 0 to 1 range
      float alphaFinal1 = 10.0f + alphaNormalized * (200.0f - 120.0f); // Scale to range 20 to 255
      float alphaFinal2 = 40.0f + alphaNormalized * (255.0f - 40.0f); // Scale to range 20 to 255
 
      // First, draw the larger box
      rlPushMatrix();
         rlRotatef(animMatrixDeg, 0.0f, 0.0f, 1.0f);  // Rotate around Z-axis
         DrawLine3D((Vector3){-1.5f,-1.5f,-15.0f}, (Vector3){-1.5f, 1.5f,-15.0f}, {255,255,255,(unsigned char)alphaFinal1}); // Double the size
         DrawLine3D((Vector3){ 1.5f,-1.5f,-15.0f}, (Vector3){ 1.5f, 1.5f,-15.0f}, {255,255,255,(unsigned char)alphaFinal1});
         DrawLine3D((Vector3){-1.5f,-1.5f,-15.0f}, (Vector3){ 1.5f,-1.5f,-15.0f}, {255,255,255,(unsigned char)alphaFinal1});
         DrawLine3D((Vector3){-1.5f, 1.5f,-15.0f}, (Vector3){ 1.5f, 1.5f,-15.0f}, {255,255,255,(unsigned char)alphaFinal1});
      rlPopMatrix();
 
      // Then, draw the smaller box
      rlPushMatrix();
         rlRotatef(animMatrixDeg, 0.0f, 0.0f, -1.0f);  // Rotate around Z-axis
         DrawLine3D((Vector3){-1.0f,-1.0f,-27.0f}, (Vector3){-1.0f,1.0f,-27.0f},  {255,255,255,(unsigned char)alphaFinal2}); // Original size
         DrawLine3D((Vector3){ 1.0f,-1.0f,-27.0f}, (Vector3){ 1.0f,1.0f,-27.0f},  {255,255,255,(unsigned char)alphaFinal2});
         DrawLine3D((Vector3){-1.0f,-1.0f,-27.0f}, (Vector3){ 1.0f,-1.0f,-27.0f}, {255,255,255,(unsigned char)alphaFinal2});
         DrawLine3D((Vector3){-1.0f, 1.0f,-27.0f}, (Vector3){ 1.0f,1.0f,-27.0f},  {255,255,255,(unsigned char)alphaFinal2});
      rlPopMatrix();
   }
 
   void display_coord_data()
   {
      // print to screen [x] [y] [z] world space coords
   }
 
   void loadTexture()
   {
      //if(!textureLoaded){ fnlModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = fnlTexture; textureLoaded = true;};
   }
 
   void makeGlobal()
   {
      playerPosition = {cubeSpace.m12, cubeSpace.m13, cubeSpace.m14};
      playerRotation = forwardDirection;
   }
 
   void printValues()
   {
    // Create a string containing the x, y, and z values
    std::string collectRotationsQuat = "x: " + std::to_string(collectRotations.x) +
                                       " y: " + std::to_string(collectRotations.y) +
                                       " z: " + std::to_string(collectRotations.z);
 
    // Print the string on the screen
    DrawText(collectRotationsQuat.c_str(), 2, 200, 10, GREEN);
   }
 
   void drawThisShip()
   {
            // Ship experiment draw
      Vector3 ship_init_pos = {0.0f,0.0f,0.0f};

      // /***************
      // **            **
      // ** Left Wing  **
      // **            **
      // ***************/
       DrawModel(_001, ship_init_pos, 3.0f,  WHITE);
      //                     >RL  ^UD   \/BF 
      // DrawSphere((Vector3){-3.9f, -0.31f, 4.0f}, 0.01f, BLACK); 
      // DrawSphere((Vector3){-1.87f,  0.09f, -0.7f}, 0.01f, BLACK);
      // DrawSphere((Vector3){-2.01f,  0.1f, 1.0f}, 0.01f, BLACK);
      // DrawSphere((Vector3){-2.0f,  -0.2f, 0.85f}, 0.01f, BLACK);
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
      // DrawSphere((Vector3){-1.95f, 1.5f, 1.3f}, 0.05f, GREEN);
      // DrawSphere((Vector3){-1.77f, 0.19f, -1.72f}, 0.05f, GREEN);
      // DrawSphere((Vector3){-1.34f, -0.076f, 0.85f}, 0.05f, GREEN);
      // DrawSphere((Vector3){-2.01f, -0.54f, 0.97f}, 0.05f, GREEN);
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
      // DrawSphere((Vector3){0.0f,0.02f,-5.81f}, 0.05f, GREEN); // front tip
      // DrawSphere((Vector3){0.0f,0.93f,-0.42f}, 0.05f, GREEN); // top
      // DrawSphere((Vector3){0.0f,0.30f, 0.19f}, 0.05f, GREEN); //eng top
      // DrawSphere((Vector3){-1.54f,0.02f,0.18f}, 0.05f, GREEN); // left
      // DrawSphere((Vector3){1.54f,0.02f,0.18f}, 0.05f, GREEN);  // right
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
      // DrawSphere((Vector3){1.95f, 1.5f, 1.3f}, 0.05f, GREEN);
      // DrawSphere((Vector3){1.77f, 0.19f, -1.72f}, 0.05f, GREEN);
      // DrawSphere((Vector3){1.34f, -0.076f, 0.85f}, 0.05f, GREEN);
      // DrawSphere((Vector3){2.01f, -0.54f, 0.97f}, 0.05f, GREEN);
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
      // DrawSphere((Vector3){3.9f, -0.31f, 4.0f}, 0.05f, GREEN);
      // DrawSphere((Vector3){1.87f,  0.09f, -0.7f}, 0.05f, GREEN);
      // DrawSphere((Vector3){2.01f,  0.1f, 1.0f}, 0.05f, GREEN);
      // DrawSphere((Vector3){2.0f,  -0.2f, 0.85f}, 0.05f, GREEN);
      DrawLine3D((Vector3){3.9f, -0.31f, 4.0f}, (Vector3){1.87f,  0.09f, -0.7f}, rightWingLines);
      DrawLine3D((Vector3){1.87f,  0.09f, -0.7f},(Vector3){2.01f,  0.1f, 1.0f}, rightWingLines);
      DrawLine3D((Vector3){1.87f,  0.09f, -0.7f},(Vector3){2.01f,  0.1f, 1.0f}, rightWingLines);
      DrawLine3D((Vector3){1.87f,  0.09f, -0.7f},(Vector3){2.0f,  -0.2f, 0.85f}, rightWingLines);
      DrawLine3D((Vector3){2.01f,  0.1f, 1.0f},(Vector3){2.0f,  -0.2f, 0.85f}, rightWingLines);
      DrawLine3D((Vector3){2.01f,  0.1f, 1.0f},(Vector3){3.9f, -0.31f, 4.0f}, rightWingLines);
      DrawLine3D((Vector3){2.0f,  -0.2f, 0.85f},(Vector3){3.9f, -0.31f, 4.0f}, rightWingLines);


   }




};
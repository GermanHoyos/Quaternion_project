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
 
   // MODEL + BLENDER BAKE
   Model ship = LoadModel("C:\\Users\\Hoyos\\OneDrive\\Desktop\\C++ Runner\\raylib_quaternion_example\\home\\src\\assets\\ship.obj");
   Texture2D texture = LoadTexture("C:\\Users\\Hoyos\\OneDrive\\Desktop\\C++ Runner\\raylib_quaternion_example\\home\\src\\assets\\t1.png");
   Vector3 ship_init_pos = {0.0f, 0.0f, 0.0f};
   Vector3 shipWorldPos;
   Vector3 forwardDirection;
   Vector3 rotationAxis = {0.0f, 0.0f, 1.0f}; // Z axis rot
   double shotClock = 0;
   double shotTimer = 0;
   bool   shot = false;
   float    x, y, z, dx, dy, dz, rx, ry, rz;
   float speed1 = 1.0f; float speed2 = 0.5f;
   float speed3 = 0.0f; float thrus1 = 0.0f;
   float thrus2 = 0.3f; float thrus3 = 3.4f;
   float radis1 = 0.3f; float radis2 = 0.0f;
   float negPos =-1.0f; float strfSp = 0.0f;
   float strafe = 0.0f; float rotAng = 0.0f * (3.1415f / 180.0f);
   float rotationIncrementer = 0.0f;
   int cR = 255; int cG     = 255; 
   int cB = 255; int cAlpha = 255; 
   Color      myRGB = {255,255,255,255};
   Quaternion rotation         = QuaternionIdentity();
   Quaternion rotationDelta    = QuaternionIdentity();
   Quaternion strafeDelta      = QuaternionIdentity();
   Quaternion collectRotations = QuaternionIdentity();
   Matrix     cubeSpace        = MatrixIdentity();
   Matrix     animMatrx        = MatrixIdentity();
   Matrix     tiltMatrix       = MatrixIdentity();
   bool       textureLoaded    = false;
   myPlayer(float x, float y, float z, float dx = 0.0f, float dy = 0.0f, float dz = 0.0f, float rx = 0.0f, float ry = 0.0f, float rz = 0.0f)
   : x(x), y(y), z(z), dx(dx), dy(dy), dz(dz), rx(rx), ry(ry), rz(rz) {}

   void idle()
   {
      // Anything that manipulates the player game state
   }

   void draw()
   { 
      // RANDOM NUMS
      /*sphere color*/ random_device rd_1; mt19937 gen_1(rd_1()); uniform_int_distribution<int>    dis_1(1, 255);      cG = dis_1(gen_1); cB = dis_1(gen_1); cAlpha = dis_1(gen_1);
      /*sphere xy[Z]*/ random_device rd_2; mt19937 gen_2(rd_2()); uniform_real_distribution<float> dis_2(-0.1f, 0.1f); thrus1 = thrus1 + dis_2(gen_2);
      /*sphere x[Y]z*/ random_device rd_3; mt19937 gen_3(rd_3()); uniform_real_distribution<float> dis_3(-0.1f, 0.2f); thrus2 = thrus2 + dis_3(gen_2);
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
      DrawModel(ship, ship_init_pos, 3.0f,  WHITE);                                 // blender made model
      DrawSphere({thrus1, thrus2, thrus3}, radis1, {static_cast<unsigned char>(cR), static_cast<unsigned char>(cG), static_cast<unsigned char>(cB), static_cast<unsigned char>(cAlpha)});

      // POP MATRIX // REVERT BACK TO WORLD SPACE MATRIX
      rlPopMatrix();                                                                // revert back to state before [push]

      // RESETS
      rotationDelta    = QuaternionIdentity();
      //collectRotations = QuaternionIdentity();                                      // reset the collectRotations quaternion to 0 0 0
      quaternion_brakes();
      rotation         = QuaternionIdentity();
      strafeDelta      = QuaternionIdentity();
      thrus1 = 0.0f; thrus2 = 0.3f;
      makeGlobal();
   }

   void rotate_x_axis()
   {
      // If a joystick max value is needed then convert it to int and check against a value of "1"
      if (IsKeyDown(KEY_W) || (float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_Y)) < 0.0f )
      {
         // if (rx < 1.4)
         // {
         //    rx += 0.3f;
         // }
         rx += 0.5f;
      } 
      if (IsKeyDown(KEY_S) || (float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_Y)) > 0.0f )
      {
         // if (rx > -1.4)
         // {
         //    rx -= 0.3f;
         // }
         rx -= 0.5f;
      } 
      rotationDelta = QuaternionFromAxisAngle({ 1.0f, 0.0f, 0.0f }, DEG2RAD * rx );
      collectRotations = QuaternionMultiply(collectRotations, rotationDelta);
   }

   void rotate_y_axis()
   {
      // implement
   }

   void rotate_z_axis()
   {
      // If a joystick max value is needed then convert it to int and check against a value of "1"
      if (IsKeyDown(KEY_Q) || (float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_X)) < 0.0f )
      {
         // if (rz > -1.4f) 
         // {
         //    rz -= 0.03f;
         // }
         rz -= 0.5f;
      } 
      if (IsKeyDown(KEY_E) || (float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_X)) > 0.0f )
      {
         // if (rz < 1.4f)
         // {
         //    rz += 0.03f;
         // }
         rz += 0.5f;
      } 
      rotationDelta = QuaternionFromAxisAngle({ 0.0f, -1.0f, 0.0f }, DEG2RAD * rz );
      collectRotations = QuaternionMultiply(collectRotations, rotationDelta);
   }

   void quaternion_brakes()
   {
      collectRotations = QuaternionIdentity();

      // if ((float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_X)) == 0.0f)
      // {
      //    if (rz > 0.01f) {
      //       rz -= 0.01f;
      //    } else if (rz < -0.01f) {
      //       rz += 0.01f;
      //    } else {
      //       rz = 0.0f;
      //    }
      // }

      // // if ((float)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_Y)) == 0.0f)
      // // {
      // //    if (rx > 0.01f) {
      // //       rx -= 0.01f;
      // //    } else if (rx < -0.01f) {
      // //       rx += 0.01f;
      // //    } else {
      // //       rx = 0.0f;
      // //    }
      // // }
      // collectRotations.x = 0.0f;
   }


   void strafe_x_axis() // slide left and right but maintain quaternion rotation
   {
      // Determine strafe direction based on input

      // if (IsKeyDown(KEY_A))
      // {
      //    strafe += 0.01f;
      // }

      // if (IsKeyDown(KEY_D))
      // {
      //    strafe -= 0.01f;
      // }

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
      if (IsKeyDown(KEY_UP)   || IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1) > 0) dy += 0.3f;
      if (IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1) > 0)  dy -= 0.3f;
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
      rotate_z_axis(); rotate_x_axis(); rotate_y_axis(); shootLasers(); 
   }

   void shootLasers() 
   {
      double elapsed = GetTime();
      forwardDirection = Vector3Transform({0.0f, 0.0f, -1.0f}, QuaternionToMatrix(collectRotations));
      forwardDirection = Vector3Normalize(forwardDirection);

      //START shots
      if (IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_2) > 0)
      {
         Vector3 shipWorldPos = {cubeSpace.m12, cubeSpace.m13, cubeSpace.m14};
         // Calculate forward direction vector based on quaternions

         // Create and store the laser with an initial speed (e.g., 0.5f)
         float laserSpeed = 3.5f; // Set the desired speed of the laser
         lasers laser = lasers(shipWorldPos, forwardDirection, laserSpeed);
         lasersList.push_back(laser);
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

   void makeGlobal()
   {
      playerPosition = {cubeSpace.m12, cubeSpace.m13, cubeSpace.m14};
      playerRotation = forwardDirection;
   }

};
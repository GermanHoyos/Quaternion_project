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
   Model ship = LoadModel("C:\\Users\\Hoyos\\Desktop\\C++ Runner\\raylib_quaternion_example\\home\\src\\assets\\ship.obj");
   Texture2D texture = LoadTexture("C:\\Users\\Hoyos\\Desktop\\C++ Runner\\raylib_quaternion_example\\home\\src\\assets\\t1.png");
   Vector3 ship_init_pos = {0.0f, 0.0f, 0.0f};
   Vector3 shipWorldPos;
   float    x, y, z, dx, dy, dz, rx, ry, rz;
   float speed1 = 1.0f; float speed2 = 0.5f;
   float speed3 = 0.1f; float thrus1 = 0.0f;
   float thrus2 = 0.3f; float thrus3 = 3.4f;
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
         rlPushMatrix();                                                               // save current state of matrix untill [pop]
         apply_rotation_axis();                                                        // apply quaternion math to [rx] [ry] [rz] based on start point of 0 0 0
         accelerate_foward();                                                          // apply acceleration maths to directional variable [dx] [dy] [dz]
         Quaternion resultRotation = QuaternionMultiply(rotation, collectRotations);   // store above quaternion in a variable
         // SHARE ROTATIONS [START]
         // playerRotation = resultRotation;

         // SHARE ROTATIONS [END]
         cubeSpace = QuaternionToMatrix(resultRotation);                               // calculate how to apply quaternion to this current matrix
         cubeSpace.m12 += dx;                                                          // [x] axis acceleration
         cubeSpace.m13 += dy;                                                          // [y] axis acceleration
         cubeSpace.m14 += dz;                                                          // [z] axis acceleration
         rlMultMatrixf(MatrixToFloat(cubeSpace));                                      // apply all above culculations to this current matrix
         DrawModel(ship, ship_init_pos, 3.0f,  WHITE);                                 // blender made model
         DrawSphere({thrus1,thrus2,thrus3}, radis1, {cR,cG,cB,cAlpha});
         //DrawGrid(5, 1.0f); // shifted matrix
         rlPopMatrix();                                                                // revert back to state before [push]
      // POP MATRIX // REVERT BACK TO WORLD SPACE MATRIX

      // RESETS
      rotationDelta    = QuaternionIdentity();
      collectRotations = QuaternionIdentity();                                      // reset the collectRotations quaternion to 0 0 0
      rotation         = QuaternionIdentity();
      thrus1 = 0.0f; thrus2 = 0.3f;

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

   void shootLasers() {
      if (IsKeyPressed(KEY_SPACE)) 
      {
        Vector3 shipWorldPos = {cubeSpace.m12, cubeSpace.m13, cubeSpace.m14};
        // Calculate forward direction vector based on quaternions
        Vector3 forwardDirection = Vector3Transform({0.0f, 0.0f, -1.0f}, QuaternionToMatrix(collectRotations));
        forwardDirection = Vector3Normalize(forwardDirection);
        // Create and store the laser with an initial speed (e.g., 0.5f)
        float laserSpeed = 1.5f; // Set the desired speed of the laser
        lasers laser = lasers(shipWorldPos, forwardDirection, laserSpeed);
        lasersList.push_back(laser);
      }
   }

   void accelerate_foward()
   {
      Vector3 fowardDirection = Vector3Transform({0.0f, 0.0f, -1.0f}, QuaternionToMatrix(collectRotations));
      playerDirection = fowardDirection;
      dx += fowardDirection.x * speed3; 
      dy += fowardDirection.y * speed3; 
      dz += fowardDirection.z * speed3; 
   }

   void apply_rotation_axis()
   {
      rotate_z_axis(); rotate_x_axis(); rotate_y_axis(); shootLasers();
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
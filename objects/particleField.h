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

class particleField;

vector<particleField> pField;

class particleField
{
   public:
   Vector3 position; // {0.0f,0.0f,0.0f}
   int     pR = 255; // red
   int     pG = 255; // green
   int     pB = 255; // blue
   int     pA = 255; // alpha
   Color      myRGB; // {255,255,255,255}

   particleField()
   {
      // Randomize position
      // /*[x]*/ random_device rd_1; mt19937 gen_1(rd_1()); uniform_real_distribution<float> dis_1(-40.0f, 40.0f); position.x = dis_1(gen_1);
      // /*[y]*/ random_device rd_2; mt19937 gen_2(rd_2()); uniform_real_distribution<float> dis_2(-40.0f, 40.0f); position.y = dis_2(gen_2);
      // /*[z]*/ random_device rd_3; mt19937 gen_3(rd_3()); uniform_real_distribution<float> dis_3(-40.0f, 40.0f); position.z = dis_3(gen_3);

      // position.x = playerPosition.x;
      // position.y = playerPosition.y;
      // position.z = playerPosition.z + 2.0f;

      // Randomize positions based on player position
      


   }

   void draw()
   {
      //Alpha will be based on distance from player:
      DrawSphere({position.x,position.y,position.z}, 0.2f, WHITE);
   }

};

// Attempt at autonomous particles
void createFieldOne()
{
   // Load particles
   particleField particle = particleField();
   pField.push_back(particle);

   for (int i = 0; i < 10; i++)
   {
      particleField particle = particleField();
      pField.push_back(particle);
   }
}

// Attempt at player driven particles
void createFieldTwo()
{
   particleField particle = particleField();
   pField.push_back(particle);

   // If list is greater than X length, remove first element from the list
   if (pField.size() > 9)
   {
      pField.erase(pField.begin());
   }
}

// Attempt at random particles in space
void createRandomField()
{

}
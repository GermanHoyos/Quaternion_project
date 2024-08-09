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


float lerp(float a, float b, float t)
{
   return a + t * (b - a);
}

float normalizedLerp(float a, float b, float t, float minT, float maxT)
{
   float normalizedT = (t -minT) / (maxT - minT);
   return lerp(a, b, normalizedT);
}


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
   Texture2D texture_p = LoadTexture("C:\\Users\\Hoyos\\OneDrive\\Desktop\\C++ Runner\\raylib_quaternion_example\\home\\src\\assets\\particle_sprite.png");
   int alpha = 90;
   Color my_tint = {255, 255, 255, static_cast<unsigned char>(alpha)};

   particleField()
   {
      // Randomize position
      /*[x]*/ random_device rd_1; mt19937 gen_1(rd_1()); uniform_real_distribution<float> dis_1(-1000.0f, 1000.0f); 
      /*[y]*/ random_device rd_2; mt19937 gen_2(rd_2()); uniform_real_distribution<float> dis_2(-1000.0f, 1000.0f); 
      /*[z]*/ random_device rd_3; mt19937 gen_3(rd_3()); uniform_real_distribution<float> dis_3(-1000.0f, 1000.0f); 

      // Randomize positions based on player position
      position.x = dis_1(gen_1) + playerPosition.x;
      position.y = dis_2(gen_2) + playerPosition.y;
      position.z = dis_3(gen_3) + playerPosition.z;
      
   }

   void draw()
   {
      // Get distance from ship
      Vector3 distanceFromShip = Vector3Subtract({position.x, position.y, position.z}, {playerPosition.x, playerPosition.y, playerPosition.z});
      float distance_p = Vector3Length(distanceFromShip);

      // Calculate the alpha value based on the distance
      float minDistance = 1000.0f; // Minimum distance for alpha calculation
      float maxDistance = 0.0f; // Maximum distance for alpha calculation
      float minAlpha = 0.0f; // Minimum alpha value (fully transparent)
      float maxAlpha = 155.0f; // Maximum alpha value (fully opaque)
      float alpha = normalizedLerp(minAlpha, maxAlpha, distance_p, minDistance, maxDistance);

      if (distance_p < 350.0f)
      {
         alpha = 0.0f;
      }      

      // Update the tint color with the new alpha value
      my_tint.a = static_cast<unsigned char>(alpha);

      DrawBillboard(camera, texture_p, {position.x,position.y,position.z}, 1.0f, my_tint);
   }

};

// Attempt at autonomous particles
void createFieldOne()
{

   // If the list is not empty, empty it
   // this can be commented out though to simply generate more and more
   // particles (kinda like a trail)
   if (!pField.empty()) {
      pField.clear();
   }

   for (int i = 0; i < 1000; i++)
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



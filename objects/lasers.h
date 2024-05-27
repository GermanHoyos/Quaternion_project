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

class lasers;

vector<lasers> lasersList;

Vector3  worldCenter = {0.0f,0.0f,0.0f};

static int laserIds = 0;

class lasers 
{
public:

   int      myLaserId;
   Vector3  currentPos; 
   Vector3  startPos, endPos, velocity; // for shapes
   Color    color    = WHITE;
   Color    changed  = GREEN;

   lasers(Vector3 currentPos, Vector3 forwardDirection, float speed)
   : currentPos(currentPos), endPos(Vector3Add(currentPos, Vector3Scale(forwardDirection, 4.0f))), velocity(Vector3Scale(forwardDirection, speed)) 
   {
      myLaserId = laserIds++;
      startPos = currentPos;
   }

   void selfDestruct()
   {
      // Calculate distance from world center
      Vector3 distanceFromShotStart = Vector3Subtract(currentPos, startPos);
      float distance = Vector3Length(distanceFromShotStart);
  
      if (distance > 600)
      {
         // Find the laser in the list by its ID and remove it
         for (auto laser = lasersList.begin(); laser != lasersList.end(); ++laser)
         {
            if (laser->myLaserId == myLaserId)
            {
               lasersList.erase(laser);
               break; // Exit the loop after erasing the laser
            }
         }
      }
   }

   void draw() 
   {
      currentPos = Vector3Add(currentPos, velocity);
      endPos = Vector3Add(endPos, velocity);
      DrawSphere(currentPos, 0.5f, this->color);
      selfDestruct();
   }

};



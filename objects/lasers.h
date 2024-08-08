#include "../include/MasterHeader.h"

//class lasers;

//vector<lasers> lasersList;

static int laserIds = 0;

class lasers 
{
public:

   Quaternion collectRotations = QuaternionIdentity();
   Quaternion rotationDelta    = QuaternionIdentity();
   Quaternion reflectionVector = QuaternionIdentity();
   Vector3    currentPos; 
   Vector3    startPos; 
   Vector3    endPos;
   Vector3    velocity; 
   Vector3    newForwardDirection;
   Vector3    distanceFromShotStart;
   Vector3    laserVisual;
   Color      myColor     = RED;
   Color      changed     = GREEN;
   int        myLaserId;
   float      laserLength = 20.0f;
   bool       collidedWithObject = false;

   // Arugments passed at invokation
   lasers(Vector3 currentPos, Vector3 forwardDirection, float speed)
   : 
   currentPos(currentPos),
   endPos(Vector3Add(currentPos, Vector3Scale(forwardDirection, 4.0f))),
   velocity(Vector3Scale(forwardDirection, speed)) 
   {
      myLaserId = laserIds++;
      startPos = currentPos;
   }

   void selfDestruct()
   {
      // Calculate distance from shot start
      distanceFromShotStart = Vector3Subtract(currentPos, startPos);
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

   // THE BELOW ONLY SENDS THE BULLET IN THE SURFACE NORMALS DIRECTION AND DOES NO HAVE A REFLECTION VECTOR
   // void changeDirection(Vector3 startOfLine, Vector3 endOfLine)
   // {
   //    if (!collidedWithObject)
   //    {
   //       // Calculate the new forward direction vector
   //       Vector3 newForwardDirection = Vector3Subtract(endOfLine, startOfLine);
         
   //       // Normalize the new forward direction vector
   //       newForwardDirection = Vector3Normalize(newForwardDirection);
         
   //       // Update the laser's velocity with the new forward direction
   //       velocity = Vector3Scale(newForwardDirection, Vector3Length(velocity));
         
   //       // Update the end position based on the new forward direction
   //       endPos = Vector3Add(currentPos, Vector3Scale(newForwardDirection, laserLength));

   //       //Stop listening for collisions
   //       collidedWithObject = true;
   //    }
   // }

   // THE BELOW TAKES INTO ACCOUNT REFLECTION VECTORS
   void changeDirection(Vector3 startOfLine, Vector3 endOfLine)
   {
      if (!collidedWithObject)
      {
         // My current vector of travel, otherwise known as (a)
         // Velocity is the direction vector, line 115
         Vector3 a = velocity; 

         // The surface normal of the vector the bullet collided with, otherwise known as (b)
         Vector3 b = Vector3Normalize(Vector3Subtract(endOfLine, startOfLine));

         // Calculate the dot product a * b
         float dotProduct = Vector3DotProduct(a, b);

         // Calculate the reflection vector r = a - 2(a * b)b
         // "Scale" can be interpreted as "multiply by"
         Vector3 scaledNormal = Vector3Scale(b, 2.0f * dotProduct);
         Vector3 r = Vector3Subtract(a, scaledNormal);

         // Normalize the reflection vector
         newForwardDirection = Vector3Normalize(r);

         // Update the laser's velocity with the new forward direction
         velocity = Vector3Scale(newForwardDirection, Vector3Length(velocity));

         // Stop listening for collisions. (if this is left false then the ball will be trapped)
         collidedWithObject = true;
      }
   }

   void draw() 
   {

      // Get 3D line points
      currentPos = Vector3Add(currentPos, velocity);   // Ultimatly velocity is the direction vector
      //endPos = Vector3Add(endPos, velocity);         // Get the end point of the 3d line

      // Check for direction change
      //changeDirection(); 

      // Draw a 3D line
      //DrawLine3D(currentPos, endPos, WHITE);

      // Draw a 3D sphere at the start point
      DrawSphere(currentPos, 0.4f, myColor);

      // Check for self destruct
      selfDestruct();

   }

};





// rotationDelta = QuaternionFromAxisAngle({ 1.0f, 0.0f, 0.0f }, DEG2RAD * 90 );
// collectRotations = QuaternionMultiply(collectRotations, rotationDelta);
// velocity = Vector3Transform({0.0f, 0.0f, -1.0f}, QuaternionToMatrix(collectRotations));  


// Visualize a laser by taking a starting point then getting a vector 
// representing direction and then adding a arbitrary length to the vector
// this gives you a start point and an end point in 3d space. With these
// two points you can create laser animations, or bullets etc.. etc..
// laserVisual = Vector3Add(currentPos, Vector3Scale(fwrdDirection, laserLength));
// DrawLine3D(currentPos, laserVisual, WHITE);
// Line Start:
// DrawSphere((Vector3){currentPos.x, currentPos.y, currentPos.z}, 0.2f, RED);

// Line End:
// DrawSphere((Vector3){laserVisual.x, laserVisual.y, laserVisual.z}, 0.5f, WHITE);
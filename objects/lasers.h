#include "../include/MasterHeader.h"

static int laserIds = 0;

class lasers 
{
public:
    Quaternion collectRotations = QuaternionIdentity();
    Quaternion rotationDelta    = QuaternionIdentity();
    Quaternion reflectionVector = QuaternionIdentity();
    Matrix     laserMatrix      = MatrixIdentity();
    Vector3    currentPos; 
    Vector3    startPos; 
    Vector3    endPos;
    Vector3    velocity; 
    Vector3    newForwardDirection;
    Vector3    distanceFromShotStart;
    Vector3    laserVisual;
    Color      myColor     = GREEN;
    Color      changed     = GREEN;
    int        myLaserId;
    float      laserLength = 30.0f;
    bool       collidedWithObject = false;
    float      elongationDistance = 8.0f;  // How much farther you want to move endPos

    // Arguments passed at invocation
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
        // Randomize colors (including alpha)
        random_device rd_1; mt19937 gen_1(rd_1()); uniform_real_distribution<float> dist_r(0.0f, 255.0f);
        random_device rd_2; mt19937 gen_2(rd_2()); uniform_real_distribution<float> dist_g(0.0f, 255.0f);
        random_device rd_3; mt19937 gen_3(rd_3()); uniform_real_distribution<float> dist_b(0.0f, 255.0f);
        random_device rd_a; mt19937 gen_a(rd_a()); uniform_real_distribution<float> dist_a(0.0f, 255.0f);

        unsigned char r = static_cast<unsigned char>(dist_r(gen_1));
        unsigned char g = static_cast<unsigned char>(dist_g(gen_2));
        unsigned char b = static_cast<unsigned char>(dist_b(gen_3));
        unsigned char a = static_cast<unsigned char>(dist_a(gen_a));


        // Get 3D line points
        currentPos = Vector3Add(currentPos, velocity);   // Update the current position
        endPos = Vector3Add(currentPos, velocity);       // Calculate the end position

        // Extend the endPos by elongationDistance
        Vector3 direction = Vector3Normalize(Vector3Subtract(endPos, currentPos));
        Vector3 extendedEndPos = Vector3Add(endPos, Vector3Scale(direction, elongationDistance));

        // Calculate the cylinder's orientation matrix
        Vector3 axis = Vector3Normalize(Vector3Subtract(extendedEndPos, currentPos));
        float length = Vector3Length(Vector3Subtract(extendedEndPos, currentPos));
        
        // Default axis of the cylinder in model space is along the Y-axis (0, 1, 0)
        Vector3 defaultAxis = {0.0f, 1.0f, 0.0f};
        Vector3 rotationAxis = Vector3CrossProduct(defaultAxis, axis);
        float rotationAngle = acosf(Vector3DotProduct(defaultAxis, axis));

        Matrix laserMatrix = MatrixIdentity();

        if (Vector3Length(rotationAxis) != 0)  // Check if we actually need to rotate
        {
            laserMatrix = MatrixRotate(rotationAxis, rotationAngle);
        }

        // Move the cylinder to startPos
        Matrix translationMatrix = MatrixTranslate(currentPos.x, currentPos.y, currentPos.z);
        laserMatrix = MatrixMultiply(laserMatrix, translationMatrix);

        // Draw the cylinder with the tip at the start point
        rlPushMatrix();
        rlMultMatrixf(MatrixToFloat(laserMatrix));
        //DrawCylinder((Vector3){0, 0, 0}, 1.0f, 0.0f, length, 6, {255,0,0,255});  // Swap base and top radii
        DrawCylinderWires((Vector3){0,0,0},0.2f, 0.0f, length, 6, {255,255,255,a});
        rlPopMatrix();

        // Check for self destruct
        selfDestruct();
}


};
























/*

#include "raylib.h"

int main() {
    InitWindow(800, 600, "Elongate Capsule Example");

    Vector3 currentPos = {0.0f, 0.0f, 0.0f};
    Vector3 endPos = {2.0f, 0.0f, 0.0f};
    float elongationDistance = 3.0f;  // How much farther you want to move endPos

    // Calculate direction from currentPos to endPos
    Vector3 direction = Vector3Subtract(endPos, currentPos);

    // Normalize the direction vector
    direction = Vector3Normalize(direction);

    // Scale the direction vector by the desired elongation distance
    Vector3 elongationVector = Vector3Scale(direction, elongationDistance);

    // Calculate the new endPos
    Vector3 newEndPos = Vector3Add(endPos, elongationVector);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the original capsule
        DrawCapsule(currentPos, endPos, 1.2f, 8, 8, GRAY);

        // Draw the elongated capsule
        DrawCapsule(currentPos, newEndPos, 1.2f, 8, 8, GOLD);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

*/

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
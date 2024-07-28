#include "../include/MasterHeader.h"

class panel;

vector<panel> panelList;

class panel
{
    public:
    Vector3 myPosition; // passed position from invoker
    Vector3 planesPos; // actual plane position within matrix
    Matrix  myPlane = MatrixIdentity(); // matrix of draw context
    Color   myColor = {255,255,255,255};
    bool    wasHit = false;
    bool    animActive = false;
    float   amplitude = 3.5f;
    float   tuneAmplitude = 3.5f;
    float   waveYAxis = 0.0f;

    panel(Vector3 passedPosition) 
    : myPosition(passedPosition)
    {
        myPlane.m12 = myPosition.x;
        myPlane.m13 = myPosition.y;
        myPlane.m14 = myPosition.z;
        planesPos.x = 0.0f;
        planesPos.y = 0.0f;
        planesPos.z = 0.0f;
    }
    
    void draw()
    {
        rlPushMatrix();

        //DrawSphereWires(myPosition, 2.0f, 8, 8, GREEN); // radial hit box

        rlRotatef(90, 1.0f, 0.0f, 0.0f);

        rlMultMatrixf(MatrixToFloat(myPlane));

        DrawPlane(planesPos, {4.0f,4.0f}, myColor);

        detectCollisions();

        hitAnimation();

        rlPopMatrix();
    }

    void detectCollisions()
    {
        for (auto& laser : lasersList)
        {
            float distanceOfLaser = std::sqrt
            (
                (laser.currentPos.x - myPosition.x) * (laser.currentPos.x - myPosition.x) +
                (laser.currentPos.y - myPosition.y) * (laser.currentPos.y - myPosition.y) +
                (laser.currentPos.z - myPosition.z) * (laser.currentPos.z - myPosition.z)
            );
            
            if (laser.currentPos.z < -19.0f && laser.currentPos.z > -21.0f) // this is a slice of  3d world space
            {
                if (distanceOfLaser < 2.0f)
                {

                    if (!wasHit)
                    {
                        wasHit = true;
                    }

                    if (wasHit) // reset if struck again, basicly the latest wave wins and start anim over again
                    {
                        waveYAxis = 0.0f;
                        amplitude = tuneAmplitude;
                        myColor.g = 255;
                        myColor.b = 255;
                    }

                    //wasHit = true;
                    myColor = {255,0,0,255}; // change red
                    wave newWave = wave(myPosition);
                    waveList.push_back(newWave);
                }
            }
        }

        for (auto& wave : waveList)
        {
            // Calculate the distance between the current position and the center of the wave sphere
            float distanceOfWave = std::sqrt
            (
                (myPosition.x - wave.myPosition.x) * (myPosition.x - wave.myPosition.x) +
                (myPosition.y - wave.myPosition.y) * (myPosition.y - wave.myPosition.y) +
                (myPosition.z - wave.myPosition.z) * (myPosition.z - wave.myPosition.z)
            );

            // Check if the current position is within the wave's radius
            if (distanceOfWave < wave.myRadius && distanceOfWave > wave.innerSphereRadius)
            {
                if (!wasHit)
                {
                    wasHit = true;
                }

                if (wasHit) // reset if struck again, basically the latest wave wins and start anim over again
                {
                    waveYAxis = 0.0f;
                    amplitude = tuneAmplitude;
                    myColor.g = 255;
                    myColor.b = 255;
                }
            }
        }
    }

    void hitAnimation()
    {
        if (wasHit)
        {
            // tune color
            // myColor.r += 0; // keep the same (red)

            /*if (myColor.g <= 255)*/ myColor.g += 1;
            /*if (myColor.b <= 255)*/ myColor.b += 1;
        


            // waveYAxis was declared in the class as (float waveYAxis = 0.0f;)
            waveYAxis -= 0.10f;
            planesPos.y = amplitude * sinf(waveYAxis);

            // tune the animation down and reset all inputs
            amplitude -= 0.01f;
            if (amplitude <= 0.0f)
            {
                // myColor.r += 0; // keep the same (red)
                myColor.g = 255;
                myColor.b = 255;
                wasHit    = false;
                waveYAxis = 0.0f;
                amplitude = tuneAmplitude;
            }
        }
    }

};

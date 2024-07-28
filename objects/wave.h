#include "../include/MasterHeader.h"

class wave;

vector<wave> waveList;

static int idIncrementer;

void removeWaveIfDead();

class wave 
{
    public:
    Vector3 myPosition;
    float   myRadius = 2.0f;
    float   innerSphereRadius = 1.0;
    int     myID;
    Color   myColor = {255,255,255,255};
    bool    myDeath = false;

    wave(Vector3 passedPosition)
    : myPosition(passedPosition), myID(idIncrementer++)
    {
        // further inits
    }

    void draw()
    {
        myRadius += 0.8f;
        innerSphereRadius += 0.8f;
        DrawSphereWires(myPosition, myRadius, 8, 8, GREEN); // radial hit box
        //DrawSphere(myPosition, innerSphereRadius, RED); // radial hit box
        deathCheck();
        removeWaveIfDead();
    }

    void deathCheck()
    {
        if (myRadius > 80)
        {
            myDeath = true;

        }
    }
};

void removeWaveIfDead()
{
    for (auto wave = waveList.begin(); wave != waveList.end(); ++wave)
    {
        if (wave->myRadius > 80)
        {
            waveList.erase(wave);
            break;
        }
    }
}





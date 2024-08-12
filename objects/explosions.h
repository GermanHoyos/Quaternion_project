#include "../include/MasterHeader.h"

class explosions 
{
    public:
    Vector3 myPosition;
    Vector3 rotAxis;
    float   rotAngle;
    float   maxRadius;   // Declare maxRadius here
    float   myRadius = 2.0f;
    bool    myDeath = false;

    explosions(Vector3 passedPosition)
    : myPosition(passedPosition)
    {
        // further inits
        rotAxis  = {0.0f,0.0f,0.0f};
        rotAngle = 0.0f;
        // Randomize size for maxRadius
        random_device rd_r; mt19937 gen_r(rd_r()); uniform_real_distribution<float> dist(4.0f, 20.0f); 
        maxRadius = dist(gen_r);  // Initialize maxRadius
    }

    void draw()
    {
        // Randomize colors (including alpha)
        random_device rd_1; mt19937 gen_1(rd_1()); uniform_real_distribution<float> dist_r(0.0f, 255.0f);
        random_device rd_2; mt19937 gen_2(rd_2()); uniform_real_distribution<float> dist_g(0.0f, 255.0f);
        random_device rd_3; mt19937 gen_3(rd_3()); uniform_real_distribution<float> dist_b(0.0f, 255.0f);
        random_device rd_a; mt19937 gen_a(rd_a()); uniform_real_distribution<float> dist_a(100.0f, 255.0f);

        unsigned char r = static_cast<unsigned char>(dist_r(gen_1));
        unsigned char g = static_cast<unsigned char>(dist_g(gen_2));
        unsigned char b = static_cast<unsigned char>(dist_b(gen_3));
        unsigned char a = static_cast<unsigned char>(dist_a(gen_a));

        Color shiftingColors = {r, g, b, a};

        DrawSphereEx(myPosition, myRadius, 300, 10, shiftingColors);
        
        myRadius += 2.0f;
        deathCheck();
    }


    void deathCheck()
    {
        // if radius > than X self destruct ~explosions
        for (auto explosion = explosionsList.begin(); explosion != explosionsList.end(); ++explosion)
        {
            if(explosion->myRadius > maxRadius)
            {
                explosionsList.erase(explosion);
                break;
            }
        }
    }

};
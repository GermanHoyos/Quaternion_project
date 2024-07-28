#pragma once

// Standards + Raylib
   #include <cstdlib>
   #include <iostream>
   #include <raylib-cpp.hpp>
   #include <raylib.h>
   #include <cmath> // absolute val
   #include <algorithm>
   #include <math.h>
   #include <stdlib.h>
   #include <string>
   #include <iomanip>
   #include <vector>
   #include <random>
   #include <raymath.h>
   using namespace std;

// Globals
   Vector3 playerPosition;
   Vector3 playerRotation;
   Camera camera = { 0 };
   // CONTROLLER ENTRY POINT
   int gamepad = 0;

// Customs
   // template <typename T>
   // using dynamicList = vector<T>;
   #include "rlgl.h"
   #include "reasings.h"
   #include "timeManager.h"
   #include "../objects/lasers.h"
   #include "../objects/myPlayer.h"
   #include "../objects/asteroids.h"
   #include "../objects/objectManager.h"
   #include "../objects/particleField.h"
   #include "../objects/wave.h"
   #include "../objects/boundery.h"








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
   Vector3 cubeNormals[12] = {
      { 00.0f, 00.0f, 00.0f},  //0  Top       face normal start
      { 00.0f, 00.0f, 00.0f},  //1  Top       face normal end
      { 00.0f, 00.0f, 00.0f},  //2  Bottom    face normal start
      { 00.0f, 00.0f, 00.0f},  //3  Bottom    face normal end
      { 00.0f, 00.0f, 00.0f},  //4  Left      face normal start
      { 00.0f, 00.0f, 00.0f},  //5  Left      face normal end
      { 00.0f, 00.0f, 00.0f},  //6  Right     face normal start
      { 00.0f, 00.0f, 00.0f},  //7  Right     face normal end
      { 00.0f, 00.0f, 00.0f},  //8  Forward   face normal start
      { 00.0f, 00.0f, 00.0f},  //9  Forward   face normal end
      { 00.0f, 00.0f, 00.0f},  //10 Back      face normal start
      { 00.0f, 00.0f, 00.0f}   //11 Back      face normal end
   };

float lerp(float a, float b, float t)
{
   return a + t * (b - a);
}

float normalizedLerp(float a, float b, float t, float minT, float maxT)
{
   float normalizedT = (t -minT) / (maxT - minT);
   return lerp(a, b, normalizedT);
}
   class lasers;
   vector<lasers> lasersList;
   class reflectiveCube;
   vector<reflectiveCube> cubeList;
   Vector3 playerPosition;
   Vector3 playerRotation;
   Camera camera = { 0 };
   int gamepad = 0; // Controller entry point

// Customs
   // template <typename T>
   // using dynamicList = vector<T>;
   #include "rlgl.h"
   #include "rlights.h"
   #include "reasings.h"
   #include "timeManager.h"
   #include "../objects/lasers.h"
   #include "../objects/reflectiveCube.h"
   #include "../objects/myPlayer.h"
   #include "../objects/asteroids.h"
   #include "../objects/objectManager.h"
   #include "../objects/particleField.h"
   #include "../objects/wave.h"
   #include "../objects/boundery.h"
   #include "../objects/arenaStrip.h"










#pragma once
#include "MasterHeader.h" // Include necessary headers to access GetTime() and other functions

class TimeClass
{
public:
   static int     seconds;
   static int     minutes;
   static int     gameTotalSeconds;
   static int     framesCounterResetable;
   static float   deltaTime;

   static float getDeltaTime()
   {
      return GetFrameTime();
   }

   static int getFPS()
   {
      return GetFPS(); // Assuming GetFPS() returns the frames per second
   }

   static void displayGameTime()
   {
      // Frames since game start
      framesCounterResetable++;

      // Get time in seconds for last frame drawn (delta time)
      deltaTime = GetFrameTime();

      // Get elapsed time in seconds since InitWindow()
      int currentTime = GetTime();

      if (gameTotalSeconds != currentTime)
      {
         seconds++;
         if (seconds == 60)
         {
            seconds = 0;
            minutes++;
         }
         gameTotalSeconds = currentTime;
      }

      int FPS = GetFPS();
      string fps_str = "FPS: " + to_string(FPS);
      string seconds_str = "Seconds: " + to_string(seconds);
      string minutes_str = "Minutes: " + to_string(minutes);
      string deltaTime_str = "Delta Time: " + to_string(deltaTime);
      string framesCounterResetable_str = "Frames: " + to_string(framesCounterResetable);

      DrawLine(210, 0, 210, 1000, GREEN);
      DrawText(fps_str.c_str(), 2, 2, 20, GREEN);
      DrawText(seconds_str.c_str(), 2, 20, 20, GREEN);
      DrawText(minutes_str.c_str(), 2, 40, 20, GREEN);
      DrawText(deltaTime_str.c_str(), 2, 60, 20, GREEN);
      DrawText(framesCounterResetable_str.c_str(), 2, 80, 20, GREEN);

   }
};

// Define outside the class
int   TimeClass::seconds = 0;
int   TimeClass::minutes = 0;
int   TimeClass::framesCounterResetable = 0;
int   TimeClass::gameTotalSeconds = GetTime(); // Initialize gameTotalSeconds
float TimeClass::deltaTime = 0;


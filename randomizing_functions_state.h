#pragma once

// Initialize a structure that will hold the output of randomizeTime
struct time_outputs {
  uint32_t cumulativeTime;
  int count;
};

static void RandomizeBehaviors(void);
uint32_t RandomDuration(uint32_t MinTime, uint32_t MaxTime);
int RandomSpeeds(int* Speeds[]);
static void randomizeAccel(int count);
void Report(float targetSpeed, int activityTag, String message);
void HeaderReport(int count); 
int getMinSpeed(void);

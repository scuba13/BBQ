#ifndef SYSTEM_STATUS_H
#define SYSTEM_STATUS_H

#define NUM_SAMPLES 20
#define MOVING_AVERAGE_SIZE 180



struct SystemStatus {
    int tempCalibration = 0;
    int lastPos = -1;
    int currentPos = 0;
    int bbqTemperature = 0;
    int minBBQTemp = 30;
    int maxBBQTemp = 200;
    float tempSamples[NUM_SAMPLES];
    int nextSampleIndex = 0;
    int numSamples = 0;
    bool isRelayOn = false;
    unsigned long lastDebounceTime = 0;
    unsigned long lastTempUpdateMonitor = 0;
    bool monitorMode = false;
    float samples[MOVING_AVERAGE_SIZE];
    int sampleIndex;
    int avgNumSamples;
    bool startAverage = false;
    int averageTemp = 0;
    bool hasReachedSetTemp = false;
    int calibratedTemp;
};

#endif

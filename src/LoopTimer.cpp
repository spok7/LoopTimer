#include "LoopTimer.h"
#include "stdlib.h"

LoopTimer::LoopTimer(uint16_t range) {
    this->range = range;
    setupDifference = 0;
    numMinEntries = 1;
    numMaxEntries = 1;
    numMedianEntries = 1;
    loopDelay = 0;
    timeDelay = 0;

    timestamps = new unsigned long [range];
    loopCount = 0;
}

void LoopTimer::countSetupFromHere() {
    setupDifference = millis();
}

void LoopTimer::setNumMinEntries(uint8_t num) {
    numMinEntries = num;
}

void LoopTimer::setNumMaxEntries(uint8_t num) {
    numMaxEntries = num;
}

void LoopTimer::setNumMedianEntries(uint8_t num) {
    numMedianEntries = num;
}

void LoopTimer::delayByLoops(uint8_t num) {
    loopDelay = num;
}

void LoopTimer::delayByTime(uint32_t ms) {
    timeDelay = ms;
}

static void sort(unsigned long *list, uint8_t range) {
    unsigned long i, key, j;
    for (i = 1; i < range; ++i) {
        key = list[i];
        j = i - 1;
        while (j >= 0 && list[j] > key) {
            list[j + 1] = list[j];
            j = j - 1;
        }
        list[j + 1] = key;
    }
}

static unsigned long avg(unsigned long *list, uint8_t range) {
    unsigned long average = 0;
    for (int i = 0; i < range; ++i) average += list[i];
    return average / range;
}

void LoopTimer::update() {
    if (loopCount < range) {
        timestamps[loopCount] = millis();
    } else {

        for(int i = range - 1; i > 0; --i){
            timestamps[i] = timestamps[i] - timestamps[i - 1];
        }

        // sort(timestamps + 1, range - 1);

        Serial.begin(9600);
        Serial.print("Set up: ");
        Serial.println(timestamps[0]);
        Serial.print("Average: ");
        Serial.println(avg(timestamps + 1, range - 1));
        Serial.end();
    }
}
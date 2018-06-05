#include "LoopTimer.h"
#include "stdlib.h"

LoopTimer::LoopTimer(uint16_t range) {
    Serial.begin(9600);
    delay(3000);
    Serial.print(F("\nFree Memory: "));
    Serial.println(freeMemory(), DEC);

    this->range = range;
    setupDifference = 0;
    numMinEntries = 1;
    numMaxEntries = 1;
    numMedianEntries = 1;
    loopDelay = 0;
    timeDelay = 0;

    timestamps = new unsigned long [range];
    for (int i = 0; i < range; ++i) {
        Serial.print(timestamps[i] = i);
        Serial.print(F(" "));
    }

    loopCount = 0;

    Serial.println(F("\n\nSetup has initialized."));
    Serial.print(F("Range: "));
    Serial.println(range);

    Serial.print(F("Free Memory: "));
    Serial.println(freeMemory(), DEC);
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

static void LoopTimer::sort(unsigned long *list, uint8_t range) {
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

static unsigned long LoopTimer::avg(unsigned long *list, uint8_t range) {
    unsigned long total = 0;
    for (uint8_t i = 0; i < range; ++i) total += list[i];
    return total / range + (total % range == 0 ? 0 : 1);
}

void LoopTimer::update() {
    delay(2000);
    Serial.print(F("Loop: "));
    Serial.println(loopCount);
    if (loopCount < range) {
        timestamps[loopCount] = millis();
        Serial.print(F("  Timestamp: "));
        Serial.println(timestamps[loopCount]);
    }
    // else {

    //     for (int i = range - 1; i > 0; --i) timestamps[i] -= timestamps[i - 1];

    //     // sort(timestamps + 1, range - 1);

    //     // Serial.begin(9600);
    //     Serial.print("Set up: ");
    //     Serial.println(timestamps[0]);
    //     Serial.print("Average: ");
    //     Serial.println(avg(timestamps + 1, range - 1));
    //     // Serial.end();
    // }
    ++loopCount;
}
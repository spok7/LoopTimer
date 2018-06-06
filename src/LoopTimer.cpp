#include "LoopTimer.h"
#include "stdlib.h"
#include <MemoryFree.h>

LoopTimer::LoopTimer(uint16_t range, uint16_t buffer) {
    Serial.begin(1200);

    firstRun = true;
    runsLeft = 5;
    setupDifference = 0;
    numMinEntries = 1;
    numMaxEntries = 1;
    numMedianEntries = 1;
    loopDelay = 0;
    timeDelay = 0;

    this->range = ((freeMemory() - buffer) / 4 < range) ? (freeMemory() - buffer) / 4 : range;
    timestamps = new unsigned long [this->range];

    loopCount = 0;

    Serial.println(F("\n\nSetup has initialized."));
    Serial.print(F("Range (after deadband): "));
    Serial.println(this->range);

    Serial.print(F("Free Memory after setup: "));
    Serial.println(freeMemory(), DEC);
}

void LoopTimer::countSetupFromHere() {
    setupDifference = millis();
}

void LoopTimer::setNumRuns(uint8_t num) {
    runsLeft = num;
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

void LoopTimer::delayByLoops(uint16_t num) {
    loopDelay = num;
}

void LoopTimer::delayByTime(uint32_t ms) {
    timeDelay = ms;
}

static void LoopTimer::sort(unsigned long *list, uint16_t range) {
    unsigned long temp = 0;
    for (uint16_t i = 1; i < range; ++i) {
        for (uint16_t j = i; j > 0 && list[j] < list[j - 1]; --j) {
            temp = list[j];
            list[j] = list[j - 1];
            list[j - 1] = temp;
        }
    }
}

static unsigned long LoopTimer::avg(unsigned long *list, uint16_t range) {
    unsigned long total = 0;
    for (uint8_t i = 0; i < range; ++i) total += list[i];
    return total / range + (total % range == 0 ? 0 : 1);
}

void LoopTimer::update() {
    // Serial.print(F("Loop: "));  // debug
    // Serial.println(loopCount);  // debug
    if (loopCount < range) {
        timestamps[loopCount] = millis();
        // Serial.print(F("  Timestamp: "));       // debug
        // Serial.println(timestamps[loopCount]);  // debug
        ++loopCount;
    } else {

        if (firstRun) {
            Serial.print(F("\nSet up time: "));
            Serial.print(timestamps[0] - setupDifference);
            Serial.println(F("ms"));
            firstRun = false;
        }

        for (int i = 1; i < range; ++i) {
            Serial.print(timestamps[i]);
            Serial.print(F(" "));
        }

        Serial.println();

        for (int i = range - 1; i > 0; --i)
            timestamps[i] -= timestamps[i - 1];

        for (int i = 1; i < range; ++i) {
            Serial.print(timestamps[i]);
            Serial.print(F(" "));
        }

        Serial.println();

        sort(timestamps + 1, range - 1);

        for (int i = 1; i < range; ++i) {
            Serial.print(timestamps[i]);
            Serial.print(F(" "));
        }

        Serial.print(F("\nAverage: "));
        Serial.print(avg(timestamps + 1, range - 1));
        Serial.println(F("ms"));

        // val/count pairs
        uint16_t currentMean[] = {0, 0};
        uint16_t potentialMean[] = {0, 0};

        for (uint16_t i = 1; i < range; ++i) {
            if (timestamps[i] == currentMean[0]) {
                ++currentMean[1];
                if (currentMean[1] > potentialMean[1]) {
                    potentialMean[0] = currentMean[0];
                    potentialMean[1] = currentMean[1];
                }
            } else {
                currentMean[0] = timestamps[i];
                currentMean[1] = 1;
            }
        }

        Serial.print(F("Mean: "));
        Serial.print(potentialMean[0]);
        Serial.print(F("ms\tCount: "));
        Serial.print(potentialMean[1]);
        Serial.println(F("ms"));


        if (numMinEntries != 0) {
            Serial.print(F("Min(s): "));
            for (uint8_t i = 1; i < numMinEntries + 1; ++i) {
                Serial.print(timestamps[i]);
                Serial.print(F("ms "));
            }
        }

        if (numMaxEntries != 0) {
            Serial.print(F("\nMax(s): "));
            for (uint8_t i = range - 1; i > range - numMaxEntries - 1; --i) {
                Serial.print(timestamps[i]);
                Serial.print(F("ms "));
            }
        }

        Serial.println(F("\nDone."));

        loopCount = 1;
    }
}
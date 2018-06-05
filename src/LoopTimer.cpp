#include "LoopTimer.h"
#include "stdlib.h"

LoopTimer::LoopTimer(uint16_t range) {
    Serial.begin(9600);
    delay(3000);
    Serial.print(F("\nInitial Free Memory: "));
    Serial.println(freeMemory(), DEC);

    firstRun = true;
    this->range = range;
    setupDifference = 0;
    numMinEntries = 1;
    numMaxEntries = 1;
    numMedianEntries = 1;
    loopDelay = 0;
    timeDelay = 0;

    timestamps = new unsigned long [range];
    for (uint16_t i = 0; i < range; ++i) {
        Serial.print(timestamps[i] = i);
        Serial.print(F(" "));
    }

    loopCount = 0;

    Serial.println(F("\n\nSetup has initialized."));
    Serial.print(F("Range: "));
    Serial.println(range);

    Serial.print(F("Free Memory after setup: "));
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

void LoopTimer::delayByLoops(uint16_t num) {
    loopDelay = num;
}

void LoopTimer::delayByTime(uint32_t ms) {
    timeDelay = ms;
}

static void LoopTimer::sort(unsigned long *list, uint16_t range) {
    unsigned long i, key, j;
    for (i = 1; i < range; ++i) {
        key = list[i];
        j = i - 1;
        while (j >= 0 && list[j] > key) {
            list[j + 1] = list[j];
            --j;
        }
        list[j + 1] = key;
    }
}

static unsigned long LoopTimer::avg(unsigned long *list, uint16_t range) {
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
        ++loopCount;
    } else {

        if (firstRun) {
            Serial.print("Set up: ");
            Serial.println(timestamps[0] - setupDifference);
            firstRun = false;
        }

        Serial.print("Average: ");
        Serial.println(avg(timestamps + 1, range - 1));


        for (int i = range - 1; i > 0; --i)
            timestamps[i] -= timestamps[i - 1];
        sort(timestamps + 1, range - 1);

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

        Serial.print("Mean: ");
        Serial.print(potentialMean[0]);
        Serial.print("\tCount: ");
        Serial.println(potentialMean[1]);


        if (numMinEntries != 0) {
            Serial.print("Min(s):");
            for (uint8_t i = 1; i < numMinEntries + 1; ++i) {
                Serial.print(" ");
                Serial.print(timestamps[i]);
            }
        }

        if (numMaxEntries != 0) {
            Serial.print("Max(s):");
            for (uint8_t i = range - 1; i > range - numMaxEntries - 1; --i) {
                Serial.print(" ");
                Serial.print(timestamps[i]);
            }
        }

        loopCount = 1;
    }
}
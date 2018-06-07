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
    numModeEntries = 1;
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

LoopTimer* LoopTimer::countSetupFromHere() {
    setupDifference = millis();
    return this;
}

LoopTimer* LoopTimer::setNumRuns(uint8_t num) {
    runsLeft = num;
    return this;
}

LoopTimer* LoopTimer::setNumMinEntries(uint8_t num) {
    numMinEntries = (num > range) ? range : num;
    return this;
}

LoopTimer* LoopTimer::setNumMaxEntries(uint8_t num) {
    numMaxEntries = (num > range) ? range : num;
    return this;
}

LoopTimer* LoopTimer::setNumModeEntries(uint8_t num) {
    numModeEntries = num;
    return this;
}

LoopTimer* LoopTimer::delayByLoops(uint16_t num) {
    loopDelay = num;
}

LoopTimer* LoopTimer::delayByTime(uint32_t ms) {
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

static void LoopTimer::shift(uint16_t **modes, uint8_t amount, uint16_t potentialMode[]) {
    modes[amount - 1][0] = potentialMode[0];
    modes[amount - 1][1] = potentialMode[1];
    for (int j = amount - 2; j >= 0 && modes[j][1] < modes[j + 1][1]; --j) {
        potentialMode[0] = modes[j][0];
        potentialMode[1] = modes[j][1];
        modes[j][0] = modes[j + 1][0];
        modes[j][1] = modes[j + 1][1];
        modes[j + 1][0] = potentialMode[0];
        modes[j + 1][1] = potentialMode[1];
    }
}

static void LoopTimer::modesFromSorted(uint16_t **modes, uint8_t amount, unsigned long *list, uint16_t range) {

    for (uint8_t i = 0; i < amount; ++i) {
        modes[i][0] = 0;
        modes[i][1] = 0;
    }

    uint16_t potentialMode[] = {(uint16_t) list[0], 1};

    for (uint16_t i = 1; i < range; ++i) {
        if (list[i] == potentialMode[0]) {
            ++potentialMode[1];
        } else if (potentialMode[1] > modes[amount - 1][1]) {
            shift(modes, amount, potentialMode);
            potentialMode[0] = list[i];
            potentialMode[1] = 1;
        }

        if (i == range - 1 && potentialMode[1] > modes[amount - 1][1]) {
            shift(modes, amount, potentialMode);
        }
    }
}

static unsigned long LoopTimer::avg(unsigned long *list, uint16_t range) {
    unsigned long total = 0;
    for (uint8_t i = 0; i < range; ++i) total += list[i];
    return total / range + (total % range == 0 ? 0 : 1);
}

void LoopTimer::update() {

    if (loopCount < loopDelay || millis() < timeDelay) {
        ++loopCount;
        return;
    }
    
    if (runsLeft == 0) return;
    if (loopCount < range) {
        timestamps[loopCount] = millis();
        ++loopCount;
    } else {

        if (firstRun) {
            Serial.print(F("\nSet up time: "));
            Serial.print(timestamps[0] - setupDifference);
            Serial.println(F("ms"));
            firstRun = false;
        }

        // calculate differences
        for (int i = range - 1; i > 0; --i)
            timestamps[i] -= timestamps[i - 1];

        Serial.print(F("\n\nMean: "));
        Serial.print(avg(timestamps + 1, range - 1));
        Serial.println(F("ms"));

        sort(timestamps + 1, range - 1);

        Serial.print(F("Median: "));
        Serial.print(timestamps[range / 2]);
        Serial.println(F("ms"));

        if (numModeEntries != 0) {
            uint16_t **modes = new uint16_t*[numModeEntries];
            for (int i = 0; i < numModeEntries; ++i) {
                modes[i] = new uint16_t[2];
            }

            modesFromSorted(modes, numModeEntries, timestamps + 1, range - 1);

            Serial.print(F("Modes (val/count):"));
            for (int i = 0; i < numModeEntries; ++i) {
                Serial.print(F(" ["));
                Serial.print(modes[i][0]);
                Serial.print(F(", "));
                Serial.print(modes[i][1]);
                Serial.print(F("]"));
            }

            for (int i = 0; i < numModeEntries; ++i) {
                delete[] modes[i];
            }
            delete[] modes;
        }
        

        if (numMinEntries != 0) {
            Serial.print(F("\nMin(s): "));
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

        loopCount = 0;
        --runsLeft;
    }
}
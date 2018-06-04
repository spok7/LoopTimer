#include "LoopTimer.h"

LoopTimer::LoopTimer(uint16_t range) {
    this->range = range;
    setupDifference = 0;
    numMinEntries = 1;
    numMaxEntries = 1;
    numMedianEntries = 1;
    loopDelay = 0;
    timeDelay = 0;

    timestamps = new unsigned long [range];
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

void LoopTimer::update() {
    return;
}
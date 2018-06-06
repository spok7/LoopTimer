#ifndef LOOPTIMER_H
#define LOOPTIMER_H

#include <stdint.h>
#include <Arduino.h>
#define SPARERAM 50

class LoopTimer {
	private:
		bool firstRun;
		uint8_t runsLeft;
		uint16_t range;
		uint16_t loopCount;
		unsigned long setupDifference;
		uint8_t numMinEntries;
		uint8_t numMaxEntries;
		uint8_t numMedianEntries;
		uint16_t loopDelay;
		unsigned long timeDelay;
		unsigned long *timestamps;

		static void sort(unsigned long *list, uint16_t range);
		static unsigned long avg(unsigned long *list, uint16_t range);

	public:
		LoopTimer(uint16_t range, uint16_t buffer = 50);
		void countSetupFromHere();
		void setNumRuns(uint8_t);
		void setNumMinEntries(uint8_t);
		void setNumMaxEntries(uint8_t);
		void setNumMedianEntries(uint8_t);
		void delayByLoops(uint16_t);
		void delayByTime(uint32_t);
		void update();
};

#endif //LOOPTIMER_H
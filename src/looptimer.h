#ifndef LOOPTIMER_H
#define LOOPTIMER_H

#include <stdint.h>
#include <Arduino.h>
#include <MemoryFree.h>

class LoopTimer {
	private:
		uint8_t range;
		unsigned long setupDifference;
		uint8_t numMinEntries;
		uint8_t numMaxEntries;
		uint8_t numMedianEntries;
		uint8_t loopDelay;
		unsigned long timeDelay;

		unsigned long *timestamps;
		unsigned long *potato;
		uint8_t loopCount;

		static void sort(unsigned long *list, uint8_t range);
		static unsigned long avg(unsigned long *list, uint8_t range);

	public:
		LoopTimer(uint16_t range);
		void countSetupFromHere();
		void setNumMinEntries(uint8_t);
		void setNumMaxEntries(uint8_t);
		void setNumMedianEntries(uint8_t);
		void delayByLoops(uint8_t);
		void delayByTime(uint32_t);
		void update();
};

#endif //LOOPTIMER_H
#ifndef LOOPTIMER_H
#define LOOPTIMER_H

class LoopTimer {
	private:
		unsigned long setupDifference;
		

	public:
		LoopTimer(uint16_t range);
		countSetupFromHere();
		numMinEntries(uint8_t);
		numMaxEntries(uint8_t);
		numMedianEntries(uint8_t);
		delayByLoops(uint8_t);
		delayByTime(uint32_t);
		update();
}
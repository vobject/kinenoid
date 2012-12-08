#include "defines.h"
#include "moves.h"


/* calculate fading between 2 points */
inline uint8_t getFaded(uint8_t from, uint8_t to, uint8_t step) {
	if (from > to) {
		int16_t diff = from - to;
		diff = diff * step / MOVESTEPS;
		return from - diff;
	} else {
		int16_t diff = to - from;
		diff = diff * step / MOVESTEPS;
		return from + diff;
	}

}


/* get current servo Position */
int8_t movesGetServo(uint8_t servo) {
	uint8_t val = getFaded(servoData[servo].from, servoData[servo].to, servoData[servo].step);
	if (servo >= 9) {
		return 128 - val;
	} else {
		return val - 128;
	}

}






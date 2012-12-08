#include "defines.h"
#include "moves.h"
#include "control.h"

/* calculation */
#define NS_PER_CLK		(1000000000UL / F_CPU)
#define CLKS_PER_VAL	(uint8_t)(1200000UL / 256UL / NS_PER_CLK)
#define START_CLK		(900000UL / NS_PER_CLK)	// = 0,9 MS = circa start position


/* set the timer interrupt */
static void setNextServo() {

	static uint8_t curServo = 0;
	if (++curServo == SERVOS) {curServo = 0;}				// next servo

	uint16_t cycles;
	cycles = (servoData[curServo].root + movesGetServo(curServo)) * CLKS_PER_VAL;	// additional value

	if (curServo == SL_HIPROT) {
		xPort |= (1 << 0);
	} else if (curServo == SR_HIPROT) {
		xPort |= (1 << 1);
	} else if (curServo == S_HEAD) {
		xPort |= (1 << 2);
	} else if (curServo < 2) {
		lPort = (1 << curServo);
	} else if (curServo < 9) {
		lPort = (1 << (curServo - 1));
	} else if (curServo < 11) {
		rPort = (1 << (curServo - 9));
	} else {
		rPort = (1 << (curServo - 10));
	}

	TCNT1 = 0;
	OCR1A = START_CLK + cycles;								// set timer end value
}



/* servo control interrupt */
ISR (TIMER1_COMPA_vect) {
	lPort = 0x00;											// set complete ServoPort1 low
	rPort = 0x00;
	xPort &= ~(0b00000111);

	setNextServo();
}



/* initialize the controls */
inline void controlInit() {

	servoData[S_HEAD].root = 135;		// Servo #18

	servoData[SL_SHOULDER].root = 150;	// Servo #6
	servoData[SR_SHOULDER].root = 120;	// Servo #15

	servoData[SL_ARM].root = 120;		// Servo #7
	servoData[SR_ARM].root = 128;		// Servo #16

	servoData[SL_HAND].root = 135;		// Servo #8
	servoData[SR_HAND].root = 125;		// Servo #17

	servoData[SL_HIPROT].root = 145;	// Servo #2
	servoData[SR_HIPROT].root = 140;	// Servo #11

	servoData[SL_HIPSIDE].root = 124;	// Servo #0
	servoData[SR_HIPSIDE].root = 124;	// Servo #9

	servoData[SL_HIPUP].root = 130;		// Servo #1
	servoData[SR_HIPUP].root = 125;		// Servo #10

	servoData[SL_KNEE].root = 20;		// Servo #3
	servoData[SR_KNEE].root = 254;		// Servo #12

	servoData[SL_FOOTSIDE].root = 140;	// Servo #5
	servoData[SR_FOOTSIDE].root = 132;	// Servo #14

	servoData[SL_FOOT].root = 140;		// Servo #4
	servoData[SR_FOOT].root = 110;		// Servo #13

	/* set servo data to root (=128) */
	for (uint8_t i = 0; i < SERVOS; ++i) {
		servoData[i].from = 128;
		servoData[i].to = 128;
		servoData[i].step = 128;
	}

	TCCR1A = 0x00;
	TCCR1B = (1<<CS10);										// Normal Mode with full clock
	TIMSK = (1<<OCIE1A);						// enable Timer1 CompareA

	setNextServo();
}






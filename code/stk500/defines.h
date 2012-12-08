#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>


/* defines */
#define lPort PORTA
#define rPort PORTC
#define xPort PORTB


/* servos */
#define SERVOS			19

#define SL_HIPSIDE		0
#define SL_HIPUP		1
#define SL_HIPROT		2
#define SL_KNEE			3
#define SL_FOOT			4
#define SL_FOOTSIDE		5
#define SL_SHOULDER		6
#define SL_ARM			7
#define SL_HAND			8


#define SR_HIPSIDE		9
#define SR_HIPUP		10
#define SR_HIPROT		11
#define SR_KNEE			12
#define SR_FOOT			13
#define SR_FOOTSIDE		14
#define SR_SHOULDER		15
#define SR_ARM			16
#define SR_HAND			17


#define S_HEAD			18



/* struct for servo data */
struct servos {
	uint8_t root;
	uint8_t from;
	uint8_t to;
	uint8_t step;
};

/* servo data for 16 servos */
volatile struct servos servoData[SERVOS];

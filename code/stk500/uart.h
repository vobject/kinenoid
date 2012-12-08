#pragma once

/* Port Settings */
#define BAUD 38400UL								// Baudrate 76800 Quarz// 38400 ohne Quarz
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)	// clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))		// Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD)		// Fehler in Promille, 1000 = kein Fehler.


#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch! 
#endif



/* methods */
void uartInit();
uint8_t uartRead();

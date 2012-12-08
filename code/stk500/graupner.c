#include "defines.h"
#include "control.h"
#include "uart.h"

//uint8_t limit(int16_t in)
//{
//   return ((in - 128) * 4 / 5 + 128);
//}

int main(void) {

   // ddrs
   DDRC = 0xFF;
   DDRB = 0xFF;
   DDRA = 0xFF;

   // init
   controlInit();
   uartInit();

   // activate interrupts
   sei();

   while(1)
   {
      if (0xff == uartRead())
      {
         for (uint8_t i = SL_HIPSIDE; i <= S_HEAD; ++i)
         {
            servoData[i].root = uartRead();
         }

         const uint8_t led_status = uartRead();

         if (led_status)
         {
            PORTB |= (1 << 3);
         }
         else
         {
            PORTB &= ~(1 << 3);
         }
      }
   }
}

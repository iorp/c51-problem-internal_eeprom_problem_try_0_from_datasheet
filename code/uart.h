
#ifndef UART_H
#define UART_H
#include "reg51.h"

#ifndef F_CPU
#define F_CPU 22118400UL
#endif

#ifndef UART_ENABLE
#define UART_ENABLE 1
#endif

#ifndef UART_PRINT_ENABLE
#define UART_PRINT_ENABLE 1
#endif

#ifndef UART_PRESCALER 
#define UART_PRESCALER 384U  // 12*32 refer datasheet or Mazidi serial communication
#endif
 
 


#define UART_getBaudRate(baudrate)  -(F_CPU / (UART_PRESCALER *baudrate))
 
       
void UART_init(unsigned long v_baudRate_u32);
void UART_setBaudRate(unsigned long v_baudRate_u32);
void UART_txByte(unsigned char byte);
 


    #if (UART_ENABLE==1 && UART_PRINT_ENABLE==1  ) 
        void UART_printf(const char *format, unsigned int arg1); 
        void UART_printStr(const char *string);  
        void UART_printDec(unsigned long value);
        void UART_printHex(unsigned long value, unsigned char byteCount); 
    #endif
 #endif
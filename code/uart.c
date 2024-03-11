 
#include "uart.h"

 
void UART_init(unsigned long v_baudRate_u32)
{    
    TMOD |= 0x20; //Timer1 in Mode2.
    SCON=0x50;    // Asynchronous mode, 8-bit data and 1-stop bit
    TR1=1;        //Turn ON the timer for Baud rate generation
    UART_setBaudRate(v_baudRate_u32);  
} 
 
void UART_setBaudRate(unsigned long v_baudRate_u32)
{
    char RegValue;
     RegValue = UART_getBaudRate(v_baudRate_u32);

    TH1 = RegValue; 
}




void UART_txByte(unsigned char byte) {
   
        SBUF = byte;  // Send byte
        while (!TI);      // Wait for transmit complete
        TI = 0;           // Clear transmit interrupt flag
 
}

void UART_Listen(){
 unsigned char receivedData;
// COMMAND LOGIC GOES HERE 
    if (RI) {
        receivedData = SBUF;  // Read the received character
        RI = 0;               // Clear the receive interrupt flag

				
        // Process the received data (mirror it back)
        SBUF = receivedData;  // Send the character back
        while (!TI);          // Wait for the transmit to complete
        TI = 0;               // Clear the transmit interrupt flag
    }
}
  
  #if (UART_ENABLE==1 && UART_PRINT_ENABLE==1  )  
void UART_printStr(const char *string) {
   
    while (*string != '\0') {
        UART_txByte(*string) ;
        string++;        // Move to the next character
			 
    }
  
}

void UART_printDec(unsigned long value) {
  
    unsigned char str[11];
    unsigned char index;

    // Null-terminate the string
    str[sizeof(str) - 1] = '\0';

    // Start from the end of the buffer
    index = sizeof(str) - 2;

    // Handle special case for value == 0
    if (value == 0) {
        str[index--] = '0';
    } else {
        // Convert each digit to char and store in reverse order
        while (value > 0 && index >= 0) {
            str[index--] = '0' + (value % 10);
            value /= 10;
        }
    }

    // Move the string to the beginning of the buffer if necessary
    if (index >= 0) {
        unsigned char i;
        for (i = 0; i < sizeof(str) - index - 1; ++i) {
            str[i] = str[index + 1 + i];
        }
    }

    // Transmit the string using UART_printStr
    UART_printStr(str);
 
}

void UART_printHex(unsigned long value, unsigned char byteCount) {
  
    unsigned char str[5];  // Assuming a 16-bit value, 4 characters (plus null terminator)
    unsigned char index;
		unsigned char i;
    // Null-terminate the string
    str[sizeof(str) - 1] = '\0';

    // Start from the end of the buffer
    index = sizeof(str) - 2;

    // Convert each nibble to char and store in reverse order
    for (i = 0; i < byteCount * 2 && index >= 0; ++i) {
        unsigned char nibble = (value >> (4 * i)) & 0x0F;
        str[index--] = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
    }

    // Move the string to the beginning of the buffer if necessary
    if (index >= 0) {
        unsigned char i;
        for (i = 0; i < sizeof(str) - index - 1; ++i) {
            str[i] = str[index + 1 + i];
        }
    }

    // Transmit the string using UART_printStr
    UART_printStr(str);
    
 

}

void UART_printf(const char *format, unsigned int arg1) {
  
    char c;
    unsigned int value = 0;
    unsigned char byteCount = 2;  // Default byte count for %x specifier

    while ((c = *format++)) {
        if (c == '%') {
            c = *format++;
            if (c == 'd') {
                value = arg1;
                UART_printDec(value);
            } else if (c == 'x') {
                // Parse the width specifier for %x
                if (*format >= '1' && *format <= '9') {
                    byteCount = *format - '0';
                    while (*(format + 1) >= '0' && *(format + 1) <= '9') {
                        byteCount = byteCount * 10 + (*(++format) - '0');
                    }
                }
                UART_printHex(arg1, byteCount);
            }   else {
                // Unknown specifier, just print it
                UART_txByte('%');
                UART_txByte(c);
            }
        } else {
            // Regular character, just print it
            UART_txByte(c);
        }
    }
 
}




#endif
















// working but not used 
// #if (Enable_UART_TxBytes==1  )
// void UART_txBytes(const unsigned char *bytes, unsigned int length) {
//     unsigned int i =0;
//     for (i = 0; i < length; i++) {
// 			UART_TxByte(bytes[i]) ;
//     }
// }
// #endif

// #if (Enable_UART_Listen==1 )
// void UART_listen() {
//     unsigned char receivedData;

//     // COMMAND LOGIC GOES HERE 

//     if (RI) {
//         receivedData = SBUF;  // Read the received character
//         RI = 0;               // Clear the receive interrupt flag

//         // Process the received data (mirror it back)
//         SBUF = receivedData;  // Send the character back
//         while (!TI);          // Wait for the transmit to complete
//         TI = 0;               // Clear the transmit interrupt flag
//     }
// }
// #endif
 

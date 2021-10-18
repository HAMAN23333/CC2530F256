//-------------------------------------------------------------------
// Filename: hal_uart.h
// Description: hal UART library header file
//-------------------------------------------------------------------
//-------------------------------------------------------------------
// FUNCTIONS - API
//-------------------------------------------------------------------
// Initialize UART at the startup
//-------------------------------------------------------------------
extern void halUart1Init(uint32 baud);

//-------------------------------------------------------------------
// Read a buffer from the UART
//-------------------------------------------------------------------
extern uint16 halUart1Read(uint8 *pBuffer, uint16 length);

//-------------------------------------------------------------------
// Write a buff to the uart 
//-------------------------------------------------------------------
extern uint16 halUart1Write(uint8 *pBuffer, uint16 length);

//-------------------------------------------------------------------
// Return the number of bytes in the Rx buffer
//-------------------------------------------------------------------
extern uint16 halUart1RxLen(void);

//-------------------------------------------------------------------
// Abort UART when entering sleep mode
//-------------------------------------------------------------------
extern void halUart1Suspend(void);

//-------------------------------------------------------------------
// Resume UART after wakeup from sleep
//-------------------------------------------------------------------
extern void halUart1Resume(void);
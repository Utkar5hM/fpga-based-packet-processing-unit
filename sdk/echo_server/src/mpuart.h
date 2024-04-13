#ifndef __MPUART_H_
#define __MPUART_H_

#include "xparameters.h"
#include "xstatus.h"
#include "xuartlite.h"
#include "xil_printf.h"


#ifndef SDT
#define UARTLITE_DEVICE_ID	XPAR_UARTLITE_0_DEVICE_ID
#else
#define XUARTLITE_BASEADDRESS	XPAR_XUARTLITE_0_BASEADDR
#endif

#define TEST_BUFFER_SIZE 16


#ifndef SDT
int UartLitePolledExample(u16 DeviceId);
#else
int UartLitePolledExample(UINTPTR BaseAddress);
#endif

XUartLite UartLite;		/* Instance of the UartLite Device */

/*
 * The following buffers are used in this example to send and receive data
 * with the UartLite.
 */
u8 SendBuffer[TEST_BUFFER_SIZE];	/* Buffer for Transmitting Data */
u8 RecvBuffer[TEST_BUFFER_SIZE];	/* Buffer for Receiving Data */

#ifndef SDT
int UartLitePolledExample(u16 DeviceId);
#else
int UartLitePolledExample(UINTPTR BaseAddress);
#endif

#endif




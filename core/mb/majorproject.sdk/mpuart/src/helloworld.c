
/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "microblaze_sleep.h"

#include "xparameters.h"
#include "xuartlite.h"
#include "xintc.h"
#include "xil_exception.h"

// ----------------------------------------------------
// CONSTANT
// ----------------------------------------------------

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define UARTLITE_DEVICE_ID      XPAR_UARTLITE_0_DEVICE_ID
#define INTC_DEVICE_ID          XPAR_INTC_0_DEVICE_ID
#define UARTLITE_INT_IRQ_ID     XPAR_INTC_0_UARTLITE_0_VEC_ID

/*
 * The following constant controls the length of the buffers to be sent
 * and received with the UartLite device.
 */
#define TEST_BUFFER_SIZE        1502
#define MAX_FRAME_SIZE			1600

// ----------------------------------------------------
// FUNCTION PROTOTYPE
// ----------------------------------------------------
int UartLiteInit(u16 DeviceId);
int SetupInterruptSystem(XUartLite *UartLitePtr);
void RecvHandler(void *CallBackRef, unsigned int EventData);
// ----------------------------------------------------
// VARIABLE DEFINITIONS
// ----------------------------------------------------
XUartLite UartLite;            		// The instance of the UartLite Device
XIntc InterruptController;     		// The instance of the Interrupt Controller

u8 ReceiveBuffer[TEST_BUFFER_SIZE];	// UART Rx buffer (shared with interrupt code)
u8 FrameBuffer[MAX_FRAME_SIZE];		// UART RX buffer (shared with interrupt code)
static volatile int TotalReceivedCount;	// UART RX char counter (shared with interrupt code)
static volatile int TotalSentCount;		// UART TX char counter (shared with interrupt code)
static volatile int TxIsDone=TRUE;		// UART TX flag (TRUE=tx is possible) (shared with interrupt code)
static volatile int TxMaxChr;			// UART TX chars number to transmit (shared with interrupt code)

int UartLiteInit(u16 DeviceId)
{
	int Status;

	//Initialize the UartLite driver so that it's ready to use.
	Status = XUartLite_Initialize(&UartLite, DeviceId);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Connect the UartLite to the interrupt subsystem such that interrupts can
	// occur. This function is application specific.
	Status = SetupInterruptSystem(&UartLite);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Setup the handlers for the UartLite that will be called from the
	// interrupt context when data has been sent and received, specify a
	// pointer to the UartLite driver instance as the callback reference so
	// that the handlers are able to access the instance data.
	XUartLite_SetRecvHandler(&UartLite, RecvHandler, &UartLite);
	// Enable the interrupt of the UartLite so that interrupts will occur.
	XUartLite_EnableInterrupt(&UartLite);

	return XST_SUCCESS;
}

volatile u16 FrameLength;
volatile u8 FrameCaptured;

void RecvHandler(void *CallBackRef, unsigned int EventData)
{
	if(EventData==1502&&
		ReceiveBuffer[0]==0xFF&&
		ReceiveBuffer[1]==0xFF&&
		ReceiveBuffer[2]==0xFF&&
		ReceiveBuffer[3]==0xFF&&
		ReceiveBuffer[4]==0xFF&&
		ReceiveBuffer[5]==0xFF
		){
		FrameLength = ((u16)ReceiveBuffer[TEST_BUFFER_SIZE - 2] << 8) | ReceiveBuffer[TEST_BUFFER_SIZE - 1];
		memcpy(FrameBuffer, ReceiveBuffer, FrameLength);
		XUartLite_ResetFifos(&UartLite);
		XUartLite_Recv(&UartLite, ReceiveBuffer, TEST_BUFFER_SIZE);
		FrameCaptured=1;
	} else {
		XUartLite_ResetFifos(&UartLite);
		XUartLite_Recv(&UartLite, ReceiveBuffer, TEST_BUFFER_SIZE);
	}
	// if(EventData==1){
	// if(ReceiveBuffer[0]!='\n'){
	// 	if((FrameIndex==0&&ReceiveBuffer[0]==0xFF)||FrameIndex<MAX_FRAME_SIZE){
	// 		xil_printf("ReceiveBuffer: %X\r\n", ReceiveBuffer[0]);
	// 		FrameBuffer[FrameIndex]=ReceiveBuffer[0];
	// 		FrameIndex++;
	// 	} else {
	// 		FrameIndex=0;
	// 	}
	// } else {
	// 	FrameCaptured=1;
	// 	for(int i=0;i<FrameIndex;i++){
	// 		FrameBuffer[i]=0;
	// 	}
	// 	FrameIndex=0;
	// }
	// } else {
	// 	for(int i=0;i<FrameIndex;i++){
	// 		FrameBuffer[i]=0;
	// 	}
	// 	FrameIndex=0;
	// 	xil_printf("Error: %lu", EventData);
	// }

}

int SetupInterruptSystem(XUartLite *UartLitePtr)
{

	int Status;

	// Initialize the interrupt controller driver so that it is ready to use
	Status = XIntc_Initialize(&InterruptController, INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect a device driver handler that will be called when an interrupt
	 * for the device occurs, the device driver handler performs the
	 * specific interrupt processing for the device.
	 */
	Status = XIntc_Connect(&InterruptController, UARTLITE_INT_IRQ_ID,
			   (XInterruptHandler)XUartLite_InterruptHandler,
			   (void *)UartLitePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Start the interrupt controller such that interrupts are enabled for
	 * all devices that cause interrupts, specific real mode so that
	 * the UartLite can cause interrupts through the interrupt controller.
	 */
	Status = XIntc_Start(&InterruptController, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Enable the interrupt for the UartLite device.
	XIntc_Enable(&InterruptController, UARTLITE_INT_IRQ_ID);

	// Initialize the exception table.
	Xil_ExceptionInit();

	// Register the interrupt controller handler with the exception table.
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			 (Xil_ExceptionHandler)XIntc_InterruptHandler,
			 &InterruptController);

	// Enable exceptions.
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}

/***************************************************************************
* MAIN
****************************************************************************/
int main()
{
	int Status;

    init_platform();

    // UARTLite initialization and interrupt setup
    Status = UartLiteInit(UARTLITE_DEVICE_ID);
    if (Status != XST_SUCCESS) {
        cleanup_platform();
        return 0;
    }
	xil_printf("UARTLite Initialized\r\n");
	XUartLite_Recv(&UartLite, ReceiveBuffer, TEST_BUFFER_SIZE);
    // Infinite loop for microcontroller activity
    while(1){
		if(FrameCaptured > 0){
		xil_printf("Frame Captured, length:%lu\r\n", FrameLength);
    	FrameCaptured=0;
		FrameLength=0;
		}
    	// MB_Sleep(100);
    }

    cleanup_platform();
    return 0;
}

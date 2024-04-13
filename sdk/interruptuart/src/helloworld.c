#include "xparameters.h"
#include "xuartlite.h"
#ifndef SDT
#ifdef XPAR_INTC_0_DEVICE_ID
#include "xintc.h"
#else
#include "xscugic.h"
#endif
#endif
#include "xil_exception.h"
#include "xil_printf.h"
#ifdef SDT
#include "xinterrupt_wrap.h"
#endif

/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define XUARTLITE_BASEADDRESS	XPAR_AXI_UARTLITE_0_BASEADDR

/*
 * The following constant controls the length of the buffers to be sent
 * and received with the UartLite device.
 */
#define TEST_BUFFER_SIZE        100

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

int UartLiteIntrExample(XUartLite *UartLiteInstancePtr,
			UINTPTR BaseAddress);


void SendHandler(void *CallBackRef, unsigned int EventData);

void RecvHandler(void *CallBackRef, unsigned int EventData);

/************************** Variable Definitions *****************************/

XUartLite UartLite; /* The instance of the UartLite Device */


/*
 * The following variables are shared between non-interrupt processing and
 * interrupt processing such that they must be global.
 */

/*
 * The following buffers are used in this example to send and receive data
 * with the UartLite.
 */
u8 SendBuffer[TEST_BUFFER_SIZE];
u8 ReceiveBuffer[TEST_BUFFER_SIZE];

/*
 * The following counters are used to determine when the entire buffer has
 * been sent and received.
 */
static volatile int TotalReceivedCount;
static volatile int TotalSentCount;

/******************************************************************************/
/**
 *
 * Main function to call the UartLite interrupt example.
 *
 *
 * @return	XST_SUCCESS if successful, XST_FAILURE if unsuccessful
 *
 * @note		None
 *
 *******************************************************************************/
int main(void)
{
	int Status;

	/*
	 * Run the UartLite Interrupt example, specify the Device ID that is
	 * generated in xparameters.h.
	 */

	Status = UartLiteIntrExample(&UartLite, XUARTLITE_BASEADDRESS);
	if (Status != XST_SUCCESS) {
		xil_printf("Uartlite interrupt Example Failed\r\n");
		return XST_FAILURE;
	}

	xil_printf("Successfully ran Uartlite interrupt Example\r\n");
	return XST_SUCCESS;
}


int UartLiteIntrExample(XUartLite *UartLiteInstPtr,
			UINTPTR BaseAddress)
{
	int Status;
	int Index;
	XUartLite_Config *CfgPtr;

	/*
	 * Initialize the UartLite driver so that it's ready to use.
	 */
	CfgPtr = XUartLite_LookupConfig(BaseAddress);
	Status = XUartLite_Initialize(&UartLite, BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XUartLite_SelfTest(&UartLite);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the UartLite to the interrupt subsystem such that interrupts can
	 * occur. This function is application specific.
	 */

	Status = XSetupInterruptSystem(&UartLite, &XUartLite_InterruptHandler,
				       CfgPtr->IntrId, CfgPtr->IntrParent,
				       XINTERRUPT_DEFAULT_PRIORITY);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Setup the handlers for the UartLite that will be called from the
	 * interrupt context when data has been sent and received, specify a
	 * pointer to the UartLite driver instance as the callback reference so
	 * that the handlers are able to access the instance data.
	 */
	XUartLite_SetSendHandler(&UartLite, SendHandler, &UartLite);
	XUartLite_SetRecvHandler(&UartLite, RecvHandler, &UartLite);

	/*
	 * Enable the interrupt of the UartLite so that interrupts will occur.
	 */
	XUartLite_EnableInterrupt(&UartLite);

	/*
	 * Initialize the send buffer bytes with a pattern to send and the
	 * the receive buffer bytes to zero to allow the receive data to be
	 * verified.
	 */
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		SendBuffer[Index] = Index;
		ReceiveBuffer[Index] = 0;
	}

	/*
	 * Start receiving data before sending it since there is a loopback.
	 */
	XUartLite_Recv(&UartLite, ReceiveBuffer, TEST_BUFFER_SIZE);

	/*
	 * Wait for the entire buffer to be received, letting the interrupt
	 * processing work in the background, this function may get locked
	 * up in this loop if the interrupts are not working correctly.
	 */
	while ((TotalReceivedCount != TEST_BUFFER_SIZE)
	       || (TotalSentCount != TEST_BUFFER_SIZE)) {
	}

	/*
	 * Verify the entire receive buffer was successfully received.
	 */
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		if (ReceiveBuffer[Index] != SendBuffer[Index]) {
			return XST_FAILURE;
		}
	}

	return XST_SUCCESS;
}

void RecvHandler(void *CallBackRef, unsigned int EventData)
{
	TotalReceivedCount = EventData;
}




#include "mpuart.h"


#ifndef SDT
int UartLitePolledExample(u16 DeviceId)
#else
int UartLitePolledExample(UINTPTR BaseAddress)
#endif
{
	int Status;
	unsigned int SentCount;
	unsigned int ReceivedCount = 0;
	int Index;

	/*
	 * Initialize the UartLite driver so that it is ready to use.
	 */
#ifndef	SDT
	Status = XUartLite_Initialize(&UartLite, DeviceId);
#else
	Status = XUartLite_Initialize(&UartLite, BaseAddress);
#endif
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
	 * Initialize the send buffer bytes with a pattern to send and the
	 * the receive buffer bytes to zero.
	 */
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		SendBuffer[Index] = Index;
		RecvBuffer[Index] = 0;
	}

	/*
	 * Send the buffer through the UartLite waiting til the data can be sent
	 * (block), if the specified number of bytes was not sent successfully,
	 * then an error occurred.
	 */
	SentCount = XUartLite_Send(&UartLite, SendBuffer, TEST_BUFFER_SIZE);
	if (SentCount != TEST_BUFFER_SIZE) {
		return XST_FAILURE;
	}

	/*
	 * Receive the number of bytes which is transferred.
	 * Data may be received in fifo with some delay hence we continuously
	 * check the receive fifo for valid data and update the receive buffer
	 * accordingly.
	 */
	while (1) {
		ReceivedCount += XUartLite_Recv(&UartLite,
						RecvBuffer + ReceivedCount,
						TEST_BUFFER_SIZE - ReceivedCount);
		if (ReceivedCount == TEST_BUFFER_SIZE) {
			break;
		}
	}

	/*
	 * Check the receive buffer data against the send buffer and verify the
	 * data was correctly received.
	 */
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		if (SendBuffer[Index] != RecvBuffer[Index]) {
			return XST_FAILURE;
		}
	}

	return XST_SUCCESS;
}
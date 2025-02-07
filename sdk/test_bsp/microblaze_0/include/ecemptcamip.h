
#ifndef ECEMPTCAMIP_H
#define ECEMPTCAMIP_H


/****************** Include Files ********************/
#include "xil_types.h"
#include "xstatus.h"

#define ECEMPTCAMIP_S00_AXI_SLV_REG0_OFFSET 0
#define ECEMPTCAMIP_S00_AXI_SLV_REG1_OFFSET 4
#define ECEMPTCAMIP_S00_AXI_SLV_REG2_OFFSET 8
#define ECEMPTCAMIP_S00_AXI_SLV_REG3_OFFSET 12
#define ECEMPTCAMIP_S00_AXI_SLV_REG4_OFFSET 16
#define ECEMPTCAMIP_S00_AXI_SLV_REG5_OFFSET 20


/**************************** Type Definitions *****************************/
/**
 *
 * Write a value to a ECEMPTCAMIP register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the ECEMPTCAMIPdevice.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void ECEMPTCAMIP_mWriteReg(u32 BaseAddress, unsigned RegOffset, u32 Data)
 *
 */
void ECEMPTCAMIP_mWriteReg(u32 BaseAddress, unsigned RegOffset, u32 Data);

/**
 *
 * Read a value from a ECEMPTCAMIP register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the ECEMPTCAMIP device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	u32 ECEMPTCAMIP_mReadReg(u32 BaseAddress, unsigned RegOffset)
 *
 */
u32 ECEMPTCAMIP_mReadReg(u32 BaseAddress, unsigned RegOffset);

/************************** Function Prototypes ****************************/
/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the ECEMPTCAMIP instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus ECEMPTCAMIP_Reg_SelfTest(void * baseaddr_p);

void ECEMPTCAMIP_SetKey(u32 BaseAddress, u8 Address, u8 Data, u32 Key, u32 KeyMask);

u32 ECEMPTCAMIP_GetKey(u32 BaseAddress, u32 Key);

u8 ECEMPTCAMIP_GetRespAddr(u32 Response);

u8 ECEMPTCAMIP_GetRespData(u32 Response);

u8 ECEMPTCAMIP_GetRespValid(u32 Response);

u8 ECEMPTCAMIP_GetRespNull(u32 Response);

void ECEMPTCAMIP_delay(u64 d1);

#endif // ECEMPTCAMIP_H

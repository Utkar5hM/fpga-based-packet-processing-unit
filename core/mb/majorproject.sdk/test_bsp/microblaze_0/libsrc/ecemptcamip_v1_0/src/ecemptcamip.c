

/***************************** Include Files *******************************/
#include "ecemptcamip.h"
#include "xil_io.h"

#include "xil_printf.h"
/************************** Function Definitions ***************************/

void ECEMPTCAMIP_mWriteReg(u32 BaseAddress, unsigned RegOffset, u32 Data){
	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data));
}

u32 ECEMPTCAMIP_mReadReg(u32 BaseAddress, unsigned RegOffset){
	 return Xil_In32((BaseAddress) + (RegOffset));
}

void ECEMPTCAMIP_SetKey(u32 BaseAddress, u8 Address, u8 Data, u32 Key, u32 KeyMask){

	// Set Key
	ECEMPTCAMIP_mWriteReg(BaseAddress, 4*1, Key);

	// Set Key Mask
	ECEMPTCAMIP_mWriteReg(BaseAddress, 4*2, KeyMask);

	//	Set Address[4:0], data[9:5], set_clr[10]=0,set_valid[11]=1, req_valid[12]=0
	u32 RegData =0;
	// Mask Address and Data to use only lower 5 bits
	Address &= 0x1F; // 0x1F = 0001 1111 in binary, ensuring only lower 5 bits are used
	Data &= 0x1F;

	// Shift Address and Data to their correct positions and set in RegData
	RegData |= (Address << 0);  // Address is at bits 4:0
	RegData |= (Data << 5);     // Data is at bits 9:5

	// Set set_valid to 1 at bit 11
	RegData |= (1 << 11);       // Set bit 11
	ECEMPTCAMIP_mWriteReg(BaseAddress, 0, RegData);
	ECEMPTCAMIP_delay(999999);
	ECEMPTCAMIP_delay(999999);
	// Set set_valid back to 0
	RegData &= ~(1 << 11);
	ECEMPTCAMIP_mWriteReg(BaseAddress, 0, RegData);
	ECEMPTCAMIP_delay(999999);
	ECEMPTCAMIP_delay(999999);
	return;
}

u32 ECEMPTCAMIP_GetKey(u32 BaseAddress, u32 Key){
	// set Req_key
	ECEMPTCAMIP_mWriteReg(BaseAddress, 4*3, Key);

	// Set_valid to false and other stuff as well.
	u32 RegData =0;
	// Set req_valid to 1 at bit 12
	RegData |= (1 << 12);       // Set bit 11
	ECEMPTCAMIP_mWriteReg(BaseAddress, 0, RegData);
	u32 Response = 0;
	u8 ResponseReady=0;
	u32 ResponseCheckCount=0;
	while(ResponseReady==0){
		ECEMPTCAMIP_delay(999999);
		Response = ECEMPTCAMIP_mReadReg(BaseAddress, 4*5);
	    xil_printf("ResponseCheckCount : %lu\n\r", ResponseCheckCount);
	    // Extracting out_req_ready from Response[10]
	    ResponseReady = (u8)((Response >> 10) & 0x01) || (ECEMPTCAMIP_GetRespValid(Response) &( !ECEMPTCAMIP_GetRespNull(Response)));  // Only need 1 bit
	    ResponseCheckCount++;
	}
	return Response;
}

u8 ECEMPTCAMIP_GetRespAddr(u32 Response) {
	return ((u8)(Response & 0x1F));
}

u8 ECEMPTCAMIP_GetRespData(u32 Response) {
	return ((u8)((Response >> 5) & 0x1F));
}

u8 ECEMPTCAMIP_GetRespValid(u32 Response) {
	return  (u8)((Response >> 11) & 0x01);
}

u8 ECEMPTCAMIP_GetRespNull(u32 Response) {
	return (u8)((Response >> 12) & 0x01);
}

void ECEMPTCAMIP_delay(u64 d1){
	for(u64 i=0; i<d1; i++){
		for(u64 j=0;j<9999999999999999;)
			j=j+1;
	    }
}

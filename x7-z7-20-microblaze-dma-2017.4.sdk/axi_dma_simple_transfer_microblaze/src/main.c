/*
 * main.c
 *
 *  Created on: 2018. 3. 29.
 *      Author: JoonSoo Ha
 */

#include <stdio.h>
#include "xil_printf.h"
#include "xaxidma.h"
#include "xparameters.h"

//#define FOR_SIM

// no simulation printf
//void ns_printf(char *str, ...){
//#ifndef FOR_SIM
//	printf(str);
//#endif
//}

#define BRAM_SIZE_IN_BYTE (XPAR_AXI_BRAM_CTRL_0_S_AXI_HIGHADDR - XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR + 1)

int main()
{
//	XAxiDma xAxiDma0Instance;
	XAxiDma_Config *xAxiDma0_CftPtr;
	int status;
	int transferSizeInWord = BRAM_SIZE_IN_BYTE / (sizeof(32) * 2);
	u32 *bramBaseAddr = (u32 *)XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR;
	u32 *srcMemPtr = (u32 *)(bramBaseAddr + 0);
	u32 *destMemPtr = (u32 *)(bramBaseAddr + transferSizeInWord);
	int printJump = 8;

#ifndef FOR_SIM
	printf("XAxiDma size = %d", sizeof(XAxiDma));
    printf("Hello CDMA\n\r");
#endif
#if 0
    xAxiDma0_CftPtr = XAxiDma_LookupConfig(XPAR_AXI_DMA_0_DEVICE_ID);
    if(!xAxiDma0_CftPtr) {
#ifndef FOR_SIM
    	printf("AXI CDMA 0 Configuration Failed\n\r");
#endif
    	return XST_FAILURE;
    }


    status = XAxiDma_CfgInitialize(&xAxiDma0Instance, xAxiDma0_CftPtr);
    if(status == XST_FAILURE) {
#ifndef FOR_SIM
    	printf("AXI CDMA 0 Initialization Failed\n\r");
#endif
        return status;
    }

    // disable interrupt
    XAxiDma_IntrDisable(&xAxiDma0Instance, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
    XAxiDma_IntrDisable(&xAxiDma0Instance, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
#endif
    // initialize BRAM
    memset((u8 *)bramBaseAddr, 0, BRAM_SIZE_IN_BYTE);
    for(int i=0; i<transferSizeInWord; i++) {
    	if(i % printJump == 0)
#ifndef FOR_SIM
		printf("srcMem[%d] = %d : destMem[%d] = %d\n\r", i, (int)srcMemPtr[i], i, (int)destMemPtr[i]);
#endif
    }
#ifndef FOR_SIM
    	printf("\n\r\n\r\n\r\n\r");
#endif

    // initialize srcMem
    for(int i=0; i<transferSizeInWord; i++) {
    	srcMemPtr[i] = i;
    }
#if 0
    // Simple Transfer using AXI DMA
    status = XAxiDma_SimpleTransfer(&xAxiDma0Instance, (UINTPTR)destMemPtr, transferSizeInWord*sizeof(u32), XAXIDMA_DEVICE_TO_DMA);
    if(status == XST_FAILURE) {
#ifndef FOR_SIM
    	printf("AXI DMA Device-to-DMA Transfer Failed\n\r");
#endif
    	return status;
    }
    status = XAxiDma_SimpleTransfer(&xAxiDma0Instance, (UINTPTR)srcMemPtr, transferSizeInWord*sizeof(u32), XAXIDMA_DMA_TO_DEVICE);
    if(status == XST_FAILURE) {
#ifndef FOR_SIM
    	printf("AXI DMA DMA-to-Device Transfer Failed\n\r");
#endif
    }

    // wait;
    while(XAxiDma_Busy(&xAxiDma0Instance, XAXIDMA_DEVICE_TO_DMA) || XAxiDma_Busy(&xAxiDma0Instance, XAXIDMA_DMA_TO_DEVICE));

    // compare result
    for(int i=0; i<transferSizeInWord; i++) {
    	if(i % printJump == 0)
#ifndef FOR_SIM
    	printf("srcMem[%d] = %d : destMem[%d] = %d\n\r", i, (int)srcMemPtr[i], i, (int)destMemPtr[i]);
#endif
    }
#endif
    status = memcmp((u32 *)srcMemPtr, (u32 *)destMemPtr, 32);
    if(status == XST_SUCCESS) {
#ifndef FOR_SIM
    	printf("\n\rResult Compare Success\n\r");
#endif
    } else {
#ifndef FOR_SIM
    printf("\n\rResult Compare Failed\n\r");
#endif
    }
    return status;
}


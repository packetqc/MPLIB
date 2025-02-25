/* FROM, PORTING TO C++ CLASS, MP
 * Some constants, hardware definitions and comments taken from ST's HAL driver
 * library, COPYRIGHT(c) 2015 STMicroelectronics.
 */

/* FROM, PORTING TO C++ CLASS, MP
 * FreeRTOS+TCP V2.3.2
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

#ifndef NETSERVICES_H_
#define NETSERVICES_H_

#include "mpapp.h"
#include "./FreeRTOSIPConfig.h"


#ifdef __cplusplus
extern "C" {
#endif

#include "../../FreeRTOS-Plus-TCP/source/include/FreeRTOS_IP.h"
#include "../../FreeRTOS-Plus-TCP/source/portable/NetworkInterface/include/phyHandling.h"

uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress, uint16_t usSourcePort, uint32_t ulDestinationAddress, uint16_t usDestinationPort );
void vApplicationIPNetworkEventHook_Multi( eIPCallbackEvent_t eNetworkEvent,  struct xNetworkEndPoint * pxEndPoint );
//BaseType_t vSendPingGW( const int8_t *pcIPAddress );
void vApplicationPingReplyHook( ePingReplyStatus_t eStatus, uint16_t usIdentifier );
BaseType_t xApplicationGetRandomNumber( uint32_t * pulNumber );
eDHCPCallbackAnswer_t xApplicationDHCPHook_Multi( eDHCPCallbackPhase_t eDHCPPhase, struct xNetworkEndPoint * pxEndPoint,IP_Address_t * pxIPAddress );
void vNetworkInterfaceAllocateRAMToBuffers( NetworkBufferDescriptor_t pxNetworkBuffers[ ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS ] );

void prvEMACHandlerTask( void * pvParameters );
void prvIPTask( void * pvParameters );

uint8_t isNetworkStarted();
void StartNetServices(void *argument);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

//#include <stdint.h>
//#include "stdio.h"
//#include <DataServices.h>
//
////#include "../../FreeRTOS-Plus-TCP/source/include/FreeRTOS_IP.h"
//#include <string.h>
//
///* FreeRTOS includes. */
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "semphr.h"
//
///* FreeRTOS+TCP includes. */
//#include "../../FreeRTOS-Plus-TCP/source/include/FreeRTOS_IP.h"
//#include "../../FreeRTOS-Plus-TCP/source/include/FreeRTOS_Sockets.h"
//#include "../../FreeRTOS-Plus-TCP/source/include/FreeRTOS_IP_Private.h"
//#include "../../FreeRTOS-Plus-TCP/source/include/FreeRTOS_DNS.h"
//#include "../../FreeRTOS-Plus-TCP/source/include/FreeRTOS_Routing.h"
//#include "../../FreeRTOS-Plus-TCP/source/include/NetworkBufferManagement.h"
//
////#include "NetworkInterface.h"
//
//#include "../../FreeRTOS-Plus-TCP/source/portable/NetworkInterface/include/phyHandling.h"
//
///* ST includes. */
//#include "stm32h7xx_hal.h"
//
////#ifndef STM32Hxx_HAL_ETH_H
//#ifndef STM32H7xx_HAL_ETH_H
//
///*
// * The ST HAL library provides stm32h7xx_hal_eth.{c,h}.
// * This FreeRTOS+TCP driver renamed these files to stm32hxx_hal_eth.{c,h}
// * by removing the '7'.
// * Please make sure that "portable/NetworkInterface/STM32Hxx" is included
// * in the include paths earlier than "STM32H7xx_HAL_Driver/Inc".
// * and also make sure that you have defined 'HAL_ETH_MODULE_ENABLED'
// * in your copy of "stm32h7xx_hal_conf".
// */
//    #error stm32hxx_hal_eth.h is possibly not included
//#endif
//
///* Interrupt events to process: reception, transmission and error handling. */
//#define EMAC_IF_RX_EVENT                1U
//#define EMAC_IF_TX_EVENT                2U
//#define EMAC_IF_ERR_EVENT               4U
//
///*
// * Enable either Hash or Perfect Filter, Multicast filter - None,
// * Enable Hash Multicast (HMC), and Enable Hash Unicast (HUC).
// */
//#define ENABLE_HASH_FILTER_SETTINGS      (uint32_t) 0x00000416U
////#define ENABLE_HASH_FILTER_SETTINGS     (uint32_t) 0x7FFFFFFF
////#define ENABLE_HASH_FILTER_SETTINGS     (uint32_t) 0x80000000
////#define ENABLE_HASH_FILTER_SETTINGS     (uint32_t) 0xFFFFFFFF
//
//#ifndef niEMAC_HANDLER_TASK_NAME
//    #define niEMAC_HANDLER_TASK_NAME    "EMAC-task"
//#endif
//
//#ifndef niEMAC_HANDLER_TASK_STACK_SIZE
//    #define niEMAC_HANDLER_TASK_STACK_SIZE    ( 4 * configMINIMAL_STACK_SIZE )
//#endif
//
//#ifndef niEMAC_HANDLER_TASK_PRIORITY
//    #define niEMAC_HANDLER_TASK_PRIORITY    configMAX_PRIORITIES - 1
//#endif
//
//
//const uint8_t ucIPAddress[ 4 ] = { 192, 168, 13, 133 };
//const uint8_t ucNetMask[ 4 ] =  { 255, 255, 255, 0 };
//const uint8_t ucGatewayAddress[ 4 ] = { 192, 168, 13, 1 };
//const uint8_t ucDNSServerAddress[ 4 ] = { 8, 8, 8, 8 };
//const uint8_t ucMACAddress[ 6 ] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };
//
//
typedef enum
{
    eMACInit,   /* Must initialise MAC. */
    eMACPass,   /* Initialisation was successful. */
    eMACFailed, /* Initialisation failed. */
} eMAC_INIT_STATUS_TYPE;

//
///* About the section ".ethernet_data" : the DMA wants the descriptors and buffers allocated in the
// * RAM3 memory, which can be added to the .LD file as follows::
// *
// * RAM3 (xrw)      : ORIGIN = 0x24040000, LENGTH = 0x8000
// *
// * .ethernet_data :
// * {
// *  PROVIDE_HIDDEN (__ethernet_data_start = .);
// *  KEEP (*(SORT(.ethernet_data.*)))
// *  KEEP (*(.ethernet_data*))
// *  PROVIDE_HIDDEN (__ethernet_data_end = .);
// * } >RAM3
// *
// */
///* Ethernet Rx DMA Descriptors */
//ETH_DMADescTypeDef DMARxDscrTab[ ETH_RX_DESC_CNT ]    __attribute__( ( section( ".ethernet_data" ), aligned( 32 ) ) );
//
///* Ethernet Receive Buffer */
//#if ( ipconfigZERO_COPY_TX_DRIVER == 0 )
//    uint8_t Rx_Buff[ ETH_RX_DESC_CNT ][ ETH_RX_BUF_SIZE ] __attribute__( ( section( ".ethernet_data" ), aligned( 32 ) ) );
//#endif
//
///* Ethernet Tx DMA Descriptors */
//ETH_DMADescTypeDef DMATxDscrTab[ ETH_TX_DESC_CNT ]    __attribute__( ( section( ".ethernet_data" ), aligned( 32 ) ) );
//
///* Ethernet Transmit Buffer */
//#if ( ipconfigZERO_COPY_TX_DRIVER == 0 )
//    uint8_t Tx_Buff[ ETH_TX_DESC_CNT ][ ETH_TX_BUF_SIZE ]                __attribute__( ( section( ".ethernet_data" ), aligned( 32 ) ) );
//#endif
//
//uint8_t ucNetworkPackets[ ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS * ETH_RX_BUF_SIZE ]
//#if ( ipconfigZERO_COPY_RX_DRIVER != 0 || ipconfigZERO_COPY_TX_DRIVER != 0 )
//	__attribute__( ( section( ".ethernet_data" ) ) )
//#endif /* ( ipconfigZERO_COPY_RX_DRIVER != 0 || ipconfigZERO_COPY_TX_DRIVER != 0 ) */
//__attribute__( ( aligned( 32 ) ) );
//
/* For local use only: describe the PHY's properties: */
//const PhyProperties_t xPHYProperties =
//{
//    .ucSpeed  = PHY_SPEED_AUTO,
//    .ucDuplex = PHY_DUPLEX_AUTO,
//    .ucMDI_X  = PHY_MDIX_DIRECT,
//};
//const PhyProperties_t xPHYProperties =
//{
//    .ucSpeed  = PHY_SPEED_AUTO,
//    .ucMDI_X  = PHY_MDIX_DIRECT,
//	.ucDuplex = PHY_DUPLEX_AUTO,
//	.ucSpare = 0 //MP ADDED...
//};

#include "touchgfx/Unicode.hpp"

class NetServices {
	static int iNet;
	static NetServices *instance;
	static char *name;

public:
//	static ETH_HandleTypeDef heth;
	bool start = false;
	uint8_t statusETH = NET_NOTOK;
	uint8_t statusCLOUD = CLOUD_NOTOK;

	static NetServices* CreateInstance() {
		if(iNet==0) {
			instance =new NetServices;
			snprintf(name, CAT_LENGTH, "NETWORK");
			iNet=1;
		}
		return instance;
	}


	char* getName();

	static BaseType_t xSTM32H_NetworkInterfaceInitialise( NetworkInterface_t * pxInterface );
	//static
	NetworkInterface_t * pxSTM32H_FillInterfaceDescriptor( BaseType_t xEMACIndex, NetworkInterface_t * pxInterface );
	static BaseType_t xSTM32H_NetworkInterfaceOutput( NetworkInterface_t * pxInterface, NetworkBufferDescriptor_t * const pxBuffer, BaseType_t xReleaseAfterSend );
	static BaseType_t xSTM32H_GetPhyLinkStatus( NetworkInterface_t * pxInterface );

	NetworkInterface_t* getFirstInterface();
	NetworkEndPoint_t* getEndPoint(uint8_t index);
	NetworkEndPoint_t* getFirstEndPoint(NetworkInterface_t* interface);
	uint8_t getNumberEndpoints();


	void PrintARPCache( uint8_t* number, touchgfx::Unicode::UnicodeChar textarea[][MAXDISPLAY] );
	void showEndPoint();
	void init();
	int Connect();
	int Disconnect();
	bool IsConnected();
	void PushToLogs(char *alog,...);
	void PushToLogsError(int code);
	char* GetErrorDescription(int code);

//	NetworkEndPoint_t*& getXEndPoints() {
//		return xEndPoints;
//	}
//
//	NetworkInterface_t*& getXInterfaces() {
//		return xInterfaces;
//	}

	ETH_HandleTypeDef* getHeth();

//	const ETH_HandleTypeDef& getHeth() const {
//		return heth;
//	}
//
//	void setHeth(const ETH_HandleTypeDef &hethIn) {
////		this->heth = heth;
//		heth = hethIn;
//	}

	static uint32_t prvRevBits32( uint32_t ulValue );
	static uint32_t prvComputeCRC32_MAC( const uint8_t * pucMAC );
	static uint32_t prvComputeEthernet_MACHash( const uint8_t * pucMAC );
	static void prvSetMAC_HashFilter( ETH_HandleTypeDef * pheth, const uint8_t * pucMAC );
	static uint8_t* pucGetRXBuffer( size_t uxSize );

	void prvSetFlagsAndNotify( uint32_t ulFlags );
	const uint8_t * FreeRTOS_GetMACAddress( void );
	void vApplicationIPNetworkEventHook_Multi( eIPCallbackEvent_t eNetworkEvent,  struct xNetworkEndPoint * pxEndPoint );
	void vApplicationPingReplyHook( ePingReplyStatus_t eStatus, uint16_t usIdentifier );
	void vNetworkInterfaceAllocateRAMToBuffers( NetworkBufferDescriptor_t pxNetworkBuffers[ ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS ] );
	uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress, uint16_t usSourcePort, uint32_t ulDestinationAddress, uint16_t usDestinationPort );
	BaseType_t xApplicationGetRandomNumber( uint32_t * pulNumber );
	eDHCPCallbackAnswer_t xApplicationDHCPHook_Multi( eDHCPCallbackPhase_t eDHCPPhase, struct xNetworkEndPoint * pxEndPoint, IP_Address_t * pxIPAddress );
//	void prvSetFlagsAndNotify( uint32_t ulFlags );

	BaseType_t vSendPingGW();

	/* This function binds PHY IO functions, then inits and configures */
	void prvMACBProbePhy( void );

	/* Force a negotiation with the Switch or Router and wait for LS. */
	void prvEthernetUpdateConfig( BaseType_t xForce );

	/*
	 * A deferred interrupt handler task that processes
	 */
//	static void prvEMACHandlerTask( void * pvParameters );

	/*
	 * See if there is a new packet and forward it to the IP-task.
	 */
	BaseType_t prvNetworkInterfaceInput( void );

	/* Private PHY IO functions and properties */
	static	int32_t ETH_PHY_IO_ReadReg( uint32_t DevAddr,
	                                   uint32_t RegAddr,
	                                   uint32_t * pRegVal );
	static	int32_t ETH_PHY_IO_WriteReg( uint32_t DevAddr,
	                                    uint32_t RegAddr,
	                                    uint32_t RegVal );

	static void vClearOptionBit( volatile uint32_t * pulValue,
	                             uint32_t ulValue );

	#if ( ipconfigHAS_PRINTF != 0 )
	    static size_t uxGetOwnCount( ETH_HandleTypeDef * heth );
	#endif

	void showEndPoint(NetworkInterface_t * pxEndPoint);

protected:
	bool connected = false;
	bool debug = false;
	bool ready = false;
	char log[LOG_LENGTH];
	char *logP;

//	BaseType_t xRet;
//	BaseType_t xEndPointCount = 0;
//
//	NetworkInterface_t *xInterfaces;
//	NetworkEndPoint_t *xEndPoints;
//
//	static eMAC_INIT_STATUS_TYPE xMacInitStatus;// = eMACInit;
//
//	static SemaphoreHandle_t xTXDescriptorSemaphore;// = NULL;
//	static SemaphoreHandle_t xTransmissionMutex;
//
//	static ETH_TxPacketConfig xTxConfig;
//	static NetworkInterface_t * pxMyInterface;// = NULL;
//	static uint32_t ulIPAddress;
//
//	static EthernetPhy_t xPhyObject;
};

extern NetServices *NS;

#endif

#endif /* NETSERVICES_H_ */

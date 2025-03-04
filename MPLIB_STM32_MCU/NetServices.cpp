#include <NetServices.h>

#include "mpapp.h"
#include "stm32h743i_eval.h"
#include "stm32h743i_eval_io.h"

#include "cmsis_os.h"


#include <MQTTServices.h>


//===================================================================================================
//
//===================================================================================================
extern RNG_HandleTypeDef hrng;

//===================================================================================================
//
//===================================================================================================
#include <stdint.h>
#include "stdio.h"
#include <DataServices.h>

//#include "../../FreeRTOS-Plus-TCP/source/include/FreeRTOS_IP.h"
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTOS+TCP includes. */
//#include "../../FreeRTOS-Plus-TCP/source/include/FreeRTOS_IP.h"

#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP_Private.h"
#include "FreeRTOS_DNS.h"
#include "FreeRTOS_Routing.h"
#include "NetworkBufferManagement.h"

//#include "NetworkInterface.h"

//#include "../../FreeRTOS-Plus-TCP/source/portable/NetworkInterface/include/phyHandling.h"

/* ST includes. */
#include "stm32h7xx_hal.h"

//#ifndef STM32Hxx_HAL_ETH_H
#ifndef STM32H7xx_HAL_ETH_H

/*
 * The ST HAL library provides stm32h7xx_hal_eth.{c,h}.
 * This FreeRTOS+TCP driver renamed these files to stm32hxx_hal_eth.{c,h}
 * by removing the '7'.
 * Please make sure that "portable/NetworkInterface/STM32Hxx" is included
 * in the include paths earlier than "STM32H7xx_HAL_Driver/Inc".
 * and also make sure that you have defined 'HAL_ETH_MODULE_ENABLED'
 * in your copy of "stm32h7xx_hal_conf".
 */
    #error stm32hxx_hal_eth.h is possibly not included
#endif

#define ipconfigMAC_INTERRUPT_PRIORITY	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY )

/* Definition of the Ethernet driver buffers size and count */
//#define ETH_RX_BUF_SIZE                1524 /* buffer size for receive               */
//#define ETH_TX_BUF_SIZE                ETH_MAX_PACKET_SIZE /* buffer size for transmit              */
#define ETH_RX_BUF_SIZE                ( ipconfigNETWORK_MTU + 36 )
#define ETH_TX_BUF_SIZE                ( ipconfigNETWORK_MTU + 36 )

#ifndef ETH_RXBUFNB
	#define ETH_RXBUFNB                    4U       /* 4 Rx buffers of size ETH_RX_BUF_SIZE  */
#endif

#ifndef ETH_TXBUFNB
	#define ETH_TXBUFNB                    4U       /* 4 Tx buffers of size ETH_TX_BUF_SIZE  */
#endif

//===================================================================================================
//
//===================================================================================================

///* Default MAC address configuration. */
//
//#define configMAC_ADDR0		0x11
//#define configMAC_ADDR1		0x22
//#define configMAC_ADDR2		0x33
//#define configMAC_ADDR3		0x44
//#define configMAC_ADDR4		0x55
//#define configMAC_ADDR5		0x66
//
///* Default IP address configuration.  Used in case ipconfigUSE_DHCP is set to 0,
//or ipconfigUSE_DHCP is set to 1 but a DHCP server cannot be contacted. */
//#define configIP_ADDR0		192
//#define configIP_ADDR1		168
//#define configIP_ADDR2		13
//#define configIP_ADDR3		113
//
///* Default gateway IP address configuration.  Used in case ipconfigUSE_DHCP is
//set to 0, or ipconfigUSE_DHCP is set to 1 but a DHCP server cannot be contacted. */
//#define configGATEWAY_ADDR0	192
//#define configGATEWAY_ADDR1	168
//#define configGATEWAY_ADDR2	13
//#define configGATEWAY_ADDR3	1
//
///* Default DNS server configuration.  OpenDNS addresses are 208.67.222.222 and
//208.67.220.220.  Used in ipconfigUSE_DHCP is set to 0, or ipconfigUSE_DHCP is set
//to 1 but a DHCP server cannot be contacted.*/
//#define configDNS_SERVER_ADDR0 	8
//#define configDNS_SERVER_ADDR1 	8
//#define configDNS_SERVER_ADDR2 	8
//#define configDNS_SERVER_ADDR3 	8
//
///* Default netmask configuration.  Used in case ipconfigUSE_DHCP is set to 0,
//or ipconfigUSE_DHCP is set to 1 but a DHCP server cannot be contacted. */
//#define configNET_MASK0		255
//#define configNET_MASK1		255
//#define configNET_MASK2		255
//#define configNET_MASK3		0
//
///* The address of an echo server that will be used by the two demo echo client
//tasks.
//http://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/TCP_Echo_Clients.html
//http://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/UDP_Echo_Clients.html */
//#define configECHO_SERVER_ADDR0	192
//#define configECHO_SERVER_ADDR1 168
//#define configECHO_SERVER_ADDR2 13
//#define configECHO_SERVER_ADDR3 28
//
///* The address used as the root by the HTTP server. */
//#define configHTTP_ROOT "/"
//
///* If configINCLUDE_DEMO_DEBUG_STATS is set to one, then a few basic IP trace
//macros are defined to gather some UDP stack statistics that can then be viewed
//through the CLI interface. */
//#define configINCLUDE_DEMO_DEBUG_STATS 		0
//
///* Set configINCLUDE_QUERY_HEAP_COMMAND to 1 to include the query-heap command
//in the CLI. */
//#define configINCLUDE_QUERY_HEAP_COMMAND 	0
//
///****** UDP logging settings. *************************************************
//See http://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/UDP_Logging.html */
//
///* If set to 1 then each message sent via the UDP logging facility will end
//with \r.  If set to 0 then each message sent via the UDP logging facility will
//end with . */
//#define configUDP_LOGGING_NEEDS_CR_LF  ( 1 )
//
///* Sets the maximum length for a string sent via the UDP logging facility. */
//#define configUDP_LOGGING_STRING_LENGTH	( 200 )
//
///* The UDP logging facility buffers messages until the UDP logging task is able
//to transmit them.  configUDP_LOGGING_MAX_MESSAGES_IN_BUFFER sets the maximum
//number of messages that can be buffered at any one time. */
//#define	configUDP_LOGGING_MAX_MESSAGES_IN_BUFFER	( 20 )
//
///* The UDP logging facility creates a task to send buffered messages to the UDP
//port.  configUDP_LOGGING_TASK_STACK_SIZE sets the task's stack size. */
//#define	configUDP_LOGGING_TASK_STACK_SIZE  	( 512 )
//
///* The UDP logging facility creates a task to send buffered messages to the UDP
//port.  configUDP_LOGGING_TASK_PRIORITY sets the task's priority.  It is
//suggested to give the task a low priority to ensure it does not adversely effect
//the performance of other TCP/IP stack activity. */
//#define configUDP_LOGGING_TASK_PRIORITY   	( tskIDLE_PRIORITY + 1 )
//
///* The UDP port to which the UDP logging facility sends messages. */
//#define configUDP_LOGGING_PORT_LOCAL	1499
//#define configUDP_LOGGING_PORT_REMOTE	1500
//
///* Here it is possible to define the target IP address to send all UDP
//logging as e.g.:
//
//#define configUDP_LOGGING_ADDR0	192
//#define configUDP_LOGGING_ADDR1	168
//#define configUDP_LOGGING_ADDR2	0
//#define configUDP_LOGGING_ADDR3	3
//
//If not defined, the local broadcast address will be used, e.g. 192.168.0.255
//*/
//#define configUDP_LOGGING_ADDR0	192
//#define configUDP_LOGGING_ADDR1	168
//#define configUDP_LOGGING_ADDR2	13
//#define configUDP_LOGGING_ADDR3	255

//===================================================================================================
//
//===================================================================================================

/* Interrupt events to process: reception, transmission and error handling. */
#define EMAC_IF_RX_EVENT                1U
#define EMAC_IF_TX_EVENT                2U
#define EMAC_IF_ERR_EVENT               4U

/*
 * Enable either Hash or Perfect Filter, Multicast filter - None,
 * Enable Hash Multicast (HMC), and Enable Hash Unicast (HUC).
 */
//#define ENABLE_HASH_FILTER_SETTINGS      (uint32_t) 0x00000416U
//#define ENABLE_HASH_FILTER_SETTINGS     (uint32_t) 0x7FFFFFFF
//#define ENABLE_HASH_FILTER_SETTINGS     (uint32_t) 0x80000000
#define ENABLE_HASH_FILTER_SETTINGS     (uint32_t) 0xFFFFFFFF

#ifndef niEMAC_HANDLER_TASK_NAME
    #define niEMAC_HANDLER_TASK_NAME    "EMAC-task"
#endif

#ifndef niEMAC_HANDLER_TASK_STACK_SIZE
    #define niEMAC_HANDLER_TASK_STACK_SIZE    ( 4 * configMINIMAL_STACK_SIZE )
#endif

#ifndef niEMAC_HANDLER_TASK_PRIORITY
//    #define niEMAC_HANDLER_TASK_PRIORITY    configMAX_PRIORITIES - 1
	#define niEMAC_HANDLER_TASK_PRIORITY    24
#endif


const uint8_t ucIPAddress[ 4 ] = { 192, 168, 13, 133 };
const uint8_t ucNetMask[ 4 ] =  { 255, 255, 255, 0 };
const uint8_t ucGatewayAddress[ 4 ] = { 192, 168, 13, 1 };
const uint8_t ucDNSServerAddress[ 4 ] = { 8, 8, 8, 8 };
const uint8_t ucMACAddress[ 6 ] = { 0x00, 0x80, 0xE1, 0x11, 0x22, 0x55 };


////static
//int32_t ETH_PHY_IO_ReadReg( uint32_t ulDevAddr, uint32_t ulRegAddr, uint32_t * pulRegVal );
////static
//int32_t ETH_PHY_IO_WriteReg( uint32_t ulDevAddr, uint32_t ulRegAddr, uint32_t pulRegVal );

//static
extern ETH_HandleTypeDef heth;
//extern TaskHandle_t prvEMACTaskHandle;
static volatile uint32_t ulISREvents;

/* Holds the handle of the task used as a deferred interrupt processor.  The
 * handle is used so direct notifications can be sent to the task for all EMAC/DMA
 * related interrupts. */
static TaskHandle_t xEMACTaskHandle = NULL;
//extern osThreadId_t xEMACTaskHandle;

//int32_t ETH_PHY_IO_ReadReg( uint32_t DevAddr,
//                                   uint32_t RegAddr,
//                                   uint32_t * pRegVal );
//int32_t ETH_PHY_IO_WriteReg( uint32_t DevAddr,
//                                    uint32_t RegAddr,
//                                    uint32_t RegVal );
//
//typedef enum
//{
//    eMACInit,   /* Must initialise MAC. */
//    eMACPass,   /* Initialisation was successful. */
//    eMACFailed, /* Initialisation failed. */
//} eMAC_INIT_STATUS_TYPE;

/* About the section ".ethernet_data" : the DMA wants the descriptors and buffers allocated in the
 * RAM3 memory, which can be added to the .LD file as follows::
 *
 * RAM3 (xrw)      : ORIGIN = 0x24040000, LENGTH = 0x8000
 *
 * .ethernet_data :
 * {
 *  PROVIDE_HIDDEN (__ethernet_data_start = .);
 *  KEEP (*(SORT(.ethernet_data.*)))
 *  KEEP (*(.ethernet_data*))
 *  PROVIDE_HIDDEN (__ethernet_data_end = .);
 * } >RAM3
 *
 */


/* Ethernet Tx DMA Descriptors */
//ETH_DMADescTypeDef DMATxDscrTab[ ETH_TX_DESC_CNT ]    __attribute__( ( section( ".ethernet_data" ), aligned( 32 ) ) ); //at(0x24000000),
extern ETH_DMADescTypeDef DMATxDscrTab[ ETH_TX_DESC_CNT ];

/* Ethernet Transmit Buffer */
#if ( ipconfigZERO_COPY_TX_DRIVER == 0 )
    uint8_t Tx_Buff[ ETH_TX_DESC_CNT ][ ETH_TX_BUF_SIZE ]  __attribute__( ( section( ".ethernet_data" ), aligned( 32 ) ) );
#endif

/* Ethernet Rx DMA Descriptors */
//ETH_DMADescTypeDef DMARxDscrTab[ ETH_RX_DESC_CNT ]    __attribute__( ( section( ".ethernet_data" ), aligned( 32 ) ) ); //at(0x24000100),
extern ETH_DMADescTypeDef DMARxDscrTab[ ETH_RX_DESC_CNT ];

/* Ethernet Receive Buffer */
#if ( ipconfigZERO_COPY_RX_DRIVER == 0 )
	uint8_t Rx_Buff[ ETH_RX_DESC_CNT ][ ETH_RX_BUF_SIZE ] __attribute__( ( section( ".ethernet_data" ), aligned( 32 ) ) );
#endif

uint8_t ucNetworkPackets[ ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS * ETH_RX_BUF_SIZE ]

#if ( ipconfigZERO_COPY_RX_DRIVER != 0 || ipconfigZERO_COPY_TX_DRIVER != 0 )
	__attribute__( ( section( ".ethernet_data" ) ) )
#endif /* ( ipconfigZERO_COPY_RX_DRIVER != 0 || ipconfigZERO_COPY_TX_DRIVER != 0 ) */

__attribute__( ( aligned( 32 ) ) );


//const PhyProperties_t xPHYProperties =
//{
//    .ucSpeed  = PHY_SPEED_AUTO,
//    .ucDuplex = PHY_DUPLEX_AUTO,
//    .ucMDI_X  = PHY_MDIX_DIRECT
//};
const PhyProperties_t xPHYProperties =
{
    .ucSpeed  = PHY_SPEED_AUTO,
    .ucMDI_X  = PHY_MDIX_DIRECT,
	.ucDuplex = PHY_DUPLEX_AUTO,
	.ucSpare = 0 //MP ADDED...
};

//static SemaphoreHandle_t xTXDescriptorSemaphore;// = NULL;
static SemaphoreHandle_t xTransmissionMutex;
static eMAC_INIT_STATUS_TYPE xMacInitStatus = eMACInit;
SemaphoreHandle_t xTXDescriptorSemaphore = NULL;
static ETH_TxPacketConfig xTxConfig;
static NetworkInterface_t * pxMyInterface = NULL;
static uint32_t ulIPAddress;
static EthernetPhy_t xPhyObject;

QueueHandle_t xPingReplyQueue;

BaseType_t xRet;
BaseType_t xEndPointCount = 0;

NetworkInterface_t xInterfaces[1];
NetworkEndPoint_t xEndPoints[4];

extern QueueHandle_t xNetworkEventQueue;
//===================================================================================================
//
//===================================================================================================
int NetServices::iNet = 0;
NetServices *NetServices::instance=NULL;
char* NetServices::name = (char*)pvPortMalloc(CAT_LENGTH * sizeof(char));

//===================================================================================================
//
//===================================================================================================
uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress, uint16_t usSourcePort, uint32_t ulDestinationAddress, uint16_t usDestinationPort ) {
	return NS->ulApplicationGetNextSequenceNumber( ulSourceAddress, usSourcePort, ulDestinationAddress, usDestinationPort );
}

void vApplicationIPNetworkEventHook_Multi( eIPCallbackEvent_t eNetworkEvent,  struct xNetworkEndPoint * pxEndPoint ) {
	return NS->vApplicationIPNetworkEventHook_Multi( eNetworkEvent, pxEndPoint );
}

void vApplicationPingReplyHook( ePingReplyStatus_t eStatus, uint16_t usIdentifier ) {
	return NS->vApplicationPingReplyHook( eStatus, usIdentifier );
}

BaseType_t xApplicationGetRandomNumber( uint32_t * pulNumber ) {
	return NS->xApplicationGetRandomNumber( pulNumber );
}

eDHCPCallbackAnswer_t xApplicationDHCPHook_Multi( eDHCPCallbackPhase_t eDHCPPhase, struct xNetworkEndPoint * pxEndPoint,IP_Address_t * pxIPAddress ) {
	return NS->xApplicationDHCPHook_Multi( eDHCPPhase, pxEndPoint, pxIPAddress );
}

void vNetworkInterfaceAllocateRAMToBuffers( NetworkBufferDescriptor_t pxNetworkBuffers[ ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS ] ) {
	return NS->vNetworkInterfaceAllocateRAMToBuffers( pxNetworkBuffers );
}

uint8_t isNetworkStarted() {
	return NS->start ? 1:0;
}

//void prvEMACHandlerTask( void * pvParameters ) {
//	return NS->prvEMACHandlerTask(pvParameters);
//}
//===================================================================================================
//
//===================================================================================================

void StartNetServices(void *argument) {
	uint32_t tickstart = 1000; //HAL_GetTick();
	NetServices *NS = NetServices::CreateInstance();

	NS->init();

	BSP_LED_Toggle(LED_ORANGE);

	uint8_t runPing = 5; //5*THREAD_HEARTBEAT
	uint8_t willPing = 5; //5*THREAD_HEARTBEAT
	for(;;) {
	  if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
		  BSP_LED_Toggle(LED_ORANGE);
		  tickstart = HAL_GetTick();

		  if(mpappGetStatus_ETH() == NET_OK) {
			  if(mpappGetStatus_IP() == IP_OK) {
				  if( willPing >= runPing ) {
					  willPing = 0;
//					  NS->vSendPingGW();
				  }
				  else {
					  willPing++;
				  }
			  }
		  }
	  }

	  if(mpappGetStatus_ETH() == NET_OK) {
		  if(mpappGetStatus_IP() == IP_OK) {
			  if(!MQ->IsConnected()) {
				  if(!MQ->start) {
					  MQ->Start();
				  }
			  }
		  }
		  else {
			  if(MQ->IsConnected()) {
				  if(!MQ->stop) {
					  MQ->Stop();
				  }
			  }
		  }
	  }
	  else {
		  if(mpappGetStatus_IP() == IP_OK) {
			  SetStatus_IP(IP_NOTOK);
		  }
		  if(MQ->IsConnected()) {
			  MQ->Stop();
		  }
	  }

	  osDelay(100);
	}
}

//===================================================================================================
//
//===================================================================================================
void NetServices::PushToLogsError(int code) {
	DS->pushToLogsMon(name, LOG_INFO, GetErrorDescription(code));
}

void NetServices::PushToLogs(char *alog, ...) {
	va_list args;
	va_start(args,alog);

	logP = (char*)pvPortMalloc(TEXTBUFF_SIZE*sizeof(char));

	vsnprintf(logP,TEXTBUFF_SIZE,alog,args);
	va_end(args);

	DS->pushToLogsMon(name, LOG_OK, logP);

	vPortFree((char*)logP);
}

ETH_HandleTypeDef* NetServices::getHeth() {
	return &heth;
}

uint8_t NetServices::getNumberEndpoints() {
	return sizeof(xEndPoints);
}

NetworkInterface_t* NetServices::getFirstInterface() {
//	return FreeRTOS_FirstNetworkInterface();
	return &(xInterfaces[0]);
}

NetworkEndPoint_t* NetServices::getEndPoint(uint8_t index) {
//	return FreeRTOS_FirstNetworkInterface();
	return &(xEndPoints[index]);
}

NetworkEndPoint_t* NetServices::getFirstEndPoint(NetworkInterface_t* interface) {
//	return FreeRTOS_FirstNetworkInterface();
	return FreeRTOS_FirstEndPoint(interface);
//	return &(xEndPoints[0]);
}

void prvEMACHandlerTask( void * pvParameters )
{
/* When sending a packet, all descriptors in the transmission channel may
 * be occupied.  In stat case, the program will wait (block) for the counting
 * semaphore. */
	const TickType_t ulMaxBlockTime = pdMS_TO_TICKS( 100U );

	#if ( ipconfigHAS_PRINTF != 0 )
		size_t uxTXDescriptorsUsed = 0U;
		size_t uxRXDescriptorsUsed = ETH_RX_DESC_CNT;
	#endif

	( void ) pvParameters;

	for( ; ; )
	{
		//MP if using code from cubemx designed...
//		while(!NS->start) { osThreadYield(); }

		BaseType_t xResult = 0;

		#if ( ipconfigHAS_PRINTF != 0 )
		{
			size_t uxUsed;
			size_t uxOwnCount;

			/* Call a function that monitors resources: the amount of free network
			 * buffers and the amount of free space on the heap.  See FreeRTOS_IP.c
			 * for more detailed comments. */
			vPrintResourceStats();

			/* Some more statistics: number of free descriptors. */
			uxUsed = ETH_TX_DESC_CNT - uxSemaphoreGetCount( xTXDescriptorSemaphore );

			if( uxTXDescriptorsUsed < uxUsed )
			{
				uxTXDescriptorsUsed = uxUsed;
				FreeRTOS_printf( ( "TX descriptors %u/%u",
								   uxTXDescriptorsUsed,
								   ETH_TX_DESC_CNT ) );
			}

			uxOwnCount = NS->uxGetOwnCount( &( heth ) );

			if( uxRXDescriptorsUsed > uxOwnCount )
			{
				uxRXDescriptorsUsed = uxOwnCount;
				FreeRTOS_printf( ( "RX descriptors %u/%u",
								   uxRXDescriptorsUsed,
								   ETH_RX_DESC_CNT ) );
			}
		}
		#endif /* ( ipconfigHAS_PRINTF != 0 ) */

		ulTaskNotifyTake( pdFALSE, ulMaxBlockTime );

		/* Wait for the Ethernet MAC interrupt to indicate that another packet
		 * has been received. */
		if( ( ulISREvents & EMAC_IF_RX_EVENT ) != 0U )
		{
//			iptraceNETWORK_EVENT_ISR_RX( ulISREvents );
			NS->vClearOptionBit( &( ulISREvents ), EMAC_IF_RX_EVENT );
			xResult = NS->prvNetworkInterfaceInput();
		}

		/* When a packet has been transmitted, the descriptor must be
		 * prepared for a next transmission.
		 * When using zero-copy, the network buffer must be released
		 * ( i.e. returned to the pool of network buffers ). */

		if( ( ulISREvents & EMAC_IF_TX_EVENT ) != 0U )
		{
//			iptraceNETWORK_EVENT_ISR_TX( ulISREvents );
			NS->vClearOptionBit( &( ulISREvents ), EMAC_IF_TX_EVENT );

			if( xSemaphoreTake( xTransmissionMutex, 10000U ) != pdFAIL )
			{
				ETH_Clear_Tx_Descriptors( &( heth ) );
				xSemaphoreGive( xTransmissionMutex );
			}
		}

		/* Some error has occurred, possibly an overflow or an underflow. */
		if( ( ulISREvents & EMAC_IF_ERR_EVENT ) != 0U )
		{
//			iptraceNETWORK_EVENT_ISR_ERR( ulISREvents );
			NS->vClearOptionBit( &( ulISREvents ), EMAC_IF_ERR_EVENT );

			heth.gState = HAL_ETH_STATE_READY;
			/* Enable all interrupts */
			HAL_ETH_Start_IT( &( heth ) );
			xResult += NS->prvNetworkInterfaceInput();
		}

		if( xPhyCheckLinkStatus( &xPhyObject, xResult ) != pdFALSE )
		{
			/*
			 * The function xPhyCheckLinkStatus() returns pdTRUE if the
			 * Link Status has changes since it was called the last time.
			 */
			if( NS->xSTM32H_GetPhyLinkStatus( pxMyInterface ) == pdFALSE )
			{
				/* Stop the DMA transfer. */
				HAL_ETH_Stop_IT( &( heth ) );
				/* Clear the Transmit buffers. */
				memset( &( DMATxDscrTab ), '\0', sizeof( DMATxDscrTab ) );
				/* Since the link is down, clear the descriptors. */
				ETH_Clear_Tx_Descriptors( &( heth ) );
			}
			else
			{
				/* Something has changed to a Link Status, need re-check. */
				NS->prvEthernetUpdateConfig( pdFALSE );
			}
		}
	}
}

#include "FreeRTOS_IP.h"
#include "FreeRTOS_IP_Timers.h"
#include "FreeRTOS_ICMP.h"

#ifndef ipINITIALISATION_RETRY_DELAY
    #define ipINITIALISATION_RETRY_DELAY    ( pdMS_TO_TICKS( 3000U ) )
#endif
#if ( ( ipconfigUSE_TCP == 1 ) && !defined( ipTCP_TIMER_PERIOD_MS ) )
    /** @brief When initialising the TCP timer, give it an initial time-out of 1 second. */
    #define ipTCP_TIMER_PERIOD_MS    ( 1000U )
#endif
#if ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES == 0
    #define ipCONSIDER_FRAME_FOR_PROCESSING( pucEthernetBuffer )    eConsiderFrameForProcessing( ( pucEthernetBuffer ) )
#else
    #define ipCONSIDER_FRAME_FOR_PROCESSING( pucEthernetBuffer )    eProcessBuffer
#endif
#ifndef ipARP_RESOLUTION_MAX_DELAY
    #define ipARP_RESOLUTION_MAX_DELAY    ( pdMS_TO_TICKS( 2000U ) )
#endif

BaseType_t xIPTaskInitialised = pdFALSE;
volatile BaseType_t xNetworkDownEventPending = pdFALSE;
//TaskHandle_t xIPTaskHandle = NULL;
extern osThreadId_t xIPTaskHandle;


static void prvIPTask_Initialise( void )
{
    NetworkInterface_t * pxInterface;

    /* A possibility to set some additional task properties. */
//    iptraceIP_TASK_STARTING();

    /* Generate a dummy message to say that the network connection has gone
     * down.  This will cause this task to initialise the network interface.  After
     * this it is the responsibility of the network interface hardware driver to
     * send this message if a previously connected network is disconnected. */

    vNetworkTimerReload( pdMS_TO_TICKS( ipINITIALISATION_RETRY_DELAY ) );

    for( pxInterface = pxNetworkInterfaces; pxInterface != NULL; pxInterface = pxInterface->pxNext )
    {
        /* Post a 'eNetworkDownEvent' for every interface. */
        FreeRTOS_NetworkDown( pxInterface );
    }

    #if ( ipconfigUSE_TCP == 1 )
    {
        /* Initialise the TCP timer. */
        vTCPTimerReload( pdMS_TO_TICKS( ipTCP_TIMER_PERIOD_MS ) );
    }
    #endif

    /* Mark the timer as inactive since we are not waiting on any ARP resolution as of now. */
    vIPSetARPResolutionTimerEnableState( pdFALSE );

    #if ( ( ipconfigDNS_USE_CALLBACKS != 0 ) && ( ipconfigUSE_DNS != 0 ) )
    {
        /* The following function is declared in FreeRTOS_DNS.c and 'private' to
         * this library */
        vDNSInitialise();
    }
    #endif /* ( ipconfigDNS_USE_CALLBACKS != 0 ) && ( ipconfigUSE_DNS != 0 ) */

    #if ( ( ipconfigUSE_DNS_CACHE != 0 ) && ( ipconfigUSE_DNS != 0 ) )
    {
        /* Clear the DNS cache once only. */
        FreeRTOS_dnsclear();
    }
    #endif /* ( ( ipconfigUSE_DNS_CACHE != 0 ) && ( ipconfigUSE_DNS != 0 ) ) */

    /* Initialisation is complete and events can now be processed. */
    xIPTaskInitialised = pdTRUE;
}

static eFrameProcessingResult_t prvProcessUDPPacket( NetworkBufferDescriptor_t * const pxNetworkBuffer )
{
    eFrameProcessingResult_t eReturn = eReleaseBuffer;
    BaseType_t xIsWaitingARPResolution = pdFALSE;
    /* The IP packet contained a UDP frame. */
    /* MISRA Ref 11.3.1 [Misaligned access] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Plus-TCP/blob/main/MISRA.md#rule-113 */
    /* coverity[misra_c_2012_rule_11_3_violation] */
    const UDPPacket_t * pxUDPPacket = ( ( UDPPacket_t * ) pxNetworkBuffer->pucEthernetBuffer );
    const UDPHeader_t * pxUDPHeader = &( pxUDPPacket->xUDPHeader );

    size_t uxMinSize = ipSIZE_OF_ETH_HEADER + ( size_t ) uxIPHeaderSizePacket( pxNetworkBuffer ) + ipSIZE_OF_UDP_HEADER;
    size_t uxLength;
    uint16_t usLength;

    #if ( ipconfigUSE_IPv6 != 0 )
        if( pxUDPPacket->xEthernetHeader.usFrameType == ipIPv6_FRAME_TYPE )
        {
            const ProtocolHeaders_t * pxProtocolHeaders;

            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Plus-TCP/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            pxProtocolHeaders = ( ( ProtocolHeaders_t * ) &( pxNetworkBuffer->pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER + ipSIZE_OF_IPv6_HEADER ] ) );
            pxUDPHeader = &( pxProtocolHeaders->xUDPHeader );
        }
    #endif /* ( ipconfigUSE_IPv6 != 0 ) */

    usLength = FreeRTOS_ntohs( pxUDPHeader->usLength );
    uxLength = ( size_t ) usLength;

    /* Note the header values required prior to the checksum
     * generation as the checksum pseudo header may clobber some of
     * these values. */
    #if ( ipconfigUSE_IPv4 != 0 )
        if( ( pxUDPPacket->xEthernetHeader.usFrameType == ipIPv4_FRAME_TYPE ) &&
            ( usLength > ( FreeRTOS_ntohs( pxUDPPacket->xIPHeader.usLength ) - uxIPHeaderSizePacket( pxNetworkBuffer ) ) ) )
        {
            eReturn = eReleaseBuffer;
        }
        else
    #endif /* ( ipconfigUSE_IPv4 != 0 ) */

    if( ( pxNetworkBuffer->xDataLength >= uxMinSize ) &&
        ( uxLength >= sizeof( UDPHeader_t ) ) )
    {
        size_t uxPayloadSize_1, uxPayloadSize_2;

        /* Ensure that downstream UDP packet handling has the lesser
         * of: the actual network buffer Ethernet frame length, or
         * the sender's UDP packet header payload length, minus the
         * size of the UDP header.
         *
         * The size of the UDP packet structure in this implementation
         * includes the size of the Ethernet header, the size of
         * the IP header, and the size of the UDP header. */
        uxPayloadSize_1 = pxNetworkBuffer->xDataLength - uxMinSize;
        uxPayloadSize_2 = uxLength - ipSIZE_OF_UDP_HEADER;

        if( uxPayloadSize_1 > uxPayloadSize_2 )
        {
            pxNetworkBuffer->xDataLength = uxPayloadSize_2 + uxMinSize;
        }

        pxNetworkBuffer->usPort = pxUDPHeader->usSourcePort;
        pxNetworkBuffer->xIPAddress.ulIP_IPv4 = pxUDPPacket->xIPHeader.ulSourceIPAddress;

        /* ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM:
         * In some cases, the upper-layer checksum has been calculated
         * by the NIC driver. */

        /* Pass the packet payload to the UDP sockets
         * implementation. */
        if( xProcessReceivedUDPPacket( pxNetworkBuffer,
                                       pxUDPHeader->usDestinationPort,
                                       &( xIsWaitingARPResolution ) ) == pdPASS )
        {
            eReturn = eFrameConsumed;
        }
        else
        {
            /* Is this packet to be set aside for ARP resolution. */
            if( xIsWaitingARPResolution == pdTRUE )
            {
                eReturn = eWaitingARPResolution;
            }
        }
    }
    else
    {
        /* Length checks failed, the buffer will be released. */
    }

    return eReturn;
}

static eFrameProcessingResult_t prvProcessIPPacket( const IPPacket_t * pxIPPacket,
                                                    NetworkBufferDescriptor_t * const pxNetworkBuffer )
{
    eFrameProcessingResult_t eReturn;
    UBaseType_t uxHeaderLength = ipSIZE_OF_IPv4_HEADER;
    uint8_t ucProtocol = 0U;

    #if ( ipconfigUSE_IPv6 != 0 )
        const IPHeader_IPv6_t * pxIPHeader_IPv6 = NULL;
    #endif /* ( ipconfigUSE_IPv6 != 0 ) */

    #if ( ipconfigUSE_IPv4 != 0 )
        const IPHeader_t * pxIPHeader = &( pxIPPacket->xIPHeader );
    #endif /* ( ipconfigUSE_IPv4 != 0 ) */

    switch( pxIPPacket->xEthernetHeader.usFrameType )
    {
        #if ( ipconfigUSE_IPv6 != 0 )
            case ipIPv6_FRAME_TYPE:

                if( pxNetworkBuffer->xDataLength < sizeof( IPPacket_IPv6_t ) )
                {
                    /* The packet size is less than minimum IPv6 packet. */
                    eReturn = eReleaseBuffer;
                }
                else
                {
                    /* MISRA Ref 11.3.1 [Misaligned access] */
                    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Plus-TCP/blob/main/MISRA.md#rule-113 */
                    /* coverity[misra_c_2012_rule_11_3_violation] */
                    pxIPHeader_IPv6 = ( ( const IPHeader_IPv6_t * ) &( pxNetworkBuffer->pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER ] ) );

                    uxHeaderLength = ipSIZE_OF_IPv6_HEADER;
                    ucProtocol = pxIPHeader_IPv6->ucNextHeader;
                    /* MISRA Ref 11.3.1 [Misaligned access] */
                    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Plus-TCP/blob/main/MISRA.md#rule-113 */
                    /* coverity[misra_c_2012_rule_11_3_violation] */
                    eReturn = prvAllowIPPacketIPv6( ( ( const IPHeader_IPv6_t * ) &( pxIPPacket->xIPHeader ) ), pxNetworkBuffer, uxHeaderLength );

                    /* The IP-header type is copied to a special reserved location a few bytes before the message
                     * starts. In the case of IPv6, this value is never actually used and the line below can safely be removed
                     * with no ill effects. We only store it to help with debugging. */
                    pxNetworkBuffer->pucEthernetBuffer[ 0 - ( BaseType_t ) ipIP_TYPE_OFFSET ] = pxIPHeader_IPv6->ucVersionTrafficClass;
                }
                break;
        #endif /* ( ipconfigUSE_IPv6 != 0 ) */

        #if ( ipconfigUSE_IPv4 != 0 )
            case ipIPv4_FRAME_TYPE:
               {
                   size_t uxLength = ( size_t ) pxIPHeader->ucVersionHeaderLength;

                   /* Check if the IP headers are acceptable and if it has our destination.
                    * The lowest four bits of 'ucVersionHeaderLength' indicate the IP-header
                    * length in multiples of 4. */
                   uxHeaderLength = ( size_t ) ( ( uxLength & 0x0FU ) << 2 );

                   if( ( uxHeaderLength > ( pxNetworkBuffer->xDataLength - ipSIZE_OF_ETH_HEADER ) ) ||
                       ( uxHeaderLength < ipSIZE_OF_IPv4_HEADER ) )
                   {
                       eReturn = eReleaseBuffer;
                   }
                   else
                   {
                       ucProtocol = pxIPPacket->xIPHeader.ucProtocol;
                       /* Check if the IP headers are acceptable and if it has our destination. */
                       eReturn = prvAllowIPPacketIPv4( pxIPPacket, pxNetworkBuffer, uxHeaderLength );

                       {
                           /* The IP-header type is copied to a special reserved location a few bytes before the
                            * messages starts.  It might be needed later on when a UDP-payload
                            * buffer is being used. */
                           pxNetworkBuffer->pucEthernetBuffer[ 0 - ( BaseType_t ) ipIP_TYPE_OFFSET ] = pxIPHeader->ucVersionHeaderLength;
                       }
                   }

                   break;
               }
        #endif /* ( ipconfigUSE_IPv4 != 0 ) */

        default:
            eReturn = eReleaseBuffer;
            FreeRTOS_debug_printf( ( "prvProcessIPPacket: Undefined Frame Type " ) );
            /* MISRA 16.4 Compliance */
            break;
    }

    /* MISRA Ref 14.3.1 [Configuration dependent invariant] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Plus-TCP/blob/main/MISRA.md#rule-143 */
    /* coverity[misra_c_2012_rule_14_3_violation] */
    /* coverity[cond_const] */
    if( eReturn == eProcessBuffer )
    {
        /* Are there IP-options. */
        /* Case default is never toggled because eReturn is not eProcessBuffer in previous step. */
        switch( pxIPPacket->xEthernetHeader.usFrameType ) /* LCOV_EXCL_BR_LINE */
        {
            #if ( ipconfigUSE_IPv4 != 0 )
                case ipIPv4_FRAME_TYPE:

                    if( uxHeaderLength > ipSIZE_OF_IPv4_HEADER )
                    {
                        /* The size of the IP-header is larger than 20 bytes.
                         * The extra space is used for IP-options. */
                        eReturn = prvCheckIP4HeaderOptions( pxNetworkBuffer );
                    }
                    break;
            #endif /* ( ipconfigUSE_IPv4 != 0 ) */

            #if ( ipconfigUSE_IPv6 != 0 )
                case ipIPv6_FRAME_TYPE:

                    if( xGetExtensionOrder( ucProtocol, 0U ) > 0 )
                    {
                        eReturn = eHandleIPv6ExtensionHeaders( pxNetworkBuffer, pdTRUE );

                        if( eReturn != eReleaseBuffer )
                        {
                            /* Ignore warning for `pxIPHeader_IPv6`. */
                            ucProtocol = pxIPHeader_IPv6->ucNextHeader;
                        }
                    }
                    break;
            #endif /* ( ipconfigUSE_IPv6 != 0 ) */

            /* Case default is never toggled because eReturn is not eProcessBuffer in previous step. */
            default:   /* LCOV_EXCL_LINE */
                /* MISRA 16.4 Compliance */
                break; /* LCOV_EXCL_LINE */
        }

        /* MISRA Ref 14.3.1 [Configuration dependent invariant] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Plus-TCP/blob/main/MISRA.md#rule-143 */
        /* coverity[misra_c_2012_rule_14_3_violation] */
        /* coverity[const] */
        if( eReturn != eReleaseBuffer )
        {
            /* Add the IP and MAC addresses to the ARP table if they are not
             * already there - otherwise refresh the age of the existing
             * entry. */
            if( ucProtocol != ( uint8_t ) ipPROTOCOL_UDP )
            {
                if( xCheckRequiresARPResolution( pxNetworkBuffer ) == pdTRUE )
                {
                    eReturn = eWaitingARPResolution;
                }
                else
                {
                    /* Refresh the ARP cache with the IP/MAC-address of the received
                     * packet.  For UDP packets, this will be done later in
                     * xProcessReceivedUDPPacket(), as soon as it's know that the message
                     * will be handled.  This will prevent the ARP cache getting
                     * overwritten with the IP address of useless broadcast packets. */
                    /* Case default is never toggled because eReturn is not eProcessBuffer in previous step. */
                    switch( pxIPPacket->xEthernetHeader.usFrameType ) /* LCOV_EXCL_BR_LINE */
                    {
                        #if ( ipconfigUSE_IPv6 != 0 )
                            case ipIPv6_FRAME_TYPE:
                                vNDRefreshCacheEntry( &( pxIPPacket->xEthernetHeader.xSourceAddress ), &( pxIPHeader_IPv6->xSourceAddress ), pxNetworkBuffer->pxEndPoint );
                                break;
                        #endif /* ( ipconfigUSE_IPv6 != 0 ) */

                        #if ( ipconfigUSE_IPv4 != 0 )
                            case ipIPv4_FRAME_TYPE:
                                /* Refresh the age of this cache entry since a packet was received. */
                                vARPRefreshCacheEntryAge( &( pxIPPacket->xEthernetHeader.xSourceAddress ), pxIPHeader->ulSourceIPAddress );
                                break;
                        #endif /* ( ipconfigUSE_IPv4 != 0 ) */

                        /* Case default is never toggled because eReturn is not eProcessBuffer in previous step. */
                        default:   /* LCOV_EXCL_LINE */
                            /* MISRA 16.4 Compliance */
                            break; /* LCOV_EXCL_LINE */
                    }
                }
            }

            if( eReturn != eWaitingARPResolution ) /*TODO eReturn != eReleaseBuffer */
            {
                switch( ucProtocol )
                {
                    #if ( ipconfigUSE_IPv4 != 0 )
                        case ipPROTOCOL_ICMP:

                            /* The IP packet contained an ICMP frame.  Don't bother checking
                             * the ICMP checksum, as if it is wrong then the wrong data will
                             * also be returned, and the source of the ping will know something
                             * went wrong because it will not be able to validate what it
                             * receives. */
                            #if ( ipconfigREPLY_TO_INCOMING_PINGS == 1 ) || ( ipconfigSUPPORT_OUTGOING_PINGS == 1 )
                            {
                                eReturn = ProcessICMPPacket( pxNetworkBuffer );
                            }
                            #endif /* ( ipconfigREPLY_TO_INCOMING_PINGS == 1 ) || ( ipconfigSUPPORT_OUTGOING_PINGS == 1 ) */
                            break;
                    #endif /* ( ipconfigUSE_IPv4 != 0 ) */

                    #if ( ipconfigUSE_IPv6 != 0 )
                        case ipPROTOCOL_ICMP_IPv6:
                            eReturn = prvProcessICMPMessage_IPv6( pxNetworkBuffer );
                            break;
                    #endif /* ( ipconfigUSE_IPv6 != 0 ) */

                    case ipPROTOCOL_UDP:
                        /* The IP packet contained a UDP frame. */

                        eReturn = prvProcessUDPPacket( pxNetworkBuffer );
                        break;

                        #if ipconfigUSE_TCP == 1
                            case ipPROTOCOL_TCP:

                                if( xProcessReceivedTCPPacket( pxNetworkBuffer ) == pdPASS )
                                {
                                    eReturn = eFrameConsumed;
                                }

                                /* Setting this variable will cause xTCPTimerCheck()
                                 * to be called just before the IP-task blocks. */
                                xProcessedTCPMessage++;
                                break;
                        #endif /* if ipconfigUSE_TCP == 1 */
                    default:
                        /* Not a supported frame type. */
                        eReturn = eReleaseBuffer;
                        break;
                }
            }
        }
    }

    return eReturn;
}

static void prvProcessEthernetPacket( NetworkBufferDescriptor_t * const pxNetworkBuffer )
{
    const EthernetHeader_t * pxEthernetHeader;
    eFrameProcessingResult_t eReturned = eReleaseBuffer;

    /* Use do{}while(pdFALSE) to allow the use of break; */
    do
    {
        /* prvHandleEthernetPacket() already checked for ( pxNetworkBuffer != NULL ) so
         * it is safe to break out of the do{}while() and let the second half of this
         * function handle the releasing of pxNetworkBuffer */

        if( ( pxNetworkBuffer->pxInterface == NULL ) || ( pxNetworkBuffer->pxEndPoint == NULL ) )
        {
            break;
        }

        /* Beyond this point,
         * ( pxNetworkBuffer != NULL ),
         * ( pxNetworkBuffer->pxInterface != NULL ),
         * ( pxNetworkBuffer->pxEndPoint != NULL ),
         * Additionally, FreeRTOS_FillEndPoint() and FreeRTOS_FillEndPoint_IPv6() guarantee
         * that endpoints always have a valid interface assigned to them, and consequently:
         * ( pxNetworkBuffer->pxEndPoint->pxInterface != NULL )
         * None of the above need to be checked again in code that handles incoming packets. */

        iptraceNETWORK_INTERFACE_INPUT( pxNetworkBuffer->xDataLength, pxNetworkBuffer->pucEthernetBuffer );

        /* Interpret the Ethernet frame. */
        if( pxNetworkBuffer->xDataLength < sizeof( EthernetHeader_t ) )
        {
            break;
        }

        eReturned = ipCONSIDER_FRAME_FOR_PROCESSING( pxNetworkBuffer->pucEthernetBuffer );

        /* Map the buffer onto the Ethernet Header struct for easy access to the fields. */

        /* MISRA Ref 11.3.1 [Misaligned access] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Plus-TCP/blob/main/MISRA.md#rule-113 */
        /* coverity[misra_c_2012_rule_11_3_violation] */
        pxEthernetHeader = ( ( const EthernetHeader_t * ) pxNetworkBuffer->pucEthernetBuffer );

        /* The condition "eReturned == eProcessBuffer" must be true. */
        #if ( ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES == 0 )
            if( eReturned == eProcessBuffer )
        #endif
        {
            /* Interpret the received Ethernet packet. */
            switch( pxEthernetHeader->usFrameType )
            {
                #if ( ipconfigUSE_IPv4 != 0 )
                    case ipARP_FRAME_TYPE:

                        /* The Ethernet frame contains an ARP packet. */
                        if( pxNetworkBuffer->xDataLength >= sizeof( ARPPacket_t ) )
                        {
                            /* MISRA Ref 11.3.1 [Misaligned access] */
                            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Plus-TCP/blob/main/MISRA.md#rule-113 */
                            /* coverity[misra_c_2012_rule_11_3_violation] */
                            eReturned = eARPProcessPacket( pxNetworkBuffer );
                        }
                        else
                        {
                            eReturned = eReleaseBuffer;
                        }
                        break;
                #endif /* ( ipconfigUSE_IPv4 != 0 ) */

                case ipIPv4_FRAME_TYPE:
                case ipIPv6_FRAME_TYPE:

                    /* The Ethernet frame contains an IP packet. */
                    if( pxNetworkBuffer->xDataLength >= sizeof( IPPacket_t ) )
                    {
                        /* MISRA Ref 11.3.1 [Misaligned access] */
                        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Plus-TCP/blob/main/MISRA.md#rule-113 */
                        /* coverity[misra_c_2012_rule_11_3_violation] */
                        eReturned = prvProcessIPPacket( ( ( IPPacket_t * ) pxNetworkBuffer->pucEthernetBuffer ), pxNetworkBuffer );
                    }
                    else
                    {
                        eReturned = eReleaseBuffer;
                    }

                    break;

                default:
                    #if ( ipconfigPROCESS_CUSTOM_ETHERNET_FRAMES != 0 )
                        /* Custom frame handler. */
                        eReturned = eApplicationProcessCustomFrameHook( pxNetworkBuffer );
                    #else
                        /* No other packet types are handled.  Nothing to do. */
                        eReturned = eReleaseBuffer;
                    #endif
                    break;
            } /* switch( pxEthernetHeader->usFrameType ) */
        }
    } while( pdFALSE );

    /* Perform any actions that resulted from processing the Ethernet frame. */
    switch( eReturned )
    {
        case eReturnEthernetFrame:

            /* The Ethernet frame will have been updated (maybe it was
             * an ARP request or a PING request?) and should be sent back to
             * its source. */
            vReturnEthernetFrame( pxNetworkBuffer, pdTRUE );

            /* parameter pdTRUE: the buffer must be released once
             * the frame has been transmitted */
            break;

        case eFrameConsumed:

            /* The frame is in use somewhere, don't release the buffer
             * yet. */
            break;

        case eWaitingARPResolution:

            if( pxARPWaitingNetworkBuffer == NULL )
            {
                pxARPWaitingNetworkBuffer = pxNetworkBuffer;
                vIPTimerStartARPResolution( ipARP_RESOLUTION_MAX_DELAY );

                iptraceDELAYED_ARP_REQUEST_STARTED();
            }
            else
            {
                /* We are already waiting on one ARP resolution. This frame will be dropped. */
                vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );

                iptraceDELAYED_ARP_BUFFER_FULL();
            }

            break;

        case eReleaseBuffer:
        case eProcessBuffer:
        default:

            /* The frame is not being used anywhere, and the
             * NetworkBufferDescriptor_t structure containing the frame should
             * just be released back to the list of free buffers. */
            vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
            break;
    }
}

static void prvHandleEthernetPacket( NetworkBufferDescriptor_t * pxBuffer )
{
    #if ( ipconfigUSE_LINKED_RX_MESSAGES == 0 )
    {
        /* When ipconfigUSE_LINKED_RX_MESSAGES is set to 0 then only one
         * buffer will be sent at a time.  This is the default way for +TCP to pass
         * messages from the MAC to the TCP/IP stack. */
        if( pxBuffer != NULL )
        {
            prvProcessEthernetPacket( pxBuffer );
        }
    }
    #else /* ipconfigUSE_LINKED_RX_MESSAGES */
    {
        NetworkBufferDescriptor_t * pxNextBuffer;

        /* An optimisation that is useful when there is high network traffic.
         * Instead of passing received packets into the IP task one at a time the
         * network interface can chain received packets together and pass them into
         * the IP task in one go.  The packets are chained using the pxNextBuffer
         * member.  The loop below walks through the chain processing each packet
         * in the chain in turn. */

        /* While there is another packet in the chain. */
        while( pxBuffer != NULL )
        {
            /* Store a pointer to the buffer after pxBuffer for use later on. */
            pxNextBuffer = pxBuffer->pxNextBuffer;

            /* Make it NULL to avoid using it later on. */
            pxBuffer->pxNextBuffer = NULL;

            prvProcessEthernetPacket( pxBuffer );
            pxBuffer = pxNextBuffer;
        }
    }
    #endif /* ipconfigUSE_LINKED_RX_MESSAGES */
}

static void prvForwardTxPacket( NetworkBufferDescriptor_t * pxNetworkBuffer,
                                BaseType_t xReleaseAfterSend )
{
    iptraceNETWORK_INTERFACE_OUTPUT( pxNetworkBuffer->xDataLength, pxNetworkBuffer->pucEthernetBuffer );

    if( pxNetworkBuffer->pxInterface != NULL )
    {
        ( void ) pxNetworkBuffer->pxInterface->pfOutput( pxNetworkBuffer->pxInterface, pxNetworkBuffer, xReleaseAfterSend );
    }
}

static void prvCallDHCP_RA_Handler( NetworkEndPoint_t * pxEndPoint )
{
    BaseType_t xIsIPv6 = pdFALSE;

    #if ( ( ipconfigUSE_DHCP == 1 ) || ( ipconfigUSE_DHCPv6 == 1 ) || ( ipconfigUSE_RA == 1 ) )
        if( pxEndPoint->bits.bIPv6 != pdFALSE_UNSIGNED )
        {
            xIsIPv6 = pdTRUE;
        }
    #endif
    /* The DHCP state machine needs processing. */
    #if ( ipconfigUSE_DHCP == 1 )
    {
        if( ( pxEndPoint->bits.bWantDHCP != pdFALSE_UNSIGNED ) && ( xIsIPv6 == pdFALSE ) )
        {
            /* Process DHCP messages for a given end-point. */
            vDHCPProcess( pdFALSE, pxEndPoint );
        }
    }
    #endif /* ipconfigUSE_DHCP */
    #if ( ( ipconfigUSE_DHCPv6 == 1 ) && ( ipconfigUSE_IPv6 != 0 ) )
    {
        if( ( xIsIPv6 == pdTRUE ) && ( pxEndPoint->bits.bWantDHCP != pdFALSE_UNSIGNED ) )
        {
            /* Process DHCPv6 messages for a given end-point. */
            vDHCPv6Process( pdFALSE, pxEndPoint );
        }
    }
    #endif /* ipconfigUSE_DHCPv6 */
    #if ( ( ipconfigUSE_RA == 1 ) && ( ipconfigUSE_IPv6 != 0 ) )
    {
        if( ( xIsIPv6 == pdTRUE ) && ( pxEndPoint->bits.bWantRA != pdFALSE_UNSIGNED ) )
        {
            /* Process RA messages for a given end-point. */
            vRAProcess( pdFALSE, pxEndPoint );
        }
    }
    #endif /* ( ( ipconfigUSE_RA == 1 ) && ( ipconfigUSE_IPv6 != 0 ) ) */

    /* Mention pxEndPoint and xIsIPv6 in case they have not been used. */
    ( void ) pxEndPoint;
    ( void ) xIsIPv6;
}

static void prvIPTask_CheckPendingEvents( void )
{
    NetworkInterface_t * pxInterface;

    if( xNetworkDownEventPending != pdFALSE )
    {
        /* A network down event could not be posted to the network event
         * queue because the queue was full.
         * As this code runs in the IP-task, it can be done directly by
         * calling prvProcessNetworkDownEvent(). */
        xNetworkDownEventPending = pdFALSE;

        for( pxInterface = FreeRTOS_FirstNetworkInterface();
             pxInterface != NULL;
             pxInterface = FreeRTOS_NextNetworkInterface( pxInterface ) )
        {
            if( pxInterface->bits.bCallDownEvent != pdFALSE_UNSIGNED )
            {
                prvProcessNetworkDownEvent( pxInterface );
                pxInterface->bits.bCallDownEvent = pdFALSE_UNSIGNED;
            }
        }
    }
}

static void prvProcessIPEventsAndTimers( void )
{
    IPStackEvent_t xReceivedEvent;
    TickType_t xNextIPSleep;
    FreeRTOS_Socket_t * pxSocket;
    struct freertos_sockaddr xAddress;

    ipconfigWATCHDOG_TIMER();

    /* Check the ARP, DHCP and TCP timers to see if there is any periodic
     * or timeout processing to perform. */
    vCheckNetworkTimers();

    /* Calculate the acceptable maximum sleep time. */
    xNextIPSleep = xCalculateSleepTime();

    /* Wait until there is something to do. If the following call exits
     * due to a time out rather than a message being received, set a
     * 'NoEvent' value. */
    if( xQueueReceive( xNetworkEventQueue, ( void * ) &xReceivedEvent, xNextIPSleep ) == pdFALSE )
    {
        xReceivedEvent.eEventType = eNoEvent;
    }

    #if ( ipconfigCHECK_IP_QUEUE_SPACE != 0 )
    {
        if( xReceivedEvent.eEventType != eNoEvent )
        {
            UBaseType_t uxCount;

            uxCount = uxQueueSpacesAvailable( xNetworkEventQueue );

            if( uxQueueMinimumSpace > uxCount )
            {
                uxQueueMinimumSpace = uxCount;
            }
        }
    }
    #endif /* ipconfigCHECK_IP_QUEUE_SPACE */

//    iptraceNETWORK_EVENT_RECEIVED( xReceivedEvent.eEventType );

    switch( xReceivedEvent.eEventType )
    {
        case eNetworkDownEvent:
            /* Attempt to establish a connection. */
            prvProcessNetworkDownEvent( ( ( NetworkInterface_t * ) xReceivedEvent.pvData ) );
            break;

        case eNetworkRxEvent:

            /* The network hardware driver has received a new packet.  A
             * pointer to the received buffer is located in the pvData member
             * of the received event structure. */
//        	iptraceNETWORK_EVENT_RECEIVED( xReceivedEvent.eEventType );
            prvHandleEthernetPacket( ( NetworkBufferDescriptor_t * ) xReceivedEvent.pvData );
            break;

        case eNetworkTxEvent:

            /* Send a network packet. The ownership will  be transferred to
             * the driver, which will release it after delivery. */
            prvForwardTxPacket( ( ( NetworkBufferDescriptor_t * ) xReceivedEvent.pvData ), pdTRUE );
            break;

        case eARPTimerEvent:
            /* The ARP timer has expired, process the ARP cache. */
            #if ( ipconfigUSE_IPv4 != 0 )
                vARPAgeCache();
            #endif /* ( ipconfigUSE_IPv4 != 0 ) */

            #if ( ipconfigUSE_IPv6 != 0 )
                vNDAgeCache();
            #endif /* ( ipconfigUSE_IPv6 != 0 ) */

            break;

        case eSocketBindEvent:

            /* FreeRTOS_bind (a user API) wants the IP-task to bind a socket
             * to a port. The port number is communicated in the socket field
             * usLocalPort. vSocketBind() will actually bind the socket and the
             * API will unblock as soon as the eSOCKET_BOUND event is
             * triggered. */
            pxSocket = ( ( FreeRTOS_Socket_t * ) xReceivedEvent.pvData );
            xAddress.sin_len = ( uint8_t ) sizeof( xAddress );

            switch( pxSocket->bits.bIsIPv6 ) /* LCOV_EXCL_BR_LINE */
            {
                #if ( ipconfigUSE_IPv4 != 0 )
                    case pdFALSE_UNSIGNED:
                        xAddress.sin_family = FREERTOS_AF_INET;
                        xAddress.sin_address.ulIP_IPv4 = FreeRTOS_htonl( pxSocket->xLocalAddress.ulIP_IPv4 );
                        /* 'ulLocalAddress' will be set again by vSocketBind(). */
                        pxSocket->xLocalAddress.ulIP_IPv4 = 0;
                        break;
                #endif /* ( ipconfigUSE_IPv4 != 0 ) */

                #if ( ipconfigUSE_IPv6 != 0 )
                    case pdTRUE_UNSIGNED:
                        xAddress.sin_family = FREERTOS_AF_INET6;
                        ( void ) memcpy( xAddress.sin_address.xIP_IPv6.ucBytes, pxSocket->xLocalAddress.xIP_IPv6.ucBytes, sizeof( xAddress.sin_address.xIP_IPv6.ucBytes ) );
                        /* 'ulLocalAddress' will be set again by vSocketBind(). */
                        ( void ) memset( pxSocket->xLocalAddress.xIP_IPv6.ucBytes, 0, sizeof( pxSocket->xLocalAddress.xIP_IPv6.ucBytes ) );
                        break;
                #endif /* ( ipconfigUSE_IPv6 != 0 ) */

                default:
                    /* MISRA 16.4 Compliance */
                    break;
            }

            xAddress.sin_port = FreeRTOS_ntohs( pxSocket->usLocalPort );
            /* 'usLocalPort' will be set again by vSocketBind(). */
            pxSocket->usLocalPort = 0U;
            ( void ) vSocketBind( pxSocket, &xAddress, sizeof( xAddress ), pdFALSE );

            /* Before 'eSocketBindEvent' was sent it was tested that
             * ( xEventGroup != NULL ) so it can be used now to wake up the
             * user. */
            pxSocket->xEventBits |= ( EventBits_t ) eSOCKET_BOUND;
            vSocketWakeUpUser( pxSocket );
            break;

        case eSocketCloseEvent:

            /* The user API FreeRTOS_closesocket() has sent a message to the
             * IP-task to actually close a socket. This is handled in
             * vSocketClose().  As the socket gets closed, there is no way to
             * report back to the API, so the API won't wait for the result */
            ( void ) vSocketClose( ( ( FreeRTOS_Socket_t * ) xReceivedEvent.pvData ) );
            break;

        case eStackTxEvent:

            /* The network stack has generated a packet to send.  A
             * pointer to the generated buffer is located in the pvData
             * member of the received event structure. */
            vProcessGeneratedUDPPacket( ( NetworkBufferDescriptor_t * ) xReceivedEvent.pvData );
            break;

        case eDHCPEvent:
            prvCallDHCP_RA_Handler( ( ( NetworkEndPoint_t * ) xReceivedEvent.pvData ) );
            break;

        case eSocketSelectEvent:

            /* FreeRTOS_select() has got unblocked by a socket event,
             * vSocketSelect() will check which sockets actually have an event
             * and update the socket field xSocketBits. */
            #if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )
            #if ( ipconfigSELECT_USES_NOTIFY != 0 )
                {
                    SocketSelectMessage_t * pxMessage = ( ( SocketSelectMessage_t * ) xReceivedEvent.pvData );
                    vSocketSelect( pxMessage->pxSocketSet );
                    ( void ) xTaskNotifyGive( pxMessage->xTaskhandle );
                }
            #else
                {
                    vSocketSelect( ( ( SocketSelect_t * ) xReceivedEvent.pvData ) );
                }
            #endif /* ( ipconfigSELECT_USES_NOTIFY != 0 ) */
            #endif /* ipconfigSUPPORT_SELECT_FUNCTION == 1 */
            break;

        case eSocketSignalEvent:
            #if ( ipconfigSUPPORT_SIGNALS != 0 )

                /* Some task wants to signal the user of this socket in
                 * order to interrupt a call to recv() or a call to select(). */
                ( void ) FreeRTOS_SignalSocket( ( Socket_t ) xReceivedEvent.pvData );
            #endif /* ipconfigSUPPORT_SIGNALS */
            break;

        case eTCPTimerEvent:
            #if ( ipconfigUSE_TCP == 1 )

                /* Simply mark the TCP timer as expired so it gets processed
                 * the next time prvCheckNetworkTimers() is called. */
                vIPSetTCPTimerExpiredState( pdTRUE );
            #endif /* ipconfigUSE_TCP */
            break;

        case eTCPAcceptEvent:

            /* The API FreeRTOS_accept() was called, the IP-task will now
             * check if the listening socket (communicated in pvData) actually
             * received a new connection. */
            #if ( ipconfigUSE_TCP == 1 )
                pxSocket = ( ( FreeRTOS_Socket_t * ) xReceivedEvent.pvData );

                if( xTCPCheckNewClient( pxSocket ) != pdFALSE )
                {
                    pxSocket->xEventBits |= ( EventBits_t ) eSOCKET_ACCEPT;
                    vSocketWakeUpUser( pxSocket );
                }
            #endif /* ipconfigUSE_TCP */
            break;

        case eTCPNetStat:

            /* FreeRTOS_netstat() was called to have the IP-task print an
             * overview of all sockets and their connections */
            #if ( ( ipconfigUSE_TCP == 1 ) && ( ipconfigHAS_PRINTF == 1 ) )
                vTCPNetStat();
            #endif /* ipconfigUSE_TCP */
            break;

        case eSocketSetDeleteEvent:
            #if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )
            {
                SocketSelect_t * pxSocketSet = ( SocketSelect_t * ) ( xReceivedEvent.pvData );

                iptraceMEM_STATS_DELETE( pxSocketSet );
                vEventGroupDelete( pxSocketSet->xSelectGroup );
                vPortFree( ( void * ) pxSocketSet );
            }
            #endif /* ipconfigSUPPORT_SELECT_FUNCTION == 1 */
            break;

        case eNoEvent:
            /* xQueueReceive() returned because of a normal time-out. */
            break;

        default:
            /* Should not get here. */
            break;
    }

    prvIPTask_CheckPendingEvents();
}

void IPTask( void * pvParameters )
{
    /* Just to prevent compiler warnings about unused parameters. */
    ( void ) pvParameters;

    prvIPTask_Initialise();

    FreeRTOS_debug_printf( ( "prvIPTask started" ) );

    NetworkInterface_t * pxInterface = NS->getFirstInterface(); //FreeRTOS_FirstNetworkInterface();
    NetworkEndPoint_t *endpoint = NS->getFirstEndPoint(pxInterface); //FreeRTOS_FirstEndPoint(pxInterface);

    /* Loop, processing IP events. */
    while( ipFOREVER() == pdTRUE )
    {
    	while(!isNetworkStarted()) {}
        prvProcessIPEventsAndTimers();

        if(endpoint != NULL) {
        	if( endpoint->bits.bEndPointUp & 0x1 ) {
        		SetStatus_IP(IP_OK);
        	}
        }
        else {
        	endpoint = NS->getFirstEndPoint(pxInterface);
        	SetStatus_IP(IP_NOTOK);
        }
    }
}

BaseType_t IPInit_Multi( void )
{
    BaseType_t xReturn = pdFALSE;

    /* There must be at least one interface and one end-point. */
    configASSERT( FreeRTOS_FirstNetworkInterface() != NULL );

    /* Check that the configuration values are correct and that the IP-task has not
     * already been initialized. */
    vPreCheckConfigs();

    /* Attempt to create the queue used to communicate with the IP task. */
    #if ( configSUPPORT_STATIC_ALLOCATION == 1 )
    {
        static StaticQueue_t xNetworkEventStaticQueue;
        static uint8_t ucNetworkEventQueueStorageArea[ ipconfigEVENT_QUEUE_LENGTH * sizeof( IPStackEvent_t ) ];
        xNetworkEventQueue = xQueueCreateStatic( ipconfigEVENT_QUEUE_LENGTH,
                                                 sizeof( IPStackEvent_t ),
                                                 ucNetworkEventQueueStorageArea,
                                                 &xNetworkEventStaticQueue );
    }
    #else
    {
        xNetworkEventQueue = xQueueCreate( ipconfigEVENT_QUEUE_LENGTH, sizeof( IPStackEvent_t ) );
        configASSERT( xNetworkEventQueue != NULL );
    }
    #endif /* configSUPPORT_STATIC_ALLOCATION */

    if( xNetworkEventQueue != NULL )
    {
        #if ( configQUEUE_REGISTRY_SIZE > 0 )
        {
            /* A queue registry is normally used to assist a kernel aware
             * debugger.  If one is in use then it will be helpful for the debugger
             * to show information about the network event queue. */
            vQueueAddToRegistry( xNetworkEventQueue, "NetEvnt" );
        }
        #endif /* configQUEUE_REGISTRY_SIZE */

        if( xNetworkBuffersInitialise() == pdPASS )
        {
            /* Prepare the sockets interface. */
            vNetworkSocketsInit();

            /* Create the task that processes Ethernet and stack events. */
            #if ( configSUPPORT_STATIC_ALLOCATION == 1 )
            {
                //MP CUBEMX GEN CODE FROM FREERTOS TASKS
            	//MP not starting thread from native code but from cubemx integrated task/queue config
            	//MP COMMENT REST OF SCOPE and xReturn = pdTRUE
            	//xReturn = pdTRUE;
                static StaticTask_t xIPTaskBuffer;
                static StackType_t xIPTaskStack[ ipconfigIP_TASK_STACK_SIZE_WORDS ];

                xIPTaskHandle = xTaskCreateStatic( IPTask,
                                                   "IP-Task",
                                                   ipconfigIP_TASK_STACK_SIZE_WORDS,
                                                   NULL,
                                                   24, /*ipconfigIP_TASK_PRIORITY,*/
                                                   xIPTaskStack,
                                                   &xIPTaskBuffer );

                if( xIPTaskHandle != NULL )
                {
                    xReturn = pdTRUE;
                }

            }
            #else /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
            {
            	//MP SAME HERE IN THAT CONTEXT
            	//            	xReturn = pdTRUE;
            	xReturn = xTaskCreate( IPTask,
                                       "IP-task",
                                       ipconfigIP_TASK_STACK_SIZE_WORDS,
                                       NULL,
                                       ipconfigIP_TASK_PRIORITY,
                                       &( xIPTaskHandle ) );

            }
            #endif /* configSUPPORT_STATIC_ALLOCATION */
        }
        else
        {
            FreeRTOS_debug_printf( ( "FreeRTOS_IPInit_Multi: xNetworkBuffersInitialise() failed" ) );

            /* Clean up. */
            vQueueDelete( xNetworkEventQueue );
            xNetworkEventQueue = NULL;
        }
    }
    else
    {
        FreeRTOS_debug_printf( ( "FreeRTOS_IPInit_Multi: Network event queue could not be created" ) );
    }

    return xReturn;
}

void NetServices::init() {
/*
//	xMacInitStatus = eMACInit;
//	xTXDescriptorSemaphore = NULL;
//	pxMyInterface = NULL;
//
//	xInterfaces = (NetworkInterface_t*)pvPortMalloc(1*sizeof(NetworkInterface_t));
//	xEndPoints = (NetworkEndPoint_t*)pvPortMalloc(4*sizeof(NetworkEndPoint_t));
//
 *
 */

	/* Initialize the interface descriptor. */
	pxSTM32H_FillInterfaceDescriptor(0, &(xInterfaces[0]));

	/* === End-point 0 === */
	#if ( ipconfigUSE_IPv4 != 0 )
	    {
	        FreeRTOS_FillEndPoint(&(xInterfaces[0]), &(xEndPoints[xEndPointCount]), ucIPAddress, ucNetMask, ucGatewayAddress, ucDNSServerAddress, ucMACAddress);
	        #if ( ipconfigUSE_DHCP != 0 )
	        {
	            /* End-point 0 wants to use DHCPv4. */
	            xEndPoints[xEndPointCount].bits.bWantDHCP = pdTRUE;
	        }
	        #endif /* ( ipconfigUSE_DHCP != 0 ) */

	        xEndPointCount += 1;
	    }
	#endif /* ( ipconfigUSE_IPv4 != 0 */

	/* Initialise the TCP/IP stack. */
	start = IPInit_Multi() == pdTRUE ? true:false;

	if(start == true) {
		SetStatus_ETH(NET_OK);
//		SetStatus_IP(IP_OK);
	}

	snprintf(log, LOG_LENGTH, "Initialization completed");
	DS->pushToLogsMon(name, LOG_OK, log);

//	showEndPoint();
}

bool NetServices::IsConnected() {
	bool retour = false;

//	retour = (connected && statusETH == NET_OK) ? true : false;
	retour = (connected && mpappGetStatus_ETH() == NET_OK) ? true : false;

	return(retour);
}

int NetServices::Connect() {
	int retour = 0;

	snprintf(log,LOG_LENGTH, "connecting to services...");
	DS->pushToLogsMon(name, LOG_INFO, log);

//	statusCLOUD = SSL_Client();
//	statusCLOUD = SSL_Client_Secure();

	if(statusCLOUD == CLOUD_OK) {
		snprintf(log,LOG_LENGTH, "connected to services");
		DS->pushToLogsMon(name, LOG_OK, log);
		connected = true;
	}
	else {
		snprintf(log,LOG_LENGTH, "NOT connected to services");
		DS->pushToLogsMon(name, LOG_ERROR, log);
		connected = false;
	}

	SetStatus_CLOUD(statusCLOUD);

	return(retour);
}

int NetServices::Disconnect() {
	int retour = 0;

	connected = false;
	SetStatus_CLOUD(CLOUD_NOTOK);

	snprintf(log, LOG_LENGTH, "disconnected from services, status cloud is %d", CLOUD_NOTOK);
	DS->pushToLogsMon(name, LOG_INFO, log);

	return(retour);
}

char* NetServices::GetErrorDescription(int code) {
	char *description = (char*)pvPortMalloc(150 * sizeof(char));

	switch(code) {
	case -0x7080:
		sprintf(description, "The requested feature is not available.");
		break;
	case -0x7100:
		sprintf(description, "Bad input parameters to function.");
		break;
	case -0x7180:
		sprintf(description, "Verification of the message MAC failed.");
		break;
	case -0x7200:
		sprintf(description, "An invalid SSL record was received.");
		break;
	case -0x7280:
		sprintf(description, "The connection indicated an EOF.");
		break;
	case -0x7300:
		sprintf(description, "An unknown cipher was received.");
		break;
	case -0x7380:
		sprintf(description, "The server has no ciphersuites in common with the client.");
		break;
	case -0x7400:
		sprintf(description, "No RNG was provided to the SSL module.");
		break;
	case -0x7480:
		sprintf(description, "No client certification received from the client, but required by the authentication mode.");
		break;
	case -0x7500:
		sprintf(description, "Our own certificate(s) is/are too large to send in an SSL message.");
		break;
	case -0x7580:
		sprintf(description, "The own certificate is not set, but needed by the server.");
		break;
	case -0x7600:
		sprintf(description, "The own private key or pre-shared key is not set, but needed.");
		break;
	case -0x7680:
		sprintf(description, "No CA Chain is set, but required to operate.");
		break;
	case -0x7700:
		sprintf(description, "An unexpected message was received from our peer.");
		break;
	case -0x7780:
		sprintf(description, "A fatal alert message was received from our peer.");
		break;
	case -0x7800:
		sprintf(description, "Verification of our peer failed.");
		break;
	case -0x7880:
		sprintf(description, "The peer notified us that the connection is going to be closed.");
		break;
	case -0x7900:
		sprintf(description, "Processing of the ClientHello handshake message failed.");
		break;
	case -0x7980:
		sprintf(description, "Processing of the ServerHello handshake message failed.");
		break;
	case -0x7A00:
		sprintf(description, "Processing of the Certificate handshake message failed.");
		break;
	case -0x7A80:
		sprintf(description, "Processing of the CertificateRequest handshake message failed.");
		break;
	case -0x7B00:
		sprintf(description, "Processing of the ServerKeyExchange handshake message failed.");
		break;
	case -0x7B80:
		sprintf(description, "Processing of the ServerHelloDone handshake message failed.");
		break;
	case -0x7C00:
		sprintf(description, "Processing of the ClientKeyExchange handshake message failed.");
		break;
	case -0x7C80:
		sprintf(description, "Processing of the ClientKeyExchange handshake message failed in DHM / ECDH Read Public.");
		break;
	case -0x7D00:
		sprintf(description, "Processing of the ClientKeyExchange handshake message failed in DHM / ECDH Calculate Secret.");
		break;
	case -0x7D80:
		sprintf(description, "Processing of the CertificateVerify handshake message failed.");
		break;
	case -0x7E00:
		sprintf(description, "Processing of the ChangeCipherSpec handshake message failed.");
		break;
	case -0x7E80:
		sprintf(description, "Processing of the Finished handshake message failed.");
		break;
	case -0x7F00:
		sprintf(description, "Memory allocation failed");
		break;
	case -0x7FF4:
		sprintf(description, "no available description, dig in my friend :-)");
		break;
	case -0x7F80:
		sprintf(description, "Hardware acceleration function returned with error");
		break;
	case -0x6F80:
		sprintf(description, "Hardware acceleration function skipped / left alone data");
		break;
	case -0x6F00:
		sprintf(description, "Processing of the compression / decompression failed");
		break;
	case -0x6E80:
		sprintf(description, "Handshake protocol not within min/max boundaries");
		break;
	case -0x6E00:
		sprintf(description, "Processing of the NewSessionTicket handshake message failed.");
		break;
	case -0x6D80:
		sprintf(description, "Session ticket has expired.");
		break;
	case -0x6D00:
		sprintf(description, "Public key type mismatch (eg, asked for RSA key exchange and presented EC key)");
		break;
	case -0x6C80:
		sprintf(description, "Unknown identity received (eg, PSK identity)");
		break;
	case -0x6C00:
		sprintf(description, "Internal error (eg, unexpected failure in lower-level module)");
		break;
	case -0x6B80:
		sprintf(description, "A counter would wrap (eg, too many messages exchanged).");
		break;
	case -0x6B00:
		sprintf(description, "Unexpected message at ServerHello in renegotiation.");
		break;
	case -0x6A80:
		sprintf(description, "DTLS client must retry for hello verification");
		break;
	case -0x6A00:
		sprintf(description, "A buffer is too small to receive or write a message");
		break;
	case -0x6980:
		sprintf(description, "None of the common ciphersuites is usable (eg, no suitable certificate, see debug messages).");
		break;
	case -0x6900:
		sprintf(description, "No data of requested type currently available on underlying transport.");
		break;
	case -0x6880:
		sprintf(description, "Connection requires a write call.");
		break;
	case -0x6800:
		sprintf(description, "The operation timed out.");
		break;
	case -0x6780:
		sprintf(description, "The client initiated a reconnect from the same port.");
		break;
	case -0x6700:
		sprintf(description, "Record header looks valid but is not expected.");
		break;
	case -0x6680:
		sprintf(description, "The alert message received indicates a non-fatal error.");
		break;
	case -0x6600:
		sprintf(description, "Couldn't set the hash for verifying CertificateVerify");
		break;
	case -0x6580:
		sprintf(description, "Internal-only message signaling that further message-processing should be done");
		break;
	case -0x6500:
		sprintf(description, "The asynchronous operation is not completed yet.");
		break;
	case -0x6480:
		sprintf(description, "Internal-only message signaling that a message arrived early.");
		break;
	case -0x7000:
		sprintf(description, "A cryptographic operation is in progress. Try again later.");
		break;
	case 0:
		sprintf(description, "OK");
		break;
	default:
		sprintf(description, "no description documented");
		break;
	}

	return(description);
}

//===================================================================================================
//
//===================================================================================================
/* Reverse the bits of a 32 bit unsigned integer */
uint32_t NetServices::prvRevBits32( uint32_t ulValue )
{
    uint32_t ulRev32;
    int iIndex;

    ulRev32 = 0;

    for( iIndex = 0; iIndex < 32; iIndex++ )
    {
        if( ulValue & ( 1 << iIndex ) )
        {
            {
                ulRev32 |= 1 << ( 31 - iIndex );
            }
        }
    }

    return ulRev32;
}

/* Compute the CRC32 of the given MAC address as per IEEE 802.3 CRC32 */
uint32_t NetServices::prvComputeCRC32_MAC( const uint8_t * pucMAC )
{
    int iiIndex, ijIndex;
    uint32_t ulCRC32 = 0xFFFFFFFF;

    for( ijIndex = 0; ijIndex < 6; ijIndex++ )
    {
        ulCRC32 = ulCRC32 ^ ( uint32_t ) pucMAC[ ijIndex ];

        for( iiIndex = 0; iiIndex < 8; iiIndex++ )
        {
            if( ulCRC32 & 1 )
            {
                ulCRC32 = ( ulCRC32 >> 1 ) ^ prvRevBits32( 0x04C11DB7 ); /* IEEE 802.3 CRC32 polynomial - 0x04C11DB7 */
            }
            else
            {
                ulCRC32 = ( ulCRC32 >> 1 );
            }
        }
    }

    ulCRC32 = ~( ulCRC32 );
    return ulCRC32;
}

/* Compute the hash value of a given MAC address to index the bits in the Hash Table
 * Registers (ETH_MACHT0R and ETH_MACHT1R) */
uint32_t NetServices::prvComputeEthernet_MACHash( const uint8_t * pucMAC )
{
    uint32_t ulCRC32;
    uint32_t ulHash;

    /*  Calculate the 32-bit CRC for the MAC */
    ulCRC32 = prvComputeCRC32_MAC( pucMAC );

    /* Perform bitwise reversal on the CRC32 */
    ulHash = prvRevBits32( ulCRC32 );

    /* Take the upper 6 bits of the above result */
    return( ulHash >> 26 );
}

/* Update the Hash Table Registers
 * (ETH_MACHT0R and ETH_MACHT1R) with hash value of the given MAC address */
void NetServices::prvSetMAC_HashFilter( ETH_HandleTypeDef * pheth,
                                  const uint8_t * pucMAC )
{
    uint32_t ulHash;

    /* Compute the hash */
    ulHash = prvComputeEthernet_MACHash( pucMAC );

    /* Use the upper (MACHT1R) or lower (MACHT0R) Hash Table Registers
     * to set the required bit based on the ulHash */
    if( ulHash < 32 )
    {
        pheth->Instance->MACHT0R |= ( 1 << ulHash );
    }
    else
    {
        pheth->Instance->MACHT1R |= ( 1 << ( ulHash % 32 ) );
    }
}

//===================================================================================================
// Network Interface API Functions
//===================================================================================================
BaseType_t NetServices::xSTM32H_NetworkInterfaceInitialise( NetworkInterface_t * pxInterface )
{
    BaseType_t xResult = pdFAIL;
    NetworkEndPoint_t * pxEndPoint;
    HAL_StatusTypeDef xHalEthInitStatus;
    size_t uxIndex = 0;

    if( xMacInitStatus == eMACInit )
    {
        pxMyInterface = pxInterface;

        pxEndPoint = FreeRTOS_FirstEndPoint( pxInterface );
        configASSERT( pxEndPoint != NULL );

        /*
         * Initialize ETH Handler
         * It assumes that Ethernet GPIO and clock configuration
         * are already done in the ETH_MspInit()
         */
        heth.Instance = ETH;
        heth.Init.MACAddr = ( uint8_t * ) pxEndPoint->xMACAddress.ucBytes;
        heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
        heth.Init.TxDesc = DMATxDscrTab;
        heth.Init.RxDesc = DMARxDscrTab;
        heth.Init.RxBuffLen = ( ETH_RX_BUF_SIZE - ipBUFFER_PADDING ) & ~( ( uint32_t ) 3U );

        /* Make sure that all unused fields are cleared. */
        memset( &( DMATxDscrTab ), '\0', sizeof( DMATxDscrTab ) );
        memset( &( DMARxDscrTab ), '\0', sizeof( DMARxDscrTab ) );

        xHalEthInitStatus = HAL_ETH_Init( &( heth ) );

        if( xHalEthInitStatus == HAL_OK )
        {
            /* Update MAC filter settings */
            heth.Instance->MACPFR |= ENABLE_HASH_FILTER_SETTINGS;

            /* Configuration for HAL_ETH_Transmit(_IT). */
            memset( &( xTxConfig ), 0, sizeof( ETH_TxPacketConfig ) );
            xTxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CRCPAD;

            #if ( ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM != 0 )
            {
                /*xTxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC; */
                xTxConfig.Attributes |= ETH_TX_PACKETS_FEATURES_CSUM;
                xTxConfig.ChecksumCtrl = ETH_DMATXNDESCRF_CIC_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
            }
            #else
            {
                xTxConfig.ChecksumCtrl = ETH_CHECKSUM_DISABLE;
            }
            #endif
            xTxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;

            /* This counting semaphore will count the number of free TX DMA descriptors. */
            xTXDescriptorSemaphore = xSemaphoreCreateCounting( ( UBaseType_t ) ETH_TX_DESC_CNT, ( UBaseType_t ) ETH_TX_DESC_CNT );
            configASSERT( xTXDescriptorSemaphore );

            xTransmissionMutex = xSemaphoreCreateMutex();
            configASSERT( xTransmissionMutex );

            /* Assign Rx memory buffers to a DMA Rx descriptor */
            for( uxIndex = 0; uxIndex < ETH_RX_DESC_CNT; uxIndex++ )
            {
                uint8_t * pucBuffer;

                #if ( ipconfigZERO_COPY_RX_DRIVER != 0 )
                {
                    pucBuffer = pucGetRXBuffer( ETH_RX_BUF_SIZE );
                    configASSERT( pucBuffer != NULL );
                }
                #else
                {
                    pucBuffer = Rx_Buff[ uxIndex ];
                }
                #endif

                HAL_ETH_DescAssignMemory( &( heth ), uxIndex, pucBuffer, NULL );
            }

            #if ( ipconfigUSE_MDNS == 1 )
            {
                /* Program the MDNS address. */
                prvSetMAC_HashFilter( &heth, ( uint8_t * ) xMDNS_MacAddress.ucBytes );
            }
            #endif
            #if ( ( ipconfigUSE_MDNS == 1 ) && ( ipconfigUSE_IPv6 != 0 ) )
            {
                prvSetMAC_HashFilter( &heth, ( uint8_t * ) xMDNS_MacAddressIPv6.ucBytes );
            }
            #endif
            #if ( ipconfigUSE_LLMNR == 1 )
            {
                /* Program the LLMNR address. */
                prvSetMAC_HashFilter( &heth, ( uint8_t * ) xLLMNR_MacAddress.ucBytes );
            }
            #endif
            #if ( ( ipconfigUSE_LLMNR == 1 ) && ( ipconfigUSE_IPv6 != 0 ) )
            {
                prvSetMAC_HashFilter( &heth, ( uint8_t * ) xLLMNR_MacAddressIPv6.ucBytes );
            }
            #endif

            {
                /* The EMAC address of the first end-point has been registered in HAL_ETH_Init(). */
                for( ;
                     pxEndPoint != NULL;
                     pxEndPoint = FreeRTOS_NextEndPoint( pxMyInterface, pxEndPoint ) )
                {
                    switch( pxEndPoint->bits.bIPv6 )
                    {
                        #if ( ipconfigUSE_IPv4 != 0 )
                            case pdFALSE_UNSIGNED:

                                if( heth.Init.MACAddr != ( uint8_t * ) pxEndPoint->xMACAddress.ucBytes )
                                {
                                    prvSetMAC_HashFilter( &heth, pxEndPoint->xMACAddress.ucBytes );
                                }
                                break;
                        #endif /* ( ipconfigUSE_IPv4 != 0 ) */

                        #if ( ipconfigUSE_IPv6 != 0 )
                            case pdTRUE_UNSIGNED:
                               {
                                   uint8_t ucMACAddress[ 6 ] = { 0x33, 0x33, 0xff, 0, 0, 0 };

                                   ucMACAddress[ 3 ] = pxEndPoint->ipv6_settings.xIPAddress.ucBytes[ 13 ];
                                   ucMACAddress[ 4 ] = pxEndPoint->ipv6_settings.xIPAddress.ucBytes[ 14 ];
                                   ucMACAddress[ 5 ] = pxEndPoint->ipv6_settings.xIPAddress.ucBytes[ 15 ];

                                   /* Allow traffic destined to Solicited-Node multicast address of this endpoint
                                    * for Duplicate Address Detection (DAD) */
                                   prvSetMAC_HashFilter( &heth, ucMACAddress );
                               }
                               break;
                        #endif /* ( ipconfigUSE_IPv6 != 0 ) */

                        default:
                            /* MISRA 16.4 Compliance */
                            break;
                    }
                }
            }

            #if ( ipconfigUSE_IPv6 != 0 )
            {
                /* Allow traffic destined to IPv6 all nodes multicast MAC 33:33:00:00:00:01 */
                const uint8_t ucMACAddress[ 6 ] = { 0x33, 0x33, 0, 0, 0, 0x01 };
                prvSetMAC_HashFilter( &heth, ucMACAddress );
            }
            #endif /* ( ipconfigUSE_IPv6 != 0 ) */

            /* Initialize the MACB and set all PHY properties */
            NS->prvMACBProbePhy();

            /* Force a negotiation with the Switch or Router and wait for LS. */
            NS->prvEthernetUpdateConfig( pdTRUE );

            /* The deferred interrupt handler task is created at the highest
             *  possible priority to ensure the interrupt handler can return directly
             *  to it.  The task's handle is stored in xEMACTaskHandle so interrupts can
             *  notify the task when there is something to process. */
          if( xTaskCreate( prvEMACHandlerTask, niEMAC_HANDLER_TASK_NAME, niEMAC_HANDLER_TASK_STACK_SIZE, NULL, niEMAC_HANDLER_TASK_PRIORITY, &( xEMACTaskHandle ) ) == pdPASS )
            if( xEMACTaskHandle != NULL )
            {
                /* The task was created successfully. */
                xMacInitStatus = eMACPass;
            }
            else
            {
                xMacInitStatus = eMACFailed;
            }
        }
        else
        {
            /* HAL_ETH_Init() returned an error, the driver gets into a fatal error sate. */
            xMacInitStatus = eMACFailed;
        }
    } /* ( xMacInitStatus == eMACInit ) */

    if( xMacInitStatus == eMACPass )
    {
        if( xPhyObject.ulLinkStatusMask != 0U )
        {
            xResult = pdPASS;
            FreeRTOS_printf( ( "Link Status is high" ) );
        }
        else
        {
            /* For now pdFAIL will be returned. But prvEMACHandlerTask() is running
             * and it will keep on checking the PHY and set 'ulLinkStatusMask' when necessary. */
        }
    }

//	showEndPoint(pxMyInterface);
//	traceETHHandler(&heth);

    return xResult;
}

BaseType_t NetServices::xSTM32H_NetworkInterfaceOutput( NetworkInterface_t * pxInterface,
                                                  NetworkBufferDescriptor_t * const pxBuffer,
                                                  BaseType_t xReleaseAfterSend )
{
    BaseType_t xResult = pdFAIL;
    TickType_t xBlockTimeTicks = pdMS_TO_TICKS( 100U );
    uint8_t * pucTXBuffer;

    if( xSTM32H_GetPhyLinkStatus( pxInterface ) == pdPASS )
    {
        #if ( ipconfigZERO_COPY_TX_DRIVER != 0 )
            /* Zero-copy method, pass the buffer. */
            pucTXBuffer = pxBuffer->pucEthernetBuffer;

            /* As the buffer is passed to the driver, it must exist.
             * The library takes care of this. */
            configASSERT( xReleaseAfterSend != pdFALSE );
        #else
            pucTXBuffer = Tx_Buff[ heth.TxDescList.CurTxDesc ];
            /* The copy method, left here for educational purposes. */
            configASSERT( pxBuffer->xDataLength <= sizeof( Tx_Buff[ 0 ] ) );
        #endif

        ETH_BufferTypeDef xTransmitBuffer =
        {
            .buffer = pucTXBuffer,
            .len    = pxBuffer->xDataLength,
            .next   = NULL /* FreeRTOS+TCP does not use linked buffers. */
        };
        /* This is the total length, which is equal to the buffer. */
        xTxConfig.Length = pxBuffer->xDataLength;
        xTxConfig.TxBuffer = &( xTransmitBuffer );

        /* This counting semaphore counts the number of free TX DMA descriptors. */
        if( xSemaphoreTake( xTXDescriptorSemaphore, xBlockTimeTicks ) != pdPASS )
        {
            /* If the logging routine is using the network, the following message
             * may cause a new error message. */
            FreeRTOS_printf( ( "emacps_send_message: Time-out waiting for TX buffer" ) );
        }
        else
        {
            /* Memory barrier: Make sure that the data written to the packet buffer got written. */
            __DSB();

            /* Get exclusive access to the TX process.
             * Both the IP-task and the EMAC task will work on the TX process. */
            if( xSemaphoreTake( xTransmissionMutex, xBlockTimeTicks ) != pdFAIL )
            {
                #if ( ipconfigZERO_COPY_TX_DRIVER != 0 )
                {
                    /* Do not release the buffer. */
                    xReleaseAfterSend = pdFALSE;
                }
                #else
                {
                    memcpy( pucTXBuffer, pxBuffer->pucEthernetBuffer, pxBuffer->xDataLength );

                    /* A memory barrier to make sure that the outgoing packets has been written
                     * to the physical memory. */
                    __DSB();
                }
                #endif /* if ( ipconfigZERO_COPY_TX_DRIVER != 0 ) */

                if( HAL_ETH_Transmit_IT( &( heth ), &( xTxConfig ) ) == HAL_OK )
                {
                    xResult = pdPASS;
                }
                else
                {
                    /* As the transmission packet was not queued,
                     * the counting semaphore should be given. */
                    xSemaphoreGive( xTXDescriptorSemaphore );
                }

                /* And release the mutex. */
                xSemaphoreGive( xTransmissionMutex );
            }

            /* Call the standard trace macro to log the send event. */
            iptraceNETWORK_INTERFACE_TRANSMIT();
        }
    }

    if( xReleaseAfterSend != pdFALSE )
    {
        vReleaseNetworkBufferAndDescriptor( pxBuffer );
    }

    return xResult;
}

NetworkInterface_t * NetServices::pxSTM32H_FillInterfaceDescriptor( BaseType_t xEMACIndex,
                                                       NetworkInterface_t * pxInterface )
{
//    static char pcName[ 17 ];

/* This function pxSTM32Hxx_FillInterfaceDescriptor() adds a network-interface.
 * Make sure that the object pointed to by 'pxInterface'
 * is declared static or global, and that it will remain to exist. */

	char *pcName = (char*)pvPortMalloc((17 * sizeof(char)));
    snprintf( pcName, 17, "eth%u", ( unsigned ) xEMACIndex );

    memset( pxInterface, '\0', sizeof( *pxInterface ) );
    pxInterface->pcName = pcName;                    /* Just for logging, debugging. */
    pxInterface->pvArgument = ( void * ) xEMACIndex; /* Has only meaning for the driver functions. */
    pxInterface->pfInitialise = xSTM32H_NetworkInterfaceInitialise;
    pxInterface->pfOutput = xSTM32H_NetworkInterfaceOutput;
    pxInterface->pfGetPhyLinkStatus = xSTM32H_GetPhyLinkStatus;

    FreeRTOS_AddNetworkInterface( pxInterface );

    return pxInterface;
}

#if ( ipconfigIPv4_BACKWARD_COMPATIBLE != 0 )

/* Do not call the following function directly. It is there for downward compatibility.
 * The function FreeRTOS_IPInit() will call it to initialice the interface and end-point
 * objects.  See the description in FreeRTOS_Routing.h. */
    NetworkInterface_t * NetServices::pxFillInterfaceDescriptor( BaseType_t xEMACIndex,
                                                    NetworkInterface_t * pxInterface )
    {
        return pxSTM32H_FillInterfaceDescriptor( xEMACIndex, pxInterface );
    }

#endif

BaseType_t NetServices::xSTM32H_GetPhyLinkStatus( NetworkInterface_t * pxInterface )
{
    BaseType_t xReturn;

    if( xPhyObject.ulLinkStatusMask != 0U )
    {
        xReturn = pdPASS;
    }
    else
    {
        xReturn = pdFAIL;
    }

    return xReturn;
}

uint8_t * NetServices::pucGetRXBuffer( size_t uxSize )
{
    TickType_t uxBlockTimeTicks = ipMS_TO_MIN_TICKS( 10U );
    NetworkBufferDescriptor_t * pxBufferDescriptor;
    uint8_t * pucReturn = NULL;

    pxBufferDescriptor = pxGetNetworkBufferWithDescriptor( uxSize, uxBlockTimeTicks );

    if( pxBufferDescriptor != NULL )
    {
        pucReturn = pxBufferDescriptor->pucEthernetBuffer;
    }

    return pucReturn;
}

//===================================================================================================
// Network Interface Static Functions
//===================================================================================================
void NetServices::prvMACBProbePhy( void )
{
    /* Bind the write and read access functions. */
    vPhyInitialise( &( xPhyObject ),
                    ( xApplicationPhyReadHook_t ) ETH_PHY_IO_ReadReg,
                    ( xApplicationPhyWriteHook_t ) ETH_PHY_IO_WriteReg );
    /* Poll the bus for all connected PHY's. */
    xPhyDiscover( &( xPhyObject ) );
    /* Configure them using the properties provided. */
    xPhyConfigure( &( xPhyObject ), &( xPHYProperties ) );
}
/*-----------------------------------------------------------*/

void NetServices::prvEthernetUpdateConfig( BaseType_t xForce )
{
    ETH_MACConfigTypeDef MACConf;
    uint32_t speed = 0, duplex = 0;

    FreeRTOS_printf( ( "prvEthernetUpdateConfig: LS mask %02lX Force %d",
                       xPhyObject.ulLinkStatusMask,
                       ( int ) xForce ) );

    if( ( xForce != pdFALSE ) || ( xPhyObject.ulLinkStatusMask != 0 ) )
    {
        /* Restart the auto-negotiation. */
        xPhyStartAutoNegotiation( &xPhyObject, xPhyGetMask( &( xPhyObject ) ) );

        /* Configure the MAC with the Duplex Mode fixed by the
         * auto-negotiation process. */
        if( xPhyObject.xPhyProperties.ucDuplex == PHY_DUPLEX_FULL )
        {
            duplex = ETH_FULLDUPLEX_MODE;
        }
        else
        {
            duplex = ETH_HALFDUPLEX_MODE;
        }

        /* Configure the MAC with the speed fixed by the
         * auto-negotiation process. */
        if( xPhyObject.xPhyProperties.ucSpeed == PHY_SPEED_10 )
        {
            speed = ETH_SPEED_10M;
        }
        else
        {
            speed = ETH_SPEED_100M;
        }

        /* Get MAC and configure it */
        HAL_ETH_GetMACConfig( &( heth ), &( MACConf ) );
        MACConf.DuplexMode = duplex;
        MACConf.Speed = speed;
        HAL_ETH_SetMACConfig( &( heth ), &( MACConf ) );
        #if ( ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM != 0 )
        {
            MACConf.ChecksumOffload = ENABLE;
        }
        #else
        {
            MACConf.ChecksumOffload = DISABLE;
        }
        #endif /* ( ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM != 0 ) */

        /* Restart MAC interface */
        HAL_ETH_Start_IT( &( heth ) );
    }
    else
    {
        /* Stop MAC interface */
        HAL_ETH_Stop_IT( &( heth ) );
    }
}
/*-----------------------------------------------------------*/

BaseType_t NetServices::prvNetworkInterfaceInput( void )
{
    BaseType_t xReturn = 0;

    /* For as long as a packet is immediately available. */
    for( ; ; )
    {
        NetworkBufferDescriptor_t * pxBufferDescriptor;
        NetworkBufferDescriptor_t * pxReceivedBuffer = NULL;
        ETH_BufferTypeDef data_buffer;
        size_t uxDataLength;
        size_t uxLength;

        uxDataLength = HAL_ETH_GetRxData( &( heth ), &( data_buffer ) );

        if( uxDataLength == 0U )
        {
            /* No more packets received. */
            break;
        }

        xReturn++;

        #if ( ipconfigZERO_COPY_RX_DRIVER != 0 )
        {
            /* Reserve the maximum length for the next reception. */
            uxLength = ETH_RX_BUF_SIZE;

            if( data_buffer.buffer != NULL )
            {
                pxReceivedBuffer = pxPacketBuffer_to_NetworkBuffer( data_buffer.buffer );
                #if ( ipconfigTCP_IP_SANITY != 0 )
                {
                    configASSERT( bIsValidNetworkDescriptor( pxReceivedBuffer ) != 0 );
                }
                #endif
            }

            if( pxReceivedBuffer == NULL )
            {
                FreeRTOS_printf( ( "Strange: no descriptor received" ) );
            }
        }
        #else /* if ( ipconfigZERO_COPY_RX_DRIVER != 0 ) */
        {
            /* Reserve the length of the packet that was just received. */
            uxLength = uxDataLength;
        }
        #endif /* if ( ipconfigZERO_COPY_RX_DRIVER != 0 ) */

        pxBufferDescriptor = pxGetNetworkBufferWithDescriptor( uxLength, 0u );

        if( pxBufferDescriptor == NULL )
        {
            /* The event was lost because a network buffer was not available.
             * Call the standard trace macro to log the occurrence. */
            iptraceETHERNET_RX_EVENT_LOST();
        }

        #if ( ipconfigZERO_COPY_RX_DRIVER != 0 )
        {
            if( pxBufferDescriptor == NULL )
            {
                /* Can not receive this packet. Buffer will be re-used. */
                pxReceivedBuffer = NULL;
            }
            else if( pxReceivedBuffer != NULL )
            {
                pxReceivedBuffer->xDataLength = uxDataLength;
            }
            else
            {
                /* Allocating a new buffer failed. */
            }
        }
        #else /* if ( ipconfigZERO_COPY_RX_DRIVER != 0 ) */
        {
            if( pxBufferDescriptor != NULL )
            {
                pxReceivedBuffer = pxBufferDescriptor;
                /* The copy method. */
                memcpy( pxReceivedBuffer->pucEthernetBuffer, data_buffer.buffer, uxDataLength );
                pxReceivedBuffer->xDataLength = uxDataLength;
                /* Make sure that the descriptor isn't used any more. */
                pxBufferDescriptor = NULL;
            }
        }
        #endif /* if ( ipconfigZERO_COPY_RX_DRIVER != 0 ) */

        {
            uint8_t * pucBuffer = NULL;

            if( pxBufferDescriptor != NULL )
            {
                pucBuffer = pxBufferDescriptor->pucEthernetBuffer;
            }

            /* Assign an RX buffer to the descriptor, so that
             * a next packet can be received. */
            HAL_ETH_BuildRxDescriptors( &( heth ), pucBuffer );
        }

        /* See if the data contained in the received Ethernet frame needs
        * to be processed.  NOTE! It is preferable to do this in
        * the interrupt service routine itself, which would remove the need
        * to unblock this task for packets that don't need processing. */

        if( pxReceivedBuffer != NULL )
        {
            BaseType_t xDoRelease = pdFALSE;

            if( eConsiderFrameForProcessing( pxReceivedBuffer->pucEthernetBuffer ) != eProcessBuffer )
            {
                /* The Ethernet frame can be dropped, but the Ethernet buffer must be released. */
                xDoRelease = pdTRUE;
            }
            else
            {
                /* The event about to be sent to the TCP/IP is an Rx event.
                 * pvData is used to point to the network buffer descriptor that
                 * now references the received data. */

                IPStackEvent_t xRxEvent =
                {
                    .eEventType = eNetworkRxEvent,
                    .pvData     = ( void * ) pxReceivedBuffer
                };

                pxReceivedBuffer->pxInterface = pxMyInterface;
                pxReceivedBuffer->pxEndPoint = FreeRTOS_MatchingEndpoint( pxMyInterface, pxReceivedBuffer->pucEthernetBuffer );

                /* Send the data to the TCP/IP stack. */
                if( xSendEventStructToIPTask( &( xRxEvent ), 0 ) != pdFALSE )
                {
                    /* The message was successfully sent to the TCP/IP stack.
                    * Call the standard trace macro to log the occurrence. */
//                    iptraceNETWORK_INTERFACE_RECEIVE();
                }
                else
                {
                    xDoRelease = pdTRUE;

                    /* The buffer could not be sent to the IP task so the buffer
                     * must be released. */

                    /* Make a call to the standard trace macro to log the
                     * occurrence. */
                    iptraceETHERNET_RX_EVENT_LOST();
                }
            }

            if( xDoRelease != pdFALSE )
            {
                vReleaseNetworkBufferAndDescriptor( pxReceivedBuffer );
            }
        }
    }

    return xReturn;
}

//===================================================================================================
// PHY IO Functions
//===================================================================================================
//static
int32_t NetServices::ETH_PHY_IO_ReadReg( uint32_t ulDevAddr,
                                   uint32_t ulRegAddr,
                                   uint32_t * pulRegVal )
{
    int32_t iResult = -1;

    if( &heth == NULL ) {
    	DS->pushToLogsMon(name, LOG_CRITICAL, "ETH_PHY_IO_ReadReg heth value is NULL");
    }
    else {
		if( HAL_ETH_ReadPHYRegister( &heth, ulDevAddr, ulRegAddr, pulRegVal ) == HAL_OK )
		{
			iResult = 0;
		}
    }

    return iResult;
}
/*-----------------------------------------------------------*/

/**
 * @brief  Write a value to a PHY register through the MDIO interface.
 * @param  DevAddr: PHY port address
 * @param  RegAddr: PHY register address
 * @param  RegVal: Value to be written
 * @retval 0 if OK -1 if Error
 */
//static
int32_t NetServices::ETH_PHY_IO_WriteReg( uint32_t ulDevAddr,
                                    uint32_t ulRegAddr,
                                    uint32_t pulRegVal )
{
    int32_t iResult = -1;

    if( HAL_ETH_WritePHYRegister( &heth, ulDevAddr, ulRegAddr, pulRegVal ) == HAL_OK )
    {
        iResult = 0;
    }

    return iResult;
}

//===================================================================================================
// Ethernet Handling Functions
//===================================================================================================
void ETH_IRQHandler( void )
{
    HAL_ETH_IRQHandler( &( heth ) );
}

static void prvSetFlagsAndNotify( uint32_t ulFlags )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Ethernet RX-Complete callback function, elsewhere declared as weak.
     * No critical section needed, this function is called from an ISR. */
    ulISREvents |= ulFlags;

    /* Wakeup the prvEMACHandlerTask. */
    if( xEMACTaskHandle != NULL )
    {
//        vTaskNotifyGiveFromISR( xEMACTaskHandle, &( xHigherPriorityTaskWoken ) );
//        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}

void HAL_ETH_TxCpltCallback( ETH_HandleTypeDef * heth )
{
    ( void ) heth;
    prvSetFlagsAndNotify( EMAC_IF_TX_EVENT );
}

void HAL_ETH_RxCpltCallback( ETH_HandleTypeDef * heth )
{
    ( void ) heth;
    prvSetFlagsAndNotify( EMAC_IF_RX_EVENT );
}

void HAL_ETH_DMAErrorCallback( ETH_HandleTypeDef * heth )
{
    ( void ) heth;
    prvSetFlagsAndNotify( EMAC_IF_ERR_EVENT );
}

//===================================================================================================
//
//===================================================================================================
void NetServices::vNetworkInterfaceAllocateRAMToBuffers( NetworkBufferDescriptor_t pxNetworkBuffers[ ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS ] )
{
    uint8_t * ucRAMBuffer = ucNetworkPackets;
    uint32_t ul;

    for( ul = 0; ul < ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS; ul++ )
    {
        pxNetworkBuffers[ ul ].pucEthernetBuffer = ucRAMBuffer + ipBUFFER_PADDING;
        *( ( unsigned * ) ucRAMBuffer ) = ( unsigned ) ( &( pxNetworkBuffers[ ul ] ) );
        ucRAMBuffer += ETH_RX_BUF_SIZE;
    }
}

void NetServices::vClearOptionBit( volatile uint32_t * pulValue,
                             uint32_t ulValue )
{
    portENTER_CRITICAL();
    *( pulValue ) &= ~( ulValue );
    portEXIT_CRITICAL();
}

#if ( ipconfigHAS_PRINTF != 0 )
    size_t NetServices::uxGetOwnCount( ETH_HandleTypeDef * heth )
    {
        BaseType_t xIndex;
        BaseType_t xCount = 0;
        ETH_RxDescListTypeDef * dmarxdesclist = &heth->RxDescList;

        /* Count the number of RX descriptors that are owned by DMA. */
        for( xIndex = 0; xIndex < ETH_RX_DESC_CNT; xIndex++ )
        {
            __IO const ETH_DMADescTypeDef * dmarxdesc =
                ( __IO const ETH_DMADescTypeDef * )dmarxdesclist->RxDesc[ xIndex ];

            if( ( dmarxdesc->DESC3 & ETH_DMARXNDESCWBF_OWN ) != 0U )
            {
                xCount++;
            }
        }

        return xCount;
    }
#endif /* if ( ipconfigHAS_PRINTF != 0 ) */

//===================================================================================================
//
//===================================================================================================
//void prvIPTask( void * pvParameters )
//{
//    /* Just to prevent compiler warnings about unused parameters. */
//    ( void ) pvParameters;
//
//    prvIPTask_Initialise();
//
//    FreeRTOS_debug_printf( ( "prvIPTask started" ) );
//
//    /* Loop, processing IP events. */
//    while( ipFOREVER() == pdTRUE )
//    {
//    	while(!NS->start) {}
//        prvProcessIPEventsAndTimers();
//    }
//}

//static
//void NetServices::prvEMACHandlerTask( void * pvParameters )
//void prvEMACHandlerTask( void * pvParameters )
//{
///* When sending a packet, all descriptors in the transmission channel may
// * be occupied.  In stat case, the program will wait (block) for the counting
// * semaphore. */
//	const TickType_t ulMaxBlockTime = pdMS_TO_TICKS( 100U );
//
//	#if ( ipconfigHAS_PRINTF != 0 )
//		size_t uxTXDescriptorsUsed = 0U;
//		size_t uxRXDescriptorsUsed = ETH_RX_DESC_CNT;
//	#endif
//
//	( void ) pvParameters;
//
//	for( ; ; )
//	{
//		//MP if using code from cubemx designed...
////		while(!NS->start) { osThreadYield(); }
//
//		BaseType_t xResult = 0;
//
//		#if ( ipconfigHAS_PRINTF != 0 )
//		{
//			size_t uxUsed;
//			size_t uxOwnCount;
//
//			/* Call a function that monitors resources: the amount of free network
//			 * buffers and the amount of free space on the heap.  See FreeRTOS_IP.c
//			 * for more detailed comments. */
//			vPrintResourceStats();
//
//			/* Some more statistics: number of free descriptors. */
//			uxUsed = ETH_TX_DESC_CNT - uxSemaphoreGetCount( xTXDescriptorSemaphore );
//
//			if( uxTXDescriptorsUsed < uxUsed )
//			{
//				uxTXDescriptorsUsed = uxUsed;
//				FreeRTOS_printf( ( "TX descriptors %u/%u",
//								   uxTXDescriptorsUsed,
//								   ETH_TX_DESC_CNT ) );
//			}
//
//			uxOwnCount = NS->uxGetOwnCount( &( heth ) );
//
//			if( uxRXDescriptorsUsed > uxOwnCount )
//			{
//				uxRXDescriptorsUsed = uxOwnCount;
//				FreeRTOS_printf( ( "RX descriptors %u/%u",
//								   uxRXDescriptorsUsed,
//								   ETH_RX_DESC_CNT ) );
//			}
//		}
//		#endif /* ( ipconfigHAS_PRINTF != 0 ) */
//
//		ulTaskNotifyTake( pdFALSE, ulMaxBlockTime );
//
//		/* Wait for the Ethernet MAC interrupt to indicate that another packet
//		 * has been received. */
//		if( ( ulISREvents & EMAC_IF_RX_EVENT ) != 0U )
//		{
////			iptraceNETWORK_EVENT_ISR_RX( ulISREvents );
//			NS->vClearOptionBit( &( ulISREvents ), EMAC_IF_RX_EVENT );
//			xResult = NS->prvNetworkInterfaceInput();
//		}
//
//		/* When a packet has been transmitted, the descriptor must be
//		 * prepared for a next transmission.
//		 * When using zero-copy, the network buffer must be released
//		 * ( i.e. returned to the pool of network buffers ). */
//
//		if( ( ulISREvents & EMAC_IF_TX_EVENT ) != 0U )
//		{
////			iptraceNETWORK_EVENT_ISR_TX( ulISREvents );
//			NS->vClearOptionBit( &( ulISREvents ), EMAC_IF_TX_EVENT );
//
//			if( xSemaphoreTake( xTransmissionMutex, 10000U ) != pdFAIL )
//			{
//				ETH_Clear_Tx_Descriptors( &( heth ) );
//				xSemaphoreGive( xTransmissionMutex );
//			}
//		}
//
//		/* Some error has occurred, possibly an overflow or an underflow. */
//		if( ( ulISREvents & EMAC_IF_ERR_EVENT ) != 0U )
//		{
//			iptraceNETWORK_EVENT_ISR_ERR( ulISREvents );
//			NS->vClearOptionBit( &( ulISREvents ), EMAC_IF_ERR_EVENT );
//
//			heth.gState = HAL_ETH_STATE_READY;
//			/* Enable all interrupts */
//			HAL_ETH_Start_IT( &( heth ) );
//			xResult += NS->prvNetworkInterfaceInput();
//		}
//
//		if( xPhyCheckLinkStatus( &xPhyObject, xResult ) != pdFALSE )
//		{
//			/*
//			 * The function xPhyCheckLinkStatus() returns pdTRUE if the
//			 * Link Status has changes since it was called the last time.
//			 */
//			if( NS->xSTM32H_GetPhyLinkStatus( pxMyInterface ) == pdFALSE )
//			{
//				/* Stop the DMA transfer. */
//				HAL_ETH_Stop_IT( &( heth ) );
//				/* Clear the Transmit buffers. */
//				memset( &( DMATxDscrTab ), '\0', sizeof( DMATxDscrTab ) );
//				/* Since the link is down, clear the descriptors. */
//				ETH_Clear_Tx_Descriptors( &( heth ) );
//			}
//			else
//			{
//				/* Something has changed to a Link Status, need re-check. */
//				NS->prvEthernetUpdateConfig( pdFALSE );
//			}
//		}
//	}
//}

//===================================================================================================
//
//===================================================================================================
eDHCPCallbackAnswer_t NetServices::xApplicationDHCPHook_Multi( eDHCPCallbackPhase_t eDHCPPhase,
                                                  struct xNetworkEndPoint * pxEndPoint,
                                                  IP_Address_t * pxIPAddress ) {
	eDHCPCallbackAnswer_t eReturn;
	uint32_t ulStaticIPAddress, ulStaticNetMask;

	  /* This hook is called in a couple of places during the DHCP process, as
	  identified by the eDHCPPhase parameter. */
	  switch( eDHCPPhase )
	  {
	    case eDHCPPhasePreDiscover  :
	      /* A DHCP discovery is about to be sent out.  eDHCPContinue is
	      returned to allow the discovery to go out.

	      If eDHCPUseDefaults had been returned instead then the DHCP process
	      would be stopped and the statically configured IP address would be
	      used.

	      If eDHCPStopNoChanges had been returned instead then the DHCP
	      process would be stopped and whatever the current network
	      configuration was would continue to be used. */
	      eReturn = eDHCPContinue;
	      break;

	    case eDHCPPhasePreRequest  :
	      /* An offer has been received from the DHCP server, and the offered
	      IP address is passed in the ulIPAddress parameter.  Convert the
	      offered and statically allocated IP addresses to 32-bit values. */

//	    	const uint8_t ucIPAddress[ 4 ] = { 192, 168, 13, 133 };
//	    	const uint8_t ucNetMask[ 4 ] =  { 255, 255, 255, 0 };
//	    	const uint8_t ucGatewayAddress[ 4 ] = { 192, 168, 13, 1 };
//	    	const uint8_t ucDNSServerAddress[ 4 ] = { 8, 8, 8, 8 };
//	    	const uint8_t ucMACAddress[ 6 ] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };

		ulStaticIPAddress = FreeRTOS_inet_addr_quick( ucIPAddress[0], ucIPAddress[1], ucIPAddress[2], ucIPAddress[3] );

	    ulStaticNetMask = FreeRTOS_inet_addr_quick( ucNetMask[0], ucNetMask[1], ucNetMask[2], ucNetMask[3] );

	      /* Mask the IP addresses to leave just the sub-domain octets. */
	      ulStaticIPAddress &= ulStaticNetMask;
	      ulIPAddress &= ulStaticNetMask;

	      /* Are the sub-domains the same? */
	      if( ulStaticIPAddress == ulIPAddress )
	      {
	        /* The sub-domains match, so the default IP address can be
	        used.  The DHCP process is stopped at this point. */
	        eReturn = eDHCPUseDefaults;
	      }
	      else
	      {
	        /* The sub-domains don't match, so continue with the DHCP
	        process so the offered IP address is used. */
	        eReturn = eDHCPContinue;
	      }

	      break;

	    default :
	      /* Cannot be reached, but set eReturn to prevent compiler warnings
	      where compilers are disposed to generating one. */
	      eReturn = eDHCPContinue;
	      break;
	  }

	  return eReturn;
}

BaseType_t NetServices::xApplicationGetRandomNumber( uint32_t * pulNumber ) {
	BaseType_t retour = pdFALSE;

	if(HAL_RNG_GenerateRandomNumber(&hrng, pulNumber) != HAL_OK) {
		retour = pdFALSE;
	}
	else {
		retour = pdTRUE;
	}

	return retour;
}


void NetServices::vApplicationPingReplyHook( ePingReplyStatus_t eStatus, uint16_t usIdentifier ) {
	switch( eStatus )
	    {
	        case eSuccess    :
	            /* A valid ping reply has been received.  Post the sequence number
	            on the queue that is read by the vSendPing() function below.  Do
	            not wait more than 10ms trying to send the message if it cannot be
	            sent immediately because this function is called from the TCP/IP
	            RTOS task - blocking in this function will block the TCP/IP RTOS task. */
//	            xQueueSend( xPingReplyQueue, &usIdentifier, 10 / portTICK_PERIOD_MS );
	            break;

	        case eInvalidChecksum :
	        case eInvalidData :
	            /* A reply was received but it was not valid. */
	            break;
	    }
}

BaseType_t NetServices::vSendPingGW()
{
	 const char *pcIPAddress = "192.168.13.1";

	uint16_t usRequestSequenceNumber, usReplySequenceNumber;
	uint32_t ulIPAddress;

	DS->pushToLogsMon(name, LOG_INFO, "Starting to ping gateway IPv4 address");

    /* The pcIPAddress parameter holds the destination IP address as a string in
    decimal dot notation (for example, "192.168.0.200").  Convert the string into
    the required 32-bit format. */
    ulIPAddress = FreeRTOS_inet_addr( (const char*)pcIPAddress ); //MP ADD prototyping (const char*)

    /* Send a ping containing 8 data bytes.  Wait (in the Blocked state) a
    maximum of 100ms for a network buffer into which the generated ping request
    can be written and sent. */
    usRequestSequenceNumber = FreeRTOS_SendPingRequest( ulIPAddress, 8, 100 / portTICK_PERIOD_MS );

    if( usRequestSequenceNumber == pdFAIL )
    {
        /* The ping could not be sent because a network buffer could not be
        obtained within 100ms of FreeRTOS_SendPingRequest() being called. */
    	return pdFALSE;
    }
    else
    {
        /* The ping was sent.  Wait 200ms for a reply.  The sequence number from
        each reply is sent from the vApplicationPingReplyHook() on the
        xPingReplyQueue queue (this is not standard behaviour, but implemented in
        the example function above).  It is assumed the queue was created before
        this function was called! */
        if( xQueueReceive( xPingReplyQueue,
                           &usReplySequenceNumber,
                           200 / portTICK_PERIOD_MS ) == pdPASS )
        {
            /* A ping reply was received.  Was it a reply to the ping just sent? */
            if( usRequestSequenceNumber == usReplySequenceNumber )
            {
                /* This was a reply to the request just sent. */
            	DS->pushToLogsMon(name, LOG_INFO, "Received ping reply from gateway");
            }
        }
    }

    return pdTRUE;
}

void NetServices::vApplicationIPNetworkEventHook_Multi( eIPCallbackEvent_t eNetworkEvent,  struct xNetworkEndPoint * pxEndPoint ) {
	static BaseType_t xTasksAlreadyCreated = pdFALSE;

	/* Check this was a network up event, as opposed to a network down event. */
	if( eNetworkEvent == eNetworkUp )
	{
		/* Create the tasks that use the TCP/IP stack if they have not already been
		   created. */
		if( xTasksAlreadyCreated == pdFALSE )
		{
			/*
			 * Create the tasks here.
			 */

			xTasksAlreadyCreated = pdTRUE;
			ready = true;
		}

		/* Print out the network configuration, which may have come from a DHCP
		 * server. */
//		showEndPoint( pxEndPoint );
	}
}

uint32_t NetServices::ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress,
                                             uint16_t usSourcePort,
                                             uint32_t ulDestinationAddress,
                                             uint16_t usDestinationPort ) {
	uint32_t pulNumber = 0;
	if(HAL_RNG_GenerateRandomNumber(&hrng, &pulNumber) != HAL_OK) {
		//LOG
	}
	else {
		//LOG
	}

	return pulNumber;
}

//DEPRECATED since v4.0.0 use FreeRTOS_GetEndPointConfiguration
const uint8_t * NetServices::FreeRTOS_GetMACAddress( void ) {
//	uint32_t pulIPAddress;
//	uint32_t pulNetMask;
//	uint32_t pulGatewayAddress;
//	uint32_t pulDNSServerAddress;
//	const struct xNetworkEndPoint pxEndPoint;
//
//	FreeRTOS_GetEndPointConfiguration( &pulIPAddress, &pulNetMask, &pulGatewayAddress, &pulDNSServerAddress, &pxEndPoint );
//
//	return (uint8_t)&pxEndPoint.xMACAddress;
	return ucMACAddress;
}
//CONVERSION TEMPORAIRE DOUTEUSE, À REVOIR DGB...MACAddress_t to uint8_t


#include "../../FreeRTOS-Plus-TCP/source/include/FreeRTOS_ARP.h"
extern ARPCacheRow_t xARPCache[ ipconfigARP_CACHE_ENTRIES ];

void NetServices::PrintARPCache( uint8_t* number, touchgfx::Unicode::UnicodeChar todisplay[][MAXDISPLAY] )
{
	BaseType_t x, xCount = 0;

//	char todisplay[ipconfigARP_CACHE_ENTRIES][MAXDISPLAY];
//	uint8_t displaysize = 0;

	/* Loop through each entry in the ARP cache. */
	for( x = 0; x < ipconfigARP_CACHE_ENTRIES; x++ )
	{
		if( ( xARPCache[ x ].ulIPAddress != 0U ) && ( xARPCache[ x ].ucAge > ( uint8_t ) 0U ) )
		{
			/* See if the MAC-address also matches, and we're all happy */
//example ref for touchgfx::Unicode::snprintf(pxNetworkInterfaceBuffer, PXNETWORKINTERFACE_SIZE, "0x%X", endpoint->pxNetworkInterface);
//			FreeRTOS_printf( ( "%2d: %3u - %16x : %02X:%02X:%02X:%02X:%02X:%02X",

			char *prettyIP = MP_Pretty_IP( xARPCache[ x ].ulIPAddress );
			touchgfx::Unicode::snprintf( todisplay[x], MAXDISPLAY, "%2d: %3u - %16s - %02X:%02X:%02X:%02X:%02X:%02X",
			   ( int ) x+1,
			   xARPCache[ x ].ucAge,
			   prettyIP,
			   xARPCache[ x ].xMACAddress.ucBytes[ 0 ],
			   xARPCache[ x ].xMACAddress.ucBytes[ 1 ],
			   xARPCache[ x ].xMACAddress.ucBytes[ 2 ],
			   xARPCache[ x ].xMACAddress.ucBytes[ 3 ],
			   xARPCache[ x ].xMACAddress.ucBytes[ 4 ],
			   xARPCache[ x ].xMACAddress.ucBytes[ 5 ] );

			xCount++;
			vPortFree(prettyIP);
		}
	}

	*number = xCount;
////	FreeRTOS_printf( ( "Arp has %ld entries", xCount ) );
//	if(*number == 0) {
//		touchgfx::Unicode::snprintf(textarea,sizeof("(empty list)"),"(empty list)");
//	}
//	else {
//		for(uint8_t i=0; i<xCount; i++) {
//			touchgfx::Unicode::snprintf(&textarea[0]+(displaysize*sizeof(touchgfx::Unicode::UnicodeChar)),sizeof(todisplay[i]),todisplay[i]);
//	//			touchgfx::Unicode::snprintf(textarea,displaysize,todisplay);
//			displaysize = sizeof(todisplay[i]);
//		}
//	}
}

char* NetServices::getName() {
	return name;
}

//===================================================================================================
//
//===================================================================================================
NetServices *NS = NetServices::CreateInstance();

//#endif


#ifndef __MPAPP_H
#define __MPAPP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"
#include "stdint.h"
#include <stdarg.h>
//#include "FreeRTOS_IP.h"
//#include "../../FreeRTOS-Plus-TCP/source/include/FreeRTOS_IP.h"
//#include "../../FreeRTOS-Plus-TCP/source/include/FreeRTOS_Routing.h"

#include "stm32h743i_eval.h"

#include "mqtt_config.h"


#define COLOR_STATUS_OK			0xFFFFFF
#define COLOR_STATUS_DEBUG		0xF3E1FF
#define COLOR_STATUS_ERROR		0xF7BCBC
#define COLOR_STATUS_CRITICAL	0xF7BCBC
#define COLOR_STATUS_INFO		0xFFFFFF
#define COLOR_STATUS_WARNING	0xFFF8C4
#define COLOR_IMAGE_WARNING		0xFAEE14
#define COLOR_STATUS_CATEGORY	0xEBEBEB


#define MP_LOGS_MAX 		50
#define CAT_LENGTH 			15U
#define LOG_LENGTH 			150U
#define LOGS_MAX			50U
#define THREAD_HEARTBEAT	1000
#define TIME_TO_CONNECT		3
#define TEXTLINE_SIZE 20
#define TEXTBUFF_SIZE 150
#define TEXTLOGS_SIZE 50
//#define TEXTLOGS_SIZE 25
#define MAXDISPLAY 50
#define PRETTY_IP_LENGTH	16
#define PRETTY_MAC_LENGTH	18


char* MP_Pretty_IP(uint32_t theip);
char* MP_Pretty_MAC(void * themac);

//void showEndPoint( NetworkEndPoint_t * pxEndPoint );

void MP_Blink_LED(Led_TypeDef LED, uint8_t times);


#define DEBUG_MP_OUTPUT 	0
#define DEBUG_MQTT_OUTPUT 	0
//#define DEBUG_IPTRACE		0

#ifdef DEBUG_IPTRACE
#define iptraceIP_TASK_STARTING()					\
	pushToLogsMon("IP TRACE", LOG_DEBUG, "Network IP task starting");

#define iptraceNETWORK_EVENT_RECEIVED( eEvent )		\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "Network event received: %d", (int)eEvent);

#define iptraceNETWORK_EVENT_ISR_RX( ulISREvents )		\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "Network event ISR RX: %d", (int)ulISREvents); \
		pushToLogsMon("IP TRACE", LOG_DEBUG, "");

#define iptraceNETWORK_EVENT_ISR_TX( ulISREvents )		\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "Network event ISR TX: %d", (int)ulISREvents); \
		pushToLogsMon("IP TRACE", LOG_DEBUG, "");

#define iptraceNETWORK_EVENT_ISR_ERR( ulISREvents )		\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "Network event ISR TX: %d", (int)ulISREvents); \
		pushToLogsMon("IP TRACE", LOG_DEBUG, "");

#define iptraceFAILED_TO_CREATE_EVENT_GROUP()		\
		pushToLogsMon("IP TRACE", LOG_ERROR, "Network failed to create event group");

#define iptraceFAILED_TO_NOTIFY_SELECT_GROUP()		\
		pushToLogsMon("IP TRACE", LOG_ERROR, "Network failed to notify selected group");

#define iptraceWAITING_FOR_TX_DMA_DESCRIPTOR() \
		pushToLogsMon("IP TRACE", LOG_DEBUG, "Network waiting for tx dma descriptor");

#define iptraceSENDING_UDP_PACKET(ulIPAddress) \
		char *theip = MP_Pretty_IP(ulIPAddress); \
		pushToLogsMon("IP TRACE", LOG_DEBUG, "Sending UDP packet to address: %s ...", theip); \
		vPortFree(theip);
//		pushToLogsMon("IP TRACE", LOG_OK, "Sending UDP packet");

/*#define iptraceNETWORK_BUFFER_OBTAINED( pxReturn ) \
		pushToLogsMon("IP TRACE", LOG_DEBUG, "Network buffer obtained");*/

#define iptraceFAILED_TO_OBTAIN_NETWORK_BUFFER() \
		pushToLogsMon("IP TRACE", LOG_DEBUG, "Network buffer failed to be obtained");

#define iptraceNETWORK_BUFFER_OBTAINED_FROM_ISR() \
		pushToLogsMon("IP TRACE", LOG_DEBUG, "Network buffer obtained by ISR");

#define iptraceFAILED_TO_OBTAIN_NETWORK_BUFFER_FROM_ISR() \
		pushToLogsMon("IP TRACE", LOG_ERROR, "Network buffer failed to obtained by ISR");

/*
#define iptraceNETWORK_BUFFER_RELEASED(pxNetworkBuffer) \
		pushToLogsMon("IP TRACE", LOG_DEBUG, "Network buffer released");
*/

#define iptraceNETWORK_DOWN() \
		pushToLogsMon("IP TRACE", LOG_WARNING, "Network down");

/*#define iptraceRECVFROM_TIMEOUT() \
		pushToLogsMon("IP TRACE", LOG_WARNING, "Network timeout to receive from");*/

#define iptraceNETWORK_INTERFACE_RECEIVE() \
		pushToLogsMon("IP TRACE", LOG_DEBUG, "Network interface receive");
/*
#define iptraceNETWORK_INTERFACE_TRANSMIT() \
		pushToLogsMon("IP TRACE", LOG_DEBUG, "Network interface transmit");
*/
#define iptraceETHERNET_RX_EVENT_LOST() \
		pushToLogsMon("IP TRACE", LOG_ERROR, "Network stack rx event lost");
/*
#define iptraceNETWORK_INTERFACE_INPUT( uxDataLength, pucEthernetBuffer ) \
		pushToLogsMon("IP TRACE", LOG_DEBUG, "Network interface input");
*/
/*
#define iptraceNETWORK_INTERFACE_OUTPUT( uxDataLength, pucEthernetBuffer ) \
		pushToLogsMon("IP TRACE", LOG_DEBUG, "Network interface output");
//		pushToLogsMon("IP TRACE", LOG_DEBUG, "Network interface output len: %d", uxDataLength);
*/
#define iptraceSTACK_TX_EVENT_LOST( xEvent )		\
		pushToLogsMon("IP TRACE", LOG_ERROR, "Network stack TX event lost"); //: %d", xEvent);

#define iptraceDELAYED_ARP_REQUEST_STARTED()		\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP request delayed started");

#define iptrace_DELAYED_ARP_REQUEST_REPLIED()		\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP delayed request replied");

#define iptraceDELAYED_ARP_BUFFER_FULL()		\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP delayed buffer full");

/*
#define traceARP_PACKET_RECEIVED()		\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP packet received");
*/
/*#define iptraceMEM_STATS_DELETE(pxSocketSet)		\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "MEM stats deleted: %x", pxSocketSet);
*/

#define iptraceICMP_PACKET_RECEIVED()				\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ICMP packet received");

#define iptraceICMP_PACKET_RECEIVED()				\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ICMP packet received");

#define iptraceSENDING_PING_REPLY( ulIPAddress ) 	\
		char *theip = MP_Pretty_IP(ulIPAddress); 	\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ICMP echo reply to %s", theip); \
		vPortFree(theip);
//		pushToLogsMon("IP TRACE", LOG_DEBUG, "ICMP echo reply");

/*
#define iptraceSENDING_ARP_REPLY( ulIPAddress ) 	\
		char *theip = MP_Pretty_IP(ulIPAddress); 	\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP reply send to %s", theip); \
		vPortFree(theip);
//		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP request");
*/
#define iptraceDROPPED_INVALID_ARP_PACKET(pxARPHeader)		\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP dropped invalid packet");

#define iptracePROCESSING_RECEIVED_ARP_REPLY( ulIPAddress ) 	\
		char *theip = MP_Pretty_IP(ulIPAddress); 	\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP reply from %s", theip); \
		vPortFree(theip);
//		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP reply");

#define iptracePACKET_DROPPED_TO_GENERATE_ARP( ulIPAddress ) 	\
		pushToLogsMon("IP TRACE", LOG_ERROR, "Packet dropped to generate ARP");

#define iptraceDELAYED_ARP_TIMER_EXPIRED( ulIPAddress ) 	\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP timer expired");

#define iptraceARP_TABLE_ENTRY_EXPIRED( ulIPAddress ) 	\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP table entry expired");

#define iptraceARP_TABLE_ENTRY_CREATED( ulIPAddress, pxMACAddress ) 	\
		char *theip  = MP_Pretty_IP(ulIPAddress); 	\
		char *themac = MP_Pretty_MAC(pxMACAddress); 	\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP table entry, created: %s = %s", theip, themac); \
		vPortFree(theip);	\
		vPortFree(themac);
//		pushToLogsMon("IP TRACE", LOG_WARNING, "ARP table entry, created");
/*		char *theip  = MP_Pretty_IP(ulIPAddress); 	\
		pushToLogsMon("IP TRACE", LOG_WARNING, "ARP table entry, created: %s", theip); \
		vPortFree(theip);	\
		vPortFree(themac); */

#define iptraceARP_TABLE_ENTRY_WILL_EXPIRE( ulIPAddress ) 	\
		char *theip = MP_Pretty_IP(ulIPAddress); 	\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP table entry, will expire for: %s", theip); \
		vPortFree(theip);
//		pushToLogsMon("IP TRACE", LOG_WARNING, "ARP table entry, will expire");

#define iptraceCREATING_ARP_REQUEST( ulIPAddress ) 	\
		char *theip = MP_Pretty_IP(ulIPAddress); 	\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP creating request for: %s", theip); \
		vPortFree(theip);
//		pushToLogsMon("IP TRACE", LOG_DEBUG, "ARP creating request");


#define iptraceFAILED_TO_CREATE_SOCKET()		\
		pushToLogsMon("IP TRACE", LOG_ERROR, "TCP failed to create socket");

#define iptraceRECVFROM_DISCARDING_BYTES( byteslendiscarded )		\
		pushToLogsMon("IP TRACE", LOG_WARNING, "RECV discarded bytes");

#define iptraceRECVFROM_INTERRUPTED()		\
		pushToLogsMon("IP TRACE", LOG_ERROR, "RECV interrupted");

#define iptraceNO_BUFFER_FOR_SENDTO()		\
		pushToLogsMon("IP TRACE", LOG_ERROR, "SEND no buffer to sendto");

#define iptraceSENDTO_SOCKET_NOT_BOUND()		\
		pushToLogsMon("IP TRACE", LOG_ERROR, "SEND socket not bound");

#define iptraceSENDTO_DATA_TOO_LONG()		\
		pushToLogsMon("IP TRACE", LOG_ERROR, "SEND data too long");

#define iptraceBIND_FAILED( pxSocket, sin_port ) \
		pushToLogsMon("IP TRACE", LOG_ERROR, "SOCKET bind failed from socket"); //: %x on port: %u", pxSocket, sin_port);

#define iptraceDHCP_REQUESTS_FAILED_USING_DEFAULT_IP_ADDRESS( ulIPAddress ) 	\
		char *theip = MP_Pretty_IP(ulIPAddress); \
		pushToLogsMon("IP TRACE", LOG_WARNING, "DHCP request failed using default IPv4 address: %s", theip); \
		vPortFree(theip);

#define iptraceDHCP_REQUESTS_FAILED_USING_DEFAULT_IPv6_ADDRESS( xIPAddress ) 	\
		pushToLogsMon("IP TRACE", LOG_WARNING, "DHCP request failed using default IPv6 address");

#define iptraceDHCP_SUCCEEDED(ulOfferedIPAddress) 	\
		char *theip = MP_Pretty_IP(ulOfferedIPAddress); \
		pushToLogsMon("IP TRACE", LOG_DEBUG, "DHCP negotiation succeeded with IPv4 address: %s", theip); \
		vPortFree(theip);

#define iptraceSENDING_DHCP_DISCOVER()		\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "DHCP sending discovery...");

#define iptraceSENDING_DHCP_REQUEST()		\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "DHCP sending request...");

#define iptraceSENDING_DNS_REQUEST()		\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "DNS sending request...");

#define iptraceND_TABLE_ENTRY_EXPIRED( pxIPAddress ) 	\
		pushToLogsMon("IP TRACE", LOG_WARNING, "ND table entry expired");

#define iptraceND_TABLE_ENTRY_WILL_EXPIRE( pxIPAddress ) 	\
		pushToLogsMon("IP TRACE", LOG_WARNING, "ND table entry will expire...");

#define iptraceRA_REQUESTS_FAILED_USING_DEFAULT_IP_ADDRESS( ipv6_address )	\
		pushToLogsMon("IP TRACE", LOG_WARNING, "RA request failed using default IPv6 address");

#define iptraceRA_SUCCEEDED( ipv6_address )	\
		pushToLogsMon("IP TRACE", LOG_DEBUG, "RA request succeeded using default IPv6 address");

#endif

void MP_Output(const char* toprint,...);
void MP_MQTT_Output(const char* toprint,...);
const char * pcApplicationHostnameHook( void );

void SetStatus_APP(uint8_t value);
void SetStatus_ETH(uint8_t value);
void SetStatus_CLOUD(uint8_t value);
void SetStatus_IP(uint8_t value);

uint8_t mpappGetStatus_ETH();
uint8_t mpappGetStatus_IP();
uint8_t mpappGetStatus_APP();
uint8_t mpappGetStatus_CLOUD();
uint8_t mpappGetStatus_IOT_CONNECTED();
uint8_t mpappGetStatus_IOT_TOTAL();

extern osSemaphoreId_t canLogHandle;
extern osMessageQueueId_t gui_msgHandle;
extern osMessageQueueId_t gui_logs_msgHandle;
extern osMessageQueueId_t logs_msgHandle;
extern osMessageQueueId_t logsmon_msgHandle;
extern osMessageQueueId_t ConnectionEventHandle;
extern osMessageQueueId_t ETHEventHandle;

enum {
	CLEAR_LOGS,
	APP_NOTOK,
	APP_OK,
	NET_NOTOK,
	NET_OK,
	IP_NOTOK,
	IP_OK,
	STORAGE_NOTOK,
	STORAGE_OK,
	CLOUD_NOTOK,
	CLOUD_OK,
	CONTROL_PAUSE,
	CONTROL_PLAY
};

enum {
	LOG_OK,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_CRITICAL,
	LOG_DEBUG
};


typedef struct {
	uint8_t severity;
	char* category;
	char* alog;
} aLogStruct;

typedef aLogStruct* aLogStructPtr;


char* getErrorDescription(int code);
//void pushToLogsError(int code);
//void pushToLogs(char* alog);
void pushToLogsMon(char* category, uint8_t severity, char* alog, ...);

void traceETHHandler(uint32_t address);

osStatus_t pushGUIStatus(uint8_t *status);

void Error_Handler(void);
void Fast_View_Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif

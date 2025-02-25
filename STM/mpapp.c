#include "mpapp.h"

#include "string.h"
#include <stdio.h>

#include "../FreeRTOS-Plus-TCP/source/include/FreeRTOS_IP.h"

extern osEventFlagsId_t ACTIVITY_ETH_HANDLERHandle;


uint8_t statusAPP = APP_OK;
uint8_t statusETH = NET_NOTOK;
uint8_t statusIP = IP_NOTOK;
uint8_t statusCLOUD = CLOUD_NOTOK;
uint8_t statusIOT_CONNECTED = 0;
uint8_t statusIOT_TOTAL = 0;

char* LOGS[MP_LOGS_MAX];
aLogStructPtr LOGSMON[MP_LOGS_MAX];
uint8_t qtyLogs = 0;
uint8_t runOnce = 0;

char* MP_Pretty_MAC(void * themac) {
//	(MACAddress_t*) themac;

	char *retour = (char*)pvPortMalloc((PRETTY_MAC_LENGTH * sizeof(char)));
//	char *retour = (char*)pvPortMalloc((PRETTY_MAC_LENGTH ));

	if( themac != NULL ) {
		snprintf(retour, PRETTY_MAC_LENGTH, "%2X:%2X:%2X:%2X:%2X:%2X", (uint8_t)(((MACAddress_t*)themac)->ucBytes[0]), (uint8_t)(((MACAddress_t*)themac)->ucBytes[1]), (uint8_t)(((MACAddress_t*)themac)->ucBytes[2]), (uint8_t)(((MACAddress_t*)themac)->ucBytes[3]), (uint8_t)(((MACAddress_t*)themac)->ucBytes[4]), (uint8_t)(((MACAddress_t*)themac)->ucBytes[5]));
	}

	return retour;
}

char* MP_Pretty_IP(uint32_t theip) {
	char *retour = (char*)pvPortMalloc((PRETTY_IP_LENGTH * sizeof(char)));
//	char *retour = (char*)pvPortMalloc((PRETTY_IP_LENGTH));
	snprintf(retour, PRETTY_IP_LENGTH, "%d.%d.%d.%d", (uint8_t)(theip & 0x000000ff), (uint8_t)((theip & 0x0000ff00) >> 8), (uint8_t)((theip & 0x00ff0000) >> 16), (uint8_t)((theip & 0xff000000) >> 24) );

	return retour;
}

void SetStatus_ETH(uint8_t value) {
	statusETH = value;

	if( pushGUIStatus((uint8_t*)&statusETH) != osOK) {
		Error_Handler();
	}
}

void SetStatus_IP(uint8_t value) {
	statusIP = value;

	if( pushGUIStatus((uint8_t*)&statusIP) != osOK) {
		Error_Handler();
	}
}

void SetStatus_CLOUD(uint8_t value) {
	statusCLOUD = value;

	if( pushGUIStatus((uint8_t*)&statusCLOUD) != osOK) {
		Error_Handler();
	}
}

void SetStatus_APP(uint8_t value) {
	statusAPP = value;

	if( pushGUIStatus((uint8_t*)&statusAPP) != osOK) {
		Error_Handler();
	}
}

uint8_t mpappGetStatus_ETH() {
	return(statusETH);
}

uint8_t mpappGetStatus_IP() {
	return(statusIP);
}

uint8_t mpappGetStatus_APP() {
	return(statusAPP);
}

uint8_t mpappGetStatus_CLOUD() {
	return(statusCLOUD);
}

uint8_t mpappGetStatus_IOT_CONNECTED() {
	return(statusIOT_CONNECTED);
}

uint8_t mpappGetStatus_IOT_TOTAL() {
	return(statusIOT_TOTAL);
}

//void pushToLogsError(int code) {
//	pushToLogs(getErrorDescription(code));
//}

void pushToLogsMon(char* category, uint8_t severity, char* alog, ...)  {
	va_list args;
//	va_start(args,alog);

	uint8_t size = LOG_LENGTH;
	uint8_t sizeLog = strlen(alog)+1;
	uint8_t sizeCat = strlen(category)+1;
//	uint8_t sizeLog = strlen(alog);
//	uint8_t sizeCat = strlen(category);

	if(sizeLog < size)
		size = sizeLog;

	if(sizeCat > CAT_LENGTH)
		sizeCat = CAT_LENGTH;

	if(qtyLogs >= MP_LOGS_MAX)
	{
		qtyLogs = 0;
		runOnce = 1;
	}

	if( runOnce > 0 ) {
		vPortFree((char*)LOGSMON[qtyLogs]->category);
		vPortFree((char*)LOGSMON[qtyLogs]->alog);
		vPortFree((aLogStructPtr)LOGSMON[qtyLogs]);
	}


	LOGSMON[qtyLogs] = (aLogStructPtr)pvPortMalloc(sizeof(aLogStruct));

	if(LOGSMON[qtyLogs] == NULL)
		Error_Handler();

	LOGSMON[qtyLogs]->category = (char*)pvPortMalloc((sizeCat * sizeof(char)));
	snprintf(LOGSMON[qtyLogs]->category, sizeCat, category);

	LOGSMON[qtyLogs]->alog = (char*)pvPortMalloc((size * sizeof(char)));
//	LOGSMON[qtyLogs]->alog = (char*)pvPortMalloc((LOG_LENGTH * sizeof(char)));
//	snprintf(LOGSMON[qtyLogs]->alog, size, alog, args);
	snprintf(LOGSMON[qtyLogs]->alog, size, alog);

	LOGSMON[qtyLogs]->severity = severity;

	if(osMessageQueuePut(logsmon_msgHandle, &LOGSMON[qtyLogs],0,0) != osOK) {
//		Error_Handler();
		Fast_View_Error_Handler();
	}

	va_end(args);

	qtyLogs++;
}

void MP_Blink_LED(Led_TypeDef LED, uint8_t times) {
	for(uint8_t i=0; i<=times*2; i++) {
		BSP_LED_Toggle(LED);
		osDelay(100);
	}
}

void traceETHHandler(uint32_t address) {
//	if(osMessageQueuePut(ETHEventHandle, address,0,0) != osOK) {
//		Error_Handler();
//	}
////	else {
//////		MP_Blink_LED(LED_BLUE, 20);
//////		osEventFlagsSet(ACTIVITY_ETH_HANDLERHandle, 0x00000001U);
//////		osThreadYield();
////	}
//
////	MP_Blink_LED(LED_BLUE, 10);
}

void pushToLogs(char* alog) {
	uint8_t size = LOG_LENGTH;
	uint8_t sizeLog = strlen(alog)+1;

	if(sizeLog < size)
		size = sizeLog;

	if(qtyLogs >= MP_LOGS_MAX)
	{
		qtyLogs = 0;
	}

	vPortFree(LOGS[qtyLogs]);

	LOGS[qtyLogs] = (char*)pvPortMalloc(size * sizeof(char));

	snprintf(LOGS[qtyLogs], size, alog);
	osMessageQueuePut(logs_msgHandle, &(LOGS[qtyLogs]),0,0);

	qtyLogs++;
}

osStatus_t pushGUIStatus(uint8_t *status) {
	return(osMessageQueuePut(gui_msgHandle, (uint8_t*)status,0,0));
}

//char* getErrorDescription(int code) {
//	char *description = (char*)pvPortMalloc(150 * sizeof(char));
//
//	switch(code) {
//	case -0x7080:
//		sprintf(description, "SSL: ERROR, The requested feature is not available.");
//		break;
//	case -0x7100:
//		sprintf(description, "SSL: ERROR, Bad input parameters to function.");
//		break;
//	case -0x7180:
//		sprintf(description, "SSL: ERROR, Verification of the message MAC failed.");
//		break;
//	case -0x7200:
//		sprintf(description, "SSL: ERROR, An invalid SSL record was received.");
//		break;
//	case -0x7280:
//		sprintf(description, "SSL: ERROR, The connection indicated an EOF.");
//		break;
//	case -0x7300:
//		sprintf(description, "SSL: ERROR, An unknown cipher was received.");
//		break;
//	case -0x7380:
//		sprintf(description, "SSL: ERROR, The server has no ciphersuites in common with the client.");
//		break;
//	case -0x7400:
//		sprintf(description, "SSL: ERROR, No RNG was provided to the SSL module.");
//		break;
//	case -0x7480:
//		sprintf(description, "SSL: ERROR, No client certification received from the client, but required by the authentication mode.");
//		break;
//	case -0x7500:
//		sprintf(description, "SSL: ERROR, Our own certificate(s) is/are too large to send in an SSL message.");
//		break;
//	case -0x7580:
//		sprintf(description, "SSL: ERROR, The own certificate is not set, but needed by the server.");
//		break;
//	case -0x7600:
//		sprintf(description, "SSL: ERROR, The own private key or pre-shared key is not set, but needed.");
//		break;
//	case -0x7680:
//		sprintf(description, "SSL: ERROR, No CA Chain is set, but required to operate.");
//		break;
//	case -0x7700:
//		sprintf(description, "SSL: ERROR, An unexpected message was received from our peer.");
//		break;
//	case -0x7780:
//		sprintf(description, "SSL: ERROR, A fatal alert message was received from our peer.");
//		break;
//	case -0x7800:
//		sprintf(description, "SSL: ERROR, Verification of our peer failed.");
//		break;
//	case -0x7880:
//		sprintf(description, "SSL: ERROR, The peer notified us that the connection is going to be closed.");
//		break;
//	case -0x7900:
//		sprintf(description, "SSL: ERROR, Processing of the ClientHello handshake message failed.");
//		break;
//	case -0x7980:
//		sprintf(description, "SSL: ERROR, Processing of the ServerHello handshake message failed.");
//		break;
//	case -0x7A00:
//		sprintf(description, "SSL: ERROR, Processing of the Certificate handshake message failed.");
//		break;
//	case -0x7A80:
//		sprintf(description, "SSL: ERROR, Processing of the CertificateRequest handshake message failed.");
//		break;
//	case -0x7B00:
//		sprintf(description, "SSL: ERROR, Processing of the ServerKeyExchange handshake message failed.");
//		break;
//	case -0x7B80:
//		sprintf(description, "SSL: ERROR, Processing of the ServerHelloDone handshake message failed.");
//		break;
//	case -0x7C00:
//		sprintf(description, "SSL: ERROR, Processing of the ClientKeyExchange handshake message failed.");
//		break;
//	case -0x7C80:
//		sprintf(description, "SSL: ERROR, Processing of the ClientKeyExchange handshake message failed in DHM / ECDH Read Public.");
//		break;
//	case -0x7D00:
//		sprintf(description, "SSL: ERROR, Processing of the ClientKeyExchange handshake message failed in DHM / ECDH Calculate Secret.");
//		break;
//	case -0x7D80:
//		sprintf(description, "SSL: ERROR, Processing of the CertificateVerify handshake message failed.");
//		break;
//	case -0x7E00:
//		sprintf(description, "SSL: ERROR, Processing of the ChangeCipherSpec handshake message failed.");
//		break;
//	case -0x7E80:
//		sprintf(description, "SSL: ERROR, Processing of the Finished handshake message failed.");
//		break;
//	case -0x7F00:
//		sprintf(description, "SSL: ERROR, Memory allocation failed");
//		break;
//	case -0x7FF4:
//		sprintf(description, "SSL: ERROR, no available description, dig in my friend :-)");
//		break;
//	case -0x7F80:
//		sprintf(description, "SSL: ERROR, Hardware acceleration function returned with error");
//		break;
//	case -0x6F80:
//		sprintf(description, "SSL: ERROR, Hardware acceleration function skipped / left alone data");
//		break;
//	case -0x6F00:
//		sprintf(description, "SSL: ERROR, Processing of the compression / decompression failed");
//		break;
//	case -0x6E80:
//		sprintf(description, "SSL: ERROR, Handshake protocol not within min/max boundaries");
//		break;
//	case -0x6E00:
//		sprintf(description, "SSL: ERROR, Processing of the NewSessionTicket handshake message failed.");
//		break;
//	case -0x6D80:
//		sprintf(description, "SSL: ERROR, Session ticket has expired.");
//		break;
//	case -0x6D00:
//		sprintf(description, "SSL: ERROR, Public key type mismatch (eg, asked for RSA key exchange and presented EC key)");
//		break;
//	case -0x6C80:
//		sprintf(description, "SSL: ERROR, Unknown identity received (eg, PSK identity)");
//		break;
//	case -0x6C00:
//		sprintf(description, "SSL: ERROR, Internal error (eg, unexpected failure in lower-level module)");
//		break;
//	case -0x6B80:
//		sprintf(description, "SSL: ERROR, A counter would wrap (eg, too many messages exchanged).");
//		break;
//	case -0x6B00:
//		sprintf(description, "SSL: ERROR, Unexpected message at ServerHello in renegotiation.");
//		break;
//	case -0x6A80:
//		sprintf(description, "SSL: ERROR, DTLS client must retry for hello verification");
//		break;
//	case -0x6A00:
//		sprintf(description, "SSL: ERROR, A buffer is too small to receive or write a message");
//		break;
//	case -0x6980:
//		sprintf(description, "SSL: ERROR, None of the common ciphersuites is usable (eg, no suitable certificate, see debug messages).");
//		break;
//	case -0x6900:
//		sprintf(description, "SSL: ERROR, No data of requested type currently available on underlying transport.");
//		break;
//	case -0x6880:
//		sprintf(description, "SSL: ERROR, Connection requires a write call.");
//		break;
//	case -0x6800:
//		sprintf(description, "SSL: ERROR, The operation timed out.");
//		break;
//	case -0x6780:
//		sprintf(description, "SSL: ERROR, The client initiated a reconnect from the same port.");
//		break;
//	case -0x6700:
//		sprintf(description, "SSL: ERROR, Record header looks valid but is not expected.");
//		break;
//	case -0x6680:
//		sprintf(description, "SSL: ERROR, The alert message received indicates a non-fatal error.");
//		break;
//	case -0x6600:
//		sprintf(description, "SSL: ERROR, Couldn't set the hash for verifying CertificateVerify");
//		break;
//	case -0x6580:
//		sprintf(description, "SSL: ERROR, Internal-only message signaling that further message-processing should be done");
//		break;
//	case -0x6500:
//		sprintf(description, "SSL: ERROR, The asynchronous operation is not completed yet.");
//		break;
//	case -0x6480:
//		sprintf(description, "SSL: ERROR, Internal-only message signaling that a message arrived early.");
//		break;
//	case -0x7000:
//		sprintf(description, "SSL: ERROR, A cryptographic operation is in progress. Try again later.");
//		break;
//	case 0:
//		sprintf(description, "SSL: OK");
//		break;
//	default:
//		sprintf(description, "SSL: ERROR, no description documented");
//		break;
//	}
//
//	return(description);
//}

/*
 * MPLibs.h
 *
 *  Created on: Mar 6, 2025
 *      Author: admin
 */

#ifndef CORE_INC_MPLIBS_H_
#define CORE_INC_MPLIBS_H_


#define LOG_LENGTH			150U
#define CAT_LENGTH 			15U
#define THREAD_HEARTBEAT	1000


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
	CONTROL_PLAY,
	SYSTEM_NOTOK,
	SYSTEM_OK
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


#endif /* CORE_INC_MPLIBS_H_ */

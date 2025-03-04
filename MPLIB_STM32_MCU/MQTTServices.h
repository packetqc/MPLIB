#ifndef MQTTSERVICES_H_
#define MQTTSERVICES_H_

#include "mpapp.h"
//#include "mqtt.h"


#ifdef __cplusplus
extern "C" {
#endif

void StartMQTTServices(void *argument);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

#include "stdio.h"

#include "stm32h743i_eval.h"

//#include "altcp_tls_mbedtls_structs.h"
//#include "altcp_tls.h"


//typedef mqtt_client_s* clientP;


class MQTTServices {
	static int iMQ;
	static MQTTServices *instance;
	static char *name;

public:
	bool start = false;
	bool stop = false;

	static MQTTServices* CreateInstance() {
		if(iMQ==0) {
			instance =new MQTTServices;
			snprintf(name, CAT_LENGTH, "MQTT");
			iMQ=1;
		}
		return instance;
	}

	void blinkLED(uint8_t times);
	void init();

	char* getName();

//	static void mqtt_resolved_cb(const char *host, const ip_addr_t *ipaddr,void *callback_arg);

	void Start();
	void Stop();
	int Connect();
	int Disconnect();
	bool IsConnected();
//		const ip_addr_t *ipaddr, uint16_t port,
//	   const uint8_t *ca_cert_str, size_t ca_cert_len,
//	   const uint8_t *dev_cert_str, size_t dev_cert_len,
//	   const uint8_t *dev_key_str, size_t dev_key_len,
//	   const uint8_t *dev_key_pass_str, size_t dev_key_pass_len);
//	static void prvInitializeTopicBuffers( void );

protected:
	bool connected = false;
//	clientP client;
//	mqtt_client_t *client;
//	struct mqtt_connect_client_info_t client_info;

	void mqtt_connect();
//	void example_publish(mqtt_client_t *client, void *arg);

	Led_TypeDef LED = LED_ORANGE;
	char log[LOG_LENGTH];
};

extern MQTTServices *MQ;

#endif

#endif /* MQTTSERVICES_H_ */

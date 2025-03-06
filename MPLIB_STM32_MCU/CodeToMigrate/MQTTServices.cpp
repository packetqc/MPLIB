#include <MQTTServices.h>

#include <DataServices.h>
#include "mpapp.h"

//#include "stm32h743i_eval.h"
#include "stm32h743i_eval_io.h"

//#include "mbedtls/net.h"
//#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
//#include "mbedtls/debug.h"
//
//#include "mbedtls/threading.h"
//#include "mbedtls/x509.h"
//#include "mbedtls/error.h"

#include "stdio.h"
#include "string.h"
#include "stddef.h"


/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "jobs.h"
#include "core_json.h"

///* Demo Specific configs. */
//#include "mqtt_config.h"
//
///* MQTT library includes. */
//#include "core_mqtt.h"
//
///* Exponential backoff retry include. */
//#include "backoff_algorithm.h"
//
///* Transport interface implementation include header for TLS. */
//#include "transport_mbedtls.h"

//#include <mqtt_demo_helpers.h>
#include <awsMQTT/mqtt_demo_helpers.h>


/* Compile time error for undefined configs. */
#ifndef configMQTT_BROKER_ENDPOINT
    #error "Define the config democonfigMQTT_BROKER_ENDPOINT by following the instructions in file demo_config.h."
#endif
#ifndef configROOT_CA_PEM
    #error "Please define Root CA certificate of the MQTT broker(democonfigROOT_CA_PEM) in demo_config.h."
#endif

#ifndef configTHING_NAME
    #error "Please define the Thing resource name, configTHING_NAME, in demo_config.h"
#endif

/**
 * @brief The length of #democonfigTHING_NAME.
 */
#define THING_NAME_LENGTH    ( ( uint16_t ) ( sizeof( configTHING_NAME ) - 1 ) )


/* Default values for configs. */
#ifndef configCLIENT_IDENTIFIER

/**
 * @brief The MQTT client identifier used in this example.  Each client identifier
 * must be unique so edit as required to ensure no two clients connecting to the
 * same broker use the same client identifier.
 *
 * @note Appending __TIME__ to the client id string will help to create a unique
 * client id every time an application binary is built. Only a single instance of
 * this application's compiled binary may be used at a time, since the client ID
 * will always be the same.
 */
    #define configCLIENT_IDENTIFIER    "toxicairmonhead"__TIME__
#endif

#ifndef configMQTT_BROKER_PORT

/**
 * @brief The port to use for the demo.
 */
    #define configMQTT_BROKER_PORT    ( 8883 )
#endif


//=====================================================================================================
//
//=====================================================================================================

//#include "FreeRTOS_sockets.h"
//#include "backoff_algorithm.h"
//#include "tcp_sockets_wrapper.h"
////#include "common.h"
//
////#include "error.h"

//=====================================================================================================
//
//=====================================================================================================

//
///* mbed TLS includes. */
//#include "mbedtls/ctr_drbg.h"
//#include "mbedtls/entropy.h"
//#include "mbedtls/ssl.h"
//#include "mbedtls/threading.h"
//#include "mbedtls/x509.h"
//#include "mbedtls/error.h"
//
///* MbedTLS includes. */
//#if !defined( MBEDTLS_CONFIG_FILE )
//    #include "mbedtls/mbedtls_config.h"
//#else
//    #include MBEDTLS_CONFIG_FILE
//#endif
////#include "threading_alt.h"
//#include "mbedtls/entropy.h"
//#include "mbedtls/ssl.h"
///* TCP Sockets Wrapper include.*/
//#include "tcp_sockets_wrapper.h"
///* MbedTLS Bio TCP sockets wrapper include. */
//#include "mbedtls_bio_tcp_sockets_wrapper.h"
//#include "transport_mbedtls.h"
//
////#include "error.h"
////#include "error.c"
////#include "asn1.h"


/**
 * @brief The JSON key of the execution object.
 *
 * Job documents received from the AWS IoT Jobs service are in JSON format.
 * All such JSON documents will contain this key, whose value represents the unique
 * identifier of a Job.
 */
#define jobsexampleEXECUTION_KEY                    "execution"

/**
 * @brief The length of #jobsexampleEXECUTION_KEY.
 */
#define jobsexampleEXECUTION_KEY_LENGTH             ( sizeof( jobsexampleEXECUTION_KEY ) - 1 )

/**
 * @brief The query key to use for searching the Job ID key in message payload
 * from AWS IoT Jobs service.
 *
 * Job documents received from the AWS IoT Jobs service are in JSON format.
 * All such JSON documents will contain this key, whose value represents the unique
 * identifier of a Job.
 */
#define jobsexampleQUERY_KEY_FOR_JOB_ID             jobsexampleEXECUTION_KEY  ".jobId"

/**
 * @brief The length of #jobsexampleQUERY_KEY_FOR_JOB_ID.
 */
#define jobsexampleQUERY_KEY_FOR_JOB_ID_LENGTH      ( sizeof( jobsexampleQUERY_KEY_FOR_JOB_ID ) - 1 )

/**
 * @brief The query key to use for searching the Jobs document ID key in message payload
 * from AWS IoT Jobs service.
 *
 * Job documents received from the AWS IoT Jobs service are in JSON format.
 * All such JSON documents will contain this key, whose value represents the unique
 * identifier of a Job.
 */
#define jobsexampleQUERY_KEY_FOR_JOBS_DOC           jobsexampleEXECUTION_KEY  ".jobDocument"

/**
 * @brief The length of #jobsexampleQUERY_KEY_FOR_JOBS_DOC.
 */
#define jobsexampleQUERY_KEY_FOR_JOBS_DOC_LENGTH    ( sizeof( jobsexampleQUERY_KEY_FOR_JOBS_DOC ) - 1 )

/**
 * @brief The query key to use for searching the Action key in Jobs document
 * from AWS IoT Jobs service.
 *
 * This demo program expects this key to be in the Job document. It is a key
 * specific to this demo.
 */
#define jobsexampleQUERY_KEY_FOR_ACTION             "action"

/**
 * @brief The length of #jobsexampleQUERY_KEY_FOR_ACTION.
 */
#define jobsexampleQUERY_KEY_FOR_ACTION_LENGTH      ( sizeof( jobsexampleQUERY_KEY_FOR_ACTION ) - 1 )

/**
 * @brief The query key to use for searching the Message key in Jobs document
 * from AWS IoT Jobs service.
 *
 * This demo program expects this key to be in the Job document if the "action"
 * is either "publish" or "print". It represents the message that should be
 * published or printed, respectively.
 */
#define jobsexampleQUERY_KEY_FOR_MESSAGE            "message"

/**
 * @brief The length of #jobsexampleQUERY_KEY_FOR_MESSAGE.
 */
#define jobsexampleQUERY_KEY_FOR_MESSAGE_LENGTH     ( sizeof( jobsexampleQUERY_KEY_FOR_MESSAGE ) - 1 )

/**
 * @brief The query key to use for searching the topic key in Jobs document
 * from AWS IoT Jobs service.
 *
 * This demo program expects this key to be in the Job document if the "action"
 * is "publish". It represents the MQTT topic on which the message should be
 * published.
 */
#define jobsexampleQUERY_KEY_FOR_TOPIC              "topic"

/**
 * @brief The length of #jobsexampleQUERY_KEY_FOR_TOPIC.
 */
#define jobsexampleQUERY_KEY_FOR_TOPIC_LENGTH       ( sizeof( jobsexampleQUERY_KEY_FOR_TOPIC ) - 1 )

/**
 * @brief Utility macro to generate the PUBLISH topic string to the
 * DescribeJobExecution API of AWS IoT Jobs service for requesting
 * the next pending job information.
 *
 * @param[in] thingName The name of the Thing resource to query for the
 * next pending job.
 */
#define DESCRIBE_NEXT_JOB_TOPIC( thingName ) \
    ( JOBS_API_PREFIX thingName JOBS_API_BRIDGE JOBS_API_JOBID_NEXT "/" JOBS_API_GETPENDING )

/**
 * @brief Utility macro to generate the subscription topic string for the
 * NextJobExecutionChanged API of AWS IoT Jobs service that is required
 * for getting notification about changes in the next pending job in the queue.
 *
 * @param[in] thingName The name of the Thing resource to query for the
 * next pending Job.
 */
#define NEXT_JOB_EXECUTION_CHANGED_TOPIC( thingName ) \
    ( JOBS_API_PREFIX thingName JOBS_API_BRIDGE JOBS_API_NEXTJOBCHANGED )

/**
 * @brief Format a JSON status message.
 *
 * @param[in] x one of "IN_PROGRESS", "SUCCEEDED", or "FAILED"
 */
#define MAKE_STATUS_REPORT( x )    "{\"status\":\"" x "\"}"

/**
 * @brief The maximum number of times to run the loop in this demo.
 *
 * @note The demo loop is attempted to re-run only if it fails in an iteration.
 * Once the demo loop succeeds in an iteration, the demo exits successfully.
 */
#ifndef JOBS_MAX_DEMO_LOOP_COUNT
    #define JOBS_MAX_DEMO_LOOP_COUNT    ( 3 )
#endif

/**
 * @brief Time in ticks to wait between retries of the demo loop if
 * demo loop fails.
 */
#define DELAY_BETWEEN_DEMO_RETRY_ITERATIONS_TICKS    ( pdMS_TO_TICKS( 5000U ) )

/**
 * @brief Length of the queue to pass Jobs messages to the job handling task.
 */
#define JOBS_MESSAGE_QUEUE_LEN                       ( 10U )

/*-----------------------------------------------------------*/

/**
 * @brief Currently supported actions that a job document can specify.
 */
typedef enum JobActionType
{
    JOB_ACTION_PRINT,   /**< Print a message. */
    JOB_ACTION_PUBLISH, /**< Publish a message to an MQTT topic. */
    JOB_ACTION_EXIT,    /**< Exit the demo. */
    JOB_ACTION_UNKNOWN  /**< Unknown action. */
} JobActionType;

//=====================================================================================================
//
//=====================================================================================================
/**
 * @brief Each compilation unit that consumes the NetworkContext must define it.
 * It should contain a single pointer to the type of your desired transport.
 * When using multiple transports in the same compilation unit, define this pointer as void *.
 *
 * @note Transport stacks are defined in FreeRTOS-Plus/Source/Application-Protocols/network_transport.
 */
//struct NetworkContext
//{
//    TlsTransportParams_t * pParams;
//};

/*-----------------------------------------------------------*/

/**
 * @brief Global entry time into the application to use as a reference timestamp
 * in the #prvGetTimeMs function. #prvGetTimeMs will always return the difference
 * between the current time and the global entry time. This will reduce the chances
 * of overflow for the 32 bit unsigned integer used for holding the timestamp.
 */
static uint32_t ulGlobalEntryTimeMs;

/**
 * @brief The MQTT context used for MQTT operation.
 */
static MQTTContext_t xMqttContext;

/**
 * @brief The network context used for mbedTLS operation.
 */
static NetworkContext_t xNetworkContext;

/**
 * @brief The parameters for the network context using mbedTLS operation.
 */
static TlsTransportParams_t xTlsTransportParams;


/**
 * @brief Static buffer used to hold MQTT messages being sent and received.
 */
static uint8_t usMqttConnectionBuffer[ configNETWORK_BUFFER_SIZE ];

/**
 * @brief Static buffer used to hold MQTT messages being sent and received.
 */
static MQTTFixedBuffer_t xBuffer =
{
    .pBuffer = usMqttConnectionBuffer,
    .size    = configNETWORK_BUFFER_SIZE
};

/**
 * @brief Static buffer used to hold the job ID of the single job that
 * is executed at a time in the demo. This buffer allows re-use of the MQTT
 * connection context for sending status updates of a job while it is being
 * processed.
 */
static uint8_t usJobIdBuffer[ configNETWORK_BUFFER_SIZE ];

/**
 * @brief Static buffer used to hold the job document of the single job that
 * is executed at a time in the demo. This buffer allows re-use of the MQTT
 * connection context for sending status updates of a job while it is being processed.
 */
static uint8_t usJobsDocumentBuffer[ configNETWORK_BUFFER_SIZE ];

/**
 * @brief A global flag which represents whether a job for the "Exit" action
 * has been received from AWS IoT Jobs service.
 */
static BaseType_t xExitActionJobReceived = pdFALSE;

/**
 * @brief A global flag which represents whether an error was encountered while
 * executing the demo.
 *
 * @note When this flag is set, the demo terminates execution.
 */
static BaseType_t xDemoEncounteredError = pdFALSE;

/**
 * @brief Queue used to pass incoming Jobs messages to a task to handle them.
 */
static QueueHandle_t xJobMessageQueue;

/**
 * @brief Converts a string in a job document to a #JobActionType
 * value.
 *
 * @param[in] pcAction The job action as a string.
 * @param[in] xActionLength The length of @p pcAction.
 *
 * @return A #JobActionType equivalent to the given string.
 */
static JobActionType prvGetAction( const char * pcAction, size_t xActionLength );

/**
 * @brief This example uses the MQTT library of the AWS IoT Device SDK for
 * Embedded C. This is the prototype of the callback function defined by
 * that library. It will be invoked whenever the MQTT library receives an
 * incoming message.
 *
 * @param[in] pxMqttContext MQTT context pointer.
 * @param[in] pxPacketInfo Packet Info pointer for the incoming packet.
 * @param[in] pxDeserializedInfo Deserialized information from the incoming packet.
 */
static void prvEventCallback( MQTTContext_t * pxMqttContext, MQTTPacketInfo_t * pxPacketInfo, MQTTDeserializedInfo_t * pxDeserializedInfo );

/**
 * @brief Process payload from NextJobExecutionChanged and DescribeJobExecution
 * API MQTT topics of AWS IoT Jobs service.
 *
 * This handler parses the received payload about the next pending job, identifies
 * the action requested in the job document, and executes the action.
 *
 * @param[in] pPublishInfo Deserialized publish info pointer for the incoming
 * packet.
 */
static void prvNextJobHandler( MQTTPublishInfo_t * pxPublishInfo );

/**
 * @brief Sends an update for a job to the UpdateJobExecution API of the AWS IoT Jobs service.
 *
 * @param[in] pcJobId The job ID whose status has to be updated.
 * @param[in] usJobIdLength The length of the job ID string.
 * @param[in] pcJobStatusReport The JSON formatted report to send to the AWS IoT Jobs service
 * to update the status of @p pcJobId.
 */
static void prvSendUpdateForJob( char * pcJobId, uint16_t usJobIdLength, const char * pcJobStatusReport );

/**
 * @brief Executes a job received from AWS IoT Jobs service and sends an update back to the service.
 * It parses the received job document, executes the job depending on the job "Action" type, and
 * sends an update to AWS for the Job.
 *
 * @param[in] pcJobId The ID of the job to execute.
 * @param[in] usJobIdLength The length of the job ID string.
 * @param[in] pcJobDocument The JSON document associated with the @a pcJobID job
 * that is to be processed.
 * @param[in] usDocumentLength The length of the job document.
 */
static void prvProcessJobDocument( char * pcJobId, uint16_t usJobIdLength, char * pcJobDocument, uint16_t jobDocumentLength );

/**
 * @brief The task used to demonstrate the Jobs library API.
 *
 * @param[in] pvParameters Parameters as passed at the time of task creation.
 * Not used in this example.
 */
void prvJobsDemoTask( void * pvParameters );

static JobActionType prvGetAction( const char * pcAction,
                                   size_t xActionLength )
{
    JobActionType xAction = JOB_ACTION_UNKNOWN;

    configASSERT( pcAction != NULL );

    if( strncmp( pcAction, "print", xActionLength ) == 0 )
    {
        xAction = JOB_ACTION_PRINT;
    }
    else if( strncmp( pcAction, "publish", xActionLength ) == 0 )
    {
        xAction = JOB_ACTION_PUBLISH;
    }
    else if( strncmp( pcAction, "exit", xActionLength ) == 0 )
    {
        xAction = JOB_ACTION_EXIT;
    }

    return xAction;
}

static void prvSendUpdateForJob( char * pcJobId,
                                 uint16_t usJobIdLength,
                                 const char * pcJobStatusReport )
{
    char pUpdateJobTopic[ JOBS_API_MAX_LENGTH( THING_NAME_LENGTH ) ];
    size_t ulTopicLength = 0;
    JobsStatus_t xStatus = JobsSuccess;

    configASSERT( ( pcJobId != NULL ) && ( usJobIdLength > 0 ) );
    configASSERT( pcJobStatusReport != NULL );

    /* Generate the PUBLISH topic string for the UpdateJobExecution API of AWS IoT Jobs service. */
    xStatus = Jobs_Update( pUpdateJobTopic,
                           sizeof( pUpdateJobTopic ),
                           configTHING_NAME,
                           THING_NAME_LENGTH,
                           pcJobId,
                           usJobIdLength,
                           &ulTopicLength );

    if( xStatus == JobsSuccess )
    {
        if( xPublishToTopic( &xMqttContext,
                             pUpdateJobTopic,
                             ulTopicLength,
                             pcJobStatusReport,
                             strlen( pcJobStatusReport ) ) == pdFALSE )
        {
            /* Set global flag to terminate demo as PUBLISH operation to update job status failed. */
            xDemoEncounteredError = pdTRUE;

            LogError( ( "Failed to update the status of job: JobID=%.*s, NewStatePayload=%s",
                        usJobIdLength, pcJobId, (char*)pcJobStatusReport ) );
        }
    }
    else
    {
        /* Set global flag to terminate demo as topic generation for UpdateJobExecution API failed. */
        xDemoEncounteredError = pdTRUE;

        LogError( ( "Failed to generate Publish topic string for sending job update: "
                    "JobID=%.*s, NewStatePayload=%s",
                    usJobIdLength, pcJobId, (char*)pcJobStatusReport ) );
    }
}

static void prvProcessJobDocument( char * pcJobId,
                                   uint16_t usJobIdLength,
                                   char * pcJobDocument,
                                   uint16_t jobDocumentLength )
{
    char * pcAction = NULL;
    size_t uActionLength = 0U;
    JSONStatus_t xJsonStatus = JSONSuccess;

    configASSERT( pcJobId != NULL );
    configASSERT( usJobIdLength > 0 );
    configASSERT( pcJobDocument != NULL );
    configASSERT( jobDocumentLength > 0 );

    xJsonStatus = JSON_Search( pcJobDocument,
                               jobDocumentLength,
                               jobsexampleQUERY_KEY_FOR_ACTION,
                               jobsexampleQUERY_KEY_FOR_ACTION_LENGTH,
                               &pcAction,
                               &uActionLength );

    if( xJsonStatus != JSONSuccess )
    {
        LogError( ( "Job document schema is invalid. Missing expected \"action\" key in document." ) );
        prvSendUpdateForJob( pcJobId, usJobIdLength, MAKE_STATUS_REPORT( "FAILED" ) );
    }
    else
    {
        JobActionType xActionType = JOB_ACTION_UNKNOWN;
        char * pcMessage = NULL;
        size_t ulMessageLength = 0U;

        xActionType = prvGetAction( pcAction, uActionLength );

        switch( xActionType )
        {
            case JOB_ACTION_EXIT:
                LogInfo( ( "Received job contains \"exit\" action. Updating state of demo." ) );
                xExitActionJobReceived = pdTRUE;
                prvSendUpdateForJob( pcJobId, usJobIdLength, MAKE_STATUS_REPORT( "SUCCEEDED" ) );
                break;

            case JOB_ACTION_PRINT:
                LogInfo( ( "Received job contains \"print\" action." ) );

                xJsonStatus = JSON_Search( pcJobDocument,
                                           jobDocumentLength,
                                           jobsexampleQUERY_KEY_FOR_MESSAGE,
                                           jobsexampleQUERY_KEY_FOR_MESSAGE_LENGTH,
                                           &pcMessage,
                                           &ulMessageLength );

                if( xJsonStatus == JSONSuccess )
                {
                    /* Print the given message if the action is "print". */
                    LogInfo( ( "\r"
                               "/*-----------------------------------------------------------*/\r"
                               "\r"
                               "%.*s\r"
                               "\r"
                               "/*-----------------------------------------------------------*/\r"
                               "\r", ulMessageLength, pcMessage ) );
                    prvSendUpdateForJob( pcJobId, usJobIdLength, MAKE_STATUS_REPORT( "SUCCEEDED" ) );
                }
                else
                {
                    LogError( ( "Job document schema is invalid. Missing \"message\" for \"print\" action type." ) );
                    prvSendUpdateForJob( pcJobId, usJobIdLength, MAKE_STATUS_REPORT( "FAILED" ) );
                }

                break;

            case JOB_ACTION_PUBLISH:
            {
                LogInfo( ( "Received job contains \"publish\" action." ) );
                char * pcTopic = NULL;
                size_t ulTopicLength = 0U;

                xJsonStatus = JSON_Search( pcJobDocument,
                                           jobDocumentLength,
                                           jobsexampleQUERY_KEY_FOR_TOPIC,
                                           jobsexampleQUERY_KEY_FOR_TOPIC_LENGTH,
                                           &pcTopic,
                                           &ulTopicLength );

                /* Search for "topic" key in the Jobs document.*/
                if( xJsonStatus != JSONSuccess )
                {
                    LogError( ( "Job document schema is invalid. Missing \"topic\" key for \"publish\" action type." ) );
                    prvSendUpdateForJob( pcJobId, usJobIdLength, MAKE_STATUS_REPORT( "FAILED" ) );
                }
                else
                {
                    xJsonStatus = JSON_Search( pcJobDocument,
                                               jobDocumentLength,
                                               jobsexampleQUERY_KEY_FOR_MESSAGE,
                                               jobsexampleQUERY_KEY_FOR_MESSAGE_LENGTH,
                                               &pcMessage,
                                               &ulMessageLength );

                    /* Search for "message" key in Jobs document.*/
                    if( xJsonStatus == JSONSuccess )
                    {
                        /* Publish to the parsed MQTT topic with the message obtained from
                         * the Jobs document.*/
                        if( xPublishToTopic( &xMqttContext,
                                             pcTopic,
                                             ulTopicLength,
                                             pcMessage,
                                             ulMessageLength ) == pdFALSE )
                        {
                            /* Set global flag to terminate demo as PUBLISH operation to execute job failed. */
                            xDemoEncounteredError = pdTRUE;

                            LogError( ( "Failed to execute job with \"publish\" action: Failed to publish to topic. "
                                        "JobID=%.*s, Topic=%.*s",
                                        usJobIdLength, pcJobId, ulTopicLength, pcTopic ) );
                        }

                        prvSendUpdateForJob( pcJobId, usJobIdLength, MAKE_STATUS_REPORT( "SUCCEEDED" ) );
                    }
                    else
                    {
                        LogError( ( "Job document schema is invalid. Missing \"message\" key for \"publish\" action type." ) );
                        prvSendUpdateForJob( pcJobId, usJobIdLength, MAKE_STATUS_REPORT( "FAILED" ) );
                    }
                }

            } break;

            case JOB_ACTION_UNKNOWN:
			default:
                configPRINTF( ( "Received Job document with unknown action %.*s.", uActionLength, pcAction ) );
                break;
        }
    }
}

static void prvNextJobHandler( MQTTPublishInfo_t * pxPublishInfo )
{
    configASSERT( pxPublishInfo != NULL );
    configASSERT( ( pxPublishInfo->pPayload != NULL ) && ( pxPublishInfo->payloadLength > 0 ) );

    /* Check validity of JSON message response from server.*/
    if( JSON_Validate( (const char*)(pxPublishInfo->pPayload), pxPublishInfo->payloadLength ) != JSONSuccess )
    {
        LogError( ( "Received invalid JSON payload from AWS IoT Jobs service" ) );
    }
    else
    {
        char * pcJobId = NULL;
        size_t ulJobIdLength = 0UL;

        /* Parse the Job ID of the next pending job execution from the JSON payload. */
        if( JSON_Search( ( char * ) pxPublishInfo->pPayload,
                         pxPublishInfo->payloadLength,
                         jobsexampleQUERY_KEY_FOR_JOB_ID,
                         jobsexampleQUERY_KEY_FOR_JOB_ID_LENGTH,
                         &pcJobId,
                         &ulJobIdLength ) != JSONSuccess )
        {
            LogWarn( ( "Failed to parse Job ID in message received from AWS IoT Jobs service: "
                       "IncomingTopic=%.*s, Payload=%.*s",
                       pxPublishInfo->topicNameLength, pxPublishInfo->pTopicName,
                       pxPublishInfo->payloadLength, (char*)(pxPublishInfo->pPayload) ) );
        }
        else
        {
            char * pcJobDocLoc = NULL;
            size_t ulJobDocLength = 0UL;

            configASSERT( ulJobIdLength < JOBS_JOBID_MAX_LENGTH );
            LogInfo( ( "Received a Job from AWS IoT Jobs service: JobId=%.*s",
                       ulJobIdLength, pcJobId ) );

            /* Copy the Job ID in the global buffer. This is done so that
             * the MQTT context's network buffer can be used for sending jobs
             * status updates to the AWS IoT Jobs service. */
            memcpy( usJobIdBuffer, pcJobId, ulJobIdLength );

            /* Search for the jobs document in the payload. */
            if( JSON_Search( ( char * ) pxPublishInfo->pPayload,
                             pxPublishInfo->payloadLength,
                             jobsexampleQUERY_KEY_FOR_JOBS_DOC,
                             jobsexampleQUERY_KEY_FOR_JOBS_DOC_LENGTH,
                             &pcJobDocLoc,
                             &ulJobDocLength ) != JSONSuccess )
            {
                LogWarn( ( "Failed to parse document of next job received from AWS IoT Jobs service: "
                           "Topic=%.*s, JobID=%.*s",
                           pxPublishInfo->topicNameLength, pxPublishInfo->pTopicName,
                           ulJobIdLength, pcJobId ) );
            }
            else
            {
                /* Copy the Job document in buffer. This is done so that the MQTT connection buffer can
                 * be used for sending jobs status updates to the AWS IoT Jobs service. */
                memcpy( usJobsDocumentBuffer, pcJobDocLoc, ulJobDocLength );

                /* Process the Job document and execute the job. */
                prvProcessJobDocument( (char*)usJobIdBuffer,
                                       ( uint16_t ) ulJobIdLength,
                                       (char*)usJobsDocumentBuffer,
                                       ulJobDocLength );
            }
        }
    }
}

//=====================================================================================================
//
//=====================================================================================================

static uint32_t prvGetTimeMs( void )
{
    TickType_t xTickCount = 0;
    uint32_t ulTimeMs = 0UL;

    /* Get the current tick count. */
    xTickCount = xTaskGetTickCount();

    /* Convert the ticks to milliseconds. */
    ulTimeMs = ( uint32_t ) xTickCount * MILLISECONDS_PER_TICK;

    /* Reduce ulGlobalEntryTimeMs from obtained time so as to always return the
     * elapsed time in the application. */
    ulTimeMs = ( uint32_t ) ( ulTimeMs - ulGlobalEntryTimeMs );

    return ulTimeMs;
}
//
///**
// * @brief Connect to MQTT broker with reconnection retries.
// *
// * If connection fails, retry is attempted after a timeout.
// * Timeout value will exponentially increase until the maximum
// * timeout value is reached or the number of attempts are exhausted.
// *
// * @param[out] pxNetworkContext The output parameter to return the created network context.
// *
// * @return The status of the final connection attempt.
// */
//static TlsTransportStatus_t prvConnectToServerWithBackoffRetries( NetworkCredentials_t * pxNetworkCredentials,NetworkContext_t * pNetworkContext );

//=====================================================================================================
//
//=====================================================================================================


//=====================================================================================================
//
//=====================================================================================================


//static TlsTransportStatus_t prvConnectToServerWithBackoffRetries( NetworkCredentials_t * pxNetworkCredentials,
//                                                                  NetworkContext_t * pxNetworkContext )
//{
//    TlsTransportStatus_t xNetworkStatus;
//    BackoffAlgorithmStatus_t xBackoffAlgStatus = BackoffAlgorithmSuccess;
//    BackoffAlgorithmContext_t xReconnectParams;
//    uint16_t usNextRetryBackOff = 0U;
//
//    /* Set the credentials for establishing a TLS connection. */
//    pxNetworkCredentials->pRootCa = ( const unsigned char * ) configROOT_CA_PEM;
//    pxNetworkCredentials->rootCaSize = sizeof( configROOT_CA_PEM );
//    pxNetworkCredentials->disableSni = configDISABLE_SNI;
//
//    /* Initialize reconnect attempts and interval.*/
//    BackoffAlgorithm_InitializeParams( &xReconnectParams,
//                                       mqttexampleRETRY_BACKOFF_BASE_MS,
//                                       mqttexampleRETRY_MAX_BACKOFF_DELAY_MS,
//                                       mqttexampleRETRY_MAX_ATTEMPTS );
//
//    /* Attempt to connect to the MQTT broker. If connection fails, retry after
//     * a timeout. Timeout value will exponentially increase until maximum
//     * attempts are reached.
//     */
//    do
//    {
//        /* Establish a TLS session with the MQTT broker. This example connects to
//         * the MQTT broker as specified in democonfigMQTT_BROKER_ENDPOINT and
//         * democonfigMQTT_BROKER_PORT at the top of this file. */
////        LogInfo( ( "Creating a TLS connection to %s:%u.\r",
////                   democonfigMQTT_BROKER_ENDPOINT,
////                   democonfigMQTT_BROKER_PORT ) );
//
//        DS->pushToLogsMon(MQ->getName(), LOG_INFO,
//			"Creating a TLS connection to %s:%u.\r",
//			configMQTT_BROKER_ENDPOINT,
//			configMQTT_BROKER_PORT );
//
//        /* Attempt to create a server-authenticated TLS connection. */
//        xNetworkStatus = TLS_FreeRTOS_Connect( pxNetworkContext,
//		   configMQTT_BROKER_ENDPOINT,
//		   configMQTT_BROKER_PORT,
//		   pxNetworkCredentials,
//		   mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS,
//		   mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS );
//
//        if( xNetworkStatus != TLS_TRANSPORT_SUCCESS )
//        {
//            /* Generate a random number and calculate backoff value (in milliseconds) for
//             * the next connection retry. */
//            xBackoffAlgStatus = BackoffAlgorithm_GetNextBackoff( &xReconnectParams, uxRand(), &usNextRetryBackOff );
//
//            if( xBackoffAlgStatus == BackoffAlgorithmRetriesExhausted )
//            {
////                LogError( ( "Connection to the broker failed, all attempts exhausted." ) );
//            	DS->pushToLogsMon(MQ->getName(), LOG_ERROR, "Connection to the broker failed, all attempts exhausted");
//            }
//            else if( xBackoffAlgStatus == BackoffAlgorithmSuccess )
//            {
////                LogWarn( ( "Connection to the broker failed. "
////                           "Retrying connection with backoff and jitter." ) );
//                DS->pushToLogsMon(MQ->getName(), LOG_WARNING, "Connection to the broker failed");
//                DS->pushToLogsMon(MQ->getName(), LOG_INFO, "Retrying connection with backoff and jitter");
//
//                vTaskDelay( pdMS_TO_TICKS( usNextRetryBackOff ) );
//            }
//        }
//    } while( ( xNetworkStatus != TLS_TRANSPORT_SUCCESS ) && ( xBackoffAlgStatus == BackoffAlgorithmSuccess ) );
//
//    return xNetworkStatus;
//}
//
//static void prvCreateMQTTConnectionWithBroker( MQTTContext_t * pxMQTTContext,
//                                               NetworkContext_t * pxNetworkContext )
//{
//    MQTTStatus_t xResult;
//    MQTTConnectInfo_t xConnectInfo;
//    bool xSessionPresent;
//    TransportInterface_t xTransport;
//
//    /***
//     * For readability, error handling in this function is restricted to the use of
//     * asserts().
//     ***/
//
//    /* Fill in Transport Interface send and receive function pointers. */
//    xTransport.pNetworkContext = pxNetworkContext;
//    xTransport.send = TLS_FreeRTOS_send;
//    xTransport.recv = TLS_FreeRTOS_recv;
//    xTransport.writev = NULL;
//
//    /* Initialize MQTT library. */
//    xResult = MQTT_Init( pxMQTTContext, &xTransport, prvGetTimeMs, prvEventCallback, &xBuffer );
//    configASSERT( xResult == MQTTSuccess );
//    xResult = MQTT_InitStatefulQoS( pxMQTTContext,
//                                    pOutgoingPublishRecords,
//                                    mqttexampleOUTGOING_PUBLISH_RECORD_LEN,
//                                    pIncomingPublishRecords,
//                                    mqttexampleINCOMING_PUBLISH_RECORD_LEN );
//    configASSERT( xResult == MQTTSuccess );
//
//    /* Some fields are not used in this demo so start with everything at 0. */
//    ( void ) memset( ( void * ) &xConnectInfo, 0x00, sizeof( xConnectInfo ) );
//
//    /* Start with a clean session i.e. direct the MQTT broker to discard any
//     * previous session data. Also, establishing a connection with clean session
//     * will ensure that the broker does not store any data when this client
//     * gets disconnected. */
//    xConnectInfo.cleanSession = true;
//
//    /* The client identifier is used to uniquely identify this MQTT client to
//     * the MQTT broker. In a production device the identifier can be something
//     * unique, such as a device serial number. */
//    xConnectInfo.pClientIdentifier = configCLIENT_IDENTIFIER;
//    xConnectInfo.clientIdentifierLength = ( uint16_t ) strlen( configCLIENT_IDENTIFIER );
//
//    /* Set MQTT keep-alive period. If the application does not send packets at an interval less than
//     * the keep-alive period, the MQTT library will send PINGREQ packets. */
//    xConnectInfo.keepAliveSeconds = mqttexampleKEEP_ALIVE_TIMEOUT_SECONDS;
//
//    /* Send MQTT CONNECT packet to broker. LWT is not used in this demo, so it
//     * is passed as NULL. */
//    xResult = MQTT_Connect( pxMQTTContext,
//                            &xConnectInfo,
//                            NULL,
//                            mqttexampleCONNACK_RECV_TIMEOUT_MS,
//                            &xSessionPresent );
//    configASSERT( xResult == MQTTSuccess );
//
//    /* Successfully established and MQTT connection with the broker. */
////    LogInfo( ( "An MQTT connection is established with %s.", democonfigMQTT_BROKER_ENDPOINT ) );
//    DS->pushToLogsMon(MQ->getName(), LOG_INFO, "An MQTT connection is established with %s.", configMQTT_BROKER_ENDPOINT);
//}
//
//static void prvMQTTProcessResponse( MQTTPacketInfo_t * pxIncomingPacket,
//                                    uint16_t usPacketId )
//{
//    uint32_t ulTopicCount = 0U;
//
//    switch( pxIncomingPacket->type )
//    {
//        case MQTT_PACKET_TYPE_PUBACK:
////            LogInfo( ( "PUBACK received for packet ID %u.\r", usPacketId ) );
//            DS->pushToLogsMon(MQ->getName(), LOG_INFO, "PUBACK received for packet ID %u", usPacketId);
//
//            break;
//
//        case MQTT_PACKET_TYPE_SUBACK:
//
////            LogInfo( ( "SUBACK received for packet ID %u.", usPacketId ) );
//            DS->pushToLogsMon(MQ->getName(), LOG_INFO, "SUBACK received for packet ID %u.", usPacketId);
//
//            /* A SUBACK from the broker, containing the server response to our subscription request, has been received.
//             * It contains the status code indicating server approval/rejection for the subscription to the single topic
//             * requested. The SUBACK will be parsed to obtain the status code, and this status code will be stored in global
//             * variable #xTopicFilterContext. */
//            prvUpdateSubAckStatus( pxIncomingPacket );
//
//            for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ )
//            {
//                if( xTopicFilterContext[ ulTopicCount ].xSubAckStatus != MQTTSubAckFailure )
//                {
////                    LogInfo( ( "Subscribed to the topic %s with maximum QoS %u.\r",
////                               xTopicFilterContext[ ulTopicCount ].pcTopicFilter,
////                               xTopicFilterContext[ ulTopicCount ].xSubAckStatus ) );
//                    DS->pushToLogsMon(MQ->getName(), LOG_INFO, "Subscribed to the topic %s with maximum QoS %u",
//                            xTopicFilterContext[ ulTopicCount ].pcTopicFilter,
//                            xTopicFilterContext[ ulTopicCount ].xSubAckStatus);
//                }
//            }
//
//            /* Make sure ACK packet identifier matches with Request packet identifier. */
//            configASSERT( usSubscribePacketIdentifier == usPacketId );
//            break;
//
//        case MQTT_PACKET_TYPE_UNSUBACK:
////            LogInfo( ( "UNSUBACK received for packet ID %u.", usPacketId ) );
//            DS->pushToLogsMon(MQ->getName(), LOG_INFO, "UNSUBACK received for packet ID %u", usPacketId);
//
//            /* Make sure ACK packet identifier matches with Request packet identifier. */
//            configASSERT( usUnsubscribePacketIdentifier == usPacketId );
//            break;
//
//        case MQTT_PACKET_TYPE_PINGRESP:
//
//            /* Nothing to be done from application as library handles
//             * PINGRESP with the use of MQTT_ProcessLoop API function. */
////            LogWarn( ( "PINGRESP should not be handled by the application callback when using MQTT_ProcessLoop" ) );
//            DS->pushToLogsMon(MQ->getName(), LOG_WARNING, "PINGRESP should not be handled by the application callback when using MQTT_ProcessLoop");
//            break;
//
//        case MQTT_PACKET_TYPE_PUBREC:
////            LogInfo( ( "PUBREC received for packet id %u.",usPacketId ) );
//            DS->pushToLogsMon(MQ->getName(), LOG_INFO, "PUBREC received for packet id %u",usPacketId);
//            break;
//
//        case MQTT_PACKET_TYPE_PUBREL:
//
//            /* Nothing to be done from application as library handles
//             * PUBREL. */
////            LogInfo( ( "PUBREL received for packet id %u.",usPacketId ) );
//            DS->pushToLogsMon(MQ->getName(), LOG_INFO, "PUBREL received for packet id %u",usPacketId);
//            break;
//
//        case MQTT_PACKET_TYPE_PUBCOMP:
//
//            /* Nothing to be done from application as library handles
//             * PUBCOMP. */
////            LogInfo( ( "PUBCOMP received for packet id %u.",usPacketId ) );
//            DS->pushToLogsMon(MQ->getName(), LOG_INFO, "PUBCOMP received for packet id %u",usPacketId);
//            break;
//
//        /* Any other packet type is invalid. */
//        default:
////            LogWarn( ( "prvMQTTProcessResponse() called with unknown packet type:(%02X).\r",pxIncomingPacket->type ) );
//            DS->pushToLogsMon(MQ->getName(), LOG_WARNING, "prvMQTTProcessResponse() called with unknown packet type:(%02X)",pxIncomingPacket->type);
//    }
//}
//
//static void prvEventCallback( MQTTContext_t * pxMQTTContext,
//                              MQTTPacketInfo_t * pxPacketInfo,
//                              MQTTDeserializedInfo_t * pxDeserializedInfo )
//{
//    /* The MQTT context is not used in this function. */
//    ( void ) pxMQTTContext;
//
//    if( ( pxPacketInfo->type & 0xF0U ) == MQTT_PACKET_TYPE_PUBLISH )
//    {
////        LogInfo( ( "PUBLISH received for packet id %u.",
////                   pxDeserializedInfo->packetIdentifier ) );
//        DS->pushToLogsMon(MQ->getName(), LOG_INFO, "PUBLISH received for packet id %u",pxDeserializedInfo->packetIdentifier);
//
//        prvMQTTProcessIncomingPublish( pxDeserializedInfo->pPublishInfo );
//    }
//    else
//    {
//        prvMQTTProcessResponse( pxPacketInfo, pxDeserializedInfo->packetIdentifier );
//    }
//}
//
//static void prvMQTTProcessIncomingPublish( MQTTPublishInfo_t * pxPublishInfo )
//{
//    uint32_t ulTopicCount;
//    BaseType_t xTopicFound = pdFALSE;
//
//    configASSERT( pxPublishInfo != NULL );
//
//    /* Process incoming Publish. */
////    LogInfo( ( "Incoming QoS : %d", pxPublishInfo->qos ) );
//    DS->pushToLogsMon(MQ->getName(), LOG_INFO, "Incoming QoS : %d", pxPublishInfo->qos );
//
//    /* Verify the received publish is for one of the topics that's been subscribed to. */
//    for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ )
//    {
//        if( ( pxPublishInfo->topicNameLength == strlen( (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter ) ) &&
//            ( strncmp( (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter, pxPublishInfo->pTopicName, pxPublishInfo->topicNameLength ) == 0 ) )
//        {
//            xTopicFound = pdTRUE;
//            break;
//        }
//    }
//
//    if( xTopicFound == pdTRUE )
//    {
////        LogInfo( ( "\rIncoming Publish Topic Name: %.*s matches a subscribed topic.\r",
////                   pxPublishInfo->topicNameLength,
////                   pxPublishInfo->pTopicName ) );
//        DS->pushToLogsMon(MQ->getName(), LOG_INFO,
//			"Incoming Publish Topic Name: %.*s matches a subscribed topic %s",
//			pxPublishInfo->topicNameLength,
//			pxPublishInfo->pTopicName);
//    }
//    else
//    {
////        LogError( ( "Incoming Publish Topic Name: %.*s does not match a subscribed topic.\r",
////                    pxPublishInfo->topicNameLength,
////                    pxPublishInfo->pTopicName ) );
//        DS->pushToLogsMon(MQ->getName(), LOG_ERROR,
//			"Incoming Publish Topic Name: %.*s does not match a subscribed topic %s",
//			pxPublishInfo->topicNameLength,
//			pxPublishInfo->pTopicName);
//    }
//
//    /* Verify the message received matches the message sent. */
//    if( strncmp( mqttexampleMESSAGE, ( const char * ) ( pxPublishInfo->pPayload ), pxPublishInfo->payloadLength ) != 0 )
//    {
////        LogError( ( "Incoming Publish Message: %.*s does not match Expected Message: %s.\r",
////                    pxPublishInfo->topicNameLength,
////                    pxPublishInfo->pTopicName, mqttexampleMESSAGE ) );
//        DS->pushToLogsMon(MQ->getName(), LOG_ERROR,
//        		"Incoming Publish Message: %.*s does not match Expected Message: %s",
//                pxPublishInfo->topicNameLength,
//                pxPublishInfo->pTopicName, mqttexampleMESSAGE);
//    }
//}
//
//static void prvMQTTSubscribeWithBackoffRetries( MQTTContext_t * pxMQTTContext )
//{
//    MQTTStatus_t xResult = MQTTSuccess;
//    BackoffAlgorithmStatus_t xBackoffAlgStatus = BackoffAlgorithmSuccess;
//    BackoffAlgorithmContext_t xRetryParams;
//    uint16_t usNextRetryBackOff = 0U;
//    MQTTSubscribeInfo_t xMQTTSubscription[ mqttexampleTOPIC_COUNT ];
//    bool xFailedSubscribeToTopic = false;
//    uint32_t ulTopicCount = 0U;
//
//    /* Some fields not used by this demo so start with everything at 0. */
//    ( void ) memset( ( void * ) &xMQTTSubscription, 0x00, sizeof( xMQTTSubscription ) );
//
//    /* Get a unique packet id. */
//    usSubscribePacketIdentifier = MQTT_GetPacketId( pxMQTTContext );
//
//    /* Populate subscription list. */
//    for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ )
//    {
//        xMQTTSubscription[ ulTopicCount ].qos = MQTTQoS2;
//        xMQTTSubscription[ ulTopicCount ].pTopicFilter = (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter;
//        xMQTTSubscription[ ulTopicCount ].topicFilterLength = strlen( (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter );
//    }
//
//    /* Initialize context for backoff retry attempts if SUBSCRIBE request fails. */
//    BackoffAlgorithm_InitializeParams( &xRetryParams,
//                                       mqttexampleRETRY_BACKOFF_BASE_MS,
//                                       mqttexampleRETRY_MAX_BACKOFF_DELAY_MS,
//                                       mqttexampleRETRY_MAX_ATTEMPTS );
//
//    do
//    {
//        /* The client is now connected to the broker. Subscribe to the topic
//         * as specified in mqttexampleTOPIC at the top of this file by sending a
//         * subscribe packet then waiting for a subscribe acknowledgment (SUBACK).
//         * This client will then publish to the same topic it subscribed to, so it
//         * will expect all the messages it sends to the broker to be sent back to it
//         * from the broker. This demo uses QOS2 in Subscribe, therefore, the Publish
//         * messages received from the broker will have QOS2. */
//        xResult = MQTT_Subscribe( pxMQTTContext,
//                                  xMQTTSubscription,
//                                  sizeof( xMQTTSubscription ) / sizeof( MQTTSubscribeInfo_t ),
//                                  usSubscribePacketIdentifier );
//        configASSERT( xResult == MQTTSuccess );
//
//        for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ )
//        {
////            LogInfo( ( "SUBSCRIBE sent for topic %s to broker.",
////                       xTopicFilterContext[ ulTopicCount ].pcTopicFilter ) );
//            DS->pushToLogsMon(MQ->getName(), LOG_INFO, "SUBSCRIBE sent for topic %s to broker",
//                    xTopicFilterContext[ ulTopicCount ].pcTopicFilter);
//        }
//
//        /* Process incoming packet from the broker. After sending the subscribe, the
//         * client may receive a publish before it receives a subscribe ack. Therefore,
//         * call generic incoming packet processing function. Since this demo is
//         * subscribing to the topic to which no one is publishing, probability of
//         * receiving Publish message before subscribe ack is zero; but application
//         * must be ready to receive any packet.  This demo uses the generic packet
//         * processing function everywhere to highlight this fact. */
//        xResult = prvProcessLoopWithTimeout( pxMQTTContext, mqttexamplePROCESS_LOOP_TIMEOUT_MS );
//        configASSERT( xResult == MQTTSuccess );
//
//        /* Reset flag before checking suback responses. */
//        xFailedSubscribeToTopic = false;
//
//        /* Check if recent subscription request has been rejected. #xTopicFilterContext is updated
//         * in the event callback to reflect the status of the SUBACK sent by the broker. It represents
//         * either the QoS level granted by the server upon subscription, or acknowledgement of
//         * server rejection of the subscription request. */
//        for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ )
//        {
//            if( xTopicFilterContext[ ulTopicCount ].xSubAckStatus == MQTTSubAckFailure )
//            {
//                xFailedSubscribeToTopic = true;
//
//                /* Generate a random number and calculate backoff value (in milliseconds) for
//                 * the next connection retry.
//                 * Note: It is recommended to seed the random number generator with a device-specific
//                 * entropy source so that possibility of multiple devices retrying failed network operations
//                 * at similar intervals can be avoided. */
//                xBackoffAlgStatus = BackoffAlgorithm_GetNextBackoff( &xRetryParams, uxRand(), &usNextRetryBackOff );
//
//                if( xBackoffAlgStatus == BackoffAlgorithmRetriesExhausted )
//                {
////                    LogError( ( "Server rejected subscription request. All retry attempts have exhausted. Topic=%s",
////                                xTopicFilterContext[ ulTopicCount ].pcTopicFilter ) );
//                    DS->pushToLogsMon(MQ->getName(), LOG_INFO,
//                    		"Server rejected subscription request. All retry attempts have exhausted. Topic=%s",
//                            xTopicFilterContext[ ulTopicCount ].pcTopicFilter);
//                }
//                else if( xBackoffAlgStatus == BackoffAlgorithmSuccess )
//                {
////                    LogWarn( ( "Server rejected subscription request. Attempting to re-subscribe to topic %s.",
////                               xTopicFilterContext[ ulTopicCount ].pcTopicFilter ) );
//                    DS->pushToLogsMon(MQ->getName(), LOG_INFO,
//                    		"Server rejected subscription request. Attempting to re-subscribe to topic %s.",
//                            xTopicFilterContext[ ulTopicCount ].pcTopicFilter);
//
//                    /* Backoff before the next re-subscribe attempt. */
//                    vTaskDelay( pdMS_TO_TICKS( usNextRetryBackOff ) );
//                }
//
//                break;
//            }
//        }
//
//        configASSERT( xBackoffAlgStatus != BackoffAlgorithmRetriesExhausted );
//    } while( ( xFailedSubscribeToTopic == true ) && ( xBackoffAlgStatus == BackoffAlgorithmSuccess ) );
//}
//static void prvMQTTPublishToTopics( MQTTContext_t * pxMQTTContext )
//{
//    MQTTStatus_t xResult;
//    MQTTPublishInfo_t xMQTTPublishInfo;
//    uint32_t ulTopicCount;
//
//    /***
//     * For readability, error handling in this function is restricted to the use of
//     * asserts().
//     ***/
//
//    for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ )
//    {
//        /* Some fields are not used by this demo so start with everything at 0. */
//        ( void ) memset( ( void * ) &xMQTTPublishInfo, 0x00, sizeof( xMQTTPublishInfo ) );
//
//        /* This demo uses QoS2 */
//        xMQTTPublishInfo.qos = MQTTQoS2;
//        xMQTTPublishInfo.retain = false;
//        xMQTTPublishInfo.pTopicName = (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter;
//        xMQTTPublishInfo.topicNameLength = strlen( ( char* ) xTopicFilterContext[ ulTopicCount ].pcTopicFilter );
//        xMQTTPublishInfo.pPayload = mqttexampleMESSAGE;
//        xMQTTPublishInfo.payloadLength = strlen( mqttexampleMESSAGE );
//
//        /* Get a unique packet id. */
//        usPublishPacketIdentifier = MQTT_GetPacketId( pxMQTTContext );
//
////        LogInfo( ( "Publishing to the MQTT topic %s.\r", xTopicFilterContext[ ulTopicCount ].pcTopicFilter ) );
//        DS->pushToLogsMon(MQ->getName(), LOG_INFO, "Publishing to the MQTT topic %s", xTopicFilterContext[ ulTopicCount ].pcTopicFilter);
//        /* Send PUBLISH packet. */
//        xResult = MQTT_Publish( pxMQTTContext, &xMQTTPublishInfo, usPublishPacketIdentifier );
//        configASSERT( xResult == MQTTSuccess );
//    }
//}
//
//static void prvMQTTUnsubscribeFromTopics( MQTTContext_t * pxMQTTContext )
//{
//    MQTTStatus_t xResult;
//    MQTTSubscribeInfo_t xMQTTSubscription[ mqttexampleTOPIC_COUNT ];
//    uint32_t ulTopicCount;
//
//    /* Some fields are not used by this demo so start with everything at 0. */
//    memset( ( void * ) &xMQTTSubscription, 0x00, sizeof( xMQTTSubscription ) );
//
//    /* Populate subscription list. */
//    for( ulTopicCount = 0; ulTopicCount < mqttexampleTOPIC_COUNT; ulTopicCount++ )
//    {
//        xMQTTSubscription[ ulTopicCount ].qos = MQTTQoS2;
//        xMQTTSubscription[ ulTopicCount ].pTopicFilter = (char*) xTopicFilterContext[ ulTopicCount ].pcTopicFilter;
//        xMQTTSubscription[ ulTopicCount ].topicFilterLength = strlen( (char*)xTopicFilterContext[ ulTopicCount ].pcTopicFilter );
//
////        LogInfo( ( "Unsubscribing from topic %s.\r", xTopicFilterContext[ ulTopicCount ].pcTopicFilter ) );
//        DS->pushToLogsMon(MQ->getName(), LOG_INFO, "Unsubscribing from topic %s", xTopicFilterContext[ ulTopicCount ].pcTopicFilter);
//    }
//
//    /* Get next unique packet identifier. */
//    usUnsubscribePacketIdentifier = MQTT_GetPacketId( pxMQTTContext );
//    /* Make sure the packet id obtained is valid. */
//    configASSERT( usUnsubscribePacketIdentifier != 0 );
//
//    /* Send UNSUBSCRIBE packet. */
//    xResult = MQTT_Unsubscribe( pxMQTTContext,
//                                xMQTTSubscription,
//                                sizeof( xMQTTSubscription ) / sizeof( MQTTSubscribeInfo_t ),
//                                usUnsubscribePacketIdentifier );
//
//    configASSERT( xResult == MQTTSuccess );
//}

char* getTopicName(MQTTDeserializedInfo_t * pxDeserializedInfo) {
	char *name = (char*)pvPortMalloc(((pxDeserializedInfo->pPublishInfo->topicNameLength+1) * sizeof(char)));

	snprintf(name, pxDeserializedInfo->pPublishInfo->topicNameLength+1, pxDeserializedInfo->pPublishInfo->pTopicName );
//	name[pxDeserializedInfo->pPublishInfo->topicNameLength+1] = '\0';

	return name;
}


char* getMessage(MQTTDeserializedInfo_t * pxDeserializedInfo) {
//	char *message = (char*)pvPortMalloc((LOG_LENGTH * sizeof(char)));

	const char* json = (const char*)(pxDeserializedInfo->pPublishInfo->pPayload);
	uint16_t length = pxDeserializedInfo->pPublishInfo->payloadLength;

	char *message = (char*)pvPortMalloc((length * sizeof(char)));

	snprintf(message,length,json);

//	size_t start = 0, next = 0;
//	JSONPair_t pair = { 0 };
//	JSONStatus_t result;
//
//	const char* json = (const char*)(pxDeserializedInfo->pPublishInfo->pPayload);
//	uint16_t length = pxDeserializedInfo->pPublishInfo->payloadLength;
//
//	result = JSON_Validate( json, length );
//	if( result == JSONSuccess )
//	{
//		result = JSON_Iterate( json, length, &start, &next, &pair );
//	}
//
//	uint16_t index = 0;
//	while( result == JSONSuccess )
//	{
//		if( pair.key != NULL )
//		{
//			uint8_t lengthCopy = pair.keyLength + pair.valueLength;
//
//			if(pair.jsonType == JSONString)
//				snprintf( message+index, lengthCopy, "%s: %s ", pair.key, pair.value );
//			else if(pair.jsonType == JSONNumber)
//				snprintf( message+index, lengthCopy, "%s: %d ", pair.key, pair.value );
//
//			index = index + lengthCopy + 4;
//
//			if(index >= LOG_LENGTH)
//				break;
//		}
//
////		printf( "value: (%s) %.*s", json_types[ pair.jsonType ],
////			 ( int ) pair.valueLength, pair.value );
//
//		result = JSON_Iterate( json, length, &start, &next, &pair );
//	}

	return message;
}

/* This is the callback function invoked by the MQTT stack when it receives
 * incoming messages. This function demonstrates how to use the Jobs_MatchTopic
 * function to determine whether the incoming message is a Jobs message
 * or not. If it is, it handles the message depending on the message type.
 */
static void prvEventCallback( MQTTContext_t * pxMqttContext, MQTTPacketInfo_t * pxPacketInfo, MQTTDeserializedInfo_t * pxDeserializedInfo )
{
    uint16_t usPacketIdentifier;

    ( void ) pxMqttContext;

    configASSERT( pxDeserializedInfo != NULL );
    configASSERT( pxMqttContext != NULL );
    configASSERT( pxPacketInfo != NULL );

    usPacketIdentifier = pxDeserializedInfo->packetIdentifier;

    /* Handle incoming publish. The lower 4 bits of the publish packet
     * type is used for the dup, QoS, and retain flags. Hence masking
     * out the lower bits to check if the packet is publish. */
    if( ( pxPacketInfo->type & 0xF0U ) == MQTT_PACKET_TYPE_PUBLISH )
    {
        configASSERT( pxDeserializedInfo->pPublishInfo != NULL );

//        DS->pushToLogsMon(MQ->getName(), LOG_DEBUG,
//			"Received an incoming publish message on topic: %s length: %d",
//			( const char * ) pxDeserializedInfo->pPublishInfo->pTopicName,
//			pxDeserializedInfo->pPublishInfo->topicNameLength
//		);

	//    const char * pTopic = "topic/match/1";
	//    const char * pFilter = "topic/#";
		const char * pTopic = "toxic/";
		const char * pFilter = "#";
		MQTTStatus_t mStatus = MQTTSuccess;
		bool match = false;

		mStatus = MQTT_MatchTopic(pTopic, strlen( pTopic ), pFilter, strlen( pFilter ), &match);

		if( mStatus == MQTTSuccess ) {
			if( match )
			{
				char *name = getTopicName(pxDeserializedInfo);
				char *message = getMessage(pxDeserializedInfo);

//				DS->pushToLogsMon(MQ->getName(), LOG_DEBUG, "Incoming mqtt received matching global topic");
//				DS->pushToLogsMon(name, LOG_INFO, "Size of message: %d", pxDeserializedInfo->pPublishInfo->payloadLength);
				DS->pushToLogsMon(name, LOG_INFO, message);
			}
			else {
				DS->pushToLogsMon(MQ->getName(), LOG_DEBUG, "Incoming mqtt received NOT matching global topic");
				DS->pushToLogsMon(getTopicName(pxDeserializedInfo), LOG_INFO, "Size of message: %d", pxDeserializedInfo->pPublishInfo->payloadLength);
			}
		}
		else {
//			/* Let the Jobs library tell us whether this is a Jobs message. */
//	        JobsTopic_t topicType = JobsMaxTopic;
//	        JobsStatus_t xStatus = JobsError;
//
//			xStatus = Jobs_MatchTopic( ( char * ) pxDeserializedInfo->pPublishInfo->pTopicName,
//									   pxDeserializedInfo->pPublishInfo->topicNameLength,
//									   configTHING_NAME,
//									   THING_NAME_LENGTH,
//									   &topicType,
//									   NULL,
//									   NULL );
//
//			if( xStatus == JobsSuccess )
//			{
//				/* Upon successful return, the messageType has been filled in. */
//				if( ( topicType == JobsDescribeSuccess ) || ( topicType == JobsNextJobChanged ) )
//				{
//					MQTTPublishInfo_t * pxJobMessagePublishInfo = NULL;
//					char * pcTopicName = NULL;
//					char * pcPayload = NULL;
//
//					/* Copy message to pass into queue. */
//					pxJobMessagePublishInfo = ( MQTTPublishInfo_t * ) pvPortMalloc( sizeof( MQTTPublishInfo_t ) );
//					pcTopicName = ( char * ) pvPortMalloc( pxDeserializedInfo->pPublishInfo->topicNameLength );
//					pcPayload = ( char * ) pvPortMalloc( pxDeserializedInfo->pPublishInfo->payloadLength );
//
//					if( ( pxJobMessagePublishInfo == NULL ) || ( pcTopicName == NULL ) || ( pcPayload == NULL ) )
//					{
//						DS->pushToLogsMon(MQ->getName(), LOG_ERROR, "Malloc failed for copying job publish info." );
//	//                    LogError( ( "Malloc failed for copying job publish info." ) );
//
//						if( pxJobMessagePublishInfo != NULL )
//						{
//							vPortFree( pxJobMessagePublishInfo );
//						}
//
//						if( pcTopicName != NULL )
//						{
//							vPortFree( pcTopicName );
//						}
//
//						if( pcPayload != NULL )
//						{
//							vPortFree( pcPayload );
//						}
//					}
//					else
//					{
//						memcpy( pxJobMessagePublishInfo, pxDeserializedInfo->pPublishInfo, sizeof( MQTTPublishInfo_t ) );
//						memcpy( pcTopicName, pxDeserializedInfo->pPublishInfo->pTopicName, pxDeserializedInfo->pPublishInfo->topicNameLength );
//						memcpy( pcPayload, pxDeserializedInfo->pPublishInfo->pPayload, pxDeserializedInfo->pPublishInfo->payloadLength );
//
//						pxJobMessagePublishInfo->pTopicName = pcTopicName;
//						pxJobMessagePublishInfo->pPayload = pcPayload;
//
//						if( xQueueSend( xJobMessageQueue, &pxJobMessagePublishInfo, 0 ) == errQUEUE_FULL )
//						{
//							DS->pushToLogsMon(MQ->getName(), LOG_ERROR, "Could not enqueue Jobs message." );
//	//                        LogError( ( "Could not enqueue Jobs message." ) );
//
//							vPortFree( pxJobMessagePublishInfo );
//							vPortFree( pcTopicName );
//							vPortFree( pcPayload );
//						}
//					}
//				}
//				else if( topicType == JobsUpdateSuccess )
//				{
//					DS->pushToLogsMon(MQ->getName(), LOG_INFO, "Job update status request has been accepted by AWS Iot Jobs service." );
//	//                LogInfo( ( "Job update status request has been accepted by AWS Iot Jobs service." ) );
//				}
//				else if( topicType == JobsStartNextFailed )
//				{
//					DS->pushToLogsMon(MQ->getName(), LOG_WARN, "Request for next job description rejected: RejectedResponse=%.*s.",
//							pxDeserializedInfo->pPublishInfo->payloadLength,
//							( const char * ) pxDeserializedInfo->pPublishInfo->pPayload );
//
//	//                LogWarn( ( "Request for next job description rejected: RejectedResponse=%.*s.",
//	//                           pxDeserializedInfo->pPublishInfo->payloadLength,
//	//                           ( const char * ) pxDeserializedInfo->pPublishInfo->pPayload ) );
//				}
//				else if( topicType == JobsUpdateFailed )
//				{
//					/* Set the global flag to terminate the demo, because the request for updating and executing the job status
//					 * has been rejected by the AWS IoT Jobs service. */
//					xDemoEncounteredError = pdTRUE;
//
//					DS->pushToLogsMon(MQ->getName(), LOG_WARN, "Request for job update rejected: RejectedResponse=%.*s.",
//							pxDeserializedInfo->pPublishInfo->payloadLength,
//							( const char * ) pxDeserializedInfo->pPublishInfo->pPayload );
//	//                LogWarn( ( "Request for job update rejected: RejectedResponse=%.*s.",
//	//                           pxDeserializedInfo->pPublishInfo->payloadLength,
//	//                           ( const char * ) pxDeserializedInfo->pPublishInfo->pPayload ) );
//
//					DS->pushToLogsMon(MQ->getName(), LOG_ERROR, "Terminating demo as request to update job status has been rejected by "
//							"AWS IoT Jobs service..." );
//	//                LogError( ( "Terminating demo as request to update job status has been rejected by "
//	//                            "AWS IoT Jobs service..." ) );
//				}
//				else
//				{
//					DS->pushToLogsMon(MQ->getName(), LOG_WARN, "Received an unexpected messages from AWS IoT Jobs service: "
//							"JobsTopicType=%u", topicType );
//	//                LogWarn( ( "Received an unexpected messages from AWS IoT Jobs service: "
//	//                           "JobsTopicType=%u", topicType ) );
//				}
//			}
//			else if( xStatus == JobsNoMatch )
//			{
//				DS->pushToLogsMon(MQ->getName(), LOG_WARN, "Incoming message topic does not belong to AWS IoT Jobs!: topic=%.*s",
//						pxDeserializedInfo->pPublishInfo->topicNameLength,
//						( const char * ) pxDeserializedInfo->pPublishInfo->pTopicName );
//	//            LogWarn( ( "Incoming message topic does not belong to AWS IoT Jobs!: topic=%.*s",
//	//                       pxDeserializedInfo->pPublishInfo->topicNameLength,
//	//                       ( const char * ) pxDeserializedInfo->pPublishInfo->pTopicName ) );
//			}
//			else
//			{
//				DS->pushToLogsMon(MQ->getName(), LOG_ERROR, "Failed to parse incoming publish job. Topic=%.*s!",
//						pxDeserializedInfo->pPublishInfo->topicNameLength,
//						( const char * ) pxDeserializedInfo->pPublishInfo->pTopicName );
//	//            LogError( ( "Failed to parse incoming publish job. Topic=%.*s!",
//	//                        pxDeserializedInfo->pPublishInfo->topicNameLength,
//	//                        ( const char * ) pxDeserializedInfo->pPublishInfo->pTopicName ) );
//			}
		}
    }
	else
	{
		vHandleOtherIncomingPacket( pxPacketInfo, usPacketIdentifier );
	}
}


//=====================================================================================================
//
//=====================================================================================================

int MQTTServices::iMQ = 0;
MQTTServices *MQTTServices::instance=NULL;
char* MQTTServices::name = (char*)pvPortMalloc(CAT_LENGTH * sizeof(char));

void MQTTServices::blinkLED(uint8_t times) {
	for(uint8_t i=0; i<=times; i++) {
		BSP_LED_Toggle(LED);
		osDelay(100);
	}
}

void MQTTServices::init() {
//	client = mqtt_client_new();

	snprintf(log, LOG_LENGTH, "MQTT Client initialized and ready to be configured");
	DS->pushToLogsMon(name, LOG_INFO, log);

//	snprintf(log, LOG_LENGTH, "Initialization completed");
//	DS->pushToLogsMon(name, LOG_OK, log);

//	Connect();
}

char* MQTTServices::getName() {
	return name;
}

bool MQTTServices::IsConnected() {
	bool retour = false;

	retour = connected;
//	retour = (connected) ? true : false;
////	retour = (connected && mpappGetStatus_ETH() == NET_OK) ? true : false;
//	retour = (mqtt_client_is_connected(client) == 1 ) ? true : false;

	return(retour);
}

int MQTTServices::Connect() {
	int retour = 0;

	snprintf(log,LOG_LENGTH, "connecting to services...");
	DS->pushToLogsMon(name, LOG_INFO, log);

	mqtt_connect();

	if(mpappGetStatus_CLOUD() == CLOUD_OK) {
		snprintf(log,LOG_LENGTH, "connected to services");
		DS->pushToLogsMon(name, LOG_OK, log);
		connected = true;
	}
	else {
		snprintf(log,LOG_LENGTH, "NOT connected to services");
		DS->pushToLogsMon(name, LOG_ERROR, log);
		connected = false;
	}

	return(retour);
}

int MQTTServices::Disconnect() {
	int retour = 0;

//	mqtt_disconnect(client);
	connected = false;
//	start = false;
	SetStatus_CLOUD(CLOUD_NOTOK);

	snprintf(log, LOG_LENGTH, "disconnected from services, status cloud is %d", CLOUD_NOTOK);
	DS->pushToLogsMon(name, LOG_INFO, log);

	return(retour);
}


/**
 * This demo uses helper functions for MQTT operations that have internal
 * loops to process incoming messages. Those are not the focus of this demo
 * and therefore, are placed in a separate file mqtt_demo_helpers.c.
 *
 * This function also shows that the communication with the AWS IoT Jobs services does
 * not require explicit subscriptions to the response MQTT topics for request commands that
 * sent to the MQTT APIs (like DescribeJobExecution API) of the service. The service
 * will send messages on the response topics for the request commands on the same
 * MQTT connection irrespective of whether the client subscribes to the response topics.
 * Therefore, this demo processes incoming messages from response topics of DescribeJobExecution
 * and UpdateJobExecution APIs without explicitly subscribing to the topics.
 */
void MQTTServices::mqtt_connect()
{
	uint32_t tickstart = HAL_GetTick();

	BaseType_t xDemoStatus = pdPASS;
    UBaseType_t uxDemoRunCount = 0UL;
    BaseType_t retryDemoLoop = pdFALSE;

    /* Set the pParams member of the network context with desired transport. */
    xNetworkContext.pParams = &xTlsTransportParams;

    /* Initialize Jobs message queue. */
    xJobMessageQueue = xQueueCreate( JOBS_MESSAGE_QUEUE_LEN, sizeof( MQTTPublishInfo_t * ) );
    configASSERT( xJobMessageQueue != NULL );

//	uint32_t ulPublishCount = 0U;
//	uint32_t ulTopicCount = 0U;
//	const uint32_t ulMaxPublishCount = 5UL;
//	NetworkContext_t xNetworkContext = { 0 };
//    TlsTransportParams_t xTlsTransportParams = { 0 };
//    NetworkCredentials_t xNetworkCredentials = { 0 };
//    MQTTContext_t xMQTTContext = { 0 };
//    MQTTStatus_t xMQTTStatus;
//    MQTTFixedBuffer_t xMQTTBuffer;
//    TlsTransportStatus_t xNetworkStatus;

//    /* Remove compiler warnings about unused parameters. */
//    ( void ) pvParameters;
//
//    /* Set the pParams member of the network context with desired transport. */
//    xNetworkContext.pParams = &xTlsTransportParams;
//
//    /* Set the entry time of the demo application. This entry time will be used
//     * to calculate relative time elapsed in the execution of the demo application,
//     * by the timer utility function that is provided to the MQTT library.
//     */
    ulGlobalEntryTimeMs = prvGetTimeMs();

	while( !stop )
	{
    	if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
			MQ->blinkLED(4);
			tickstart = HAL_GetTick();
		}
//
//        if( xPlatformIsNetworkUp() == pdFALSE )
//        {
//            LogInfo( ( "Waiting for the network link up event..." ) );
//
//            while( xPlatformIsNetworkUp() == pdFALSE )
//            {
//                vTaskDelay( pdMS_TO_TICKS( 1000U ) );
//            }
//        }

        /* Establish an MQTT connection with AWS IoT over a mutually authenticated TLS session. */
        xDemoStatus = xEstablishMqttSession( (MQTTContext_t*)&xMqttContext, (NetworkContext_t*)&xNetworkContext, (MQTTFixedBuffer_t*)&xBuffer, (MQTTEventCallback_t)prvEventCallback );

        if( xDemoStatus == pdFAIL )
        {
            /* Log error to indicate connection failure. */
//            LogError( ( "Failed to connect to AWS IoT broker." ) );
            DS->pushToLogsMon(name, LOG_ERROR, "Failed to connect to AWS IoT broker." );
            SetStatus_CLOUD(CLOUD_NOTOK);
        }
        else
        {
        	DS->pushToLogsMon(name, LOG_OK, "Connected to AWS IoT broker." );
        	SetStatus_CLOUD(CLOUD_OK);

            /* Print out a short user guide to the console. The default logging
             * limit of 255 characters can be changed in demo_logging.c, but breaking
             * up the only instance of a 1000+ character string is more practical. */
//            LogInfo( ( "\r"
//                       "/*-----------------------------------------------------------*/\r"
//                       "\r"
//                       "The Jobs demo is now ready to accept Jobs.\r"
//                       "Jobs may be created using the AWS IoT console or AWS CLI.\r"
//                       "See the following link for more information.\r" ) );
//            LogInfo( ( "\r"
//        	"https://awscli.amazonaws.com/v2/documentation/api/latest/reference/iot/create-job.html\r"
//                       "\r"
//                       "This demo expects Job documents to have an \"action\" JSON key.\r"
//                       "The following actions are currently supported:\r" ) );
//            LogInfo( ( "\r"
//                       " - print          \r"
//                       "   Logs a message to the local console. The Job document must also contain a \"message\".\r"
//                       "   For example: { \"action\": \"print\", \"message\": \"Hello world!\"} will cause\r"
//                       "   \"Hello world!\" to be printed on the console.\r" ) );
//            LogInfo( ( "\r"
//                       " - publish        \r"
//                       "   Publishes a message to an MQTT topic. The Job document must also contain a \"message\" and \"topic\".\r" ) );
//            LogInfo( ( "\r"
//                       "   For example: { \"action\": \"publish\", \"topic\": \"demo/jobs\", \"message\": \"Hello world!\"} will cause\r"
//                       "   \"Hello world!\" to be published to the topic \"demo/jobs\".\r" ) );
//            LogInfo( ( "\r"
//                       " - exit           \r"
//                       "   Exits the demo program. This program will run until { \"action\": \"exit\" } is received.\r"
//                       "\r"
//                       "/*-----------------------------------------------------------*/\r" ) );

            /* Subscribe to the NextJobExecutionChanged API topic to receive notifications about the next pending
             * job in the queue for the Thing resource used by this demo. */
            if( xSubscribeToTopic( &xMqttContext, mqttTOPIC, sizeof( mqttTOPIC ) - 1 ) != pdPASS )
            {
                xDemoStatus = pdFAIL;
                DS->pushToLogsMon(name, LOG_ERROR, "Failed to subscribe");// to: %s", mqttTOPIC );
            }
            else {
            	DS->pushToLogsMon(name, LOG_OK, "Success to subscribe");// to: %s", mqttTOPIC );
            }
        }

        if( xDemoStatus == pdPASS )
        {
//            /* Publish to AWS IoT Jobs on the DescribeJobExecution API to request the next pending job.
//             *
//             * Note: It is not required to make MQTT subscriptions to the response topics of the
//             * DescribeJobExecution API because the AWS IoT Jobs service sends responses for
//             * the PUBLISH commands on the same MQTT connection irrespective of whether the client has subscribed
//             * to the response topics or not.
//             * This demo processes incoming messages from the response topics of the API in the prvEventCallback()
//             * handler that is supplied to the coreMQTT library. */
//            if( xPublishToTopic( &xMqttContext,
//                                 DESCRIBE_NEXT_JOB_TOPIC( configTHING_NAME ),
//                                 sizeof( DESCRIBE_NEXT_JOB_TOPIC( configTHING_NAME ) ) - 1,
//                                 NULL,
//                                 0 ) != pdPASS )
//            {
//                xDemoStatus = pdFAIL;
//                LogError( ( "Failed to publish to DescribeJobExecution API of AWS IoT Jobs service: "
//                            "Topic=%s", DESCRIBE_NEXT_JOB_TOPIC( democonfigTHING_NAME ) ) );
//            }
        }
//
        /* Keep on running the demo until we receive a job for the "exit" action to exit the demo. */
        while( ( xExitActionJobReceived == pdFALSE ) &&
               ( xDemoEncounteredError == pdFALSE ) &&
               ( xDemoStatus == pdPASS ) )
        {
            MQTTPublishInfo_t * pxJobMessagePublishInfo;
            MQTTStatus_t xMqttStatus = MQTTSuccess;

            /* Check if we have notification for the next pending job in the queue from the
             * NextJobExecutionChanged API of the AWS IoT Jobs service. */
            xMqttStatus = MQTT_ProcessLoop( &xMqttContext );

            /* Receive any incoming Jobs message. */
            if( xQueueReceive( xJobMessageQueue, &pxJobMessagePublishInfo, 0 ) == pdTRUE )
            {
                /* Handler function to process Jobs message payload. */
                prvNextJobHandler( pxJobMessagePublishInfo );
                vPortFree( (void*)(pxJobMessagePublishInfo->pTopicName) );
                vPortFree( (void*)(pxJobMessagePublishInfo->pPayload) );
                vPortFree( pxJobMessagePublishInfo );
            }

            if( xMqttStatus != MQTTSuccess )
            {
                xDemoStatus = pdFAIL;
//                LogError( ( "Failed to receive notification about next pending job: "
//                            "MQTT_ProcessLoop failed" ) );
                DS->pushToLogsMon(name, LOG_ERROR, "Failed to receive notification about next pending job: MQTT_ProcessLoop failed");
            }
        }
//
//        /* Increment the demo run count. */
//        uxDemoRunCount++;
//
//        /* Retry demo loop only if there is a failure before completing
//         * the processing of any pending jobs. Any failure in MQTT unsubscribe
//         * or disconnect is considered a failure in demo execution and retry
//         * will not be attempted since a retry without any pending jobs will
//         * make this demo indefinitely wait. */
//        if( ( xDemoStatus == pdFAIL ) || ( xDemoEncounteredError == pdTRUE ) )
//        {
//            if( uxDemoRunCount < JOBS_MAX_DEMO_LOOP_COUNT )
//            {
//                LogWarn( ( "Demo iteration %lu failed. Retrying...", uxDemoRunCount ) );
//                retryDemoLoop = pdTRUE;
//            }
//            else
//            {
//                LogError( ( "All %d demo iterations failed.", JOBS_MAX_DEMO_LOOP_COUNT ) );
//                retryDemoLoop = pdFALSE;
//            }
//        }
//        else
//        {
//            /* Reset the flag for demo retry. */
//            retryDemoLoop = pdFALSE;
//        }

        /* Unsubscribe from the NextJobExecutionChanged API topic. */
        if( xUnsubscribeFromTopic( &xMqttContext,
                                   NEXT_JOB_EXECUTION_CHANGED_TOPIC( configTHING_NAME ),
                                   sizeof( NEXT_JOB_EXECUTION_CHANGED_TOPIC( configTHING_NAME ) ) - 1 ) != pdPASS )
        {
            xDemoStatus = pdFAIL;
            DS->pushToLogsMon(name, LOG_ERROR, "Failed to subscribe unsubscribe from the NextJobExecutionChanged API of AWS IoT Jobs service"); //: Topic=%s", NEXT_JOB_EXECUTION_CHANGED_TOPIC( democonfigTHING_NAME ) );
//            LogError( ( "Failed to subscribe unsubscribe from the NextJobExecutionChanged API of AWS IoT Jobs service: "
//                        "Topic=%s", NEXT_JOB_EXECUTION_CHANGED_TOPIC( democonfigTHING_NAME ) ) );

        }

        /* Disconnect the MQTT and network connections with AWS IoT. */
        if( xDisconnectMqttSession( &xMqttContext, &xNetworkContext ) != pdPASS )
        {
            xDemoStatus = pdFAIL;
//            LogError( ( "Disconnection from AWS IoT failed..." ) );
            DS->pushToLogsMon(name, LOG_ERROR, "Disconnection from AWS IoT failed...");
        }

//        /* Add a delay if a retry is required. */
//        if( retryDemoLoop == pdTRUE )
//        {
//            /* Clear the flag that indicates that indicates the demo error
//             * before attempting a retry. */
//            xDemoEncounteredError = pdFALSE;
//
//            LogInfo( ( "A short delay before the next demo iteration." ) );
//            vTaskDelay( DELAY_BETWEEN_DEMO_RETRY_ITERATIONS_TICKS );
//        }
	}
}

//=====================================================================================================
//
//=====================================================================================================

void MQTTServices::Start() {
	start = true;
	  snprintf(log,LOG_LENGTH, "Start command received");
	  DS->pushToLogsMon(name, LOG_INFO, log);
}

void MQTTServices::Stop() {
	stop = true;
	snprintf(log,LOG_LENGTH, "Stop command received");
	DS->pushToLogsMon(name, LOG_INFO, log);
}

//=====================================================================================================
//
//=====================================================================================================

MQTTServices *MQ = MQTTServices::CreateInstance();

//=====================================================================================================
//
//=====================================================================================================



void StartMQTTServices(void *argument) {
	uint32_t tickstart = HAL_GetTick();

	MQ->init();

	for( ; ; )
	{
		if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
			MQ->blinkLED(2);
			tickstart = HAL_GetTick();
		}

		if(MQ->start) {
			MQ->Connect();
			MQ->start = false;
		}

		if( MQ->stop ) {
			MQ->Disconnect();
			MQ->stop = false;
		}

//		osDelay(100);
	}
}

int mbedtls_hardware_poll( void *data, unsigned char *output, size_t len, size_t *olen ) {

}

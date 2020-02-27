// ----------------------------------------------------------------------------
// Copyright (c) 2018 Semiconductor Components Industries LLC
// (d/b/a "ON Semiconductor").  All rights reserved.
// This software and/or documentation is licensed by ON Semiconductor under
// limited terms and conditions.  The terms and conditions pertaining to the
// software and/or documentation are available at
// http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf ("ON Semiconductor Standard
// Terms and Conditions of Sale, Section 8 Software") and if applicable the
// software license agreement.  Do not use this software and/or documentation
// unless you have carefully read and you agree to the limited terms and
// conditions.  By using this software and/or documentation, you agree to the
// limited terms and conditions.
// ----------------------------------------------------------------------------

#ifndef _CS_H_
#define _CS_H_


//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include <stddef.h>
#include <stdint.h>

#include "RTE_CS_Feature.h"

#include <ics/CS_Log.h>

#ifdef __cplusplus
extern "C"
{
#endif

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

/** Maximum number of data bytes that can be fit inside of a response packet. */
#define CS_MAX_RESPONSE_LENGTH         (18)

//-----------------------------------------------------------------------------
// EXPORTED DATA TYPES DEFINITION
//-----------------------------------------------------------------------------

enum CS_PowerMode
{
    /** \brief Enter normal operation mode.
     *
     * Node does not need to implement this if it can be activated on-demand
     * when request handler is called.
     */
    CS_POWER_MODE_NORMAL = 0,

    /** \brief Enter low power mode.
     *
     * Disable all external sensors, etc.
     * Nodes will be usually set to sleep mode when connected BLE central device
     * disconnects.
     *
     * Nodes have to be able to automatically exit sleep mode when its request
     * handler is called.
     */
    CS_POWER_MODE_SLEEP  = 1,
};

/** \brief Structure passed to nodes via request handler.
 *
 * Request handler must not modify values of any member pointers.
 */
struct CS_Request_Struct
{
	/** \brief Pointer to zero terminated string containing request token. */
	const char* token;

	/** \brief Pointer to zero terminated string containing node name. */
	const char* node;

	/** \brief Pointer to zero terminated string containing node property
	 * name.
	 */
	const char* property;

	/** \brief Pointer to zero terminated string containing new property value
	 * of write request or NULL for read requests.
	 */
	const char* property_value;
};

/** \brief Function prototype for node request handler.
 *
 * \param[in] request
 * Details of the request passed to the node.
 *
 * \param[out] response
 * Pointer to array where node can print its response to request.
 * It does not contain request token.
 * Maximum number of bytes that can be written to this array is
 * \ref CS_MAX_RESPONSE_LENGTH excluding the terminating character.
 *
 * \returns CS_OK when request was successfully processed and response contains
 * valid zero terminated string.
 * \returns CS_ERROR when request processing failed or response does not contain
 * valid string.
 * \returns CS_NO_RESPONSE when request was processed but no response is to be
 * send back to connected device.
 */
typedef int (*CS_RequestHandler)(const struct CS_Request_Struct* request, char* response);

/** \brief Function prototype for node power manager function.
 *
 * \param[in] mode
 * New desired power mode level.
 *
 * \returns CS_OK on success.
 */
typedef int (*CS_PowerModeHandler)(enum CS_PowerMode mode);

/** \brief Function prototype for node poll function that is to be executed
 * from main loop.
 */
typedef void (*CS_PollHandler)(void);

struct CS_Node_Struct
{
	const char* name;
	uint32_t avail_bit;

	/** \brief Function that will be called when a request addressed to this
	 * node is received.
	 */
	CS_RequestHandler request_handler;

	/** \brief Function for managing of node power management.
	 *
	 * It is used to put node to sleep when no peer device is connected.
	 */
	CS_PowerModeHandler power_handler;

	/** \brief Polling function that is called from main loop to handle internal
	 * processes of the node.
	 *
	 * This function is optional and can be set to NULL if not required.
	 */
	CS_PollHandler poll_handler;
};

struct CS_Handle_Struct
{
	int node_cnt;
	struct CS_Node_Struct* node[CS_MAX_NODE_COUNT];

	const char* conf_content;
	int conf_content_len;
	int conf_page_cnt;
};

enum CS_ErrorCodes
{
	CS_OK          = 0, /* \brief Generic success return value. */
	CS_ERROR       = 1, /* \brief Generic error return value. */
	CS_TIMEOUT     = 2,
	CS_NO_RESPONSE = 3
};


//-----------------------------------------------------------------------------
// EXPORTED FUNCTION DECLARATIONS
//-----------------------------------------------------------------------------

extern int CS_Init();

extern int CS_RegisterNode(struct CS_Node_Struct* node);

/**
 *
 * \param[in] request
 * Pointer to zero terminated C-string that contains request payload.
 */
extern int CS_ProcessRequest(char *request);

/** */
extern int CS_PollNodes(void);

/**
 *
 * \param response
 * Pointer to zero terminated C-string containing the whole response packet to
 * be sent (including token).
 */
extern int CS_InjectResponse(char* response);

extern int CS_SetPowerMode(enum CS_PowerMode mode);

//extern void CS_SetAppConfig(const char* content);


#ifdef __cplusplus
}
#endif


#endif /* _CS_H_ */

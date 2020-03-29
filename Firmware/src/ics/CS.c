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


//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include "RTE_Components.h"

#include <ctype.h>
#include <ics/CS.h>
#include <ics/CS_Platform.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined RTE_DEVICE_BDK_OUTPUT_REDIRECTION && CS_LOG_WITH_ANSI_COLORS != 0
#include "ansi_color.h"
#endif

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

#define CSN_SYS_NODE_NAME "SYS"

#define CSN_SYS_AVAIL_BIT ((uint32_t)0x00000000)


//-----------------------------------------------------------------------------
// FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

static int CSN_SYS_RequestHandler(const struct CS_Request_Struct* request,
                                  char* response);


//-----------------------------------------------------------------------------
// INTERNAL VARIABLES
//-----------------------------------------------------------------------------

static struct CS_Handle_Struct cs;

static char cs_tx_buffer[21];

static char cs_node_response[21];

static struct CS_Node_Struct cs_sys_node = {
		CSN_SYS_NODE_NAME,
		CSN_SYS_AVAIL_BIT,
		&CSN_SYS_RequestHandler
};


//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

int CS_Init()
{
	int i, errcode;

	// Initialize structure with default values
	cs.node_cnt = 0;
	for (i = 0; i < CS_MAX_NODE_COUNT; ++i)
	{
		cs.node[i] = NULL;
	}
	cs.conf_content = NULL;
	cs.conf_content_len = 0;
	cs.conf_page_cnt = 0;

	errcode = CS_PlatformInit(&cs);
	if (errcode != CS_OK)
	{
		CS_SYS_Error("Platform initialization failed.");
		return errcode;
	}

	CS_SYS_Info("Platform initialized.");

	// Add SYS service node.
	CS_RegisterNode(&cs_sys_node);

	return CS_OK;
}

int CS_RegisterNode(struct CS_Node_Struct* node)
{
	if (node == NULL ||
	    node->name == NULL ||
		node->request_handler == NULL)
	{
		CS_SYS_Error("Failed to register node.");
		return CS_ERROR;
	}

	if (cs.node_cnt == CS_MAX_NODE_COUNT)
	{
		CS_SYS_Error("Reached maximum node count.");
		return CS_ERROR;
	}

	cs.node[cs.node_cnt] = node;
	cs.node_cnt += 1;

	CS_SYS_Info("Registered node '%s'", cs.node[cs.node_cnt - 1]->name);
	return CS_OK;
}

int CS_ProcessRequest(char *request)
{
	ledNotif2(3, 200);

	int errcode, i;
	uint32_t timestamp;
	struct CS_Request_Struct parsed_request;

	if (request == NULL)
	{
        return CS_ERROR;
	}

	timestamp = CS_PlatformTime();

#if CS_LOG_WITH_ANSI_COLORS != 0 && defined RTE_DEVICE_BDK_OUTPUT_REDIRECTION
    CS_SYS_Info("Received request packet: '" COLORIZE("%s", CYAN, BOLD) "'",
            request);
#else
	CS_SYS_Info("Received request packet: '%s'", request);
#endif

	// Parse header information
	parsed_request.token = strtok(request, "/");
	if (parsed_request.token == NULL)
	{
		CS_SYS_Error("Failed to parse request token.");
		return CS_ERROR;
	}
	if (strlen(parsed_request.token) != 1)
	{
		CS_SYS_Error("Invalid request token length.");
		return CS_ERROR;
	}
	if (parsed_request.token[0] < '0' || parsed_request.token[0] > '~')
	{
		CS_SYS_Error("Invalid request token value.");
		return CS_ERROR;
	}

	parsed_request.node = strtok(NULL, "/");
	if (parsed_request.node == NULL)
	{
		CS_SYS_Error("Failed to parse request node name.");
		return CS_ERROR;
	}

	parsed_request.property = strtok(NULL, "/");
	if (parsed_request.property == NULL)
	{
		CS_SYS_Error("Failed to parse request node property.");
		return CS_ERROR;
	}

	// NULL for read requests
	parsed_request.property_value = strtok(NULL, "/");

	// Iterate all available nodes to find a match.
	for (i = 0; i < cs.node_cnt; ++i)
	{
		if (strcmp(parsed_request.node, cs.node[i]->name) == 0)
		{
			// Matching node was found -> pass request
			errcode = cs.node[i]->request_handler(&parsed_request, cs_node_response);
			if (errcode == CS_OK &&
			    strlen(cs_node_response) <= CS_MAX_RESPONSE_LENGTH) // 2b for token + response = 20b
			{
				// Compose response packet from token + node response
				sprintf(cs_tx_buffer, "%s/%s", parsed_request.token, cs_node_response);
#if CS_LOG_WITH_ANSI_COLORS != 0 && defined RTE_DEVICE_BDK_OUTPUT_REDIRECTION
                CS_SYS_Info(
                        "Composed response packet '" COLORIZE("%s", MAGENTA, BOLD) "'",
                        cs_tx_buffer);
#else
				CS_SYS_Info("Composed response packet '%s'", cs_tx_buffer);
#endif

				// Send response to platform
				int res_len = strlen(cs_tx_buffer);
				if (CS_PlatformWrite(cs_tx_buffer, res_len) == CS_OK)
				{
					timestamp = CS_PlatformTime() - timestamp;
					CS_SYS_Verbose("Request completed in %lu ms.", timestamp);
					return CS_OK;
				}
				else
				{
					CS_SYS_Error("Platform send failed. (errcode=%d)", errcode);
					return CS_ERROR;
				}
			}
			else if (errcode == CS_NO_RESPONSE)
			{
			    return CS_OK;
			}
			else
			{
				CS_SYS_Error("Node request processing error. (errcode=%d)", errcode);
				sprintf(cs_tx_buffer, "%s/e/UNK_ERROR", parsed_request.token);
				errcode = CS_PlatformWrite(cs_tx_buffer, strlen(cs_tx_buffer));
				if (errcode != CS_OK)
				{
					CS_SYS_Error("Platform send failed. (errcode=%d)", errcode);
					return CS_ERROR;
				}
				// Node failed to process request
				return CS_ERROR;
			}
		}
	}

	CS_SYS_Error("No matching node found for '%s'", parsed_request.node);
	sprintf(cs_tx_buffer, "%s/e/UNK_NODE", parsed_request.token);
	errcode = CS_PlatformWrite(cs_tx_buffer, strlen(cs_tx_buffer));
	if (errcode != CS_OK)
	{
		CS_SYS_Error("Platform send failed. (errcode=%d)", errcode);
		return CS_ERROR;
	}

	return CS_ERROR;
}

int CS_PollNodes(void)
{
    for (int i = 0; i < cs.node_cnt; ++i)
    {
        if (cs.node[i]->poll_handler != NULL)
        {
            cs.node[i]->poll_handler();
        }
    }

    return CS_OK;
}

int CS_InjectResponse(char* response)
{
    int errcode;
    int response_len;

    // check response length, including 2 bytes for token and 1 extra byte to
    // detect long response
    response_len = strnlen(response, CS_MAX_RESPONSE_LENGTH + 3);
    if (response_len > CS_MAX_RESPONSE_LENGTH + 2)
    {
        CS_SYS_Error("Attempting to inject too long response packet.");
        return CS_ERROR;
    }

#if CS_LOG_WITH_ANSI_COLORS != 0 && defined RTE_DEVICE_BDK_OUTPUT_REDIRECTION
    CS_SYS_Info(
            "Injecting response packet '" COLORIZE("%s", MAGENTA, BOLD) "'",
            cs_tx_buffer);
#else
    CS_SYS_Info("Injecting response packet '%s'", response);
#endif

    // Send response to platform
    errcode = CS_PlatformWrite(response, response_len);
    if (errcode == CS_OK)
    {
        return CS_OK;
    }
    else
    {
        CS_SYS_Error("Platform send failed. (errcode=%d)", errcode);
        return CS_ERROR;
    }
}

int CS_SetPowerMode(enum CS_PowerMode mode)
{
    for (int i = 0; i < cs.node_cnt; ++i)
    {
        if (cs.node[i]->power_handler != NULL)
        {
            cs.node[i]->power_handler(mode);
        }
    }

    return CS_OK;
}

void CS_SetAppConfig(const char* content)
{
	if (content == NULL)
	{
		return;
	}

	cs.conf_content_len = strlen(content);
	if (cs.conf_content_len == 0)
	{
		return;
	}

	cs.conf_page_cnt = (cs.conf_content_len / CS_TEXT_PAGE_LEN);
	if (cs.conf_page_cnt * CS_TEXT_PAGE_LEN < cs.conf_content_len)
	{
		cs.conf_page_cnt += 1;
	}

	cs.conf_content = content;

	CS_SYS_Info("Set app config. %dB long, %d pages.", cs.conf_content_len, cs.conf_page_cnt);
}

void CS_Log(enum CS_Log_Level level, const char* module, const char* fmt, ...)
{

#if defined RTE_DEVICE_BDK_OUTPUT_REDIRECTION && CS_LOG_WITH_ANSI_COLORS != 0
    static const char* log_level_str[] = {
          COLORIZE("ERROR", RED, BOLD),
          COLORIZE("WARN", YELLOW, BOLD),
          COLORIZE("INFO", GREEN),
          "VERBOSE"
      };
#else
    static const char* log_level_str[] = {
        "ERROR",
        "WARN",
        "INFO",
        "VERBOSE"
    };
#endif



	if (module != NULL && fmt != NULL)
	{
		CS_PlatformLogLock();

		CS_PlatformLogPrintf("[CS %s][%s] ", log_level_str[level], module);
		va_list args;
		va_start(args, fmt);
		CS_PlatformLogVprintf(fmt, args);
		va_end(args);
		CS_PlatformLogPrintf("\r\n");

		CS_PlatformLogUnlock();
	}
}

static int CSN_SYS_RequestHandler(const struct CS_Request_Struct* request, char* response)
{
	ledNotif2(2, 200);
	// CONF property request (RW)
	if (strcmp(request->property, "CONF") == 0)
	{
		if (request->property_value == NULL)
		{
			// Read -> Return page count
			sprintf(response, "i/%d", cs.conf_page_cnt);
			return CS_OK;
		}
		else
		{
			if (cs.conf_content == NULL)
			{
				strcpy(response, "e/NO_CONF");
				return CS_OK;
			}

			int page_id = atoi(request->property_value);
			if (page_id >= 0 && page_id < cs.conf_page_cnt)
			{
				int content_index = page_id * CS_TEXT_PAGE_LEN;
				sprintf(response, "t/%.*s", CS_TEXT_PAGE_LEN, cs.conf_content + content_index);
				return CS_OK;
			}
			else
			{
				strcpy(response, "e/INV_INDEX");
				return CS_OK;
			}
		}
	}

	// Check request type. Only R requests below.
	if (request->property_value != NULL)
	{
		CS_SYS_Error("SYS properties support only read requests.");
		sprintf(response, "e/ACCESS");
		return CS_OK;
	}

	// AVAIL property request
	if (strcmp(request->property, "AVAIL") == 0)
	{
		int i;
		uint32_t avail_bit_map = 0;

		for (i = 0; i < cs.node_cnt; ++i)
		{
			avail_bit_map |= cs.node[i]->avail_bit;
		}

		sprintf(response, "h/%lX", avail_bit_map);
		return CS_OK;
	}

	// NODE property request
	if (strcmp(request->property, "NODE") == 0)
	{
		sprintf(response, "i/%d", cs.node_cnt - 1);
		return CS_OK;
	}

	// NODEx property request
	if (strlen(request->property) > 4 &&
	    memcmp(request->property, "NODE", 4) == 0)
	{
		// check if there are only digits after first 4 characters
		char* c = (char*)&request->property[4];
		int valid_number = 1;
		while (*c != '\0')
		{
			if (isdigit(*c) == 0)
			{
				valid_number = 0;
				break;
			}
			++c;
		}

		if (valid_number == 1)
		{
			int node_index = atoi(&request->property[4]);
			if (node_index >= 0 && node_index < (cs.node_cnt - 1))
			{
				sprintf(response, "n/%s", cs.node[node_index + 1]->name);
				return CS_OK;
			}
			else
			{
				CS_SYS_Error("Out of bound NODEx request.");
				// Invalid property error
			}
		}
		else
		{
			// Invalid property error
		}
	}

	CS_SYS_Error("SYS property '%s' does not exist.", request->property);
	sprintf(response, "e/UNK_PROP");
	return CS_OK;
}

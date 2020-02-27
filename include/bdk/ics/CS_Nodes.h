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

#ifndef CS_NODES_H_
#define CS_NODES_H_

#include "RTE_Components.h"

#include "CSN_LP_ALS.h"
#include "CSN_LP_ENV.h"
#include "CSN_LP_AO.h"


//#ifdef RTE_ICS_PROTOCOL_NODE_ENV
//#include "node/CSN_ENV.h"
//#endif /* RTE_BDK_ICS_NODE_ENV */

//#ifdef RTE_ICS_PROTOCOL_NODE_AO
//#include "node/CSN_AO.h"
//#endif /* RTE_BDK_ICS_NODE_AO */

#ifdef RTE_ICS_PROTOCOL_NODE_PIR
#include "node/CSN_PIR.h"
#endif /* RTE_BDK_ICS_NODE_PIR */

#ifdef RTE_ICS_PROTOCOL_NODE_STPR
#include "node/CSN_STPR.h"
#endif /* RTE_BDK_ICS_NODE_STPR */

#ifdef RTE_ICS_PROTOCOL_NODE_LEDB
#include "node/CSN_LEDB.h"
#endif /* RTE_BDK_ICS_NODE_LEDB */

#ifdef RTE_ICS_PROTOCOL_NODE_BLDC
#include "node/CSN_BLDC.h"
#endif /* RTE_BDK_ICS_NODE_BLDC */

#endif /* CS_NODES_H_ */

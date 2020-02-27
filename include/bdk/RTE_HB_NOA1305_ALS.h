//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
//! \file RTE_HB_NOA1305_ALS.h
//!
//!
//-----------------------------------------------------------------------------

#ifndef RTE_HB_NOA1305_ALS_H_
#define RTE_HB_NOA1305_ALS_H_

// <<< Use Configuration Wizard in Context Menu >>>

// <e> Enable Interrupt generation for NOA1305
#ifndef RTE_NOA1305_ALS_INT_ENABLED
#define RTE_NOA1305_ALS_INT_ENABLED          1
#endif

// <o> Interrupt signal DIO Pad <0-15>
// <i> DIO used for NOA1305 interrupt signal.
// <i> Default: DIO13 for HB-GEVK board.
#ifndef RTE_NOA1305_ALS_DIO_INT_PIN
#define RTE_NOA1305_ALS_DIO_INT_PIN          13
#endif

// <o> DIO Interrupt Source <0-3>
// <i> Which of the four DIO interrupt sources to use.
// <i> Default: 2
#ifndef RTE_NOA1305_ALS_INT_SRC
#define RTE_NOA1305_ALS_INT_SRC          2
#endif

// </e>

// <<< end of configuration section >>>

#endif /* RTE_HB_NOA1305_ALS_H_ */

//! \}
//! \}
//! \}
//! \}

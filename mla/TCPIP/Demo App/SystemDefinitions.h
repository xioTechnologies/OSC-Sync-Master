/**
 * @file SystemDefinitions.h
 * @author Seb Madgwick
 * @brief Global system definitions.  All I/O pin assignments and timing
 * calculations must be derived from this file.
 */

#ifndef SYSTEM_DEFINITIONS_H
#define SYSTEM_DEFINITIONS_H

//------------------------------------------------------------------------------
// Includes

#include <xc.h>

//------------------------------------------------------------------------------
// Definitions - System clock

#define SYSCLK  80000000ul
#define PBCLK   (SYSCLK / (1 << OSCCONbits.PBDIV))

//------------------------------------------------------------------------------
// Definitions - I/O assignments

// LEDs
#define LED1_TRIS               _TRISD0
#define LED1_LAT                _LATD0
#define LED2_TRIS               _TRISD1
#define LED2_LAT                _LATD1
#define LED3_TRIS               _TRISD2
#define LED3_LAT                _LATD2

// Buttons
#define SW1_PORT                _RD6
#define SW1_CNPUE               CNPUEbits.CNPUE15
#define SW2_PORT                _RD7
#define SW2_CNPUE               CNPUEbits.CNPUE16
#define SW3_PORT                _RD13
#define SW3_CNPUE               CNPUEbits.CNPUE19

// External clock
#define EXTERNAL_CLOCK_PORT     _RC14
#define EXTERNAL_CLOCK_CNEN     CNENbits.CNEN0
#define EXTERNAL_CLOCK_CNPUE    CNPUEbits.CNPUE0

#endif

//------------------------------------------------------------------------------
// End of file

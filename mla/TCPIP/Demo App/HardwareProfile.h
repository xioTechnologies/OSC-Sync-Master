/**
 * @file HardwareProfile.h
 * @author Seb Madgwick
 * @brief Definitions required by Microchip libraries.
 */

#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

//------------------------------------------------------------------------------
// Includes

#include "Compiler.h"
#include "SystemDefinitions.h"

//------------------------------------------------------------------------------
// Definitions - Clock frequency values

#define GetSystemClock()        (SYSCLK)
#define GetInstructionClock()   (GetSystemClock()/1)
#define GetPeripheralClock()    (GetSystemClock()/1)

//------------------------------------------------------------------------------
// Definitions - Ethernet

// External National PHY configuration
#define	PHY_RMII				// external PHY runs in RMII mode
#define	PHY_CONFIG_ALTERNATE	// alternate configuration used
#define	PHY_ADDRESS			0x1	// the address of the National DP83848 PHY

#endif

//------------------------------------------------------------------------------
// End of file

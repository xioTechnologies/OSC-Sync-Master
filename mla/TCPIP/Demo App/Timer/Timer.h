/**
 * @file Timer.h
 * @author Seb Madgwick
 * @brief Timer library for PIC32MX.
 */

#ifndef TIMER_H
#define TIMER_H

//------------------------------------------------------------------------------
// Includes

#include <stdint.h> // uint32_t, uint64_t
#include "HardwareProfile.h" // GetSystemClock()

//------------------------------------------------------------------------------
// Definitions

/**
 * @bief Number of timer ticks per second.  Necessary for timing calculations.
 */
#define TIMER_TICKS_PER_SECOND GetSystemClock()

/**
 * @brief 32-bit timer ticks type.
 */
typedef uint32_t Ticks32;

/**
 * @brief 64-bit timer ticks type.
 */
typedef union {
    uint64_t value;

    struct {

        union {
            uint32_t dword0;
            Ticks32 ticks32;
        }; // least-significant dword
        uint32_t dword1; // most-significant dword
    };
} Ticks64;

//------------------------------------------------------------------------------
// Function prototypes

void TimerInitialise();
Ticks32 TimerGetTicks32();
Ticks64 TimerGetTicks64();
void TimerDelay(uint32_t milliseconds);

#endif

//------------------------------------------------------------------------------
// End of file

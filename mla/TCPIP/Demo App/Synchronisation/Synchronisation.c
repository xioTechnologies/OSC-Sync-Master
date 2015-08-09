/**
 * @file Synchronisation.c
 * @author Seb Madgwick
 * @brief Provides a measurement of time synchronised with a remote
 * synchronisation master.
 */

//------------------------------------------------------------------------------
// Includes

#include <ieee754.h>
#include <stdint.h> // UINT32_MAX
#include "Synchronisation.h"

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Threshold (in timer ticks) used to determine if the slave clock
 * should jump directly to the master clock.  This value may be adjusted as
 * required by the user application.
 *
 * The threshold value should be greater than the expected maximum communication
 * latency between the master and client.  The units are timer ticks.  For
 * example, a threshold of 500 ms would be specified as a THRESHOLD value of
 * (TIMER_TICKS_PER_SECOND / 2).
 */
#define THRESHOLD (TIMER_TICKS_PER_SECOND / 2)

/**
 * @brief Drift rate of the slow clock (in timer ticks).  This value may be
 * adjusted as required by the user application.
 *
 * Must be greater than the expected worse-case relative difference in speed
 * between the master and slave clocks.  For example, if both the master and
 * slave derive clocks are derived from a ±10 ppm crystal then SLOW_CLOCK_DRIFT
 * should be at least 20 us per second.  The units are timer ticks.  For
 * example, a rate of 20 us per second would be specified as a SLOW_CLOCK_DRIFT
 * value of (TIMER_TICKS_PER_SECOND / (1000000ul / 20)).
 */
#define SLOW_CLOCK_DRIFT 0

//------------------------------------------------------------------------------
// Variable declarations

static ieee754dp ticksToOscTimeTag; // constant ratio
static ieee754dp oscTimeTagToTicks; // constant ratio
static uint64_t slaveClockOffset; // offset added to timer ticks to yield the slave clock
static uint64_t observedMasterClockOffset; // offset added to timer ticks to yield the observed master clock

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises module.  This function should be called once on system
 * start up.
 */
void SynchronisationInitialise() {
    ticksToOscTimeTag = ieee754dp_div(ieee754dp_fulong(UINT32_MAX), ieee754dp_fulong(TIMER_TICKS_PER_SECOND + SLOW_CLOCK_DRIFT));
    oscTimeTagToTicks = ieee754dp_div(ieee754dp_fulong(TIMER_TICKS_PER_SECOND + SLOW_CLOCK_DRIFT), ieee754dp_fulong(UINT32_MAX));
}

/**
 * @brief Updates synchronisation algorithm with time received from master.
 * This function should be called each time a synchronisation message is
 * received from the master.
 * @param oscTimeTag OSC time tag received from master.
 * @param timeOfArrival Timer ticks value when the OSC time tag was received
 * from the master.
 */
void SynchronisationUpdate(const OscTimeTag oscTimeTag, const Ticks64 timeOfArrival) {
    const uint64_t observedMasterClock = ieee754dp_tulong(ieee754dp_mul(ieee754dp_fulong(oscTimeTag.value), oscTimeTagToTicks)); // convert OSC time tag to ticks
    const uint64_t slowClock = timeOfArrival.value + slaveClockOffset;
    observedMasterClockOffset = observedMasterClock - timeOfArrival.value;
    if (observedMasterClock < slowClock) {
        if ((slowClock - observedMasterClock) < THRESHOLD) {
            return; // ignore update if behind slave time and within threshold
        }
    }
    slaveClockOffset = observedMasterClockOffset;
}

/**
 * @brief Converts timer ticks value to an OSC time tag time corresponding to
 * the slave clock synchronised with the master.
 * @param ticks64 Timer ticks value.
 * @return OSC time tag time corresponding to the slave clock synchronised with
 * the master.
 */
OscTimeTag SynchronisationTicksToOscTimeTag(const Ticks64 ticks64) {
    const OscTimeTag oscTimeTag = {.value = ieee754dp_tulong(ieee754dp_mul(ieee754dp_fulong(ticks64.value + slaveClockOffset), ticksToOscTimeTag))};
    return oscTimeTag;
}

/**
 * @brief Converts timer ticks value to an OSC time tag time corresponding to
 * the observed master clock.
 * @param ticks64 Timer ticks value.
 * @return OSC time tag time corresponding to the observed master clock.
 */
OscTimeTag SynchronisationTicksToOscTimeTagAsObserved(const Ticks64 ticks64) {
    const OscTimeTag oscTimeTag = {.value = ieee754dp_tulong(ieee754dp_mul(ieee754dp_fulong(ticks64.value + observedMasterClockOffset), ticksToOscTimeTag))};
    return oscTimeTag;
}

//------------------------------------------------------------------------------
// End of file

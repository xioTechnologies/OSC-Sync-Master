/**
 * @file Send.c
 * @author Seb Madgwick
 * @brief Application tasks and functions for sending messages.
 */

//------------------------------------------------------------------------------
// Includes

#include "Ethernet/Ethernet.h"
#include "Osc99/Osc99.h"
#include "Send.h"
#include "Synchronisation/Synchronisation.h"
#include "SystemDefinitions.h"
#include "Timer/Timer.h"

//------------------------------------------------------------------------------
// Definitions

/**
 * @breif Rate (messages per second) at which synchronisation messages are sent.
 */
#define SYNCHRONISATION_RATE 1

#define CN_IFSXCLR IFS1CLR
#define CN_IECXSET IEC1SET
#define CN_IECXCLR IEC1CLR
#define CN_INT_BIT (1 << 0)

//------------------------------------------------------------------------------
// Function prototypes

static void BroadcastSynchronisationMessage();
static void UnicastExternalClockTimestamp();

//------------------------------------------------------------------------------
// Variables

volatile static Ticks64 externalTriggerTimestamp;
volatile static bool externalTriggerState;

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises module.  This function should be called once on system
 * start up.
 *
 * The unpopulated RTC crystal pin (labelled Y3 on PCB) are configured for input
 * change notification interrupt.
 */
void SendInitialise() {
    EXTERNAL_CLOCK_CNEN = 1;
    CNCONbits.ON = 1;
    IPC6bits.CNIP = 6;
    CN_IFSXCLR = CN_INT_BIT; // clear interrupt flag
    CN_IECXSET = CN_INT_BIT; // enable interrupt
}

/**
 * @brief Do tasks.  This function should be called repeatedly within the main
 * program loop.
 */
void SendDoTasks() {

    // Broadcast synchronisation message
    static Ticks32 ledTicks = 0;
    const Ticks32 currentTicks = TimerGetTicks32();
    static Ticks32 previousTicks;
    if ((currentTicks - previousTicks) >= (TIMER_TICKS_PER_SECOND / SYNCHRONISATION_RATE)) {
        previousTicks = currentTicks;
        BroadcastSynchronisationMessage();
        LED3_LAT = 1;
        ledTicks = currentTicks;
        if (ledTicks == 0) {
            ledTicks = 1; // value cannot be zero as this would indicate that the LED is not currently blinking
        }
    }
    if (ledTicks != 0) {
        if ((currentTicks - ledTicks) >= (TIMER_TICKS_PER_SECOND / 10)) { // turn LED off after 100 ms
            LED3_LAT = 0;
            ledTicks = 0;
        }
    }

    // Unicast external clock edge timestamp
    if (externalTriggerTimestamp.value != 0) {

        UnicastExternalClockTimestamp();
        externalTriggerTimestamp.value = 0;
    }
}

/**
 * @brief Broadcasts synchronisation message.
 */
static void BroadcastSynchronisationMessage() {
    OscMessage oscMessage;
    OscMessageInitialise(&oscMessage, "/sync");
    OscMessageAddTimeTag(&oscMessage, SynchronisationTicksToOscTimeTag(TimerGetTicks64()));
    OscPacket oscPacket;
    OscPacketInitialiseFromContents(&oscPacket, &oscMessage);
    EthernetBroadcast(oscPacket.contents, oscPacket.size);
}

/**
 * @brief Unicasts external clock edge timestamp.
 */
static void UnicastExternalClockTimestamp() {
    OscBundle oscBundle;
    OscBundleInitialise(&oscBundle, SynchronisationTicksToOscTimeTag(externalTriggerTimestamp));
    OscMessage oscMessage;
    OscMessageInitialise(&oscMessage, "/external");
    OscMessageAddBool(&oscMessage, externalTriggerState);
    OscBundleAddContents(&oscBundle, &oscMessage);
    OscPacket oscPacket;
    OscPacketInitialiseFromContents(&oscPacket, &oscBundle);
    EthernetUnicast(oscPacket.contents, oscPacket.size);
}

//------------------------------------------------------------------------------
// Functions - Interrupt

/**
 * Input change notification interrupt service routine to store timestamp and
 * state of external trigger signal.
 */
void __attribute__((interrupt(), vector(_CHANGE_NOTICE_VECTOR))) CNInterrupt() {
    externalTriggerTimestamp = TimerGetTicks64();
    LED2_LAT = EXTERNAL_CLOCK_PORT; // must read port else interrupt will persist
    CN_IFSXCLR = CN_INT_BIT; // clear interrupt flag
}

//------------------------------------------------------------------------------
// End of file

/**
 * @file Main.c
 * @author Seb Madgwick
 * @brief Firmware main file.
 *
 * Device:
 * PIC32MX795F512L
 *
 * Compiler:
 * XC32 v1.34
 */

//------------------------------------------------------------------------------
// Includes

#include "Ethernet/Ethernet.h"
#include "Send/Send.h"
#include "stdbool.h"
#include "Synchronisation/Synchronisation.h"
#include "SystemDefinitions.h"
#include "Timer/Timer.h"

//------------------------------------------------------------------------------
// Configuration Bits

#pragma config FPLLODIV = DIV_1, FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FWDTEN = OFF, FPBDIV = DIV_1, POSCMOD = XT, FNOSC = PRIPLL, CP = OFF
#pragma config FMIIEN = OFF, FETHIO = OFF	// external PHY in RMII/alternate configuration
#pragma config FSOSCEN = OFF // disable secondary oscillator

//------------------------------------------------------------------------------
// Function prototypes

static void Initialise();

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Firmware entry point.
 * @return This function should never return.
 */
int main(void) {

    // Initialise CPU and I/O
    Initialise();

    // Initialise driver and application modules
    TimerInitialise();
    SynchronisationInitialise();
    EthernetInitialise();
    SendInitialise();

    // Main loop
    while (true) {
        EthernetDoTasks();
        SendDoTasks();
    }
}

/**
 * @brief Initialise CPU and I/O.
 */
static void Initialise() {

    // Configure CPU
    INTEnableSystemMultiVectoredInt();
    SYSTEMConfigPerformance(SYSCLK);
    OSCSetPBDIV(OSC_PB_DIV_1); // PBCLK is SYSCLK divided by 1   

    // Configure LEDs I/O
    LED1_TRIS = 0;
    LED2_TRIS = 0;
    LED3_TRIS = 0;

    // Configure Buttons I/O
    SW1_CNPUE = 1;
    SW2_CNPUE = 1;
    SW3_CNPUE = 1;

    // Configure external clock I/O
    EXTERNAL_CLOCK_CNPUE = 1;
}

/**
 * @brief General exception handler.  If this function is called then something
 * has gone horribly wrong.  All the LEDs will be lit and the PIC32 will hang.
 * Pressing a button will perform a software reset.  See page 11 of "PIC32
 * Family Reference Manual, Sect. 07 Resets".
 */
void _general_exception_handler() {

    // Turn on all LEDs
    LED1_LAT = 1;
    LED2_LAT = 1;
    LED3_LAT = 1;

    // Wait for button press before software reset
    while (true) {
        if (!SW1_PORT || !SW2_PORT || !SW3_PORT) {
            SYSKEY = 0x00000000; //write invalid key to force lock
            SYSKEY = 0xAA996655; //write key1 to SYSKEY
            SYSKEY = 0x556699AA; //write key2 to SYSKEY
            RSWRSTSET = 1; //set SWRST bit to arm reset
            unsigned int dummy;
            dummy = RSWRST; // read RSWRST register to trigger reset
            while (1); // prevent any unwanted code execution until reset occurs
        }
    }
}

//------------------------------------------------------------------------------
// End of file

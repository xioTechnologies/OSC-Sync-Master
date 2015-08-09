/**
 * @file Ethernet.h
 * @author Seb Madgwick
 * @brief Ethernet module for sending and receiving UDP packets.
 */

#ifndef ETHERNET_H
#define ETHERNET_H

//------------------------------------------------------------------------------
// Includes

#include <stddef.h> // size_t, NULL

//------------------------------------------------------------------------------
// Function prototypes

void EthernetInitialise();
void EthernetDoTasks();
int EthernetUnicast(const char* const source, const size_t numberOfBytes);
int EthernetBroadcast(const char* const source, const size_t numberOfBytes);
size_t EthernetGet(const char* const destination, const size_t destinationSize);

#endif

//------------------------------------------------------------------------------
// End of file

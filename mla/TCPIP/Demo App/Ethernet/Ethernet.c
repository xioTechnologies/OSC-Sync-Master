/**
 * @file Ethernet.c
 * @author Seb Madgwick
 * @brief Ethernet module for sending and receiving UDP packets.
 */

//------------------------------------------------------------------------------
// Includes

#include "Ethernet/Ethernet.h"
#include "InitAppConfig.h"
#include "TCPIP Stack/TCPIP.h"

//------------------------------------------------------------------------------
// Definitions

#define UNICAST_IP "192.168.1.2"
#define UNICAST_PORT 8000
#define BROADCAST_PORT 9000
#define RECEIVE_PORT 9000

//------------------------------------------------------------------------------
// Variables

static UDP_SOCKET unicastSocket = INVALID_UDP_SOCKET;
static UDP_SOCKET broadcastSocket = INVALID_UDP_SOCKET;
static UDP_SOCKET receiveSocket = INVALID_UDP_SOCKET;
static IP_ADDR unicastIP;

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises module.  This function should be called once on system
 * start up.
 */
void EthernetInitialise() {
    
    // Initialise TCP/IP stack
    TickInit();
    InitAppConfig();
    StackInit();
    
    // Parse IP address from string
    StringToIPAddress((BYTE*) UNICAST_IP, &unicastIP);
}

/**
 * @brief Do tasks.  This function should be called repeatedly within the main
 * program loop.
 */
void EthernetDoTasks() {
    
    // Perform TCP/IP stack tasks and applications
    StackTask();
    StackApplications();        

    // Maintain UDP sockets
    if (MACIsLinked()) {
        
        // Open unicast socket
        if (unicastSocket == INVALID_UDP_SOCKET) {
            if (unicastIP.Val == 0xFFFFFFFF) { // broadcast IP address 255.255.255.255
                unicastSocket = UDPOpenEx((DWORD) NULL, UDP_OPEN_NODE_INFO, RECEIVE_PORT, UNICAST_PORT);
            } else {
                unicastSocket = UDPOpenEx((DWORD) unicastIP.Val, UDP_OPEN_IP_ADDRESS, RECEIVE_PORT, UNICAST_PORT);
            }
        }

        // Open broadcast socket
        if (broadcastSocket == INVALID_UDP_SOCKET) {
            broadcastSocket = UDPOpenEx((DWORD) NULL, UDP_OPEN_NODE_INFO, RECEIVE_PORT, BROADCAST_PORT);
        }
        
        // Open receive socket
        if (receiveSocket == INVALID_UDP_SOCKET) {
            receiveSocket = UDPOpenEx(0, UDP_OPEN_SERVER, RECEIVE_PORT, 0);
        }        

    } else {
        UDPClose(unicastSocket);
        UDPClose(broadcastSocket);
        UDPClose(receiveSocket);
        unicastSocket = INVALID_UDP_SOCKET;
        broadcastSocket = INVALID_UDP_SOCKET;
        receiveSocket = INVALID_UDP_SOCKET;
    }
}

/**
 * @brief Unicasts UDP packet.
 * @param source Address of packet.
 * @param numberOfBytes Size of packet.
 * @return 0 if successful.
 */
int EthernetUnicast(const char* const source, const size_t numberOfBytes) {
    if (!MACIsLinked()) {
        return 1; // error: no link
    }
    if (UDPIsPutReady(unicastSocket) < numberOfBytes) {
        return 1; // error: too many bytes to write to socket
    }
    UDPPutArray((BYTE*) source, numberOfBytes);
    UDPFlush();
    return 0;
}

/**
 * @brief Broadcasts UDP packet.
 * @param source Address of packet.
 * @param numberOfBytes Size of packet.
 * @return 0 if successful.
 */
int EthernetBroadcast(const char* const source, const size_t numberOfBytes) {
    if (!MACIsLinked()) {
        return 1; // error: no link
    }
    if (UDPIsPutReady(broadcastSocket) < numberOfBytes) {
        return 1; // error: too many bytes to write to socket
    }
    UDPPutArray((BYTE*) source, numberOfBytes);
    UDPFlush();
    return 0;
}

/**
 * @brief Gets UDP packet from receive buffer.
 * @param destination Destination address.
 * @param destinationSize Size of destination that must not be exceeded.
 * @return Size of UDP packet.  0 if receive buffer empty.
 */
size_t EthernetGet(const char* const destination, const size_t destinationSize) {
    if (UDPIsGetReady(receiveSocket) > 0) {
        return UDPGetArray((BYTE*) destination, destinationSize);
    }
    return 0;
}

//------------------------------------------------------------------------------
// End of file

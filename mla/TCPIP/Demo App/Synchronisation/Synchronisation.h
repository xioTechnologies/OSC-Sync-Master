/**
 * @file Synchronisation.h
 * @author Seb Madgwick
 * @brief Provides a measurement of time synchronised with a remote server.
 */

#ifndef SYNCHRONISATION_H
#define SYNCHRONISATION_H

//------------------------------------------------------------------------------
// Includes

#include "Timer/Timer.h"
#include "Osc99/Osc99.h"

//------------------------------------------------------------------------------
// Function prototypes

void SynchronisationInitialise();
void SynchronisationUpdate(const OscTimeTag oscTimeTag, const Ticks64 timeOfReception);
OscTimeTag SynchronisationTicksToOscTimeTag(const Ticks64 ticks64);
OscTimeTag SynchronisationTicksToOscTimeTagAsObserved(const Ticks64 ticks64);

#endif

//------------------------------------------------------------------------------
// End of file

/**
 * @file OscMessage.h
 * @author Seb Madgwick
 * @brief Functions and structures for constructing and deconstructing OSC
 * messages.
 *
 * MAX_OSC_ADDRESS_PATTERN_LENGTH and MAX_NUMBER_OF_ARGUMENTS may be modified as
 * required by the user application.
 *
 * @see http://opensoundcontrol.org/spec-1_0
 */

#ifndef OSC_MESSAGE_H
#define OSC_MESSAGE_H

//------------------------------------------------------------------------------
// Includes

#include "OscCommon.h"
#include <stdbool.h> // bool, true, false
#include <stddef.h> // size_t, NULL
#include <stdint.h> // int32_t, int64_t

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Minimum size (number of bytes) of an OSC message as per the OSC
 * specification.  The size is 8 bytes which includes the terminating null
 * character.
 */
#define MIN_OSC_MESSAGE_SIZE sizeof("/\0\0\0,\0\0")

/**
 * @brief Maximum size (number of bytes) of an OSC message equal to the maximum
 * packet size  permitted by the transport protocol.
 */
#define MAX_OSC_MESSAGE_SIZE MAX_TRANSPORT_SIZE

/**
 * @brief Maximum string length (excludes terminating null characters) of an OSC
 * address pattern.  This value may be modified as required by the user
 * application.
 */
#define MAX_OSC_ADDRESS_PATTERN_LENGTH 64

/**
 * @brief Maximum number of arguments that may be contained within an OSC
 * message.  This value may be modified as required by the user application.
 */
#define MAX_NUMBER_OF_ARGUMENTS 16

/**
 * @brief Maximum length of an OSC type tag string (includes comma but not
 * terminating null characters).
 */
#define MAX_OSC_TYPE_TAG_STRING_LENGTH (1 + MAX_NUMBER_OF_ARGUMENTS)

/**
 * @brief Maximum combined size (number of bytes) of all arguments that may be
 * contained within an OSC message.  The calculation assumes the worst case of
 * and extra 4 null characters for both the OSC address pattern and the OSC type
 * tag string.
 */
#define MAX_ARGUMENTS_SIZE (MAX_OSC_MESSAGE_SIZE - (MAX_OSC_ADDRESS_PATTERN_LENGTH + 4) - (MAX_OSC_TYPE_TAG_STRING_LENGTH + 4))

/**
 * @brief OSC bundle structure.  Must be initialised using either
 * OscMessageInitialise or OscMessageInitialiseFromCharArray.
 */
typedef struct {
    char oscAddressPattern[MAX_OSC_ADDRESS_PATTERN_LENGTH + 1]; // null terminated, must be first member so that first byte of structure is equal to '/'.
    char oscTypeTagString[MAX_OSC_TYPE_TAG_STRING_LENGTH + 1]; // includes comma, null terminated
    char arguments[MAX_ARGUMENTS_SIZE];
    size_t oscAddressPatternLength; // does not include null characters
    size_t oscTypeTagStringLength; // includes comma but not null characters
    size_t argumentsSize;
    int oscTypeTagStringIndex;
    int argumentsIndex;
} OscMessage;

/**
 * @brief OSC type tag string characters indicating argument type.
 */
typedef enum {
    OscTypeTagInt32 = 'i',
    OscTypeTagFloat32 = 'f',
    OscTypeTagString = 's',
    OscTypeTagBlob = 'b',
    OscTypeTagInt64 = 'h',
    OscTypeTagTimeTag = 't',
    OscTypeTagDouble = 'd',
    OscTypeTagAlternateString = 'S',
    OscTypeTagCharacter = 'c',
    OscTypeTagRgbaColour = 'r',
    OscTypeTagMidiMessage = 'm',
    OscTypeTagTrue = 'T',
    OscTypeTagFalse = 'F',
    OscTypeTagNil = 'N',
    OscTypeTagInfinitum = 'I',
    OscTypeTagBeginArray = '[',
    OscTypeTagEndArray = ']',
} OscTypeTag;

//------------------------------------------------------------------------------
// Function prototypes

// Message construction
int OscMessageInitialise(OscMessage * const oscMessage, const char* oscAddressPattern);
int OscMessageSetAddressPattern(OscMessage * const oscMessage, const char* oscAddressPattern);
int OscMessageAppendAddressPattern(OscMessage * const oscMessage, const char* appendedParts);
int OscMessageAddInt32(OscMessage * const oscMessage, const int32_t int32);
int OscMessageAddFloat32(OscMessage * const oscMessage, const float float32);
int OscMessageAddString(OscMessage * const oscMessage, const char* string);
int OscMessageAddBlob(OscMessage * const oscMessage, const char* const source, const size_t sourceSize);
int OscMessageAddInt64(OscMessage * const oscMessage, const uint64_t int64);
int OscMessageAddTimeTag(OscMessage * const oscMessage, const OscTimeTag oscTimeTag);
int OscMessageAddDouble(OscMessage * const oscMessage, const Double64 double64);
int OscMessageAddAlternateString(OscMessage * const oscMessage, const char* string);
int OscMessageAddCharacter(OscMessage * const oscMessage, const char asciiChar);
int OscMessageAddRgbaColour(OscMessage * const oscMessage, const RgbaColour rgbaColour);
int OscMessageAddMidiMessage(OscMessage * const oscMessage, const MidiMessage midiMessage);
int OscMessageAddBool(OscMessage * const oscMessage, const bool boolean);
int OscMessageAddNil(OscMessage * const oscMessage);
int OscMessageAddInfinitum(OscMessage * const oscMessage);
int OscMessageAddBeginArray(OscMessage * const oscMessage);
int OscMessageAddEndArray(OscMessage * const oscMessage);
size_t OscMessageGetSize(const OscMessage * const oscMessage);
int OscMessageToCharArray(const OscMessage * const oscMessage, size_t * const oscMessageSize, char* const destination, const size_t destinationSize);

// Message deconstruction
int OscMessageInitialiseFromCharArray(OscMessage * const oscMessage, const char* const source, const size_t size);
bool OscMessageIsArgumentAvailable(OscMessage * const oscMessage);
OscTypeTag OscMessageGetArgumentType(OscMessage * const oscMessage);
int OscMessageSkipArgument(OscMessage * const oscMessage);
int OscMessageGetInt32(OscMessage * const oscMessage, int32_t * const int32);
int OscMessageGetFloat32(OscMessage * const oscMessage, float * const float32);
int OscMessageGetString(OscMessage * const oscMessage, char* const destination, const size_t destinationSize);
int OscMessageGetBlob(OscMessage * const oscMessage, size_t * const blobSize, char* const destination, const size_t destinationSize);
int OscMessageGetInt64(OscMessage * const oscMessage, int64_t * const int64);
int OscMessageGetTimeTag(OscMessage * const oscMessage, OscTimeTag * const oscTimeTag);
int OscMessageGetDouble(OscMessage * const oscMessage, Double64 * const double64);
int OscMessageGetCharacter(OscMessage * const oscMessage, char* const character);
int OscMessageGetRgbaColour(OscMessage * const oscMessage, RgbaColour * const rgbaColour);
int OscMessageGetMidiMessage(OscMessage * const oscMessage, MidiMessage * const midiMessage);

#endif

//------------------------------------------------------------------------------
// End of file

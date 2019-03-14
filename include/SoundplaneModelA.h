// Driver for Soundplane Model A.
// Copyright (c) 2013 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#pragma once

#include <array>
#include "SensorFrame.h"

// Soundplane data format:
// The Soundplane Model A sends frames of data over USB using an isochronous interface with two endpoints.
// Each endpoint carries the data for one of the two sensor boards in the Soundplane. There is a left sensor board, endpoint 0,
// and a right sensor board, endpoint 1.  Each sensor board has 8 pickups (horizontal) and 32 carriers (vertical)
// for a total of 256 taxels of data.
//
// The data is generated from FFTs run on the DSP inside the Soundplane. The sampling rate is 125000 Hz, which is created by
// the Soundplane processor’s internal clock dividing a 12 mHz crystal clock by 96. An FFT is performed every 128 samples
// to make data blocks for each endpoint at a post-FFT rate of 976.5625 Hz.
//
// Each data block for a surface contains 256 12-bit taxels packed into 192 16-bit words, followed by one 16-bit
// sequence number for a total of 388 bytes. The taxel data are packed as follows:
// 12 bits taxel 1 [hhhhmmmmllll]
// 12 bits taxel 2 [HHHHMMMMLLLL]
// 24 bits combined in three bytes: [mmmmllll LLLLhhhh HHHHMMMM]
//
// The packed data are followed by a 16-bit sequence number.
// Two bytes of padding are also present in the data packet. A full packet is always requested, and the
// Soundplane hardware returns either 0, or the data minus the padding. The padding is needed because for some
// arcane reason the data sent should be less than the negotiated size. So the negotiated size includes the
// padding (388 bytes) while 386 bytes are typically received in any transaction.

const int kSoundplaneTouchWidth = 8;
const int kSoundplaneCalibrateSize = 1024;
const int kSoundplaneHistorySize = 2048;
const float kSoundplaneFrameRate = 976.5625f;
const int kSoundplaneFrameIntervalMicros = 1000.f*1000.f/kSoundplaneFrameRate;
const float kZeroFilterFrequency = 10.f;

const int kSoundplaneAKeyWidth = 30;
const int kSoundplaneAKeyHeight = 5;
const int kSoundplaneAMaxZones = 150;

// Soundplane A hardware
const uint16_t kSoundplaneUSBVendor = 0x0451;
const uint16_t kSoundplaneUSBProduct = 0x5100;
const int kSoundplaneASampleRate = 125000;
const int kSoundplaneAFFTSize = 128;

const int kSoundplaneANumCarriers = 32;
const int kSoundplaneAPickupsPerBoard = 8;

const int kSoundplaneNumCarriers = 32;
const int kSoundplanePossibleCarriers = 64;
const float kMaxFrameDiff = 1.0f;

// Soundplane A USB firmware
const int kSoundplaneANumEndpoints = 2;
const int kSoundplaneAEndpointStartIdx = 1;
const int kSoundplaneADataBitsPerTaxel = 12;
const int kSoundplaneAPackedDataSize = (kSoundplaneANumCarriers*kSoundplaneAPickupsPerBoard*kSoundplaneADataBitsPerTaxel / 8); // 384 bytes
const int kSoundplaneAlternateSetting = 1;

typedef struct
{
  unsigned char packedData[kSoundplaneAPackedDataSize];
  uint16_t seqNum;
  uint16_t padding;
} SoundplaneADataPacket; // 388 bytes


// default carriers.  avoiding 32 (always bad)
// in use these should be overridden by the selected carriers.
//
const unsigned char kDefaultCarriers[kSoundplaneNumCarriers] =
{
	0, 0, 4, 5, 6, 7, 8, 9, 10, 11,
	12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25, 26, 27,
	28, 29, 30, 31, 33, 34
};


// USB device requests and indexes
//
typedef enum
{
  kRequestStatus = 0,
  kRequestMask = 1,
  kRequestCarriers = 2
} MLSoundplaneUSBRequest;

typedef enum
{
  kRequestCarriersIndex = 0,
  kRequestMaskIndex = 1
} MLSoundplaneUSBRequestIndex;

inline float carrierToFrequency(int carrier)
{
    int cIdx = carrier; // offset?
    float freq = kSoundplaneASampleRate/kSoundplaneAFFTSize*cIdx;
    return freq;
}


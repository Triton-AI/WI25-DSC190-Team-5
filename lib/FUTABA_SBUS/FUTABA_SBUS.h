#ifndef FUTABA_SBUS_H
#define FUTABA_SBUS_H

#include "mbed.h"

#define SBUS_FRAME_SIZE 25
#define SBUS_START_BYTE 0x0F

class FUTABA_SBUS {
public:
    FUTABA_SBUS(BufferedSerial *serial);
    void FeedLine();
    void UpdateChannels();

    uint16_t channels[16];
    volatile uint8_t toChannels;  // flag: 1 when a full frame has been received

private:
    BufferedSerial *sbusSerial;
    uint8_t frameBuffer[SBUS_FRAME_SIZE];
};

#endif // FUTABA_SBUS_H

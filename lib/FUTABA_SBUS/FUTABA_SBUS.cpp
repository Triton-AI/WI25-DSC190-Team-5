#include "FUTABA_SBUS.h"
#include <string.h>

FUTABA_SBUS::FUTABA_SBUS(BufferedSerial *serial) : sbusSerial(serial), toChannels(0) {
    memset(channels, 0, sizeof(channels));
}

void FUTABA_SBUS::FeedLine() {
    while (sbusSerial->readable()) {
        uint8_t byte;
        if (sbusSerial->read(&byte, 1)) {
            if (byte == SBUS_START_BYTE) {
                frameBuffer[0] = byte;
                size_t bytesRead = sbusSerial->read(frameBuffer + 1, SBUS_FRAME_SIZE - 1);
                if (bytesRead == SBUS_FRAME_SIZE - 1) {
                    toChannels = 1;
                    return;
                }
            }
        }
    }
}

void FUTABA_SBUS::UpdateChannels() {
    if (toChannels == 0) return;
    channels[0]  = ((uint16_t)frameBuffer[1]       | ((uint16_t)frameBuffer[2] << 8)) & 0x07FF;
    channels[1]  = (((uint16_t)frameBuffer[2] >> 3)  | ((uint16_t)frameBuffer[3] << 5)) & 0x07FF;
    channels[2]  = (((uint16_t)frameBuffer[3] >> 6)  | ((uint16_t)frameBuffer[4] << 2)  | (((uint16_t)frameBuffer[5] & 0x01) << 10)) & 0x07FF;
    channels[3]  = (((uint16_t)frameBuffer[5] >> 1)  | ((uint16_t)frameBuffer[6] << 7)) & 0x07FF;
    channels[4]  = (((uint16_t)frameBuffer[6] >> 4)  | ((uint16_t)frameBuffer[7] << 4)) & 0x07FF;
    channels[5]  = (((uint16_t)frameBuffer[7] >> 7)  | ((uint16_t)frameBuffer[8] << 1)  | (((uint16_t)frameBuffer[9] & 0x03) << 9)) & 0x07FF;
    channels[6]  = (((uint16_t)frameBuffer[9] >> 2)  | ((uint16_t)frameBuffer[10] << 6)) & 0x07FF;
    channels[7]  = (((uint16_t)frameBuffer[10] >> 5) | ((uint16_t)frameBuffer[11] << 3)) & 0x07FF;
    channels[8]  = ((uint16_t)frameBuffer[12]       | ((uint16_t)frameBuffer[13] << 8)) & 0x07FF;
    channels[9]  = (((uint16_t)frameBuffer[13] >> 3) | ((uint16_t)frameBuffer[14] << 5)) & 0x07FF;
    channels[10] = (((uint16_t)frameBuffer[14] >> 6) | ((uint16_t)frameBuffer[15] << 2) | (((uint16_t)frameBuffer[16] & 0x01) << 10)) & 0x07FF;
    channels[11] = (((uint16_t)frameBuffer[16] >> 1) | ((uint16_t)frameBuffer[17] << 7)) & 0x07FF;
    channels[12] = (((uint16_t)frameBuffer[17] >> 4) | ((uint16_t)frameBuffer[18] << 4)) & 0x07FF;
    channels[13] = (((uint16_t)frameBuffer[18] >> 7) | ((uint16_t)frameBuffer[19] << 1) | (((uint16_t)frameBuffer[20] & 0x03) << 9)) & 0x07FF;
    channels[14] = (((uint16_t)frameBuffer[20] >> 2) | ((uint16_t)frameBuffer[21] << 6)) & 0x07FF;
    channels[15] = (((uint16_t)frameBuffer[21] >> 5) | ((uint16_t)frameBuffer[22] << 3)) & 0x07FF;
    toChannels = 0;
}

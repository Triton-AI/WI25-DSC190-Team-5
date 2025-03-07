#ifndef VESC_CAN_TOOLS_HPP_
#define VESC_CAN_TOOLS_HPP_

#include <cstdint>
#include <iostream>
#include <map>
#include "mbed.h"
#include "config.hpp"


namespace tritonai::gkc {

    // Initialize CAN interfaces with the corresponding RX/TX pins and baud rates.
    CAN can1(CAN1_RX, CAN1_TX, CAN1_BAUDRATE);
    CAN can2(CAN2_RX, CAN2_TX, CAN2_BAUDRATE);

    /**
     * @brief Transmits a CAN message with an extended ID.
     *
     * This function dynamically allocates a CANMessage with the given
     * id, data, and length, and then attempts to send it using the CAN2 interface.
     * If the transmission fails, it resets the interface and sets the correct baud rate.
     *
     * @param id The extended identifier for the CAN message.
     * @param data Pointer to the data payload.
     * @param len Number of bytes in the data payload.
     */
    static void can_transmit_eid(uint32_t id, const uint8_t *data, uint8_t len) {
        CANMessage *cMsg;
        cMsg = new CANMessage(id, data, len, CANData, CANExtended);

        if (!can2.write(*cMsg)) {
            can2.reset();
            can2.frequency(CAN2_BAUDRATE);

        }
        delete cMsg;

    }

    typedef enum {
        CAN_PACKET_SET_DUTY = 0,
        CAN_PACKET_SET_CURRENT,
        CAN_PACKET_SET_CURRENT_BRAKE,
        CAN_PACKET_SET_RPM,
        CAN_PACKET_SET_POS,
        CAN_PACKET_SET_CURRENT_REL = 10,
        CAN_PACKET_SET_CURRENT_BRAKE_REL,
        CAN_PACKET_SET_CURRENT_HANDBRAKE,
        CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
        CAN_PACKET_MAKE_ENUM_32_BITS = 0xFFFFFFFF,
    } CAN_PACKET_ID;

    /**
     * @brief Append a 16-bit integer to a buffer in big-endian format.
     *
     * This function writes the most significant byte first followed by the least
     * significant byte to the provided buffer at the position indicated by index.
     *
     * @param buffer The target buffer.
     * @param number The 16-bit integer to append.
     * @param index Pointer to the current buffer index; it is incremented accordingly.
     */
    void buffer_append_int16(uint8_t* buffer, int16_t number, int32_t *index) {
        buffer[(*index)++] = number >> 8;
        buffer[(*index)++] = number;
    }

    /**
     * @brief Append a 32-bit integer to a buffer in big-endian format.
     *
     * This function writes the 32-bit integer byte-by-byte starting from the most
     * significant byte to the least significant byte.
     *
     * @param buffer The target buffer.
     * @param number The 32-bit integer to append.
     * @param index Pointer to the current buffer index; it is incremented accordingly.
     */
    void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index) {
        buffer[(*index)++] = number >> 24;
        buffer[(*index)++] = number >> 16;
        buffer[(*index)++] = number >> 8;
        buffer[(*index)++] = number;
    }

    /**
     * @brief Append a scaled float as a 16-bit integer to a buffer.
     *
     * The float is multiplied by the provided scale factor, converted to an int16,
     * and appended to the buffer in big-endian format.
     *
     * @param buffer The target buffer.
     * @param number The float value to be scaled and appended.
     * @param scale The scale factor to apply before conversion.
     * @param index Pointer to the current buffer index; it is incremented accordingly.
     */
    void buffer_append_float16(uint8_t* buffer, float number, float scale, int32_t *index) {
        buffer_append_int16(buffer, (int16_t)(number * scale), index);
    }

    /**
     * @brief Append a scaled float as a 32-bit integer to a buffer.
     *
     * The float is multiplied by the provided scale factor, converted to an int32,
     * and appended to the buffer in big-endian format.
     *
     * @param buffer The target buffer.
     * @param number The float value to be scaled and appended.
     * @param scale The scale factor to apply before conversion.
     * @param index Pointer to the current buffer index; it is incremented accordingly.
     */
    void buffer_append_float32(uint8_t* buffer, float number, float scale, int32_t *index) {
        buffer_append_int32(buffer, (int32_t)(number * scale), index);
    }

    // Message sending functions.

    /**
     * @brief Sends a CAN message to set the motor duty cycle.
     *
     * This function prepares a 4-byte buffer containing the duty cycle (scaled)
     * and sends it over CAN using the CAN_PACKET_SET_DUTY identifier.
     *
     * @param controller_id The CAN controller identifier.
     * @param duty The duty cycle value to set.
     */
    void comm_can_set_duty(uint8_t controller_id, float duty) {
        int32_t send_index = 0;
        uint8_t buffer[4];
        buffer_append_int32(buffer, (int32_t)(duty * 100000.0), &send_index);
        can_transmit_eid(controller_id |
                ((uint32_t)CAN_PACKET_SET_DUTY << 8), buffer, send_index);
    }

    /**
     * @brief Sends a CAN message to set the motor current.
     *
     * This function prepares a 4-byte buffer containing the current (scaled) and
     * sends it over CAN using the CAN_PACKET_SET_CURRENT identifier.
     *
     * @param controller_id The CAN controller identifier.
     * @param current The current value to set.
     */
    void comm_can_set_current(uint8_t controller_id, float current) {
        int32_t send_index = 0;
        uint8_t buffer[4];
        buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
        can_transmit_eid(controller_id |
                ((uint32_t)CAN_PACKET_SET_CURRENT << 8), buffer, send_index);
    }

    /**
     * @brief Sends a CAN message to set the motor brake current.
     *
     * This function prepares a 4-byte buffer containing the brake current (scaled)
     * and sends it over CAN using the CAN_PACKET_SET_CURRENT_BRAKE identifier.
     *
     * @param controller_id The CAN controller identifier.
     * @param current The brake current value to set.
     */
    void comm_can_set_current_brake(uint8_t controller_id, float current) {
        int32_t send_index = 0;
        uint8_t buffer[4];
        buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
        can_transmit_eid(controller_id |
                ((uint32_t)CAN_PACKET_SET_CURRENT_BRAKE << 8), buffer, send_index);
    }

    /**
     * @brief Sends a CAN message to set the motor RPM.
     *
     * This function prepares a 4-byte buffer containing the RPM value and sends it
     * over CAN using the CAN_PACKET_SET_RPM identifier.
     *
     * @param controller_id The CAN controller identifier.
     * @param rpm The RPM value to set.
     */
    void comm_can_set_rpm(uint8_t controller_id, float rpm) {
        int32_t send_index = 0;
        uint8_t buffer[4];
        buffer_append_int32(buffer, (int32_t)rpm, &send_index);
        can_transmit_eid(controller_id |
                ((uint32_t)CAN_PACKET_SET_RPM << 8), buffer, send_index);
    }

    /**
     * @brief Sends a CAN message to set the motor position.
     *
     * The function converts the provided position to a scaled value, applies an inversion,
     * and then transmits it using the CAN_PACKET_SET_POS identifier.
     *
     * @param controller_id The CAN controller identifier.
     * @param pos The position value to set.
     */
    void comm_can_set_pos(uint8_t controller_id, float pos) {
        int32_t send_index = 0;
        uint8_t buffer[4];
        buffer_append_int32(buffer, (int32_t)(-1.0*pos * 1000000.0), &send_index);
        can_transmit_eid(controller_id |
                ((uint32_t)CAN_PACKET_SET_POS << 8), buffer, send_index);
    }

    /**
     * @brief Sends a CAN message to set a relative motor current.
     *
     * This function prepares a 4-byte buffer containing the relative current (scaled)
     * and sends it over CAN using the CAN_PACKET_SET_CURRENT_REL identifier.
     *
     * @param controller_id The CAN controller identifier.
     * @param current_rel The relative current value to set.
     */
    void comm_can_set_current_rel(uint8_t controller_id, float current_rel) {
        int32_t send_index = 0;
        uint8_t buffer[4];
        buffer_append_float32(buffer, current_rel, 1e5, &send_index);
        can_transmit_eid(controller_id |
                ((uint32_t)CAN_PACKET_SET_CURRENT_REL << 8), buffer, send_index);
    }

    /**
     * @brief Sends a CAN message to set the motor current with an off delay.
     *
     * Same as above, but also sets the off delay. Note that this command uses 6 bytes now. 
     * The off delay is useful to set to keep the current controller running for a while even 
     * after setting currents below the minimum current.
     *
     * @param controller_id The CAN controller identifier.
     * @param current The current value to set.
     * @param off_delay The off delay time.
     */
    void comm_can_set_current_off_delay(uint8_t controller_id, float current, float off_delay) {
        int32_t send_index = 0;
        uint8_t buffer[6];
        buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
        buffer_append_float16(buffer, off_delay, 1e3, &send_index);
        can_transmit_eid(controller_id |
                ((uint32_t)CAN_PACKET_SET_CURRENT << 8), buffer, send_index);
    }

    /**
     * @brief Sends a CAN message to set a relative motor current with an off delay.
     *
     * This function prepares a 6-byte buffer containing the scaled relative current and off delay.
     *
     * @param controller_id The CAN controller identifier.
     * @param current_rel The relative current value to set.
     * @param off_delay The off delay time.
     */
    void comm_can_set_current_rel_off_delay(uint8_t controller_id, float current_rel, float off_delay) {
        int32_t send_index = 0;
        uint8_t buffer[6];
        buffer_append_float32(buffer, current_rel, 1e5, &send_index);
        buffer_append_float16(buffer, off_delay, 1e3, &send_index);
        can_transmit_eid(controller_id |
                ((uint32_t)CAN_PACKET_SET_CURRENT_REL << 8), buffer, send_index);
    }

    /**
     * @brief Sends a CAN message to set a relative brake current.
     *
     * This function prepares a 4-byte buffer containing the relative brake current (scaled)
     * and sends it over CAN using the CAN_PACKET_SET_CURRENT_BRAKE_REL identifier.
     *
     * @param controller_id The CAN controller identifier.
     * @param current_rel The relative brake current value to set.
     */
    void comm_can_set_current_brake_rel(uint8_t controller_id, float current_rel) {
        int32_t send_index = 0;
        uint8_t buffer[4];
        buffer_append_float32(buffer, current_rel, 1e5, &send_index);
        can_transmit_eid(controller_id |
                ((uint32_t)CAN_PACKET_SET_CURRENT_BRAKE_REL << 8), buffer, send_index);
    }

    /**
     * @brief Sends a CAN message to set the handbrake current.
     *
     * This function prepares a 4-byte buffer containing the handbrake current (scaled)
     * and sends it over CAN using the CAN_PACKET_SET_CURRENT_HANDBRAKE identifier.
     *
     * @param controller_id The CAN controller identifier.
     * @param current The handbrake current value to set.
     */
    void comm_can_set_handbrake(uint8_t controller_id, float current) {
        int32_t send_index = 0;
        uint8_t buffer[4];
        buffer_append_float32(buffer, current, 1e3, &send_index);
        can_transmit_eid(controller_id |
                ((uint32_t)CAN_PACKET_SET_CURRENT_HANDBRAKE << 8), buffer, send_index);
    }

    /**
     * @brief Sends a CAN message to set a relative handbrake current.
     *
     * This function prepares a 4-byte buffer containing the relative handbrake current (scaled)
     * and sends it over CAN using the CAN_PACKET_SET_CURRENT_HANDBRAKE_REL identifier.
     *
     * @param controller_id The CAN controller identifier.
     * @param current_rel The relative handbrake current value to set.
     */
    void comm_can_set_handbrake_rel(uint8_t controller_id, float current_rel) {
        int32_t send_index = 0;
        uint8_t buffer[4];
        buffer_append_float32(buffer, current_rel, 1e5, &send_index);
        can_transmit_eid(controller_id |
                ((uint32_t)CAN_PACKET_SET_CURRENT_HANDBRAKE_REL << 8), buffer, send_index);
    }

    /**
     * @brief Sets the motor speed based on a given speed in m/s.
     *
     * This function converts a speed in meters per second to the equivalent electrical RPM (erpm)
     * using motor and gear ratios along with the wheel circumference. It then sends a CAN message
     * to set the RPM.
     *
     * @param speed_ms Speed in meters per second.
     */
    void comm_can_set_speed(float speed_ms) { // in m/s
        float motor_poles = 5.0;
        float gear_ratio = 59.0/22.0;
        float wheel_circumference = 0.85; // in meters
        float speed_to_erpm = speed_ms * motor_poles * gear_ratio / wheel_circumference * 60.0 ;
        // std::cout << "Speed to erpm: " << (int)(speed_to_erpm) << std::endl;
        // std::cout << "speed: " << (int)(speed_ms*60*60/1000) << endl;
        comm_can_set_rpm(THROTTLE_CAN_ID, speed_to_erpm);
    }

    /**
     *
     * @brief Clamp a value between a minimum and maximum range
     * This function takes a value and clamps it to a specified range between a
     * minimum and maximum value.
     * @tparam T The type of the value to clamp (must be comparable with the <
     * operator and support assignment)
     * @param val The value to clamp
     * @param min The minimum value of the range to clamp to
     * @param max The maximum value of the range to clamp to
     * @return The value clamped to the specified range between min and max
     */
    template <typename T>
    constexpr T clamp(const T &val, const T &min, const T &max) {
        if (val > max) {
            return max;
        } else if (val < min) {
            return min;
        } else {
            return val;
    }
    }

    /**
     *
     * @brief Map a value from one range to another range
     * This function takes a value from a source range and maps it to a destination
     * range. The input value is first clamped to the source range between
     * source_min and source_max. Then, it is linearly interpolated to a value
     * between 0 and 1, where 0 represents the minimum of the source range and 1
     * represents the maximum of the source range. This normalized value is then
     * linearly interpolated to a value between dest_min and dest_max, which
     * represents the corresponding value in the destination range. The resulting
     * value is then returned.
     * @tparam S The type of the input source value to map (must be comparable with
     * the < operator and support assignment)
     * @tparam D The type of the mapped output destination value (must support
     * assignment)
     * @param source The input value to map to the destination range
     * @param source_min The minimum value of the source range
     * @param source_max The maximum value of the source range
     * @param dest_min The minimum value of the destination range
     * @param dest_max The maximum value of the destination range
     * @return The input value mapped to the destination range
     */
    template <typename S, typename D>
    constexpr D map_range(const S &source, const S &source_min, const S &source_max,
                        const D &dest_min, const D &dest_max) {
        // Clamp the input value to the source range between source_min and source_max
        float source_f = clamp<S>(source, source_min, source_max);

        // Normalize the clamped value between 0 and 1 based on its position within
        // the source range
        source_f = (source_f - source_min) / (source_max - source_min);

        // Map the normalized value to the destination range
        return static_cast<D>(source_f * (dest_max - dest_min) + dest_min);
    }


    /**
     *
     * @brief Map steering angle to motor angle
     * This function maps the steering angle in radians to the corresponding motor
     * angle in radians, based on a predefined lookup table. The lookup table is a
     * map between motor angles and steering angles, obtained through empirical
     * measurements of the relationship between the two angles. The function finds
     * the closest two entries in the map that bound the input steering angle and
     * linearly interpolates the motor angle between these two entries using the
     * map_range() function. The resulting motor angle is then returned.
     *
     * This look up table is defined in config.hpp as STERING_MAPPTING
     *
     * @param steer_angle The steering angle in radians to map to a motor angle
     * @return The corresponding motor angle in radians
     */
    float map_steer2motor(float steer_angle) {
        // Define the lookup table as a map between motor angles and steering angles
        std::map<float, float> mapping = STERING_MAPPING;

        // Save sign
        int sign = steer_angle >= 0 ? 1 : -1;

        // Find the two entries in the map that bound the input steering angle and
        // linearly interpolate between them
        for (auto it = mapping.begin(); it != mapping.end(); it++) {
            if ((std::next(it))->second >= sign * steer_angle) {

            auto returned =
                sign * map_range<float, float>(sign * steer_angle, it->second,
                                                std::next(it)->second, it->first,
                                                std::next(it)->first);

            return returned;
            }
        }

        // If the input steering angle is out of bounds of the look-up table, return 0
        return 0;
    }

    /**
     * @brief Sets the motor speed based on a given speed in m/s.
     *
     * This function converts a speed in meters per second to the equivalent electrical RPM (erpm)
     * using motor and gear ratios along with the wheel circumference. It then sends a CAN message
     * to set the RPM.
     *
     * @param speed_ms Speed in meters per second.
     */
    void comm_can_set_angle(float steer_angle) 
    { // in radians

        float motor_angle = steer_angle*4.0+MOTOR_OFFSET;
        float rad_to_deg = 180.0 / 3.14159265358979323846*motor_angle;
        comm_can_set_pos(STEER_CAN_ID, rad_to_deg);
    }

     /**
     * @brief Sends a CAN message to set the brake position.
     *
     * This function clamps the brake position between 0.0 and 1.0, scales it to the range
     * defined by MIN_BRAKE_VAL and MAX_BRAKE_VAL, and then constructs and sends an 8-byte CAN
     * message with the appropriate format.
     *
     * @param brake_position The normalized brake position (0.0 to 1.0).
     */
    void comm_can_set_brake_position(float brake_position) {
        if(brake_position<0.0) {
            brake_position = 0.0;
        }
        else if(brake_position>1.0) {
            brake_position = 1.0;
        }
        // Change 0.0 to 1.0 from unsigned int 0 to 2000
        unsigned int pos = (unsigned int)(brake_position*(MAX_BRAKE_VAL - MIN_BRAKE_VAL)) + MIN_BRAKE_VAL;
        static unsigned char buffer[8] = {0x0F, 0x4A, 0x00, 0xC0, 0, 0, 0, 0};
        
        buffer[2] = pos & 0xFF;
        buffer[3] = 0xC0 | ((pos >> 8) & 0x1F);

        can_transmit_eid(BRAKE_CAN_ID, buffer, 8);
        
    }
        
} // namespace tritonai::gkc

#endif // VESC_CAN_TOOLS_HPP_
/**
 * @file config.hpp
 * @author Haoru Xue (haoru.xue@autoware.org)
 * @brief
 * @version 0.1
 * @date 2022-02-11
 *
 * @copyright Copyright 2022 Triton AI
 *
 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

// *************
// Build Options
// *************

// *************
// Communication
// *************
// Choose one of the available interfaces
//#define COMM_USB_SERIAL
#define COMM_UART_SERIAL
//#define COMM_ETHERNET  // not implemented
//#define COMM_CAN  // not implemented

// UART-specific settings
#define BAUD_RATE 115200
#define UART_RX_PIN PB_12
#define UART_TX_PIN PB_13
//#define UART_RX_PIN PE_7
//#define UART_TX_PIN PE_8
#define REMOTE_UART_TX_PIN PE_7 // 14th pin, 1st pin on DuraClik, UART7_RX, ELRS_TX
#define REMOTE_UART_RX_PIN PE_8 // 16th pin, 2nd pin on DuraClik, UART7_TX, ELRS_RX

// Vesc disable pins
#define THROTTLE_VESC_DISABLE_PIN PD_14
#define STEERING_VESC_DISABLE_PIN PD_12

// Tower light pins
#define TOWER_LIGHT_RED PD_15
#define TOWER_LIGHT_YELLOW PD_11
#define TOWER_LIGHT_GREEN PE_12

// Generic comm settings
#define RECV_BUFFER_SIZE 32
// millisecond to wait between each serial/ethernet/can read
#define WAIT_READ_MS 5
// outbound packet queue size
#define SEND_QUEUE_SIZE 10
// interval of sending sensor packets
#define SEND_SENSOR_INTERVAL_MS 50

// *********
// Watchdogs
// *********
// What's the update frequency of the watchdog
#define DEFAULT_WD_INTERVAL_MS 1000
#define DEFAULT_WD_MAX_INACTIVITY_MS 3000
#define DEFAULT_WD_WAKEUP_INTERVAL_MS 2
// How often should the MCU send heartbeat by default
#define DEFAULT_MCU_HEARTBEAT_INTERVAL_MS 1000
#define DEFAULT_MCU_HEARTBEAT_LOST_TOLERANCE_MS 2000
// How often should the MCU expect heartbeat from PC by default
#define DEFAULT_PC_HEARTBEAT_INTERVAL_MS 1000
#define DEFAULT_PC_HEARTBEAT_LOST_TOLERANCE_MS 2000
// In active mode, how often should the MCU expect control command from PC
#define DEFAULT_CTL_CMD_INTERVAL_MS 10
#define DEFAULT_CTL_CMD_LOST_TOLERANCE_MS 200
// How often should actuation be active
#define DEFAULT_ACTUATION_INTERVAL_MS 1000
#define DEFAULT_ACTUATION_LOST_TOLERANCE_MS 2000
// How often should sensor polling happen
#define DEFAULT_SENSOR_POLL_INTERVAL_MS 1000
#define DEFAULT_SENSOR_POLL_LOST_TOLERANCE_MS 3000
// How often shoudl the communication manager polling happen
#define DEFAULT_COMM_POLL_INTERVAL_MS 1000
#define DEFAULT_COMM_POLL_LOST_TOLERANCE_MS 3000
// How ofteh the controller should be checked by watchdog
#define DEFAULT_CONTROLLER_POLL_INTERVAL_MS 1000
#define DEFAULT_CONTROLLER_POLL_LOST_TOLERANCE_MS 3000
// How often should the RCCOntroller be checked by watchdog
#define DEFAULT_RC_CONTROLLER_POLL_INTERVAL_MS 100
#define DEFAULT_RC_CONTROLLER_POLL_LOST_TOLERANCE_MS 3000
// How often should RC Heartbeat be checked by watchdog
#define DEFAULT_RC_HEARTBEAT_INTERVAL_MS 100
#define DEFAULT_RC_HEARTBEAT_LOST_TOLERANCE_MS 500
#define RC_TAKEOVER_INTERVAL_MS 100


// *********
// Actuation
// *********
#define CAN1_RX PD_0
#define CAN1_TX PD_1
#define CAN1_BAUDRATE 500000
#define CAN2_RX PB_5
#define CAN2_TX PB_6
#define CAN2_BAUDRATE 500000
// Throttle
// #define THROTTLE_PWM_PIN PA_6
#define CAN_THROTTLE CAN_2
#define MAX_THROTTLE_SPEED_ERPM 2000
#define MAX_THROTTLE_CURRENT_MA 5000
#define MAX_THROTTLE_MS 10
#define CONST_ERPM2MS 0.0000961111108
//#define THROTTLE_ERPM_TO_RPS_RATIO 0.1
#define THROTTLE_VESC_ID 1
// Braking
#define CAN_BRAKE CAN_1 // Which CAN bus to use for brake [CAN_1 | CAN_2]
#define MAX_BRAKE_VAL 3000
#define MIN_BRAKE_VAL 600
// Steering
#define CAN_STEER CAN_2 // Which CAN bus to use for steer [CAN_1 | CAN_2]
#define MAX_STEER_DEG 100.0
#define MIN_STEER_DEG -100.0
#define VIRTUAL_LIMIT_OFF 5
#define NEUTRAL_STEER_DEG 0.0
#define STEERING_CAL_OFF 0 //this changes the calibration angle
#define MAX_STEER_SPEED_ERPM 50000
#define MAX_STEER_SPEED_MA 1 //this controls the max steering current i.e strength 
#define MIN_STEER_SPEED_MA -1 //this controls the max steering current i.e strength 

#define STEERING_CALIB_CURRENT 2700 // mA

#define MAX_STEER_CURRENT_MA 24000 //this controls the max steering current i.e strength 
#define MIN_STEER_CURRENT_MA -32000 //this controls the max steering current i.e strength 
//Good configuration for current PID
// #define STEER_P 30000.0
// #define STEER_I 7000
// #define STEER_D 2000//5000.0
//#define STEADY_STATE_CURRENT_MULT 20000
//Good configuration for current PID in the air
#define STEER_P 30000
#define STEER_I 5000
#define STEER_D 3000//5000.0
#define STEADY_STATE_CURRENT_MULT_POS 18000
#define STEADY_STATE_CURRENT_MULT_NEG 25000
//good configuration for RPM PID
// #define STEER_P 25000.0
// #define STEER_I 0.0
// #define STEER_D 100
// #define STEADY_STATE_CURRENT_MULT 0
#define STEER_DEADBAND_DEG 0.5 //VESC already has a limit of min ERPM := 600. Enything bellow this is already used as 0.
#define PID_INTERVAL_MS 10
#define STEER_VESC_ID 2
#define RIGHT_LSWITCH PF_0
#define LEFT_LSWITCH PF_1
#define ENABLE_LSWITCH      //comment to remove limit switches behaviour

// Throttle
#define THROTTLE_CAN_PORT  2 // To which can port should the throttle be sent
#define THROTTLE_CAN_ID 1    // To which can port should the throttle be sent
#define THROTTLE_MAX_REVERSE_SPEED 20.0 // m/s
#define THROTTLE_MAX_FORWARD_SPEED 20.0 // m/s
#define RC_MAX_SPEED_FORWARD 20.0 // m/s
#define RC_MAX_SPEED_REVERSE 5.0 // m/s

// Steering
#define STEER_CAN_PORT  2 // To which can port should the throttle be sent
#define STEER_CAN_ID 2    // To which can port should the throttle be sent

// Brake
#define BRAKE_CAN_PORT  1 // To which can port should the throttle be sent
#define BRAKE_CAN_ID 0x00FF0000    // To which can port should the throttle be sent

#define EMERGENCY_BRAKE_PRESSURE 1.0 // 0.5 bar

// *******
// Sensors
// *******
// PWM steering encoder
#define STEER_ENCODER_PIN PC_7

// motor angle - left wheel - right wheel - average (in degrees)
// 0	0	0	0
// 30	8	10	9
// 50	9	15	12
// 70	12	22	17
// 80	10	30	20
// 90           23
// 100          26
// 110          29

//(in radians)
#define STERING_MAPPING    {{0, 0,},\
                            {0.523599, 0.15708,},\
                            {0.872665, 0.20944,},\
                            {1.22173, 0.296706,},\
                            {1.39626, 0.349066,},\
                            {1.57079, 0.401425},\
                            {1.74532, 0.453785},\
                            {1.91986, 0.506145}}; // takes first and last column
#define MIN__WHEEL_STEER_DEG -20
#define MAX__WHEEL_STEER_DEG 20
#define MOTOR_OFFSET 0.3

// *****
// ESTOP
// *****
#define ESTOP_PIN PB_10

//PWM pins for RC car

#define Steer_Pin PA_5
#define Throttle_Pin PA_6
#define Red_Pin PD_12


//// ELRS
#define ELRS_THROTLE 1
#define ELRS_STEERING 3
#define ELRS_EMERGENCY_STOP_LEFT 4
#define ELRS_EMERGENCY_STOP_RIGHT 7
#define ELRS_TRI_SWITCH_RIGHT 6
#define ELRS_RATIO_THROTTLE 9
#define ELRS_HOLD_THROTTLE 8

#endif // CONFIG_HPP_

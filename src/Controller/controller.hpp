#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "Comm/comm.hpp"
#include "tai_gokart_packet/gkc_packet_subscriber.hpp"
#include "Watchdog/watchdog.hpp"
#include "Sensor/sensor_reader.hpp"
#include "Actuation/actuation_controller.hpp"
#include "RCController/RCController.hpp"
#include "StateMachine/state_machine.hpp"
#include <chrono>

namespace tritonai::gkc
{
  class Controller :
    public GkcPacketSubscriber,
    public Watchable,
    public ILogger,
    public GkcStateMachine
  {
    public:
      Controller();

      void agx_heartbeat();
      void update_lights();


    protected:
      // GkcPacketSubscriber API
      void packet_callback(const Handshake1GkcPacket & packet);
      void packet_callback(const Handshake2GkcPacket & packet);
      void packet_callback(const GetFirmwareVersionGkcPacket & packet);
      void packet_callback(const FirmwareVersionGkcPacket & packet);
      void packet_callback(const ResetRTCGkcPacket & packet);
      void packet_callback(const HeartbeatGkcPacket & packet);
      void packet_callback(const ConfigGkcPacket & packet);
      void packet_callback(const StateTransitionGkcPacket & packet);
      void packet_callback(const ControlGkcPacket & packet);
      void packet_callback(const SensorGkcPacket & packet);
      void packet_callback(const Shutdown1GkcPacket & packet);
      void packet_callback(const Shutdown2GkcPacket & packet);
      void packet_callback(const LogPacket & packet);
      void packet_callback(const RCControlGkcPacket & packet);

      // ILogger API
      void send_log(const LogPacket::Severity &severity, 
                    const std::string &what) override;
      LogPacket::Severity _severity;

      // Watchable API
      void watchdog_callback();

      // GkcStateMachine API
      StateTransitionResult on_initialize(const GkcLifecycle &last_state) override;
      StateTransitionResult on_deactivate(const GkcLifecycle &last_state) override;
      StateTransitionResult on_activate(const GkcLifecycle &last_state) override;
      StateTransitionResult on_emergency_stop(const GkcLifecycle &last_state) override;
      StateTransitionResult on_reinitialize(const GkcLifecycle &last_state) override;

    private:
      CommManager _comm;
      Watchdog _watchdog;
      SensorReader _sensor_reader;
      ActuationController _actuation;
      RCController _rc_controller;

      Thread _keep_alive_thread{osPriorityNormal, OS_STACK_SIZE, nullptr, "keep_alive_thread"};
      bool _rc_commanding{false};
      std::chrono::time_point<std::chrono::steady_clock> _last_rc_command=std::chrono::steady_clock::now();
      Watchable _rc_heartbeat;
      void on_rc_disconnect();
      bool _stop_on_rc_disconnect{true};
      void set_actuation_values(float throttle, float steering, float brake);
      DigitalOut _led{LED1};
      DigitalOut _tower_light_red{TOWER_LIGHT_RED, 0};
      DigitalOut _tower_light_yellow{TOWER_LIGHT_YELLOW, 0};
      DigitalOut _tower_light_green{TOWER_LIGHT_GREEN, 0};
      DigitalOut _throttle_vesc_disable{THROTTLE_VESC_DISABLE_PIN, 0};
      DigitalOut _steering_vesc_disable{STEERING_VESC_DISABLE_PIN, 0};
      
      // Light control variables
      bool _rc_connected{false};
      bool _emergency_active{false};
      AutonomyMode _current_autonomy_mode{AUTONOMOUS};
      chrono::time_point<chrono::steady_clock> _last_light_toggle = chrono::steady_clock::now();
      bool _light_state{false}; // For flashing
  };
}

#endif // CONTROLLER_HPP
#ifndef CATAPULT_H_
#define CATAPULT_H_

#include <lib/motor_sensor/motor.hpp>
#include <lib/motor_sensor/sensor.hpp>

namespace catapult {

    extern bool in_macro;

    // motors/sensors
    extern int motors;
    extern Sensor limit_switch;

    // control data
    extern bool is_shooting;
    extern bool is_loaded;

    // initialozer
    void init();

    // control functions
    void fire();
    void waitForLoadPos(int timeout);

    // controllers
    void updateDriverControl();
    void update(int delta_time);

}

#endif

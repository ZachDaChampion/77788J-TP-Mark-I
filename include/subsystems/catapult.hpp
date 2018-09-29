#ifndef CATAPULT_H_
#define CATAPULT_H_

#include <lib/motor_sensor/motor.hpp>
#include <lib/motor_sensor/sensor.hpp>

namespace catapult {

    // motors/sensors
    extern int motors;
    extern Sensor limit_switch;

    // control data
    extern bool is_shooting;

    // initialozer
    void init();

    // controllers
    void updateDriverControl();
    void update();

}

#endif
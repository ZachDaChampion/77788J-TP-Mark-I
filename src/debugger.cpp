#include "controller.hpp"
#include "subsystems/subsystems.hpp"
#include "lib/motor_sensor/motor.hpp"

void debugTask() {
    printf("LIFT: %f\n", lift::angle);
}
#include "controller.hpp"
#include "main.hpp"
#include "lib/joystick.hpp"
#include "lib/motor_sensor/motor.hpp"
#include "lib/motor_sensor/sensor.hpp"

// task update intervals
#define TASK_SENSOR_INTERVAL 5
#define TASK_PHYSICAL_MOTOR_INTERVAL 15
#define TASK_CONTROL_INTERVAL 15

// variables to store tasks
TaskHandle sensor_task; // includes motor data updates
TaskHandle motor_task; // updates physical motors
TaskHandle control_task; // overall driver control

// global joystick reference
Joystick joystick;

// sensor task
void sensorTask() {
    for (int i = 0; i < all_motors_count; i++) {
        all_motors[i].updateStats(TASK_SENSOR_INTERVAL);
    }

    // custom sensors


    // custom PIDs

}

// motor task
void motorTask() {
    for (int i = 0; i < all_motors_count; i++) {
        all_motors[i].updatePhysical(TASK_PHYSICAL_MOTOR_INTERVAL);
    }
}

// driver control task
void controlTask() {
    // stuffs
}

// starts all background tasks
void startAllTasks() {
    sensor_task = taskRunLoop(sensorTask, TASK_SENSOR_INTERVAL);
    motor_task = taskRunLoop(motorTask, TASK_PHYSICAL_MOTOR_INTERVAL);
    control_task = taskRunLoop(controlTask, TASK_CONTROL_INTERVAL);
}
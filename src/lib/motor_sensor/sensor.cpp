#include "lib/motor_sensor/sensor.hpp"

// calculates the acceleration of a value given its history
float calcAccel(float *history, float prev_vel, float prev_accel, int delta_time) {
  float current = calcVel(history, prev_vel, delta_time) - prev_vel;
  current /= delta_time;
  return current * .5f + prev_accel * .5f;
}

// calculates the velocity of a value given its history
float calcVel(float *history, float prev_vel, int delta_time) {
  float current_diff =(*history - *(history + 1)) / 360.f;
  current_diff /= delta_time;
  current_diff *= 1000.f;
  current_diff *= 60.f;
  return current_diff * .25f + prev_vel * .75f;
}

// initialize sensor
void Sensor :: init(SensorType _type, int _port1, int _port2, bool _reversed, float _extra_data, void (*_customInit)(), float (*_customUpdate)()) {

  // set variables
  type = _type;
  port1 = _port1;
  port2 = _port2;
  reversed = _reversed;
  extra_data = _extra_data;
  customUpdate = _customUpdate;

  // init specific sensors
  switch (type) {

    // sensor_digital sensor
    case (sensor_digital): pinMode(port1, INPUT); break;
    
    // sensor_sonar sensor
    case (sensor_sonar): sonar_sensor = ultrasonicInit(port1, port2); break;

    // quadrature sensor_encoder
    case (sensor_encoder): encoder_sensor = encoderInit(port1, port2, false); break;

    // sensor_gyro
    case (sensor_gyro): gyro_sensor = gyroInit(port1, extra_data); break;

    // sensor_custom sensor
    case (sensor_custom): _customInit(); break;

    // added cause compiler warnings are annoying
    default: break;
  }
}

// returns the sensor value x intervals ago (0 is current)
float Sensor :: getValue(int x) {
  return (x >= 0 && x < SENSOR_HISTORY) ? history[x] : 0;
}

// get sensor statistics
float Sensor :: getVelocity() {return velocity;}
float Sensor :: getAcceleration() {return acceleration;}

// returns the type of sensor
SensorType Sensor :: getType() {
  return type;
}

// initializes history
  void Sensor :: initHistory(float x) {
    for (int i = 0; i < SENSOR_HISTORY; i++) {
      history[i] = x;
    }
    history_initialized = true;
  }

// adds x to front of history and pushes everything else back (removes oldest value)
void Sensor :: pushValue(float x) {
  for (int i = SENSOR_HISTORY - 1; i > 0; i--) {
    history[i] = history[i - 1];
  }

  history[0] = x;
}

// updates the history database with the latest sensor value
void Sensor :: update(int delta_time) {

  // inits value variable
  float value = 0;

  // different code for different sensors
  switch (type) {

    // standard sensor_digital input
    case (sensor_digital): {
      value = digitalRead(port1) == LOW;
      if (reversed) value = 1.f - value; // invert if reversed
    } break;

    // sensor_sonar sensor (cm)
    case (sensor_sonar): {
      value = ultrasonicGet(sonar_sensor);
      if (reversed) value = SONAR_MAX - value + SONAR_MIN; // invert if reversed
      value *= extra_data;
    } break;

    // quadrature sensor_encoder (deg)
    case (sensor_encoder): {
      value = (float) encoderGet(encoder_sensor) / (float) extra_data;
      if (reversed) value *= -1.f; // reverse if reversed
    } break;

    // sensor_ime (deg)
    case (sensor_ime): {
      int val; // cause IMEs are stupid and can't be stored in floats
      imeGet(port1, &val);
      value = (float) val * 360.f / (float) extra_data;
      if (reversed) value *= -1.f; // reverse if reversed;
    } break;

    // sensor_potentiometer (deg)
    case (sensor_potentiometer): {
      value = (float) analogRead(port1) * (260.f/4095.f) / (float) extra_data;
      if (reversed) value = 260.f - value; // invert if reversed
    } break;

    // sensor_gyro (deg)
    case (sensor_gyro): {
      value = gyroGet(gyro_sensor);
      if (reversed) value *= -1.f; // reverse if reversed;
    } break;

    // sensor_light sensor/line follower
    case (sensor_light): {
      value = analogRead(port1);
      if (reversed) value = 4095.f - value; // invert if reversed
    } break;

    // sensor_custom sensor
    case (sensor_custom): {
      value = customUpdate();
    } break;

    default: print("wtf"); break;
  }

  // push new value to history
  if (history_initialized) pushValue(value);
  else initHistory(value);

  // update sensor statistics
  acceleration = calcAccel(history, velocity, acceleration, delta_time);
  velocity = calcVel(history, velocity, delta_time);
}

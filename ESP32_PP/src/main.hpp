#include <Arduino.h>
#include <driver/adc.h>
#include "BluetoothSerial.h"
#include "motor.hpp"
#include "sensor_presence.hpp"
#include "nvs_manager.hpp"

#define PIN_LED 5
void init_all_param();
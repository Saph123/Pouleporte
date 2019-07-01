#include <Arduino.h>
#include "sensor_presence.hpp"
#include "nvs_manager.hpp"
void upCmd(bool door_open, int time_up, boolean use_sensors);
int downCmd(bool door_open, int time_down, boolean use_sensors);
void stopMotor();
void tuningUp();
void tuningDown();

#define CAPTEUR_DU_HAUT 1
#define CAPTEUR_DU_BAS 0

const int H_A_Pin = 16;
const int H_B_Pin = 21;
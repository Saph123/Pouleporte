#include <Arduino.h>
void upCmd(bool door_open, int time_up);
void downCmd(bool door_open, int time_down);
void tuningUp();
void tuningDown();


const int H_A_Pin = 16;
const int H_B_Pin = 21;
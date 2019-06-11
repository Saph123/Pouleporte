#include "sensor_presence.hpp"

void initSensor()
{
  adc2_config_channel_atten(ADC2_CHANNEL_0,ADC_ATTEN_DB_11); // configure the ADC2 ch0
}
int getVal(int sensor)
{
  if(sensor == 1)
  {
  int val_sensor1 = 0;
  adc2_get_raw(ADC2_CHANNEL_0,ADC_WIDTH_BIT_12, &val_sensor1);
  return val_sensor1;
  }
  return 0;
}
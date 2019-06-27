#include "sensor_presence.hpp"

void initSensor()
{
  adc2_config_channel_atten(ADC2_CHANNEL_0, ADC_ATTEN_DB_11); // configure the ADC2 ch0 D4
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11); // configure the ADC1 ch6 D34
}
int getVal(int sensor)
{
  int val_sensor = 0;
  if(sensor == 1)
  {
  adc2_get_raw(ADC2_CHANNEL_0,ADC_WIDTH_BIT_12, &val_sensor);
  return val_sensor;
  }
  else if(sensor == 0)
  {
    // capteur du haut sur D34
    // adc2_get_raw(ADC2_CHANNEL_2,ADC_WIDTH_BIT_12, &val_sensor);
    val_sensor = adc1_get_raw(ADC1_CHANNEL_6);
    return val_sensor;
  }
  return 0;
}
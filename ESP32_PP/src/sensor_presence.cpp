#include "sensor_presence.hpp"
void initSensor()
{
  adc2_config_channel_atten(ADC2_CHANNEL_0,ADC_ATTEN_DB_11); // configure the ADC2 ch0
}
int getVal();
#ifndef TEMPERATURE_CONTROL_H
#define TEMPERATURE_CONTROL_H

#include "SystemStatus.h"
#include <max6675.h>
#include <LiquidCrystal_I2C.h>


void updateRelayState(int temp, SystemStatus& sysStat);
int getCalibratedTemp(MAX6675& thermocouple, SystemStatus& sysStat);
void controlTemperature(SystemStatus& sysStat);
void resetSystem(SystemStatus& sysStat);
void calculateMovingAverage(int temp, SystemStatus& sysStat);
void collectSample(SystemStatus& sysStat);


#endif

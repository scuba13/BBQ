#ifndef BBQ_MONITOR_H
#define BBQ_MONITOR_H

#include <RotaryEncoder.h>
#include <max6675.h>
#include "SystemStatus.h"
#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>

void monitor(SystemStatus& sysStat, LiquidCrystal_I2C& lcd, Bounce& encoderButton, RotaryEncoder& encoder);
void loopDisplayMenuAndMonitor(SystemStatus& sysStat, bool bbqControl, bool monitorMode, RotaryEncoder& encoder, Bounce& encoderButton, LiquidCrystal_I2C& lcd);

#endif  // BBQ_MONITOR_H

#pragma once

#include "SystemStatus.h"
#include <RotaryEncoder.h>
#include <Bounce2.h>
#include <LiquidCrystal_I2C.h>

void setNumericValue(int& value, int minValue, int maxValue, const char* message, SystemStatus& sysStat, RotaryEncoder& encoder, Bounce& encoderButton, LiquidCrystal_I2C& lcd);
void displayMenu(SystemStatus& sysStat, RotaryEncoder& encoder, Bounce& encoderButton, LiquidCrystal_I2C& lcd);
void returnMenu(SystemStatus& sysStat, RotaryEncoder& encoder, Bounce& encoderButton, LiquidCrystal_I2C& lcd);

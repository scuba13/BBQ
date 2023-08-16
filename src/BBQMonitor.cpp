#include "BBQMonitor.h"
#include "TemperatureControl.h"
#include "MenuControl.h"
#include <Arduino.h>


void loopDisplayMenuAndMonitor(SystemStatus& sysStat, bool bbqControl, bool monitorMode, RotaryEncoder& encoder, Bounce& encoderButton, LiquidCrystal_I2C& lcd) {
  if (sysStat.bbqTemperature > 0 && bbqControl) {
    controlTemperature(sysStat);
  }

  if (monitorMode) {
    monitor(sysStat, lcd, encoderButton, encoder);
  } else {
    displayMenu(sysStat, encoder, encoderButton, lcd);
  }
}


void monitor(SystemStatus& sysStat, LiquidCrystal_I2C& lcd, Bounce& encoderButton, RotaryEncoder& encoder) {

  if (sysStat.lastTempUpdateMonitor == 0 || millis() - sysStat.lastTempUpdateMonitor >= 1000) {
    sysStat.lastTempUpdateMonitor = millis();
    
    lcd.setCursor(0, 0);
    lcd.print("Monitor BBQ Temps:");
    
    lcd.setCursor(0, 1);
    lcd.print("BBQ Temp: ");
    lcd.print(sysStat.calibratedTemp);
    lcd.print("C     ");

    lcd.setCursor(0, 2);
    lcd.print("Set Temp: ");
    lcd.print(sysStat.bbqTemperature);
    lcd.print("C     ");

    lcd.setCursor(0, 3);
    lcd.print("Fire:    ");
    lcd.setCursor(6, 3);
    lcd.print(sysStat.isRelayOn ? "ON " : "OFF");
    
    lcd.setCursor(12, 3);
    lcd.print("3H: ");
    lcd.print(sysStat.averageTemp);
    lcd.print("C");
  }

  if (encoderButton.update() && encoderButton.fell()) {
    delay(200);
    lcd.setCursor(0, 3);
    lcd.print("Return Menu         ");
    delay(500);
    sysStat.monitorMode = false;
    returnMenu(sysStat, encoder, encoderButton, lcd);
  }

  delay(10);
}



#include "MenuControl.h"
#include <Arduino.h>


const char* menu[] = { "Monitor BBQ", "Set Temp BBQ", "Calibrate Temp" };
const int menuLength = sizeof(menu) / sizeof(char*);



void setNumericValue(int& value, int minValue, int maxValue, const char* message, SystemStatus& sysStat, RotaryEncoder& encoder, Bounce& encoderButton, LiquidCrystal_I2C& lcd) {
   lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
  lcd.setCursor(0, 1);
  lcd.print("Value: ");
  lcd.print(value);
  lcd.print("     ");

  bool exitLoop = false;
  while (!exitLoop) {
    encoder.tick();
    int newPos = encoder.getPosition();

    if (newPos != sysStat.currentPos) {
      value += newPos - sysStat.currentPos;
      value = constrain(value, minValue, maxValue);
      lcd.setCursor(7, 1);
      lcd.print("     ");
      lcd.setCursor(7, 1);
      lcd.print(value);
      sysStat.currentPos = newPos;
    }

    if (encoderButton.update() && encoderButton.fell()) {
      delay(200);
      exitLoop = true;
    }
    delay(10);
  }
  lcd.setCursor(0, 3);
  lcd.print("Value Set");
returnMenu(sysStat, encoder, encoderButton, lcd);

}

void displayMenu(SystemStatus& sysStat, RotaryEncoder& encoder, Bounce& encoderButton, LiquidCrystal_I2C& lcd) {
  encoder.tick();
  sysStat.currentPos = encoder.getPosition();

  if (sysStat.currentPos > menuLength) { encoder.setPosition(1); }
  if (sysStat.currentPos < 1) { encoder.setPosition(menuLength); }

  if (sysStat.lastPos != sysStat.currentPos) {
    lcd.clear();
    for (int i = 0; i < menuLength; i++) {
      lcd.setCursor(0, i);
      if (i == sysStat.currentPos - 1)
        lcd.print("->");
      else
        lcd.print("  ");
      lcd.print(menu[i]);
    }
    sysStat.lastPos = sysStat.currentPos;
  }

  encoderButton.update();

  if (encoderButton.fell()) {
    lcd.clear();
    delay(200);
    switch (sysStat.currentPos) {
      case 1:
        sysStat.monitorMode = true;
        break;
      case 2:
        setNumericValue(sysStat.bbqTemperature, sysStat.minBBQTemp, sysStat.maxBBQTemp, "Set BBQ Temp", sysStat, encoder, encoderButton, lcd);
        break;
      case 3:
        setNumericValue(sysStat.tempCalibration, -20, 20, "Calibrate Temp", sysStat, encoder, encoderButton, lcd);
        break;
    }
  }
}

void returnMenu(SystemStatus& sysStat, RotaryEncoder& encoder,Bounce& encoderButton, LiquidCrystal_I2C& lcd) {
    delay(500);
  lcd.clear();
  sysStat.currentPos = 1;
  encoder.setPosition(sysStat.currentPos);
  sysStat.lastPos = -1;
}

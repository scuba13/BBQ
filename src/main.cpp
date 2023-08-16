#include <WiFiManager.h>
#include <RotaryEncoder.h>
#include <max6675.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>
#include "SystemStatus.h"
#include "BBQMonitor.h"
#include "PinDefinitions.h"
#include "WebServerControl.h"
#include "TemperatureControl.h"
#include <Arduino.h>

MAX6675 thermocouple(MAX6675_SCK, MAX6675_CS, MAX6675_SO);
LiquidCrystal_I2C lcd(0x27, 20, 4);
RotaryEncoder encoder(ENCODER_PIN1, ENCODER_PIN2, RotaryEncoder::LatchMode::TWO03);
Bounce encoderButton = Bounce();
SystemStatus sysStat;
WebServerControl webServerControl(sysStat);

void welcomeScreen() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("BBQ Controller");
  lcd.setCursor(7, 1);
  lcd.print("BY DU");

  if (WiFi.status() == WL_CONNECTED) {
    lcd.setCursor(0, 3);
    lcd.print("IP: ");
    lcd.print(WiFi.localIP());
  }

  delay(2000);  // delay to display the welcome screen and IP address
  lcd.clear();
}


void setup() {
  lcd.init();
  lcd.backlight();
  encoder.setPosition(1);
  Serial.begin(115200);

WiFiManager wifiManager;
if(wifiManager.autoConnect("BBQController")) {
    Serial.println("Conexão WiFi estabelecida!");
    Serial.println("O IP da ESP32 é: ");
    Serial.println(WiFi.localIP());
} else {
    Serial.println("Falha na conexão WiFi");
}

  
  encoderButton.attach(ENCODER_BUTTON, INPUT_PULLUP);
  encoderButton.interval(5);

  pinMode(RELAY_PIN, OUTPUT);
  webServerControl.begin();
  welcomeScreen();
  delay(500);
}

void loop() {
  getCalibratedTemp(thermocouple, sysStat);
  loopDisplayMenuAndMonitor(sysStat, true, sysStat.monitorMode, encoder, encoderButton, lcd);
 
}

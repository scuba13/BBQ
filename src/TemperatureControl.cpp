#include "TemperatureControl.h"
#include "PinDefinitions.h"
#include <Arduino.h>


int getCalibratedTemp(MAX6675 &thermocouple, SystemStatus &sysStat) {
  
  static unsigned long lastTempUpdate = 0;

  if (millis() - lastTempUpdate >= 50) {
    lastTempUpdate = millis();
    float temp = thermocouple.readCelsius() + sysStat.tempCalibration;
    sysStat.tempSamples[sysStat.nextSampleIndex] = temp;
    sysStat.nextSampleIndex = (sysStat.nextSampleIndex + 1) % NUM_SAMPLES;
    if (sysStat.numSamples < NUM_SAMPLES) {
      sysStat.numSamples++;
    }
  }

  float sum = 0;
  for (int i = 0; i < sysStat.numSamples; i++) {
    sum += sysStat.tempSamples[i];
  }

  // Atualiza a temperatura calibrada na estrutura sysStat
  sysStat.calibratedTemp = (int)round(sum / sysStat.numSamples);

  return sysStat.calibratedTemp;
}

void updateRelayState(int temp, SystemStatus &sysStat) {
  if (temp <= sysStat.bbqTemperature) {
    digitalWrite(RELAY_PIN, HIGH);
    sysStat.isRelayOn = true;
  } else if (temp > sysStat.bbqTemperature) {
    digitalWrite(RELAY_PIN, LOW);
    sysStat.isRelayOn = false;
  }
}

void controlTemperature(SystemStatus& sysStat) {
  static unsigned long lastTempUpdate = 0;
  if (millis() - lastTempUpdate < 1000) return;

  lastTempUpdate = millis();
  int temp = sysStat.calibratedTemp;
  
  // Verifica se a temperatura atingiu o valor configurado
  if (temp >= sysStat.bbqTemperature) {
    sysStat.hasReachedSetTemp = true;
  }

  // Inicia o cálculo da média apenas se a temperatura configurada já foi atingida
  if (sysStat.hasReachedSetTemp) {
    sysStat.startAverage = true;
  }

  updateRelayState(temp, sysStat);
  collectSample(sysStat);
}

void resetSystem(SystemStatus& sysStat) {
  digitalWrite(RELAY_PIN, LOW);
  sysStat.isRelayOn = false;
  sysStat.bbqTemperature = 0;

  // Reset variáveis de média
  sysStat.startAverage = false;
  sysStat.averageTemp = 0;
  sysStat.numSamples = 0;
  sysStat.hasReachedSetTemp = false;
  
  // Apaga as amostras
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sysStat.tempSamples[i] = 0;
  }
}


void addSample(int temp, SystemStatus& sysStat) {
  sysStat.samples[sysStat.sampleIndex] = temp;
  sysStat.sampleIndex = (sysStat.sampleIndex + 1) % MOVING_AVERAGE_SIZE;
  if (sysStat.avgNumSamples < MOVING_AVERAGE_SIZE) {
    sysStat.avgNumSamples++;
  }
}

void calculateAverage(SystemStatus& sysStat) {
  float sum = 0;
  for (int i = 0; i < sysStat.avgNumSamples; i++) {
    sum += sysStat.samples[i];
  }
  sysStat.averageTemp = sum / sysStat.avgNumSamples;
}

void collectSample(SystemStatus& sysStat) {
  static unsigned long lastSampleCollection = 0;
  unsigned long currentMillis = millis();
  
  if (sysStat.startAverage) { // Só começa a coleta de amostras se startAverage for verdadeiro
    if (currentMillis - lastSampleCollection >= 60000) {
      lastSampleCollection = currentMillis;
      
      int temp = sysStat.calibratedTemp;
      addSample(temp, sysStat);
      calculateAverage(sysStat);
    }
  }
}





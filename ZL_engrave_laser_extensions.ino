// ZiKE Labs engravement laser extensions
// Version: 0.6v 2019-12-13
// By Rafał Mlicki

// Liberies:
#include <NeoPixelBus.h>

// Constants:
const int NUMBER_OF_LEDS = 15;
const int LED_PIN = 8;
const int TIME_RESOLUTION = 30;

// Variables:
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> ledStrip(NUMBER_OF_LEDS, LED_PIN);
int mode = 0;
int yellowLedDuration = 0;
int tmpTime1 = 0;
int tmpTime2 = 0;

// Main program setup
void setup() {
  
    pinMode(6, INPUT_PULLUP);
    
    yellowLedDuration=yellowLedDurationRead();
    ledStrip.Begin();
    ledStrip.Show();
    ledStripInitialization();
    
}

// Main program loop
void loop() {
    
    if (yellowLedDurationRead()<(yellowLedDuration-TIME_RESOLUTION) || yellowLedDurationRead()>(yellowLedDuration+TIME_RESOLUTION))
    {
      mode=3;
      tmpTime2=stime();
    }
  
    switch (mode) {
      case 0:
      ledStripGreen();
      if (digitalRead(6)==LOW) {
        mode=1;
      }
      break;
      
      case 1:
      ledStripRed();
      if (digitalRead(6)==HIGH) {
        mode=2;
        tmpTime1 = stime();
      }
      break;
      
      case 2:
      ledStripYellow();
      if (digitalRead(6)==LOW) {
        mode=1;
      }
      if ((tmpTime1+yellowLedDuration)<stime()) {
        mode=0;
      }
      break;
      
      case 3:
      yellowLedDuration=(yellowLedDurationRead()/TIME_RESOLUTION)*TIME_RESOLUTION;
      ledStripBlackOut();
      for (int i=0;i*30<yellowLedDuration;i++) {
        ledStrip.SetPixelColor(i, RgbColor(0, 0, ledBrightnessRead()));
      }
      ledStrip.Show();
      if (tmpTime2+15<=stime()) {
        mode=0;
      }
      
      break;
    }
}

// Led strip off
void ledStripBlackOut() {
  for (uint8_t i=0;i<NUMBER_OF_LEDS;i++) {
    ledStrip.SetPixelColor(i, RgbColor(0, 0, 0));
  }
  ledStrip.Show();
}

// Led strip green
void ledStripGreen() {
  for (uint8_t i=0;i<NUMBER_OF_LEDS;i++) {
    ledStrip.SetPixelColor(i, RgbColor(0, ledBrightnessRead(), 0));
  }
  ledStrip.Show();
}

// Led strip red
void ledStripRed() {
  for (uint8_t i=0;i<NUMBER_OF_LEDS;i++) {
    ledStrip.SetPixelColor(i, RgbColor(ledBrightnessRead(), 0, 0));
  }
  ledStrip.Show();
}

// Led strip yellow
void ledStripYellow() {
  for (uint8_t i=0;i<NUMBER_OF_LEDS;i++) {
    ledStrip.SetPixelColor(i, RgbColor(ledBrightnessRead(), ledBrightnessRead(), 0));
  }
  ledStrip.Show();
}

// Led strip initialize animation
void ledStripInitialization() {
  for (uint8_t i=0;i<NUMBER_OF_LEDS;i++) {
    ledStrip.SetPixelColor(i, RgbColor(0, ledBrightnessRead(), ledBrightnessRead()));
    ledStrip.Show();
    delay(100);
  }
  for (uint8_t i=0;i<NUMBER_OF_LEDS;i++) {
    ledStrip.SetPixelColor(i, RgbColor(0, 0, 0));
    ledStrip.Show();
    delay(100);
  }
}

// Return brightness potentiometer value
int ledBrightnessRead() {
  int sensor=analogRead(A0);
  return map(sensor, 0, 1023, 254, 0);
}

// Return yellowLedDuration potentiometer value
int yellowLedDurationRead() {
  int sensor=analogRead(A1);
  return map(sensor, 0, 1023, 450, 30);
}

// Return time in seconds
int stime() {
  return millis()/1000;
}
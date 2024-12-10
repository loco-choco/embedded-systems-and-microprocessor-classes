#include "ESP32Servo.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define ADC_PIN 26
#define PWM_PIN 5
#define MANUAL_PIN 17
#define AUTO_PIN 16
#define OFF_PIN 4
int auto_mode;
int on;
Servo pwm;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup(){
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println("Nao achou display");
  }
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);

  on = 0;
  auto_mode = 0;
  pwm.attach(PWM_PIN, 500, 2500);
  pinMode(MANUAL_PIN, INPUT);
  pinMode(AUTO_PIN, INPUT);
  pinMode(OFF_PIN, INPUT);
  Serial.println("Ligado!");
}
int direction = 1;
int angle = 0;
int delay_after_loop = 0;
void automatic(){
  if(angle < 180){
    if(direction > 0)
      angle++;
    else if(angle > 0)
      angle--;
    else{
      angle++;
      direction = 1;
    }
  }
  else if(angle == 180){
    if(delay_after_loop == 10){
      direction = -1;
      angle--;
      delay_after_loop = 0;
    }
    else
      delay_after_loop++;
  }
  if(direction > 0)
    delay(100);
  else
    delay(200);
}
void manual(){
  int val = analogRead(ADC_PIN);
  angle = map(val, 0, 4096, 0, 180);
  delay(100);
}
int manual_pin_old = 0;
int auto_pin_old = 0;
int off_pin_old = 0;
void loop(){
  // Pegando entrada do usuario
  int manual_pin = digitalRead(MANUAL_PIN);
  if(manual_pin == 1 && manual_pin_old == 0){
    auto_mode = 0;
    on = 1;
    manual_pin_old = 1;
    Serial.println("Modo Manual!");
  }
  else if (manual_pin == 0 && manual_pin_old == 1)
    manual_pin_old = 0;
  int auto_pin = digitalRead(AUTO_PIN);
  if(auto_pin == 1 && auto_pin_old == 0){
    auto_mode = 1;
    on = 1;
    auto_pin_old = 1;
    Serial.println("Modo Auto!");
  }
  else if (auto_pin == 0 && auto_pin_old == 1)
    auto_pin_old = 0;
  int off_pin = digitalRead(OFF_PIN);
  if(off_pin == 1 && off_pin_old == 0){
    on = 0;
    off_pin_old = 1;
    Serial.println("Desligado!");
  }
  else if (off_pin == 0 && off_pin_old == 1)
    off_pin_old = 0;

  // Executando modo
  if(on){
    if(auto_mode)
      automatic();
    else
      manual();
    pwm.write(angle);
  }
  else
    delay(1);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Auto(1) Manu(0): ");
  display.print("Modo: ");
  display.println(auto_mode);
  display.print("Ang: ");
  display.println(angle);
  display.print("Dir: ");
  display.println(direction);
  display.display();
}


#include <Wire.h>

// Definicao dos pinos de leds e endereco do I2C
#define ANA_PIN A0
#define I2C_ADDR 0x8

int value;

// Recepcao de comando de I2C
void request_event(){
  Wire.write(highByte(value)); 
  Wire.write(lowByte(value)); 
}

void setup() {
  // Configuracao de recepcao do I2C
  Serial.begin(9600);
  Wire.begin(I2C_ADDR);
  Wire.onRequest(request_event);
  //Configuracao dos pinos de LED
  pinMode(ANA_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  value = analogRead(ANA_PIN);
  Serial.println(value);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
}

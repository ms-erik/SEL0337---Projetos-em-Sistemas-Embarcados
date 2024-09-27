#include <Wire.h>
const int ledPin = LED_BUILTIN;

void requestEvent() {
  int sensorValue = analogRead(A0); // Leitura do potenciômetro
  Wire.write(highByte(sensorValue)); // Envia byte alto
  Wire.write(lowByte(sensorValue));  // Envia byte baixo
}

void setup() {
  Wire.begin(0x08); // Inicia I2C com endereço 0x08
  Wire.onRequest(requestEvent); // Chama requestEvent ao receber pedido
  Serial.begin(9600); // Comunicação serial para monitoramento
}

void loop() {
  delay(100);
}

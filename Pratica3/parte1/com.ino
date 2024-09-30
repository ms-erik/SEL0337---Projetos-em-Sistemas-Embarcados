#include <Wire.h>
const int ledPin = LED_BUILTIN;

void setup_receive() {
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); 
}

void receiveEvent(int howMany) {
  while (Wire.available()) {
    char c = Wire.read(); 
    digitalWrite(ledPin, c);
  }
}

void loop() {
  delay(100);
}

void requestEvent() {
  int sensorValue = analogRead(A0); // Leitura do potenciômetro
  Wire.write(highByte(sensorValue)); // Envia byte alto
  Wire.write(lowByte(sensorValue));  // Envia byte baixo
}

void setup_request() {
  Wire.begin(0x08); // Inicia I2C com endereço 0x08
  Wire.onRequest(requestEvent); // Chama requestEvent ao receber pedido
  Serial.begin(9600); // Comunicação serial para monitoramento
}

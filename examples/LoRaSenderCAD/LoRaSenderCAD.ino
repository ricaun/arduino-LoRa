/*
  LoRa Sender CAD Exemple

  created 14 October 2018
  by Luiz H. Cassettari
*/

#include <SPI.h>
#include <LoRa.h>

const long frequency = 915E6;  // LoRa Frequency

const int csPin = 10;          // LoRa radio chip select
const int resetPin = 9;        // LoRa radio reset
const int irqPin = 2;          // change for your board; must be a hardware interrupt pin

static int sf = 7;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(frequency)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();
  LoRa.setSpreadingFactor(sf);
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();
}

void loop() {
  if (runEvery(1000)) { // repeat every 1000 millis

    String message = "HeLoRa World! ";
    message += "I'm a Node! ";
    message += millis();

    LoRa_sendMessage(message); // send a message

    Serial.println("Send Message!");
  }
}

void LoRa_rxMode() {
  LoRa.receive();
}

void LoRa_txMode() {
  LoRa.idle();
  LoRa.setSpreadingFactor(sf);
  if (++sf > 12) sf = 7;
}

void LoRa_sendMessage(String message) {
  LoRa_txMode();
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket(true);
}

void onReceive(int packetSize) {
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }

  Serial.print("SF: ");
  Serial.println(LoRa.getSpreadingFactor());
  Serial.print("Node Receive: ");
  Serial.println(message);
}

void onTxDone() {
  Serial.println("TxDone");
  LoRa_rxMode();
}

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
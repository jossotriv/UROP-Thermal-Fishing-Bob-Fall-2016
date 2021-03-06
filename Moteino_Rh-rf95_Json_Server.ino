// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX

#include <SPI.h>
#include <RH_RF95.h>

#define NODEID 1
#define GATEWAYID 1

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 868.0

// Singleton instance of the radio driver
RH_RF95 rf95;

// Blinky on receipt
#define LED 9

typedef struct {
  int           nodeId; //store this nodeId
  int           gatewayId; 
  unsigned long packetCount; 
  float         temp;
  float         humid;
  float         lux;
  float         batt;
} Payload;
Payload theData;

void setup() 
{
  pinMode(LED, OUTPUT);     
//  pinMode(RFM95_RST, OUTPUT);
//  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(115200);
  delay(100);

  //Serial.println("Feather LoRa RX Test!");
  
  // manual reset
  //digitalWrite(RFM95_RST, LOW);
  //delay(10);
  //digitalWrite(RFM95_RST, HIGH);
  //delay(10);

  while (!rf95.init()) {
    //Serial.println("LoRa radio init failed");
    while (1);
  }
  //Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    //Serial.println("setFrequency failed");
    while (1);
  }
  //Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  //uncomment this later////
  /////rf95.setTxPower(23, false);
}

void loop()
{
  
  if (rf95.available())
  {
    // Should be a message for us now   
  //  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  //  uint8_t len = sizeof(buf);

   uint8_t buf[sizeof(Payload)];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len))
    {
      
      //RH_RF95::printBuffer("Received: ", buf, len);   

      Serial.print("RSSI: ");
      Serial.print(rf95.lastRssi(), DEC);
      
      Serial.print('\t'); // TSV
      
      Serial.println((char*)buf);
      
     theData = *(Payload*)buf;
     
      digitalWrite(LED,HIGH);
    delay(200);
    digitalWrite(LED, LOW);


    int           nodeId; //store this nodeId
    int gatewayId; 
  unsigned long packetCount; 
  float         batt;   
  float         temp;
  float         humid;
  float         lux;

  gatewayId = theData.gatewayId;
  nodeId = theData.nodeId;
  packetCount = theData.packetCount;
  temp = theData.temp;
  humid = theData.humid;
  lux = theData.lux;
  batt = theData.batt;

  Serial.print("lux=");
  Serial.println(lux);

  
      /*
      // Send a reply
      uint8_t data[] = "And hello back to you";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      digitalWrite(LED, LOW);
      */
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
}

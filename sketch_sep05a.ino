#include <SPI.h>    
#include <nRF24L01.h>
#include <RF24.h>
#include<SoftwareSerial.h>

 
SoftwareSerial bt(0,1);
RF24 radio(7,8);

const byte address_trans1[6] = "00001";
const byte address_recieve1[6] = "00002";
const byte address_trans2[6] = "10001";
const byte address_recieve2[6] = "10002";

const int button1 = 2;
const int lost_led1 = 3;
const int button2 = 4;
const int lost_led2 = 5;
void setup() {
  radio.begin();
  pinMode(lost_led1,OUTPUT);
  pinMode(button1,INPUT);
    pinMode(lost_led2,OUTPUT);
    pinMode(button2,INPUT);
    Serial.begin(9600);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  bt.begin(9600);
}

void loop() {
  //Serial.println("stopped LIs");
  //delay(2000);
  const char text[] = "nrftest";
  char ifAvail[32] = ""; 
  String input;
    
    if (bt.available()) 
{      input=bt.readString();
      Serial.println("keys");
      delay(200);
} 
  while(digitalRead(button1) == 1 || input == "keys")
  { //delay(2000);
    radio.openWritingPipe(address_trans1);
    radio.openReadingPipe(0,address_recieve1);
    delay(5);
    radio.stopListening();
    Serial.println("Sending(Keys)");
    radio.write(&text, sizeof(text));
    input="Null";
    delay(500);
  }
  while(digitalRead(button2) == 1 || input == "phone")
  { //delay(2000);
    radio.openWritingPipe(address_trans2);
    radio.openReadingPipe(0,address_recieve2);
    delay(5);
    radio.stopListening();
    Serial.println("Sending(Phone)");
    radio.write(&text, sizeof(text));
    input="Null";
    delay(500);
  }

  radio.openWritingPipe(address_trans1);
  radio.openReadingPipe(0,address_recieve1);
  delay(5);
  radio.startListening();
  if (radio.available()) {
    digitalWrite(lost_led1,LOW);
    radio.read(&ifAvail, sizeof(ifAvail));
    String transData = String(ifAvail);
    if (transData == "in Range") {
        Serial.println("Keys are in range");
        delay(1000);
    }    
  }
  else if (!radio.available()){
    digitalWrite(lost_led1,HIGH);
    Serial.println("Keys are not in range");
    delay(3000);
  }  
  
  radio.openWritingPipe(address_trans2);
  radio.openReadingPipe(0,address_recieve2);
  delay(5);
  radio.startListening();
  if (radio.available()) {
    digitalWrite(lost_led2,LOW);
    radio.read(&ifAvail, sizeof(ifAvail));
    String transData = String(ifAvail);
    if (transData == "in Range") {
        Serial.println("Phone is in range");
        delay(1000);
    }    
  }
  else if (!radio.available()){
    digitalWrite(lost_led2,HIGH);
    Serial.println("Phone is not in range");
    delay(3000);
  }
}

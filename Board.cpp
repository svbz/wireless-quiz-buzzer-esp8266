#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

#include "Main.h"

#define PORT_X_0                  0   // [detects boot mode. no internal pullup available. Huzzah: connected to tactile switch to GND and 1K plus red led to VCC]
#define PORT_X_2                  2   // [detects boot mode. tie high permanently. Huzzah: 10K external pullup, connected to blue led]
#define PORT_ANCHOR_4             4   // big LED + (for component anchor only)
#define PORT_LED_5                5   // big LED -
#define PORT_X_12                 12
#define PORT_BEEP_13              13  // beeper
#define PORT_PB_14                14  // player:buzzin controller:reset
#define PORT_X_15                 15  // [detects boot mode. tie low permanently. Huzzah: 10K external pulldown]
#define PORT_ESP_RESET_16         16  // [self initiated reset and used to awake from deep sleep if 1N914 tied to RST]

volatile boolean buttonPressed = false;
int lowGlowCount = 0;

void blueLedState( byte );

void boardSetup() {
  pinMode(A0,INPUT);
  pinMode(PORT_X_0,INPUT);
  pinMode(PORT_X_2,INPUT);
  pinMode(PORT_ANCHOR_4,INPUT);
  pinMode(PORT_LED_5,OUTPUT);
  pinMode(PORT_X_12,INPUT_PULLUP);
  pinMode(PORT_BEEP_13,OUTPUT);
  pinMode(PORT_PB_14,INPUT_PULLUP);
  pinMode(PORT_X_15,INPUT);
  pinMode(PORT_ESP_RESET_16,OUTPUT);

  digitalWrite(PORT_BEEP_13,LOW);      
  digitalWrite(PORT_ESP_RESET_16,HIGH);   
  
  blueLedOff();
}

void boardBounce() {
  digitalWrite(PORT_ESP_RESET_16,LOW); 
  delay(100000);
}

void blueLedFlash() {
  blueLedOn();
  delay(30);
  blueLedOff();
}

void blueLedBlink( byte times ) {
  byte saveState = digitalRead(PORT_LED_5);
  blueLedOff();
  delay(600);
  for( byte ix=0; ix < times; ix++ ) {
    blueLedOn();
    delay(30);
    blueLedOff();
    delay(300);
  }
  delay(600);
  blueLedState(saveState);
}

void blueLedOn() { 
  digitalWrite(PORT_LED_5,LOW);
}

void blueLedOff() { 
  digitalWrite(PORT_LED_5,HIGH);
}

void blueLedState( byte state ) { 
  digitalWrite(PORT_LED_5,state); 
}

void buttonPressedIsr() { buttonPressed = true; }
void buttonInterruptOn() { buttonPressed = false; attachInterrupt(PORT_PB_14, buttonPressedIsr, FALLING); }
void buttonInterruptOff() { detachInterrupt(PORT_PB_14); buttonPressed = false; }
boolean buttonGetAndReset() { boolean ret = buttonPressed; buttonPressed = false; return ret; }

void beepBuzzedIn() {
  for( byte ix = 0; ix < 3; ix++ ) {
    beepOnce();
    delay(100);
  }
}

void blueLedLowGlow() {
  lowGlowCount++;
  if( lowGlowCount == 10 ) {
    blueLedOn();
    blueLedOff();
    lowGlowCount = 0;
  }
}

void beep( int millis ) {
  digitalWrite(PORT_BEEP_13,HIGH);      
  delay(millis);
  digitalWrite(PORT_BEEP_13,LOW);      
}
void beepOnce() { beep( 50 ); }
void beepTimeUp() { beep( 450 ); }

/* for non self drive piezo
void beepOnce() {
  for( int ix = 0; ix < 130; ix++ ) {
    digitalWrite(PORT_BEEP_13,HIGH);      
    delayMicroseconds(200); 
    digitalWrite(PORT_BEEP_13,LOW);      
    delayMicroseconds(200); 
  }  
}
void beepTimeUp() {
  for( int ix = 0; ix < 1000; ix++ ) {
    digitalWrite(PORT_BEEP_13,HIGH);      
    delayMicroseconds(200); 
    digitalWrite(PORT_BEEP_13,LOW);      
    delayMicroseconds(200); 
  }    
}
*/

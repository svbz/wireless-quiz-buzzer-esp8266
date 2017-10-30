// remember to Sketch | Include Library | ESP8266WiFi
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include "Main.h"

#define CONTROLLER
//#define PLAYER

#ifdef PLAYER
void checkBattery();
boolean waitForBytes();

WiFiUDP playerUdp;

void setup() {
  boardSetup();
  WiFi.persistent( false );
  WiFi.mode( WIFI_STA );
  blueLedOn();
  delay( 1000 );
  checkBattery();
  blueLedOff();
  randomSeed( RANDOM_REG32 );
  playerUdp.begin( UDP_PORT );
}

void loop() {
  if( WiFi.status() != WL_CONNECTED ) {
    buttonInterruptOff();
    checkBattery();
    if( !wiFiConnect( true ) )
      return;  
    buttonInterruptOn();
  }  

  blueLedLowGlow();
  
  if( buttonGetAndReset() ) {  
    buttonInterruptOff();  
    
    playerUdp.flush();
    playerUdp.beginPacket( CONTROLLER_IP, UDP_PORT );
    playerUdp.write("?");
    playerUdp.endPacket();

    if( waitForBytes() ) {
      blueLedOn();
      while( waitForBytes() )
        ;
      blueLedOff();
    }

    delay( 1000 );
    checkBattery();
    buttonInterruptOn();
  }
}

// returns true on REPLY_YES received
// returns false if no reply is received within about 3 seconds
// the controller is sending the winning player a yes reply every second
boolean waitForBytes() {
  unsigned long startMillis = millis();
  while( true ) {
    if( playerUdp.parsePacket() ) {
      byte packet = 0;
      playerUdp.read(&packet,1);
      playerUdp.flush();
      return packet == REPLY_YES;      
    }
    if( WiFi.status() != WL_CONNECTED )
      return false;
    if( (int)(millis() - startMillis) > 3000 )
      return false;
    yield();    
  }
}

void checkBattery() {
  int batteryLevel = analogRead(A0); 
  if( batteryLevel < 705 ) { // 3.1v x 10K/(10K+33K) * 1024 = 738 but ends up being 705
    while( true ) {
      blueLedFlash();
      delay( 500 );
    }
  }  
}
#endif

#ifdef CONTROLLER
IPAddress localPlaceHolder( 127, 0, 0, 1 );

void setup() {
  boardSetup();
  WiFi.persistent( false );
  WiFi.mode( WIFI_AP );
  blueLedOn();
  delay( 1000 );
  blueLedOff();
  userServerSetup();
  buttonInterruptOn();
}

void loop() {
  blueLedLowGlow();
  runUserServer();
  if( buttonGetAndReset() ) {
    beepTimeUp();     
    clearClients( localPlaceHolder );
    buttonGetAndReset();
  }
}
#endif



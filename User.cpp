#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include "Main.h"

void toPlayer( IPAddress, unsigned char );
WiFiUDP controlUdp;

void userServerSetup() {
  WiFi.softAP( zidGetControllerSSID(), zidGetControllerPass() );
  controlUdp.begin( UDP_PORT );
}

void runUserServer() {
  yield();
  int packetSize = controlUdp.parsePacket();
  if( !packetSize )
    return;    
  controlUdp.flush();    

  IPAddress winner = controlUdp.remoteIP();
  toPlayer( winner, REPLY_YES );  
  blueLedOn();
  beepBuzzedIn();
  
  int delayMs = 100 + 100 * analogRead(A0); // 786 max read on ADC with 3.3v * (10K/(10K+33K) * 1024. Map to 60 seconds: 60000ms / 786 = 76 multiplier but ends up being 100
  if( delayMs > 65000 )
    delayMs = 0x7fffffff;
  int delayedMs = 0;
  unsigned long oneSecondMillis = millis();
  unsigned long tenSecondMillis = millis();
  while( !buttonGetAndReset() && delayedMs < delayMs ) {
    unsigned long now = millis();
    if( (int)(now - oneSecondMillis) > 1000 ) {
      toPlayer( winner, REPLY_YES );
      oneSecondMillis = now;
    }    
    if( (int)(now - tenSecondMillis) > 10000 ) {
      tenSecondMillis = now;    
      if( delayMs != 0x7fffffff ) {
        beepOnce();
        delayedMs += (int)(millis() - tenSecondMillis);
      }
    }    
    delayedMs += clearClients( winner );
    delay( 1 );
    delayedMs++;    
  }

  toPlayer( winner, REPLY_NO );
  blueLedOff();    
  beepTimeUp();  // also provides delay for reply_no before stopping
  clearClients( winner );
  buttonGetAndReset();
}

// return the number of millis taken
int clearClients( IPAddress & winner ) {
  int millisTaken = 0;
  while( true ) {
    yield();
    int packetSize = controlUdp.parsePacket();
    if( !packetSize )
      return millisTaken;
    controlUdp.flush();
    if( controlUdp.remoteIP() != winner ) {
      toPlayer( controlUdp.remoteIP(), REPLY_NO );
      delay( 1 );
      millisTaken++;
    }
  }
  return millisTaken;
}

void toPlayer( IPAddress remoteIp, unsigned char what ) {
  controlUdp.beginPacket( remoteIp, UDP_PORT );
  controlUdp.write( what );
  controlUdp.endPacket();
}


#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include "Main.h"

// if required, keeps trying up to 20 times then bounces
// if not required, the connection must have already been made
// returns success:true fail:false
bool wiFiConnect( bool required ) {
  if( WiFi.status() == WL_CONNECTED )
    return true;
  if( WiFi.status() != WL_CONNECTED && !required )
    return false;
  int tries = 0;
  WiFi.disconnect();  
  WiFi.begin( zidGetControllerSSID(), zidGetControllerPass() );
  while( WiFi.status() != WL_CONNECTED ) {
    tries++;
    if( tries >= 10 )
      boardBounce();
    delay( random( 800, 1200 ) );
    blueLedBlink( BLINKS_WIFI_3 );
    yield();
  }    
  
  return true;
}



#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include "Main.h"

const char CONTROLLER_ID[] = "Q00001";
const char CONTROLLER_PASS[] = "00000000";

char * zidGetControllerSSID() { return (char *)CONTROLLER_ID; }
char * zidGetControllerPass() { return (char *)CONTROLLER_PASS; }



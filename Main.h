#ifndef MAIN_H
#define MAIN_H

//#define DEBUG

#define BLINKS_X_1                1  // X
#define BLINKS_X_2                2  // X
#define BLINKS_WIFI_3             3  // connecting to controller WiFi
#define BLINKS_X_4                4  // X
#define BLINKS_SERVICE_5          5  // connecting to controller server
#define BLINKS_DEBUG_6            6  // debug

#define REPLY_YES                 'y'
#define REPLY_NO                  'n'

#define UDP_PORT                  9999
#define CONTROLLER_IP             "192.168.4.1"

void boardSetup();
void boardBounce();
void blueLedFlash();
void blueLedBlink( byte );
void blueLedOn();
void blueLedOff();
void blueLedLowGlow();
void beepOnce();
void beepBuzzedIn();
void beepTimeUp();
void buttonPressedIsr();
void buttonInterruptOn();
void buttonInterruptOff();
boolean buttonGetAndReset();

bool wiFiConnect( bool );
bool serviceConnect( WiFiClient &, bool );

void userServerSetup();
void runUserServer();
int clearClients( IPAddress & );

char * zidGetControllerSSID();
char * zidGetControllerPass();

#endif

# wireless-quiz-buzzer-esp8266
Wireless Quiz Buzzer ESP8266

This is the Arduino IDE ino source code to create your own wireless, battery-powered quiz buzzer system. The ESP8266 limits simultaneous client connections to around 6; therefore this source will permit 6 buzzer players: one "controller" and six "player" units. A #define in the main file switches the compiled code between controller and player. Player battery low functionality is included using the ESP8266 ADC input. The controller creates its own wireless network to which the players connect. UDP communication is used. Multiple systems operating in the same room are no problem - simply create each system with a unique WiFi network name. Have fun.

# wireless-quiz-buzzer-esp8266
Wireless Quiz Buzzer ESP8266

This is the Arduino IDE ino source code to create your own wireless, battery-powered quiz buzzer system. The ESP8266 limits simultaneous client connections to around 6; therefore this source will permit 6 buzzer players: one "controller" and six "player" units. A #define in the main file switches the compiled code between controller and player. Player battery low functionality is included using the ESP8266 ADC input. The controller creates its own wireless network to which the players connect. UDP communication is used. Multiple systems operating in the same room are no problem - simply create each system with a unique WiFi network name.

Reset Modes

A 10K pot is connected to the ADC input on the controller, allowing the setting of automatic and moderator reset times as follows.
Automatic: In automatic reset mode, turn the pot for an automatic reset time from seconds to around a minute. When a competitor buzzes in, his light stays lit and everyone stays locked out for this adjustable duration then everything resets automatically for the next round. At any time the moderator can also press the red reset button to reset the round.
Moderator: For moderator reset mode, turn the black controller knob all the way to the right and then after a competitor buzzes in, all the other competitors stay locked out and the winner's light stays lit until the moderator presses the red reset button.

Operation

A competitor presses his red button to buzz in. If that comptetitor is the first to buzz in, his light turns on and all others are locked out from buzzing in. The controller also lights up and beeps when a competitor buzzes in and beeps again when the controller resets.

Player Low Battery

If the player battery is low, its light will flash constantly and it cannot be used again until the battery is changed. The battery level is checked when the player is turned on, when the player pairs with the controller, and following reset after a competitor pushes his button. If a player indicates a low battery right after its button is pressed, that round does not need to be replayed because that competitor was not first – the low battery check comes after the determination of who was first. A competitor with a low battery that buzzed in first will still win that round properly, and the low battery indicator will show right after the controller resets that round. 

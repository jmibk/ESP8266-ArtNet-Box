# ESP8266-ArtNet-Box
A multifunctional DMX ArtNet Box based on ESP8266

WHAT IT SHOULD DO

- sending DMX out of Art-Net packets
- supports up to 2 DMX universes
- connects to a network via wifi or cable network (W5100)
- can open a access point
- supports a oled display for control and analizing
- enter values via rotary encoder

CONNECTION TO A ESP8266

This code depends on a Lolin ESP8266 board using following connections:

OLED display:

Use a 'off the shelf' chinese OLED display with 4 pins: VCC, GND, SCK and SDA.

Connect as follows:
VCC:	3,3V
GND:	Ground
SCK:	D1 (GPIO5)
SDA:	D2 (GPIO4)

Rotary Encoder:

Usa a rotary encoder board for arduino (5 pins) or a single rotary encoder with 4 state changes per klick.
Connect it with the common pin to GND and connect the 2 signal pins to the ESP. Use 10k pullup resitors to 3,3V on these lines.
Connect the push button of the encoder to GND and to the ESP. You can use a 10k pullup if you like.

The arduino rotary encoder board has 5 pins: CLK, DT, SW, + and GND.

Connect as follows:
CLK:	D3 (GPIO0)
DT:		D0 (GPIO16)
SW:		RX (GPIO3)
+:		3,3V
GND:	Ground

If the direction of the rotary encoder is inverse, swap CLK and DT pins.

W5100 ethernet board

Use a W5100 board of your choice. In the current version the W5500 is NOT supported!

Connect as follows:

GND:	Ground
V+5:	5,0V
RST:	leave open
NSS:	D8 (GPIO15)
SOK:	D5 (GPIO14)
MO:		D7 (GPIO13)
MI:		D6 (GPIO12)
GND:	Ground 
POE+:	leave open
POE-:	leave open

DMX outputs

Connect your 75176s on following outputs:

Universe 0:		TX (GPIO1)
Universe 2:		D4 (GPIO2)


ISSUES

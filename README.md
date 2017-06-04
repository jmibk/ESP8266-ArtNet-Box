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


HOW TO CONTROL

Rotate the encoder to switch through the menus
Press the encoder to switch through the displayed items. There is a box on the choosen item. Rotate the encoder to change its value.
Press again to switch to the next item. Pressing it at the last item saves all data and exit the edit mode.
Double press the button exits the edit mode without saving any item on the page (even you edited one before)
Double press on the menu returns to the first page of the menu
Long Press on the main menu (page 1) for >3secs resets all settings. Restart the unit to load the default settings.

Display:
Top:
left:	wifi strength - not accurate yet
middle:	wifi state or ssid 
right:	wifi state in short form: OFF, ERR (Error), AP (AccessPoint) or CL (Client)

Bottom:
left:	IP address of the unit, subnetmask next to "/". 8 means 255.0.0.0, 24 means 255.255.255.0 (the number is the count of 1s in the address)
right:	time since starting the unit

Middle:
Here you can display individual information and menus:
left: you can see the menu number with a headline next to it. This helps navigating through the menus.

1 - Status: (no edit)
Here you can see a overview of all Art-Net activity on this node.

Net:			the current Art-Net net of this node
Sub:			the current Art-Net subnet of this node
Uni:			the current Art-Net univsere of this node. There are two of them, because the node has two outputs.
"x" or "tick":	are the ArtDmx packets? If yes: display "receiving", if no: display of the time elapsed since the last packet received (or the unit turned on)

2 - Last Art-Net received: (no edit)
Here you can see the last received ArtDmx packet, that was addressed to this node. The settings Net, Subnet and Uni of this node have no effect to this display. This functionality is for troubleshooting.

Net:			the Art-Net net of last received ArtDmx packet
Sub:			the Art-Net subnet of last received ArtDmx packet
Uni:			the Art-Net universe of last received ArtDmx packet
Time:			the time since last ArtDmx packet was received (or time since turn on)

3 - DMX Channel Viewer
Here you can monitor the received dmx channel data for both universes. You can choose between a multi channel view or a single channel view with history graph.

Press the encoder to select, rotate to change values.

Scr:			Source universe of dmx data (ArtNet A or ArtNet B)
Rng:			Page of the multi channel view. One page displays 20 channels, you can choose pages 0 (chans 1 to 20) to 25 (chans 501 to 512)
Chan:			Select the channel to be displayed. Channel 0 selects the multi channel view, a channel between 1 and 512 selects the single channel view

4 - Node IP settings 1

Press the encoder to select, rotate to change values.

DHCP:			DHCP enable or disable (disable means, you can enter an ip address on next pages, enable means receiving an ip by an external dhcp service - a router for example)

5 - Node IP settings 2

Press the encoder to select, rotate to change values.

IP:				The IP address of this node. Wifi and ethernet share same ip address. Do not connect both connections to the same network.
Mask:			The network mask of this node. Wifi and ethernet share same mask.

6 - Wifi Mode

Press the encoder to select, rotate to change values.

Wifi Radio:		Enables the Wifi functionality
Wifi Mode:		Mode of Wifi module. Client means to connect to a network, Access Point means that other devices can connect to this node without a external router or access point.

7 - Wifi Client

Press the encoder to select, rotate to change values. Entering the menu with a button press scans the network for wifi networks. 

Wifi Client SSID:		The wifi network ssid you want to connect to
Wifi Client Password:	Enter the password for your wifi network

8 - Art-Net Settings 1:

Press the encoder to select, rotate to change values.

Art-Net Net:	The Art-Net net address of this node
Art-Net Subnet:	The Art-Net subnet address of this node

9 - Art-Net Settings 2:

Press the encoder to select, rotate to change values.

Art-Net Universe A:		The Art-Net universe address for DMX output 0 of this node.
Art-Net Universe B:		The Art-Net universe address for DMX output 1 of this node.

10 - System Settings:

Press the encoder to select, rotate to change values.

LCD Contrast:	The contrast of the connected display. Note that wrong values can affect the display to be unreadable. In that case, reset all values. Power on the unit, wait for 10 secs and press the button for 5 secs. Then power cycle the unit. All settings will be lost!
Logo Screen Time: Time the boot logo is going to be displayed. 0 skips logo display. 


ISSUES

Display is showing a Logo and nothing happens.
If the W5100 is not connected to the unit, there will be a endless loop on start. 

If Wifi is turning on, no connection can be established. The unit change to AP mode.
Restart the unit. 

MAIN ISSUES

If the unit receives Art-Net, it restarts in an unperiodic timespan of minutes or hours. In that case you can see some stack things on the serial monitor from the esp. 9600 BAUD 8n1
This is only when receiving Art-Net. If there are no Art-Net packets, there is no restart. 
<try that if it is difference between wifi and ethernet>


# Tenways_Activator
If the OLED display of your Tenways CGO600 Bike is broken, this activator can get it going again

This code was developed for an Arduino Beetle (very small Arduino compatible board, based on Arduino Leonardo). 
https://www.dfrobot.com/product-1075.html

Normally the OLED display sends a fixed stream of commands (begins with 0x16) or requests for data (begins with 0x11) to the controller. 

The datastream has a total length of 1400 msec and then repeats.
4 commands are sent to the Controller :

PAS level : 0, 1, 2, 3 or Walking mode.
Wheel circumference  
PAS Level again 
Light On Of : 

Inbetween several requests for data from the controller are issued by the display :
Voltage
Battery level
Battery Voltage
Current
Wheel RPM
Status
Speed

Each data request or command is issued in a 100msec interval. After the 11 requests and commands are sent, a 300 msec wait time indicates the end of a cycle. 

All numbers in HEX value !!
disp->contr             response from controller (response depends on speed, current, voltage etc)
    0ms 11 20           -> 00 00 20 (3 Bytes) 11 20 wheel rpm. 00 00 is the data, 20 = CRC 
  100ms 16 0B 03 24                 (0 bytes) 03 24 for PAS3, 02 23 for PAS2, 0C 2D for PAS1, 00 21 for PAS0, 06 27 for WALK MODE
  200ms 11 08           -> 01       (1 byte)  11 08 Status request => 01 response is no error. Other numbers indicate certain errors
  300ms 11 31           -> 30 30    (2 bytes) 11 31 Voltage request => 30 Hex is 48 ??? does not compute maybe 48% ??
  400ms 11 0A           -> 00 00    (2 bytes) 11 0A Current consumption request => response is in A (divide by 2 ??)
  500ms 11 11           -> 34 34    (2 bytes) 11 11 = GET Battery => 34 Hex = 52% (both bytes in the response contain the same data)
  600ms 16 1F 00 BD F2              (0 bytes) 00 BD = wheel circumference BD Hex = 189 cm ??  F2 = CRC (16+1F+00+BD=F2)
  700ms 11 20           -> 00 00 20 (3 bytes) 11 20 = Speed request
  800ms 16 0B 03 24                 (2 bytes) 03 24 for PAS3, 02 23 for PAS2, 0C 2D for PAS1, 00 21 for PAS0, 06 27 for WALK MODE
  900ms 16 1A F1                    (0 bytes) (F1 for Light ON, F0 for Light OFF)
 1000ms 11 22 33        -> FF FF FE (3 bytes) ??? Closure byte ???
 1100ms 11 60 71        -> C8 00 C8 00 C8 00 98 3A 00 00 00 00 7B 06 4E 0C 34 34 06 00 98 01 00 18 00 00 24 (27 bytes!)
 1400ms sequence restarts
 this sequence does NOT change apart from the PAS level and the light. No dynamic alteration above 25 kph. That is done in the controller. So the configuration for top speed must be in the message or hardcoded in the controller

             /***********************************************************************

  Nextion Based DCC++EX Controller No WiFi

  COPYRIGHT (c) 2022 Norman Halland (NormHal@gmail.com)

   This program and all its associated modules is free software:
   you can redistribute it and/or modify it under the terms of the
   GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option)
   any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see http://www.gnu.org/licenses

************************************************************************/
/*
 * Designed and written by Norman Halland  2023
 * 
 ***********************************************************************

Getting Started

This is a brief summary of the README.md file on Github - https://github.com/normhal/DCCppEX-Nextion-Controller

Pin Assignments for UNO (and variants)

    Rotary Encoder
        Reverse Button - Arduino pin D8
        Clock Pulses - Arduino pin D3 for Interrupt option, D6 for Polled
        Data Pulses - Arduino pin D2 for Interrupt option, D7 for Polled
        - Swop Clock and Data to change rotation direction
        Refer to "Config.h" Line 32 to select option

    Nextion Display
        Software Serial RX - Arduino pin D4 to Nextion TX wire (Blue)
        Software Serial TX - Arduino pin D5 to Nextion RX wire (Yellow)

    Connection to Command Station
        Hardware Serial RX - Arduino pin D0 to DCC++, DCC-EX, or HC-12 TX pin
        Hardware Serial TX - Arduino pin D1 to DCC++, DCC-EX, or HC-12 RX pin

Note Regarding Rotary Encoders

There are a few different versions of Rotary Encoders. Refer to the appropriate Datasheet for specific pin assignments

    Reverse button = "SW"
    Data pulses = "DT"
    Clock pulses = "CLK"

Note Regarding UNO variants

Arduino boards using the ATMega32U4 processor and most mini boards using the ATMega328P chip can be 
programmed to use the "UNO" bootloader which is smaller and yields the same usable memory as a Standard UNO. 
In view of the size of the Arduino Sketch, this change is required to provide sufficient progam memory. 
"Erik" (erik84750) kindly provided the Addendum below for instructions how to replace the UNO bootloader.

**************************************************************************************************************************
*/
#include "DCCEX_Controller.h"
/*
**************************************************************************************************************************
* Setup Code
**************************************************************************************************************************
*/
void setup()
{
  Serial.begin(115200);     //For the direct connection to DCC++ or DCC-EX
  nextion.begin(57600);     //This speed has been found to be suitable for both Hard- and Software Nextion Connections.
  wait(250);
  initPage(CoverPage);
  wait(250);
  nextionSetText("Version", Version);
  wait(4000);

  // Setup pins and Interrupts for Rotary Encoder
  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(REButtonPin, INPUT_PULLUP);
  attachInterrupt(0, PinA, CHANGE); //interrupt on PinA, rising edge signal & executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, PinB, CHANGE); // same for B

  if (EEPROM.read(eepromEnd) != EEPROMCODE) initEEPROM();       //Test to see if EEPROM has been initialized before
  uint8_t r = 0;
  for (uint16_t s = eeSelIDs; s < (eeSelIDs + numLocoSlots); s++)
  {
    selectedIDs[r] = readEEPROMByte(s);
    r++;
  }

#if defined SEND_POWER_STATE
  if (readEEPROMByte(eePUState) == 1) powerONButton();    // Set both the Nextion and Command Station Power State
  else powerOFFButton();
#endif

  activeSlot = readEEPROMByte(eeActiveSlot);
  initPage(MenuPage);                       //Display the Menu Page first after the Cover Page/
  nextionCommand("bkcmd=0");    //Suppress Error details from the Nextion
  //Ensure Edit Mode is OFF
  eMode = 0;

#if defined NEW_LEFT_CHAR
  nextionSetText("keybdA.b241",NEW_LEFT_CHAR);
  wait(10);
#endif
#if defined NEW_RIGHT_CHAR
  nextionSetText("keybdA.b243",NEW_RIGHT_CHAR);
  wait(10);
#endif
}   //Setup
/*
 **************************************************************************************************************************
 * Main Program Loop
 **************************************************************************************************************************
*/
void loop()
{
  rotaryEncoder();                      // check for rotary encoder activity
  checkREButton();                      // check for change in direction from Rotary Encoder
//  updateThrottle();                     //
  receiveCMD();                         // Receive and Process anything from the Command Station
  refreshDCC();                         // Refresh current Loco if enabled
  buttonScheduler();                    // Process any received Nextion data
}

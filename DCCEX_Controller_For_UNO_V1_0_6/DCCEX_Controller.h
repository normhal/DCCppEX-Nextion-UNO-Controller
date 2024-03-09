/***********************************************************************
*                  
* COPYRIGHT (c) 2022 Norman Halland (NormHal@gmail.com)
*
*  This program and all its associated modules is free software: 
*  you can redistribute it and/or modify it under the terms of the 
*  GNU General Public License as published by the Free Software 
*  Foundation, either version 3 of the License, or (at your option) 
*  any later version.
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see http://www.gnu.org/licenses
*
************************************************************************/
/* Copyright Norman Halland 2022
 * Header File for DCCEX_Controller_No_WiFi
*/
#ifndef DCCEX_CONTROLLER_H
  #define DCCEX_CONTROLLER_H

  #define Version "V1.0.6"
  const uint8_t EEPROMCODE = 106;
  
  #include <EEPROM.h>

  //**************************************************************************************
  
  #if defined (ARDUINO_AVR_LEONARDO)
    #define PROCESSOR_OK
  #elif defined (ARDUINO_AVR_UNO)
    #define PROCESSOR_OK
  #elif defined (ARDUINO_AVR_PRO)
    #define PROCESSOR_OK
  #elif defined (ARDUINO_AVR_NANO)
    #define PROCESSOR_OK
  #elif defined (ARDUINO_AVR_ATmega328)
     #define PROCESSOR_OK
  #else !defined PROCESSOR_OK
    #error "This Program is Designed for UNO Only!"
  #endif

  #include "Config.h"

  //**************************************************************************************

    #include <SoftwareSerial.h>
    SoftwareSerial nextion(4, 5); // Nextion TX to pin 4 and RX to pin 5 of Arduino UNO
    #define REButtonPin 8      // the Arduino Pin number for the encoder reverse button
 
    #define pinA 3                  // the first hardware interrupt pin is digital pin 2
    #define pinB 2                  // the second hardware interrupt pin is digital pin 3
    
    #define accNameLen 9

    #define eepromSize 1023             //Integrated Arduino UNO EEPROM
    #define eepromEnd eepromSize
    #define numLocos 10                 //Multiples of 10
    #define numAccs 36                  //Multiples of 12
    #define numRoutes 6                 //Multiples of 6
    #define numLocoSlots 10             //Always 10 for this version
    const uint8_t numFSlots = 10;       //Function Slots per loco  - always 10 for this version

    const uint8_t fBlockSize = (numFSlots * 2);       //Slots x 2 bytes each - Function and Image number
/*
*******************************************************************************************************************************
* Defines and Variables for Loco Details
*******************************************************************************************************************************
*/
  struct locomotive {
	uint8_t dir = 1;
	uint8_t speed;
};
typedef struct locomotive loco;

  loco locos[numLocos];
  
  //THE 6 OPTIONS BELOW SHOULD BE CHANGED VIA THE CONTROLLER CONFIG PAGE ONLY
  //CHANGING THEM HERE AFTER THE FIRST BOOT WILL HAVE NO EFFECT AS
  //THEY ARE USED ONLY ONCE TO INITIALIZE EEPROM
  
  const uint8_t ReverseThreshold = 30;       //Speed after which loco will stop when direction is changed
  const uint8_t REAccAmount = 5;             //Change this to your preferred acceleration rate of the Rotary Encoder
  const bool DCCRefresh = false;
  uint8_t PowerUpState  = true;                  //Change to "false" to Start up withPower OFF
  const uint8_t AccDelay = 10;                //Number of 10ms intervals between Route Accessory activations
  const uint8_t FunctionPulse = 5;
  uint8_t LocoStopAll = true;

  //uint8_t REpinA;
  //uint8_t lastREpinA;
  unsigned long lastButtonPress = 0;
/*
 **************************************************************************************************************************************
 * Function Prototypes for all functions
 **************************************************************************************************************************************
 */

void buttonScheduler();
void rotaryEncoder();
void refreshDCC();
void stopButton();
void powerONButton();
void powerOFFButton();
void menuPage(uint8_t);
void creditsPage(uint8_t);
void accImagesPage(uint8_t);
void displayCredits();
void setLocoTabs(uint8_t);
void populateSlots();

void setPageButtons(uint8_t, uint8_t, uint8_t);
void acc_Page(uint8_t);
void accDrawPage(uint8_t);

void accEditPage(uint8_t);

void sendCMD(String);
void receiveCMD();
void wait(uint16_t);
void parse(char *com);
void doDCC(uint8_t);
void doDCCAcc(uint8_t);
void doDCCfunction04();
void doDCCfunction58();
void doDCCfunction912();
void doDCCfunction1320();
void doDCCfunction2128();
uint8_t toggleFunction(uint8_t, uint8_t);

void configPage(uint8_t);
void locoEditPage(uint8_t);
void functionEditPage(uint8_t);

void initEEPROM();
uint16_t readLocoAddress(uint8_t);
uint16_t readAccAddress(uint8_t);
uint8_t readAccImage(uint8_t);
String readEEPROMName(uint16_t);  
String readLocoName(uint8_t);
uint16_t readEEPROMAddr(uint16_t);
uint8_t readEEPROMByte(uint16_t);
void readAccName(uint8_t, uint8_t);
void writeLocoName(uint8_t, const String &Name);
void writeAccAddress(uint8_t, uint16_t);
void writeLocoAddress(uint8_t, uint16_t);
void writeEEPROMByte(uint16_t, uint8_t);
void writeEEPROMName(uint16_t, const String &Name);
void writeEEPROMAddr(uint16_t, uint16_t);

void initPage(uint8_t);
void activatePage(uint8_t);

void programPage(uint8_t);

void locosPage(uint8_t);
void locosDrawPage(uint8_t);

void encoderInterrupt();
void checkREButton();
void PinA();
void PinB();

void routesPage(uint8_t);
void routeDrawPage(uint8_t);

void throttlePage(uint8_t);
void refreshThrottleValues(uint8_t);
void setActiveSlot(uint8_t);
void activateSlot(uint8_t);
void deActivateSlot(uint8_t);
void setLocoDetails(uint8_t);
void updateSlotAddresses();
void loadFunctions(uint8_t, uint8_t);
void changeDir(uint8_t);
void updateThrottle();

void nextionSetText(String, String);
void nextionCommand(String);
void nextionSetValue(String, uint8_t);
void updateNextionThrottle(uint8_t);

String nextionReceive();
void setGuest();
void getNextionText(String);
void getValue(String);

/*
 **********************************************************************************************************
 * Data types expected to be received from the Nextion
 **********************************************************************************************************
*/
#define GUEST_ADDRESS 1
#define LOCO_ADDRESS 2
#define THROTTLE_VALUE 3
#define TOUCH_STATE 4
#define ROAD_NAME 5 
#define ROAD_NUMBER 6
#define LOCO_TYPE 7
#define THRESHOLD_VALUE 11
#define RE_ACCEL_VALUE 12
#define ACC_CMD_GAP 13
#define FUNCTION_PULSE_LENGTH 14
#define ACC_ADDRESS 20
#define ACC_IMAGE 21
#define ACC_NAME 22
#define FUNC_NUMBER 23
#define FUNC_IMAGE 24
#define READ_TIMEOUT 25
//#define FUNC_TYPE 26

/*
 * EEPROM Names, Addresses and Memory Map
 * Also refer to EEPROM.ino file for detailed breakdown
 * 
 * ARDUINO EEPROM MEMORY MAP
 * 
 * Loco and Accessory Names are stored on the Enhanced Nextion Models
 * 
 * These are the EEPROM and Emulated EEPROM assignments
 * 
 * numLocos defines the number of addresses, loco names, and function groups
 * - 10 for AVR devices using Basic Nextion
 * - 20 for AVR devices (1k Arduino EEPROM + 1k Enhanced Nextion EEPROM available)
 * numFSlots defines the number of function slots for each loco
 * - 10 for All devices
 * numAccs defines the number of Accessories supported
 * - 36 for AVR using Basic Nextion
 * - 48 for AVR using Enhanced Nextion
 * - 96 for ESP using either Basic or Enhanced Nextion
 * numRoutes defines the number of Routes supported
 * - 6 for AVR using Basic Nextion
 * - 12 for AVR using Enhanced Nextion
 * - 48 for ESP using either Basic or Enhanced Nextion
 * 
 * AVR devices using Basic Nextion use 1K of integrated EEPROM as follows:
 * 
 * 10 Loco Addresses of 2 bytes each                      =  20 - Addresses 0 to 39    - Base Address = 0
 * 10 Loco Names of 21 bytes each                         = 210
 * 10 function slots using 2 bytes for each slot 20x20    = 200 - Addresses 40 to 439  - Base Address = 40
 * 36 Accessory Addresses using 2 bytes each              =  72 - addresses 440 to 536 - Base Address = 440
 * 36 Accessory Names of 9 bytes each                     = 324
 * 36 Accessory Image numbers                             =  36 - addresses 536 to 584 - Base Address = 536
 * 36 Accessory Types (and optional State Planned)        =  36 - addresses 584 to 632 - Base Address = 584
 * 6 Route Contents of 12 bytes each                      =  72 - addresses 632 to 776 - Base Address = 632
 * Configuration details reside from 920 - 1024           =  54 - addresses 920 to 1024- Base Address = 920
 *                                                         1024 bytes
 *                                                         
 * For the sake of code simplicity, ID numbers internally always start at 0
 * Any communication with the base station where ID number is needed adds the relevant baseID to the ID
 * 
 * To Calculate the Relative position within the appropriate EEPROM block 
 * 
 * ID numbers are first multiplied by their block size, then added to their respective Base Address to 
 * determine their physical address
 * 
 * The Base addresses which are used by the ESP and AVR with Basic Nextion are as follows:
 * locoNameBase   = 0
 * locoRNumBase   = locoNameBase + (numLocos * 19)
 * locoAddrBase   = locoRNumBase + (numLocos * 2)
 * locoFuncBase   = locoAddrBase + (numLocos * 2)
 * accNameBase    = locoFuncBase + (numSlots * 2 * numLocos)
 * accAddrBase    = accNameBase + (numAccs * 9)
 * accImageBase   = accAddrBase + (numAccs * 2)
 * accTypeBase    = accImageBase + numAccs
 * routeListBase  = accTypeBase + numAccs                     //These are the Accessory IDs in the Route
 * routeStateBase  = routeListBase + (numRoutes * routeLen/2) //These are the States for the IDs in the Route
 * configBase     = routeListBase + (numRoutes * routeLen)    //- not used
 * 
 * eg. The DCC Address of Loco ID 7 would be located at: 7(ID) x 2(bytes per address) + 0(locoBaseAddress) = Address 0014
 *     The DCC Address of Accessory ID 10 would be located at 10(ID) x 2(bytes per address) + 440(locoBaseAddress) = Address 0460
 *                                                     
 * Nextion EEPROM is used to store Loco and Accessory Names for AVR devices using Enhanced Nextion Displays only
 * 
 * Loco Names:
 * - 20 names of 20 characters each (21 total)         = 420 uint8_ts   Range is from 0 to 420    Base Address is:   0
 *   - 20 for Name, 1 for null terminator
 * Accessory Names
 * - 48 names of 8 characters each (9  total)          = 432 uint8_ts   Range is from 420 to 852  Base Address is: 420
 *   - 8 for name, 1 for null terminator
 *  
*/
//EEPROM Memory Assignments
  #define locoAddrLen 2
  #define locoNameLen 9
  #define locoTypeLen 9
  #define locoRNumLen 2
  #define accNameLen 9
  #define addrLen 2
  #define slotLen 2       //function slots
  #define routeLen 12     //2 bytes per object - ID and State

  
  //UNO Integrated EEPROM Assignments

  //ID Numbers
  const uint8_t accIDBase         = 0;
  const uint8_t locoIDBase        = 128;
  const uint8_t routeIDBase       = 200;
/*
  const uint16_t locoAddrBase  = 0;                                                 //Starts at 0 and ends at 0+(10*2) = 20 bytes = 0-19
  const uint16_t locoNameBase = locoAddrBase + (numLocos*locoAddrLen);              //Starts at 20 and ends at 20+(10*9) = 110 bytes = 20 to 119
  const uint16_t locoTypeBase  = locoNameBase + (numLocos*locoNameLen);             //Starts at 120 and ends at 120+(10*9) = 210 bytes = 120 to 209
  const uint16_t locoRNumBase = locoTypeBase + (numLocos*locoTypeLen);              //Starts at 210 and ends at 210+(10*2) = 230 bytes = 210 to 229
  const uint16_t locoFuncBase  = locoRNumBase + (numLocos*locoRNumLen);             //Starts at 230 and ends at 230+(10*2) = 250 bytes = 230 to 249
  const uint16_t accAddrBase   = locoFuncBase + (numLocos*numFSlots*slotLen);  //Starts at 250 and ends at 250+(10*10*2) = 450 = 250 to 449
  const uint16_t accNameBase   = accAddrBase + (numAccs * addrLen);
  const uint16_t accImageBase  = accNameBase + (numAccs * accNameLen);              //Starts at 450 and ends at 450+(36*2) = 522  = 450 to 521
  const uint16_t accTypeBase   = accImageBase + numAccs;                            //Starts at 522 and ends at 522+(36) = 538  = 522 to 557
  const uint16_t routeListBase  = accTypeBase + numAccs;                            //Starts at 558 and ends at 558+(36) = 594  = 558 to 593
  const uint16_t routeStateBase = routeListBase + (numRoutes * routeLen/2);         //Starts at 594 and ends at 594+(6*6) = 630  = 594 to 629
*/
  #define locoAddrBase  0                                                   //Starts at 0 and ends at 0+(10*2) = 20 bytes = 0-19
  #define locoNameBase (locoAddrBase + (numLocos*locoAddrLen))              //Starts at 20 and ends at 20+(10*9) = 110 bytes = 20 to 119
  #define locoTypeBase  (locoNameBase + (numLocos*locoNameLen))             //Starts at 120 and ends at 120+(10*9) = 210 bytes = 120 to 209
  #define locoRNumBase (locoTypeBase + (numLocos*locoTypeLen))              //Starts at 210 and ends at 210+(10*2) = 230 bytes = 210 to 229
  #define locoFuncBase  (locoRNumBase + (numLocos*locoRNumLen))             //Starts at 230 and ends at 230+(10*2) = 250 bytes = 230 to 249
  #define accAddrBase  (locoFuncBase + (numLocos*numFSlots*slotLen))        //Starts at 250 and ends at 250+(10*10*2) = 450 = 250 to 449
  #define accNameBase   (accAddrBase + (numAccs * (addrLen)))
  #define accImageBase  (accNameBase + (numAccs * accNameLen))              //Starts at 450 and ends at 450+(36*2) = 522  = 450 to 521
  #define accTypeBase   (accImageBase + numAccs)                            //Starts at 522 and ends at 522+(36) = 538  = 522 to 557
  #define routeListBase  (accTypeBase + numAccs)                            //Starts at 558 and ends at 558+(36) = 594  = 558 to 593
  #define routeStateBase (routeListBase + (numRoutes * routeLen/2))         //Starts at 594 and ends at 594+(6*6) = 630  = 594 to 629

  #define eeLocoStopAll (eepromEnd - 21)      //1023 - 79 = 944
  #define eeFunctionPulse (eepromEnd - 20)    //1023 - 78 = 945
  #define eeRNumEnabled (eepromEnd - 19)      //1023 - 77 = 946
  #define eeAccDelay    (eepromEnd - 18)      //1023 - 76 = 947  Delay between Accessory Route Commands
  #define eeSelIDs      (eepromEnd - 16)      //1023 - 16 = 1007  10 Selected IDs
  #define eeThreshold   (eepromEnd - 6)       //1023 - 6  = 1017
  #define eeREIncrement (eepromEnd - 5)       //1023 - 5  = 1018
  #define eePUState     (eepromEnd - 4)       //1023 - 4  = 1019
  #define eeDCCRefresh  (eepromEnd - 3)       //1023 - 3  = 1020
  #define eeActiveSlot  (eepromEnd - 2)       //1023 - 2  = 1021
  #define eeCheckByte   eepromEnd             //1023 - 0  = 1023


/*
 * eeSelIDs         @             //10 Bytes
 * eeLocoRNumBase   @             //20 Bytes
 * eeLocoAddrBase   @             //20 Bytes
 * eeRNumEnabled    @             //1 Byte
 * eeActiveSlot     @             //1 Byte
 * eeThreshhold     @             //1 Byte
 * eeREincrement    @             //1 Byte
 * eePUState        @             //1 Byte
 * eeDCCRefresh     @             //1 Byte
 * eeAccDelay       @             //1 Byte
 * eeInitCheck      @1023
*/

const uint8_t eeAddrLen = 2;
const uint8_t accessoriesPerPage = 12;
const uint8_t locosPerPage = 10;
const uint8_t functionsPerPage = 10;
const uint8_t routesPerPage = 6;

const uint16_t accStartAddress = 440;   //locoAddressBlock(40) + funcDetailsBlock(400)

/*
 ********************************************************************************************************************************
 * Defines for The Nextion Pages
 ********************************************************************************************************************************
*/
#define CoverPage 0
#define MenuPage 1
#define ThrottlePage 2
#define LocoEditPage 3
#define LocosPage 4
#define AccPage 5
#define RoutesPage 6
#define ProgramPage 7 
#define ConfigPage 8
#define WiFiPage 9
#define AccEditPage 10
#define FNumbersPage 11     //Function Numbers Page
#define FImagesPage 12      //Function Image Page
#define AccImagesPage 13
#define FunctionEditPage 14
#define CreditsPage 15
/*
 ****************************************************************************************************
 * Nextion Buttons
 * 
 * These numbers are what each Button sends to Identify itself 
 * and is Hard Coded into the Nextion HMI Touch or Release Event
 ****************************************************************************************************
 * Menu Page and Navigation Buttons
 **********************************
*/
#define ThrottleButton 108
#define LocosButton 110
#define AccButton 112
#define RoutesButton 114
#define ProgramButton 116
#define MenuButton 126
#define ConfigButton 118         //To Select Config Page
//#define WiFiButton 102
//#define TipsButton 120
#define CreditsButton 122
//#define InfoButton 124

#define SelectButton 142
#define DeleteButton 140
#define DoneButton 100
#define CancelButton 104
#define PageUpButton 130
#define PageDownButton 132
#define SaveButton 106
#define LoadButton 178
#define EditButton 128

/*
 ********************************************************************************************************
 * Page Buttons
 **********************************
 * All Pages
*/
#define PowerOFFButton 134
#define PowerONButton 135
#define StopButton 136
#define GoButton 137

/*
 ********************************************************************************************************
 * Throttle Page
 */
#define SliderPress 146
#define SliderMove 146
#define SliderEvent 146

#define ForwardButton 149
#define ReverseButton 148

#define GuestButton 144
#define GuestButtonRelease 145

/*
 ********************************************************************************************************
 * Loco Edit Page
*/
#define AD_Press 150
#define Na_Press 152
#define Nb_Press 154
#define Nc_Press 156

/*
 ****************************************************************************************************
 * Function Edit Page
*/
#define TestButtonOn 204
#define TestButtonOff 205

#define Func_Num_Press 208
#define Func_Image_Press 226
#define Func_Pulsed 210
#define Func_Latched 211
/*
 ****************************************************************************************************
 * Accessory Edit Page
*/
#define Acc_AD_Press 194
#define Image_Selected 224
#define Acc_N_Press 206
/*
 ****************************************************************************************************
 * Program Page
*/
#define Program_AD_Press 156
#define CVN_Press 162
#define CVV_Press 164

#define PROGRAM_ADDRESS 8
#define CV_NUMBER 9
#define CV_VALUE 10
/*
 ****************************************************************************************************
 * Configuration Page
*/
#define Th_Press 216
#define RE_Accel_Press 218
#define Acc_Cmd_Delay 220
#define Func_Pulse_Len 222
#define Read_Timeout 224
/*
 * Offsets for start of Slot numbers
*/
#define SlotAStart 20           //Used by Functions
#define SlotBStart 50           //Used by Accessories
#define SlotCStart 30           //Used by Accessories and Routes
#define TabSlotStart 70         //Used by Throttle
#define FunctionSlotStart 80    //Used by Functions
#define FunctionPressStart 80   //Used by Functions
#define FunctionReleaseStart 90 //Used by Functions
/*
 ****************************************************************************************************
 * Nextion Object Image Numbers
 * IMPORTANT:
 * These numbers correspond DIRECTLY with the position of the Nextion HMI Images
 * DO NOT MODIFY!!!
 ****************************************************************************************************
*/

//Throttle Page Tab Images
#define Left0 10
#define Left1 11
#define Left2 12
#define Left3 13
#define Left4 14
#define Right0 15
#define Right1 16
#define Right2 17
#define Right3 18
#define Right4 19
#define LeftNone 20
#define RightNone 21

#define tabBase 10

//General Image Numbers

#define BLANK 22
#define GREYED_BUTTON 23
#define QMARK 119
#define PAGEUP 24
#define PAGEUPON 25
#define PAGEUPGREYED 26
#define PAGEDN 27
#define PAGEDNON 28
#define PAGEDNGREYED 29
#define BUTTON_OFF 132
#define BUTTON_ON 133
const char SELECT_OFF = 144;
const char SELECT_ON = 145;
#define EDIT_OFF 164
#define EDIT_ON 165
#define LATCH 0
#define PULSE 1
#define RNUM_OFF 170
#define RNUM_ON 171
#define PU_OFF 172
#define PU_ON 173
#define DCC_OFF 174
#define DCC_ON 175
#define STOP_OFF 176
#define STOP_ON 177
#define ROUTE_OFF 244
#define ROUTE_ON 245
#define DELETE_OFF 19
#define DELETE_ON 18

#define BLANK_BUTTON 184
/*
 ****************************************************************************************************
 * Locos Page - Buttons, defines and Constants
 * 
*/
const uint8_t rowsPerNextionLocosPage = 10;         //Nextion's design

/*
 ****************************************************************************************************
 * Accessory Page - Buttons, defines, and constants
 *
*/
bool accEditMode = false;
bool accSelectMode = false;
const uint8_t eeAccBlockSize = 3;       //2 uint8_t Address and 1 uint8_t Image

bool funcEditMode = false;

/*
 ****************************************************************************************************
 * Routes Page - Buttons, defines and Constants
 * 
*/
const uint8_t rowsPerNextionRoutePage = 6;         //Nextion's design
#define accessoriesPerRoute 6
uint8_t routeStartID = 0;
uint8_t routeList[accessoriesPerRoute][2];
uint8_t accCount = 0;

/*
 ****************************************************************************************************
 * Programming Page - Buttons, defines and Constants
 * 
*/
#define MainButton 159
#define ProgButton 161
#define ReadButton 166
#define WriteButton 168
bool replyExpected = false;            //reply from Command Station Expected
uint8_t progMode = 0;                      //progMain or progProg
const uint8_t progMain = 1;            //Programming on the Main Track
const uint8_t progProg = 2;            //Programming on the Programming Track

/*
 *****************************************************************************************************
 * Accessory Edit and Accessory Images Page Specific Buttons
 * 
*/

#define DCCButton 196
#define VPinButton 198
#define ServoButton 200
#define LCNButton 202
//#define ImageButton 65

/*
 ****************************************************************************************************
 * Convenient Names used by Multiple routines in the Throttle Code
 * 
*/
#define  MAX_COMMAND_LENGTH         30 //Maximum length of a string received from the Base Station

#define fGroup4 222           // Don't change - mask to generate higher function numbers
#define fGroup5 223           // Don't change - mask to generate higher function numbers
#define incAmount 5           // change this to your preferred acceleration rate of the Rotary Encoder

/*
 * Slot Names for Loco Selection
*/
#define slot1 0
#define slot2 1
#define slot3 2
#define slot4 3
#define slot5 4
#define slot6 5
#define slot7 6
#define slot8 7
#define slot9 8
#define slot10 9
/*
 * Colours
*/
const uint16_t BLACK = 0;
const uint16_t WHITE = 65535;

#define LIGHT 50712
#define DARK 19049

const uint16_t GREY = GREY_TO_USE;

const uint16_t DARK_GREY = 25388;
const uint16_t YELLOW = 65504;
/*
************************************************************************************************************************
* 
***********************************************************************************************************************
*/

uint8_t accStates[numAccs];
uint8_t selectedAccs[numAccs];  //for Routes...
/*
 * Global Variables used by Throttle
*/

//static char commandString[MAX_COMMAND_LENGTH+1];   //MAX_COMMAND_LENGTH+1];
char commandString[MAX_COMMAND_LENGTH+1];   //MAX_COMMAND_LENGTH+1];

bool  deleteMode = 0;
uint8_t eMode = 0;
String message;
//String wifiStatus;
//String wifiProgress;
int key ;
uint8_t g_fSlot;
uint8_t editImage;
uint8_t funcNum;

unsigned long lastTime;
unsigned long elapsedTime;
unsigned long DCCtime;
unsigned long DCCcurrentTime;
bool lastA = false;
unsigned long number = 0;
unsigned int lowest = 0;
unsigned int highest = 126; //126;
bool rcvCMD = false;
//bool editMode  = false;

uint8_t activeSlot;
uint8_t previousSlot;
uint8_t selectedIDs[numLocoSlots];
uint8_t resumeSpeeds[numLocoSlots];
uint8_t backupID;
uint8_t editingID;
uint8_t selectedLocoID;
uint8_t locoStartID = 0;
uint8_t accStartID = 0;
uint8_t routesStartID = 0;
uint8_t programID;
uint8_t nextID;               //Next available ID

#if defined DCCPP
  uint8_t LocoFN0to4[numLocos] = {128};   //{128, 128, 128, 128, 128, 128, 128, 128, 128, 128};
  uint8_t LocoFN5to8[numLocos] = {176};   //, 176, 176, 176, 176, 176, 176, 176, 176, 176};
  uint8_t LocoFN9to12[numLocos] = {160};  //, 160, 160, 160, 160, 160,160, 160, 160, 160};
  uint8_t LocoFN13to20[numLocos] = {0};   //, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t LocoFN21to28[numLocos] = {0};   //, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif

uint8_t functions[numLocos][numFSlots]; 

uint8_t id;                                   //index into arrays
boolean DCCflag = 0;
boolean ledPin_state;
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long currentMillis = millis();
const long interval = 300;
unsigned int saveAddress = 0;
uint8_t aFlag = 0;
uint8_t bFlag = 0;
int encoderPos = 0;
uint8_t oldEncPos = 0;
uint8_t oldSpeed = 0;
uint8_t reading = 0;
uint8_t old_pos = 0;  //encoderPos;
boolean buttonState = 0;
unsigned long interruptTime;
uint8_t button;
uint8_t buttonPos;
boolean dir = 1;    // 0 for backwards, 1 for forwards
uint8_t functionSlot = 127;
uint8_t directionFlag = 0;

uint8_t fType = 0;  //Function type being edited
String changeButton = "";
int nextionPage = 0;      //Current Page displayed on Nextion
boolean PowerState = 1;
boolean Processed = 0;
int TOAddress = 0;
int TOSubAddress = 0;
boolean TOState = 0;
boolean REFlag = 0;
String dccppCMD = "";      //String sent to DCC++ Base Station

uint8_t nextionDataType = 0;
uint8_t savedID = 0;

//Guest Additions
uint8_t guestActive = 0;
//uint8_t guestSpeed = 0;
//uint8_t oldGuestSpeed = 0;
uint8_t guestDir = 0;
uint16_t guestAddress = 0;
uint8_t newSpeed = 0;

int long startTime = 0;
uint8_t readTimeout = 15;

uint16_t g_programAddress = 0;
uint8_t g_cvNumber = 0;
uint8_t g_cvValue = 0;
uint8_t ProgReturnPage = MenuPage;
uint8_t AccReturnPage = MenuPage;
uint8_t LocoEditReturnPage = 0;
uint8_t LocosReturnPage = 0;
uint8_t RoutesReturnPage = 0;

#endif

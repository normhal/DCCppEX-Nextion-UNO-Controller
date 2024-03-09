#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "ImageDefs.h"

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
    #error "This Program is Designed for UNO and variants above Only!"
  #endif

  #define EEPROMCODE 106

  const uint8_t activeSlot = 0;
  const uint8_t ReverseThreshold = 30;        //Speed after which loco will stop when direction is changed
  const uint8_t REAccAmount = 5;              //Change this to your preferred acceleration rate of the Rotary Encoder
  const bool DCCRefresh = false;
  uint8_t PowerUpState  = true;               //Change to "false" to Start up withPower OFF
  const uint8_t AccDelay = 10;                //Number of 10ms intervals between Route Accessory activations
  const uint8_t FunctionPulse = 5;
  uint8_t LocoStopAll = true;

  SoftwareSerial nextion(4, 5); // Nextion TX to pin 4 and RX to pin 5 of Arduino UNO
  
  #define eepromSize 1023              //Integrated Arduino UNO EEPROM
  #define eepromEnd eepromSize
  #define numLocos 10                 //Multiples of 10
  #define numAccs 36                  //Multiples of 12
  #define numRoutes 6                 //Multiples of 6
  #define numLocoSlots 10             //Always 10 for this version
  const uint8_t numFSlots = 10;       //Function Slots per loco  - always 10 for this version
  #define BLANK 0x00

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

  #define locoAddrLen 2
  #define locoNameLen 9
  #define locoTypeLen 9
  #define locoRNumLen 2
  #define accNameLen 9
  #define addrLen 2
  #define slotLen 2       
  #define routeLen 12

  const uint8_t accIDBase         = 0;
  const uint8_t locoIDBase        = 128;
  const uint8_t routeIDBase       = 200;

  const uint16_t locoAddrBase  = 0;                                                 //Starts at 0 and ends at 0+(10*2) = 20 bytes = 0-19
  const uint16_t locoNameBase = locoAddrBase + (numLocos*locoAddrLen);              //Starts at 20 and ends at 20+(10*9) = 110 bytes = 20 to 119
  const uint16_t locoTypeBase  = locoNameBase + (numLocos*locoNameLen);             //Starts at 120 and ends at 120+(10*9) = 210 bytes = 120 to 209
  const uint16_t locoRNumBase = locoTypeBase + (numLocos*locoTypeLen);              //Starts at 210 and ends at 210+(10*2) = 230 bytes = 210 to 229
  const uint16_t locoFuncBase  = locoRNumBase + (numLocos*locoRNumLen);             //Starts at 230 and ends at 230+(10*2) = 250 bytes = 230 to 249
  const uint16_t accAddrBase   = locoFuncBase + (numLocos*numFSlots*slotLen);       //Starts at 250 and ends at 250+(10*10*2) = 450 = 250 to 449
  const uint16_t accNameBase   = accAddrBase + (numAccs * (addrLen));
  const uint16_t accImageBase  = accNameBase + (numAccs * accNameLen);              //Starts at 450 and ends at 450+(36*2) = 522  = 450 to 521
  const uint16_t accTypeBase   = accImageBase + numAccs;                            //Starts at 522 and ends at 522+(36) = 538  = 522 to 557
  const uint16_t routeListBase  = accTypeBase + numAccs;                            //Starts at 558 and ends at 558+(36) = 594  = 558 to 593
  const uint16_t routeStateBase = routeListBase + (numRoutes * routeLen/2);         //Starts at 594 and ends at 594+(6*6) = 630  = 594 to 629

  /***********************************************************************************************************/
  //Up to 10 Locos can be defined for an UNO
       
  #define numLocoDefs 10    //Don't exceed 10 for an UNO
  //Loco Road Names - max 8 characters each
  String userLocoNames[numLocoDefs]   = {"Santa Fe", "UP", "CP Rail", "BNSF", "UP"        //Max 8 characters per name
                                          ,"Santa Fe", "UP", "CP Rail", "CP Rail", "BNSF"
                                        };
  //Loco Types - max 8 characters each
  String userLocoTypes[numLocoDefs]   = {"SD40-2", "SD40-2", "SD40-3", "SD40-2", "SD80"        //Max 8 characters per type
                                          ,"SD40-2", "SD40-2", "SD40-3", "SD80", "SD40-2"
                                        };
  //Loco Addresses - max 4 digits per address
  uint16_t userLocoAddrs[numLocoDefs] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

  //Loco Road Numbers - max 4 digits per road number                                   
  uint16_t userLocoRNums[numLocoDefs] = {5108, 5109, 5110, 5111, 5112 
                                          ,5113, 5114, 5115, 5116, 5117
                                        }; 
/* 
  Possible values for Function numbers:-
    f0 = Function 0
    f1 = Function 1
    etc f2..f28 = Functions F2 to F28

  Possible Values for Images:-
    F0 = Full Button with F0 text
    F1 = Full Button with F1 text
    etc F2 to F28 for Function 2 to 28 

  Images for any Funcion
    HEADLIGHT = Headlight without a button image
    TAILLIGHT = Taillight without a button image
    CABLIGHT = Cab light without a button image
    ENGINE = Sound without a button Image
    SMOKE = Smoker without a button image
    BELL = Bell without a button Image
    HORN = Horn without a button image

    FL_B = FL on a Button Image
    HEADLIGHT_B = Headlight image on a Button
    TAILLIGHT_B = Taillight on a button image
    CABLIGHT_B = Cab light on a button image
    ENGINE_B = Sound on a button image
    SMOKE_B = Smoker on a button image
    BELL_B = Bell on a button image
    HORN_B = Horn on a button image
*/

    //Loco Functions for Slot 1
    uint8_t userLocoFSlot1Num[numLocoDefs] = {f0,f0,f0,f0,f0,f0,f0,f0,f0,f0};       //all 10 locos have an F0F
    uint8_t userLocoFSlot1Img[numLocoDefs] = {F0,FL_B,HEADLIGHT,HEADLIGHT_B,F0,F0,F0,F0,F0,F0};                          //Smaple images for f0
    //Loco Functions for Slot 2
    uint8_t userLocoFSlot2Num[numLocoDefs] = {f2,f3,f1,f4,f5,f6,f7,f8,f1,f1};     
    uint8_t userLocoFSlot2Img[numLocoDefs] = {F2,TAILLIGHT,CABLIGHT,HORN,SMOKE_B,HORN_B,TAILLIGHT_B,HORN_B,F1,F1};
    //Loco Functions for Slot 3
    uint8_t userLocoFSlot3Num[numLocoDefs] = {f2,f2,f2,f2,f2,f2,f2,f2,f2,f2};    //all 10 locos have a function 2
    uint8_t userLocoFSlot3Img[numLocoDefs] = {HORN,HORN,HORN,HORN,HORN,HORN,HORN,HORN,HORN,HORN};
    //Loco Functions for Slot 4
    uint8_t userLocoFSlot4Num[numLocoDefs] = {f8,f8,f8,f8,f8,f8,f8,f8,f8,f8};    //all 10 locos have a function 8
    uint8_t userLocoFSlot4Img[numLocoDefs] = {ENGINE,ENGINE,ENGINE,ENGINE,ENGINE,ENGINE,ENGINE,ENGINE,ENGINE,ENGINE};

  /***********************************************************************************************************/
  //Up to 12 Accessory Names can be defined
      
  #define numAccDefs 12       //12 fills one page on the Nextion Controller

  //Accessory Names - max 9 characters each
  String userAccNames[numAccDefs] = {"ST1-W1",
                                     "ST1-W2",
                                     "ST1-W3",
                                     "ST2-W1",
                                     "ST2-W2",
                                     "ST2-W3",
                                     "ST1-E1",
                                     "ST1-E2",
                                     "ST1-E3",
                                     "ST2-E1",
                                     "ST2-E2",
                                     "ST2-E3"
                                    };
    
  uint16_t userAccAddrs[numAccDefs] = {400,401,402,406,407,408,403,404,405,409,410,411};

  // Accessory Images as defined within the Nextion
  // Don't modify these defines - they're there to help:-)
  #define LH0 202     //Left Hand 0 degrees
  #define LH90 200    //Left Hand 90 Degrees
  #define LH180 206   //Left Hand 180 Degrees
  #define LH270 204   //Left Hand 270 Degrees
  #define RH0 210     //Right Hand 0 degrees
  #define RH90 208    //Right Hand 90 Degrees
  #define RH180 214   //Right Hand 180 Degrees
  #define RH270 212   //Right Hand 270 Degrees

  uint8_t  userAccImages[numAccDefs] = {LH0,RH180,RH0,LH180,RH0,LH180,LH0,RH180,LH0,RH180,LH0,RH180};
  uint8_t  userAccTypes[numAccDefs] = {0,0,0,0,0,0,0,0,0,0,0,0};    //Types not used (yet)

  /***********************************************************************************************************/
  //Up to 6 Routes can be defined - 6 accessory IDs per route   
    
  #define numRouteDefs 2          //6 Routes per Nextion Page

  #define accessoriesPerRoute 6
  #define Closed 0                     //Closed = 0
  #define Thrown 1                     //Thrown = 1
    
  uint8_t userRouteIDs[numRouteDefs*accessoriesPerRoute]    = {0,1,2,6,7,8,
                                                                0,1,2,6,7,8
                                                              };            //Format of a Route is accessoriesPerRoute x Acc IDs, state pairs
  uint8_t userRouteStates[numRouteDefs*accessoriesPerRoute] = {Closed,Closed,Closed,Thrown,Thrown,Thrown,
                                                                Thrown,Thrown,Thrown,Closed,Closed,Closed
                                                              };

  //Don't modify this
  const uint8_t fBlockSize = (numFSlots * 2);       //Slots x 2 bytes each - Function and Image number

/*
 ***********************************************************************************************************************************
*/
void writeEEPROMByte(uint16_t eeAddress, uint8_t eeData)
{
  EEPROM.update(eeAddress, eeData);  
}
/*
 ***********************************************************************************************************************************
 * Write a String to Arduino or ESP EEPROM
 ***********************************************************************************************************************************
*/
void writeEEPROMName(uint16_t eeAddress, const String &Name)
{
  uint8_t i;
  uint8_t len = Name.length();
  for (i = 0; i < len; i++) EEPROM.update(eeAddress + i, Name[i]);
  EEPROM.update(eeAddress + i, '\0');
}
// ***********************************************************************************************************************************
// * EEPROM Write Routines - for Arduino Integrated EEPROM
// * Loco Addresses are stored in Arduino/ESP EEPROM from address 0 to 39 - 2 bytes each
// ***********************************************************************************************************************************
void writeLocoAddress(uint8_t locoID, uint16_t locoAddress)
{ 
  uint16_t eeAddress = (locoAddrBase+(locoID * 2));
  uint8_t byte1 = locoAddress >> 8;
  uint8_t byte2 = locoAddress & 0xFF;
  EEPROM.update(eeAddress, byte1);
  EEPROM.update(eeAddress + 1, byte2);
}
/*********************************************************************/
void writeLocoRNum(uint8_t locoID, uint16_t locoRNum)
{ 
  uint16_t eeAddress = (locoRNumBase+(locoID * 2));
  uint8_t byte1 = locoRNum >> 8;
  uint8_t byte2 = locoRNum & 0xFF;
  EEPROM.update(eeAddress, byte1);
  EEPROM.update(eeAddress + 1, byte2);
}
/*********************************************************************/
void writeAccAddress(uint8_t ID, uint16_t Address)
{ 
  uint16_t eeAddress = (accAddrBase+(ID * 2));
  uint8_t byte1 = Address >> 8;
  uint8_t byte2 = Address & 0xFF;
  EEPROM.update(eeAddress, byte1);
  EEPROM.update(eeAddress + 1, byte2);
}
/*
 ***********************************************************************************************************************************
 * Write a 2 uint8_t "Address" to EEPROM
 ***********************************************************************************************************************************
*/
void writeEEPROMAddr(uint16_t eeAddress, uint16_t wordToWrite)
{ 
  uint8_t uint8_t1 = wordToWrite >> 8;
  uint8_t uint8_t2 = wordToWrite & 0xFF;
  EEPROM.update(eeAddress, uint8_t1);
  EEPROM.update(eeAddress + 1, uint8_t2);
}
/**********************************************************************/
// initEEPROM ensures all important EEPROM values are set to Default
/**********************************************************************/
void initEEPROM()
{
  uint8_t j = 0;
  for (uint16_t i = eeSelIDs; i < (eeSelIDs + numLocoSlots); i++)
  {
    writeEEPROMByte(i ,j);  //Initialize the Selected IDs with incrementing values from 0 to numLocoSlots
    j++;
  }
  writeEEPROMByte(eeActiveSlot, activeSlot);
  writeEEPROMByte(eeThreshold, ReverseThreshold);
  writeEEPROMByte(eeREIncrement, REAccAmount);
  writeEEPROMByte(eePUState, PowerUpState);
  writeEEPROMByte(eeDCCRefresh, DCCRefresh);
  writeEEPROMByte(eeAccDelay, AccDelay);
  writeEEPROMByte(eeFunctionPulse, FunctionPulse);
  writeEEPROMByte(eeLocoStopAll, LocoStopAll);

  //Initialize every Road Number to 0
  for (int i = locoRNumBase; i < (locoRNumBase + (numLocos * 2)); i++) writeEEPROMByte(i, 0);  //the *2 is  because of a two byte address
  for (int i = locoAddrBase; i < (locoAddrBase + (numLocos * 2)); i++) writeEEPROMAddr(i, 0);  //the *2 is  because of a two byte address
  for(int i = locoFuncBase; i < accAddrBase; i = i+2)
  {
    writeEEPROMByte(i, 127);
    writeEEPROMByte(i+1, BLANK);
  }
  for(int i = accAddrBase; i < (accImageBase); i++) writeEEPROMByte(i, 0);
  for(int i = accImageBase; i < (routeListBase); i++) writeEEPROMByte(i, BLANK);
}
//************************************************************************************************
void setup() 
{
  Serial.begin(115200);
  
  delay(2000);
  Serial.println("UNO EEPROM being Initialized...");
  initEEPROM();
  Serial.print("Now Initializing Loco Names starting at: ");
  Serial.println(locoNameBase);
  Serial.print("Now Initializing Loco Types starting at: ");
  Serial.println(locoTypeBase);
  Serial.print("Now Initializing Loco Addresses starting at: ");
  Serial.println(locoAddrBase);
  Serial.print("Now Initializing Loco Road Numbers starting at: ");
  Serial.println(locoRNumBase);
  for(uint16_t i = 0; i < numLocoDefs; i++)
  {
    writeEEPROMName((locoNameBase + (i * (locoNameLen))), String((userLocoNames[i]) + '\0'));
    writeEEPROMName((locoTypeBase + (i * (locoTypeLen))), String((userLocoTypes[i]) + '\0'));
    writeLocoAddress(i, userLocoAddrs[i]);
    writeLocoRNum(i, userLocoRNums[i]);

    writeEEPROMByte((locoFuncBase + (i * 20)) +  0, userLocoFSlot1Num[i]);
    writeEEPROMByte((locoFuncBase + (i * 20)) +  1, userLocoFSlot1Img[i]);
    writeEEPROMByte((locoFuncBase + (i * 20)) +  2, userLocoFSlot2Num[i]);
    writeEEPROMByte((locoFuncBase + (i * 20)) +  3, userLocoFSlot2Img[i]);
    writeEEPROMByte((locoFuncBase + (i * 20)) +  4, userLocoFSlot3Num[i]);
    writeEEPROMByte((locoFuncBase + (i * 20)) +  5, userLocoFSlot3Img[i]);
    writeEEPROMByte((locoFuncBase + (i * 20)) +  6, userLocoFSlot4Num[i]);
    writeEEPROMByte((locoFuncBase + (i * 20)) +  7, userLocoFSlot4Img[i]);
  }
  Serial.println("Now Initializing Accessories...");
  for(uint16_t a = 0; a < numAccDefs; a++)                             //"a" is equivalent to "ID" in this instance
  {
    writeEEPROMName(accNameBase + (a*(accNameLen)), userAccNames[a] + '\0');
    writeAccAddress(a, userAccAddrs[a]);
    writeEEPROMByte(accImageBase + a, userAccImages[a]); 
    writeEEPROMByte(accTypeBase + a, userAccTypes[a]);               //default type 0 for now
  }
  Serial.println("Now Initializing Routes...");
  for(uint8_t r = 0; r < numRouteDefs; r++)      //number of Routes to initialize
  {
    for(uint8_t i = 0; i < accessoriesPerRoute; i++)
    {
      writeEEPROMByte((routeListBase + (r * accessoriesPerRoute)+i), userRouteIDs[(r*accessoriesPerRoute)+i]);
      writeEEPROMByte((routeStateBase + (r * accessoriesPerRoute)+i), userRouteStates[(r*accessoriesPerRoute)+i]);
    }
  }
  writeEEPROMByte(eepromEnd, EEPROMCODE);

  Serial.println("EEPROM Initialized and ready!");
  Serial.print("EEPROM Code Written: ");
  Serial.println(EEPROMCODE);
  Serial.println("Load DCCEX Controller Sketch, compile and flash:-)");
}

void loop() 
{
  
}

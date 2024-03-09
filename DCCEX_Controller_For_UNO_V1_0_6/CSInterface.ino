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
/*
 **************************************************************************************************************************
 * Routines to Process Data to and from the Command Station 
 **************************************************************************************************************************
*/
/*  
 **************************************************************************************************************************
 *  Refresh Loco DCC Data (if Enabled)
 **************************************************************************************************************************
*/
void refreshDCC()                          // Refresh DCC Loco Data 
{
  if (readEEPROMByte(eeDCCRefresh) == 1)
  {
    DCCcurrentTime = millis(); //send DCC every 30000 ms
    if (((DCCcurrentTime - DCCtime) >= 30000) | (DCCflag == 1))
    {
      DCCflag = 0;
      DCCtime = DCCcurrentTime;
      doDCC(activeSlot);
    }
  }
}
/*
 **************************************************************************************************************************
 * Routines to Process Data to and from the Command Station 
 **************************************************************************************************************************
 * Send DCC++ Command String to Command Station
 ***************************************************
*/
void sendCMD(String CMD)
{
  Serial.println(CMD);            //Print to Serial if no WiFi support
}
/*
 ***************************************************
 * Process Data received from the Command Station
 ***************************************************
*/
void receiveCMD()
{
  char c;
  while(Serial.available()>0)
  {
    c=Serial.read();
    if(c=='<') sprintf(commandString,"");                //Don't store the "<"
    else if(c=='>') parse(commandString);                //Also strip off the ">" and parse the received String
    else if(strlen(commandString)<MAX_COMMAND_LENGTH)    // if commandString still has space, append character just read from serial line
    sprintf(commandString,"%s%c",commandString,c);     // otherwise, character is ignored (but continue to look for '<' or '>')
  }
}
/*
 ***************************************************
 * Parse Data received from the Command Station
 * This code is intended to grow as more integration 
 * with the Command Station is included
 ***************************************************
*/
void parse(char *com)
{
  switch(com[0])
  {
    case 'p':       //Power Command Received from Command Station
    {
      if(com[1] == '1') nextionCommand("Power.val=1");
      if(com[1] == '0') nextionCommand("Power.val=0");
      break;
    }
    case 'r':       //Response to Programming Track "Read" command
    {
      if(replyExpected == true)
      {
        char * token = strtok(com, " ");
        String lastToken = "";
        while (token != NULL )
        {
          if(String(token).startsWith("-1")) nextionSetText("Status", "Read Unsuccessful");
          else 
          {
            nextionSetText("Status", "Read Successful");
          }
          token = strtok(NULL, " ");
          lastToken = token;
          nextionSetText("Value", String(lastToken));                         
        }
        replyExpected = false;
      }
      break;
    }
    default:
      break;
  }
}
/*
 *************************************************************************************************************************
 * Routines to Construct Command Strings and send them to the Command Station
 *************************************************************************************************************************
 * Build Loco instruction to Send to Command Station
 *********************************************************
 *
*/
void doDCC(uint8_t locoSlot) 
{
  String dccppCMD = "";
  if(readLocoAddress(selectedIDs[locoSlot]) != 0)
  {
    #if defined DCCPP
      dccppCMD = ("<t " + String(selectedIDs[locoSlot] + 1) + " " + String(readLocoAddress(selectedIDs[locoSlot])) + " "
                 + String(locos[selectedIDs[locoSlot]].speed ) + " " + String(locos[selectedIDs[locoSlot]].dir ) + ">");
    #else
      dccppCMD = ("<t " + String(readLocoAddress(selectedIDs[locoSlot])) + " "
                + String(locos[selectedIDs[locoSlot]].speed ) + " " + String(locos[selectedIDs[locoSlot]].dir ) + ">");
    #endif
    sendCMD(dccppCMD);
  }
  updateNextionThrottle(encoderPos);
}
/*
 *********************************************************
 * Build Function instructions and Send to Command Station
 *********************************************************
*/
#if defined DCCPP

  void doDCCfunction04() 
  {
    String dccppCMD = "";
    dccppCMD = ("<f " + String(readLocoAddress((selectedIDs[activeSlot]))) +
                    " " + String(LocoFN0to4[selectedIDs[activeSlot]]) + ">");
    sendCMD(dccppCMD);
  }
  void doDCCfunction58() 
  {
    String dccppCMD = "";
    dccppCMD = ("<f " + String(readLocoAddress((selectedIDs[activeSlot]))) +
                    " " + String(LocoFN5to8[selectedIDs[activeSlot]]) + ">");
    sendCMD(dccppCMD);
  }
  void doDCCfunction912()          
  {
    String dccppCMD = "";
    dccppCMD = ("<f " + String(readLocoAddress((selectedIDs[activeSlot]))) +
                    " " + String(LocoFN9to12[selectedIDs[activeSlot]]) + ">");
    sendCMD(dccppCMD);
  }
  void doDCCfunction1320()         
  {
    String dccppCMD = "";
    dccppCMD = ("<f " + String(readLocoAddress((selectedIDs[activeSlot]))) + " " + String(fGroup4) +
                    " " + String(LocoFN13to20[selectedIDs[activeSlot]]) + ">");
    sendCMD(dccppCMD);
  }
  void doDCCfunction2128()         
  {
    String dccppCMD = "";
    dccppCMD = ("<f " + String(readLocoAddress((selectedIDs[activeSlot]))) + " " + String(fGroup5)+
                    " " + String(LocoFN21to28[selectedIDs[activeSlot]]) + ">");
    sendCMD(dccppCMD);
  }
#endif
  //Required command for DCCEis "<F Address Function State>"
#if !defined DCCPP
  void doDCCfunctions(uint8_t fNum)
  {
    String dccppCMD = "";
    dccppCMD = ("<F " + String(readLocoAddress((selectedIDs[activeSlot]))) +
              " " + String(fNum) + 
              " " + String(functions[selectedIDs[activeSlot]][g_fSlot]) + ">");
    sendCMD(dccppCMD);  
  }
#endif
/*
 *********************************************************
 * Send Accessory command to Command Station
 *********************************************************
*/
void doDCCAcc(uint8_t accID)      //Send DCC Accessory command
{
  String dccppCMD = "";
  dccppCMD = ("<a " + String(readAccAddress(accID)) + " " + String(accStates[accID]) + ">");
  sendCMD(dccppCMD);
}
/*
 *********************************************************
 * Update Nextion Slider position and Speed value
 *********************************************************
*/
void updateNextionThrottle(uint8_t speed)
{
  encoderPos = speed;
  nextionSetValue("S", speed);
  nextionSetValue("S1", speed);
  nextionSetValue("T", speed);
}

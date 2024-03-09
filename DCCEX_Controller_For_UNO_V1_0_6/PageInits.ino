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
 *************************************************************************************************************************
 * Page Initializations
 *************************************************************************************************************************
*/
void initPage(uint8_t page)
{
  if(page == WiFiPage) return(0);
  activatePage(page);
  if(!PowerState) nextionSetValue("Power",0);
  if(PowerState)  nextionSetValue("Power",1);
  setLocoDetails(activeSlot);
  switch (page)
  {
    //******************************************************************************************************************************
    case ThrottlePage:
    {  
      populateSlots();
      activateSlot(activeSlot);
      break;
    }
    //******************************************************************************************************************************
    case LocoEditPage:
    {
      nextionSetText("AD", String(readLocoAddress(editingID)));
      if(readLocoAddress(editingID) == 0) 
      {
        nextionSetText("Na","");
        nextionSetText("Nb","");
      }else 
      {
        nextionSetText("Na", readEEPROMName(locoNameBase + (editingID * (locoNameLen))));      //Get Loco Name from UNO EEPROM
        nextionSetText("Nb", readEEPROMName(locoTypeBase + (editingID * (locoTypeLen))));      //Get Loco Type from UNO EEPROM
      }
      nextionSetText("RNum", String(readLocoRNum(editingID)));
      loadFunctions(LocoEditPage, editingID);
      break;
    }
    //******************************************************************************************************************************
    case LocosPage:
    {
      backupID = selectedIDs[activeSlot];       //Used by "Cancel" button
      locosDrawPage(locoStartID);
      break;
    }
    //****************************************************************************************************************************** 
    case AccPage:                                                                                                                   
    {
      if (accStartID == 0) nextionCommand("PageDn.pic=PAGEDNGREYED");
      accDrawPage(accStartID);
      break;
    }
    //******************************************************************************************************************************
    case AccEditPage: 
    {
      nextionSetText("ID", String(editingID + accIDBase));
      nextionSetText("N", readEEPROMName(accNameBase + (editingID * (accNameLen))));
      nextionSetText("A", String(readAccAddress(editingID)));
      if(readAccAddress(editingID) != 0)
      {
        nextionCommand(("ImageA.pic=" + String(readAccImage(editingID))).c_str());
        nextionCommand(("ImageB.pic=" + String((readAccImage(editingID))+1)).c_str());    //Add 1 for the partner image
        nextionCommand(("Test.pic=" + String(readAccImage(editingID))).c_str());
        nextionCommand(("Test.pic2=" + String((readAccImage(editingID))+1)).c_str());    //Add 1 for the partner image
        nextionSetValue("DCC",1);
      }else{
        nextionCommand("ImageA.pic=23");   //+ String(GREYED_BUTTON)).c_str());  //23
        nextionCommand("ImageB.pic=23");  //+ String(GREYED_BUTTON)).c_str());
        nextionCommand("Test.pic=23");  //" + String(GREYED_BUTTON)).c_str());
        nextionCommand("Test.pic2=23");  //" + String(GREYED_BUTTON)).c_str());
        nextionSetValue("DCC",1);                                       //Default to DCC
      }
      eMode = 0;
      nextionSetValue("Edit",0);
      break;
    }
    //******************************************************************************************************************************
    case RoutesPage:
    {
      if (routeStartID == 0) nextionCommand("PageDn.pic=PAGEDNGREYED");
      accSelectMode = false;
      eMode = 0;
      routeDrawPage(routeStartID);
      break;
    }
    //******************************************************************************************************************************
    case ProgramPage:
    {
      nextionSetValue("Main", 0);
      nextionSetValue("Prog", 0);
      nextionSetText("AD", String(readLocoAddress(selectedIDs[activeSlot])));
      nextionSetText("CV", "");
      nextionSetText("Value", "");
      progMode = 0;
      break;
    }
    //******************************************************************************************************************************
    case ConfigPage:                                                                                //Done
    {
      nextionCommand("RN.val=" + String(readEEPROMByte(eeRNumEnabled)));
      nextionCommand("Stop.val=" + String(readEEPROMByte(eeLocoStopAll)));
      nextionCommand("PU.val=" + String(readEEPROMByte(eePUState)));
      nextionCommand("DCC.val=" + String(readEEPROMByte(eeDCCRefresh)));       //",String(readEEPROMByte(ee
      nextionSetText("n0", String(readEEPROMByte(eeThreshold)));
      nextionSetText("n1", String(readEEPROMByte(eeREIncrement)));
      nextionSetText("n2", String(readEEPROMByte(eeAccDelay)));
      nextionSetText("n3", String(readEEPROMByte(eeFunctionPulse)));
      break;
    }
    //******************************************************************************************************************************
    case FunctionEditPage:
    {
      if(readLocoAddress(editingID) != 0)
      {
        nextionSetText("Na", readEEPROMName(locoNameBase + (editingID * (locoNameLen))));
        nextionSetText("Nb", readEEPROMName(locoTypeBase + (editingID * (locoTypeLen))));
      }
      nextionSetText("AD", String(readLocoAddress(editingID)));                                               //Load Address
      uint8_t funcNum = (readEEPROMByte((locoFuncBase + (editingID * 20)) + (g_fSlot*2)));
      nextionSetText("F", String(funcNum & 0x7F));   //Load Function Number
      uint8_t funcImage = 118;
      if(funcNum != 127) funcImage = readEEPROMByte((locoFuncBase + (editingID * 20)) + ((g_fSlot*2)+1));                      //Retrieve Image Number
      nextionSetValue("n1", funcImage);                                                               //Load hidden Image Number
      if((funcNum & 0x80) == 128) 
      {
        nextionSetValue("Type", 1);
        fType = 1;
      }else{
        nextionSetValue("Type", 0);
        fType = 0;
      }
      nextionCommand(("ImageA.pic=" + String(funcImage)).c_str());                                         //Load ImageA
      nextionCommand(("Test.pic=" + String(funcImage)).c_str());                                           //Load Test Image
      nextionCommand(("ImageB.pic=" + String(funcImage + 1)).c_str());                                     //Load ImageB
    }
    //******************************************************************************************************************************
    case CreditsPage:                                                                           //Still to Follow
    {
      break;
    }
  }
}
/*
 *************************************************************************************************************************
 * Activate a Nextion Page
 *************************************************************************************************************************
*/
void activatePage(uint8_t page)
{
  nextionPage = page;               //Save new Page reference
  nextionCommand(("page " + String(nextionPage)).c_str());
}
/*
 *************************************************************************************************************************
 * Set PAGEUP and PAGEDN Buttons
 *************************************************************************************************************************
*/
void setPageButtons(uint8_t startID, uint8_t rowsPerPage, uint8_t numRows)
{
  #if !defined NO_PAGING
    if (startID == 0)
    {
      nextionCommand("PageUp.pic=24");    // + String(PAGEUP)).c_str());     //24
      nextionCommand("PageUp.pic2=25");   // + String(PAGEUPON)).c_str());  //25
      nextionCommand(F("PageDn.pic=29"));    // + String(PAGEDNGREYED)).c_str());   //29
      nextionCommand(F("PageDn.pic2=29"));    // + String(PAGEDNGREYED)).c_str());    //29
    }else{   
      nextionCommand("PageDn.pic=27");      // + String(PAGEDN)).c_str());     //27
      nextionCommand("PageDn.pic2=28");     // + String(PAGEDNON)).c_str());  //28
    }
    if (startID < (numRows - rowsPerPage))
    {
      nextionCommand("PageUp.pic=24");   // + String(PAGEUP)).c_str());    //24
      nextionCommand("PageUp.pic2=25");   // + String(PAGEUPON)).c_str());  //25
    }else{
      nextionCommand("PageUp.pic=26");     //+ String(PAGEUPGREYED)).c_str());   //26
      nextionCommand("PageUp.pic2=26");    //+ String(PAGEUPGREYED)).c_str());  //26
    }
  #endif  
}

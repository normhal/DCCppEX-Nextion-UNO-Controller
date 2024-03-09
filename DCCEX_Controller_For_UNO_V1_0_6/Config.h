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

#ifndef CONFIG_H
  #define CONFIG_H

  //#define DCCPP                   //Default is for DCCEX - uncomment for use with DCC++
  
  #define USE_ROTARY_ENCODER      //

  #define SEND_POWER_STATE        //This option allows the Nextion Controller to send Power State Commands at Power-Up

  #define NEW_LEFT_CHAR "_"       //The character on the Left side of the Nextion Spacebar can be re-defined here
  #define NEW_RIGHT_CHAR "$"      //The character on the Rightt side of the Nextion Spacebar can be re-defined here

  //#define DISPLAY_TAB_DETAILS_GREY_BG     //This option ENABLES displaying Road Name, Number or Address, and Loco Type on ALL Throttle Tabs
  #define DISPLAY_TAB_DETAILS_YELLOW_BG   //As Above, but with a YELLOW background on the Active Tab
                                          //Comment BOTH Lines out to have ONLY the Address or Road Number displayed on the Active Tab
  #define GREY_TO_USE LIGHT       //Only applies when using the "Dark" version of the 3.2in Nextion HMI
  //#define GREY_TO_USE DARK
  
  /*
  ***********************************************************************************************************
  * Make all Configuration Changes in the DCCEX_Controller_UNO_Upload_Utility
  * It's vitally important to use maching Version Numbers
  * Compile and Flash that Utility ONCE BEFORE flashing this version.
  * All EEPROM values and Hard Coded Details will then be available 
  ***********************************************************************************************************
  */
 
#endif

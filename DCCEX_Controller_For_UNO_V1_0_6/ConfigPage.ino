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
#include "Config.h"
/*
 *************************************************************************************************************************
 * Process Configuration Page
 *************************************************************************************************************************
*/
void configPage(uint8_t button)
{
  switch (button)
  {
    case SaveButton:
    {
      saveSelected();
      break;
    }
    case DoneButton:
    {
      initPage(MenuPage);
      break;
    }
    case RNUM_OFF:
    {
      writeEEPROMByte(eeRNumEnabled, 0);
      break;
    }
    case RNUM_ON:
    {
      writeEEPROMByte(eeRNumEnabled, 1);
      break;
    }
    case PU_OFF:
    {
      writeEEPROMByte(eePUState, 0);
      break;
    }
    case PU_ON:
    {
      writeEEPROMByte(eePUState, 1);
      break;
    }
    case DCC_OFF:
    {
      writeEEPROMByte(eeDCCRefresh, 0);
      break;
    }
    case DCC_ON:
    {
      writeEEPROMByte(eeDCCRefresh, 1);
      break;
    }
    case STOP_OFF:
    {
      writeEEPROMByte(eeLocoStopAll, 0);
      break;
    }
    case STOP_ON:
    {
      writeEEPROMByte(eeLocoStopAll, 1);
      break;
    }
    case LoadButton:    //Code to load pre-defined details
    {
      nextionCommand(("Load.pic=" + String(BUTTON_ON)).c_str());
      wait(1000);
      nextionCommand(("Load.pic=" + String(BUTTON_OFF)).c_str());
      break;
    }
    case Th_Press:
      nextionDataType = THRESHOLD_VALUE;
      break;
    case RE_Accel_Press:
      nextionDataType = RE_ACCEL_VALUE;
      break;
    case Acc_Cmd_Delay:
      nextionDataType = ACC_CMD_GAP;
      break;
    case Func_Pulse_Len:
      nextionDataType = FUNCTION_PULSE_LENGTH;
      break;
    case Read_Timeout:
      nextionDataType = READ_TIMEOUT;
      break;
    default:
      break;
  }
}

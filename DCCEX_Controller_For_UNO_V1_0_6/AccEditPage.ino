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
 * Accessory Edit Page Routines
 *************************************************************************************************************************
 * Initialize Accessory Edit Page
 * On completion will always return to the Accessory Page
 * Input "Record" components will always come from "editingID"
 * All output data will be placed in their relevant locations:-)
 *************************************************************************************************************************
 * Process Accessory Edit Page
 *
*/
void accEditPage(uint8_t button)
{
  #if !defined NO_ACCESSORIES
    switch(button)
    {
      case DoneButton:
      {
        eMode = 0;
        initPage(AccPage);
        break;
      }
      case CancelButton:
        initPage(AccPage);
        break;
      case Image_Selected:
        nextionDataType = ACC_IMAGE;
        break;
      case Acc_AD_Press:
        nextionDataType = ACC_ADDRESS;
        break;
      case Acc_N_Press:
        nextionDataType = ACC_NAME;
        break;
      case DCCButton:
        break;
      case VPinButton:
        break;
      case ServoButton:
        break;
      case LCNButton:
        break;
      
      default:
        break;
    }
  #endif
}

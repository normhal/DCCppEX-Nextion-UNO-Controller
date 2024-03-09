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
 **********************************************************
 * Poll the Rotary Encoder 
 **********************************************************
*/
void rotaryEncoder() 
{
//#if defined USE_ROTARY_ENCODER
  if (oldEncPos != encoderPos)      // If activity, transmit same to Base Station and Update Nextion
  {    
    oldEncPos = encoderPos;
    if(guestActive == 0)
    { 
      locos[selectedIDs[activeSlot]].speed = encoderPos;
      doDCC(activeSlot);
    }else{
      updateNextionThrottle(encoderPos);
      setGuest();
    }
  }
//#endif
}
/*
 **********************************************************
 * Check the Rotary Encoder Direction Change Button
 **********************************************************
*/
void checkREButton() 
{
  #if defined USE_ROTARY_ENCODER
  buttonState = digitalRead(REButtonPin);
  uint8_t directionFlag = 0;
  currentMillis = millis();
  if (buttonState == LOW) 
  {
    do 
    {
      if (millis() - currentMillis < 200) 
      {
      }
      buttonState = digitalRead(REButtonPin);
      if (millis() - currentMillis >= 1000) 
      {
        if(guestActive == 0)
        {
          locos[selectedIDs[activeSlot]].speed = 0;
          encoderPos = 0;
          oldEncPos = 0;
          directionFlag = 1;
        }else
        {
          encoderPos = 0;
        }
      }
    }
    while (buttonState == LOW);
    if (directionFlag == 0) 
    {
//      directionFlag = 0;
      if(guestActive == 0)
      {
        dir = !dir;
        if (encoderPos >= readEEPROMByte(eeThreshold))
        {
          locos[selectedIDs[activeSlot]].speed = 0;
          encoderPos = 0;
          oldEncPos = 0;
        }
        if(dir) nextionSetValue(F("FR"), 1);
        else nextionSetValue(F("FR"), 0);
        currentMillis = millis();
        locos[selectedIDs[activeSlot]].dir = dir;
        doDCC(activeSlot);
      }else
      {
        guestDir = !guestDir;
        if(guestDir) nextionSetValue("FR", 1);
        else nextionSetValue("FR", 0);
        if(encoderPos >= readEEPROMByte(eeThreshold)){
          encoderPos = 0;
          updateNextionThrottle(encoderPos);
        }
        setGuest();
      }
    }
  }
  #endif
}
/*
 **********************************************************************************************************
 * Send New Throttle values to CS
 **********************************************************************************************************
*/
void updateThrottle()
{
}

/*
 **********************************************************
 * Interrupt Service Routine to PinB
 * This routine increments encoderPos
 **********************************************************
*/
void PinB() 
  {
#if defined USE_ROTARY_ENCODER
  reading = 0;
  if (digitalRead(pinA)) reading = reading | B00000100;
  if (digitalRead(pinB)) reading = reading | B00001000;
  if (reading == B00001100 && bFlag) //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
  {
    if(encoderPos < highest) 
    {
      elapsedTime = (millis() - lastTime);
      encoderPos ++;      //increment the encoder's position count
      if (elapsedTime <= 100)
      {
        if (encoderPos <= (highest - REAccAmount))
        {
          encoderPos = (encoderPos + (REAccAmount - 1));
        }
      }
    } 
    lastTime = millis();
    aFlag = 0; //reset flags for the next turn
    bFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
#endif
}
/*
 **********************************************************
 * Interrupt Service Routine for PinA
 * This routine decrements encoderPos
 **********************************************************
*/
void PinA()
{
#if defined USE_ROTARY_ENCODER
//  cli(); //stop interrupts happening before we read pin values
  reading = 0;
  if (digitalRead(pinA)) reading = reading | B00000100;
  if (digitalRead(pinB)) reading = reading | B00001000;
  if (reading == B00001100 && aFlag) //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
  {
    if (encoderPos >= 1) 
    {
      elapsedTime = (millis() - lastTime);    // Debounce Time
      encoderPos --;
      if (elapsedTime <= 100)
      {
        if (encoderPos >= REAccAmount)
        {
          encoderPos = (encoderPos - (REAccAmount - 1));    //decrement the encoder's position count
        }
      }
    }
    lastTime = millis();
    aFlag = 0; //reset flags for the next turn
    bFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
#endif
}

/* TMC222 library version 1.0

Written by B@tto 
Contact : batto@hotmail.fr


  MCP3424.h - ADC 18 bits i2c library for Wiring & Arduino
  Copyright (c) 2014 Yann LEFEBVRE.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef TMC22_H
#define TMC22_H

#include <Wire.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/* Current constants definition */

#define I_59mA 0x0
#define I_71mA 0x1
#define I_84mA 0x2
#define I_100mA 0x3
#define I_119mA 0x4
#define I_141mA 0x5
#define I_168mA 0x6
#define I_200mA 0x7
#define I_238mA 0x8
#define I_283mA 0x9
#define I_336mA 0xA
#define I_400mA 0xB
#define I_476mA 0xC
#define I_566mA 0xD
#define I_673mA 0xE
#define I_800mA 0xF

/* Shaft bytes */
#define Shaft_Clockwise 0x0
#define Shaft_AntiClockwise 0x1

/* StepMode bytes */
#define StepMode_1_2 0x0
#define StepMode_1_4 0x1
#define StepMode_1_8 0x2
#define StepMode_1_16 0x3

/* Status bytes */
#define Stopped 0x0
#define Acc_Pos_C 0x1
#define Acc_Neg_C 0x2
#define Moving_Vmax_C 0x3
#define Hard_Stopped 0x4
#define Acc_Pos_AC 0x5
#define Acc_Neg_AC 0x6
#define Moving_Vmax_AC 0x7


/* Command bytes */
#define C_GetFullStatus1 0x81
#define C_GetFullStatus2 0xFC
#define C_GetOTPParam 0x82
#define C_GotoSecurePosition 0x84
#define C_HardStop 0x85
#define C_ResetPosition 0x86
#define C_ResetToDefault 0x87
#define C_RunInit 0x88
#define C_SetMotorParam 0x89
#define C_SetOTPParam 0x90
#define C_SetPosition 0x8B
#define C_SoftStop 0x8F

class TMC222 {

public:

 TMC222(byte Adress);
 ~TMC222();
 
 void GetFullStatus1();
 void GetFullStatus2();
 byte GetState() ;
 int GetActualPosition();
 long GetTargetPosition();
 long GetSecurePosition();
 byte GetOTPParam(byte index);
 
 byte SoftMove(long Steps, byte ResetPosNeeded);
 void SoftStop();
 void HardStop();
 byte HardMove(long Steps);
 void GotoSecurePosition();
 void RunInit();

 
 void SetSecurePosition(long SecPos);
 void SetTargetPosition(long Position);
 void ResetPosition();
 
 void SetOTPParam(byte OTPA, byte PBits);
 void SetMotorParam(byte Irun, byte Ihold, byte Vmax, byte Vmin, unsigned int SecPos, byte Shaft,  byte Acc, byte AccShape, byte StepMode);
 
   struct { 
  unsigned char SlaveAddress; 
  unsigned char Address; 
  unsigned char Irun; 
  unsigned char Ihold; 
  unsigned char Vmax; 
  unsigned char Vmin; 
  unsigned char AccShape; 
  unsigned char StepMode; 
  unsigned char Shaft; 
  unsigned char ACC; 
  unsigned char VddReset; 
  unsigned char StepLoss; 
  unsigned char EIDef; 
  unsigned char UV2; 
  unsigned char TSD; 
  unsigned char TW; 
  unsigned char Tinfo; 
  unsigned char Motion; 
  unsigned char ESW;
  unsigned char OVC1;
  unsigned char OVC2;
  unsigned char CPFail;
} 
 FullStatus_1 ;
 
 struct { 
  byte SlaveAddress; 
  byte Address; 
  int ActPos; 
  int TagPos; 
  int SecPos; 
} 
FullStatus_2;

 private:
 
 byte _Irun,_Ihold,_Vmax,_Vmin, _SecPos,_Shaft,_Acc,_AccShape,_StepMode,_State;
 int _Adress;
 void Command(byte Reg);
 byte GetBytePart(byte Source, byte StartPos, byte BitQuantity);
 
 };


 #endif

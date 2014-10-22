#include "TMC222.h"
 
TMC222::TMC222(byte Adress) {

  _Adress=Adress;
  Wire.begin();
  
}

TMC222::~TMC222() {

}

int TMC222::GetActualPosition() {

  GetFullStatus2();

  return FullStatus_2.ActPos ;

}

long TMC222::GetTargetPosition() {

  GetFullStatus2();

  return FullStatus_2.TagPos ;

}

long TMC222::GetSecurePosition() {

  GetFullStatus2();

  return FullStatus_2.SecPos ;

}

void TMC222::SetTargetPosition(long Position) {

  Wire.beginTransmission(_Adress);
  Wire.write(C_SetPosition);
  Wire.write(0xFF);
  Wire.write(0xFF);
  Wire.write((Position>>8) & 0xFF);
  Wire.write(Position & 0xFF);
  Wire.endTransmission();

}

void TMC222::RunInit() {

  Command(C_RunInit);

}

void TMC222::HardStop(){

  Command(C_HardStop);

}

void TMC222::SoftStop(){

  Command(C_SoftStop);

}

byte TMC222::SoftMove(long Steps, byte ResetPosNeeded){

  while(GetState()!=Stopped) {
    if (_State==Hard_Stopped) return 0;
    }
  
  if(ResetPosNeeded) {
    ResetPosition();
    SetTargetPosition(Steps);
    ResetPosition();
  } else {
    SetTargetPosition(GetActualPosition()+Steps);
  }
  
  return 1;

}

byte TMC222::HardMove(long Steps) {
 
  HardStop();
  GetFullStatus1();
  ResetPosition();
  SetTargetPosition(Steps);
   //ResetPosition();
  
  
}


byte TMC222::GetState() {
  GetFullStatus1();
  _State=FullStatus_1.Motion;
  return _State;
}

void TMC222::ResetPosition(){

  Command(C_ResetPosition);

}

void TMC222::GotoSecurePosition(){

  Command(C_GotoSecurePosition);

}

void TMC222::SetSecurePosition(long SecPos){
  
  _SecPos=SecPos;
  
  SetMotorParam(_Irun,_Ihold,_Vmax,_Vmin, SecPos,_Shaft,_Acc,_AccShape,_StepMode);

}



void TMC222::SetMotorParam(byte Irun, byte Ihold, byte Vmax, byte Vmin, unsigned int SecPos, byte Shaft,  byte Acc, byte AccShape, byte StepMode) {

  _Irun =Irun;
  _Ihold=Ihold;
  _Vmax=Vmax;
  _Vmin=Vmin;
  _SecPos=SecPos;
  _Shaft=Shaft;
  _Acc=Acc;
  _AccShape=AccShape;
  _StepMode=StepMode;
  
  Wire.beginTransmission(_Adress);
  Wire.write(C_SetMotorParam);
  Wire.write(0xFF);
  Wire.write(0xFF);
  Wire.write(((Irun & 0xF)<<4) | (Ihold & 0xF));
  Wire.write(((Vmax & 0xF)<<4) | (Vmin & 0xF));
  Wire.write(((SecPos&0xE0)<<5) | ((Shaft&1)<<4) | (Acc & 0xF));
  Wire.write(SecPos & 0xFF);
  Wire.write(((AccShape & 1) <<4) | ((StepMode & 0x3)<<2));
  Wire.endTransmission();

}

void TMC222::SetOTPParam(byte OTPA, byte PBits) {

  Wire.beginTransmission(_Adress);
  Wire.write(C_SetOTPParam);
  Wire.write(0xFF);
  Wire.write(0xFF);
  Wire.write( 0xF8 | (OTPA & 0x7));
  Wire.write( PBits & 0xFF );
  Wire.endTransmission();

}

byte TMC222::GetOTPParam(byte index){
 
 byte Buffer[8];
 
  Command(C_GetOTPParam);
  
 Wire.requestFrom(_Adress, 8);  
 
 Wire.read();
 
 int i=0;
 
while(Wire.available() && i<8) Buffer[i++]=Wire.read();
 
while(Wire.available()) Wire.read(); // Flush   

return Buffer[index];
 
 
 }

void TMC222::GetFullStatus1() {

  byte Buffer;

  Command(C_GetFullStatus1);
  
  Wire.requestFrom(_Adress, 6);    

  FullStatus_1.Address = Wire.read();
 
  Buffer=Wire.read();
  
  FullStatus_1.Irun = GetBytePart(Buffer,4,4);
  FullStatus_1.Ihold = GetBytePart(Buffer,0,4);
  Buffer=Wire.read();
  
  FullStatus_1.Vmax = GetBytePart(Buffer,4,4);
  FullStatus_1.Vmin = GetBytePart(Buffer,0,4);
  Buffer=Wire.read();
  
  FullStatus_1.AccShape = GetBytePart(Buffer,7,1);
  FullStatus_1.StepMode = GetBytePart(Buffer,5,2);
  FullStatus_1.Shaft = GetBytePart(Buffer,4,1);
  FullStatus_1.ACC = GetBytePart(Buffer,0,4);
  Buffer=Wire.read();
  
  FullStatus_1.VddReset = GetBytePart(Buffer,7,1);
  FullStatus_1.StepLoss = GetBytePart(Buffer,6,1);
  FullStatus_1.EIDef= GetBytePart(Buffer,5,1);
  FullStatus_1.UV2 = GetBytePart(Buffer,4,1);
  FullStatus_1.TSD = GetBytePart(Buffer,3,1);
  FullStatus_1.TW = GetBytePart(Buffer,2,1);
  FullStatus_1.Tinfo = GetBytePart(Buffer,0,2);
  Buffer=Wire.read();
  
  FullStatus_1.Motion = GetBytePart(Buffer,5,3);
  FullStatus_1.ESW = GetBytePart(Buffer,4,1);
  FullStatus_1.OVC1= GetBytePart(Buffer,3,1);
  FullStatus_1.OVC2 = GetBytePart(Buffer,2,1);
  FullStatus_1.CPFail = GetBytePart(Buffer,0,1);
  
  while(Wire.available()) Wire.read(); // Flush    
}

void TMC222::GetFullStatus2() {
  
  Command(C_GetFullStatus2);
 
  Wire.requestFrom(_Adress, 6);    

  FullStatus_2.Address = Wire.read();
  
  FullStatus_2.ActPos = Wire.read()<<8;
  FullStatus_2.ActPos |= Wire.read();
  FullStatus_2.TagPos = Wire.read()<<8;
  FullStatus_2.TagPos |= Wire.read();
  FullStatus_2.SecPos = Wire.read();
  FullStatus_2.SecPos |= (Wire.read()&0x07)<<8;
  
  while(Wire.available()) Wire.read(); // Flush    
  
}

void TMC222::Command(byte Reg) {

  Wire.beginTransmission(_Adress);
  Wire.write(Reg);
  Wire.endTransmission();

}

byte TMC222::GetBytePart(byte Source, byte StartPos, byte BitQuantity) {
  
  byte Extracted=0;
  
  Source = ((Source & 0xFF) >> StartPos); 
  
  for(byte i=0;i<BitQuantity;i++) {
    Extracted |= Source & (1<<i);
  }

return Extracted;
  
  
}

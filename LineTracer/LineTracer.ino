/* Copyright (c) 2018 Sienori All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
int motor_L = 5;
int motor_R = 6;
void setup() {
  Serial.begin(9600);
}

int isBlack(int sensorA){
  return sensorA > 35?1:0;
}

int readSensor(int sensorA0,int sensorA1, int sensorA2, int sensorA3){
 int result = 0; 
 result += isBlack(sensorA0)*1000;
 result += isBlack(sensorA1)*100;
 result += isBlack(sensorA2)*10;
 result += isBlack(sensorA3)*1;
 return result;
}

double CurveAve=0;
double calcCurveAve(int sensorValue){
  int curveValue=0;
  switch(sensorValue){      
    case 1:
      curveValue=4;
      break;
    case 11:
      curveValue=3;
      break;
    case 111:
      curveValue=2;
      break;
    case 10:
      curveValue=1;
      break;
    case 110:
      curveValue=0;
      break;
    case 100:
      curveValue=-1;
      break;
    case 1110:
      curveValue=-2;
      break;
    case 1100:
      curveValue=-3;
      break;
    case 1000:
      curveValue=-4;
      break;
    case 1111:
      curveValue=0;
      break;
    case 0:
      curveValue=CurveAve>0? 4:-4;
      break;
    default:
      curveValue=0;
      break;
  }
  
  if(curveValue==0) return CurveAve;
  
  CurveAve=((0.007*curveValue)+(0.993*CurveAve));
  return CurveAve;
}

void runMotor(double curveAve){
  int speedMax=90;
  int speedMin=12;
    
  double absoluteAve=sqrt(curveAve*curveAve);
  
  double mainSpeed=(speedMin-speedMax)/4*absoluteAve+speedMax;
  double subSpeed=speedMax;

  boolean isRight=curveAve>0;
  int speedR=isRight? mainSpeed:subSpeed;
  int speedL=isRight? subSpeed:mainSpeed;

  analogWrite(motor_L, speedL);
  analogWrite(motor_R, speedR);
}

void loop() {
  int sensorA0=analogRead(A0);
  int sensorA1=analogRead(A1);
  int sensorA2=analogRead(A2);
  int sensorA3=analogRead(A3);
  int sensorValue = readSensor(sensorA0, sensorA1, sensorA2, sensorA3);
  double curveAve = calcCurveAve(sensorValue);
  runMotor(curveAve);
}

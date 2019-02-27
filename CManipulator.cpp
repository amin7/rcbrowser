/*
 * CManipulator.cpp
 *
 *  Created on: Oct 24, 2018
 *      Author: ominenko
 */

#include "CManipulator.h"
#include <iostream>
#include <math.h>
using namespace std;
CManipulator::CManipulator(uint8_t pin_base, uint8_t pin_shoulder, uint8_t pin_elbow) :
    servo_base(pin_base, 0, 180), servo_shoulder(pin_shoulder, 0, 180), servo_elbow(pin_elbow, 0, 180) {

}

int CManipulator::InverseKinematicsTransform(double R, double H, double &Shoulder, double &Elbow) {
  double sqr_d;
  double d;
  double Shoulder_Rad_1;
  double Shoulder_Rad_2;

  sqr_d = R * R + H * H;
  d = sqrt(sqr_d);
  //Serial.println (sqr_d);
  if (d > BackArm_mm + ForeArm_mm || d > d_Max) // trivially out of range
    return 1;
  if (d <= 0 || d < d_Min) // not possible for structure of arm
    return 2;

  R = R - R_offset_mm;
  Elbow = acos((BackArm_mm * BackArm_mm + ForeArm_mm * ForeArm_mm - sqr_d) / (2 * BackArm_mm * ForeArm_mm));

  Shoulder_Rad_1 = asin((ForeArm_mm * sin(Elbow)) / d);
  Shoulder_Rad_2 = atan(H / R);

  //Serial.println (Shoulder_Rad_1);
  //Serial.println (Shoulder_Rad_2);

  Shoulder = (Shoulder_Rad_1 + Shoulder_Rad_2) * 180 / M_PI;
  Elbow = Elbow * 180 / M_PI;

  return 0;
}

////Function to transform XYZ(Cartesian ) to RHA (Polar)
//int C2P (double x, double y, double z, double &R, double &H, double &A){
//  H=z;
//  if (y<0) {
//    return 1;
//  }
//  R=sqrt(x*x+y*y);
//  if (x==0) {
//    A=90;
//  }else if (x>0){
//    A=atan(y/x)*180/M_PI;
//  }else {
//    // the case of x<0
//    A=180+atan(y/x)*180/M_PI;
//  }
//  return 0;
//}
void CManipulator::init() {
  servo_base.init();
  servo_shoulder.init();
  servo_elbow.init();
}
void CManipulator::set_bse(int16_t base, int16_t shoulder, int16_t elbow) {
  cout << "CManipulator=" << base << " " << shoulder << " " << elbow << endl;
  servo_base.setVal(base);
  servo_shoulder.setVal(shoulder);
  servo_elbow.setVal(elbow);
}

void CManipulator::set_xyz(int16_t _x, int16_t _y, int16_t _z) {
  double Shoulder;
  double Elbow;
  if (0 == InverseKinematicsTransform(_x, _y, Shoulder, Elbow)) {
    set_bse(_z, 180 - static_cast<int16_t>(Shoulder), static_cast<int16_t>(Elbow) - static_cast<int16_t>(Shoulder));
  }
}

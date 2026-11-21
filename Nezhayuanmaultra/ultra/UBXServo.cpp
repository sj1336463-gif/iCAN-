#include <Arduino.h>
#include "UBXServo.h"


/**********************
 * 
 * 指定时间内运动到指定位置，完成时间是指转到指定位置后，如果完成时间比运动时间大，那么舵机还会等待一段时间,这个时间建议全为0，可以用延时来调节
 * 
 * 初始化转到舵机中间位置0x79
 * 位置0xFE逆时针不停的转
 * 0xFD顺时针 
 * 0xFF不转  
 * 0x79是中间位置
 * 
 * 
 ***********************/
UBXServo::UBXServo(uchar ID)
{
  _id=ID;
}
void UBXServo::ReadAngle()
{  
  uchar _checknumber=_id+READ_Angle;
  Serial2.write(HEAD_one);
  Serial2.write(HEAD_two);
  Serial2.write(_id);
  Serial2.write(READ_Angle);
  Serial2.write(0x00);
  Serial2.write(0x00);
  Serial2.write(0x00);
  Serial2.write(0x00);
  Serial2.write(_checknumber);
  Serial2.write(END);
}

void UBXServo::LED_ONorOFF(int Button)
{  
  uchar _checknumber=_id+LED+Button;
  Serial2.write(HEAD_one);
  Serial2.write(HEAD_two);
  Serial2.write(_id);
  Serial2.write(LED);
  Serial2.write(Button);
  Serial2.write(0x00);
  Serial2.write(0x00);
  Serial2.write(0x00);
  Serial2.write(_checknumber);
  Serial2.write(END);
}

void UBXServo::ChangeID(uchar New_Id)
{  
  uchar _checknumber=_id+CHANGE_ID+New_Id;
  Serial2.write(HEAD_one);
  Serial2.write(HEAD_two);
  Serial2.write(_id);
  Serial2.write(CHANGE_ID);
  Serial2.write(0x00);
  Serial2.write(New_Id);
  Serial2.write(0x00);
  Serial2.write(0x00);
  Serial2.write(_checknumber);
  Serial2.write(END);
}
//到达的角度，时间，整个运动过程时间
void UBXServo::Do(int Terminal,int Time,int Achtime8l,int Achtime8H)
{  
  _terminal=Terminal;
  _time=Time;
  _achtime8l=Achtime8l;
  _achtime8H=Achtime8H;
  uchar _checknumber=DO+_id+_terminal+_time+_achtime8l+_achtime8H;
  Serial2.write(HEAD_one);
  Serial2.write(HEAD_two);
  Serial2.write(_id);
  Serial2.write(DO);
  Serial2.write(_terminal);
  Serial2.write(_time);
  Serial2.write(_achtime8l);
  Serial2.write(_achtime8H);
  Serial2.write(_checknumber);
  Serial2.write(END);
}
//这个就转到中间位置 0x7E
void UBXServo::init()
{  
  uchar _checknumber=DO+_id+0x79+0x20;
  Serial2.write(HEAD_one);
  Serial2.write(HEAD_two);
  Serial2.write(_id);
  Serial2.write(DO);
  Serial2.write(0x79);
  Serial2.write(0x20);
  Serial2.write(0x00);
  Serial2.write(0x00);
  Serial2.write(_checknumber);
  Serial2.write(END);
}

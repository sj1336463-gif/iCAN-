/************************************
 *指令格式为 HEAD_one HEAD_one 
 *总共转的角度为240°
 *
 *************************************/
#ifndef _UBXSERVO_H_
#define _UBXSERVO_H_

#define uchar unsigned char 
#define uint unsigned int
#define HEAD_one 0xFA
#define HEAD_two 0xAF
#define DO   0x01
#define LED  0x04
#define CHANGE_ID 0xCD
#define READ_Angle 0x02
#define END  0xED
#define MID  0X79
#define STOP 0xFF
#define SHUN 0xFD
#define NI 0xFE
#define LEDON 0X00
#define LEDOFF 0x01

class UBXServo
{
  public:
  UBXServo(uchar ID);//初始化一个舵机
  uchar _id;
  uint _terminal;
  uint _time;
  uint _achtime8l;//这个参数建议为0
  uint _achtime8H;//建议为0
  //初始化
  void init();//成功
  void Do(int Terminal,int Time,int Achtime8l,int Achtime8H);//后两个参数直接添0
  void ChangeID(uchar New_Id);
  void ReadAngle();//暂时没对回读的程序进行处理
  void LED_ONorOFF(int Button);
};
#endif

#include "UBXServo.h"
#include <Servo.h>
// #include <Arduino.h> 

// Servo myServo;
// const int servoPin = 7;
// int currentAngle = 90;

// 定义舵机 ID
UBXServo servo(0x00);
UBXServo servoy1(0x01);
UBXServo servoy2(0x02);
UBXServo servoy3(0x03);
UBXServo servoy4(0x04);
UBXServo servoy5(0x05);
UBXServo servoy6(0x06);
UBXServo servoy7(0x07);
UBXServo servoy8(0x08);
UBXServo servoy9(0x09);
UBXServo servoy10(0x0a);
UBXServo servoy11(0x0b);
UBXServo servoy12(0x0c);
UBXServo servoy13(0x0d);
UBXServo servoy14(0x0e);
UBXServo servoy15(0x0f);
UBXServo servoy16(0x10);

	// 新增：声明executeDance变量
volatile bool executeDance = false;  // 执行跳舞标志，必须加 volatile！
		// === 无线遥控引脚定义 (改用 D2 支持外部中断) ===
		const int remotePin = 2;      // 信号线接 D2 引脚
		// ==============================

// 数据帧解析变量
#define FRAME_LENGTH 8
byte frameBuffer[FRAME_LENGTH];
int bufferIndex = 0;
bool isDancing = false;  // 是否正在跳舞
unsigned long lastDanceStart = 0;  // 上次开始跳舞时间
#define DANCE_COOLDOWN 1000  // 跳舞冷却时间（毫秒）

void setup() 
{
  Serial.begin(115200);    // 串口监视器
  Serial2.begin(115200);   // 舵机通信
  Serial3.begin(115200);   // 接收控制指令

	delay(1000); // 等待初始化
		  // === 配置遥控引脚并开启外部中断 ===
		// 在 setup() 中
	pinMode(remotePin, INPUT_PULLUP); // 开启内部上拉，平时 D2 被拉高为高电平
	// 检测下降沿：按下遥控器时，模块的 OUT- 内部接地，D2 电平从高变低，瞬间触发中断
	attachInterrupt(digitalPinToInterrupt(remotePin), triggerDance, FALLING);  
		  // ==================================
	  // Serial.begin(9600);

  // Serial.begin(9600);
  // myServo.attach(servoPin);
  // setServoAngle(90);

}

void loop() 
{

//servoy5.ChangeID(0x07);

servoy1.Do(110, 50, 0, 0);delay(2);//加度数向后摆
 servoy2.Do(135, 50, 0, 0);delay(2);//加度数向上
 servoy3.Do(135, 50, 0, 0);delay(2);//加度数向上

 servoy4.Do(115, 50, 0, 0);delay(2);//加度数向前挥
 servoy5.Do(135, 50, 0, 0);delay(2);//加度数向下
 servoy6.Do(135, 50, 0, 0);delay(2);//加度数向下

 servoy7.Do(110, 50, 0, 0);delay(2);//减度数向外
 servoy8.Do(90, 50, 0, 0);delay(2);//加度数向后
 servoy9.Do(75, 50, 0, 0);delay(2);//加度数向前
 servoy10.Do(130, 50, 0, 0);delay(2);//加度数向前
 servoy11.Do(90, 50, 0, 0);delay(2);//加度数脚内扣

 servoy12.Do(110, 50, 0, 0);delay(2);//加度数向外
 servoy13.Do(120, 50, 0, 0);delay(2);//加度数向前
servoy14.Do(122, 50, 0, 0);delay(2);//加度数向后
 servoy15.Do(115, 50, 0, 0);delay(2);//加度数向后
servoy16.Do(90, 50, 0, 0);delay(2);//加度数脚外翻
  
		 delay(2000); // 恢复原版单纯的2秒等待
		 // 检查是否需要开始新的跳舞程序
		 if (executeDance && !isDancing && (millis() - lastDanceStart > DANCE_COOLDOWN)) 
		 {
		   executeDance = false;  // 重置标志
		   isDancing = true;      // 设置跳舞状态
		   lastDanceStart = millis();
		   Serial.println("开始执行跳舞程序！");
		   executeDanceProgram();
		   isDancing = false;     // 跳舞结束
		   Serial.println("跳舞程序执行完毕");
		 }
		}

// 其余代码保持不变...
// 串口3数据解析函数
void handleSerial3Data() 
{
  while (Serial3.available()) 
  {
    byte b = Serial3.read();
    
    // 填充数据帧缓冲区
    frameBuffer[bufferIndex++] = b;
    
    // 当缓冲区填满时校验数据帧
    if (bufferIndex >= FRAME_LENGTH) 
    {
      bufferIndex = 0; // 重置缓冲区指针
      
      // 校验是否为目标数据帧
      if (isTargetFrame(frameBuffer)) 
      {
        Serial.println("收到有效指令，准备跳舞");
        executeDance = true;  // 设置跳舞标志
      } 
      else 
      {
        Serial.print("无效帧: ");
        printFrameHex(frameBuffer);
      }
    }
  }
}

// 校验目标数据帧函数
bool isTargetFrame(byte* frame) 
{
  // 校验帧头、帧尾和关键字节
  return 
  (
    frame[0] == 0xA5 &&
    frame[1] == 0xFA &&
    frame[2] == 0x00 &&
    frame[3] == 0x81 &&
    (frame[4] == 0x02 || frame[4] == 0x06) &&
    frame[5] == 0x00 &&
    frame[6] == 0x22 &&
    frame[7] == 0xFB
  );
}

// 辅助函数：打印十六进制帧
void printFrameHex(byte* frame) 
{
  for (int i = 0; i < FRAME_LENGTH; i++) 
  {
    Serial.print("0x");
    Serial.print(frame[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

// 执行完整跳舞程序
void executeDanceProgram() 
{
//setServoAngle(120);
//hengcha();
//bolang();
//right();
//left();

//prepare();
//chushi();


//two();
///chushi();
//three();
//chushi();
//four();
//chushi();
//five();

//q();
//chushi();
//w();
//chushi();
//q();
//chushi();
//w();
//chushi();
//q();
//chushi();
//w();
//chushi();
//sb();
//houbai();
//fw();
abc();


}

// 原跳舞函数保持不变（jushuangshou、hengcha等函数定义保持原样）
// 原跳舞函数保持不变（jushuangshou、hengcha等函数定义保持原样）
// 原跳舞函数保持不变（jushuangshou、hengcha等函数定义保持原样）
void sb()
{servoy2.Do(180, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(225, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(190, 50, 0, 0);delay(2);//加度数向前挥
servoy5.Do(225, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(210, 50, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外    1
servoy12.Do(105, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(145, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(100, 50, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(45, 50, 0, 0);delay(2);//加度数向后摆
servoy2.Do(45, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(45, 20, 0, 0);delay(2);//加度数向上
servoy4.Do(115, 50, 0, 0);delay(2);//加度数向前挥
servoy5.Do(90, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(60, 50, 0, 0);delay(2);//加度数向下
servoy7.Do(110, 50, 0, 0);delay(2);//减度数向外 y
servoy12.Do(115, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(95, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 50, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy2.Do(180, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(225, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(190, 50, 0, 0);delay(2);//加度数向前挥
servoy5.Do(225, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(210, 50, 0, 0);delay(2);//加度数向下
servoy7.Do(120, 50, 0, 0);delay(2);//减度数向外    1
servoy12.Do(135, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(85, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(40, 50, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(45, 50, 0, 0);delay(2);//加度数向后摆
servoy2.Do(45, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(45, 20, 0, 0);delay(2);//加度数向上
servoy4.Do(115, 50, 0, 0);delay(2);//加度数向前挥
servoy5.Do(90, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(60, 50, 0, 0);delay(2);//加度数向下
servoy7.Do(110, 50, 0, 0);delay(2);//减度数向外 y
servoy12.Do(115, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(95, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 50, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(110, 50, 0, 0);delay(2);//加度数向后摆
servoy2.Do(180, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(225, 20, 0, 0);delay(2);//加度数向上
servoy4.Do(190, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(225, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(210, 50, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外    1
servoy12.Do(105, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(145, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(100, 50, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy2.Do(115, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(235, 20, 0, 0);delay(2);//加度数向上
servoy4.Do(220, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(225, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(210, 50, 0, 0);delay(2);//加度数向下
servoy7.Do(110, 50, 0, 0);delay(2);//减度数向外 y
servoy12.Do(115, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(95, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 50, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy2.Do(180, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(225, 20, 0, 0);delay(2);//加度数向上
servoy4.Do(190, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(225, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(210, 50, 0, 0);delay(2);//加度数向下
servoy7.Do(120, 50, 0, 0);delay(2);//减度数向外  2
servoy12.Do(135, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(85, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(40, 50, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy2.Do(115, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(235, 20, 0, 0);delay(2);//加度数向上
servoy4.Do(220, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(225, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(210, 50, 0, 0);delay(2);//加度数向下
servoy7.Do(110, 50, 0, 0);delay(2);//减度数向外 y
servoy12.Do(115, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(95, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 50, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy2.Do(180, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(225, 20, 0, 0);delay(2);//加度数向上
servoy4.Do(190, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(225, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(210, 50, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外    1
servoy12.Do(105, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(145, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(100, 50, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy2.Do(115, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(235, 20, 0, 0);delay(2);//加度数向上
servoy4.Do(220, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(225, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(210, 50, 0, 0);delay(2);//加度数向下
servoy7.Do(110, 50, 0, 0);delay(2);//减度数向外 y
servoy12.Do(115, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(95, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 50, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy2.Do(115, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(235, 20, 0, 0);delay(2);//加度数向上
servoy4.Do(220, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(225, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(210, 50, 0, 0);delay(2);//加度数向下
servoy7.Do(110, 50, 0, 0);delay(2);//减度数向外 y
servoy12.Do(115, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(95, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 50, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy2.Do(115, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(115, 50, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 50, 0, 0);delay(2);//加度数向下

delay(1000);
servoy2.Do(55, 20, 0, 0);delay(2);//加度数向上
servoy5.Do(215, 20, 0, 0);delay(2);//加度数向下
delay(1000);
servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy4.Do(205, 20, 0, 0);delay(2);//加度数向前挥
servoy2.Do(55, 20, 0, 0);delay(2);//加度数向上
servoy5.Do(215, 20, 0, 0);delay(2);//加度数向下
servoy3.Do(75, 20, 0, 0);delay(2);//加度数向上
servoy6.Do(195, 20, 0, 0);delay(2);//加度数向下
servoy11.Do(105, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(100, 50, 0, 0);delay(2);//加度数脚外翻
servoy8.Do(65, 50, 0, 0);delay(2);//加度数向后   前25
servoy9.Do(65, 50, 0, 0);delay(2);//加度数向前   后10
servoy10.Do(110, 50, 0, 0);delay(2);//加度数向前  后20

servoy13.Do(80, 50, 0, 0);delay(2);//加度数向前  后40
servoy15.Do(85, 50, 0, 0);delay(2);//加度数向后  前30
delay(1000);

servoy1.Do(40, 20, 0, 0);delay(2);//加度数向后摆
servoy4.Do(185, 20, 0, 0);delay(2);//加度数向前挥
servoy2.Do(55, 50, 0, 0);delay(2);//加度数向上
servoy5.Do(215, 50, 0, 0);delay(2);//加度数向下
servoy3.Do(75, 50, 0, 0);delay(2);//加度数向上
servoy6.Do(195, 50, 0, 0);delay(2);//加度数向下
servoy8.Do(130, 50, 0, 0);delay(2);//加度数向后   前25
servoy9.Do(75, 50, 0, 0);delay(2);//加度数向前   后10
servoy10.Do(160, 50, 0, 0);delay(2);//加度数向前  后20
servoy11.Do(100, 50, 0, 0);delay(2);//加度数脚内扣
servoy13.Do(135, 50, 0, 0);delay(2);//加度数向前  后40
servoy14.Do(132, 50, 0, 0);delay(2);//加度数向后
servoy15.Do(135, 50, 0, 0);delay(2);//加度数向后
servoy16.Do(85, 50, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy4.Do(205, 20, 0, 0);delay(2);//加度数向前挥
servoy2.Do(55, 50, 0, 0);delay(2);//加度数向上
servoy5.Do(215, 50, 0, 0);delay(2);//加度数向下
servoy3.Do(75, 50, 0, 0);delay(2);//加度数向上
servoy6.Do(195, 50, 0, 0);delay(2);//加度数向下
servoy16.Do(90, 50, 0, 0);delay(2);//加度数脚外翻
servoy11.Do(105, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(100, 50, 0, 0);delay(2);//加度数脚外翻
servoy8.Do(65, 50, 0, 0);delay(2);//加度数向后   前25
servoy9.Do(65, 50, 0, 0);delay(2);//加度数向前   后10
servoy10.Do(110, 50, 0, 0);delay(2);//加度数向前  后20
servoy14.Do(122, 50, 0, 0);delay(2);//加度数向后
servoy13.Do(80, 50, 0, 0);delay(2);//加度数向前  后40
servoy15.Do(85, 50, 0, 0);delay(2);//加度数向后  前30

delay(1000);
servoy1.Do(40, 20, 0, 0);delay(2);//加度数向后摆
servoy4.Do(185, 20, 0, 0);delay(2);//加度数向前挥
servoy2.Do(55, 50, 0, 0);delay(2);//加度数向上
servoy5.Do(215, 50, 0, 0);delay(2);//加度数向下
servoy3.Do(75, 50, 0, 0);delay(2);//加度数向上
servoy6.Do(195, 50, 0, 0);delay(2);//加度数向下
servoy8.Do(130, 50, 0, 0);delay(2);//加度数向后   前25
servoy9.Do(75, 50, 0, 0);delay(2);//加度数向前   后10
servoy10.Do(160, 50, 0, 0);delay(2);//加度数向前  后20
servoy13.Do(135, 50, 0, 0);delay(2);//加度数向前  后40
servoy14.Do(132, 50, 0, 0);delay(2);//加度数向后
servoy15.Do(135, 50, 0, 0);delay(2);//加度数向后
servoy16.Do(85, 50, 0, 0);delay(2);//加度数脚外翻
servoy11.Do(100, 50, 0, 0);delay(2);//加度数脚内扣
delay(1000);
servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy4.Do(205, 20, 0, 0);delay(2);//加度数向前挥
servoy2.Do(55, 50, 0, 0);delay(2);//加度数向上
servoy5.Do(215, 50, 0, 0);delay(2);//加度数向下
servoy3.Do(75, 50, 0, 0);delay(2);//加度数向上
servoy6.Do(195, 50, 0, 0);delay(2);//加度数向下
servoy8.Do(90, 50, 0, 0);delay(2);//加度数向后   前25
servoy9.Do(75, 50, 0, 0);delay(2);//加度数向前   后10
servoy10.Do(130, 50, 0, 0);delay(2);//加度数向前  后20
servoy13.Do(120, 50, 0, 0);delay(2);//加度数向前  后40
servoy14.Do(122, 50, 0, 0);delay(2);//加度数向后
servoy15.Do(115, 50, 0, 0);delay(2);//加度数向后
servoy16.Do(90, 50, 0, 0);delay(2);//加度数脚外翻
servoy11.Do(100, 50, 0, 0);delay(2);//加度数脚内扣
delay(1000);
servoy1.Do(110, 50, 0, 0);delay(2);//加度数向后摆
servoy4.Do(25, 50, 0, 0);delay(2);//加度数向前挥
servoy2.Do(135, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 50, 0, 0);delay(2);//加度数向上
servoy6.Do(135, 50, 0, 0);delay(2);//加度数向下
servoy5.Do(135, 50, 0, 0);delay(2);//加度数向下
delay(1000);

servoy5.Do(45, 50, 0, 0);delay(2);//加度数向下 
servoy6.Do(75, 50, 0, 0);delay(2);//加度数向下
delay(1000);

servoy5.Do(45, 50, 0, 0);delay(2);//加度数向下 
servoy6.Do(75, 50, 0, 0);delay(2);//加度数向下
servoy4.Do(25, 50, 0, 0);delay(2);//加度数向前挥
servoy1.Do(200, 50, 0, 0);delay(2);//加度数向后
servoy2.Do(45, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 50, 0, 0);delay(2);//加度数向上
delay(1000);
servoy5.Do(45, 50, 0, 0);delay(2);//加度数向下 
servoy6.Do(75, 50, 0, 0);delay(2);//加度数向下
servoy4.Do(25, 50, 0, 0);delay(2);//加度数向前挥
servoy1.Do(200, 50, 0, 0);delay(2);//加度数向后
servoy2.Do(225, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(225, 50, 0, 0);delay(2);//加度数向上
delay(1000);
servoy5.Do(45, 50, 0, 0);delay(2);//加度数向下 
servoy6.Do(75, 50, 0, 0);delay(2);//加度数向下
servoy4.Do(25, 50, 0, 0);delay(2);//加度数向前挥
servoy1.Do(220, 50, 0, 0);delay(2);//加度数向后
servoy2.Do(205, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(225, 50, 0, 0);delay(2);//加度数向上
delay(1000);
servoy5.Do(45, 50, 0, 0);delay(2);//加度数向下 
servoy6.Do(75, 50, 0, 0);delay(2);//加度数向下
servoy4.Do(25, 50, 0, 0);delay(2);//加度数向前挥
servoy1.Do(200, 50, 0, 0);delay(2);//加度数向后
servoy2.Do(225, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(225, 50, 0, 0);delay(2);//加度数向上
delay(1000);
servoy5.Do(225, 50, 0, 0);delay(2);//加度数向下 
servoy6.Do(135, 50, 0, 0);delay(2);//加度数向下
servoy4.Do(115, 50, 0, 0);delay(2);//加度数向前挥
servoy1.Do(100, 50, 0, 0);delay(2);//加度数向后
servoy2.Do(45, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 50, 0, 0);delay(2);//加度数向上
delay(1000);

servoy1.Do(110, 50, 0, 0);delay(2);//加度数向后摆
servoy2.Do(55, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(115, 50, 0, 0);delay(2);//加度数向前挥
servoy5.Do(215, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 50, 0, 0);delay(2);//加度数向下
delay(1000); 
servoy1.Do(110, 50, 0, 0);delay(2);//加度数向后摆
servoy2.Do(65, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(115, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(115, 50, 0, 0);delay(2);//加度数向前挥
servoy5.Do(205, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(155, 50, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 50, 0, 0);delay(2);//加度数向外
delay(1000);
servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(105, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(115, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(145, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(185, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(145, 20, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy11.Do(120, 50, 0, 0);delay(2);//加度数脚内扣
servoy12.Do(140, 50, 0, 0);delay(2);//加度数向外
delay(1000);

servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(55, 10, 0, 0);delay(2);//加度数向上
servoy3.Do(115, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(145, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(185, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(145, 20, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy11.Do(120, 50, 0, 0);delay(2);//加度数脚内扣
servoy12.Do(140, 50, 0, 0);delay(2);//加度数向外
delay(1000);

servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(75, 10, 0, 0);delay(2);//加度数向上
servoy3.Do(115, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(145, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(185, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(145, 20, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy11.Do(120, 50, 0, 0);delay(2);//加度数脚内扣
servoy12.Do(140, 50, 0, 0);delay(2);//加度数向外
delay(1000);

servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(55, 10, 0, 0);delay(2);//加度数向上
servoy3.Do(115, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(145, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(185, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(145, 20, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy11.Do(120, 50, 0, 0);delay(2);//加度数脚内扣
servoy12.Do(140, 50, 0, 0);delay(2);//加度数向外
delay(1000);

servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(75, 10, 0, 0);delay(2);//加度数向上
servoy3.Do(115, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(145, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(185, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(145, 20, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy11.Do(120, 50, 0, 0);delay(2);//加度数脚内扣
servoy12.Do(140, 50, 0, 0);delay(2);//加度数向外
delay(1000);
servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(55, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(115, 20, 0, 0);delay(2);//加度数向上
servoy4.Do(205, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(215, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(155, 20, 0, 0);delay(2);//加度数向下
servoy7.Do(110, 50, 0, 0);delay(2);//减度数向外////////////////////
servoy11.Do(105, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(100, 50, 0, 0);delay(2);//加度数脚外翻
servoy8.Do(65, 50, 0, 0);delay(2);//加度数向后   前25
servoy9.Do(65, 50, 0, 0);delay(2);//加度数向前   后10
servoy10.Do(110, 50, 0, 0);delay(2);//加度数向前  后20

servoy13.Do(80, 50, 0, 0);delay(2);//加度数向前  后40
servoy15.Do(85, 50, 0, 0);delay(2);//加度数向后  前30
delay(1000);

servoy1.Do(40, 20, 0, 0);delay(2);//加度数向后摆
servoy4.Do(185, 20, 0, 0);delay(2);//加度数向前挥
servoy2.Do(55, 50, 0, 0);delay(2);//加度数向上
servoy5.Do(215, 50, 0, 0);delay(2);//加度数向下
servoy3.Do(75, 50, 0, 0);delay(2);//加度数向上
servoy6.Do(195, 50, 0, 0);delay(2);//加度数向下
servoy8.Do(130, 50, 0, 0);delay(2);//加度数向后   前25
servoy9.Do(75, 50, 0, 0);delay(2);//加度数向前   后10
servoy10.Do(160, 50, 0, 0);delay(2);//加度数向前  后20
servoy11.Do(100, 50, 0, 0);delay(2);//加度数脚内扣
servoy13.Do(135, 50, 0, 0);delay(2);//加度数向前  后40
servoy14.Do(132, 50, 0, 0);delay(2);//加度数向后
servoy15.Do(135, 50, 0, 0);delay(2);//加度数向后
servoy16.Do(85, 50, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy4.Do(205, 20, 0, 0);delay(2);//加度数向前挥
servoy2.Do(55, 50, 0, 0);delay(2);//加度数向上
servoy5.Do(215, 50, 0, 0);delay(2);//加度数向下
servoy3.Do(75, 50, 0, 0);delay(2);//加度数向上
servoy6.Do(195, 50, 0, 0);delay(2);//加度数向下
servoy16.Do(90, 50, 0, 0);delay(2);//加度数脚外翻
servoy11.Do(105, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(100, 50, 0, 0);delay(2);//加度数脚外翻
servoy8.Do(65, 50, 0, 0);delay(2);//加度数向后   前25
servoy9.Do(65, 50, 0, 0);delay(2);//加度数向前   后10
servoy10.Do(110, 50, 0, 0);delay(2);//加度数向前  后20
servoy14.Do(122, 50, 0, 0);delay(2);//加度数向后
servoy13.Do(80, 50, 0, 0);delay(2);//加度数向前  后40
servoy15.Do(85, 50, 0, 0);delay(2);//加度数向后  前30

delay(1000);
servoy1.Do(40, 20, 0, 0);delay(2);//加度数向后摆
servoy4.Do(185, 20, 0, 0);delay(2);//加度数向前挥
servoy2.Do(55, 50, 0, 0);delay(2);//加度数向上
servoy5.Do(215, 50, 0, 0);delay(2);//加度数向下
servoy3.Do(75, 50, 0, 0);delay(2);//加度数向上
servoy6.Do(195, 50, 0, 0);delay(2);//加度数向下
servoy8.Do(130, 50, 0, 0);delay(2);//加度数向后   前25
servoy9.Do(75, 50, 0, 0);delay(2);//加度数向前   后10
servoy10.Do(160, 50, 0, 0);delay(2);//加度数向前  后20
servoy13.Do(135, 50, 0, 0);delay(2);//加度数向前  后40
servoy14.Do(132, 50, 0, 0);delay(2);//加度数向后
servoy15.Do(135, 50, 0, 0);delay(2);//加度数向后
servoy16.Do(85, 50, 0, 0);delay(2);//加度数脚外翻
servoy11.Do(100, 50, 0, 0);delay(2);//加度数脚内扣
delay(1000);
servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy4.Do(205, 20, 0, 0);delay(2);//加度数向前挥
servoy2.Do(55, 50, 0, 0);delay(2);//加度数向上
servoy5.Do(215, 50, 0, 0);delay(2);//加度数向下
servoy3.Do(75, 50, 0, 0);delay(2);//加度数向上
servoy6.Do(195, 50, 0, 0);delay(2);//加度数向下
servoy8.Do(90, 50, 0, 0);delay(2);//加度数向后   前25
servoy9.Do(75, 50, 0, 0);delay(2);//加度数向前   后10
servoy10.Do(130, 50, 0, 0);delay(2);//加度数向前  后20
servoy13.Do(120, 50, 0, 0);delay(2);//加度数向前  后40
servoy14.Do(122, 50, 0, 0);delay(2);//加度数向后
servoy15.Do(115, 50, 0, 0);delay(2);//加度数向后
servoy16.Do(90, 50, 0, 0);delay(2);//加度数脚外翻
servoy11.Do(100, 50, 0, 0);delay(2);//加度数脚内扣
delay(1000);
 servoy1.Do(110, 50, 0, 0);delay(2);//加度数向后摆
servoy4.Do(25, 50, 0, 0);delay(2);//加度数向前挥
servoy2.Do(135, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 50, 0, 0);delay(2);//加度数向上
servoy6.Do(135, 50, 0, 0);delay(2);//加度数向下
servoy5.Do(135, 50, 0, 0);delay(2);//加度数向下
delay(1000);

servoy5.Do(45, 50, 0, 0);delay(2);//加度数向下 
servoy6.Do(75, 50, 0, 0);delay(2);//加度数向下
delay(1000);

 servoy5.Do(45, 50, 0, 0);delay(2);//加度数向下 
servoy6.Do(75, 50, 0, 0);delay(2);//加度数向下
servoy4.Do(25, 50, 0, 0);delay(2);//加度数向前挥
servoy1.Do(200, 50, 0, 0);delay(2);//加度数向后
 servoy2.Do(45, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 50, 0, 0);delay(2);//加度数向上
delay(1000);
servoy5.Do(45, 50, 0, 0);delay(2);//加度数向下 
servoy6.Do(75, 50, 0, 0);delay(2);//加度数向下
servoy4.Do(25, 50, 0, 0);delay(2);//加度数向前挥
servoy1.Do(200, 50, 0, 0);delay(2);//加度数向后
servoy2.Do(225, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(225, 50, 0, 0);delay(2);//加度数向上
delay(1000);
servoy5.Do(45, 50, 0, 0);delay(2);//加度数向下 
servoy6.Do(75, 50, 0, 0);delay(2);//加度数向下
servoy4.Do(25, 50, 0, 0);delay(2);//加度数向前挥
servoy1.Do(220, 50, 0, 0);delay(2);//加度数向后
 servoy2.Do(205, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(225, 50, 0, 0);delay(2);//加度数向上
delay(1000);
servoy5.Do(45, 50, 0, 0);delay(2);//加度数向下 
servoy6.Do(75, 50, 0, 0);delay(2);//加度数向下
servoy4.Do(25, 50, 0, 0);delay(2);//加度数向前挥
servoy1.Do(200, 50, 0, 0);delay(2);//加度数向后
servoy2.Do(225, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(225, 50, 0, 0);delay(2);//加度数向上
delay(1000);
servoy5.Do(225, 50, 0, 0);delay(2);//加度数向下 
servoy6.Do(135, 50, 0, 0);delay(2);//加度数向下
servoy4.Do(115, 50, 0, 0);delay(2);//加度数向前挥
servoy1.Do(100, 50, 0, 0);delay(2);//加度数向后
 servoy2.Do(45, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 50, 0, 0);delay(2);//加度数向上
delay(1000);

servoy1.Do(110, 50, 0, 0);delay(2);//加度数向后摆
servoy2.Do(55, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(115, 50, 0, 0);delay(2);//加度数向前挥
servoy5.Do(215, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 50, 0, 0);delay(2);//加度数向下
delay(1000); 
servoy1.Do(110, 50, 0, 0);delay(2);//加度数向后摆
servoy2.Do(65, 50, 0, 0);delay(2);//加度数向上
servoy3.Do(115, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(115, 50, 0, 0);delay(2);//加度数向前挥
servoy5.Do(205, 50, 0, 0);delay(2);//加度数向下
servoy6.Do(155, 50, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 50, 0, 0);delay(2);//加度数向外
delay(1000);
servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(105, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(115, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(145, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(185, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(145, 20, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy11.Do(120, 50, 0, 0);delay(2);//加度数脚内扣
servoy12.Do(140, 50, 0, 0);delay(2);//加度数向外
delay(1000);

servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(55, 10, 0, 0);delay(2);//加度数向上
servoy3.Do(115, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(145, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(185, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(145, 20, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy11.Do(120, 50, 0, 0);delay(2);//加度数脚内扣
servoy12.Do(140, 50, 0, 0);delay(2);//加度数向外
delay(1000);

servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(75, 10, 0, 0);delay(2);//加度数向上
servoy3.Do(115, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(145, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(185, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(145, 20, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy11.Do(120, 50, 0, 0);delay(2);//加度数脚内扣
servoy12.Do(140, 50, 0, 0);delay(2);//加度数向外
delay(1000);

servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(55, 10, 0, 0);delay(2);//加度数向上
servoy3.Do(115, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(145, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(185, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(145, 20, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy11.Do(120, 50, 0, 0);delay(2);//加度数脚内扣
servoy12.Do(140, 50, 0, 0);delay(2);//加度数向外
delay(1000);

servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(75, 10, 0, 0);delay(2);//加度数向上
servoy3.Do(115, 50, 0, 0);delay(2);//加度数向上
servoy4.Do(145, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(185, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(145, 20, 0, 0);delay(2);//加度数向下
servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy11.Do(120, 50, 0, 0);delay(2);//加度数脚内扣
servoy12.Do(140, 50, 0, 0);delay(2);//加度数向外
delay(1000);

servoy1.Do(20, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(55, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(115, 20, 0, 0);delay(2);//加度数向上
servoy4.Do(205, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(215, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(155, 20, 0, 0);delay(2);//加度数向下
servoy7.Do(110, 50, 0, 0);delay(2);//减度数向外////////////////////
servoy11.Do(105, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(100, 50, 0, 0);delay(2);//加度数脚外翻
servoy8.Do(65, 50, 0, 0);delay(2);//加度数向后   前25
servoy9.Do(65, 50, 0, 0);delay(2);//加度数向前   后10
servoy10.Do(110, 50, 0, 0);delay(2);//加度数向前  后20

servoy13.Do(80, 50, 0, 0);delay(2);//加度数向前  后40
servoy15.Do(85, 50, 0, 0);delay(2);//加度数向后  前30
delay(1000);
servoy2.Do(165, 30, 0, 0);delay(2);//加度数向上+30
servoy3.Do(90, 30, 0, 0);delay(2);//加度数向上+45
servoy5.Do(165, 30, 0, 0);delay(2);//加度数向下+30
servoy6.Do(90, 30, 0, 0);delay(2);//加度数向下-45
servoy7.Do(95, 30, 0, 0);delay(2);//减度数向外-20
servoy11.Do(125, 30, 0, 0);delay(2);//加度数脚内扣+20
servoy12.Do(105, 30, 0, 0);delay(2);//加度数向外-20
servoy16.Do(100, 30, 0, 0);delay(2);//加度数脚外翻+20

delay(1000);

servoy2.Do(105, 30, 0, 0);delay(2);//加度数向上-30
servoy3.Do(180, 30, 0, 0);delay(2);//加度数向上+45
servoy5.Do(105, 30, 0, 0);delay(2);//加度数向下+30
servoy6.Do(180, 30, 0, 0);delay(2);//加度数向下+45
servoy7.Do(125, 30, 0, 0);delay(2);//减度数向外+20
servoy11.Do(95, 30, 0, 0);delay(2);//加度数脚内扣-20
servoy12.Do(135, 30, 0, 0);delay(2);//加度数向外+20
servoy16.Do(70, 30, 0, 0);delay(2);//加度数脚外翻-20

delay(1000);

}


void abc()
{servoy7.Do(125, 30, 0, 0);delay(2);//减度数向外-5
servoy11.Do(80, 30, 0, 0);delay(2);//加度数脚内扣-10
servoy12.Do(130, 30, 0, 0);delay(2);//加度数向外
servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
delay(100);
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
//delay(1000);
//servoy16.Do(100, 30, 0, 0);delay(2);//加度数脚外翻
//delay(200);
delay(100);
servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
delay(100);
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
delay(100);

servoy2.Do(135, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 30, 0, 0);delay(2);//加度数向上
servoy1.Do(110, 30, 0, 0);delay(2);//加度数向后摆
delay(1000);
servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
delay(100);
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(200);

servoy5.Do(135, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 30, 0, 0);delay(2);//加度数向下
servoy4.Do(115, 30, 0, 0);delay(2);//加度数向前挥
delay(1000);
servoy5.Do(135, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 20, 0, 0);delay(2);//加度数向下
delay(2000);
servoy5.Do(90, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 20, 0, 0);delay(2);//加度数向下
delay(1000);
servoy5.Do(135, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 20, 0, 0);delay(2);//加度数向下
delay(2000);
servoy5.Do(90, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 20, 0, 0);delay(2);//加度数向下
delay(1000);
servoy8.Do(100, 50, 0, 0);delay(2);//加度数向后
servoy13.Do(110, 50, 0, 0);delay(2);//加度数向前
servoy9.Do(45, 50, 0, 0);delay(2);//加度数向前
servoy10.Do(165, 50, 0, 0);delay(2);//加度数向前
servoy14.Do(152, 50, 0, 0);delay(2);//加度数向后
servoy15.Do(80, 50, 0, 0);delay(2);//加度数向后//屈

servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy12.Do(130, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(115, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(65, 50, 0, 0);delay(2);//加度数脚外翻//岔
delay(1000);

servoy1.Do(200, 30, 0, 0);delay(2);//加度数向后摆
servoy2.Do(215, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(215, 30, 0, 0);delay(2);//加度数向上//出圈
delay(1000);




servoy1.Do(110, 40, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy16.Do(35, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(200, 40, 0, 0);delay(2);//加度数向后摆
servoy2.Do(215, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(215, 40, 0, 0);delay(2);//加度数向上
servoy16.Do(65, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(110, 40, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy16.Do(35, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(200, 40, 0, 0);delay(2);//加度数向后摆
servoy2.Do(215, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(215, 40, 0, 0);delay(2);//加度数向上
servoy16.Do(65, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(110, 40, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy16.Do(35, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(200, 40, 0, 0);delay(2);//加度数向后摆
servoy2.Do(215, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(215, 40, 0, 0);delay(2);//加度数向上
servoy16.Do(65, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);





servoy8.Do(100, 50, 0, 0);delay(2);//加度数向后
servoy13.Do(110, 50, 0, 0);delay(2);//加度数向前
servoy9.Do(45, 50, 0, 0);delay(2);//加度数向前
servoy10.Do(165, 50, 0, 0);delay(2);//加度数向前
servoy14.Do(152, 50, 0, 0);delay(2);//加度数向后
servoy15.Do(80, 50, 0, 0);delay(2);//加度数向后//屈

servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy12.Do(130, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(115, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(65, 50, 0, 0);delay(2);//加度数脚外翻//岔
delay(1000);

servoy1.Do(110, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 20, 0, 0);delay(2);//加度数向上//圈回正

servoy4.Do(25, 30, 0, 0);delay(2);//加度数向前挥
servoy5.Do(55, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(55, 30, 0, 0);delay(2);//加度数向下//出枪
delay(1000);

servoy4.Do(115, 40, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy11.Do(145, 40, 0, 0);delay(2);//加度数脚内扣
delay(1000);
servoy4.Do(25, 40, 0, 0);delay(2);//加度数向前挥
servoy5.Do(55, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(55, 40, 0, 0);delay(2);//加度数向下
servoy11.Do(115, 40, 0, 0);delay(2);//加度数脚内扣
delay(1000);
servoy4.Do(115, 40, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy11.Do(145, 40, 0, 0);delay(2);//加度数脚内扣
delay(1000);
servoy4.Do(25, 40, 0, 0);delay(2);//加度数向前挥
servoy5.Do(55, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(55, 40, 0, 0);delay(2);//加度数向下
servoy11.Do(115, 40, 0, 0);delay(2);//加度数脚内扣
delay(1000);
servoy4.Do(115, 40, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy11.Do(145, 40, 0, 0);delay(2);//加度数脚内扣
delay(1000);
servoy4.Do(25, 40, 0, 0);delay(2);//加度数向前挥
servoy5.Do(55, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(55, 40, 0, 0);delay(2);//加度数向下
servoy11.Do(115, 40, 0, 0);delay(2);//加度数脚内扣
delay(1000);
servoy7.Do(95, 50, 0, 0);delay(2);//减度数向外
servoy12.Do(125, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(105, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(75, 50, 0, 0);delay(2);//加度数脚外翻//岔
delay(1000);

servoy1.Do(110, 30, 0, 0);delay(2);//加度数向后摆
servoy4.Do(115, 30, 0, 0);delay(2);//加度数向前挥
servoy2.Do(180, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 30, 0, 0);delay(2);//加度数向上
servoy5.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 30, 0, 0);delay(2);//加度数向下
delay(500);
servoy11.Do(125, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(55, 30, 0, 0);delay(2);//加度数脚外翻
delay(500);
servoy11.Do(105, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(75, 30, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy2.Do(195, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 30, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
servoy5.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(500);
servoy11.Do(125, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(55, 30, 0, 0);delay(2);//加度数脚外翻
delay(500);
servoy11.Do(105, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(75, 30, 0, 0);delay(2);//加度数脚外翻//岔
delay(1000);

servoy1.Do(110, 30, 0, 0);delay(2);//加度数向后摆
servoy4.Do(115, 30, 0, 0);delay(2);//加度数向前挥
servoy2.Do(180, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 30, 0, 0);delay(2);//加度数向上
servoy5.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 30, 0, 0);delay(2);//加度数向下
delay(500);
servoy11.Do(125, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(55, 30, 0, 0);delay(2);//加度数脚外翻
delay(500);
servoy11.Do(105, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(75, 30, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy2.Do(195, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 30, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
servoy5.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(500);
servoy11.Do(125, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(55, 30, 0, 0);delay(2);//加度数脚外翻
delay(500);
servoy11.Do(105, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(75, 30, 0, 0);delay(2);//加度数脚外翻//岔
delay(1000);
servoy7.Do(95, 50, 0, 0);delay(2);//减度数向外
servoy12.Do(125, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(105, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(75, 50, 0, 0);delay(2);//加度数脚外翻//岔
servoy2.Do(180, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 30, 0, 0);delay(2);//加度数向上
servoy5.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 30, 0, 0);delay(2);//加度数向下
delay(1000);

servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy7.Do(95, 50, 0, 0);delay(2);//减度数向外
servoy12.Do(125, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(105, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(75, 50, 0, 0);delay(2);//加度数脚外翻//岔
delay(1000);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下//56回来
delay(200);

servoy2.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下//56回来
delay(200);

servoy2.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
delay(200);


servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下//56回来
delay(200);

servoy2.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下//56回来
delay(200);

servoy2.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上//拉伸///////////////////////////////////
delay(200);

servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
delay(100);
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy1.Do(110, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 20, 0, 0);delay(2);//加度数向上
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(100);
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy4.Do(115, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 20, 0, 0);delay(2);//加度数向下
delay(200);


servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
delay(100);
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy1.Do(110, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 20, 0, 0);delay(2);//加度数向上
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(100);
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy4.Do(115, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 20, 0, 0);delay(2);//加度数向下
delay(200);

servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
delay(100);
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy1.Do(110, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 20, 0, 0);delay(2);//加度数向上
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(100);
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy4.Do(115, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 20, 0, 0);delay(2);//加度数向下//////////////////////////////////////////
delay(200);



servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
delay(100);
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下//56回来
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(100);
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
delay(200);

servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
delay(100);
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下//56回来
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(100);
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
delay(200);

servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
delay(100);
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下//56回来
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(100);
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上///////////////////////////////////
delay(200);

}


void q()
{
servoy7.Do(125, 30, 0, 0);delay(2);//减度数向外-5
servoy11.Do(80, 30, 0, 0);delay(2);//加度数脚内扣-10
servoy12.Do(130, 30, 0, 0);delay(2);//加度数向外
servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
delay(100);
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
//delay(1000);
//servoy16.Do(100, 30, 0, 0);delay(2);//加度数脚外翻
//delay(200);
delay(2000);
servoy7.Do(100, 30, 0, 0);delay(2);//减度数向外-5
servoy12.Do(100, 30, 0, 0);delay(2);//加度数向外
servoy16.Do(100, 30, 0, 0);delay(2);//加度数脚外翻
servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
delay(100);
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
//delay(1000);
//servoy11.Do(70, 30, 0, 0);delay(2);//加度数脚内扣-10
//delay(200);
delay(2000);

}
void w()
{
servoy7.Do(100, 30, 0, 0);delay(2);//减度数向外-5
servoy12.Do(100, 30, 0, 0);delay(2);//加度数向外
servoy16.Do(100, 30, 0, 0);delay(2);//加度数脚外翻
servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
delay(100);
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
//delay(1000);
//servoy11.Do(70, 30, 0, 0);delay(2);//加度数脚内扣-10
//delay(200);
delay(2000);
}

void right()//乾坤圈
{
servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
delay(100);
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
delay(3000);

servoy2.Do(135, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 30, 0, 0);delay(2);//加度数向上
servoy1.Do(110, 30, 0, 0);delay(2);//加度数向后摆
delay(1000);
}

void left()//火尖枪
{
servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
delay(100);
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(3000);

servoy5.Do(135, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 30, 0, 0);delay(2);//加度数向下
servoy4.Do(115, 30, 0, 0);delay(2);//加度数向前挥
delay(1000);
}

void waveleft()
{
servoy5.Do(135, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 20, 0, 0);delay(2);//加度数向下
delay(2000);
servoy5.Do(90, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 20, 0, 0);delay(2);//加度数向下
delay(1000);
servoy5.Do(135, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 20, 0, 0);delay(2);//加度数向下
delay(2000);
servoy5.Do(90, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 20, 0, 0);delay(2);//加度数向下
delay(1000);
}


void prepare()//准备
{
//servoy5.Do(90, 50, 0, 0);delay(2);//加度数向前挥
//servoy6.Do(75, 50, 0, 0);delay(2);//加度数向下
//servoy4.Do(25, 50, 0, 0);delay(2);//加度数向前挥
//
//servoy2.Do(195, 50, 0, 0);delay(2);//加度数向上
//servoy3.Do(180, 50, 0, 0);delay(2);//加度数向上
//servoy1.Do(250, 50, 0, 0);delay(2);//加度数向后摆
//delay(10000);

servoy8.Do(100, 50, 0, 0);delay(2);//加度数向后
servoy13.Do(110, 50, 0, 0);delay(2);//加度数向前
servoy9.Do(45, 50, 0, 0);delay(2);//加度数向前
servoy10.Do(165, 50, 0, 0);delay(2);//加度数向前
servoy14.Do(152, 50, 0, 0);delay(2);//加度数向后
servoy15.Do(80, 50, 0, 0);delay(2);//加度数向后//屈

servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy12.Do(130, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(115, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(65, 50, 0, 0);delay(2);//加度数脚外翻//岔
delay(1000);

servoy12.Do(105, 50, 0, 0);delay(2);//加度数向外
servoy16.Do(90, 50, 0, 0);delay(2);//加度数脚外翻//偏枪
delay(1000);

servoy5.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy1.Do(200, 30, 0, 0);delay(2);//加度数向后摆
servoy2.Do(215, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(215, 30, 0, 0);delay(2);//加度数向上//胳膊准备
delay(1000);

servoy2.Do(195, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 30, 0, 0);delay(2);//加度数向上
servoy8.Do(120, 30, 0, 0);delay(2);//加度数向后
servoy10.Do(180, 30, 0, 0);delay(2);//加度数向前
servoy13.Do(90, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(60, 30, 0, 0);delay(2);//加度数向后
delay(1000);

servoy2.Do(175, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(175, 30, 0, 0);delay(2);//加度数向上
servoy8.Do(100, 30, 0, 0);delay(2);//加度数向后
servoy13.Do(110, 30, 0, 0);delay(2);//加度数向前
servoy10.Do(165, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(80, 30, 0, 0);delay(2);//加度数向后
delay(1000);

servoy2.Do(155, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(155, 30, 0, 0);delay(2);//加度数向上
servoy8.Do(120, 30, 0, 0);delay(2);//加度数向后
servoy10.Do(180, 30, 0, 0);delay(2);//加度数向前
servoy13.Do(90, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(60, 30, 0, 0);delay(2);//加度数向后
delay(1000);

servoy2.Do(135, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 30, 0, 0);delay(2);//加度数向上
servoy8.Do(100, 30, 0, 0);delay(2);//加度数向后
servoy13.Do(110, 30, 0, 0);delay(2);//加度数向前
servoy10.Do(165, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(80, 30, 0, 0);delay(2);//加度数向后//回去
delay(1000);

servoy2.Do(155, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(155, 30, 0, 0);delay(2);//加度数向上
servoy8.Do(120, 30, 0, 0);delay(2);//加度数向后
servoy10.Do(180, 30, 0, 0);delay(2);//加度数向前
servoy13.Do(90, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(60, 30, 0, 0);delay(2);//加度数向后
delay(1000);

servoy2.Do(175, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(175, 30, 0, 0);delay(2);//加度数向上
servoy8.Do(100, 30, 0, 0);delay(2);//加度数向后
servoy13.Do(110, 30, 0, 0);delay(2);//加度数向前
servoy10.Do(165, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(80, 30, 0, 0);delay(2);//加度数向后
delay(1000);

servoy2.Do(195, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 30, 0, 0);delay(2);//加度数向上
servoy8.Do(120, 30, 0, 0);delay(2);//加度数向后
servoy10.Do(180, 30, 0, 0);delay(2);//加度数向前
servoy13.Do(90, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(60, 30, 0, 0);delay(2);//加度数向后
delay(1000);

servoy2.Do(215, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(215, 30, 0, 0);delay(2);//加度数向上
servoy8.Do(100, 30, 0, 0);delay(2);//加度数向后
servoy13.Do(110, 30, 0, 0);delay(2);//加度数向前
servoy10.Do(165, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(80, 30, 0, 0);delay(2);//加度数向后//偏枪结束
delay(1000);



servoy1.Do(110, 30, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 30, 0, 0);delay(2);//加度数向上

servoy4.Do(115, 30, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 30, 0, 0);delay(2);//加度数向下

servoy8.Do(100, 50, 0, 0);delay(2);//加度数向后
servoy13.Do(110, 50, 0, 0);delay(2);//加度数向前
servoy9.Do(45, 50, 0, 0);delay(2);//加度数向前
servoy10.Do(165, 50, 0, 0);delay(2);//加度数向前
servoy14.Do(152, 50, 0, 0);delay(2);//加度数向后
servoy15.Do(80, 50, 0, 0);delay(2);//加度数向后//屈

servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy12.Do(130, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(115, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(65, 50, 0, 0);delay(2);//加度数脚外翻//岔
delay(1000);

servoy7.Do(110, 50, 0, 0);delay(2);//减度数向外
servoy11.Do(90, 50, 0, 0);delay(2);//加度数脚内扣//偏圈
delay(1000);

servoy2.Do(180, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 30, 0, 0);delay(2);//加度数向上
servoy4.Do(25, 30, 0, 0);delay(2);//加度数向前挥
servoy5.Do(55, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(55, 30, 0, 0);delay(2);//加度数向下//胳膊准备
delay(1000);

servoy5.Do(75, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(75, 30, 0, 0);delay(2);//加度数向下
servoy8.Do(120, 30, 0, 0);delay(2);//加度数向后
servoy10.Do(180, 30, 0, 0);delay(2);//加度数向前
servoy13.Do(90, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(60, 30, 0, 0);delay(2);//加度数向后
delay(1000);

servoy5.Do(95, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(95, 30, 0, 0);delay(2);//加度数向下
servoy8.Do(100, 30, 0, 0);delay(2);//加度数向后
servoy13.Do(110, 30, 0, 0);delay(2);//加度数向前
servoy10.Do(165, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(80, 30, 0, 0);delay(2);//加度数向后
delay(1000);

servoy5.Do(115, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(115, 30, 0, 0);delay(2);//加度数向下
servoy8.Do(120, 30, 0, 0);delay(2);//加度数向后
servoy10.Do(180, 30, 0, 0);delay(2);//加度数向前
servoy13.Do(90, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(60, 30, 0, 0);delay(2);//加度数向后
delay(1000);

servoy5.Do(135, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 30, 0, 0);delay(2);//加度数向下
servoy8.Do(100, 30, 0, 0);delay(2);//加度数向后
servoy13.Do(110, 30, 0, 0);delay(2);//加度数向前
servoy10.Do(165, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(80, 30, 0, 0);delay(2);//加度数向后//回去
delay(1000);

servoy5.Do(115, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(115, 30, 0, 0);delay(2);//加度数向下
servoy8.Do(120, 30, 0, 0);delay(2);//加度数向后
servoy10.Do(180, 30, 0, 0);delay(2);//加度数向前
servoy13.Do(90, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(60, 30, 0, 0);delay(2);//加度数向后
delay(1000);

servoy5.Do(95, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(95, 30, 0, 0);delay(2);//加度数向下
servoy8.Do(100, 30, 0, 0);delay(2);//加度数向后
servoy13.Do(110, 30, 0, 0);delay(2);//加度数向前
servoy10.Do(165, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(80, 30, 0, 0);delay(2);//加度数向后
delay(1000);

servoy5.Do(75, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(75, 30, 0, 0);delay(2);//加度数向下
servoy8.Do(120, 30, 0, 0);delay(2);//加度数向后
servoy10.Do(180, 30, 0, 0);delay(2);//加度数向前
servoy13.Do(90, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(60, 30, 0, 0);delay(2);//加度数向后
delay(1000);

servoy5.Do(55, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(55, 30, 0, 0);delay(2);//加度数向下
servoy8.Do(100, 30, 0, 0);delay(2);//加度数向后
servoy13.Do(110, 30, 0, 0);delay(2);//加度数向前
servoy10.Do(165, 30, 0, 0);delay(2);//加度数向前
servoy15.Do(80, 30, 0, 0);delay(2);//加度数向后//偏圈结束
delay(1000);

}



void two()// 点脚
{
servoy8.Do(100, 50, 0, 0);delay(2);//加度数向后
servoy13.Do(110, 50, 0, 0);delay(2);//加度数向前
servoy9.Do(45, 50, 0, 0);delay(2);//加度数向前
servoy10.Do(165, 50, 0, 0);delay(2);//加度数向前
servoy14.Do(152, 50, 0, 0);delay(2);//加度数向后
servoy15.Do(80, 50, 0, 0);delay(2);//加度数向后//屈

servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy12.Do(130, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(115, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(65, 50, 0, 0);delay(2);//加度数脚外翻//岔
delay(1000);

servoy1.Do(200, 30, 0, 0);delay(2);//加度数向后摆
servoy2.Do(215, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(215, 30, 0, 0);delay(2);//加度数向上//出圈
delay(1000);




servoy1.Do(110, 40, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy16.Do(35, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(200, 40, 0, 0);delay(2);//加度数向后摆
servoy2.Do(215, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(215, 40, 0, 0);delay(2);//加度数向上
servoy16.Do(65, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(110, 40, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy16.Do(35, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(200, 40, 0, 0);delay(2);//加度数向后摆
servoy2.Do(215, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(215, 40, 0, 0);delay(2);//加度数向上
servoy16.Do(65, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(110, 40, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy16.Do(35, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);
servoy1.Do(200, 40, 0, 0);delay(2);//加度数向后摆
servoy2.Do(215, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(215, 40, 0, 0);delay(2);//加度数向上
servoy16.Do(65, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);





servoy8.Do(100, 50, 0, 0);delay(2);//加度数向后
servoy13.Do(110, 50, 0, 0);delay(2);//加度数向前
servoy9.Do(45, 50, 0, 0);delay(2);//加度数向前
servoy10.Do(165, 50, 0, 0);delay(2);//加度数向前
servoy14.Do(152, 50, 0, 0);delay(2);//加度数向后
servoy15.Do(80, 50, 0, 0);delay(2);//加度数向后//屈

servoy7.Do(90, 50, 0, 0);delay(2);//减度数向外
servoy12.Do(130, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(115, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(65, 50, 0, 0);delay(2);//加度数脚外翻//岔
delay(1000);

servoy1.Do(110, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 20, 0, 0);delay(2);//加度数向上//圈回正

servoy4.Do(25, 30, 0, 0);delay(2);//加度数向前挥
servoy5.Do(55, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(55, 30, 0, 0);delay(2);//加度数向下//出枪
delay(1000);

servoy4.Do(115, 40, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy11.Do(145, 40, 0, 0);delay(2);//加度数脚内扣
delay(1000);
servoy4.Do(25, 40, 0, 0);delay(2);//加度数向前挥
servoy5.Do(55, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(55, 40, 0, 0);delay(2);//加度数向下
servoy11.Do(115, 40, 0, 0);delay(2);//加度数脚内扣
delay(1000);
servoy4.Do(115, 40, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy11.Do(145, 40, 0, 0);delay(2);//加度数脚内扣
delay(1000);
servoy4.Do(25, 40, 0, 0);delay(2);//加度数向前挥
servoy5.Do(55, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(55, 40, 0, 0);delay(2);//加度数向下
servoy11.Do(115, 40, 0, 0);delay(2);//加度数脚内扣
delay(1000);
servoy4.Do(115, 40, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy11.Do(145, 40, 0, 0);delay(2);//加度数脚内扣
delay(1000);
servoy4.Do(25, 40, 0, 0);delay(2);//加度数向前挥
servoy5.Do(55, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(55, 40, 0, 0);delay(2);//加度数向下
servoy11.Do(115, 40, 0, 0);delay(2);//加度数脚内扣
delay(1000);


}

void three()// 摇
{

servoy7.Do(95, 50, 0, 0);delay(2);//减度数向外
servoy12.Do(125, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(105, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(75, 50, 0, 0);delay(2);//加度数脚外翻//岔
delay(1000);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下//56回来
delay(200);

servoy2.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下//56回来
delay(200);

servoy2.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
delay(200);


servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下//56回来
delay(200);

servoy2.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下//56回来
delay(200);

servoy2.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上//拉伸///////////////////////////////////
delay(200);

servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
delay(100);
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy1.Do(110, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 20, 0, 0);delay(2);//加度数向上
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(100);
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy4.Do(115, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 20, 0, 0);delay(2);//加度数向下
delay(200);


servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
delay(100);
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy1.Do(110, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 20, 0, 0);delay(2);//加度数向上
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(100);
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy4.Do(115, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 20, 0, 0);delay(2);//加度数向下
delay(200);

servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
delay(100);
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy1.Do(110, 20, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 20, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 20, 0, 0);delay(2);//加度数向上
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(100);
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy4.Do(115, 20, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 20, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 20, 0, 0);delay(2);//加度数向下//////////////////////////////////////////
delay(200);



servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
delay(100);
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下//56回来
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(100);
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
delay(200);

servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
delay(100);
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下//56回来
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(100);
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
delay(200);

servoy2.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 40, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
delay(100);
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻//扭1
delay(1000);
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下//56回来
delay(200);

servoy5.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(100);
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻//扭2
delay(1000);
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上///////////////////////////////////
delay(200);


}

void four()// 双点
{

servoy7.Do(95, 50, 0, 0);delay(2);//减度数向外
servoy12.Do(125, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(105, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(75, 50, 0, 0);delay(2);//加度数脚外翻//岔
delay(1000);

servoy1.Do(110, 30, 0, 0);delay(2);//加度数向后摆
servoy4.Do(115, 30, 0, 0);delay(2);//加度数向前挥
servoy2.Do(180, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 30, 0, 0);delay(2);//加度数向上
servoy5.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 30, 0, 0);delay(2);//加度数向下
delay(500);
servoy11.Do(125, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(55, 30, 0, 0);delay(2);//加度数脚外翻
delay(500);
servoy11.Do(105, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(75, 30, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy2.Do(195, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 30, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
servoy5.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(500);
servoy11.Do(125, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(55, 30, 0, 0);delay(2);//加度数脚外翻
delay(500);
servoy11.Do(105, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(75, 30, 0, 0);delay(2);//加度数脚外翻//岔
delay(1000);

servoy1.Do(110, 30, 0, 0);delay(2);//加度数向后摆
servoy4.Do(115, 30, 0, 0);delay(2);//加度数向前挥
servoy2.Do(180, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 30, 0, 0);delay(2);//加度数向上
servoy5.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 30, 0, 0);delay(2);//加度数向下
delay(500);
servoy11.Do(125, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(55, 30, 0, 0);delay(2);//加度数脚外翻
delay(500);
servoy11.Do(105, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(75, 30, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy2.Do(195, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(195, 30, 0, 0);delay(2);//加度数向上
servoy1.Do(210, 30, 0, 0);delay(2);//加度数向后摆
servoy5.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy4.Do(15, 30, 0, 0);delay(2);//加度数向前挥
delay(500);
servoy11.Do(125, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(55, 30, 0, 0);delay(2);//加度数脚外翻
delay(500);
servoy11.Do(105, 30, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(75, 30, 0, 0);delay(2);//加度数脚外翻//岔
delay(1000);


  
}


void five()// 顺步
{
servoy7.Do(95, 50, 0, 0);delay(2);//减度数向外
servoy12.Do(125, 50, 0, 0);delay(2);//加度数向外
servoy11.Do(105, 50, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(75, 50, 0, 0);delay(2);//加度数脚外翻//岔
servoy2.Do(180, 30, 0, 0);delay(2);//加度数向上
servoy3.Do(180, 30, 0, 0);delay(2);//加度数向上
servoy5.Do(90, 30, 0, 0);delay(2);//加度数向下
servoy6.Do(90, 30, 0, 0);delay(2);//加度数向下
delay(1000);

servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy3.Do(180, 40, 0, 0);delay(2);//加度数向上
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(80, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(95, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(120, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy6.Do(90, 40, 0, 0);delay(2);//加度数向下
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy7.Do(125, 40, 0, 0);delay(2);//减度数向外
servoy12.Do(140, 40, 0, 0);delay(2);//加度数向外
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy16.Do(60, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);


}


void hengcha()// 劈叉函数
{
servoy7.Do(70, 70, 0, 0);delay(2);//减度数向外-40
servoy12.Do(150, 70, 0, 0);delay(2);//加度数向外+40
servoy11.Do(140, 70, 0, 0);delay(2);//加度数脚内扣+50
servoy16.Do(40, 70, 0, 0);delay(2);//加度数脚外翻-50
delay(3000);
}

void bolang()// 摇摆
{
servoy2.Do(165, 50, 0, 0);delay(2);//加度数向上+30
servoy3.Do(90, 50, 0, 0);delay(2);//加度数向上+45
servoy5.Do(165, 50, 0, 0);delay(2);//加度数向下+30
servoy6.Do(90, 50, 0, 0);delay(2);//加度数向下-45
servoy7.Do(98, 50, 0, 0);delay(2);//减度数向外-20+3
servoy11.Do(113, 50, 0, 0);delay(2);//加度数脚内扣+20+3-10
servoy12.Do(90, 50, 0, 0);delay(2);//加度数向外-20
servoy16.Do(110, 50, 0, 0);delay(2);//加度数脚外翻+20
delay(1000);

servoy2.Do(105, 70, 0, 0);delay(2);//加度数向上-30
servoy3.Do(180, 70, 0, 0);delay(2);//加度数向上+45
servoy5.Do(105, 70, 0, 0);delay(2);//加度数向下+30
servoy6.Do(180, 70, 0, 0);delay(2);//加度数向下+45
servoy7.Do(135, 70, 0, 0);delay(2);//减度数向外+20
servoy11.Do(75, 70, 0, 0);delay(2);//加度数脚内扣-20-10
servoy12.Do(127, 70, 0, 0);delay(2);//加度数向外+20-3
servoy16.Do(63, 70, 0, 0);delay(2);//加度数脚外翻-20-7
delay(1000);

servoy1.Do(110, 40, 0, 0);delay(2);//加度数向后摆
servoy2.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy3.Do(135, 40, 0, 0);delay(2);//加度数向上
servoy4.Do(115, 40, 0, 0);delay(2);//加度数向前挥
servoy5.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy6.Do(135, 40, 0, 0);delay(2);//加度数向下
servoy7.Do(110, 40, 0, 0);delay(2);//减度数向外
servoy8.Do(90, 40, 0, 0);delay(2);//加度数向后
servoy9.Do(75, 40, 0, 0);delay(2);//加度数向前
servoy10.Do(130, 40, 0, 0);delay(2);//加度数向前
servoy11.Do(90, 40, 0, 0);delay(2);//加度数脚内扣
servoy12.Do(110, 40, 0, 0);delay(2);//加度数向外
servoy13.Do(120, 40, 0, 0);delay(2);//加度数向前
servoy14.Do(122, 40, 0, 0);delay(2);//加度数向后
servoy15.Do(115, 40, 0, 0);delay(2);//加度数向后
servoy16.Do(90, 40, 0, 0);delay(2);//加度数脚外翻
delay(1000);

servoy2.Do(165, 50, 0, 0);delay(2);//加度数向上+30
servoy3.Do(90, 50, 0, 0);delay(2);//加度数向上+45
servoy5.Do(165, 50, 0, 0);delay(2);//加度数向下+30
servoy6.Do(90, 50, 0, 0);delay(2);//加度数向下-45
servoy7.Do(98, 50, 0, 0);delay(2);//减度数向外-20+3
servoy11.Do(113, 50, 0, 0);delay(2);//加度数脚内扣+20+3-10
servoy12.Do(90, 50, 0, 0);delay(2);//加度数向外-20
servoy16.Do(110, 50, 0, 0);delay(2);//加度数脚外翻+20
delay(1000);

servoy2.Do(105, 70, 0, 0);delay(2);//加度数向上-30
servoy3.Do(180, 70, 0, 0);delay(2);//加度数向上+45
servoy5.Do(105, 70, 0, 0);delay(2);//加度数向下+30
servoy6.Do(180, 70, 0, 0);delay(2);//加度数向下+45
servoy7.Do(135, 70, 0, 0);delay(2);//减度数向外+20
servoy11.Do(75, 70, 0, 0);delay(2);//加度数脚内扣-20-10
servoy12.Do(127, 70, 0, 0);delay(2);//加度数向外+20-3
servoy16.Do(63, 70, 0, 0);delay(2);//加度数脚外翻-20-7
delay(1000);
}

void chushi()// 初始
{
  servoy1.Do(110, 50, 0, 0);delay(2);//加度数向后摆
  servoy2.Do(135, 50, 0, 0);delay(2);//加度数向上
  servoy3.Do(135, 50, 0, 0);delay(2);//加度数向上

  servoy4.Do(115, 50, 0, 0);delay(2);//加度数向前挥
  servoy5.Do(135, 50, 0, 0);delay(2);//加度数向下
  servoy6.Do(135, 50, 0, 0);delay(2);//加度数向下

  servoy7.Do(110, 50, 0, 0);delay(2);//减度数向外
  servoy8.Do(90, 50, 0, 0);delay(2);//加度数向后
  servoy9.Do(75, 50, 0, 0);delay(2);//加度数向前
  servoy10.Do(130, 50, 0, 0);delay(2);//加度数向前
  servoy11.Do(90, 50, 0, 0);delay(2);//加度数脚内扣

  servoy12.Do(110, 50, 0, 0);delay(2);//加度数向外
  servoy13.Do(120, 50, 0, 0);delay(2);//加度数向前
  servoy14.Do(122, 50, 0, 0);delay(2);//加度数向后
  servoy15.Do(115, 50, 0, 0);delay(2);//加度数向后
  servoy16.Do(90, 50, 0, 0);delay(2);//加度数脚外翻

delay(2000);
}

// void setServoAngle(int targetAngle) 
// {
//  if (targetAngle < 0) targetAngle = 0;
//  if (targetAngle > 180) targetAngle = 180;
 
//  int direction = (targetAngle > currentAngle) ? 1 : -1;
//  for (int angle = currentAngle; angle != targetAngle; angle += direction) {
//  myServo.write(angle);
//  delay(15);
// }
  
//   myServo.write(targetAngle);
//   currentAngle = targetAngle;
// }
	// === 外部中断服务函数 ===
	// 当遥控器按下瞬间，会立刻触发此函数，不受 delay 影响
	void triggerDance() {
	  if (!isDancing) {
	    Serial.println(">>> 中断触发！收到遥控信号");
	    executeDance = true;
	  }
	}
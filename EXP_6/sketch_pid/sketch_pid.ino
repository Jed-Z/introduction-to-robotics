#include <PinChangeInt.h> //外部中断
#include <MsTimer2.h>     //定时中断
#include "define.h"

void setup()
{
    pinMode(ENCODER_L, INPUT);                                  //编码器引脚
    pinMode(DIRECTION_L, INPUT);                                //编码器引脚
    pinMode(ENCODER_R, INPUT);                                  //编码器引脚
    pinMode(DIRECTION_R, INPUT);                                //编码器引脚
    attachInterrupt(0, handlerR, CHANGE);                       //开启外部中断 编码器接口1
    attachPinChangeInterrupt(DIRECTION_L, handlerL, CHANGE);    //开启外部中断 编码器接口2

    while (select())
    {
    }                           
    MsTimer2::set(10, control);                                 //使用Timer2设置5ms定时中断
    MsTimer2::start();                                          //中断使能
}

void handlerR()
{
  if (digitalRead(ENCODER_R) == LOW)                             //如果是下降沿触发的中断
  {
    if (digitalRead(DIRECTION_R) == LOW)
      speedR++;                                              //根据另外一相电平判定方向
    else
      speedR--;
  }
  else                                                           //如果是上升沿触发的中断
  {
    if (digitalRead(DIRECTION_R) == LOW)
      speedR--;                                              //根据另外一相电平判定方向
    else
      speedR++;
  }
}

void loop() {
    
}

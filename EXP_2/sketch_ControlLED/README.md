# 实验 2_1：呼吸灯

## 预备知识

### 串口

* 串口和 IO 口一样,需要初始化,参数为波特率 `Serial.begin(9600)`

* 然后串口从板子上发送数据为` Serial.print(msg)`

* 或发送后再发一个回车的 `Serial.println(msg)`

* 接受信息为 `Serial.read()`，返回 int

* 或者使用 `Serial.readString()`，返回 String

更多功能请参阅[串口](https://www.arduino.cc/reference/en/language/functions/communication/serial/)



### EEPROM

* EEPROM 为非易失性储存器，当开发板断电后其数据仍可以保存在里面，我们使用的 UNO 其 EEPROM 有 1 K Byte，详细用法请看示例中的 EEPROM。

* 操作基本为读写

  ```c
  #include <EEPROM.h>
  EEPROM.write(addr, val);
  EEPROM.read(addr, val);
  ```



## 任务

实现一套简单的通讯协议，实现通过串口可以在电脑上输入指令后使得板载 LED 输出指定亮度。并且保存该亮度数值在 EEPROM 中，能断电后重新加载数值。
# 实验 2_2：让小车直线行驶

## 预备知识

### 舵机

舵机基本的功能为接收 PWM 信号，然后使其摇臂转动到指定角度的位置。

使用方式基本为：

```cpp
#include <Servo.h>
Servo myservo;		//新建一个对象
myservo.attach(pin);
myservo.write(deg);	//令舵机转向到指定角度
```



### 电机控制

每边电机分为使能与速度控制，使能控制电机旋转方向。

电机使能采用数字信号(`digitalWrite`) ，输出 0 为正转，反之反转；

电机速度采用模拟信号(`analogWrite`) ，输出 0 为不动，输出255为最快。

左边电机使能为 11，速度控制为 5；

右边电机使能为 3，速度控制为 6。



## 任务

仔细观察车辆结构，写出简单的控制程序，目标是让其能够直线行驶。

* Hint1: 舵机零点似乎是 95；
* Hint2: 不要让车子跑这么快；
* Hint3: 可以使用 EEPROM 储存速度和舵机零点，不用每次都修改程序烧写。

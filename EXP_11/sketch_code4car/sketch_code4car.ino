#include <SSD1306.h>
#include <Servo.h>
#include <EEPROM.h>
#include <MsTimer2.h>
#include <PinChangeInt.h>

#define AIN1 11
#define AIN2 5
#define BIN1 6
#define BIN2 3
#define SERVO 9

//Encoder
#define ENCODER_L 8 //编码器采集引脚 每路2个 共4个
#define DIRECTION_L 4
#define ENCODER_R 7
#define DIRECTION_R 2
#define KEY 18
// OLED
#define OLED_DC 10
#define OLED_CLK 19
#define OLED_MOSI 13
#define OLED_RESET 12

SSD1306 oled(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, 0);
Servo myservo;

String inputString = "";     // a String to hold incoming data
bool stringComplete = false; // whether the string is complete

/*  1:RC
    2:Serial
*/
uint8_t runningMode = 1;
byte flag_key = 1;
volatile long Velocity_L, Velocity_R;
int Velocity_Left, Velocity_Right = 0, Velocity, Angle;
/*
  0: Controller
  1: Serial receiver
  2: NotDefine */
byte sysMode = 0;

void Set_Pwm(int motora, int motorb)
{
  if (motora > 0)
  {
    analogWrite(AIN2, motora);
    digitalWrite(AIN1, LOW);
  }
  else
  {
    digitalWrite(AIN1, HIGH);
    analogWrite(AIN2, 255 + motora);
  }

  if (motorb > 0)
  {
    digitalWrite(BIN2, LOW);
    analogWrite(BIN1, motorb);
  }
  else
  {
    analogWrite(BIN1, 255 + motorb);
    digitalWrite(BIN2, HIGH);
  }
}

uint32_t int_pow(uint8_t m, uint8_t n)
{
  uint32_t result = 1;
  while (n--)
    result *= m;
  return result;
}

void OLED_ShowNumber(uint8_t x, uint8_t y, uint32_t num, uint8_t len)
{
  uint8_t t, temp;
  uint8_t enshow = 0;
  for (t = 0; t < len; t++)
  {
    temp = (num / int_pow(10, len - t - 1)) % 10;
    oled.drawchar(x + 6 * t, y, temp + '0');
  }
}

// Not fully test, alpha version
void RC(uint8_t dir, int spd)
{

  if (dir == 0)
  {
    Set_Pwm(spd * 0.64, spd);
  }
  else if (dir == 255)
  {
    Set_Pwm(spd, spd * 0.74);
  }
  else
  {
    Set_Pwm(spd, spd);
  }

  Set_Pwm(spd, spd);
  myservo.write((uint8_t)(dir / 2.83) + 50);
}

void serialEvent()
{
  inputString = "";
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    if (inChar == '\n')
    {
      stringComplete = true;
      break;
    }
    inputString += inChar;
  }
}

void READ_ENCODER_L()
{
  if (digitalRead(ENCODER_L) == LOW)
  {
    if (digitalRead(DIRECTION_L) == LOW)
      Velocity_L--;
    else
      Velocity_L++;
  }
  else
  {
    if (digitalRead(DIRECTION_L) == LOW)
      Velocity_L++;
    else
      Velocity_L--;
  }
}
void READ_ENCODER_R()
{
  if (digitalRead(ENCODER_R) == LOW)
  {
    if (digitalRead(DIRECTION_R) == LOW)
      Velocity_R++;
    else
      Velocity_R--;
  }
  else
  {
    if (digitalRead(DIRECTION_R) == LOW)
      Velocity_R--;
    else
      Velocity_R++;
  }
}

/*
  0: key not pressed
  1: key pressed
*/
int check_click(void)
{
  if (flag_key && (digitalRead(KEY) == 0))
  {
    flag_key = 0;
    if (digitalRead(KEY) == 0)
      return 1;
  }
  else if (digitalRead(KEY) == 1)
    flag_key = 1;
  return 0;
}

void setup()
{
  char err;
  //Init OLED screen
  oled.ssd1306_init(SSD1306_SWITCHCAPVCC);
  oled.clear(); // clears the screen and buffer

  //Init motor control
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  //Init servo
  myservo.attach(SERVO);

  //Init encoder
  pinMode(ENCODER_L, INPUT);
  pinMode(DIRECTION_L, INPUT);
  pinMode(ENCODER_R, INPUT);
  pinMode(DIRECTION_R, INPUT);

  //Init user key
  pinMode(KEY, INPUT);

  //Init interrupt
  attachInterrupt(0, READ_ENCODER_R, CHANGE);
  attachPinChangeInterrupt(4, READ_ENCODER_L, CHANGE);

  delay(1000);
  Serial.begin(115200);
  Serial.println("[+] Booted.");
  while (check_click() != 1)
  {
    oled.clear();
    int count = abs(Velocity_R);
    OLED_ShowNumber(0, 0, count, 7);
    oled.drawstring(0, 1, "select");
    int selectNum = Velocity_R % 500;

    runningMode = 2;

    oled.display();
  }
  Serial.println("[+] Setup done.");
}

void loop()
{
  oled.clear();
  serialEvent();
  if (stringComplete)
  {
    oled.drawstring(0, 4, (char *)inputString.c_str());
    stringComplete = false;
    Serial.println(inputString);
    // inputString = "";
  }

  // int angle = inputString.c_str()[0];
  int angle = inputString.toInt();
  OLED_ShowNumber(60, 00, angle, 3);
  // RC(angle, 255);
  RC(angle, 0);


  int volt = (5.371 * analogRead(0));
  OLED_ShowNumber(0, 3, volt / 100, 2);
  OLED_ShowNumber(12, 3, volt % 100, 2);
  oled.display();
}

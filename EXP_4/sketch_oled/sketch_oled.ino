// OLED
#include <SSD1306.h>
#define OLED_DC 10
#define OLED_CLK 19
#define OLED_MOSI 13
#define OLED_RESET 12

// SERVO pin
#define SERVO 9
//Encoder
#define ENCODER_L 8 
#define DIRECTION_L 4
#define ENCODER_R 7
#define DIRECTION_R 2
// Motor
#define Ldirection 11
#define Lspeed 5
#define Rspeed 6
#define Rdirection 3

// OLED init
SSD1306 oled(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, 0);
void setup()
{
    oled.ssd1306_init(SSD1306_SWITCHCAPVCC);
    oled.clear(); // clears the screen and buffer

    oled.clear();
    oled.drawstring(0, 1, (char*)"motor");
    oled.display();     //display the buffer
}

void loop() {
}

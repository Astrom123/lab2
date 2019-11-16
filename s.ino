#include <Arduino.h>
#include <MD_TCS230.h>
#include "LedControl.h"
#include "symbols.h"

#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5
MD_TCS230 ColorSensor(S2_OUT, S3_OUT, S0_OUT, S1_OUT);

LedControl ledDisplay = LedControl(26, 22, 24, 1);  // (DIN, CLK, CS, Количество дисплеев)

int (*digits[16])[8][8] = {&DIG_0, &DIG_1, &DIG_2, &DIG_3, &DIG_4, &DIG_5, &DIG_6, &DIG_7, &DIG_8, &DIG_9, &DIG_A, &DIG_B, &DIG_C, &DIG_D, &DIG_E, &DIG_F};

void setup()
{
    Serial.begin(115200);
    ledDisplay.shutdown(0, false);
    ledDisplay.setIntensity(0, 10);
    ledDisplay.clearDisplay(0);
  
    sensorData whiteCalibration;
    whiteCalibration.value[TCS230_RGB_R] = 120060;
    whiteCalibration.value[TCS230_RGB_G] = 117520;
    whiteCalibration.value[TCS230_RGB_B] = 157590;
    
    sensorData blackCalibration;
    blackCalibration.value[TCS230_RGB_R] = 11280;
    blackCalibration.value[TCS230_RGB_G] = 10270;
    blackCalibration.value[TCS230_RGB_B] = 13230;

    ColorSensor.begin();
    ColorSensor.setDarkCal(&blackCalibration);
    ColorSensor.setWhiteCal(&whiteCalibration);
}

void loop() 
{
    colorData rgb;
    ColorSensor.read();
    while (!ColorSensor.available())
        ;
    ColorSensor.getRGB(&rgb);
    set_rgb_led(rgb);
}

void set_rgb_led(colorData rgb)
{
    int red = rgb.value[TCS230_RGB_R];
    int green = rgb.value[TCS230_RGB_G];
    int blue = rgb.value[TCS230_RGB_B];

    Serial.println(red);
    display_symbol(SIGN_R);
    delay(1000);
    display_number(red);
    delay(1000);
    
    display_symbol(SIGN_G);
    delay(1000);
    display_number(green);
    delay(1000);
    
    display_symbol(SIGN_B);
    delay(1000);
    display_number(blue);
    delay(1000);
}

void display_symbol(int symbol[8][8])
{
    ledDisplay.clearDisplay(0);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            if (symbol[i][j] == 1)
              ledDisplay.setLed(0, i, j, true);
        }
}

void display_number(int number)
{
    ledDisplay.clearDisplay(0);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            if ((*digits[number / 16])[i][j] == 1)
                ledDisplay.setLed(0, i, j, true);
                
            if ((*digits[number % 16])[i][j] == 1)
                ledDisplay.setLed(0, i, j + 4, true);
        }
}

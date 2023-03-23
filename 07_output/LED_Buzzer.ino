#include <Adafruit_DotStar.h>
#include <SPI.h>

#define NUMPIXELS 72 // Number of LEDs in strip
#define DATAPIN 10 // LED D
#define CLOCKPIN 9 // LED C
#define BUZZPIN 8 // Buzzer
#define POTPIN 2 // Potentiometer
#define DELAY 100 // mS

Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

class Pot
{
  unsigned long previousMillis;
  long interval;

  int pixel;
  int data_pin;
  int clock_pin;
  int buzz_pin;
  int potValue;
  int pot_pin;

  int r;
  int g;
  int b;
  int f;

  public:
  Pot(int pix, int data, int clock, int buzz, int pot, long delay)
  {    
    pixel = pix;
    data_pin = data;
    clock_pin = clock;
    buzz_pin = buzz;
    pot_pin = pot;
    interval = delay;

    previousMillis = 0;

    pinMode(buzz_pin, OUTPUT);
  }

  void Update()
  {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      potValue = analogRead(pot_pin);

      r = map(potValue, 0, 4095, 255, 0); //red
      b = map(potValue, 0, 4095, 0, 255); //blue
      g = map(potValue, 0, 4095, 0, 100); //green
      f = map(potValue, 0, 4095, 1000, 100); //buzzer frequency

      strip.begin();
      strip.show();

      for (int i=0; i<NUMPIXELS; i++) {
        // strip.setPixelColor(index, green, red, blue); tested a bit to identify the sequence of RGB;
        strip.setPixelColor(i, g, r, b);
          }
      strip.setBrightness(64);
      strip.show();

      tone(buzz_pin, f, DELAY);
    }
  }
};



Pot control(NUMPIXELS, DATAPIN, CLOCKPIN, BUZZPIN, POTPIN, DELAY);

void setup()
{}

void loop()
{
  control.Update();
}
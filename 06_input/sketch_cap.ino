class Capacitor
{
  int analog_pin;
  int tx_pin;
  int tx_state;
  unsigned long previousMicros;
  long capTime;
  int read_high;
  int read_low;
  int diff;
  long sum;
  int N_samples = 100; 

  public:
  Capacitor(int tx, int rx, long interval)
  {
    tx_pin = tx;
    analog_pin = rx;
    capTime = interval;

    pinMode(tx_pin, OUTPUT);

    //tx_state = LOW;
    previousMicros = 0;
  }

  long Update()
  {
    unsigned long currentMicros = micros();
    sum = 0;

   if (currentMicros - previousMicros >= capTime)
   {
     for (int i=0; i<N_samples; i++)
     {
       previousMicros = currentMicros;
       if (tx_state == LOW)
       {
         digitalWrite(tx_pin, HIGH);
         read_high = analogRead(analog_pin);
         tx_state = HIGH;
       }
       if (tx_state == HIGH)
       {
         digitalWrite(tx_pin, LOW);
         read_low = analogRead(analog_pin);
         tx_state = LOW;
         diff = read_high - read_low;
         sum += diff;
         }
      }
      return sum;
   }
  }

};

Capacitor cap1(4, A3, 100);

long result;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  result = cap1.Update();
  Serial.println(result);
}
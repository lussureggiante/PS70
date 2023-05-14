#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_DotStar.h>

int status = WL_IDLE_STATUS;
const char* ssid = "MAKERSPACE";
const char* pass = "12345678";
const int localPort = 1234; // set the port number to listen on

char packetBuffer[256]; // buffer to hold incoming packet

const int A1A = 8;  // define pin for air pump A-1A (speed)
const int A1B = 20;  // define pin for air pump A-1B (direction)

#define NUMPIXELS 72 // Number of LEDs in strip
#define DATAPIN 10 // Grey Wire
#define CLOCKPIN 9 // Purple Wire
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
int r = 0; // red value
int g = 0; // green value
int b = 0; // blue value

const int VM1 = 2; // define pin for vibration motor #1 (Top)
const int VM2 = 3; // define pin for vibration motor #2
const int VM3 = 4; // define pin for vibration motor #3
const int VM4 = 5; // define pin for vibration motor #4
const int VM5 = 6; // define pin for vibration motor #5
const int VM6 = 7; // define pin for vibration motor #6
const int VM7 = 21; // define pin for vibration motor #7 (Bottom)

WiFiUDP Udp;

void setup() {
  
  Serial.begin(9600);
  delay(1000);

  // connect to Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  // start UDP
  Udp.begin(localPort);
  //Serial.println("UDP started.");

  pinMode(A1A, OUTPUT);     // Specify the pins as outputs for air pump
  pinMode(A1B, OUTPUT);
  digitalWrite(A1A, HIGH);   // Initialize the Wearable by pumping air in 
  digitalWrite(A1B, LOW);
  delay (5000);
  digitalWrite(A1A, LOW);   // Turn and keep the motors off
  digitalWrite(A1B, LOW);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  pinMode(VM1, OUTPUT); // Specify the pins for vibration motors
  pinMode(VM2, OUTPUT);
  pinMode(VM3, OUTPUT);
  pinMode(VM4, OUTPUT);
  pinMode(VM5, OUTPUT);
  pinMode(VM6, OUTPUT);
  pinMode(VM7, OUTPUT);
  digitalWrite(VM1, LOW); // Start with motors off
  digitalWrite(VM2, LOW);
  digitalWrite(VM3, LOW);
  digitalWrite(VM4, LOW);
  digitalWrite(VM5, LOW);
  digitalWrite(VM6, LOW);
  digitalWrite(VM7, LOW);

}

void loop() {

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBuffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }

    char *part1;
    char *part2;

    part1 = strtok(packetBuffer, ",");
    part2 = strtok(NULL, ",");

    float Freq = atof(part1); // range between 0-1000 except for anomoly
    float Gamma = atof(part2); // range between 0-100 except for anomoly

    Serial.println("Contents:");
    Serial.print(packetBuffer);
    Serial.println(Freq);
    Serial.println(Gamma);

    if (Freq>900 && Freq <=1000){
      digitalWrite(A1A, HIGH);   // setting both pins High stops the motor
      digitalWrite(A1B, LOW);
      delay(500);
      digitalWrite(A1A, LOW);   // setting both pins LOW stops the motor
      digitalWrite(A1B, LOW);
    }

    // if (Gamma == 50.00 || Gamma == -50.00), meaning no EEG signal received.
    // LED colors determined by the frequency of sound
    // Freq low to high: blue (0,0,255) to pink/purple (150,0,150) to red (255,0,0) [Extreme]
    // Adjust colors for better visual outcome

    if (Gamma == 50.00 || Gamma == -50.00) {
      
      r = Freq * 0.255;
      g = 60;
      b = 75 + (1000-Freq) * 0.18;

      strip.setBrightness(48);

      for (int i=0; i<NUMPIXELS; i++) {
        strip.setPixelColor(i, g, r, b);
        strip.show();
        delay(20);
        }
    }
    else {
      // if Gamma wave is detected, LED colors determined by the Gamma value
      // Mood low to high: green (0,255,0) to yellow (255,255,0) to orange (255,155,0)

      r = Gamma * 5;
      if (r>255) {r=255;}
      g = 200 - Gamma*1.5;
      b = 20;

      strip.setBrightness(60);

      for (int i=0; i<NUMPIXELS; i++) {
        strip.setPixelColor(i, g, r, b);
        strip.show();
        delay(20);
        }

    }

    // turn on and off vibration motors based on frequency
   
    if (Freq > 0 && Freq <= 10) {
      digitalWrite(VM1, LOW);
      digitalWrite(VM2, LOW);
      digitalWrite(VM3, LOW);
      digitalWrite(VM4, LOW);
      digitalWrite(VM5, LOW);
      digitalWrite(VM6, LOW);
      digitalWrite(VM7, HIGH);
    }
    else if (Freq > 10 && Freq <30) {
      digitalWrite(VM1, LOW);
      digitalWrite(VM2, LOW);
      digitalWrite(VM3, LOW);
      digitalWrite(VM4, LOW);
      digitalWrite(VM5, LOW);
      digitalWrite(VM6, HIGH);
      digitalWrite(VM7, LOW);
    }
    else if (Freq > 30 && Freq <60) {
      digitalWrite(VM1, LOW);
      digitalWrite(VM2, LOW);
      digitalWrite(VM3, LOW);
      digitalWrite(VM4, LOW);
      digitalWrite(VM5, HIGH);
      digitalWrite(VM6, LOW);
      digitalWrite(VM7, LOW);
    }
    else if (Freq > 60 && Freq <100) {
      digitalWrite(VM1, LOW);
      digitalWrite(VM2, LOW);
      digitalWrite(VM3, LOW);
      digitalWrite(VM4, HIGH);
      digitalWrite(VM5, LOW);
      digitalWrite(VM6, LOW);
      digitalWrite(VM7, LOW);
    }
    else if (Freq > 100 && Freq <200) {
      digitalWrite(VM1, LOW);
      digitalWrite(VM2, LOW);
      digitalWrite(VM3, HIGH);
      digitalWrite(VM4, LOW);
      digitalWrite(VM5, LOW);
      digitalWrite(VM6, LOW);
      digitalWrite(VM7, LOW);
    }
    else if (Freq > 200 && Freq <500) {
      digitalWrite(VM1, LOW);
      digitalWrite(VM2, HIGH);
      digitalWrite(VM3, LOW);
      digitalWrite(VM4, LOW);
      digitalWrite(VM5, LOW);
      digitalWrite(VM6, LOW);
      digitalWrite(VM7, LOW);
    }
    else{
      digitalWrite(VM1, HIGH);
      digitalWrite(VM2, LOW);
      digitalWrite(VM3, LOW);
      digitalWrite(VM4, LOW);
      digitalWrite(VM5, LOW);
      digitalWrite(VM6, LOW);
      digitalWrite(VM7, LOW);
    }
 

    // send a reply to the IP address and port in TouchDesigner
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.print(Freq);
    Udp.print(", ");
    Udp.print(Gamma);
    Udp.endPacket();

  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

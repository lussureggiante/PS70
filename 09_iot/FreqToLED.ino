#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_DotStar.h>

#define NUMPIXELS 72 // Number of LEDs in strip
#define DATAPIN 10 // Grey Wire
#define CLOCKPIN 9 // Purple Wire
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

int status = WL_IDLE_STATUS;
const char* ssid = "ENSEMBLE";
const char* pass = "Ensemble"; // my wifi name and password
const int localPort = 1234; // set the port number to listen on

char packetBuffer[256]; //buffer to hold incoming packet
//char  ReplyBuffer[] = "Y";       // a string to send back

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

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
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
    float freq = atof(packetBuffer);
    Serial.println("Contents:");
    Serial.print(packetBuffer);
    Serial.println(freq);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.print(freq);
    Udp.endPacket();

    // Color Mapping

    if (freq > 255) {
      freq = 255;
    }

    int r = 100 + freq*2; //red
    int b = 200 - freq; //blue
    int g = 50; //green

    strip.setBrightness(48);

    for (int i=0; i<NUMPIXELS; i++) {
      strip.setPixelColor(i, g, r, b);
      strip.show();
      delay(20);
    }

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

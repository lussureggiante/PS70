#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
const char* ssid = "MAKERSPACE";
const char* pass = "12345678";
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
    //int Freq = atoi(packetBuffer);

    char *part1;
    char *part2;

    part1 = strtok(packetBuffer, ",");
    part2 = strtok(NULL, ",");

    float Freq = atof(part1);
    float Gamma = atof(part2);

    Serial.println("Contents:");
    Serial.print(packetBuffer);
    Serial.println(Freq);
    Serial.println(Gamma);

    // if (Gamma == 50.00 || Gamma == -50.00), meaning no EEG signal received.

    // send a reply, to the IP address and port that sent us the packet we received
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

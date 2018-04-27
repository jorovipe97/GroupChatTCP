// According to docs this board have 4M of capacity in the file system 
// https://github.com/esp8266/Arduino/blob/master/doc/filesystem.rst

// Hello wifi
/*
Utils docs
https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/readme.rst
https://www.arduino.cc/en/Reference/WiFi
*/

// This library provides ESP8266 specific Wi-Fi routines we are calling to connect to network.
#include <ESP8266WiFi.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  // Connecting device to wifi network (just like a normal phone)
  // Wifi name and password
  WiFi.begin("UNE_HFC_5D50", "ADADC4F8");
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  /*
   * Shows the local IP assigned by the DHCP (The Networking Protocol That the Gives You an IP Address)
   * https://whatismyipaddress.com/dhcp
   * 
   * Is DHCP server the router or a computer in the cable company?
  */
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}

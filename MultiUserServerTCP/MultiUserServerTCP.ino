/* 
  WiFiTelnetToSerial - Example Transparent UART to Telnet Server for esp8266

  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the ESP8266WiFi library for Arduino environment.
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <ESP8266WiFi.h>

// UNE_HFC_5D50
// ADADC4F8
//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 5
const char* ssid = "UNE_HFC_5D50";
const char* password = "ADADC4F8";

WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

// String array for store incoming messages from clients
String clientMessages[MAX_SRV_CLIENTS];

void setup() {
  //start UART
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial1.println(ssid);
  uint8_t i = 0;

  
  
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if(i == 21){
    Serial1.print("Could not connect to"); Serial1.println(ssid);
    while(1) delay(500);
  }

  // Start server
  server.begin();
  server.setNoDelay(true);
  
  Serial.print("\nReady! Use 'telnet ");
  Serial.print(WiFi.localIP());
  Serial.println(":23' to connect");
}

void loop() {
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()){
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Serial.print("New client: "); Serial.print(i); Serial.println("");
        break;
      }
    }
    //no free/disconnected spot so reject
    if ( i == MAX_SRV_CLIENTS) {
       WiFiClient serverClient = server.available();
       serverClient.stop();
       Serial.println("Connection rejected ");
    }
  }
  //check clients for data
  for(i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      if(serverClients[i].available()){
        //get data from the telnet client and push it to the UART
        while(serverClients[i].available())
        {
          // Save infor of the user id
          clientMessages[i] = i + serverClients[i].readStringUntil('\n');
          Serial.print("User "); Serial.print(i); Serial.print(" Says: ");
          Serial.print(clientMessages[i]);
          // New line
          Serial.println("");

          for(int j = 0; j < MAX_SRV_CLIENTS; j++){
            serverClients[j].print(clientMessages[i]);
            delay(1);           
          }
        }
      }
    }
  }
  
  // check UART for data
  /*if(Serial.available()){
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    //push UART data to all connected telnet clients
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
        serverClients[i].write(sbuf, len);
        delay(1);
      }
    }
  }*/
}

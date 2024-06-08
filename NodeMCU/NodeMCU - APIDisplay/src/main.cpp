/*
 * HTTP Client GET Request
 * Copyright (c) 2018, circuits4you.com
 * All rights reserved.
 * https://circuits4you.com 
 * Connects to WiFi HotSpot. */

#include <ESP8266WiFi.h>
#include "SSD1306Wire.h" 
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <secrets.h>

/* Set these to your desired credentials. */
const char *ssid = SECRET_SSID;  //ENTER YOUR WIFI SETTINGS
const char *password = SECRET_PASS;

//Web/Server address to read/write from 
const char *host = "192.168.0.63";   //https://circuits4you.com website or IP address of server

// WIFI connection server
ESP8266WebServer server(80);

// WiFi AP credentials
const char* ap_ssid = "ESP8266-TEST";
const char* ap_password = "rfkfrwfkEAFKw";
String wifi_ssid = "";
String wifi_password = "";




//=======================================================================
//                    DISPLAY SETTINGS
//=======================================================================
SSD1306Wire display(0x3c, 14, 12);  // ADDRESS, SDA, SCL

//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  display.init();
  display.setFont(ArialMT_Plain_10);

  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  HTTPClient http;    //Declare object of class HTTPClient

  // String ADCData, station, getData, Link;
  // int adcvalue=analogRead(A0);  //Read Analog value of LDR
  // ADCData = String(adcvalue);   //String to interger conversion
  // station = "B";
  String Link;
  //GET Data
  // getData = "?status=" + ADCData + "&station=" + station ;  //Note "?" added at front
  WiFiClient client;

  // set port to 9000
  Link = "http://192.168.0.63:9000/";
  http.begin(client,Link);     //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload
  display.cls();
  display.clear();
  if(httpCode == 200){
    // update display
    display.clear();
    // parse new lines
    for (unsigned int i = 0; i < payload.length(); i++) {
      if (payload[i] == '\\' && payload[i+1] == 'n') {
        display.println();
        delay(1000);
        i++;
      } else {
        display.print(payload[i]);
        
      }
    }
  }
  else
  {
    display.clear();
    
    display.println("Error");
    display.println(httpCode);   //Print HTTP return code
  }
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection
  
  delay(5000);  //GET Data at every 5 seconds
  
}
//
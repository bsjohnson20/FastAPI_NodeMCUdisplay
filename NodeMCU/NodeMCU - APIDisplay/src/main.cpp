#include <ESP8266WiFi.h>
#include "SSD1306Wire.h" 
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <secrets.h>
#include <EEPROM.h>
// web server address
String host = "192.168.0.63"; // default


//=======================================================================
//                    WIFI SETTINGS
//=======================================================================

const char *ssid = SECRET_SSID;  //ENTER YOUR WIFI SETTINGS
const char *password = SECRET_PASS;

// WIFI connection server
ESP8266WebServer server(80);

// Access Point Credentials
const char *ap_ssid = "ESP8266";
const char *ap_password = "12345678";

// WiFi Credentials
String wifi_ssid = "";
String wifi_password = "";

//=======================================================================
//                    DISPLAY SETTINGS
//=======================================================================
SSD1306Wire display(0x3c, 14, 12);  // ADDRESS, SDA, SCL

//=======================================================================
//                    Power on setup
//=======================================================================

// WiFi AP credentialsQR
void mainSetup();
void startAPMode();
bool connectToWiFi();
void handleSubmit();

bool validCharacter(uint8 character){
  // check if it's a valid utf-8 char, if not return false
  if (character < 0x80) {
    return true;
  } else if ((character & 0xE0) == 0xC0) {
    return true;
  } else if ((character & 0xF0) == 0xE0) {
    return true;
  } else if ((character & 0xF8) == 0xF0) {
    return true;
  } else {
    return false;
  }
}

void setup() {
  Serial.begin(115200);
  display.init();
  display.setFont(ArialMT_Plain_10);
  // setup server
  // server.on("/", HTTP_GET, []() {
  //   server.send(200, "text/html", "<form action='/submit' method='post'>SSID: <input type='text' name='ssid'><br>Password: <input type='password' name='password'><br>api_addr: <input type='text' name='ipaddr'><br><input type='submit'></form>");
  // });
  // set the entries to load from EEPROM if they exist
  host = "";
  wifi_ssid = "";
  wifi_password = "";
  EEPROM.begin(512);
  // load from eeprom but check if it's a valid utf-8 character and if not then break
  for (unsigned int i = 0; i < 32; i++) {
    char character = EEPROM.read(i);
    if (validCharacter(character)) {
      wifi_ssid += character;
    } else {
      break;
    }
  }
  // password
  for (unsigned int i = 32; i < 64; i++) {
    char character = EEPROM.read(i);
    if (validCharacter(character)) {
      wifi_password += character;
    } else {
      break;
    }
  }
  // host
  for (unsigned int i = 64; i < 96; i++) {
    char character = EEPROM.read(i);
    if (validCharacter(character)) {
      host += character;
    } else {
      break;
    }
  }
  EEPROM.end();


  // setup server
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", "<form action='/submit' method='post'>SSID: <input type='text' name='ssid' value='" + wifi_ssid + "'><br>Password: <input type='password' name='password' value='" + wifi_password + "'><br>api_addr: <input type='text' name='ipaddr' value='" + host + "'><br><input type='submit'></form>");
  });
  server.on("/submit", HTTP_POST, handleSubmit);
  server.begin();
  
  // begin AP mode
  startAPMode();
  Serial.println("HTTP server started");
  

  // Serial.println("HTTP server started");
  // if (wifi_ssid != "" && wifi_password != "") {
  //   WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
  //   if (connectToWiFi()) {
  //     Serial.println("Connected to WiFi");
  //   } else {
  //     startAPMode();
  //   }
  // } else {
  //   startAPMode();
  // }
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
  // mainSetup();
  server.handleClient();
  // if after 30s the client has not sent a request, then try to connect to the wifi
  if (millis() % 30000 == 0) {
    if (WiFi.status() != WL_CONNECTED) {
      display.clear();
      display.drawString(0, 0, "Trying to connect to WiFi...");
      display.display();
      // print credentials
      Serial.println("Trying to connect to WiFi...");
      Serial.println("SSID: " + wifi_ssid);
      Serial.println("Password: " + wifi_password);

      if (connectToWiFi()){
        // display connected to wifi
        display.clear();
        display.drawString(0, 0, "Connected to WiFi");
        display.display();
        delay(1000);
        mainSetup();
      }
    }
  }
}

void startAPMode() {
  WiFi.softAP(ap_ssid, ap_password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // display password and ssid
  display.clear();
  display.drawString(0, 0, "SSID: " + String(ap_ssid));
  display.drawString(0, 10, "Password: " + String(ap_password));
  display.drawString(0, 20, "IP: " + IP.toString());
  display.drawString(0, 30, "Automatically trying in 30s...");
  display.display();
  delay(5000);
}

bool connectToWiFi() {
  // disconnect from AP
  WiFi.disconnect();
  // change wifi mode to station
  WiFi.mode(WIFI_STA);
  // connect to wifi
  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 15) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected!");
    // print ip
    Serial.println("IP ADDR: "+WiFi.localIP().toString());
    return true;
  } else {
    Serial.println("Failed to connect.");
    // ap mode
    startAPMode();
    return false;
  }
}

void handleSubmit() {
  wifi_ssid = server.arg("ssid");
  wifi_password = server.arg("password");
  server.send(200, "text/html", "Received. Trying to connect...");
  Serial.println("Connecting now with the following configs:");
  Serial.println(wifi_ssid.c_str());
  Serial.println(wifi_password.c_str());

  // send back to the client
  server.send(200, "text/html", "Received. Connecting now...\nNotice: The AP will be destroyed, device will attempt to connect to the wifi.\nIf it fails, the AP will be started again.\n\nSo in a way: this is my last message. Goodbye.");

  

  // check if wifi ssid or password or api_addr is empty then return
  if (wifi_ssid == "" || wifi_password == "" || host == "") {
    Serial.println("Empty fields");
    startAPMode();
    return;
  }
  // prepare wifi for connection
  WiFi.disconnect();

  // change wifi mode to station
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());

  // also fetch the IP address
  const char * x = "ipaddr";
  host = server.arg(x);
  if (connectToWiFi()) {
    Serial.println("Connected to WiFi");
    display.clear();
    display.drawString(0, 0, "Connected to WiFi");
    // ip address
    display.drawString(0, 10, "IP: " + WiFi.localIP().toString());
    display.display();

    // save to EEPROM
    EEPROM.begin(512);
    for (unsigned int i = 0; i < wifi_ssid.length(); i++) {
      EEPROM.write(i, wifi_ssid[i]);
    }

    for (unsigned int i = 0; i < wifi_password.length(); i++) {
      EEPROM.write(i + 32, wifi_password[i]);
    } 

    for (unsigned int i = 0; i < host.length(); i++) {
      EEPROM.write(i + 64, host[i]);
    }
    EEPROM.commit();
    EEPROM.end();


    delay(2000);
    // begin main setup
    mainSetup();
  } else {
    Serial.println("Failed to connect");
    startAPMode();
  }
}

void mainSetup(){
  HTTPClient http;    //Declare object of class HTTPClient
  String Link;
  //GET Data
  WiFiClient client;
  // set port to 9000
  Link = "http://" + String(host) + ":9000"; //Your Domain name with URL path or IP address with path
  // while wifi is connected
  display.cls(); display.clear();
  display.println("Beginning API mode");
  Serial.println("Beginning API mode");
  while (WiFi.status() == WL_CONNECTED) {
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
    // host web server in the background
    server.handleClient();
    delay(5000);  //GET Data at every 5 seconds
  }
}
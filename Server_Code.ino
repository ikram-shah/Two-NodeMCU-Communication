#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#define DHTPIN D1          //pin where the dht11 is connected
 
DHT dht(DHTPIN, DHT11);

const char* ssid = "ESP_D54736";  // SSID of esp8266
//const char* password = "123";   //
bool toggle=0;                  //Variable to switch on and off the solenoid
ESP8266WebServer server(80);    //Specify port for TCP connection

void handleRoot() {
  toggle=!toggle;               //Toggling Led Variable
    digitalWrite(2,toggle);     //Toggle Led
    float h = dht.readHumidity();
  float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
        String s = "";
        s += "Success!!!\n";
        s += "Temprature : ";
        s += t;
        s += " degree Celsius. \nHumidity : ";
        s += h;
        server.send(200,"text/html",s);      //Reply to the client
}


void setup() {
  delay(200);                           //Stable Wifi
  Serial.begin(115200);                 //Set Baud Rate
  pinMode(2, OUTPUT);                   //Led/Solenoid at pin 2
  WiFi.softAP(ssid);//, password);      //In Access Point Mode

  IPAddress myIP = WiFi.softAPIP();     //Check the IP assigned. Put this Ip in the client host.
  Serial.print("AP IP address: ");
  Serial.println(myIP);                 //Print the esp8266-01 IP(Client must also be on the save IP series)
  server.on("/Led", handleRoot);           //Checking client connection
  server.begin();                       // Start the server
  Serial.println("Server started");
}

void loop() {             
  server.handleClient();
}


//https://github.com/ubidots/ubidots-mqtt-esp
//Go to the Arduino IDE, click on Files -> Preferences and enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas
//Open Boards Manager from Tools -> Board menu and install esp8266 platform (and don’t forget to select your ESP8266 board from Tools > Board menu after installation)
//Download this library as .zip
//Now, click on Sketch -> Include Library -> Add .ZIP Library
//Select the .ZIP file that you have just downloaded and then “Accept” or “Choose”
//Go to Sketch/Program -> Include Library -> Library Manager and install the PubSubClient library
//Close the Arduino IDE and open it again.
//If you are using windows, please install the appropiate driver for your ESP board (CH340G if you are using a LoLin board or CP2102 if you are using an AMICA board)
//download and install the required library from Github link
#include "UbidotsESPMQTT.h"
 
/****************************************
   Define Constants
 ****************************************/
#define TOKEN "BBFF-UMz6hNj9lHFHX1tcQEWbP4Cq5FF4sU" // Your Ubidots TOKEN
#define WIFINAME "dewa406" //Your SSID
#define WIFIPASS "Ramesh16384" // Your Wifi Pass
#define DEVICE_LABEL  "load_management"  // Put here your Ubidots device label
#define VARIABLE1  "light1"  // Put here your Ubidots variable label
#define VARIABLE2  "light2"  // Put here your Ubidots variable label
#define VARIABLE3  "light3"  // Put here your Ubidots variable label
#define VARIABLE4  "light4"  // Put here your Ubidots variable label
 
int load1 = D0; 
int load2 = D1; 
int load3 = D2; 
int load4 = D3;
 
int vresistor = A0; 
float value=0; // To store incoming value.
 
Ubidots client(TOKEN);
 
/****************************************
   Auxiliar Functions
 ****************************************/
 
// cast from an array of chars to float value.
float btof(byte * payload, unsigned int length) {
  char * demo = (char *) malloc(sizeof(char) * 10);
  for (int i = 0; i < length; i++) {
    demo[i] = payload[i];
  }
  float value = atof(demo);
  free(demo);
  return value;
}
 
// Callback to handle subscription
 
void callback(char* topic, byte* payload, unsigned int length) {
  value = btof(payload, length);
  Serial.println(value);
 
if (value == 0.00)
{
  Serial.println("Light1 turned off"); 
  digitalWrite(load1, LOW); 
  client.add("light1",0);
 
}
 
if (value == 1.00)
{
  Serial.println("Light1 turned ON"); 
  digitalWrite(load1, HIGH); 
    client.add("light1",1);
 
}
 
 
if (value == 2.00)
{
  Serial.println("Light2 turned off"); 
  digitalWrite(load2, LOW); 
  
}
 
if (value == 3.00)
{
  Serial.println("Light2 turned ON"); 
  digitalWrite(load2, HIGH); 
}
 
 
 
if (value == 4.00)
{
  Serial.println("Light3 turned off"); 
  digitalWrite(load3, LOW); 
}
 
if (value == 5.00)
{
  Serial.println("Light3 turned ON"); 
  digitalWrite(load3, HIGH); 
  
}
 
 
 
if (value == 6.00)
{
  Serial.println("Light4 turned off"); 
  digitalWrite(load4, LOW); 
}
 
if (value == 7.00)
{
  Serial.println("Light4 turned ON"); 
  digitalWrite(load4, HIGH); 
}
   
}
 
/****************************************
   Main Functions
 ****************************************/
 
void setup() {
  // put your setup code here, to run once:
  client.ubidotsSetBroker("industrial.api.ubidots.com"); // Sets the broker properly for the business account
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  Serial.begin(115200);
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE1); //Insert the dataSource and Variable's Labels
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE2); //Insert the dataSource and Variable's Labels
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE3); //Insert the dataSource and Variable's Labels
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE4); //Insert the dataSource and Variable's Labels
 
 pinMode(load1, OUTPUT); 
 pinMode(load2, OUTPUT); 
 pinMode(load3, OUTPUT); 
 pinMode(load4, OUTPUT); 
 
pinMode(vresistor, INPUT); 
 
}
 
void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    client.reconnect();
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE1); //Insert the dataSource and Variable's Labels
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE2); //Insert the dataSource and Variable's Labels
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE3); //Insert the dataSource and Variable's Labels
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE4); //Insert the dataSource and Variable's Labels
 
   client.ubidotsPublish("Monitoring");
  }
 
int data = analogRead(vresistor); 
  client.add("vresistor",data); 
  client.ubidotsPublish("Monitoring"); 
  
  client.loop();
}

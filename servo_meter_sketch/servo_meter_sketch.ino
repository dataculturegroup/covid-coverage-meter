#include <WiFi.h>
#include <Servo.h>

#include "secrets.h"

boolean TRY_WIFI = false;

#define DATA_SIZE 90

int coverageData[DATA_SIZE] = {11,13,13,12,25,34,60,74,76,70,68,68,65,66,63,65,62,53,44,45,45,48,47,48,47,49,50,50,43,42,40,39,37,35,34,39,41,36,35,35,32,35,38,37,38,39,39,40,39,30,31,34,35,33,30,31,31,33,34,31,30,31,29,27,24,27,26,25,24,24,24,22,22,21,21,21,24,26,27,29,28,25,26,24,24,23,23,22,21,21};

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

Servo myservo;  // create servo object to control a servo
int potpin = A0;  // analog pin used to connect the potentiometer

void setup() {
  int attempts = 0;
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial);

  if(TRY_WIFI) {
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
      Serial.print("Attempting to connect to network (attempt ");
      Serial.print(attempts);
      Serial.print("): ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network:
      status = WiFi.begin(ssid, pass);
  
      // wait 10 seconds for connection:
      delay(10000);
      attempts += 1;
    }
    // you're connected now, so print out the data:
    Serial.println("You're connected to the network");
    Serial.println("----------------------------------------");
    printWifiData();
    Serial.println("----------------------------------------");
  }
}

int potPositionToDataIndex(int input) {
  // input 1023-0, output 100-0
  float rangeWindow = (DATA_SIZE-1) / 7;
  float ratio;
  int index; // the output
  if (input > 1010) {
    index = DATA_SIZE-1;
  } else if (input > 981) {
    index = map(input, 981, 1023, rangeWindow*6, max(DATA_SIZE-1, rangeWindow*7));
  } else if (input > 776) {
    index = map(input, 776, 981, rangeWindow*5, rangeWindow*6);
  } else if (input > 612) {
    index = map(input, 612, 776, rangeWindow*4, rangeWindow*5);
  } else if (input > 443) {
    index = map(input, 443, 612, rangeWindow*3, rangeWindow*4);
  } else if (input > 275) {
    index = map(input, 275, 443, rangeWindow*2, rangeWindow*3);
  } else if (input > 75) {
    index = map(input, 75, 275, rangeWindow*1, rangeWindow*2);
  } else {
    index = map(input, 0, 75, 0, rangeWindow);
  }
  return index;
}

int lastSensorValue = 0;
int sensorValue;
int dataIndex;
int dataValue;
int servoPosition;

void loop() {
  // check the network connection once every 10 seconds:
  // delay(10000);
  // printData();
  // Serial.println("----------------------------------------");

  // check the pot to see what date they want
  sensorValue = analogRead(potpin);
  if (abs(sensorValue - lastSensorValue) > 10) { // sensor is noisy, so use a dinwo
    Serial.println("-------------------------------------------------------");

    Serial.print("Sensor: ");
    Serial.println(sensorValue);
  
    // translate the pot position into an index in to the data array
    dataIndex = potPositionToDataIndex(sensorValue);
    Serial.print("Data Index: ");
    Serial.println(dataIndex);
  
    // figure out the current selected data based on the index
    dataValue = coverageData[dataIndex];
    Serial.print("Data Value: ");
    Serial.println(dataValue);
  
    // map from selected data value to servo position
    servoPosition = map(dataValue, 0, 100, 0, 180);
    myservo.write(servoPosition);
    Serial.print("Servo Position: ");
    Serial.println(servoPosition);
    
    lastSensorValue = sensorValue;
    delay(100); // waits for the servo to get there
  }
}

void printWifiData() {
  Serial.println("Board Information:");
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println();
  Serial.println("Network Information:");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
  
}

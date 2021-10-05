#include <WiFi.h>
#include <Servo.h>

#include "secrets.h"

boolean TRY_WIFI = false;

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

int scale(int input) {
  // input 1023-0, output 180-0
  float rangeWindow = 180 / 7;
  float ratio;
  int range; // the output
  if (input > 981) {
    range = map(input, 981, 1023, 154, 180);
  } else if (input > 776) {
    range = map(input, 776, 981, 128, 154);
  } else if (input > 612) {
    range = map(input, 612, 776, 102, 128);
  } else if (input > 443) {
    range = map(input, 443, 612, 77, 102);
  } else if (input > 275) {
    range = map(input, 275, 443, 51, 77);    
  } else if (input > 75) {
    range = map(input, 75, 275, 25, 50);
  } else {
    range = map(input, 0, 75, 0, 25);
  }
  return range;
}

int val;

void loop() {
  // check the network connection once every 10 seconds:
  // delay(10000);
  // printData();
  // Serial.println("----------------------------------------");
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  Serial.print("Sensor: ");
  Serial.print(val);
  //val = map(val, 0, 1023, 0, 180);     // scale it for use with the servo (value between 0 and 180)
  val = scale(val);
  myservo.write(val);                  // sets the servo position according to the scaled value
  Serial.print(" -> ");
  Serial.println(val);
  delay(1000);                           // waits for the servo to get there
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

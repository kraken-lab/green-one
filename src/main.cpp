#include "main.h"
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <dht.h>
#include "commands.h"

#define DHT22_PIN 5
#define SERIAL_SPEED 9600

dht DHT;

// Rx and Tx pins
const int _serialRx = 2;
const int _serialTx = 3;

SoftwareSerial esp(_serialRx, _serialTx);

const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;

String prepareAnswer(String command){
  if (command.equals(CMD_TEMP)) {
    return String(DHT.temperature);
  } else if (command.equals(CMD_HUM)) {
    return String(DHT.humidity);
  } else if (command.equals(CMD_SOIL_1)) {
    /* code */
  } else {
   return "error: no valid command found";
  }
}

String addDelimiter(String input) {
  return input+":";
}

String addStartEndMarkers(String input) {
  return "<"+input+">";
}

void checkEspCommands() {

    String receivedString = receivedChars;
    Serial.println("command: "+receivedString);

    String answer = prepareAnswer(receivedString);
    Serial.println("answer: "+answer);

    sendWithStartEndMarkers(answer);
}

void sendWithStartEndMarkers(String data) {
  data = addStartEndMarkers(data);
  esp.write(data.c_str());
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    
    Serial.println("reading one data chunk from buffer (within < and >)");
    Serial.println();
    while (esp.available() > 0 ) {
        rc = esp.read();
        Serial.print(rc);

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
                //Serial.println("reached endmarker");
                break;
            }
        }
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
    Serial.println();
}

void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        newData = false;
    }
}

void setup()
{
  Serial.begin(SERIAL_SPEED);
  
  Serial.println("GREEN ONE");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
  
  esp.begin(SERIAL_SPEED);
  while(!esp) {
    Serial.println("esp software-serial not available");
    esp.begin(SERIAL_SPEED);
  }
  esp.listen();
}

void loop()
{
  // READ DATA
  Serial.print("DHT22, \t");
  int chk = DHT.read22(DHT22_PIN);

  switch (chk)
  {
    case DHTLIB_OK:  
		Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.print("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.print("Time out error,\t"); 
		break;
    default: 
		Serial.print("Unknown error,\t"); 
		break;
  }
  // DISPLAY DATA
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t\t");
  Serial.println(DHT.temperature, 1);

  delay(1000);

  while (esp.available() != 0) {
    recvWithStartEndMarkers();
    checkEspCommands();
  }
}
//
// END OF FILE
//

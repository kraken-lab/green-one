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

SoftwareSerial esp = SoftwareSerial(_serialRx, _serialTx);

String prepareAnswer(String command){
  if (command==CMD_TEMP)
  {
    return addDelimiter(String(DHT.temperature));
  } else if (command == CMD_HUM)
  {
    return addDelimiter(String(DHT.humidity));
  } else if (command == CMD_SOIL_1)
  {
    /* code */
  }
}

String addDelimiter(String input) {
  return input+":";
}

void checkEspCommands() {
  if (esp.available() && esp.peek())
  {
    String command = esp.readString();

    String answer = prepareAnswer(command);

    esp.write(answer.c_str());

  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
  esp.begin(SERIAL_SPEED);
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

  delay(2000);

  checkEspCommands();
  
}
//
// END OF FILE
//

#include <Arduino.h>

String prepareAnswer(String command);
String addDelimiter(String input);
String addStartEndMarkers(String input);
void checkEspCommands();
void sendWithStartEndMarkers(String data);
void recvWithStartEndMarkers();
void showNewData();

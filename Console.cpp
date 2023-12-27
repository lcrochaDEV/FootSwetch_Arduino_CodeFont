#include "Console.h"

//MENSAGEM DE TODO O PROGRAMA
Console::Console(String consoleText = ""){
    this->consoleTextView = consoleText;
}
void Console::elementName(String consoleText = "ARDUINO"){
  Serial.println(consoleText);
}
void Console::helloWord(String consoleText = "Hello Word"){
  messageViewMsg(consoleText);
}
void Console::messageViewMsg(String consoleText){
  Serial.println(consoleText);
}

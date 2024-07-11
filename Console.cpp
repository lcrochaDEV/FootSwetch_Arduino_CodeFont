#include "Console.h"

//MENSAGEM DE TODO O PROGRAMA
Console::Console(String consoleText = ""){
    this->consoleTextView = consoleText;
}
void Console::elementName(String consoleText = "ARDUINO"){
  Serial.println(consoleText);
}
void Console::helloWord(String consoleText = "Hello Word"){
  menssageViewMsg(consoleText);
}
void Console::menssageViewMsg(String consoleText){
  Serial.println(consoleText);
}
void Console::consoleView(){
  if (Serial.available() > 0) {
    String consoleText = Serial.readString();
    consoleText.trim();
    if (consoleText.length() > 0) {
      consoleText.toUpperCase();
      menssageViewMsg(consoleText);
    }
  }
}
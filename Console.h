#ifndef CONSOLE_H
#define CONSOLE_H
 
#include <Arduino.h>

class Console {
  public:
    Console(String consoleText = "");
    //MENSAGEM DE TODO O PROGRAMA
    void elementName(String consoleText = "ARDUINO");
    void helloWord(String consoleText  = "Hello Word");
    void messageViewMsg(String consoleText);
  private:
    String consoleTextView;
};
 
#endif
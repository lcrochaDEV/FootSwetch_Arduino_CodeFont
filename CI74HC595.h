#ifndef CI74HC595_H
#define CI74HC595_H
 
#include <Arduino.h>

class CI74HC595 {
  public:
    CI74HC595(int dataPin, int clockPin, int latchPin);
    int Data();
    int Clock();
    int Latch();
    void toggle(int posicao, int pin_ci);
    void updateShift();
    void bits_ci(int On_Off);
  private:
    int dataPin, clockPin, latchPin;
};
 
#endif
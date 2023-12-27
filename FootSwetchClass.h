#ifndef FOOTSWETCH_H
#define FOOTSWETCH_H
 
#include <Arduino.h>

class FootSwetch {
  public:
    FootSwetch(int pin_e = NULL, int pin_l = NULL, int btns = NULL, int ledId = NULL, String mode = "");
    void testesLed();
    void pin_4Action();
    void piscaSpeed();
  private:
    int pin_e, pin_l, btns, ledId;
    String mode;
    void modLoopEditeSave(int state_e, int state_l, bool state_bit = false);
    void confirmeLed(int blinks);
    void modeMenu(int _id = NULL);
    void modeId(int _id = NULL, bool actions = false);
};

#endif
#ifndef FOOTSWETCH_H
#define FOOTSWETCH_H
 
#include <Arduino.h>

class FootSwetch {
  public:
    FootSwetch(int mode_edit = NULL, int mode_loop = NULL, int btns[] = NULL);
    void testesLed();
    void pinAction(int btnId = NULL, int ledId = NULL, int pinMode = NULL);
    void confirmAction();
  private:
    int mode_edit, mode_loop, *btns;
    String mode;
    void modLoopEditeSave(int state_e, int state_l, bool state_bit = false);
    void confirmeLed(int blinks);
    void modeMenu(int _id = NULL, int pinMode = NULL);
    void modeId(int _id = NULL, bool actions = false);
};

#endif
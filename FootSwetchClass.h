#ifndef FOOTSWETCH_H
#define FOOTSWETCH_H

#include <Arduino.h>

class FootSwetch {
  public:
    FootSwetch(int mode_edit = NULL, int mode_loop = NULL);
    void incialTestLed();
    void pinAction(int btnId = NULL, int ledId = NULL, int pinMode = NULL);
    void confirmAction(int _id = NULL, int ledId = NULL);
  private:
    int mode_edit, mode_loop;
    String mode;
    void confirmeLed(int blinks);
    void modLoopEditeSave(int state_e, int state_l, bool state_bit = false);
    void modeMenu(int _id = NULL, int ledId = NULL, int pinMode = NULL);
    void modeId(int _id = NULL, bool actions = false);
};

#endif
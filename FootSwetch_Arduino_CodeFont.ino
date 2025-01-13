/**************************LUCAS ROCHA****************************/
/*
    Portas Analogicas A = 1,2,3,4,5,6,7.
    Portas Analogicas/Digitais 14, 15, 16, 17, 18, 19, 20.
    Portas Digitais D = 2,3,4,5PWM,6PWM,7,8,9PWM,10PWM,11PWM,12.
    Data 12/09/2023
    VERSÃO 3 CI74HC595 OOP (Object-Oriented Programming)

    SKETCH ELABORADA PARA INTEGRANDO O CI74HC595 PARA ACIONAMENTO DOS LEDS DO PAINEL E RELAY.
    C:\Users\LuKInhas\AppData\Local\Temp\
*/
#include "Console.h"
#include "FootSwetchClass.h"
#include "CI74HC595.h"

#define btn_1 2 //PIN_D2
#define btn_2 3 //PIN_D3
#define btn_3 4 //PIN_D4
#define btn_4 5 //PIN_D5
#define btn_5 6 //PIN_D6

#define pin_p 7 //BTN COMUM
#define pin_e 8 //EDIT MODE
#define pin_l 9 //LOOP MODE
#define dataPin  10 // 14 74HC595
#define clockPin 11 // 11 74HC595
#define latchPin 12 // 12 74HC595

//PRESS TIME
long confirmTime;
#define tmpLongo 2000
#define tmpCurto 300

Console console;
CI74HC595 ctrl74hc595 = CI74HC595(dataPin, clockPin, latchPin);
FootSwetch footSwetch = FootSwetch(pin_e, pin_l);

void setup() {
  Serial.begin(9600);
  pinMode(btn_1, INPUT_PULLUP); //PIN_D2
  pinMode(btn_2, INPUT_PULLUP); //PIN_D3
  pinMode(btn_3, INPUT_PULLUP); //PIN_D4
  pinMode(btn_4, INPUT_PULLUP); //PIN_D5
  pinMode(btn_5, INPUT_PULLUP); //PIN_D6
  pinMode(pin_p, OUTPUT);       //PIN_D7
  pinMode(pin_e, OUTPUT);       //PIN_A0
  pinMode(pin_l, OUTPUT);       //PIN_A1
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  //CI74HC595
  int data = ctrl74hc595.Data();
  int clock = ctrl74hc595.Clock();
  int latch = ctrl74hc595.Latch();

  //CONSOLE
  console.elementName("FOOTSWETCH 5 PADS");
  console.helloWord("O Modulo Iniciou com Sucesso...");
  //FOOTSWETCH
  footSwetch.incialTestLed();
}

void loop() {
  if(digitalRead(btn_1) == LOW) {
    footSwetch.pinAction(btn_1, 0, pin_e);
  }
  if(digitalRead(btn_2) == LOW) {
    footSwetch.pinAction(btn_2, 1, pin_l);
  }
  if(digitalRead(btn_3) == LOW) {
    footSwetch.pinAction(btn_3, 2);
  }
  if(digitalRead(btn_4) == LOW) {
    footSwetch.pinAction(btn_4, 3);
  }
  if(digitalRead(btn_5) == LOW) {
    footSwetch.pinAction(btn_5, 4);
  }
  
  if(digitalRead(pin_e) == LOW && digitalRead(pin_l) == LOW){
    footSwetch.pinAction();
  }
}
//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

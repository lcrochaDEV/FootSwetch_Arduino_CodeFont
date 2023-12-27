#include "Console.h"
#include "FootSwetchClass.h"
#include "CI74HC595.h"

byte ci74HC595[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

CI74HC595::CI74HC595(int dataPin, int clockPin, int latchPin){
  this->dataPin = dataPin;
  this->clockPin = clockPin;
  this->latchPin = latchPin;
}

int CI74HC595::Data(){
  return this->dataPin;
}
int CI74HC595::Clock(){
  return this->clockPin;
}
int CI74HC595::Latch(){
  return this->latchPin;
}

//==================================CONVERSOR DECIMAL PARA BINÁRIO ACIONAMENTO LED A LED RELAY================================//
/* POSIÇÃO vai de 1-8 */
void CI74HC595::toggle(int posicao, int pin_ci) { //INTERAÇÃO ENVIO DE UM BIT
  //SELECIONA UM LED POR VEZ NO LOOP MOD
  for (int ci = 10; ci <= 16; ci++){
   ci74HC595[ci] == 1 ? ci74HC595[ci] = 0 : ci74HC595[pin_ci] = 1;
  }
    //SELECIONDA OS LEDS RALAY EM MOD EDIT
   ci74HC595[posicao - 1] == 1 ? ci74HC595[posicao - 1] = 0 : ci74HC595[posicao - 1] = 1;
   updateShift();
}

void CI74HC595::updateShift() {
  for (int j = 1; j <= 2; j++) {
    digitalWrite(this->latchPin, LOW); // baixa o portao para montar o BYTE
    for (int i = 0; i <= 7; i++) {
      digitalWrite(this->clockPin, LOW); // Avisa o Clock que vai ser escrito um BIT
      int posicao = i;
      j == 2 ? posicao = i + 8 : posicao = i;
      digitalWrite(this->dataPin, ci74HC595[posicao]); // Escreve o bit (BINARIO)
      digitalWrite(this->clockPin, HIGH); // Avisa o Clock que foi escrito um BIT
    }
    digitalWrite(this->latchPin, HIGH); // Avisa o Portão que terminamos de montar o BYTE, e ele já manda para o Shift
  }
}
//==================================CONVERSOR DECIMAL PARA BINÁRIO ACIONAMENTO DESLIGANDO TODOS OS LEDs================================//
void CI74HC595::bits_ci(int On_Off) { //INTERAÇÃO DE ENVIO DE UM BYTE COMPLETO
    digitalWrite(this->latchPin, LOW);
    digitalWrite(this->clockPin, LOW);
    shiftOut(this->dataPin, this->clockPin, LSBFIRST, On_Off >> 8); //0b01111000 = 0x7C
    shiftOut(this->dataPin, this->clockPin, LSBFIRST, On_Off);
    digitalWrite(this->latchPin, HIGH);
    digitalWrite(this->clockPin, HIGH);
}
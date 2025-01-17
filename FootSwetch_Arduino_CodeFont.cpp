#include "HardwareSerial.h"
#include "Arduino.h"
#include "Console.h"
#include "FootSwetchClass.h"
#include "CI74HC595.h"

extern Console console;
extern CI74HC595 ctrl74hc595;

//PRESS TIME
long tmpInicio;
#define tmpLongo 2000
#define tmpCurto 300

//MEMORY
//int buffermMemory[];
int bin[] = {0, 0, 0, 0, 0, 0, 0, 0};

//PINOS QUE SE REFERE AO PINOS DO CI74HC595 RELAY
int relaysArray[] = {3, 4, 5, 6, 7};

//PINOS QUE SE REFERE AO PINOS DO CI74HC595 LOOP
int ledsArray[] = {10, 11, 12, 13, 14};

// Obtenha o número de elementos em um determinado array
template <class T, size_t N> constexpr size_t len(const T (&) [N]) { return N; }

FootSwetch::FootSwetch(int mode_edit = NULL, int mode_loop = NULL){
  this->mode_edit = mode_edit;
  this->mode_loop = mode_loop;
}
//===================================INICIALIZAÇÃO E TESTES LEDS================================//
void FootSwetch::incialTestLed() { //AÇÃO REALIZADA APÓS LIGAR FOOTROUTER
  int contador = 0;
  while (contador < 2) {
     //LEDS DOS RELAYs   
    for (int r = 3; r < 8; r++) {
          ctrl74hc595.toggle(r, -1); 
          delay(50); 
      //LEDS DOS LEDS     
      for (int l = 11; l <= 15; l++) {
        ctrl74hc595.toggle(l, -1); 
        delay(50);
      }
    }
    contador++;
  }
  modLoopEditeSave(LOW, HIGH); // MODE LOOP
}
void FootSwetch::pinAction(int btnId = NULL, int ledId = NULL, int pinMode = NULL){
//**************QUADRUPLA FUNÇÃO**************BTN1//
  // BOTÃO EDIT MOD
  tmpInicio = digitalRead(btnId); //le o estado do botão - HIGH OU LOW
  if (digitalRead(btnId) == LOW) {
    tmpInicio = millis();
    while (millis() - tmpInicio < tmpLongo && digitalRead(btnId) == LOW);
    //TEMPO CURTO
    if ((millis() - tmpInicio < tmpCurto)){ //CLICK E ACIOMA O LED1 SE O EDIT ESTIVER LOW
      if(digitalRead(this->mode_loop) == HIGH && digitalRead(this->mode_edit) == LOW){
          modeId(ledId);
      }else if (digitalRead(this->mode_edit) == HIGH && digitalRead(this->mode_loop) == LOW){ //CLICK E ACIOMA O LED1 SE O EDIT ESTIVER HIGH
          modeId(ledId, true);
      }
      while (digitalRead(btnId) == LOW);
    //TEMPO LONGO
    }else if (millis() - tmpInicio >= tmpLongo){
      //PRESS + 2s ACIOMA MOD EDIT || LOOP
      if(digitalRead(this->mode_loop) == HIGH || digitalRead(this->mode_edit) == HIGH){
        modeMenu(NULL, ledId, pinMode);
      }else if(digitalRead(this->mode_loop) == LOW && digitalRead(this->mode_edit) == LOW){
        modeMenu(btnId, ledId);
      }
      while (digitalRead(btnId) == LOW);
    }else {              
      //PRESS < 2s E ACIOMA O LOOP MOD SEM SALVAR 
        modeMenu(NULL, ledId, -1);
    }
    while (digitalRead(btnId) == LOW);
  }
  modeMenu();
}
void FootSwetch::modeMenu(int _id = NULL, int ledId = NULL, int pinMode = NULL){
  //PRESS + 2s E ACIOMA O EDIT MOD E APAGA OS LED D1 - D5
  if(digitalRead(this->mode_loop) == HIGH && this->mode_edit == pinMode){
    modLoopEditeSave(HIGH, LOW, 1); // APAGA TODOS OS LEDs DO PAINEL
    console.menssageViewMsg("PRESS EDIT MODE");
  }else if(digitalRead(this->mode_edit) == HIGH && this->mode_loop == pinMode){
    modLoopEditeSave(LOW, LOW); // APAGA OS LED DE LOOP E EDITE
    console.menssageViewMsg("PRESS SAVE MODE");
  }else if(digitalRead(this->mode_edit) == HIGH && pinMode == -1){
    modLoopEditeSave(LOW, HIGH); // RETORNO SEM SALVAR
    console.menssageViewMsg("EXIT MODE");
  }else if(digitalRead(this->mode_loop) == LOW && digitalRead(this->mode_edit) == LOW){
    confirmAction(_id, ledId);//ACIONA LOOP DE CONFIRMAÇÃO
    console.menssageViewMsg("LOOP");
  }
}
void FootSwetch::modLoopEditeSave(int state_e, int state_l, int state_bit = 0){
  digitalWrite(this->mode_edit, state_e);
  digitalWrite(this->mode_loop, state_l);
  delay(400);
  if(state_bit == 1){
    ctrl74hc595.bits_ci(0x00);
  }else if(state_bit == 0){
    ctrl74hc595.bits_ci(0x08);
  }
}
void FootSwetch::confirmAction(int _id = NULL, int ledId = NULL){ //AGUARDA CONFIRMAÇÃO
  if(_id != NULL){
    Serial.println("CONFIRMACAO");
    confirmeLed(ledsArray[ledId]); //COMFIRMEDE ACTION
    modLoopEditeSave(LOW, HIGH, -1);   //RETORNA AO MODO LOOP
  }else{
    ctrl74hc595.bits_ci(0xF8);
    delay(100);
    ctrl74hc595.bits_ci(0x00);
    delay(100);
  }
}
void FootSwetch::confirmeLed(int blinks){ //CONFIRMAÇÃO
  for (int i = 0; i <= 4; i++){ 
    ctrl74hc595.bits_ci(0x00); 
    delay(100);
    ctrl74hc595.toggle(-1, blinks); 
    delay(100);
  }
}
void FootSwetch::modeId(int _id, bool actions = false){
  if(actions == false){
    ctrl74hc595.toggle(-1, ledsArray[_id]);
  }else if(actions == true){
    ctrl74hc595.toggle(relaysArray[_id], -1);
    bin[_id] == 1 ? bin[_id] = 0 : bin[_id] = 1;
  }
}
void FootSwetch::saveMemory(){
  
}
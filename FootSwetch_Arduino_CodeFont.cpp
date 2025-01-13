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
  for (int i = 0; i < 2; i++) {
     //LEDS DOS RELAYs     
     for (int r = 3; r < 8; r++) {
        ctrl74hc595.toggle(r, NULL); 
        delay(50); 
     //LEDS DOS LEDS     
     for (int l = 11; l <= 15; l++) {
        ctrl74hc595.toggle(l, NULL); 
        delay(50);
      }
    }    
  } 
  digitalWrite(this->mode_edit, HIGH);
  digitalWrite(this->mode_loop, HIGH);  //INICIA EM LOOP MODE
  delay(400);
  ctrl74hc595.toggle(NULL, ledsArray[0]); 
  digitalWrite(this->mode_edit, LOW);
}
//(int btnId = NULL, int ledId = NULL, String mode = "");
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
    modLoopEditeSave(HIGH, LOW, true); // APAGA TODOS OS LEDs DO PAINEL
    console.menssageViewMsg("PRESS EDIT MODE");
  }else if(digitalRead(this->mode_edit) == HIGH && this->mode_loop == pinMode){
    modLoopEditeSave(LOW, LOW); // APAGA OS LED DE LOOP E EDITE
    console.menssageViewMsg("PRESS SAVE MODE");
  }else if(digitalRead(this->mode_edit) == HIGH && pinMode == -1){
    modLoopEditeSave(LOW, HIGH);
    modeId(_id, ledId);
    console.menssageViewMsg("EXIT MODE");
  }else if(digitalRead(this->mode_loop) == LOW && digitalRead(this->mode_edit) == LOW){
    confirmAction(_id, ledId);//ACIONA LOOP DE CONFIRMAÇÃO
    console.menssageViewMsg("LOOP");
  }
}
void FootSwetch::modLoopEditeSave(int state_e, int state_l, bool state_bit = false){
  digitalWrite(this->mode_edit, state_e);
  digitalWrite(this->mode_loop, state_l);
  if(state_bit){
    ctrl74hc595.bits_ci(0x00);
  }
}
void FootSwetch::confirmAction(int _id = NULL, int ledId = NULL){ //AGUARDA CONFIRMAÇÃO
  if(_id != NULL){
    Serial.println("CONFIRMACAO");
    confirmeLed(ledsArray[ledId]); //COMFIRMEDE ACTION
    modLoopEditeSave(LOW, HIGH);   //RETORNA AO MODO LOOP
  }else{
    ctrl74hc595.bits_ci(0xFE);
    delay(100);
    ctrl74hc595.bits_ci(0x00);
    delay(100);
  }
}
void FootSwetch::confirmeLed(int blinks){ //CONFIRMAÇÃO
  for (int i = 0; i <= 4; i++){ 
    ctrl74hc595.toggle(NULL, 0); 
    delay(100);
    ctrl74hc595.toggle(NULL, blinks); 
    delay(100);
  }
}
void FootSwetch::modeId(int _id, bool actions = false){
  if(actions == false){
    ctrl74hc595.toggle(NULL, ledsArray[_id]);
  }else if(actions == true){
    ctrl74hc595.toggle(relaysArray[_id], NULL);
  }
}
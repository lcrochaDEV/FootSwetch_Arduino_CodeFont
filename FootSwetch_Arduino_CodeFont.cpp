#include "Console.h"
#include "FootSwetchClass.h"
#include "CI74HC595.h"

extern CI74HC595 ctrl74hc595;

//PRESS TIME
long tmpInicio;
#define tmpLongo 2000
#define tmpCurto 300

//PINOS QUE SE REFERE AO PINOS DO CI74HC595 RELAY
int relaysArray[] = {3, 4, 5, 6, 7};

//PINOS QUE SE REFERE AO PINOS DO CI74HC595 LOOP
int ledsArray[] = {10, 11, 12, 13, 14};

FootSwetch::FootSwetch(int pin_e = NULL, int pin_l = NULL, int btns = NULL, int ledId = NULL, String mode = ""){
  this->pin_e = pin_e;
  this->pin_l = pin_l;
  this->btns = btns;
  this->ledId = ledId;
  this->mode = mode;
}

//===================================INICIALIZAÇÃO E TESTES LEDS================================//
void FootSwetch::testesLed() { //ACÃO REALIZADA APÓS LIGAR FOOTROUTER
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
    digitalWrite(this->pin_e, HIGH);
    digitalWrite(this->pin_l, HIGH);  //INCIA EM LOOP MODE
    delay(400);
    ctrl74hc595.toggle(NULL, ledsArray[0]); 
    digitalWrite(this->pin_e, LOW);
}

void FootSwetch::modLoopEditeSave(int state_e, int state_l, bool state_bit = false){
  digitalWrite(this->pin_e, state_e);
  digitalWrite(this->pin_l, state_l);
  if(state_bit){
    ctrl74hc595.bits_ci(0x00);
  }
}

void FootSwetch::confirmeLed(int blinks){
  for (int i = 0; i <= 4; i++){ 
    ctrl74hc595.toggle(NULL, 0); 
    delay(100);
    ctrl74hc595.toggle(NULL, blinks); 
    delay(100); 
  }
}

void FootSwetch::confirmAction(){
  int cont = 0;
  while (cont == 0){
    ctrl74hc595.bits_ci(0xFE);
    delay(100);
    ctrl74hc595.bits_ci(0x00);
    delay(100);
    tmpInicio = digitalRead(this->btns); //le o estado do botão - HIGH OU LOW
    if (digitalRead(this->btns) == LOW) {
      tmpInicio = millis();
      while ((millis() - tmpInicio < tmpLongo) && (digitalRead(this->btns) == LOW));
        if ((millis() - tmpInicio >= tmpLongo) && (digitalRead(pin_e) == LOW) && (digitalRead(pin_l) == LOW)){     
        //PRESS 2s SALVAR BD
          modeMenu(this->ledId, true);
          break;
          while (digitalRead(this->btns) == LOW);
        }else if (millis() - tmpInicio < tmpCurto) {
          continue;
        } else {                                          
          //PRESS -2s E ACIOMA O LOOP MOD SEM SALVAR 
          modeMenu(this->ledId);
          break;
          while (digitalRead(this->btns) == LOW);
        }
    }
    //cont ++;
  }
}

void FootSwetch::pinAction(){
//**************QUADRUPLA FUNÇÃO**************BTN1//
// BOTÃO EDIT MOD
  tmpInicio = digitalRead(this->btns); //le o estado do botão - HIGH OU LOW
  if (digitalRead(this->btns) == LOW) {
    tmpInicio = millis();
    while ((millis() - tmpInicio < tmpLongo) && (digitalRead(this->btns) == LOW));
    if ((millis() - tmpInicio < tmpCurto)){         //CLICK E ACIOMA O LED1 SE O EDITI ESTIVER LOW
      if(digitalRead(this->pin_l) == HIGH){
        modeId(this->ledId);
      }
      else if (digitalRead(this->pin_e) == HIGH){   //CLICK E ACIOMA O LED1 SE O EDITI ESTIVER HIGH
        modeId(this->ledId);
      }
      while (digitalRead(this->btns) == LOW);
    } else if ((millis() - tmpInicio >= tmpLongo)){
        //PRESS 2s E ACIOMA MOD LOOP
        modeMenu(this->ledId, true);
        while (digitalRead(this->btns) == LOW);
    } else {              
        //PRESS -2s E ACIOMA O LOOP MOD SEM SALVAR 
        modeMenu(this->ledId);
        while (digitalRead(this->btns) == LOW);
    }
  }
}

void FootSwetch::modeMenu(int _id, bool tmpAction = false){
  //PRESS + 2s E ACIOMA O EDIT MOD E APAGA OS LED D1 - D5
 if(this->mode == "EDIT" && _id == 0 && digitalRead(this->pin_l) == HIGH && tmpAction == true){
    modLoopEditeSave(HIGH, LOW, true); // APAGA TODOS OS LEDs DO PAINEL
    Serial.println("PRESS EDIT MODE");
  }else if(this->mode == "SAVE" && _id == 1 && digitalRead(this->pin_e) == HIGH && tmpAction == true){
    modLoopEditeSave(LOW, LOW); // APAGA OS LED DE LOOP E EDITE
    confirmAction();
    Serial.println("PRESS SAVE MODE");
  }else if(digitalRead(this->pin_l) == LOW && digitalRead(this->pin_e) == LOW && tmpAction == true){
    confirmeLed(ledsArray[_id]); //COMFIRMEDE ACTION
    modLoopEditeSave(LOW, HIGH); //RETORNA AO MODO LOOP
    Serial.println("CONFIRMA");
  }else if(tmpAction == false){
    modLoopEditeSave(LOW, HIGH);
    modeId(this->ledId);
    Serial.println("EXIT MODE");
  }
}

void FootSwetch::modeId(int _id, bool actions = false){
  if(actions == false){
    ctrl74hc595.toggle(NULL, ledsArray[_id]);
  }else if(actions == true){
    ctrl74hc595.toggle(relaysArray[_id], NULL);
  }
}

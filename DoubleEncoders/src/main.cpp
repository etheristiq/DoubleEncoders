/*
  TODO: 
  ATmega2560 migration
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include "custom_uart.h"
#include "custom_timer.h"

volatile uint16_t counter1 = 0;
volatile uint16_t counter2 = 0;
long time;

ISR(INT0_vect){
  counter1++;
}

ISR(INT1_vect){
  counter2++;
}

void init_interrupt(void){

  //SREG |= (1<<7);                       //сказать MCU что будут использоваться прерывания 
                                          //(она и так по умолчанию знает это)
                                          //cli() и sei() управляют этим регистром

  EICRA = 0b00000010;                     //0-0-0-0-ISC11-ISC10-ISC01-ISC00
  EIMSK = 0b00000001;                     //разрешить прерывания на 3м пине (INT0) 
}

int main(void){

  uint32_t tim;
  
  init_interrupt();
  sei();
  
  uart_mini_ini(16000000, 9600);
  timer_ini();

  DDRB |= (1<<5);

  while(1){

    if (millis()-tim>500){     //каждые 500ms начинается проверка
      cli();                    //выключаются прерывания
      uart_send_c('\n');
      uart_send_uint(counter);  //отправляется кол-во замеченых пролётов
      tim=millis();             //обновляется время отсчёта
      counter1=0;                //обнуляется кол-во пролётов
      sei();                    //включается прерывания
    }
  }
}

#include "MKL46Z4.h"

int door_status1=0;
int door_status2=0;
int safe=0;   //0 es usafe 1 es safe
int hasChanged=0; //Controla los cambios de estado

void led_green_init()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
  PORTD->PCR[5] = PORT_PCR_MUX(1);
  GPIOD->PDDR |= (1 << 5);
  GPIOD->PSOR = (1 << 5);
}
void delay(void)
{
  volatile int i;

  for (i = 0; i < 1000000; i++);
}
void led_green_toggle()
{
  GPIOD->PTOR = (1 << 5);
}

void led_red_init()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
  PORTE->PCR[29] = PORT_PCR_MUX(1);
  GPIOE->PDDR |= (1 << 29);
  GPIOE->PSOR = (1 << 29);
}

void led_red_toggle(void)
{
  GPIOE->PTOR = (1 << 29);
}

void initButtons(){
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
  PORTC->PCR[3] = PORT_PCR_MUX(1);
  PORTC->PCR[3] |= PORT_PCR_PE(1) | PORT_PCR_PS(1);

  PORTC->PCR[12] = PORT_PCR_MUX(1);
  PORTC->PCR[12] |= PORT_PCR_PE(1) | PORT_PCR_PS(1);

}

void ledHandler(int safe, int hasChanged){
  if(safe!=hasChanged){          
    led_green_toggle();              
    led_red_toggle();
  }

}

int main(void)
{
  led_green_init();
  led_red_init();
  initButtons();
  led_green_toggle();
  while (1) {
    if (!(GPIOC->PDIR & (1 << 3))){
      door_status1=!door_status1;
      delay();
    }
    if (!(GPIOC->PDIR & (1 << 12))){
      door_status2=!door_status2;
      delay();
    }
    if((door_status1==1 && door_status2==1) || (door_status1==1 && door_status2==0) || (door_status1==0 && door_status2==1)){
      hasChanged=1;
    }else{
      hasChanged=0;
    }
    ledHandler(safe,hasChanged);
    safe=hasChanged;
  }

  return 0;
}

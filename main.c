#include "MKL46Z4.h"

int door_status1=0;
int door_status2=0;
int safe=0;   //0 es usafe 1 es safe

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
  SIM->SCGC5 |= ( SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTC_MASK);
  PORTC->PCR[3] = PORT_PCR_MUX(1);
  PORTC->PCR[3] |= PORT_PCR_PE(1) | PORT_PCR_PS(1);

  PORTC->PCR[12] = PORT_PCR_MUX(1);
  PORTC->PCR[12] |= PORT_PCR_PE(1) | PORT_PCR_PS(1);

}

void ledHandler(int safe){
  if(safe==1){
    led_green_toggle();
  }else {
    led_red_toggle();
  }
}

int main(void)
{
  led_green_init();
  led_red_init();
  initButtons();
  while (1) {
    if (!(GPIOC->PDIR & (1 << 3))){
      delay();
      door_status1=!door_status1;
    }
    if (!(GPIOC->PDIR & (1 << 12))){
      delay();
      door_status2=!door_status2;
    }
    if((door_status1==1 && door_status2==1) || (door_status1==1 && door_status2==0) || (door_status1==0 && door_status2==1)){
      safe=0;
    }else{
      safe=1;
    }
    ledHandler(safe);
  }

  return 0;
}

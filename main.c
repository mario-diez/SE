/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "MKL46Z4.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */

int state=0;
int state2=0;

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
  PORTC->PCR[3] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

  PORTC->PCR[3] |= PORT_PCR_IRQC(0xA);

  PORTC->PCR[12] = PORT_PCR_MUX(1);
  PORTC->PCR[12] |= PORT_PCR_PE(1) | PORT_PCR_PS(1);
  PORTC->PCR[12] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

  PORTC->PCR[12] |= PORT_PCR_IRQC(0xA);

  NVIC_SetPriority(PORTC_PORTD_IRQn,3);
  NVIC_EnableIRQ(PORTC_PORTD_IRQn);

}

// void ledHandler(int safe, int hasChanged){
//   if(safe!=hasChanged){          
//     led_green_toggle();              
//     led_red_toggle();
//   }

// }

void PORTDIntHandler(void){

    if(!(PORTC->PCR[3] & PORT_PCR_ISF_MASK)){
      state=!state;
      led_red_toggle();
      if(state2==1){
        state2=!state2;
        led_green_toggle();
      }
      
    }
    if(!(PORTC->PCR[12] & PORT_PCR_ISF_MASK)){ 
      state2=!state2;
      led_green_toggle();
      if(state==1){
        state=!state;
        led_red_toggle();
      }
    }

    PORTC->PCR[3] |= PORT_PCR_ISF_MASK;
    PORTC->PCR[12] |= PORT_PCR_ISF_MASK;

}

int main(void)
{
  char ch;

  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();
  initButtons();
  led_green_init();
  led_red_init();

  PRINTF("\r\nReinicio!\r\n");

  while (1)
    {
      ch = GETCHAR();
      PUTCHAR(ch);
    }
}

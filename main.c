#include "MKL46Z4.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "lcd.h"

QueueHandle_t queue;
int messages;
SemaphoreHandle_t semaphore;

void taskProducer(void *pvParameters)
{
    int product =0;
    for (;;) {
        xQueueSend(queue,&product,0);
        if(xSemaphoreTake(semaphore,(TickType_t)10)==pdTRUE){
            lcd_set(messages,1);
            
            messages++;
            
            xSemaphoreGive(semaphore);
            vTaskDelay((TickType_t)10);
        }
        
    }
}

void taskConsumer(void *pvParameters)
{   
    int element;
    for (;;) {

        if(messages>0){
                //xQueueReceive(queue,&element,100/portTICK_RATE_MS);
            }
        if(xSemaphoreTake(semaphore,(TickType_t)10)==pdTRUE){
            lcd_set(messages,1);
            
            
            messages--;
            
            xSemaphoreGive(semaphore);
            
            vTaskDelay(201/portTICK_RATE_MS);
        } 
        
    }
}

void irclk_ini()
{
  MCG->C1 = MCG_C1_IRCLKEN(1) | MCG_C1_IREFSTEN(1);
  MCG->C2 = MCG_C2_IRCS(0); //0 32KHZ internal reference clock; 1= 4MHz irc
}

int main(void)
{
    TaskHandle_t xHandle=NULL;
    irclk_ini();
	lcd_ini();
    queue=xQueueCreate(10,sizeof(int));
    semaphore=xSemaphoreCreateMutex();
	/* create green led task */
	xTaskCreate(taskConsumer, (signed char *)"TaskConsumer", 
		configMINIMAL_STACK_SIZE, (void *)NULL, 1, &xHandle);

	/* create red led task */
	xTaskCreate(taskProducer, (signed char *)"TaskProducer", 
		configMINIMAL_STACK_SIZE, (void *)NULL, 1, &xHandle);
	
	/* start the scheduler */
	vTaskStartScheduler();

	/* should never reach here! */
	for (;;);

	return 0;
}


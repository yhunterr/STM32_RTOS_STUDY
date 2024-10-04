#include <stdio.h>
#include "cmsis_os.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

void task1(void *pvParameters);
void task2(void *pvParameters);
QueueHandle_t xQueue;

void myTask()
{
  xQueue = xQueueCreate(5, sizeof(uint16_t));
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  uint16_t i=0;
  if(GPIO_Pin == BTN1_Pin)
  {
    printf("BTN1 clicked\n");
    xQueueSendFromISR(xQueue, &GPIO_Pin, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
  else if(GPIO_Pin == BTN2_Pin)
  {
    printf("BTN2 clicked\n");
    printf("Queue count %u \n",uxQueueMessagesWaitingFromISR(xQueue));
    if(xQueueReceiveFromISR(xQueue, &i, &xHigherPriorityTaskWoken))
    {
      printf("Queue received : 0x%x \n",i);
    }
    else
    {
      printf("Queue received FAIL\n");
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
  printf("\n");
}


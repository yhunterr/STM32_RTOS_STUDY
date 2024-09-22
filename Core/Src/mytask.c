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

  xQueue = xQueueCreate(5, sizeof(uint8_t));
  xTaskCreate(task1, "Task1", 256, NULL, 1, NULL);
  xTaskCreate(task2, "Task2", 256, NULL, 1, NULL);
}

void task1(void *pvParameters)
{
  uint8_t i = 0;
  GPIO_PinState last_state = HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin);
  GPIO_PinState current_state;
  while(1)
  {
    current_state = HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin);
    if(last_state == GPIO_PIN_SET && current_state == GPIO_PIN_RESET)
    {
      printf("BTN1 is clicked : %d \n",i);
      if(xQueueSend(xQueue,&i,0) != pdPASS)
      {
        printf("FAIL ! \n");
      }
      i++;
    }
    last_state = current_state;
    vTaskDelay(10);
  }
}

void task2(void *pvParameters)
{
  uint8_t i=0;
  while(1)
  {
    //if(uxQueueMessagesWaiting(xQueue) > 0)
    {
      if(xQueueReceive(xQueue, &i, 2000) == pdPASS)
      {
        printf("Queue received : %d \n",i);
      }
      else
      {
        printf("Queue received FAIL\n");
      }
    }
    vTaskDelay(10);
  }
}

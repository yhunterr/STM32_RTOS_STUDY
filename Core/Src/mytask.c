#include <stdio.h>
#include "cmsis_os.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

void task(void *pvParameters);

SemaphoreHandle_t xButtonSemaphore;

int global_i=0;
uint16_t GPIO_Pin_BTN;

void myTask()
{

  //xButtonSemaphore  = xSemaphoreCreateBinary(); // Binary Semaphore
  xButtonSemaphore  = xSemaphoreCreateCounting(3,0); // Counting Semaphore


  xTaskCreate(task, "Task1", 256, "TASK1", 3, NULL);
}

void task(void *pvParameters)
{
  int local_i = 0;

  while(1)
  {
    if (xSemaphoreTake(xButtonSemaphore, portMAX_DELAY) == pdTRUE)
    {
      printf("BTN PIN , Counting , local_i : %x,  %lu,  %d \n",GPIO_Pin_BTN, uxSemaphoreGetCount(xButtonSemaphore), local_i);
      local_i++;
    }
    vTaskDelay(100);
  }
}


#define DEBOUNCE_TIME_MS 50
uint32_t debounce_time;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  //Debounce
  uint32_t now = HAL_GetTick();
  if (now-debounce_time < DEBOUNCE_TIME_MS)
    return;
  debounce_time = now;


  GPIO_Pin_BTN = GPIO_Pin;

  xSemaphoreGiveFromISR(xButtonSemaphore, &xHigherPriorityTaskWoken);
  xSemaphoreGiveFromISR(xButtonSemaphore, &xHigherPriorityTaskWoken);
  xSemaphoreGiveFromISR(xButtonSemaphore, &xHigherPriorityTaskWoken);

  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


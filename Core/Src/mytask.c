#include <stdio.h>
#include "cmsis_os.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

void task(void *pvParameters);

SemaphoreHandle_t mutex;

int global_i=0;
int end = 0;

void myTask()
{
  mutex = xSemaphoreCreateMutex();

  xTaskCreate(task, "Task1", 256, "TASK1", 3, NULL);
  xTaskCreate(task, "Task2", 256, "TASK2", 3, NULL);
}

void task(void *pvParameters)
{
  int local_i = 0;
  char *task_name;
  task_name = (char *)pvParameters;


  while(1)
  {
    if(xSemaphoreTake(mutex,portMAX_DELAY)== pdTRUE)
    {
      local_i = global_i;
      local_i++;
      if((global_i == local_i) || end)
      {
        printf("%s END\n",task_name);
        end = 1;
        vTaskDelay(1000);
        vTaskDelete(NULL);
      }
      global_i = local_i;
      printf("%s %d \n",task_name, global_i);
      xSemaphoreGive(mutex);
    }
    vTaskDelay(1);
  }
}


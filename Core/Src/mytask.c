#include <stdio.h>
#include <stdbool.h>
#include "cmsis_os.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define DEBOUNCE_TIME_MS       50
#define TAP_THRESHOLD_MS       250

void btnTask(void *pvParameters);


typedef struct
{
  uint8_t ch;
  uint32_t pre_time;
  uint8_t tap_count;
  uint32_t last_tap_time;
} button_obj_t;

button_obj_t buttonObj[3];

void buttonObjectCreate(button_obj_t *p_obj, uint8_t ch)
{
  p_obj->ch = ch;
  p_obj->pre_time = HAL_GetTick();
  p_obj->tap_count = 0;
  p_obj->last_tap_time = 0;
}

void myTask()
{
  buttonObjectCreate(&buttonObj[0], 0); // button 1
  buttonObjectCreate(&buttonObj[1], 1); // button 2
  buttonObjectCreate(&buttonObj[2], 2); // button 3
  xTaskCreate(btnTask, "btnTask", 256, NULL, 3, NULL);
}

void buttonObjectProcess(button_obj_t *p_obj)
{
  uint32_t now = HAL_GetTick();

  if (p_obj->tap_count > 0)
  {
    if (now - p_obj->last_tap_time > TAP_THRESHOLD_MS)
    {
      switch (p_obj->tap_count)
      {
        case 1:
          printf("ONE TAP ch:%d\n", p_obj->ch);
          break;
        case 2:
          printf("DOUBLE TAP ch:%d\n", p_obj->ch);
          break;
        case 3:
          printf("TRIPLE TAP ch:%d\n", p_obj->ch);
          break;
        default:
          printf("TAP COUNT ERROR : %d \n",p_obj->tap_count);
          break;
      }
      p_obj->tap_count = 0;
    }
  }
}

void btnTask(void *pvParameters)
{
  printf("btn task start\n");

  while(1)
  {
    for(int i=0; i<3; i++)
    {
      buttonObjectProcess(&buttonObj[i]);
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  uint32_t now = HAL_GetTick();
  uint16_t btn_ID;

  if(GPIO_Pin == BTN1_Pin)
  {
    btn_ID = 0;
  }
  else if(GPIO_Pin == BTN2_Pin)
  {
    btn_ID = 1;
  }
  else if(GPIO_Pin == BTN3_Pin)
  {
    btn_ID = 2;
  }
  else
    return;

  // Debounce check
  if (now - buttonObj[btn_ID].pre_time < DEBOUNCE_TIME_MS)
    return;
  buttonObj[btn_ID].pre_time = now;
  //

  buttonObj[btn_ID].tap_count++;
  buttonObj[btn_ID].last_tap_time = now;
}

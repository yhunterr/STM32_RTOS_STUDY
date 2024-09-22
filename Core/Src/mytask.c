#include <stdio.h>
#include "cmsis_os.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t xHandleTask1;

void Task1(void *pvParameters)
{
    while(1)
    {
        printf("Hello world %s \n", pcTaskGetName(NULL));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void myTask()
{
    BaseType_t xReturned;
    xReturned = xTaskCreate(Task1,
                            "Task1",
                            256,
                            NULL,
                            0,
                            &xHandleTask1);
    if(xReturned == pdPASS)
    {
        printf("Task1 PASS \n");
    }
    else
    {
        printf("Task1 FAIL \n");
    }
}

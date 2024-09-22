#include <stdio.h>
#include "cmsis_os.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

void LED_Task(void *pvParameters);

TaskHandle_t xHandleTask1, xHandleTask2, xHandleTask3, xHandleTask4, xHandleTask5, xHandleTask6;

typedef struct {
    GPIO_TypeDef* GPIO_Port;
    uint16_t GPIO_Pin;
    TickType_t onTime;
    TickType_t period;
} LED_Params;

void myTask()
{
    static LED_Params led1 = {LED1_GPIO_Port, LED1_Pin, pdMS_TO_TICKS(100), pdMS_TO_TICKS(1000)};
    static LED_Params led2 = {LED2_GPIO_Port, LED2_Pin, pdMS_TO_TICKS(200), pdMS_TO_TICKS(1000)};
    static LED_Params led3 = {LED3_GPIO_Port, LED3_Pin, pdMS_TO_TICKS(300), pdMS_TO_TICKS(1000)};
    static LED_Params led4 = {LED4_GPIO_Port, LED4_Pin, pdMS_TO_TICKS(400), pdMS_TO_TICKS(1000)};
    static LED_Params led5 = {LED5_GPIO_Port, LED5_Pin, pdMS_TO_TICKS(500), pdMS_TO_TICKS(1000)};
    static LED_Params led6 = {LED6_GPIO_Port, LED6_Pin, pdMS_TO_TICKS(600), pdMS_TO_TICKS(1000)};

    xTaskCreate(LED_Task, "Task1", 256, &led1, 1, &xHandleTask1);
    xTaskCreate(LED_Task, "Task2", 256, &led2, 1, &xHandleTask2);
    xTaskCreate(LED_Task, "Task3", 256, &led3, 1, &xHandleTask3);
    xTaskCreate(LED_Task, "Task4", 256, &led4, 1, &xHandleTask4);
    xTaskCreate(LED_Task, "Task5", 256, &led5, 1, &xHandleTask5);
    xTaskCreate(LED_Task, "Task6", 256, &led6, 1, &xHandleTask6);
}

void LED_Task(void *pvParameters)
{
    LED_Params *params = (LED_Params*) pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        HAL_GPIO_WritePin(params->GPIO_Port, params->GPIO_Pin, GPIO_PIN_RESET);
        vTaskDelay(params->onTime);
        HAL_GPIO_WritePin(params->GPIO_Port, params->GPIO_Pin, GPIO_PIN_SET);
        vTaskDelayUntil(&xLastWakeTime, params->period);
    }
}

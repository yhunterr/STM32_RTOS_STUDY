#include <stdio.h>
#include "cmsis_os.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"




TimerHandle_t xTimer;
uint8_t ledState = 1;
uint8_t ledCount = 10;


typedef struct {
    GPIO_TypeDef* GPIO_Port;
    uint16_t GPIO_Pin;
} LED_Params;


// 타이머 콜백 함수
void vTimerCallback(TimerHandle_t xTimer) {
    LED_Params *ledInfo = (LED_Params *)pvTimerGetTimerID(xTimer);
    if(ledState)
    {
      HAL_GPIO_WritePin(ledInfo->GPIO_Port, ledInfo->GPIO_Pin, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(ledInfo->GPIO_Port, ledInfo->GPIO_Pin, GPIO_PIN_RESET);
    }
    ledState = !ledState;
    ledCount--;
    if(ledCount==0)
    {
      xTimerStop(xTimer,0);
    }
}


void myTask()
{
    static LED_Params led1 = {LED1_GPIO_Port, LED1_Pin};

    // 타이머 생성
    xTimer = xTimerCreate(
        "Timer1",                // 타이머 이름
        pdMS_TO_TICKS(1000),         // 타이머 주기 (1000ms)
        pdTRUE,                     // 자동 재시작 (pdTRUE: 반복, pdFALSE: 1회)
        &led1,                       // 타이머 ID
        vTimerCallback              // 타이머 콜백 함수
    );

    // 타이머 생성 확인
    if (xTimer == NULL) {
        // 타이머 생성 실패 처리
        printf("Failed to create timer!\r\n");
        while (1);
    }

    // 타이머 시작
    if (xTimerStart(xTimer, 0) != pdPASS) {
        // 타이머 시작 실패 처리
        printf("Failed to start timer!\r\n");
        while (1);
    }
}

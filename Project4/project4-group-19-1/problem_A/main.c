#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

xTaskHandle incTaskHandle;
xTaskHandle decTaskHandle;
xTaskHandle printFinalCounterTaskHandle;

SemaphoreHandle_t incTaskDone;
SemaphoreHandle_t decTaskDone;
SemaphoreHandle_t counterMutex;

volatile int counter = 0;

void incCounterTask(void *pvParameters)
{
    int i;
    for(i = 0; i < 1e7; i++)
    {
    xSemaphoreTake(counterMutex, portMAX_DELAY);
	counter = counter + 1;
    xSemaphoreGive(counterMutex); 
    }
    xSemaphoreGive(incTaskDone); 
    vTaskDelete(incTaskHandle);
}

void decCounterTask(void *pvParameters)
{
    int i;
    for(i = 0; i < 1e7; i++ )
    {
    xSemaphoreTake(counterMutex, portMAX_DELAY);
	counter = counter - 1;
    xSemaphoreGive(counterMutex); 
    }
    xSemaphoreGive(decTaskDone);
    vTaskDelete(decTaskHandle);
}

void printFinalCounterTask(void *pvParameters)
{
    xSemaphoreTake(incTaskDone, portMAX_DELAY);
    xSemaphoreTake(decTaskDone, portMAX_DELAY);
    xSemaphoreTake(counterMutex, portMAX_DELAY);
    printf("Final value: %d\n", counter);
    xSemaphoreGive(counterMutex);
    vTaskDelete(printFinalCounterTaskHandle);
}

int main(int argc, char **argv)
{
    incTaskDone = xSemaphoreCreateBinary();
    decTaskDone = xSemaphoreCreateBinary();
    counterMutex = xSemaphoreCreateMutex();
    xTaskCreate(incCounterTask, "Increase Counter Task", configMINIMAL_STACK_SIZE, NULL, 1, &incTaskHandle);
    xTaskCreate(decCounterTask, "Decrease Counter Task", configMINIMAL_STACK_SIZE, NULL, 1, &decTaskHandle);
    xTaskCreate(printFinalCounterTask, "Print Final Counter Task", configMINIMAL_STACK_SIZE, NULL, 1, &printFinalCounterTaskHandle);
    vTaskStartScheduler();
    for( ;; );
}
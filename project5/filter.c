#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "filter.h"
#include "constants.h"

#define H 0.01f
#define ALPHA 0.04f
float GAMMA = 0.95;

void filterTask(void *pvParameters) {
    struct FilterParams *params =
        (struct FilterParams*)pvParameters;

    // we keep local copies of the global state + semaphores
    double gyro_data[3];
    double acc_data[3];

    // copy the semaphore handles for convenience
    SemaphoreHandle_t sensors_sem = params->sensors_sem;
    SemaphoreHandle_t estimate_sem = params->estimate_sem;

    // local internal state.
    double estimate[3] = {0.0};
    TickType_t tickCount;
    const TickType_t xFrequency = 10;

    while(1) {
        tickCount = xTaskGetTickCount();
        // read sensor data
        memcpy(gyro_data, params->gyro_data, sizeof(gyro_data));
        memcpy(acc_data, params->acc_data, sizeof(acc_data));

        // apply filter
        xSemaphoreTake(sensors_sem, portMAX_DELAY);
        double acc_angle[2] = {0.0};
        double gyro_angle[3] = {0.0};
        acc_angle[0] = (180/M_PI) * atan2(acc_data[1], acc_data[2]);
        acc_angle[1] = (180/M_PI) * atan2(-acc_data[0], sqrt(pow(acc_data[1], 2) + pow(acc_data[2], 2)));

        gyro_angle[0] = gyro_data[0] * H;
        gyro_angle[1] = gyro_data[1] * H;
    
        estimate[0] = (1 - GAMMA) * acc_angle[0] + GAMMA * (estimate[0] + gyro_angle[0]);
        estimate[1] = (1 - GAMMA) * acc_angle[1] + GAMMA * (estimate[1] + gyro_angle[1]);

        xSemaphoreGive(sensors_sem);
        // estimate of the yaw angle provided as an example
        // estimate[2] += 0.01 * gyro_data[2];

        // example of how to log some intermediate calculation
        params->log_data[0] = estimate[0];
        params->log_data[1] = estimate[1];
        params->log_data[2] = gyro_data[2];;

        // write estimates output
        xSemaphoreTake(estimate_sem,portMAX_DELAY);
        memcpy(params->estimate, estimate, sizeof(estimate));
        xSemaphoreGive(estimate_sem);
        
        // sleep 10ms to make this task run at 100Hz
        // vTaskDelay(10 / portTICK_PERIOD_MS);
        vTaskDelayUntil( &tickCount, xFrequency );
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "controller.h"
#include "constants.h"
#define ROWS_K 4
#define COLS_K 5

/* void matrixVectorMultiplication(double matrix[][COLS_K], double vector[], double result[]) {
    int i, j;
    int base_thrust = 30000;
    for (i = 0; i < ROWS_K; i++) {
        for (j = 0; j < COLS_K; j++) {
            result[j] += matrix[i][j] * vector[j];
        }
    }
} */



void controlSystemTask(void *pvParameters) {
    struct ControlSystemParams *params =
        (struct ControlSystemParams*)pvParameters;

    // we keep local copies of the global state + semaphores
    unsigned short motors[4];
    double gyro_data[3];
    double acc_data[3];
    double r_rpdy[3];
    double estimate[3] = {0.0};
    double error[5] = {0.0};
    

    // define the K-matrix
/*     double K[4][5] = {
    {-308.025, -456.624, -3083.337000000000443833414465189, -4570.7984999999998763087205588818, -20420.109000000000378349795937538},
    {-308.025,  456.624, -3083.337000000000443833414465189,  4570.7984999999998763087205588818,  20420.109000000000378349795937538},
    { 308.025,  456.624,  3083.337000000000443833414465189,  4570.7984999999998763087205588818, -20420.109000000000378349795937538},
    { 308.025, -456.624,  3083.337000000000443833414465189, -4570.7984999999998763087205588818,  20420.109000000000378349795937538}
    };  */

    // copy the semaphore handles for convenience
    SemaphoreHandle_t motors_sem = params->motors_sem;
    SemaphoreHandle_t references_sem = params->references_sem;
    SemaphoreHandle_t sensors_sem = params->sensors_sem;
    SemaphoreHandle_t estimate_sem = params->estimate_sem;
    TickType_t tickCount;
    const TickType_t xFrequency = 10;

    while(1) {
        tickCount = xTaskGetTickCount();
        xSemaphoreTake(sensors_sem,portMAX_DELAY);
        // read sensor data (gyro)
        memcpy(gyro_data, params->gyro_data, sizeof(gyro_data));

        // read filter data (angle estimates)
        memcpy(estimate, params->estimate, sizeof(estimate));

        // read latest references
        memcpy(r_rpdy, params->r_rpdy, sizeof(r_rpdy));
        xSemaphoreGive(sensors_sem);

        // compute error
        double roll_error = r_rpdy[0] - estimate[0];
        double pitch_error = r_rpdy[1] - estimate[1]; 
        double yaw_error = r_rpdy[2] - gyro_data[2]; 


        error[0] = (M_PI/180) * roll_error;
        error[1] = (M_PI/180) * pitch_error;
        error[2] = (M_PI/180) * (- gyro_data[0]);
        error[3] = (M_PI/180) * (- gyro_data[1]);
        error[4] = (M_PI/180) * yaw_error;

        // example of how to log some intermediate calculation
        // and use the provided constants
        // params->log_data[0] = crazyflie_constants.m * crazyflie_constants.g;
        // params->log_data[2] = r_rpdy[0];

        // compute motor outputs
        // matrixVectorMultiplication(K, error, motors);
        
        double K[5] = {0.2405, 0.3564, 0.0100, 0.0148, 0.0576};
        int base_thrust = 30000;
        motors[0] = base_thrust + (-K[0]*error[0] - K[1]*error[1] - K[2]*error[2] - K[3]*error[3] - K[4]*error[4])*(1/crazyflie_constants.pwm_n);
        motors[1] = base_thrust + (-K[0]*error[0] + K[1]*error[1] - K[2]*error[2] + K[3]*error[3] + K[4]*error[4])*(1/crazyflie_constants.pwm_n);
        motors[2] = base_thrust + (K[0]*error[0] + K[1]*error[1] + K[2]*error[2] + K[3]*error[3] - K[4]*error[4])*(1/crazyflie_constants.pwm_n);
        motors[3] = base_thrust + (K[0]*error[0] - K[1]*error[1] + K[2]*error[2] - K[3]*error[3] + K[4]*error[4])*(1/crazyflie_constants.pwm_n);
          
        // write motor output
        xSemaphoreTake(motors_sem, portMAX_DELAY);
        memcpy(params->motors, motors, sizeof(motors));
        xSemaphoreGive(motors_sem);

        // sleep 10ms to make this task run at 100Hz
        // vTaskDelay(10 / portTICK_PERIOD_MS);
        vTaskDelayUntil( &tickCount, xFrequency );
    }
}

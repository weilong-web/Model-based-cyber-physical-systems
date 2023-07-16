#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define MIN_CROSSING_DELAY 500
#define CROSSING_DELAY 1000
#define ARRIVE_DELAY 1500

#define NORTH 0
#define SOUTH 1
#define EAST  2
#define WEST  3



xTaskHandle *batTaskHandles; // Create as a pointer since the number of bats are unknown at compile time

int *batIds;

volatile int counter = 0;


xSemaphoreHandle crossingMutex;
xSemaphoreHandle crossingConditions[4];
int batCount[4] = {0, 0, 0, 0};


void goSouth(int batId)
{
    const TickType_t waitingToArriveDelay = (rand() % ARRIVE_DELAY) / portTICK_PERIOD_MS; // Generate a random delay 
    const TickType_t crossingDelay = (rand() % CROSSING_DELAY + MIN_CROSSING_DELAY) / portTICK_PERIOD_MS; 
    
    vPrintf("BAT %d from North has started to run and is on its way to the crossing\n", batId);

    vTaskDelay(waitingToArriveDelay);   
    
    xSemaphoreTake(crossingMutex, portMAX_DELAY); // Acquire the mutex
    batCount[NORTH]++;

    while (batCount[EAST] > 0) {
            xSemaphoreGive(crossingMutex);
            xSemaphoreTake(crossingConditions[NORTH], portMAX_DELAY);
            xSemaphoreTake(crossingMutex, portMAX_DELAY);
        }

    vPrintf("BAT %d from North arrives at crossing\n", batId);
	
    vPrintf("BAT %d from North enters crossing\n", batId);

    xSemaphoreGive(crossingMutex); //

    vTaskDelay(crossingDelay);   

    xSemaphoreTake(crossingMutex, portMAX_DELAY); //
    
    vPrintf("BAT %d from North leaving crossing\n", batId);    

    batCount[NORTH]--;
    xSemaphoreGive(crossingConditions[NORTH]);
    xSemaphoreGive(crossingConditions[EAST]);
    xSemaphoreGive(crossingMutex);

    vTaskDelay(waitingToArriveDelay);
}

void goNorth(int batId)
{
    const TickType_t waitingToArriveDelay = (rand() % ARRIVE_DELAY) / portTICK_PERIOD_MS; // Generate a random delay 
    const TickType_t crossingDelay = (rand() % CROSSING_DELAY + MIN_CROSSING_DELAY) / portTICK_PERIOD_MS;
 
    vPrintf("BAT %d from South has started to run and is on its way to the crossing\n", batId);

    vTaskDelay(waitingToArriveDelay);   

    xSemaphoreTake(crossingMutex, portMAX_DELAY);
    batCount[SOUTH]++;

    while (batCount[WEST] > 0) {
        xSemaphoreGive(crossingMutex);
        xSemaphoreTake(crossingConditions[SOUTH], portMAX_DELAY);
        xSemaphoreTake(crossingMutex, portMAX_DELAY);
    }
  
    vPrintf("BAT %d from South arrives at crossing\n", batId);
  
    vPrintf("BAT %d from South enters crossing\n", batId);

    xSemaphoreGive(crossingMutex);
 
    vTaskDelay(crossingDelay);   

    xSemaphoreTake(crossingMutex, portMAX_DELAY);

    vPrintf("BAT %d from South leaving crossing\n", batId);  

    batCount[SOUTH]--;
    xSemaphoreGive(crossingConditions[SOUTH]);
    xSemaphoreGive(crossingConditions[WEST]);
    xSemaphoreGive(crossingMutex);  

    vTaskDelay(waitingToArriveDelay);
}

void goEast(int batId)
{
    const TickType_t waitingToArriveDelay = (rand() % ARRIVE_DELAY) / portTICK_PERIOD_MS; // Generate a random delay   
    const TickType_t crossingDelay = (rand() % CROSSING_DELAY + MIN_CROSSING_DELAY) / portTICK_PERIOD_MS; 
    
    vPrintf("BAT %d from West has started to run and is on its way to the crossing\n", batId);

    vTaskDelay(waitingToArriveDelay);   

    xSemaphoreTake(crossingMutex, portMAX_DELAY);
    batCount[WEST]++;

    while (batCount[SOUTH] > 0) {
        xSemaphoreGive(crossingMutex);
        xSemaphoreTake(crossingConditions[WEST], portMAX_DELAY);
        xSemaphoreTake(crossingMutex, portMAX_DELAY);
    }

    vPrintf("BAT %d from West arrives at crossing\n", batId);

    vPrintf("BAT %d from West enters crossing\n", batId);

    xSemaphoreGive(crossingMutex);

    vTaskDelay(crossingDelay);   

    xSemaphoreTake(crossingMutex, portMAX_DELAY);

    vPrintf("BAT %d from West leaving crossing\n", batId);    

    batCount[WEST]--;
    xSemaphoreGive(crossingConditions[WEST]);
    xSemaphoreGive(crossingConditions[SOUTH]);
    xSemaphoreGive(crossingMutex);

    vTaskDelay(waitingToArriveDelay);
}

void goWest(int batId)
{
    const TickType_t waitingToArriveDelay = (rand() % ARRIVE_DELAY) / portTICK_PERIOD_MS; // Generate a random delay 
    const TickType_t crossingDelay = (rand() % CROSSING_DELAY + MIN_CROSSING_DELAY) / portTICK_PERIOD_MS; 
     
    vPrintf("BAT %d from East has started to run and is on its way to the crossing\n", batId);

    vTaskDelay(waitingToArriveDelay);   

    xSemaphoreTake(crossingMutex, portMAX_DELAY);
    batCount[EAST]++;

    while (batCount[NORTH] > 0) {
        xSemaphoreGive(crossingMutex);
        xSemaphoreTake(crossingConditions[EAST], portMAX_DELAY);
        xSemaphoreTake(crossingMutex, portMAX_DELAY);
    }

    vPrintf("BAT %d from East arrives at crossing\n", batId);

    vPrintf("BAT %d from East enters crossing\n", batId);

    xSemaphoreGive(crossingMutex);

    vTaskDelay(crossingDelay);   

    xSemaphoreTake(crossingMutex, portMAX_DELAY);

    vPrintf("BAT %d from East leaving crossing\n", batId); 

    batCount[EAST]--;
    xSemaphoreGive(crossingConditions[EAST]);
    xSemaphoreGive(crossingConditions[NORTH]);
    xSemaphoreGive(crossingMutex);   

    vTaskDelay(waitingToArriveDelay);
}

void batFromNorth(void *pvParameters)
{
    int batId = *(int *)pvParameters;
    
    while (1) // Repeat forever
    {
	goSouth(batId); 
	goNorth(batId);
    }
}

void batFromSouth(void *pvParameters)
{
    int batId = *(int *)pvParameters;
    
    while (1)
    {
	goNorth(batId); 
	goSouth(batId);
    }
}

void batFromEast(void *pvParameters)
{
    int batId = *(int *)pvParameters;
    
    while (1)
    {
	goWest(batId); 
	goEast(batId);
    }
}

void batFromWest(void *pvParameters)
{
    int batId = *(int *)pvParameters;
    
    while (1)
    {
	goEast(batId); 
	goWest(batId);
    }
}



int nextDirectionIndex(int index) {
    return (index + 1) % 4;
}



int main(int argc, char **argv)
{
    int batId;
    int numberOfBats;
        
    if (argc < 2)
    {
    	vPrintf("Argument missing, specify sequence of arrivals. Example: ./batman nsewwewn\n");
    	exit(-1);
    }
    
    char *commands = argv[1];
    numberOfBats = strlen(argv[1]);

    batTaskHandles = malloc(sizeof(xTaskHandle) * numberOfBats); // Allocate memory for the task handles
    batIds = malloc(sizeof(int) * numberOfBats); // Allocate memory for the task ids



    crossingMutex = xSemaphoreCreateMutex();
    for (int i = 0; i < 4; i++) {
    crossingConditions[i] = xSemaphoreCreateBinary();
    }


    for (batId = 0; batId < numberOfBats; batId++)
    {
    	batIds[batId] = batId;
    	char currentBatDirection = (argv[1])[batId];
    	char batTaskIdentity[50];
    	
    	if (currentBatDirection == 'n')
    	{
    	    sprintf(batTaskIdentity, "Bat %d from North", batId);
      	    xTaskCreate(batFromNorth, batTaskIdentity, configMINIMAL_STACK_SIZE, (void *)&batIds[batId], 1, batTaskHandles[batId]);  
      	    vPrintf("Created task \"Bat %d from North\"\n", batId); 	
    	}
    	else if (currentBatDirection == 's')
    	{
      	    sprintf(batTaskIdentity, "Bat %d from South", batId);
      	    xTaskCreate(batFromSouth, batTaskIdentity, configMINIMAL_STACK_SIZE, (void *)&batIds[batId], 1, batTaskHandles[batId]);  
      	    vPrintf("Created task \"Bat %d from South\"\n", batId);   	
    	}
     	else if (currentBatDirection == 'e')
    	{
     	    sprintf(batTaskIdentity, "Bat %d from East", batId);
      	    xTaskCreate(batFromEast, batTaskIdentity, configMINIMAL_STACK_SIZE, (void *)&batIds[batId], 1, batTaskHandles[batId]);  
      	    vPrintf("Created task \"Bat %d from East\"\n", batId);    	
    	}  
    	else if (currentBatDirection == 'w')
    	{
     	    sprintf(batTaskIdentity, "Bat %d from West", batId);
      	    xTaskCreate(batFromWest, batTaskIdentity, configMINIMAL_STACK_SIZE, (void *)&batIds[batId], 1, batTaskHandles[batId]);  
      	    vPrintf("Created task \"Bat %d from West\"\n", batId);    	
    	}    	
    	else
    	{
     	    vPrintf("Unknown direction: %c\n", currentBatDirection);
    	    exit(-1);   	
    	} 	

    }
    
    vTaskStartScheduler();
    while (1)
    {
    	// Do nothing
    };
 
    free(batTaskHandles); // Return memory for the task handles
    return 0;
}

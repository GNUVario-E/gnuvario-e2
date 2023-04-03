#include <Arduino.h>
#include "freertos/task.h"

void displayTaskStats()
{
    TaskStatus_t *taskStatsArray;
    volatile UBaseType_t taskCount;
    uint32_t totalRunTime;

    // Get an array of TaskStatus_t objects describing the state of each task in the system
    taskStatsArray = new TaskStatus_t[uxTaskGetNumberOfTasks()];

    // Populate the array with task state information
    taskCount = uxTaskGetSystemState(taskStatsArray, uxTaskGetNumberOfTasks(), &totalRunTime);

    // Print the header for the task state information
    Serial.println("Task Name\t\tStatus\t\tStack\t\tTask ID\t\tCPU Time (%)");

    // Print the task state information for each task in the system
    for (UBaseType_t i = 0; i < taskCount; i++)
    {
        Serial.print(taskStatsArray[i].pcTaskName);
        Serial.print("\t\t");
        Serial.print(taskStatsArray[i].eCurrentState);
        Serial.print("\t\t");
        Serial.print(taskStatsArray[i].usStackHighWaterMark);
        Serial.print("\t\t");
        Serial.print(taskStatsArray[i].xTaskNumber);
        Serial.print("\t\t");
        Serial.print((taskStatsArray[i].ulRunTimeCounter * 100) / totalRunTime);
        Serial.println("%");
    }

    // Free the memory allocated for the task state array
    delete[] taskStatsArray;
}
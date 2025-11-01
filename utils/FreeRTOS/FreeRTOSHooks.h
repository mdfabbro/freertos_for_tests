#ifndef __FREERTOS_HOOKS_H__
#define __FREERTOS_HOOKS_H__

#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
extern "C" {
#endif

void vApplicationIdleHook(void);
void vApplicationMallocFailedHook(void);
void vApplicationStackOverflowHook(TaskHandle_t t, char *name);
void vApplicationDaemonTaskStartupHook(void);
void vApplicationTickHook(void);

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize);

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize);

#ifdef __cplusplus
}
#endif

#endif // __FREERTOS_HOOKS_H__
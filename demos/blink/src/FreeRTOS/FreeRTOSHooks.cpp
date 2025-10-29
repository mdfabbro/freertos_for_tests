#include "FreeRTOSHooks.h"

extern "C" void vAssertCalled(const char * const, unsigned long)
{
    static volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

    taskENTER_CRITICAL();
    {
        while(ulSetToNonZeroInDebuggerToContinue == 0)
        {
            __asm volatile("NOP");
        }
    }
    taskEXIT_CRITICAL();
}

void vApplicationIdleHook(void) {}
void vApplicationMallocFailedHook(void) { while(1); }

void vApplicationStackOverflowHook(TaskHandle_t t, char *name) {
    (void)t; (void)name; while(1);
}

void vApplicationDaemonTaskStartupHook(void) {}
void vApplicationTickHook(void) {}

// Elegimos un tamaño seguro para Linux
constexpr size_t IDLE_STACK_SIZE  = 1024;
constexpr size_t TIMER_STACK_SIZE = 1024;

static StackType_t uxIdleTaskStack[IDLE_STACK_SIZE];
static StaticTask_t xIdleTaskTCB;

static StackType_t uxTimerTaskStack[TIMER_STACK_SIZE];
static StaticTask_t xTimerTaskTCB;

extern "C" void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                              StackType_t **ppxIdleTaskStackBuffer,
                                              uint32_t *pulIdleTaskStackSize) {
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = IDLE_STACK_SIZE;
}

extern "C" void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                               StackType_t **ppxTimerTaskStackBuffer,
                                               uint32_t *pulTimerTaskStackSize) {
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = TIMER_STACK_SIZE;
}

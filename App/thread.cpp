// thread.cpp

#include <thread.h>
#include <FreeRTOS.h>
#include <task.h>

Thread::Thread(void) : param_(0)
{
}

Thread::~Thread(void)
{
}

void Thread::Run(const char *name, uint16_t stackSize, uint8_t priority, void *param)
{
	param_ = param;
	xTaskCreate(Thread::Execute, (signed portCHAR *)name, stackSize, this, priority, NULL);
}

void Thread::Sleep(uint32_t msec)
{
	vTaskDelay(msec);
}

void Thread::Yield(void)
{
	taskYIELD();
}

void Thread::Execute(void *p)
{
	static_cast<Thread *>(p)->ThreadFunc(static_cast<Thread *>(p)->param_);
}

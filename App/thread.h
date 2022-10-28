// thread.h - обёртка для задачи RTOS

#ifndef __THREAD_H__
#define __THREAD_H__

#include <stdint.h>

class Thread
{
public:
	virtual ~Thread(void);
	
	void Run(const char *name, uint16_t stackSize, uint8_t priority, void *param);
	static void Sleep(uint32_t msec);
	static void Yield(void);	
		
protected:
	Thread(void);
	static void Execute(void *p);
	virtual void ThreadFunc(void *param) = 0;

private:
	void *param_;
};

#endif

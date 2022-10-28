// Mes_time.c
#include <Mes_time.h>
#include <Drive_param.h>

void InitMesTime(TIM_TypeDef* TIMx)
{
  	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	uint16_t Period, Prescaler;
	
	TIM_DeInit(TIMx);
	
	Prescaler = 2; 
	Period = 0xFFFF;
	
	// Time base configuration 
	TIM_TimeBaseStructure.TIM_Period = Period;
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
}

void Start_Mes_Time(TIM_TypeDef* TIMx, Mes_Time_Def* Time)
{
  	Time->cnt_full = 0;
	Time->Delta_Timer = 0;
	TIM_SetCounter(TIMx, 0);
	TIM_Cmd(TIMx, ENABLE);
}

void Stop_Mes_Time(TIM_TypeDef* TIMx, Mes_Time_Def* Time)
{
  	Time->cnt_full = 0;
	Time->Delta_Timer = 0;
	TIM_SetCounter(TIMx, 0);
	TIM_Cmd(TIMx, DISABLE);
}

uint32_t Get_Time(TIM_TypeDef* TIMx, Mes_Time_Def* Time)
{
  	return Time->cnt_full*0xFFFF + TIM_GetCounter(TIMx);
}

uint16_t GetFullCnt(uint32_t Period)
{
  	return (uint16_t)(Period >> 16);
}

uint16_t GetDeltaTimer(uint32_t Period)
{
  	return (uint16_t)(Period - ((Period >> 16) << 16));
}

void InterruptHandlerTIMUp(TIM_TypeDef* TIMx, Mes_Time_Def* Time)
{
  	ITStatus status;
	
	status =  TIM_GetITStatus(TIMx, TIM_IT_Update);
	if (status == SET)
	{
	  	Time->cnt_full++;	
	   	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
	}
}

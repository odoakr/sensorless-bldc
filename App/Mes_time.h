// Mes_time.h
#include <stm32f10x_tim.h>
#include <stdio.h>
#include <stm32f10x_gpio.h>
#include <math.h>

typedef struct
{
  uint16_t	cnt_full;
  uint16_t	Delta_Timer;
}Mes_Time_Def;

void InitMesTime(TIM_TypeDef* TIMx);
void Start_Mes_Time(TIM_TypeDef* TIMx, Mes_Time_Def* Time);
void Stop_Mes_Time(TIM_TypeDef* TIMx, Mes_Time_Def* Time);
uint32_t Get_Time(TIM_TypeDef* TIMx, Mes_Time_Def* Time);
uint16_t GetFullCnt(uint32_t Period);
uint16_t GetDeltaTimer(uint32_t Period);
void InterruptHandlerTIMUp(TIM_TypeDef* TIMx, Mes_Time_Def* Time);
// Get_event_comm.h
#include <stm32f10x_tim.h>
#include <stdio.h>
#include <stm32f10x_gpio.h>
#include <math.h>
#include <Drive_param.h>
#include <Calc_velocity.h>

#define	SIZE_ACC_TAB	50

void InitGenEventComm(TIM_TypeDef* TIMx);
void Start_Event(TIM_TypeDef* TIMx, uint16_t TimeEvent);
void Start_ForceCommutation(TIM_TypeDef* TIMx, uint16_t TimeEvent);
void Stop_Event(TIM_TypeDef* TIMx);
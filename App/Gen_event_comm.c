// Gen_event_comm.c
#include <Gen_event_comm.h>
#include <Commutate_EC.h>
#include <Detect_zero_cross.h>
#include <Mes_time.h>

extern uint16_t	State;

void InterruptHandlerTIMCC(TIM_TypeDef* TIMx, Commutate_EC_Def* EC);

// params
uint16_t limit_dec_f_force_comm = LIMIT_DEC_F_FORCE_COMM;
uint16_t step_dec_f_force_comm = STEP_DEC_F_FORCE_COMM;
uint16_t level_on_feedback = LEVEL_ON_FEEDBACK;
uint16_t level_pause_detect_ZC = LEVEL_PAUSE_DETECT_ZC;

uint16_t cnt_step = 0;
uint16_t step_cnt = 0;

uint16_t cnt_full = 0;
uint16_t T_acc = 0;

uint16_t cnt_full_force = 0;
uint16_t T_force = 0;

uint16_t N_full[SIZE_ACC_TAB] = {43,    32,    25,    18,    14,    12,    10,    9,     8,    7,     6,     5,     5, 	   5,    4,     4,     4,     3,     3,     2,     2,     1,     1,     1, 	   1,     1,     1,     1,     1,     1,
							1,     1,     1,     1,     1,     1,     1,     1,     1,    1,     1,     1,     1,     1,     1,     0,     0,     0,     0,    0};
uint16_t DeltaT[SIZE_ACC_TAB] = {61952, 62848, 41600, 20352, 42496, 53568, 64640, 10176, 3712, 21248, 38784, 32320, 32320, 8320, 49856, 25856, 25856, 43392, 43392, 36928, 36928, 30464, 30464, 30464, 30464, 30464, 30464, 30464, 30464, 30464,
							6464,  6464,  6464,  6464,  6464, 6464, 6464, 6464, 6464, 6464, 5464, 4464, 3464, 2464, 1464, 65000, 65000, 65000, 65000, 65000};

void InitGenEventComm(TIM_TypeDef* TIMx)
{
  	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
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
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIMx, &TIM_OCInitStructure);
	TIM_OC2Init(TIMx, &TIM_OCInitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);
}

void Start_Event(TIM_TypeDef* TIMx, uint16_t TimeEvent)
{
  	TIM_SetCompare1(TIMx, TimeEvent);
	TIM_Cmd(TIMx, ENABLE);
}

void Stop_Event(TIM_TypeDef* TIMx)
{
  	TIM_SetCounter(TIMx, 0);
}

void Start_ForceCommutation(TIM_TypeDef* TIMx, uint16_t TimeEvent)
{
  	TIM_SetCompare2(TIMx, TimeEvent);
}

void InterruptHandlerTIMCC(TIM_TypeDef* TIMx, Commutate_EC_Def* EC)
{
	ITStatus status;
	uint16_t T;
	
	status =  TIM_GetITStatus(TIMx, TIM_IT_CC1);
 	if (status == SET)
	{	
	  	switch(State)
		{
		  	case	STATE_STOP:
			  	step_cnt = 0;
				cnt_step = 0;
				cnt_full = N_full[0];
				EC->Flag_fault_start = 0;
				EC->cnt_miss_zc = 0;
				EC->Wdrive = 0;
			break;
			case	STATE_PREPOS:
			  	step_cnt = 4;
				cnt_step = 4;
				cnt_full = N_full[4];
				EC->Flag_fault_start = 0;
				EC->cnt_miss_zc = 0;
				EC->Wdrive = 0;
			  	EC->cnt_miss_detectZC = 1;
				EC->Flag_missed_ZC = 0;
			break;
		  	case	STATE_ACCERATE:
			  
			 	if (cnt_full != 0)
				{
					cnt_full--;
				}
				else
				{
				  	if (EC->N_phase != Q1Q4Q6)
						Force_Event(EC);	
				  	
					if (cnt_step > level_pause_detect_ZC)
					{
						if (EC->Flag_missed_ZC == 0)	// не было пересечения 0!
						{
							EC->cnt_miss_zc++;
							EC->cnt_valid_velocity = 0;
							GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);
							EC->Flag_enable_feedback = 0;
						}
						else
						{
							EC->cnt_valid_velocity++;
							
							if (EC->cnt_valid_velocity == level_on_feedback)
							{
								GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);
								EC->Flag_enable_feedback = 1;
							}
						}
					}
					
					EC->cnt_miss_detectZC = 1;
					EC->Flag_missed_ZC = 0;
					T = TIM_GetCounter(TIMx);
					
					if (cnt_step > (SIZE_ACC_TAB - 1))
					{
					  	if (T_acc > limit_dec_f_force_comm)
					  		T_acc = T_acc - step_dec_f_force_comm;
						if (T_acc < limit_dec_f_force_comm)
						  	T_acc = T_acc + step_dec_f_force_comm;
						
						if (T_acc < limit_dec_f_force_comm)
						  	T_acc = limit_dec_f_force_comm;
					}
					else
					{				
						T_acc = DeltaT[step_cnt];
					}
					
				  	Start_Event(TIMx, T + T_acc);
					cnt_full = N_full[step_cnt];
					step_cnt++;
					cnt_step++;
					if (step_cnt > (SIZE_ACC_TAB - 1))
						step_cnt = SIZE_ACC_TAB - 1;
				}
			break;
			case	STATE_ROTATE_FB:
			  	EC->cnt_valid_velocity = 0;
				EC->cnt_miss_detectZC = 1;
				
				if (cnt_full != 0)
				{
				  	cnt_full--;
					if (cnt_full == 0)
					  	Start_Event(TIMx, T_acc);
				}
				else
				{
					EC->Wdrive = Calc_velocity(EC->P_ZC_for_W);
					EC->Flag_missed_ZC = 0;
					Force_Event(EC);	
				}
			break;
			case	STATE_REG_VELOCITY:
			  	EC->cnt_miss_detectZC = 1;

				if (cnt_full != 0)
				{
				  	cnt_full--;
					if (cnt_full == 0)
					  	Start_Event(TIMx, T_acc);
				}
				else
				{
					EC->Wdrive = Calc_velocity(EC->P_ZC_for_W);
					EC->Flag_missed_ZC = 0;
					Force_Event(EC);
				}
			break;
			case	STATE_DIRECT_CONTROL:
			  	EC->cnt_miss_detectZC = 1;

				if (cnt_full != 0)
				{
				  	cnt_full--;
					if (cnt_full == 0)
					  	Start_Event(TIMx, T_acc);
				}
				else
				{
					EC->Wdrive = Calc_velocity(EC->P_ZC_for_W);
					EC->Flag_missed_ZC = 0;
					Force_Event(EC);	
				}
			break;
		}
		TIM_ClearITPendingBit(TIMx, TIM_IT_CC1);
	}
	
	status =  TIM_GetITStatus(TIMx, TIM_IT_CC2);
 	if (status == SET)
	{
		TIM_ClearITPendingBit(TIMx, TIM_IT_CC2);
				
		if (cnt_full_force != 0)
		{
		  	cnt_full_force--;
			if (cnt_full_force == 0)
			  Start_ForceCommutation(TIMx, T_force);
		}
		else
		{
		  	if ((State == STATE_REG_VELOCITY) || (State == STATE_DIRECT_CONTROL))
			{
			  	EC->cnt_miss_detectZC = 1;
				EC->Flag_missed_ZC = 0;
			  	EC->cnt_miss_zc++;
			}
		}
	}
}
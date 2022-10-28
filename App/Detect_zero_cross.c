// Detect_zero_cross.c
#include <stm32f10x.h>
#include <stdio.h>
#include <Drive_param.h>
#include <Commutate_EC.h>
#include <Adc.h>
#include <stm32f10x_tim.h>
#include <Gen_event_comm.h>
#include <Mes_time.h>

#define	LEN_MASS_VEL		32
uint32_t	Data_P_ZC[LEN_MASS_VEL] = {0};
uint8_t		cnt_Data_P_ZC = 0;
uint16_t 	T_old = 0;
uint32_t	T32 = 0;
uint8_t		cnt_period_for_W = 0;
uint32_t	T32_for_W = 0;
// params
uint16_t	pre_commutation = PRE_COMMUTATION;
uint32_t	add_force_time = ADD_FORCE_TIME;
int16_t		add_Uin = ADD_UIN;


extern uint16_t	State;
extern Mes_Time_Def		MessTime;
extern uint16_t cnt_full, T_acc;
extern uint16_t cnt_full_force, T_force;

uint32_t	Filtr_T_ZC(uint32_t period)
{
  	uint32_t period_filtr;
	static uint32_t summ_P_ZC = 0;
	uint8_t i;

	Data_P_ZC[cnt_Data_P_ZC++] = period;
	if (cnt_Data_P_ZC == LEN_MASS_VEL)
	  cnt_Data_P_ZC = 0;
	
	summ_P_ZC = 0;
	for (i = 0; i < LEN_MASS_VEL; i++)
		summ_P_ZC += Data_P_ZC[i];  
	
	period_filtr = (uint32_t)(summ_P_ZC >> 5); // check  LEN_MASS_VEL!!!
	
	return	period_filtr;
}

void	Calc_next_commutation_detectZero(Commutate_EC_Def* comm_EC, uint32_t period)
{
  	uint16_t temp;
	
	temp = (period * (uint32_t)pre_commutation) >> 16;  // расчет опережения
	comm_EC->T_next_commutation = (period >> 1) - temp;
  	comm_EC->T_force_commutation = comm_EC->T_next_commutation + period + add_force_time;
}

uint32_t	Calc_PZC(Commutate_EC_Def* comm_EC, TIM_TypeDef* TIMx)
{
  	uint16_t T;
			
	T = TIM_GetCounter(TIMx);
	
	T32 = Get_Time(TIM2, &MessTime);
	Start_Mes_Time(TIM2, &MessTime);
		
	cnt_period_for_W++;
	T32_for_W += T32;
	if (cnt_period_for_W == 6)
	{
	  	comm_EC->P_ZC_for_W = T32_for_W / 6;
		T32_for_W = 0;
	  	cnt_period_for_W = 0;
	}
	
	T32 = Filtr_T_ZC(T32);
	
	Calc_next_commutation_detectZero(comm_EC, T32);

	if (comm_EC->Flag_enable_feedback == 1)	// обновление задания на коммутацию (ос по Emf)
	{
	  	cnt_full = GetFullCnt(comm_EC->T_next_commutation);
		T_acc = GetDeltaTimer(comm_EC->T_next_commutation) + T;
		if (cnt_full == 0)
		  	Start_Event(TIMx, T_acc);
		
		cnt_full_force = GetFullCnt(comm_EC->T_force_commutation);
		T_force = GetDeltaTimer(comm_EC->T_force_commutation) + T;
		if (cnt_full_force == 0)
			Start_ForceCommutation(TIMx, T_force);
		
	}
	
	return	comm_EC->P_ZC_for_W;
}

void Detect_pos_edge(Commutate_EC_Def* comm_EC, uint16_t Uemf, uint16_t MidleUin, TIM_TypeDef* TIMx)
{
  	if (comm_EC->Flag_enable_ZC == 1)
	{
		if ((Uemf > MidleUin) && (comm_EC->cnt_detect_point |= 0))
		{
		  	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
			comm_EC->Flag_missed_ZC = 1;	// детектировано пересечение нуля!!!
		  	comm_EC->P_ZC = Calc_PZC(comm_EC, TIMx);
			comm_EC->Flag_enable_ZC = 0;	//запрет детектирования перехода через 0
		}
		else
		  	comm_EC->cnt_detect_point++;
	}
}

void Detect_neg_edge(Commutate_EC_Def* comm_EC, uint16_t Uemf, uint16_t MidleUin, TIM_TypeDef* TIMx)
{	
  	if (comm_EC->Flag_enable_ZC == 1)
	{
		if ((Uemf < MidleUin) && (comm_EC->cnt_detect_point |= 0))
		{
		  	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
			comm_EC->Flag_missed_ZC = 1;	// детектировано пересечение нуля!!!
		  	comm_EC->P_ZC = Calc_PZC(comm_EC, TIMx);
			comm_EC->Flag_enable_ZC = 0;	//запрет детектирования перехода через 0
		}
		else
		  	comm_EC->cnt_detect_point++;
	}
}

void	DetectZeroCrossOn(Commutate_EC_Def* comm_EC, uint16_t Uemf, uint16_t Uin, TIM_TypeDef* TIMx)
{
  	uint16_t MidleUin = (Uin >> 1) + add_Uin;	// offset ???
  	
  	if (comm_EC->Dir == POSITIVE_ROTATE)
	{
		switch(comm_EC->N_phase)
		{
		  	case	Q2Q3:
				Detect_pos_edge(comm_EC, Uemf, MidleUin, TIMx);  	
			break;
			case	Q2Q5:
			  	Detect_neg_edge(comm_EC, Uemf, MidleUin, TIMx);
			break;
			case	Q4Q5:
				Detect_pos_edge(comm_EC, Uemf, MidleUin, TIMx);
			break;
			case	Q4Q1:	
			  	Detect_neg_edge(comm_EC, Uemf, MidleUin, TIMx);
			break;
			case	Q6Q1:
			  	Detect_pos_edge(comm_EC, Uemf, MidleUin, TIMx);
			break;
			case	Q6Q3:
				Detect_neg_edge(comm_EC, Uemf, MidleUin, TIMx);
			break;
		}
	}
	else if (comm_EC->Dir == NEGATIVE_ROTATE)
	{
	  	switch(comm_EC->N_phase)
		{
		  	case	Q6Q3:
			 	Detect_pos_edge(comm_EC, Uemf, MidleUin, TIMx); 
			break;
			case	Q6Q1:
			  	Detect_neg_edge(comm_EC, Uemf, MidleUin, TIMx);
			break;
			case	Q4Q1:
			  	Detect_pos_edge(comm_EC, Uemf, MidleUin, TIMx);
			break;
			case	Q4Q5:
			  	Detect_neg_edge(comm_EC, Uemf, MidleUin, TIMx);
			break;
			case	Q2Q5:
			  	Detect_pos_edge(comm_EC, Uemf, MidleUin, TIMx);
			break;
			case	Q2Q3:
			  	Detect_neg_edge(comm_EC, Uemf, MidleUin, TIMx);
			break;
			
		}
	}
	else
	{
	  	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);	
	}
}

void Set_mass_commutation(uint32_t	T)
{
  	uint8_t i;
	
	for (i = 0; i < LEN_MASS_VEL; i++)
		Data_P_ZC[i] = T;
}

void	Init_meas_PforW(void)
{
	cnt_period_for_W = 0;
	T32_for_W = 0;
}

uint16_t	DetectZeroCross(uint16_t Uback, uint16_t Uin, Commutate_EC_Def* comm_EC, TIM_TypeDef* TIMx)
{
  	uint16_t Emf_back;
	
  	// детектирование перехода при измерениях эдс при включенном ключе
	//Emf_back = (uint16_t)(((long)Uback * (long)56426) >> 16);
        Emf_back = Uback;

	if (comm_EC->cnt_miss_detectZC == 0)  
		DetectZeroCrossOn(comm_EC, Emf_back, Uin, TIMx);
	else
		comm_EC->cnt_miss_detectZC--;
	
	return	Emf_back;
}
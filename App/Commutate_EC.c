// Commutate_EC.c

#include <Commutate_EC.h>
#include <Drive_param.h>
#define CR1_CKD_Mask                ((uint16_t)0x00FF)
#define CR1_CounterMode_Mask        ((uint16_t)0x038F)

Q_hi_def	Q1, Q3, Q5;
Q_low_def	Q2, Q4, Q6;

void	Init_gpioEC(void)
{	
	GPIO_InitTypeDef gpio;
	
	Q2.port = GPIOB;
	Q2.pin = GPIO_Pin_13;
	Q2.speed = GPIO_Speed_50MHz;
	Q2.mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = Q2.pin;
	gpio.GPIO_Speed =Q2.speed;
	gpio.GPIO_Mode = Q2.mode;
	GPIO_Init(Q2.port, &gpio);
	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
        
        Q4.port = GPIOB;
	Q4.pin = GPIO_Pin_14;
	Q4.speed = GPIO_Speed_50MHz;
	Q4.mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = Q4.pin;
	gpio.GPIO_Speed =Q4.speed;
	gpio.GPIO_Mode = Q4.mode;
	GPIO_Init(Q4.port, &gpio);
	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
	
	Q6.port = GPIOB;
	Q6.pin = GPIO_Pin_15;
	Q6.speed = GPIO_Speed_50MHz;
	Q6.mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = Q6.pin;
	gpio.GPIO_Speed =Q6.speed;
	gpio.GPIO_Mode = Q6.mode;
	GPIO_Init(Q6.port, &gpio);
	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
	
	Q1.port = GPIOA;
	Q1.pin = GPIO_Pin_8;
	Q1.speed = GPIO_Speed_50MHz;
	Q1.mode = GPIO_Mode_AF_PP;
	Q1.Duty = 0;
	gpio.GPIO_Pin = Q1.pin;
	gpio.GPIO_Speed =Q1.speed;
	gpio.GPIO_Mode = Q1.mode;
	GPIO_Init(Q1.port, &gpio);
	
	Q3.port = GPIOA;
	Q3.pin = GPIO_Pin_9;
	Q3.speed = GPIO_Speed_50MHz;
	Q3.mode = GPIO_Mode_AF_PP;
	Q3.Duty = 0;
	gpio.GPIO_Pin = Q3.pin;
	gpio.GPIO_Speed =Q3.speed;
	gpio.GPIO_Mode = Q3.mode;
	GPIO_Init(Q3.port, &gpio);
	
	Q5.port = GPIOA;
	Q5.pin = GPIO_Pin_10;
	Q5.speed = GPIO_Speed_50MHz;
	Q5.mode = GPIO_Mode_AF_PP;
	Q5.Duty = 0;
	gpio.GPIO_Pin = Q5.pin;
	gpio.GPIO_Speed =Q5.speed;
	gpio.GPIO_Mode = Q5.mode;
	GPIO_Init(Q5.port, &gpio);
}

void set_Q2_PP_output(void) 
{
        GPIO_InitTypeDef gpio;
  
        Q2.port = GPIOB;
	Q2.pin = GPIO_Pin_13;
	Q2.speed = GPIO_Speed_50MHz;
	Q2.mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = Q2.pin;
	gpio.GPIO_Speed =Q2.speed;
	gpio.GPIO_Mode = Q2.mode;
	GPIO_Init(Q2.port, &gpio);
        GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
}

void set_Q2_complementary(void)
{
        GPIO_InitTypeDef gpio;
        
        Q2.port = GPIOB;
	Q2.pin = GPIO_Pin_13;
	Q2.speed = GPIO_Speed_50MHz;
	Q2.mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Pin = Q2.pin;
	gpio.GPIO_Speed = Q2.speed;
	gpio.GPIO_Mode = Q2.mode;
	GPIO_Init(Q2.port, &gpio);
}

void set_Q4_PP_output(void)
{
        GPIO_InitTypeDef gpio;
        
        Q4.port = GPIOB;
	Q4.pin = GPIO_Pin_14;
	Q4.speed = GPIO_Speed_50MHz;
	Q4.mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = Q4.pin;
	gpio.GPIO_Speed =Q4.speed;
	gpio.GPIO_Mode = Q4.mode;
	GPIO_Init(Q4.port, &gpio);
        GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
}

void set_Q4_complementary(void)
{
        GPIO_InitTypeDef gpio;
        
        Q4.port = GPIOB;
	Q4.pin = GPIO_Pin_14;
	Q4.speed = GPIO_Speed_50MHz;
	Q4.mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Pin = Q4.pin;
	gpio.GPIO_Speed = Q4.speed;
	gpio.GPIO_Mode = Q4.mode;
	GPIO_Init(Q4.port, &gpio);
}

void set_Q6_PP_output(void)
{
        GPIO_InitTypeDef gpio;
        
        Q6.port = GPIOB;
	Q6.pin = GPIO_Pin_15;
	Q6.speed = GPIO_Speed_50MHz;
	Q6.mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = Q6.pin;
	gpio.GPIO_Speed =Q6.speed;
	gpio.GPIO_Mode = Q6.mode;
	GPIO_Init(Q6.port, &gpio);
        GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
}

void set_Q6_complementary(void)
{
        GPIO_InitTypeDef gpio;
        
        Q6.port = GPIOB;
	Q6.pin = GPIO_Pin_15;
	Q6.speed = GPIO_Speed_50MHz;
	Q6.mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Pin = Q6.pin;
	gpio.GPIO_Speed = Q6.speed;
	gpio.GPIO_Mode = Q6.mode;
	GPIO_Init(Q6.port, &gpio);
}

void Init_Control_EC(TIM_TypeDef* TIMx, uint16_t Period, uint16_t CCR_Val, uint16_t DeadTime)
{
  	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
		
	//TIM_DeInit(TIMx);
  	// Time base configuration 
	TIM_TimeBaseStructure.TIM_Period = Period;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;		
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
		
	// PWM1 Mode configuration: Channel1 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //TIM_OCPolarity_Low;
	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High; //TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
			
	// Channel 1
	TIM_OCInitStructure.TIM_Pulse = F_CLK / F_SWITCH - 1 - Q1.Duty;
	TIM_OC1Init(TIMx, &TIM_OCInitStructure);
	// Channel 2
	TIM_OCInitStructure.TIM_Pulse = F_CLK / F_SWITCH - 1 - Q3.Duty;
	TIM_OC2Init(TIMx, &TIM_OCInitStructure);
	// Channel 3
	TIM_OCInitStructure.TIM_Pulse = F_CLK / F_SWITCH - 1 - Q5.Duty;
	TIM_OC3Init(TIMx, &TIM_OCInitStructure);
	
	// Channel 4 for ADC
	// TIM1 channel4 configuration in PWM mode 
  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
  	TIM_OCInitStructure.TIM_Pulse = F_CLK / F_SWITCH - 1; 
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;         
	TIM_OC4Init(TIMx, &TIM_OCInitStructure);	
	
	// Automatic Output enable, Break, dead time and lock configuration
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;//TIM_LOCKLevel_OFF; //TIM_LOCKLevel_1;
	TIM_BDTRInitStructure.TIM_DeadTime = DeadTime;
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable; //TIM_Break_Enable; //TIM_Break_Disable;
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High; //TIM_BreakPolarity_Low;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
		
	TIM_BDTRConfig(TIMx, &TIM_BDTRInitStructure);
			
	// Master Mode selection 
	TIM_SelectOutputTrigger(TIMx, TIM_TRGOSource_Update);
		
	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
	
	// разрешаем генерацию PWM
	TIM_CtrlPWMOutputs(TIMx, ENABLE);
        
        TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
        
        // TIM1 enable counter 
	TIM_Cmd(TIMx, ENABLE);
}

void Commutate_phase_EC(TIM_TypeDef* TIMx, Commutate_EC_Def* comm_EC)
{
  	static uint8_t n_meas = 0;
	
  	if (comm_EC->Event == FORCE_TR)
	{
	  if (comm_EC->Dir == POSITIVE_ROTATE)
	  {
		GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET); // индикация переходов через 0
		Commutate_Pos(TIMx, comm_EC);
		comm_EC->Flag_enable_ZC = 1;	//разрешаем детектирование перехода через 0
		comm_EC->cnt_detect_point = 0;
	  }
	  else if (comm_EC->Dir == NEGATIVE_ROTATE)
	  {
		GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET); // индикация переходов через 0
		Commutate_Neg(TIMx, comm_EC);
		comm_EC->Flag_enable_ZC = 1;	//разрешаем детектирование перехода через 0
		comm_EC->cnt_detect_point = 0;
	  }
	  else
	  {	
                        set_Q2_PP_output();
                        set_Q4_PP_output();
                        set_Q6_PP_output();
			GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
			GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
			GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
			TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
			TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
			TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
	  }
	  comm_EC->Event = NOTHING;
	}
	if (comm_EC->Event == MEASURE)
	{
	  	if (n_meas == 0)
		{
		  	On_meas_r12_Q1Q4(TIMx);
			n_meas++;
			comm_EC->N_phase = Q1Q4meas;
		}
		else if (n_meas == 1)
		{
		  	On_meas_r13_Q5Q2(TIMx);
			n_meas++;
			comm_EC->N_phase = Q5Q2meas;
		}
		else if (n_meas == 2)
		{	
		  	On_meas_r23_Q3Q6(TIMx);
			comm_EC->N_phase = Q3Q6meas;
		  	n_meas++;
		}
		else
		{
		  	Off_all_Qn(TIMx);
			comm_EC->N_phase = STOP;
		  	n_meas = 0;
		}
	  	comm_EC->Event = NOTHING;
	}
}


void InterruptHandlerTIMUP(TIM_TypeDef* TIMx, Commutate_EC_Def* Comm_EC)
{
	Commutate_phase_EC(TIMx, Comm_EC);
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
}

void Set_Pos_Dir(Commutate_EC_Def* comm_EC)
{
  	if (comm_EC->Dir == STOP_ROTATE)
  		comm_EC->Dir = POSITIVE_ROTATE;
}

void Set_Neg_Dir(Commutate_EC_Def* comm_EC)
{
  	if (comm_EC->Dir == STOP_ROTATE)
  		comm_EC->Dir = NEGATIVE_ROTATE;
}

void Start_EC(Commutate_EC_Def* Comm_EC, TIM_TypeDef* TIMx)
{
  if ((Comm_EC->Dir == POSITIVE_ROTATE) || (Comm_EC->Dir == NEGATIVE_ROTATE)) {
    GPIO_WriteBit(Q2.port, Q2.pin, Bit_SET);
    GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
    GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET); // !!! for simple outputs !!!
    TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
    TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
    //set_Q4_complementary(); //!!! for complementary !!!
    TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1 - Q3.Duty);
    Comm_EC->N_phase = Q2Q3;
  }
  else {
    Comm_EC->N_phase = STOP;
    Comm_EC->Event = FORCE_TR;
    Comm_EC->Flag_enable_ZC = 1;
    Comm_EC->Flag_enable_feedback = 0;
    Init_gpioEC();
  }
}

void StartPrePos(Commutate_EC_Def* Comm_EC)
{
  Init_gpioEC();
  Comm_EC->N_phase = Q1Q4Q6;
  Comm_EC->Event = FORCE_TR;
}

void Force_Event(Commutate_EC_Def* Comm_EC)
{
  	Comm_EC->Event = FORCE_TR;
}

void Measure_Event(Commutate_EC_Def* Comm_EC)
{
  	Comm_EC->Event = MEASURE;
}

void Stop_EC(Commutate_EC_Def* Comm_EC)
{
  Init_gpioEC();
  Comm_EC->Dir = STOP_ROTATE;
  Comm_EC->N_phase = STOP;
  Comm_EC->Event = FORCE_TR;
}

void Commutate_Pos(TIM_TypeDef* TIMx, Commutate_EC_Def* comm_EC)
{
  	switch(comm_EC->N_phase)
	{
	  	case	Q1Q4Q6:
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
			TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
			TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
			
			GPIO_WriteBit(Q4.port, Q4.pin, Bit_SET);
			GPIO_WriteBit(Q6.port, Q6.pin, Bit_SET);
			TIM_SetCompare1(TIMx, (F_CLK / F_SWITCH - 1 - Q1.Duty));
			
		break;
		case	Q2Q3:
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
			TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_SET);
		  	TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1 - Q5.Duty);
                        /*set_Q4_PP_output();
                        GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
                        set_Q2_PP_output();
                        GPIO_WriteBit(Q2.port, Q2.pin, Bit_SET);
                        TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
                        TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
                        set_Q6_complementary();
                        TIM_SetCompare3(TIMx, (F_CLK / F_SWITCH - 1 - Q5.Duty));*/
			
			ADC_RegularChannelConfig(ADC1, Chanal_BACKEMF_B, 3, ADC_SampleTime_1Cycles5);
                        //ADC_InjectedChannelConfig(ADC2, Chanal_BACKEMF_B, 1, ADC_SampleTime_28Cycles5);
					
		  	comm_EC->N_phase = Q2Q5;
		break;
		case	Q2Q5:
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
		  	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_SET);
		  	TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1 - Q5.Duty);
                        
                        /*set_Q2_PP_output();
                        GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
                        set_Q4_PP_output();
                        GPIO_WriteBit(Q4.port, Q4.pin, Bit_SET);
                        TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
                        TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
                        set_Q6_complementary();
                        TIM_SetCompare3(TIMx, (F_CLK / F_SWITCH - 1 - Q5.Duty));*/
		  	
			ADC_RegularChannelConfig(ADC1, Chanal_BACKEMF_A, 3, ADC_SampleTime_1Cycles5);
                        //ADC_InjectedChannelConfig(ADC2, Chanal_BACKEMF_A, 1, ADC_SampleTime_28Cycles5);
			
		  	comm_EC->N_phase = Q4Q5;
		break;
		case	Q4Q5:
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
		  	TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_SET);
		 	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1 - Q1.Duty);
                        
                        /*set_Q6_PP_output();
                        GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
                        set_Q4_PP_output();
                        GPIO_WriteBit(Q4.port, Q4.pin, Bit_SET);
                        TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
                        TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
                        set_Q2_complementary();
                        TIM_SetCompare1(TIMx, (F_CLK / F_SWITCH - 1 - Q1.Duty));*/
			
			ADC_RegularChannelConfig(ADC1, Chanal_BACKEMF_C, 3, ADC_SampleTime_1Cycles5);
                        //ADC_InjectedChannelConfig(ADC2, Chanal_BACKEMF_C, 1, ADC_SampleTime_28Cycles5);
			
		  	comm_EC->N_phase = Q4Q1;
		break;
		case	Q4Q1:
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
			TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_SET);
		  	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1 - Q1.Duty);
                        
                        /*set_Q4_PP_output();
                        GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
                        set_Q6_PP_output();
                        GPIO_WriteBit(Q6.port, Q6.pin, Bit_SET);
                        TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
                        TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
                        set_Q2_complementary();
                        TIM_SetCompare1(TIMx, (F_CLK / F_SWITCH - 1 - Q1.Duty));*/
			
			ADC_RegularChannelConfig(ADC1, Chanal_BACKEMF_B, 3, ADC_SampleTime_1Cycles5);
                        //ADC_InjectedChannelConfig(ADC2, Chanal_BACKEMF_B, 1, ADC_SampleTime_28Cycles5);
			
		  	comm_EC->N_phase = Q6Q1;
		break;
		case	Q6Q1:
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
			TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_SET);
		  	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1 - Q3.Duty);
                        
                        /*set_Q2_PP_output();
                        GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
                        set_Q6_PP_output();
                        GPIO_WriteBit(Q6.port, Q6.pin, Bit_SET);
                        TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
                        TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
                        set_Q4_complementary();
                        TIM_SetCompare2(TIMx, (F_CLK / F_SWITCH - 1 - Q3.Duty));*/
			
			ADC_RegularChannelConfig(ADC1, Chanal_BACKEMF_A, 3, ADC_SampleTime_1Cycles5);
                        //ADC_InjectedChannelConfig(ADC2, Chanal_BACKEMF_A, 1, ADC_SampleTime_28Cycles5);
			
		  	comm_EC->N_phase = Q6Q3;
		break;
		case	Q6Q3:
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
			TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_SET);
		  	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1 - Q3.Duty);
                        
                        /*set_Q6_PP_output();
                        GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
                        set_Q2_PP_output();
                        GPIO_WriteBit(Q2.port, Q2.pin, Bit_SET);
                        TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
                        TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
                        set_Q4_complementary();
                        TIM_SetCompare2(TIMx, (F_CLK / F_SWITCH - 1 - Q3.Duty));*/
			
			ADC_RegularChannelConfig(ADC1, Chanal_BACKEMF_C, 3, ADC_SampleTime_1Cycles5);
                        //ADC_InjectedChannelConfig(ADC2, Chanal_BACKEMF_C, 1, ADC_SampleTime_28Cycles5);
			
		  	comm_EC->N_phase = Q2Q3;
		break;
		case	STOP:
                        set_Q2_PP_output();
                        set_Q4_PP_output();
                        set_Q6_PP_output();
			GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
		  	TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
		default:
                        set_Q2_PP_output();
                        set_Q4_PP_output();
                        set_Q6_PP_output();
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
		  	TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
		break;
	}
}

void Commutate_Neg(TIM_TypeDef* TIMx, Commutate_EC_Def* comm_EC)
{	
  	switch(comm_EC->N_phase)
	{
	  	case	Q1Q4Q6:
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
			TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
			TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
			
			GPIO_WriteBit(Q4.port, Q4.pin, Bit_SET);
			GPIO_WriteBit(Q6.port, Q6.pin, Bit_SET);
			TIM_SetCompare1(TIMx, (F_CLK / F_SWITCH - 1 - Q1.Duty));
			
		break;
		case	Q6Q3:
		  
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
		   	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_SET);
		  	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1 - Q1.Duty);
                        
                        /*set_Q4_PP_output();
                        GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
                        set_Q6_PP_output();
                        GPIO_WriteBit(Q6.port, Q6.pin, Bit_SET);
                        TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
                        TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
                        set_Q2_complementary();
                        TIM_SetCompare1(TIMx, (F_CLK / F_SWITCH - 1 - Q1.Duty));*/
			
			ADC_RegularChannelConfig(ADC1, Chanal_BACKEMF_B, 3, ADC_SampleTime_1Cycles5);
                        //ADC_InjectedChannelConfig(ADC2, Chanal_BACKEMF_B, 1, ADC_SampleTime_28Cycles5);
			
			comm_EC->N_phase = Q6Q1;
		break;
		case	Q6Q1:
		  
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
			TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_SET);
		  	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1 - Q1.Duty);
                        
                        /*set_Q6_PP_output();
                        GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
                        set_Q4_PP_output();
                        GPIO_WriteBit(Q4.port, Q4.pin, Bit_SET);
                        TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
                        TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
                        set_Q2_complementary();
                        TIM_SetCompare1(TIMx, (F_CLK / F_SWITCH - 1 - Q1.Duty));*/
	  		
			ADC_RegularChannelConfig(ADC1, Chanal_BACKEMF_C, 3, ADC_SampleTime_1Cycles5);
                        //ADC_InjectedChannelConfig(ADC2, Chanal_BACKEMF_C, 1, ADC_SampleTime_28Cycles5);
			
		  	comm_EC->N_phase = Q4Q1;
		break;
		case	Q4Q1:
		  
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
			TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_SET);
		   	TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1 - Q5.Duty);
                        
                        /*set_Q2_PP_output();
                        GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
                        set_Q4_PP_output();
                        GPIO_WriteBit(Q4.port, Q4.pin, Bit_SET);
                        TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
                        TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
                        set_Q6_complementary();
                        TIM_SetCompare3(TIMx, (F_CLK / F_SWITCH - 1 - Q5.Duty));*/
			
			ADC_RegularChannelConfig(ADC1, Chanal_BACKEMF_A, 3, ADC_SampleTime_1Cycles5);
                        //ADC_InjectedChannelConfig(ADC2, Chanal_BACKEMF_A, 1, ADC_SampleTime_28Cycles5);
			
		  	comm_EC->N_phase = Q4Q5;
		break;
		case	Q4Q5:
		  
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
		   	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_SET);
		  	TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1 - Q5.Duty);
                        /*
                        set_Q4_PP_output();
                        GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
                        set_Q2_PP_output();
                        GPIO_WriteBit(Q2.port, Q2.pin, Bit_SET);
                        TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
                        TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
                        set_Q6_complementary();
                        TIM_SetCompare3(TIMx, (F_CLK / F_SWITCH - 1 - Q5.Duty));*/
			
			ADC_RegularChannelConfig(ADC1, Chanal_BACKEMF_B, 3, ADC_SampleTime_1Cycles5);
                        //ADC_InjectedChannelConfig(ADC2, Chanal_BACKEMF_B, 1, ADC_SampleTime_28Cycles5);
			
		  	comm_EC->N_phase = Q2Q5;
		break;
		case	Q2Q5:
		  
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
		   	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_SET);
		  	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1 - Q3.Duty);
                        
                        /*set_Q6_PP_output();
                        GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
                        set_Q2_PP_output();
                        GPIO_WriteBit(Q2.port, Q2.pin, Bit_SET);
                        TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
                        TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
                        set_Q4_complementary();
                        TIM_SetCompare2(TIMx, (F_CLK / F_SWITCH - 1 - Q3.Duty));*/
			
			ADC_RegularChannelConfig(ADC1, Chanal_BACKEMF_C, 3, ADC_SampleTime_1Cycles5);
                        //ADC_InjectedChannelConfig(ADC2, Chanal_BACKEMF_C, 1, ADC_SampleTime_28Cycles5);
			
		  	comm_EC->N_phase = Q2Q3;
		break;
		case	Q2Q3:
		  
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
		   	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_SET);
		  	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1- Q3.Duty);
                        
                        /*set_Q2_PP_output();
                        GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
                        set_Q6_PP_output();
                        GPIO_WriteBit(Q6.port, Q6.pin, Bit_SET);
                        TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
                        TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
                        set_Q4_complementary();
                        TIM_SetCompare2(TIMx, (F_CLK / F_SWITCH - 1 - Q3.Duty));*/
			
			ADC_RegularChannelConfig(ADC1, Chanal_BACKEMF_A, 3, ADC_SampleTime_1Cycles5);
                        //ADC_InjectedChannelConfig(ADC2, Chanal_BACKEMF_A, 1, ADC_SampleTime_28Cycles5);
			  
		  	comm_EC->N_phase = Q6Q3;
		break;
		case	STOP:
                        set_Q2_PP_output();
                        set_Q4_PP_output();
                        set_Q6_PP_output();
			GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
		  	TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
		break;
		default:
                        set_Q2_PP_output();
                        set_Q4_PP_output();
                        set_Q6_PP_output();
		  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
		  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
		  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
		  	TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
		  	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
		break;
	}
}

void	Set_current(TIM_TypeDef* TIMx, Commutate_EC_Def* comm_EC, uint16_t pwm)
{
  	switch	(comm_EC->N_phase)
	{	
	  	case	Q1Q4Q6:
		  	TIM_SetCompare1(TIMx, pwm);
		break;
		case	Q2Q3:
			TIM_SetCompare2(TIMx, pwm);
		break;
		case	Q2Q5:
		  	TIM_SetCompare3(TIMx, pwm);
		break;
		case	Q4Q5:
		  	TIM_SetCompare3(TIMx, pwm);
		break;
		case	Q4Q1:
		  	TIM_SetCompare1(TIMx, pwm);
		break;
		case	Q6Q1:
		  	TIM_SetCompare1(TIMx, pwm);
		break;
		case	Q6Q3:
		  	TIM_SetCompare2(TIMx, pwm);
		break;
		case	Q1Q4meas:
		  	TIM_SetCompare1(TIMx, pwm);
		break;
		case	Q3Q6meas:
		  	TIM_SetCompare2(TIMx, pwm);
		break;
		case	Q5Q2meas:
		  	TIM_SetCompare3(TIMx, pwm);
		break;
		case	STOP:
		  	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
			TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
			TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
		break;
		default:
		  	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
			TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
			TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
	  	break;
	}
}

void	On_meas_r12_Q1Q4(TIM_TypeDef* TIMx)
{
  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
	TIM_SetCompare3(TIMx, 0);
	TIM_SetCompare2(TIMx, 0);
	GPIO_WriteBit(Q4.port, Q4.pin, Bit_SET);
	TIM_SetCompare1(TIMx, Q1.Duty);
}

void	On_meas_r13_Q5Q2(TIM_TypeDef* TIMx)
{
  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
  	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
   	TIM_SetCompare1(TIMx, 0);
  	TIM_SetCompare2(TIMx, 0);
  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_SET);
  	TIM_SetCompare3(TIMx, Q5.Duty);
}

void	On_meas_r23_Q3Q6(TIM_TypeDef* TIMx)
{
  	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
	TIM_SetCompare1(TIMx, 0);
	TIM_SetCompare3(TIMx, 0);
	GPIO_WriteBit(Q6.port, Q6.pin, Bit_SET);
	TIM_SetCompare2(TIMx, Q3.Duty);
}

void	Off_all_Qn(TIM_TypeDef* TIMx)
{
        set_Q2_PP_output();
        set_Q4_PP_output();
        set_Q6_PP_output();
  	GPIO_WriteBit(Q2.port, Q2.pin, Bit_RESET);
	GPIO_WriteBit(Q4.port, Q4.pin, Bit_RESET);
	GPIO_WriteBit(Q6.port, Q6.pin, Bit_RESET);
	TIM_SetCompare3(TIMx, F_CLK / F_SWITCH - 1);
	TIM_SetCompare1(TIMx, F_CLK / F_SWITCH - 1);
	TIM_SetCompare2(TIMx, F_CLK / F_SWITCH - 1);
}

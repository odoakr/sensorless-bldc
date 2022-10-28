//Commutate_EC.h

#include <stm32f10x_tim.h>
#include <stdio.h>
#include <stm32f10x_gpio.h>
#include <Adc.h>

// событи€ дл€ переключени€ фаз
#define	NOTHING		0	// нет событи€
#define FORCE_TR	1	// принудительное переключение фаз
#define	CROSSING	2	// пересечение 0 BackEMF
#define MEASURE		3	// измерение сопротивлени€ обмоток

// направление вращени€
#define	POSITIVE_ROTATE	1
#define	NEGATIVE_ROTATE	2
#define STOP_ROTATE		0

// фазы вращени€
#define STOP		0
#define	Q2Q3		1
#define Q2Q5		2
#define	Q4Q5		3
#define	Q4Q1		4
#define Q6Q1		5
#define	Q6Q3		6
#define Q1Q4Q6		7 // <- начальное положение дл€ вращени€
#define Q1Q4meas	8
#define Q3Q6meas	9
#define Q5Q2meas	10

typedef struct
{
  	uint16_t	Dir;
	uint16_t	N_phase;	
	uint16_t	Event;
	uint32_t	P_ZC;
	uint16_t	Wdrive;
	uint16_t	Flag_enable_ZC;
	uint32_t	T_next_commutation;
	uint32_t	T_force_commutation;
	uint16_t	Flag_enable_feedback;
	uint16_t	Flag_missed_ZC;
	uint16_t	Flag_ON_OFF;
	uint16_t	cnt_detect_point;
	uint32_t	P_ZC_for_W;
	uint16_t	Flag_fault_start;
	uint16_t	Flag_meas_R;
	uint16_t	Flag_sound;
	uint16_t	Status;
	uint16_t	cnt_valid_velocity;
	uint16_t	cnt_miss_zc;
	uint16_t	cnt_miss_detectZC;
}Commutate_EC_Def;

// нижние ключи
typedef struct
{
  	GPIO_TypeDef 		*port;
	uint16_t 			pin;
	GPIOSpeed_TypeDef	speed;
	GPIOMode_TypeDef	mode;
}Q_low_def;

// верхние ключи
typedef struct
{
  	GPIO_TypeDef		*port;
	uint16_t			pin;
	GPIOSpeed_TypeDef	speed;
	GPIOMode_TypeDef	mode;
	uint16_t			Duty;	
}Q_hi_def;

void Init_gpioEC(void);
void Init_Control_EC(TIM_TypeDef* TIMx, uint16_t Period, uint16_t CCR_Val, uint16_t DeadTime);
void InterruptHandlerTIMUP(TIM_TypeDef* TIMx, Commutate_EC_Def* Comm_EC);
void Commutate_phase_EC(TIM_TypeDef* TIMx, Commutate_EC_Def* Comm_EC);
void Start_EC(Commutate_EC_Def* Comm_EC, TIM_TypeDef* TIMx);
void StartPrePos(Commutate_EC_Def* Comm_EC);
void Force_Event(Commutate_EC_Def* Comm_EC);
void Measure_Event(Commutate_EC_Def* Comm_EC);
void Stop_EC(Commutate_EC_Def* Comm_EC);
void Commutate_Pos(TIM_TypeDef* TIMx, Commutate_EC_Def* comm_EC);
void Commutate_Neg(TIM_TypeDef* TIMx, Commutate_EC_Def* comm_EC);
void Set_Pos_Dir(Commutate_EC_Def* comm_EC);
void Set_Neg_Dir(Commutate_EC_Def* comm_EC);
void Set_current(TIM_TypeDef* TIMx, Commutate_EC_Def* comm_EC, uint16_t pwm);
void On_meas_r12_Q1Q4(TIM_TypeDef* TIMx);
void On_meas_r13_Q5Q2(TIM_TypeDef* TIMx);
void On_meas_r23_Q3Q6(TIM_TypeDef* TIMx);
void Off_all_Qn(TIM_TypeDef* TIMx);

void set_Q2_PP_output(void);
void set_Q2_complementary(void);
void set_Q4_PP_output(void);
void set_Q4_complementary(void);
void set_Q6_PP_output(void);
void set_Q6_complementary(void);

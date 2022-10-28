// Detect_zero_cross.h

void		DetectZeroCrossOn(Commutate_EC_Def* comm_EC, uint16_t Uemf, uint16_t Uin, TIM_TypeDef* TIMx);
uint32_t	Calc_PZC(Commutate_EC_Def* comm_EC, TIM_TypeDef* TIMx, uint16_t TimeEvent);
void 		Detect_pos_edge(Commutate_EC_Def* comm_EC, uint16_t Uemf, uint16_t MidleUin, TIM_TypeDef* TIMx);
void 		Detect_neg_edge(Commutate_EC_Def* comm_EC, uint16_t Uemf, uint16_t MidleUin, TIM_TypeDef* TIMx);
uint32_t	Filtr_T_ZC(uint32_t period);
void		Calc_next_commutation_detectZero(Commutate_EC_Def* comm_EC, uint32_t period);
void 		Set_mass_commutation(uint32_t	T);
void		Init_meas_PforW(void);
uint16_t	DetectZeroCross(uint16_t Uback, uint16_t Uin, Commutate_EC_Def* comm_EC, TIM_TypeDef* TIMx);
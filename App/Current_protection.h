// Current_protection.h
#include <stdint.h>
#include <Drive_param.h>
#include <stdbool.h>


bool		Current_protection(uint16_t Current);
bool		Current_protect_avg(float Current);
uint16_t	Filtr_Iin(void);
void		Save_Iin(uint16_t I);
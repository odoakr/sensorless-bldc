//Voltage_protection.h
#include <stdint.h>
#include <Drive_param.h>
#include <stdbool.h>

bool		Check_Voltage(uint16_t U);
uint16_t	Correct_Uin(uint16_t Uin, uint16_t Out);
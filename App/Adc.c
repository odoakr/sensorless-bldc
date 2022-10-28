// Adc.c

#include <Adc.h>
#include <Current_Reg.h>
#include <Commutate_EC.h>

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

__IO uint16_t ADCConvertedValue[3] = {0};

__IO uint16_t ADC_InjectedConvertedValueTab[LEN_BUFF_INJECT] = {0};
__IO uint16_t ADC_RegularConvertedValueTab[LEN_BUFF_INJECT] = {0};


extern Current_Reg_Def		Curr_reg;
extern Commutate_EC_Def		EC;

void	Adc_init(void)
{
  	ADC_InitTypeDef           ADC_InitStructure;
	DMA_InitTypeDef           DMA_InitStructure;

	// DMA1 channel1 configuration ----------------------------------------------
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 3;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  	// Enable DMA1 channel1 
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	ADC_DeInit(ADC1);
	// ADC1 configuration ------------------------------------------------------
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ADC_ExternalTrigConv_T1_CC1;//ADC_ExternalTrigConv_None;  
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 3; 
	ADC_Init(ADC1, &ADC_InitStructure);

	// ADC1 regular channel configuration 
	ADC_RegularChannelConfig(ADC1, Chanal_IIN, 		1, ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1, Chanal_UIN, 		2, ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1, Chanal_BACKEMF_A, 	3, ADC_SampleTime_28Cycles5/*ADC_SampleTime_1Cycles5*/);
		
	ADC_DeInit(ADC2);
	// ADC2 configuration ------------------------------------------------------
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1; 
	ADC_Init(ADC2, &ADC_InitStructure);
	
	// Set injected sequencer length 
  	ADC_InjectedSequencerLengthConfig(ADC2, 1);
  	// ADC1 injected channel Configuration  
  	ADC_InjectedChannelConfig(ADC2, Chanal_IIN/*Chanal_BACKEMF_A*/, 1, /*ADC_SampleTime_28Cycles5*/ADC_SampleTime_1Cycles5);
	
  	// ADC1 injected external trigger configuration 
  	ADC_ExternalTrigInjectedConvConfig(ADC2, ADC_ExternalTrigInjecConv_T1_CC4);
	ADC_ExternalTrigInjectedConvCmd(ADC2, ENABLE);
	
	// Disable JEOC interupt 
  	ADC_ITConfig(ADC2, ADC_IT_JEOC, DISABLE);
	
	ADC_AutoInjectedConvCmd(ADC2, DISABLE);
	
	// Enable ADC2 
	ADC_Cmd(ADC2, ENABLE);
	
	// Enable ADC1 DMA 
	ADC_DMACmd(ADC1, ENABLE);
  	
	// Enable ADC1 external trigger 
  	ADC_ExternalTrigConvCmd(ADC1, ENABLE);
	
	// Enable ADC1 
	ADC_Cmd(ADC1, ENABLE);
	
	/* Enable ADC1 reset calibaration register */   
  	ADC_ResetCalibration(ADC1);
  	/* Check the end of ADC1 reset calibration register */
  	while(ADC_GetResetCalibrationStatus(ADC1));

  	/* Start ADC1 calibaration */
  	ADC_StartCalibration(ADC1);
  	/* Check the end of ADC1 calibration */
  	while(ADC_GetCalibrationStatus(ADC1));
	/* Clear channel1 transfer complete flag */
  	DMA_ClearFlag(DMA1_FLAG_TC1); 
	
	// Enable ADC2 reset calibaration register   
  	ADC_ResetCalibration(ADC2);
  	// Check the end of ADC2 reset calibration register 
  	while(ADC_GetResetCalibrationStatus(ADC2));

  	// Start ADC2 calibaration 
  	ADC_StartCalibration(ADC2);
  	// Check the end of ADC2 calibration 
  	while(ADC_GetCalibrationStatus(ADC2));
	
}
/*
void InterruptHandlerADC(uint16_t Emf, uint16_t Uin)
{
  	ADC_ClearITPendingBit(ADC2, ADC_IT_JEOC);
}
*/
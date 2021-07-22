#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"
/*******************************************************************************
  * File Name          : debounce.c
  * Description        : debounce buttons that are pressed
  *
  * Author:              Vaibhav Ariyur
  * Date:                April 5th 2021
  ******************************************************************************
  */

deBounceInit(){
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_7|GPIO_PIN_8;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

int8_t deBounceReadPin(uint16_t pin){
	int32_t msTimeStamp=HAL_GetTick();
	int8_t pinStateWeAreLookingFor=0;
	int32_t stableInterval=30;
	int8_t pinState;//=HAL_GPIO_ReadPin(GPIOA, pin);
	while(HAL_GetTick() < (msTimeStamp+stableInterval)){
		pinState=HAL_GPIO_ReadPin(GPIOA, pin);
		if(pinState != pinStateWeAreLookingFor){
			pinStateWeAreLookingFor=!pinStateWeAreLookingFor;
			msTimeStamp=HAL_GetTick();
		}
	}
	return(pinStateWeAreLookingFor);
}



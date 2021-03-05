#include "stm32f0xx_hal.h"
#include "MagusEncoder.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

static uint8_t rgENC_State[7];
static uint16_t rgENC_Data[7];

static const int8_t transitions[] = {0,-1, 1, 0, 1, 0, 0,-1,-1, 0, 0, 1, 0, 1,-1, 0};

uint8_t getEncoderState3(){
  return HAL_GPIO_ReadPin(ENC3_A_GPIO_Port, ENC3_A_Pin) << 1 |
    HAL_GPIO_ReadPin(ENC3_B_GPIO_Port, ENC3_B_Pin);
}  

uint8_t getEncoderState5(){
  return HAL_GPIO_ReadPin(ENC5_A_GPIO_Port, ENC5_A_Pin) << 1 |
    HAL_GPIO_ReadPin(ENC5_B_GPIO_Port, ENC5_B_Pin);
}  

uint8_t getEncoderState6(){
  return HAL_GPIO_ReadPin(ENC6_A_GPIO_Port, ENC6_A_Pin) << 1 |
    HAL_GPIO_ReadPin(ENC6_B_GPIO_Port, ENC6_B_Pin);
}

void Encoders_Init (void) {
  // Reset encoder values
  rgENC_Data[0]  = 0x0000;
  rgENC_Data[1]  = 0x3FFF;
  rgENC_Data[2]  = 0x3FFF;
  rgENC_Data[3]  = 0x3FFF;
  rgENC_Data[4]  = 0x3FFF;
  rgENC_Data[5]  = 0x3FFF;
  rgENC_Data[6]  = 0x3FFF;

  // Configure interrupt encoders 3, 5 and 6
  rgENC_State[3] = getEncoderState3()<<2 | getEncoderState3();
  rgENC_State[5] = getEncoderState5()<<2 | getEncoderState5();
  rgENC_State[6] = getEncoderState6()<<2 | getEncoderState6();
	
  // Configure and start TIM encoders 1, 2 and 4
  __HAL_TIM_SET_COUNTER(&htim1, rgENC_Data[1]);
  __HAL_TIM_SET_COUNTER(&htim2, rgENC_Data[2]);
  __HAL_TIM_SET_COUNTER(&htim3, rgENC_Data[4]);
  HAL_TIM_Encoder_Start_IT(&htim1, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
}
  
void Encoder_Interrupt(uint8_t eid, uint8_t state) {
 /* https://electronics.stackexchange.com/questions/99915/stm32-rotary-encoder-with-hardware-interrupts */
  uint8_t previous = rgENC_State[eid];
  state |= (previous<<2)&0x0f;
  rgENC_Data[eid] += transitions[state];
  rgENC_State[eid] = state;
}

void HAL_GPIO_EXTI_Callback(uint16_t pin){
  switch(pin) {
  case ENC3_A_Pin: 
  case ENC3_B_Pin:
    Encoder_Interrupt(3, getEncoderState3());
    break;
  case ENC5_A_Pin: 
  case ENC5_B_Pin:
    Encoder_Interrupt(5, getEncoderState5());
    break;
  case ENC6_A_Pin:
  case ENC6_B_Pin:
    Encoder_Interrupt(6, getEncoderState6());
    break;
  case SPI_NCS_Pin:
    send_SPI();
    break;
  }
}
	
void Encoders_Main (void) {
  uint16_t usiSW_States;
	
  /* // Clear ChangeReady Pin (not used) */
  /* HAL_GPIO_WritePin(CHANGE_RDY_GPIO_Port, CHANGE_RDY_Pin, GPIO_PIN_RESET); */
	
  // Read switches
  usiSW_States  = (1-HAL_GPIO_ReadPin(ENC1_SW_GPIO_Port, ENC1_SW_Pin))<<0;
  usiSW_States |= (1-HAL_GPIO_ReadPin(ENC2_SW_GPIO_Port, ENC2_SW_Pin))<<1;
  usiSW_States |= (1-HAL_GPIO_ReadPin(ENC3_SW_GPIO_Port, ENC3_SW_Pin))<<2;
  usiSW_States |= (1-HAL_GPIO_ReadPin(ENC4_SW_GPIO_Port, ENC4_SW_Pin))<<3;
  usiSW_States |= (1-HAL_GPIO_ReadPin(ENC5_SW_GPIO_Port, ENC5_SW_Pin))<<4;
  usiSW_States |= (1-HAL_GPIO_ReadPin(ENC6_SW_GPIO_Port, ENC6_SW_Pin))<<5;
	
  // Update Switch states
  rgENC_Data[0] = usiSW_States;

  // Update TIM encoders
  rgENC_Data[1] = __HAL_TIM_GET_COUNTER(&htim1); 
  rgENC_Data[2] = __HAL_TIM_GET_COUNTER(&htim2);
  rgENC_Data[4] = __HAL_TIM_GET_COUNTER(&htim3);
}

void send_SPI(void) {	
  // Send data
  HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)&rgENC_Data, 14);
}


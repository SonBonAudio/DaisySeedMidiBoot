#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include "Daisy.h"
#include "device.h"
#include "Owl.h"
#include "Graphics.h"
//#include "errorhandlers.h"
#include "message.h"
#include "gpio.h"
#include "qspicontrol.h"
#include "ProgramManager.h"
#include "PatchRegistry.h"
#include "OpenWareMidiControl.h"
#include "SoftwareEncoder.hpp"


static SoftwareEncoder encoder(
  ENC_A_GPIO_Port, ENC_A_Pin,
  ENC_B_GPIO_Port, ENC_B_Pin, 
  ENC_CLICK_GPIO_Port, ENC_CLICK_Pin);


extern "C" void updateEncoderCounter(){
  encoder.updateCounter();
}

void setGateValue(uint8_t ch, int16_t value){
  switch(ch){
  case PUSHBUTTON:
  case BUTTON_A:
    HAL_GPIO_WritePin(GATE_OUT_GPIO_Port, GATE_OUT_Pin, value ? GPIO_PIN_SET :  GPIO_PIN_RESET);
    break;
  }
}

void setup(){
  HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET); // OLED off

  if (qspi_init(QSPI_MODE_MEMORY_MAPPED) != MEMORY_OK){
    // We can end here only if QSPI settings are misconfigured
    error(RUNTIME_ERROR, "Flash init error");
  }

  extern SPI_HandleTypeDef OLED_SPI;
  graphics.begin(&OLED_SPI);

  owl_setup();
}

void loop(void){
  encoder.debounce();
  int16_t enc_data[] = {
    int16_t(encoder.isFallingEdge() | encoder.isLongPress() << 1),
    int16_t(encoder.getValue())
  };
  graphics.params.updateEncoders(enc_data, 1);

  for(int i = 0; i < NOF_ADC_VALUES; ++i)
    graphics.params.updateValue(i, 4095 - (uint16_t(getAnalogValue(i)) >> 4));
  for(int i = NOF_ADC_VALUES; i < NOF_PARAMETERS; ++i) {
    graphics.params.updateValue(i, 0);
  }
  
  owl_loop();
}

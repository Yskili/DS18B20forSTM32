#include "delay.h"
//function microseconds
void SG_Delay(volatile uint32_t microseconds)
{
  uint32_t clk_cycle_start = DWT->CYCCNT; //search register in structure

  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000); //translate in microseconds

  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}

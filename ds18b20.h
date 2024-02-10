
#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include "stm32f1xx_hal.h"
#include "delay.h"

#define DS18B20_PORT GPIOA
#define DS18B20_PIN GPIO_PIN_15

uint8_t SG_DS18B20_Start(void);
int SG_DS18B20_Get_Temperature(void);

#endif /* INC_DS18B20_H_ */

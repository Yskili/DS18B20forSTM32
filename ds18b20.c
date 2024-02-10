#include "ds18b20.h"
uint8_t Presence=0;
uint8_t Rh_byte1,Rh_byte2,Temp_byte1,Temp_byte2;
uint16_t TEMP;
int Temperature=0;
int Humidity=0;
uint16_t tr=0;
void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) //Function lowering the level to low
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) //Function level up to high
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
uint8_t SG_DS18B20_Start(void) //Sensor initialization
{
	uint8_t Response=0;
	Set_Pin_Output(DS18B20_PORT,DS18B20_PIN);
	HAL_GPIO_WritePin (DS18B20_PORT,DS18B20_PIN,0);
	SG_Delay(480);
	Set_Pin_Input(DS18B20_PORT,DS18B20_PIN);
	SG_Delay(80);

	if (!(HAL_GPIO_ReadPin (DS18B20_PORT,DS18B20_PIN))) Response =1;  //Temperature sign selection
	else Response =-1;

	SG_Delay(400);

	return Response;
}

void SG_DS18B20_Write(uint8_t data) //Function write data
{
//	Set_Pin_Output(DS18B20_PORT,DS18B20_PIN);
	for (int i=0;i<8;i++)
	{

	    if ((data&(1<<i))!=0)
	    {
			Set_Pin_Output(DS18B20_PORT,DS18B20_PIN);
			HAL_GPIO_WritePin (DS18B20_PORT,DS18B20_PIN,0);
			SG_Delay(10);

			Set_Pin_Input(DS18B20_PORT,DS18B20_PIN);
			SG_Delay(50);
	    }
		else
		{
			Set_Pin_Output(DS18B20_PORT,DS18B20_PIN);
			HAL_GPIO_WritePin (DS18B20_PORT,DS18B20_PIN,0);
			SG_Delay(60);
			Set_Pin_Input(DS18B20_PORT,DS18B20_PIN);
		}
	    SG_Delay(1);
	}
}


uint8_t SG_DS18B20_Read(void) //Function read data
{
	uint8_t value=0;
	//Set_Pin_Input(DS18B20_PORT,DS18B20_PIN);
	for (int i=0;i<8;i++)
	{
		Set_Pin_Output(DS18B20_PORT,DS18B20_PIN);

		HAL_GPIO_WritePin (DS18B20_PORT,DS18B20_PIN,0);
		SG_Delay(1);

		Set_Pin_Input(DS18B20_PORT,DS18B20_PIN);
		SG_Delay(3);
		if (HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN))
		{
			value |= 1<<i;
		}
		SG_Delay(50);
	 }
	return value;
}

uint8_t SG_DS18B20_Request_Values(void) //Function request to the sensor
{
	  Presence = SG_DS18B20_Start();
	  SG_DS18B20_Write(0xCC);
	  SG_DS18B20_Write(0x44);
	  Presence = SG_DS18B20_Start();
	  SG_DS18B20_Write(0xCC);
	  SG_DS18B20_Write(0xBE);
}

int SG_DS18B20_Get_Temperature(void) //Function get temperature data
{
	Temp_byte1=SG_DS18B20_Read();
	Temp_byte2=SG_DS18B20_Read();
	TEMP=(Temp_byte2<<8)|Temp_byte1;
	Temperature=(int)TEMP;
	return Temperature;
}

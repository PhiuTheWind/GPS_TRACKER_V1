#include "I2C.h"
#include "TIM.h"

/**
 *  Names of events used in stdperipheral library
 *
 *      I2C_EVENT_MASTER_MODE_SELECT                          : EV5
 *      I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED            : EV6     
 *      I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED               : EV6
 *      I2C_EVENT_MASTER_BYTE_RECEIVED                        : EV7
 *      I2C_EVENT_MASTER_BYTE_TRANSMITTING                    : EV8
 *      I2C_EVENT_MASTER_BYTE_TRANSMITTED                     : EV8_2
 *     
 **/

 
 
void I2C_Config(I2C_TypeDef * I2Cx, uint16_t SDA, uint16_t SCL)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin  = SDA | SCL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* sEE_I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_Init(I2Cx, &I2C_InitStructure);
	
	I2C_Cmd(I2Cx, ENABLE);

}
void I2C_Transmit(I2C_TypeDef* I2Cx, u8 slaveAddr, u8 pBuffer, u8 WriteAddr)
{
	/* While the bus is busy */
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	/* Send START condition */
	I2C_GenerateSTART(I2Cx, ENABLE);
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	/* Send slave address for write */
	I2C_Send7bitAddress(I2Cx, slaveAddr, I2C_Direction_Transmitter);
	
	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	/* Clear EV6 by setting again the PE bit */
	I2C_Cmd(I2Cx, ENABLE);
	/* Send the slave internal address to write to */
	I2C_SendData(I2Cx, WriteAddr);
	/* Test on EV8 and clear it */
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	/* Clear EV8 by setting again the PE bit */
 /* Send the byte to be written */
 I2C_SendData(I2Cx, pBuffer);
 /* Test on EV8 and clear it */
 while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
 //I2C_SendData(I2Cx, DS1307_HIGH);
 /* Send STOP condition */
 I2C_GenerateSTOP(I2Cx, ENABLE);
}

//Function Read data to DS1307
void I2C_Receive(I2C_TypeDef* I2Cx, u8 slaveAddr, u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{
	/* While the bus is busy */
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	/* Send START condition */
	I2C_GenerateSTART(I2Cx, ENABLE);
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	/* Send slave address for write */
	I2C_Send7bitAddress(I2Cx, slaveAddr, I2C_Direction_Transmitter);
	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	/* Clear EV6 by setting again the PE bit */
	I2C_Cmd(I2Cx, ENABLE);
	/* Send the slave internal address to write to */
	I2C_SendData(I2Cx, ReadAddr);
	/* Test on EV8 and clear it */
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	/* Send START condition a second time */
	I2C_GenerateSTART(I2Cx, ENABLE);
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	/* Send slave address for read */
	I2C_Send7bitAddress(I2Cx, slaveAddr, I2C_Direction_Receiver);
	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	/* While there is data to be read */
	while(NumByteToRead)
	{
		if(NumByteToRead == 1)
		{
			/* Disable Acknowledgement */
			I2C_AcknowledgeConfig(I2Cx, DISABLE);
			/* Send STOP Condition */
			I2C_GenerateSTOP(I2Cx, ENABLE);
			/* Test on EV7 and clear it */
		}

		/* Test on EV7 and clear it */
		if(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			*pBuffer = I2C_ReceiveData(I2Cx);
			/* Point to the next location where the byte read will be saved */
			pBuffer++;
			/* Decrement the read bytes counter */
			NumByteToRead--;
			/* Read a byte from the slave */
		}
	}
	/* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
}

#ifdef I2C_DS3231

//BCD Converter
uint8_t bcd2bin(uint8_t val)
{
	return val - 6 * (val >> 4);
}
uint8_t bin2bcd(uint8_t val)
{
	return val + 6 * (val / 10);
}
// Init I2C for DS3231 
void DS3231_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	/* GPIOB configuration */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin  = DS3231_SDA | DS3231_SCL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* I2C configuration */
	I2C_DeInit(DS3231_I2C);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x01;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	
	//Remap SDA_PB7 -> SDA_PB9; SCL_PB6 -> SDA_PB8;
	GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);

	I2C_Init(DS3231_I2C, &I2C_InitStructure);
	I2C_Cmd(DS3231_I2C, ENABLE);
}	
//setup time Ex: DS3231_Time_Setup(12,0,0);
void DS3231_Time_Setup(u8 hour,u8 minute,u8 second)
{
	// Convert decimal to BCD
	second = bin2bcd(second);
	minute = bin2bcd(minute);
	hour = bin2bcd(hour);
	//Setup alarm second and Setup what kind alarm
	I2C_Transmit(DS3231_I2C, DS3231_Address, second, 0x00);
	//Setup alarm minute
	I2C_Transmit(DS3231_I2C, DS3231_Address, minute, 0x01);
	//Setup alarm hour
	I2C_Transmit(DS3231_I2C, DS3231_Address, hour, 0x02);	
	//Setup alarm hour
	I2C_Transmit(DS3231_I2C, DS3231_Address, 0x01, 0x03);	
}
//setup Alarm Ex: DS3231_Alarm_Setup(12+0x80,0+0x80,0);
void DS3231_Alarm_Setup(u8 alarm_hour,u8 alarm_minute,u8 alarm_second, u8 alarm_every)
{
	DS3231_Init();
	u8 alarm_date;
	//Setup alarm (control register)
	// SET INCN and A1IE to 0
	I2C_Transmit(DS3231_I2C, DS3231_Address, 0x00, Reg_Control);
	// SET INCN and A1IE to 1
	I2C_Transmit(DS3231_I2C, DS3231_Address, 0x05, Reg_Control);
	// Convert decimal to BCD
	// Setup alarm for every seconds match
	if(alarm_every == Alarm_EverySeconds)
	{alarm_second = bin2bcd(alarm_second);
	alarm_minute = bin2bcd(alarm_minute)+0x80;
	alarm_hour = bin2bcd(alarm_hour)+0x80;}
	
	// Setup alarm for every minutes, seconds match
	else if (alarm_every == Alarm_EveryMinutes)
	{alarm_second = bin2bcd(alarm_second);
	alarm_minute = bin2bcd(alarm_minute);
	alarm_hour = bin2bcd(alarm_hour)+0x80;}
	
	// Setup alarm for every hours,minutes,seconds match
	else if (alarm_every == Alarm_EveryHours)
	{alarm_second = bin2bcd(alarm_second);
	alarm_minute = bin2bcd(alarm_minute);
	alarm_hour = bin2bcd(alarm_hour);}
	
	alarm_date = 0x80 + 10;
	//Setup alarm second and Setup what kind alarm
	I2C_Transmit(DS3231_I2C, DS3231_Address, alarm_second, Reg_AL_Second);
	//Setup alarm minute
	I2C_Transmit(DS3231_I2C, DS3231_Address, alarm_minute, Reg_AL_Minute);
	//Setup alarm hour
	I2C_Transmit(DS3231_I2C, DS3231_Address, alarm_hour, Reg_AL_Hour);	
	//Setup alarm hour
	I2C_Transmit(DS3231_I2C, DS3231_Address, alarm_date, Reg_AL_Date);	
	//Setup what kind alarm
}
// Receive current time
void DS3231_Time_Receive(u8 * time)
{
	I2C_Receive(DS3231_I2C, DS3231_Address, time, Reg_Second, 3);
	time[0] = bcd2bin(time[0]);
	time[1] = bcd2bin(time[1]);
	time[2] = bcd2bin(time[2]);
}
// Receive current alarm
void DS3231_Alarm_Receive(u8 * time)
{
	I2C_Receive(DS3231_I2C, DS3231_Address, time, Reg_AL_Second, 3);
	time[0] = bcd2bin(time[0]);
	time[1] = bcd2bin(time[1]);
	time[2] = bcd2bin(time[2]);
}
// Turn off alarm
void DS3231_Alarm_Off(void)
{
	I2C_Transmit(DS3231_I2C, DS3231_Address, 0,0x0F);
}
// Check is alarm on
u8 DS3231_Alarm_Check(void)
{
	u8 pData[1];
	I2C_Receive(DS3231_I2C, DS3231_Address, pData, Reg_Status, 1); 
	if((pData[0] & 0x01) == 0x01)
	{
		// Turn off Alarm
		DS3231_Alarm_Off();	
		return 1;
	}
	return 0;	
}
u8 DS3231_Reg_Receive(u8 Reg)
{
	u8 pData[1];
	I2C_Receive(DS3231_I2C, DS3231_Address, pData, Reg, 1); 
	return pData[0];
}
void DS3231_Reg_Transmit(u8 Reg, u8 Data)
{
	I2C_Transmit(DS3231_I2C, DS3231_Address, Data, Reg);; 
}
float DS3231_Temperature(void)
{
	float Temp;
	
	DS3231_Reg_Transmit(Reg_Control, 0x25);
	
	u8 MSB = (DS3231_Reg_Receive(Reg_Temp_MSB));
	u8 LSB = DS3231_Reg_Receive(Reg_Temp_LSB)>>6;
	Temp = (float)LSB*0.25 + MSB;
	return Temp;
}

#endif


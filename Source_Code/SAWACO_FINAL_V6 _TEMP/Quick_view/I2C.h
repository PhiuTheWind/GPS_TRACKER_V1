#include "Common.h"

#define EV5 		I2C_EVENT_MASTER_MODE_SELECT
#define EV6_T 	I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED
#define EV6_R 	I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED
#define EV7 		I2C_EVENT_MASTER_BYTE_RECEIVED
#define EV8 		I2C_EVENT_MASTER_BYTE_TRANSMITTING
#define EV8_2 	I2C_EVENT_MASTER_BYTE_TRANSMITTED

void I2C_Config(I2C_TypeDef * I2Cx, uint16_t SDA, uint16_t SCL);
void I2C_Transmit(I2C_TypeDef* I2Cx, u8 slaveAddr, u8 pBuffer, u8 WriteAddr);
void I2C_Receive(I2C_TypeDef* I2Cx, u8 slaveAddr, u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);

#ifdef I2C_DS3231

	#define DS3231_SDA GPIO_Pin_9
	#define DS3231_SCL GPIO_Pin_8
	#define DS3231_I2C I2C1

	#define DS3231_Address 0xD0
	#define Reg_Control 0x0E
	#define Reg_Status 0x0F

	#define Reg_Second 0x00
	#define Reg_Minute 0x01
	#define Reg_Hour 0x02

	#define Reg_AL_Second 0x07
	#define Reg_AL_Minute 0x08
	#define Reg_AL_Hour 0x09
	#define Reg_AL_Date 0x0A
	
	#define Reg_Temp_MSB 0x11
	#define Reg_Temp_LSB 0x12
	
	#define Bit_AL 0x80
	#define Bit_Temp_Sign 0x80
	
	#define Alarm_EverySeconds 1
	#define Alarm_EveryMinutes 2
	#define Alarm_EveryHours 3

	
	void DS3231_Init(void);
	void DS3231_Time_Setup(u8 hour,u8 minute,u8 second);
	void DS3231_Time_Receive(u8 * time);
	void DS3231_Alarm_Receive(u8 * time);
	void DS3231_Alarm_Setup(u8 alarm_hour,u8 alarm_minute,u8 alarm_second, u8 alarm_every);
	void DS3231_Alarm_Off(void);
	u8 DS3231_Alarm_Check(void);
	u8 DS3231_Reg_Receive(u8 Reg);
	void DS3231_Reg_Transmit(u8 Reg, u8 Data);

	float DS3231_Temperature(void);
	
	uint8_t bcd2bin(uint8_t val);
	uint8_t bin2bcd(uint8_t val);

#endif

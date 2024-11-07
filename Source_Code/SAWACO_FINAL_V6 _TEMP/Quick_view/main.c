#include "main.h"

// MQTT ID, Fix position and Host
#define _Host 	"tcp://20.39.193.159:1883" //tcp://52.141.29.70:1883

#ifdef c01b //Lun
#define _ID 		"c01b" // Phan Dinh Giot 
#define GPS_FIXED_LATITUDE 10.800967
#define GPS_FIXED_LONGITUDE 106.6665059
#endif


#ifdef c02b //Cao
#define _ID 		"c02b" // 3/2 Nguyen Tri Phuong
#define GPS_FIXED_LATITUDE 10.767718
#define GPS_FIXED_LONGITUDE 106.667549
#endif


#ifdef _TEST
#define _ID 		"c02b" //"c02b"
#define GPS_FIXED_LATITUDE 10.767826 
#define GPS_FIXED_LONGITUDE 106.667605
#endif

//Hardware Topic 
#define _Topic_Buffer "SAWACO/" _ID

char _Topic_Stolen[7]="NA";
char _MQTT_Buffer[64] ="\0";

char buffer[64];
int _MQTT_ConnectChecker = 0 ;
int _MQTT_ReconnectCountdown = MQTT_RECONNECT_COUNTDOWN;

u8 BLE_Status = 0;

u8 time[3];
uint8_t DS3231_Time[3];
u8 alarm[3];
unsigned int Battery_Value=0;
float Battery_Percentage = 0,
			Battery_InitialPercentage = 0,
			Temperature = 0;

int main()
{
	//Timer Init
	Systick_Interrupt();
	TIM4_Init(); //Delay Timer
	LED_Init(); //PC13 -> LED
	
	LED_Blink(5);
	GPIOC->BRR = LED_Pin;
	
	ADC_Battery_Init(&Battery_Value);
	Delay_ms(100);
	Battery_InitialPercentage = Voltage_To_Percent(ADC_Battery_Read(Battery_Value,100));
	//Independent Watch Dog
	IWDG_Init(26000/*ms*/);
	// Int Awake and Sleep
	WKUP_EXTI0Mode();
	Sleep_Init();
	//I2C
	DS3231_Init();
	if (DS3231_Alarm_Check()==1) strcpy(_Topic_Stolen, "False");
//	else	strcpy(_Topic_Stolen, "True");
	Temperature = DS3231_Temperature();
	DS3231_Alarm_Setup(12,00,00, Alarm_EveryHours);
	Delay_ms(1000);
	//Bluetooth Init
	USARTBLE_Init();
	PinBLE_Init();
	BLE_Sleep();
	//GPS Init
	USARTGPS_Init();
	//4G Init
	USART4G_Init();
	Pin4G_Init();
	SIM_WKUP();
	MQTT_Reconnect();
	while(1)
	{
		IWDG_ReloadCounter(); //Reload Counter -> do this every 26s
		if(TimeSleep >= (TIME_SLEEP/Sys_Interrupt/*ms*/)) //If no Interrupt after 60s
		{Sleep();}
		
		/* Calculation */
		if(TimeCounter_1 >= (TIME_SEND_MQTT/6)/Sys_Interrupt) // Calculate Variables every 19s
		{
			TimeCounter_1 = 0;
			do{
					Battery_Percentage = Voltage_To_Percent(ADC_Battery_Read(Battery_Value,50));
				}while(fabs(Battery_InitialPercentage - Battery_Percentage) > 1);
			Battery_InitialPercentage = Battery_Percentage;
			GPS_Calculate();
		}
		
		/* Send to MQTT */
		if(TimeCounter >= TIME_SEND_MQTT/Sys_Interrupt) //Send MQTT every 60s
		{
			TimeCounter_1 = 0;
			if(MQTT_SendGPS_New()) MQTT_Reconnect();
			else TimeCounter = 0;
			memset(_MQTT_Buffer, '\0', sizeof(_MQTT_Buffer));
		}
	}
}

void LED_Blink(uint8_t Blink)
{
	while(Blink)
	{
		GPIO_SetBits(GPIOC,LED_Pin);
		Delay_ms(80);
		GPIO_ResetBits(GPIOC,LED_Pin);
		Delay_ms(80);
		Blink--;
	}
}
#ifdef USART4G

	void SIM_WKUP(void)
	{
		GPIO_ResetBits(GPIOA,PEN4G_Pin); //SIMCOM PWR Off
		LED_Blink(5);
		GPIOC->BRR = LED_Pin;
		GPIO_SetBits(GPIOA,PEN4G_Pin); //SIMCOM PWR On
		Delay_ms(2000);
		USART_WaitString(USART4G,"+CGEV: EPS PDN ACT 1",10000);
		USART_SendNCheck(USART4G,"AT","OK","ERROR",10,1000);
		USART_SendNCheck(USART4G,"ATE0","OK","ERROR",10,1000);
		USART_SendNCheck(USART4G,"AT+CNBP?","OK","ERROR",10,1000);
		USART_SendNCheck(USART4G,"AT+CPIN?","OK","ERROR",5,1000);
		USART_SendNCheck(USART4G,"AT+CIMI","OK","ERROR",5,1000);
	}

	void SIM_RESET(void)
	{
		USART_SendNCheck(USART4G,"AT+CRESET","OK","ERROR",5,1000);
		LED_Blink(4);
		Delay_ms(1000);
	}
	void MQTT_Reconnect(void)
	{
		_MQTT_ReconnectCountdown = MQTT_RECONNECT_COUNTDOWN; // Reload Reconnect Coundown
		IWDG_ReloadCounter(); //Reload Counter -> do this every 26s
		uint32_t ResponseTime = TIME_SLEEP;
		TIM_SetCounter(TIM_Timer,0);
		do{
			Delay_ms(500);
			IWDG_ReloadCounter(); //Reload Counter -> do this every 26s
			if(TIM_GetCounter(TIM_Timer)>=1000)
			{
				_MQTT_ConnectChecker = MQTT_Connect();
				ResponseTime--;
				if(ResponseTime == 0) Sleep(); 
				TIM_SetCounter(TIM_Timer,0);
			}
		}
		while(_MQTT_ConnectChecker != 0);
	}
	
	int MQTT_Connect(void)
	{
		int rssi,ber;
		
		//check signal
		Delay_ms(500);
		USART_SendNCheck(USART4G,"AT","OK","ERROR",10,1000);
		USART_SendNCheck(USART4G,"ATE0","OK","ERROR",10,1000);
		USART_SendNCheck(USART4G,"AT+CNBP?","OK","ERROR",10,1000);
		USART_SendNCheck(USART4G,"AT+CPIN?","OK","ERROR",5,1000);
		USART_SendNCheck(USART4G,"AT+CIMI","OK","ERROR",5,1000);
		
		USART_SendString(USART4G,"AT+CSQ");
		USART_WaitString(USART4G,"+CSQ: ",1000);
		memset(buffer, '\0', sizeof(buffer));
		if(USART_StoreString(USART4G,buffer) == TIMEOUT) return 12;
		sscanf(buffer,"%d,%d",&rssi,&ber);
		
		if(rssi > 98 || rssi <= 2) return rssi;
				
		USART_SendString(USART4G,"AT+CMQTTDISC=0,60");
		if(USART_Wait2String(USART4G,"OK","ERROR",1000) == TIMEOUT) return 5;

		USART_SendString(USART4G,"AT+CMQTTREL=0");
		if(USART_Wait2String(USART4G,"OK","ERROR",1000) == TIMEOUT) return 6;
		
		if(USART_SendNCheck(USART4G,"AT+CMQTTSTOP","ERROR","OK",5,1000)) return 7;
		
		if(USART_SendNCheck(USART4G,"AT+CMQTTSTART","OK","ERROR",5,1000)) return 8;
		if(USART_SendNCheck(USART4G,"AT+CMQTTACCQ=0,\"STM32F1\",0","OK","ERROR",5,1000)) return 9;
		if(USART_SendNCheck(USART4G,"AT+CMQTTCONNECT=0,\"" _Host "\",60,1","OK","ERROR",5,1000)) return 10;
		if(USART_WaitString(USART4G,"+CMQTTCONNECT: 0,0",1000)) return 11;
		return 0;
	}

	Status MQTT_Publish(char* topic, char* message)
	{
		char buffer[256] = "\0";
	
		sprintf(buffer,"AT+CMQTTTOPIC=0,%zu",strlen(topic));
		USART_SendNCheck(USART4G,buffer,">","OK",5,1000);
		if(USART_SendNCheck(USART4G,topic,"OK","ERROR",1,1000)) return FALSE;
		sprintf(buffer,"AT+CMQTTPAYLOAD=0,%zu",strlen(message));
		USART_SendNCheck(USART4G,buffer,">","ERROR",5,1000);
		if(USART_SendNCheck(USART4G,message,"OK","ERROR",1,1000)) return FALSE;
		if(USART_SendNCheck(USART4G,"AT+CMQTTPUB=0,1,60,1","OK","ERROR",5,1000)) return FALSE;
		if(USART_WaitString(USART4G,"+CMQTTPUB: 0,0",1000)) return FALSE;
		Delay_ms(1000);
		return TRUE;
	}
	
	Status MQTT_Subscribe(char* topic, char* receivebuffer)
	{
		char buffer[128] = "\0";
		sprintf(buffer,"AT+CMQTTSUBTOPIC=0,%zu,1",strlen(topic));
		if(USART_SendNCheck(USART4G,buffer,">","OK",5,1000)) return FALSE;
		if(USART_SendNCheck(USART4G,topic,"OK","ERROR",1,1000)) return FALSE;
		sprintf(buffer,"AT+CMQTTSUB=0");
		if(USART_SendNCheck(USART4G,buffer,"OK","ERROR",5,1000)) return FALSE;
		if(USART_WaitString(USART4G,"+CMQTTRXPAYLOAD:",1000)) return FALSE;
		if(USART_WaitString(USART4G,"\r\n",1000)) return FALSE;
		USART_StoreString(USART4G, receivebuffer);
		if(USART_WaitString(USART4G,"+CMQTTRXEND: 0",1000)) return FALSE;
		return TRUE;
	}
	
	void MQTT_SleepMode(void)
	{
		USART_SendString(USART4G,"AT+CSCLK=1");
		USART_WaitString(USART4G,"OK",1000);
		GPIO_ResetBits(GPIOA,PEN4G_Pin);
	}

	#ifdef USARTGPS
		Status MQTT_SendGPS_New(void)
		{
			
				/* Publish Format */
				memset(GPSjson_Buffer,'\0',sizeof(GPSjson_Buffer));
				sprintf(GPSjson_Buffer,
				"["
				"{\"name\":\"Latitude\",\"value\":\"%.6f\",\"timestamp\":\"%s\"}"
				",{\"name\":\"Longtitude\",\"value\":\"%.6f\",\"timestamp\":\"%s\"}"
				",{\"name\":\"Battery\",\"value\":\"%.2f\",\"timestamp\":\"%s\"}"
				",{\"name\":\"Temperature\",\"value\":\"%.2f\",\"timestamp\":\"%s\"}"
				",{\"name\":\"Stolen\",\"value\":\"%s\",\"timestamp\":\"%s\"}"
				//",{\"name\":\"Countdown\",\"value\":\"%d\",\"timestamp\":\"%s\"}"
				"]"
				,GPS_Latitude,Time_Sample
				,GPS_Longitude,Time_Sample	
				,Battery_Percentage,Time_Sample		
				,Temperature,Time_Sample
				,_Topic_Stolen,Time_Sample
				//,(TIME_SLEEP - TimeSleep*Sys_Interrupt),Time_Sample
				);
				/* Publish to MQTT */
				if(MQTT_Publish(_Topic_Buffer,GPSjson_Buffer)) return FALSE;
				
			
				return TRUE;
		}
	#endif
#endif


#ifdef USARTGPS
	void RXGPS_DMARST(void)
	{
		memset(RXGPS_Buffer,'\0',sizeof(RXGPS_Buffer));
		DMA_Cmd(DMA1_Channel6,DISABLE);
		DMA_SetCurrDataCounter(DMA1_Channel6, BUFFER_SIZE);
		DMA_Cmd(DMA1_Channel6,ENABLE);
	}

	void GPS_StandbyMode(void)
	{
		USART_SendString(USARTGPS,"$PAIR382,1*2E");
		USART_SendString(USARTGPS, "$PAIR003*39");
	}
	
	void GPS_WarmStart(void)
	{
		USART_SendString(USARTGPS,"$PAIR005*3F");
	}
	void GPS_SaveNavData(void)
	{
		USART_SendString(USARTGPS,"$PAIR511*3F");
	}
	void GPS_WKUP(void)
	{
		USART_SendString(USART2, "$PAIR002*38");
	}

	void GPS_EN_AllSatellite(void)
	{
		USART_SendString(USART2,"$PAIR066,1,1,1,1,1,0*3A");
	}

	void GPS_TimetoString(char *char_buffer, uint8_t *int_buffer)
	{
		sprintf(char_buffer,"%02d:%02d:%02d",int_buffer[2], int_buffer[1], int_buffer[0]);
	}
	void GPS_TimetoArray(uint8_t *int_buffer, float Time)
	{
		int hour = ((((int)Time)%1000000)/10000), minute = ((((int)Time)%10000)/100), second = (((int)Time)%100);
		hour = hour + 7; //Convert to VietNam time
		if(hour >= 24) hour = hour - 24;
		int_buffer[0] = (uint8_t) second;
		int_buffer[1] = (uint8_t) minute;
		int_buffer[2] = (uint8_t) hour;
	}
	float GPS_DMStoDecimal(float DMS) 
	{
			return ((float)floor(DMS / 100.0) + (fmod(DMS, 100.0) / 60));
	}
	void GPS_Calculate(void)
	{
			/* Read GPS */
			memset(RXGPS_Buffer,'\0',sizeof(RXGPS_Buffer)); // Reset Buffer
			float GPS_Tem_Latitude=0, GPS_Tem_Longitude=0;
			USART_WaitString(USARTGPS,"GNGGA,",1000);
			USART_StoreString(USARTGPS,RXGPS_Buffer);
			sscanf(RXGPS_Buffer, "%f,%f,%*[^,],%f",&GPS_TimeSample,&GPS_Tem_Latitude,&GPS_Tem_Longitude);
			
			/* Convert GPS DMS to Decimal */
			GPS_Latitude = GPS_DMStoDecimal(GPS_Tem_Latitude);
			GPS_Longitude = GPS_DMStoDecimal(GPS_Tem_Longitude);
			
			/* Convert Time in Float to int Array */
			GPS_TimetoArray(DS3231_Time,GPS_TimeSample);
			
			/* Is GPS Fix */
			if ((GPS_Latitude >= 0.000001 || GPS_Latitude <= -0.000001)&&GPS_IsFirstFix == false)
			{
				GPS_IsFirstFix = true;
				DS3231_Time_Setup(DS3231_Time[2], DS3231_Time[1], DS3231_Time[0]); 
			}
			else if (GPS_Latitude <= 0.000001 && GPS_Latitude >= -0.000001) GPS_IsFirstFix = false;
		
			/* Stolen and Keeping Awake */
			if (GPS_IsFirstFix)
			{
					/* If Previous Lat/long is 0, previous lat/long is initial lat/long */
					GPS_Pre_Latitude = GPS_Latitude;
					GPS_Pre_Longitude = GPS_Longitude;
					
					/* If Stolen */	
				 if(haversine(GPS_FIXED_LATITUDE, GPS_FIXED_LONGITUDE, GPS_Latitude, GPS_Longitude) >= 0.020)
				 {
					 /* Awake */
					 TimeSleep = 0;
					strcpy(_Topic_Stolen, "True");
				 }
				 else strcpy(_Topic_Stolen, "False");
			}
			else 
			{
					strcpy(_Topic_Stolen, "NA");
					DS3231_Time_Receive(DS3231_Time);
			}
			GPS_TimetoString(Time_Sample, DS3231_Time);			
	}
#endif
void Sleep(void)
{
		LED_Blink(5);
		TimeCounter = 0;
		Sleep_Latch();
}
void SysTick_Handler(void)
{
	TimeCounter++;
	TimeCounter_1++;
	TimeSleep++;
}


void EXTI0_IRQHandler(void)
{
	LED_Blink(1);
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		TimeSleep = 0;
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}


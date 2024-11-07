#ifndef MAIN_H
#define MAIN_H

#include "Mode.h"
#include "TIM.h"
#include "USART.h"
#include "I2C.h"
#include "ADC.h"
#include "stdbool.h"

// Define ID for 
#define c02b
#define BUFFER_SIZE 300
#define TIME_SEND_MQTT 60000 //1 min
#define TIME_SLEEP 600000 // 10 min

#define PI 3.14159265358979323846
#define R 6371.0 // Ban kinh Trai Ðat theo km

uint32_t RTC_DIV = 0;
uint32_t RTC_PulseCount = 0;
uint32_t TimeCounter = 60, TimeCounter_1 = 60, TimeSleep = 0;
bool GPS_IsFirstFix = false;

void LED_Blink(uint8_t Blink);
void Sleep(void);

#ifdef USART4G
	#define MQTT_RECONNECT_COUNTDOWN 10
	
	void SIM_WKUP(void);
	void SIM_RESET(void);
	void SIM_SleepMode(void);
	
	int MQTT_Connect(void);
	void MQTT_Reconnect(void);
	Status MQTT_Publish(char* topic, char* message);
	Status MQTT_Subscribe(char* topic, char* receivebuffer);
	
	void RX4G_DMARST(void);
	
	#ifdef USARTGPS
		Status MQTT_SendGPS(void);
		Status MQTT_SendGPS_New(void);
	#endif
#endif

#ifdef USARTGPS

	char RXGPS_Buffer[BUFFER_SIZE];
	void RXGPS_DMARST(void);
	void GPS_StandbyMode(void);
	void GPS_WarmStart(void);
	void GPS_SaveNavData(void);
	void GPS_WKUP(void);
	void GPS_EN_AllSatellite(void);
	
	float GPS_TimeSample=0;
	float GPS_Latitude=0, GPS_Longitude=0, GPS_Pre_Latitude=0, GPS_Pre_Longitude=0;
	char Time_Sample[16];
	char GPSjson_Buffer[BUFFER_SIZE];
	
	void GPS_Calculate(void);
	void GPS_TimetoString(char *char_buffer, uint8_t *int_buffer);
	void GPS_TimetoArray(uint8_t *int_buffer, float Time);
	float GPS_DMStoDecimal(float DMS);
	double toRadians(double degree) {
    return degree * (PI / 180);
	}

	// Hàm tính kho?ng cách b?ng công th?c Haversine
	double haversine(double lat1, double lon1, double lat2, double lon2) {
			// Chuy?n d?i t? d? sang radian
			lat1 = toRadians(lat1);
			lon1 = toRadians(lon1);
			lat2 = toRadians(lat2);
			lon2 = toRadians(lon2);

			// Tính toán chênh l?ch
			double dLat = lat2 - lat1;
			double dLon = lon2 - lon1;

			// Công th?c Haversine
			double a = sin(dLat / 2) * sin(dLat / 2) +
								 cos(lat1) * cos(lat2) *
								 sin(dLon / 2) * sin(dLon / 2);
			double c = 2 * atan2(sqrt(a), sqrt(1 - a));

			// Tính kho?ng cách
			double distance = R * c;

			return distance;
	}
#endif
	

#endif




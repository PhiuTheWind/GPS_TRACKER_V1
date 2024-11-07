#include "Common.h"

typedef enum
{
	TRUE = 0,
	FALSE = 1,
	TIMEOUT = 2
} Status;

#ifdef USART4G
	
	#define USART4G_GPIO GPIOA
	#define DTR4G_Pin GPIO_Pin_3
	#define NET4G_Pin GPIO_Pin_15
	#define PEN4G_Pin GPIO_Pin_8
	#define RX4G_Pin GPIO_Pin_10
	#define TX4G_Pin GPIO_Pin_9
	
	void USART4G_Init(void);
	void USART4G_DMA(uint32_t Rx_Buffer, uint16_t Rx_Size,uint32_t Tx_Buffer, uint16_t Tx_Size);
	void USART4G_NVIC(void);
	void Pin4G_Init(void);
	

#endif

#ifdef USARTGPS
	#define USARTGPS_GPIO GPIOA
	
	#define RXGPS_Pin GPIO_Pin_3
	#define TXGPS_Pin GPIO_Pin_2
	#define PPSGPS_Pin GPIO_Pin_0
	
	void USARTGPS_Init(void);
	void USARTGPS_DMA(uint32_t Rx_Buffer, uint16_t Rx_Size);
	void USARTGPS_NVIC(void);
	void PinGPS_Init(void);
	
#endif

#ifdef USARTBLE

	#define USARTBLE_GPIO GPIOB
	#define ENBLE_Pin GPIO_Pin_0
	#define RXBLE_Pin GPIO_Pin_11
	#define TXBLE_Pin GPIO_Pin_10
	
	void USARTBLE_Init(void);
	void PinBLE_Init(void);
	void BLE_Sleep(void);
	void BLE_Wakeup(void);
#endif


//void USARTDMA_SendString(USART_TypeDef *USARTx, uint8_t *buffer,const char *str);
void USART_SendString(USART_TypeDef *USARTx, const char *str);
Status USART_StoreString(USART_TypeDef *USARTx, char *buffer);
char* USART_GetString(USART_TypeDef *USARTx);
Status USART_WaitString(USART_TypeDef *USARTx, const char *str, int responsetime_ms);
Status USART_Wait2String(USART_TypeDef *USARTx, const char *str1, const char *str2, int responsetime_ms);
Status USART_SendNCheck(USART_TypeDef *USARTx,const char *str, const char *str1, const char *str2, uint8_t loop, int responsetime_ms);

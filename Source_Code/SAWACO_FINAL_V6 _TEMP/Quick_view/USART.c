#include "USART.h"

#ifdef USART4G
	void USART4G_Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO,ENABLE);
		/*GPIO Configure*/
		GPIO_InitStructure.GPIO_Pin = RX4G_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(USART4G_GPIO,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = TX4G_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(USART4G_GPIO,&GPIO_InitStructure);
		
		/*USART Configure*/
		USART_InitStructure.USART_BaudRate = USART4G_Baudrate;
		USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		
		USART_Init(USART4G,&USART_InitStructure);
		USART_Cmd(USART4G,ENABLE);
	}
	
	void USART4G_DMA(uint32_t Rx_Buffer, uint16_t Rx_Size,uint32_t Tx_Buffer, uint16_t Tx_Size)
	{
		USART_Cmd(USART4G,DISABLE);
		USART4G->CR3 |= (1<<6); //DMA for receiver
//	USART4G->CR3 |= (1<<7); //DMA for transmitter
		USART_Cmd(USART4G,ENABLE);
		/*DMA RX*/
		DMA_InitTypeDef DMA_InitStructure;

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
		DMA_DeInit(DMA1_Channel5);

		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART4G->DR;
		DMA_InitStructure.DMA_MemoryBaseAddr = Rx_Buffer;		
		DMA_InitStructure.DMA_BufferSize = Rx_Size;
		DMA_Init(DMA1_Channel5,&DMA_InitStructure);
		DMA_Cmd(DMA1_Channel5,ENABLE);
		
		DMA_DeInit(DMA1_Channel4);
		
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART4G->DR;
		DMA_InitStructure.DMA_MemoryBaseAddr = Tx_Buffer;		
		DMA_InitStructure.DMA_BufferSize = Tx_Size;
		DMA_Init(DMA1_Channel4,&DMA_InitStructure);
		DMA_Cmd(DMA1_Channel4,ENABLE);
	}
	void USART4G_NVIC(void)
	{
		NVIC_InitTypeDef NVIC_InitStructure;
		USART_Cmd(USART4G,DISABLE);
		/* Enable the USARTz Interrupt */
		USART_Cmd(USART4G,DISABLE);
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		USART_ITConfig(USART4G,USART_IT_RXNE,ENABLE);
		USART_Cmd(USART4G,ENABLE);
	}
	
	
	void Pin4G_Init(void)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
		GPIO_InitTypeDef GPIO_InitStructure;
		
		GPIO_InitStructure.GPIO_Pin = NET4G_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = PEN4G_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_ResetBits(GPIOA,	PEN4G_Pin);
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		
		
		GPIO_InitStructure.GPIO_Pin = DTR4G_Pin;
		GPIO_ResetBits(GPIOA,	DTR4G_Pin);
		GPIO_Init(GPIOB,&GPIO_InitStructure);
		
	}

#endif
	
	
	
#ifdef USARTGPS
	
	void USARTGPS_Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		/*GPIO Configure*/
		GPIO_InitStructure.GPIO_Pin = RXGPS_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(USARTGPS_GPIO,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = TXGPS_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(USARTGPS_GPIO,&GPIO_InitStructure);
		
		/*USART Configure*/
		USART_InitStructure.USART_BaudRate = USARTGPS_Baudrate;
		USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		
		USART_Init(USARTGPS,&USART_InitStructure);
		
		USART_Cmd(USARTGPS,ENABLE);
	}
	
	void USARTGPS_DMA(uint32_t Rx_Buffer, uint16_t Rx_Size)
	{
		USART_Cmd(USARTGPS,DISABLE);
		USARTGPS->CR3 |= (1<<6); //DMA for receiver
		USART_Cmd(USARTGPS,ENABLE);
			/*DMA RX*/
		DMA_InitTypeDef DMA_InitStructure;

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
		DMA_DeInit(DMA1_Channel6);

		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USARTGPS->DR;
		DMA_InitStructure.DMA_MemoryBaseAddr = Rx_Buffer;		
		DMA_InitStructure.DMA_BufferSize = Rx_Size;
		DMA_Init(DMA1_Channel6,&DMA_InitStructure);
		DMA_Cmd(DMA1_Channel6,ENABLE);
	}
	void USARTGPS_NVIC(void)
	{
		NVIC_InitTypeDef NVIC_InitStructure;
		/* Enable the USARTz Interrupt */
		USART_Cmd(USARTGPS,DISABLE);
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		USART_ITConfig(USARTGPS,USART_IT_RXNE,ENABLE);
		USART_Cmd(USARTGPS,ENABLE);
	}
	void PinGPS_Init(void)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		GPIO_InitTypeDef GPIO_InitStructure;
		
		GPIO_InitStructure.GPIO_Pin = PPSGPS_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
	}
	
#endif	

#ifdef USARTBLE
	
	void USARTBLE_Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		/*GPIO Configure*/
		GPIO_InitStructure.GPIO_Pin = RXBLE_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(USARTBLE_GPIO,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = TXBLE_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(USARTBLE_GPIO,&GPIO_InitStructure);
		
		/*USART Configure*/
		USART_InitStructure.USART_BaudRate = USARTBLE_Baudrate;
		USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		
		USART_Init(USARTBLE,&USART_InitStructure);
		
		USART_Cmd(USARTBLE,ENABLE);
	}
	void PinBLE_Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		/*GPIO Configure*/
		GPIO_InitStructure.GPIO_Pin = ENBLE_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(USARTBLE_GPIO,&GPIO_InitStructure);
		
		GPIO_SetBits(USARTBLE_GPIO, ENBLE_Pin);
	}
	void BLE_Sleep(void)
	{
		USART_SendString(USARTBLE, "AT");
		USART_SendString(USARTBLE, "AT+ROLE3");
		USART_SendString(USARTBLE, "AT+STARTEN1");
		//GPIO_ResetBits(USARTBLE_GPIO, ENBLE_Pin);
		GPIO_SetBits(USARTBLE_GPIO, ENBLE_Pin);
		USART_SendString(USARTBLE, "AT+SLEEP");
		
	}
	void BLE_Wakeup(void)
	{
		GPIO_SetBits(USARTBLE_GPIO, ENBLE_Pin);
		GPIO_ResetBits(USARTBLE_GPIO, ENBLE_Pin);
		GPIO_SetBits(USARTBLE_GPIO, ENBLE_Pin);
	}

#endif

void USART_SendString(USART_TypeDef *USARTx, const char *str) 
{	
		
   while (*str != '\0') 
		{
        USART_SendData(USARTx, (uint16_t)(*str));
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
        str++;
    }
		USART_SendData(USARTx,0x0D);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx,0x0A);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_ClearFlag(USARTx,USART_FLAG_TC);
}

Status USART_StoreString(USART_TypeDef *USARTx, char *buffer)
{
	int16_t ResponseTime = 3000;//ms
	TIM_SetCounter(TIM_Timer,0);
	do{		
		//StoreString
		while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET)
		{
			//Timeout
			if(TIM_GetCounter(TIM_Timer)>=1000)
			{
				ResponseTime--;
				if(ResponseTime == 0)
				{
					*buffer = '\0';
					return TIMEOUT; 
				}
				TIM_SetCounter(TIM_Timer,0);
			}
		}
		*buffer = (char)USART_ReceiveData(USARTx);
		buffer++;		
	}while(USART_ReceiveData(USARTx)!='\r');
	*buffer = '\0';
	return TRUE;
}
char* USART_GetString(USART_TypeDef *USARTx)
{
	char* buffer = NULL;
	uint16_t bufferIndex = 0;
	do{
		while(!(USARTx->SR & (1<<5)));
		buffer = realloc(buffer, (bufferIndex + 1) * sizeof(char));
		buffer[bufferIndex] = (char)USART_ReceiveData(USARTx);
    bufferIndex++;
	}while(USARTx->SR & (1<<4));
	buffer = realloc(buffer, bufferIndex * sizeof(char));
  buffer[bufferIndex] = '\0';
	return buffer;
}
Status USART_WaitString(USART_TypeDef *USARTx, const char *str, int responsetime_ms)
{
	char receivedChar;
	size_t strIndex = 0;
	uint16_t ResponseTime = 2000;//ms
	TIM_SetCounter(TIM_Timer,0);
	do{
		while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET)
		{
			if(TIM_GetCounter(TIM_Timer)>=1000)
			{
				ResponseTime--;
				if(ResponseTime == 0) return TIMEOUT; // Response time limit reached
				
				TIM_SetCounter(TIM_Timer,0);
			}
		}
		receivedChar = USART_ReceiveData(USARTx);
		if (receivedChar != str[strIndex]) 
		{
			strIndex=0;
		}
		else strIndex++;
	}while(strIndex != strlen(str));
	return TRUE;
}

Status USART_Wait2String(USART_TypeDef *USARTx, const char *str1, const char *str2, int responsetime_ms)//Wait until get str1 or str2. If str1->return 0 else return 1. Max response time = 1s
{
	char receivedChar;
	size_t strIndex1 = 0, strIndex2 = 0;
	uint16_t ResponseTime = 2000;
	TIM_SetCounter(TIM_Timer,0);
	while (1) 
	{
		while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET)
		{
			if(TIM_GetCounter(TIM_Timer)>=1000)
			{
				ResponseTime--;
				if(ResponseTime == 0) return TIMEOUT; 
				TIM_SetCounter(TIM_Timer,0);
			}
		}
		receivedChar = USART_ReceiveData(USARTx);
		
		if (receivedChar == str1[strIndex1]) {
				strIndex1++;
		} else {
				strIndex1 = 0;
		}
		
		if (receivedChar == str2[strIndex2]) {
				strIndex2++;
		} else {
				strIndex2 = 0;
		}
		
		if (strIndex1 == strlen(str1)) {
				return TRUE;
		} else if (strIndex2 == strlen(str2)) {
				return FALSE;
		}
   }
}

Status USART_SendNCheck(USART_TypeDef *USARTx,const char *str, const char *str1, const char *str2, uint8_t loop, int responsetime_ms) //Send string then wait until get str1 or str2. If str1->return 0 else loop until #loop = 0 or get str1. Max response time = 1s
{
	do{
		USART_SendString(USARTx,str);
		if(USART_Wait2String(USARTx,str1,str2,responsetime_ms))  loop--;
		else return TRUE;
	}while(loop);
	return FALSE;
}

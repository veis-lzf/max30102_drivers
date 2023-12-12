#include "pfv_usart.h"
#include "usart.h"
#include <string.h>

// 串口DMA接收缓冲区
uint8_t rx_buffer[RX_BUFFER_SIZE] =
{ 0 };
uint32_t rx_len = 0;

/* @bref 配置USART
 * @param usartx USART逻辑序号
 * @param baudrate USART速率
 * @retval 初始化成功返回USART_RET_OK，失败返回USART_RET_ERROR
 */
uint8_t pfv_USART_Config(uint8_t usartx, uint32_t baudrate)
{
	assert_param(usartx >= USART_Seq_NULL);

	switch (usartx)
	{
	case USART_Seq1:
	{

	}
		break;
	case USART_Seq2:
	{
		GPIO_InitTypeDef GPIO_InitStruct =
		{ 0 };
		__HAL_RCC_DMA1_CLK_ENABLE();
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();

		/**USART2 GPIO Configuration
		 PD5     ------> USART2_TX
		 PD6     ------> USART2_RX
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

		huart2.Instance = USART2;
		huart2.Init.BaudRate = baudrate;
		huart2.Init.WordLength = UART_WORDLENGTH_8B;
		huart2.Init.StopBits = UART_STOPBITS_1;
		huart2.Init.Parity = UART_PARITY_NONE;
		huart2.Init.Mode = UART_MODE_TX_RX;
		huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		huart2.Init.OverSampling = UART_OVERSAMPLING_16;
		huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
		huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
		if (HAL_UART_Init(&huart2) != HAL_OK)
		{
			return USART_RET_ERROR;
		}
#if USE_DMA_TX_RX
		/* USART2 DMA Init */
		/* USART2_RX Init */
		hdma_usart2_rx.Instance = DMA1_Stream5;
		hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
		hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart2_rx.Init.Mode = DMA_NORMAL;
		hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
		hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
		{
			return USART_RET_ERROR;
		}
		__HAL_LINKDMA(&huart2, hdmarx, hdma_usart2_rx);

		/* USART2_TX Init */
		hdma_usart2_tx.Instance = DMA1_Stream6;
		hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;
		hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart2_tx.Init.Mode = DMA_NORMAL;
		hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
		hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
		{
			return USART_RET_ERROR;
		}
		__HAL_LINKDMA(&huart2, hdmatx, hdma_usart2_tx);
#endif
		/* USART2 interrupt Init */
		HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
#if USE_DMA_TX_RX
		// 启动DMA接收
		HAL_UART_Receive_DMA(&huart2, rx_buffer, RX_BUFFER_SIZE);
		// 使能空闲帧中断
		__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
#else
			__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
		#endif
	}
		break;
	case USART_Seq3:
		break;
	case USART_Seq4:
		break;
	case USART_Seq5:
		break;
	case USART_Seq6:
		break;
	default:
		return USART_RET_ERROR;
	}
	return USART_RET_OK;
}

/* @bref 写入一个字节
 * @param usartx USART逻辑序号
 * @param data 待写入的数据
 * @retval 写入成功返回USART_RET_OK，失败返回USART_RET_ERROR
 */
uint8_t pfc_USART_WriteByte(uint8_t usartx, uint8_t data)
{
	assert_param(usartx >= USART_Seq_NULL);

	switch (usartx)
	{
	case USART_Seq1:
		break;
	case USART_Seq2:
	{
#if USE_DMA_TX_RX
		if (HAL_UART_Transmit_DMA(&huart2, &data, 1) != HAL_OK)
		{
			return USART_RET_ERROR;
		}
#endif
	}
		break;
	case USART_Seq3:
		break;
	case USART_Seq4:
		break;
	case USART_Seq5:
		break;
	case USART_Seq6:
		break;
	default:
		return USART_RET_ERROR;
	}
	return USART_RET_OK;

}

/* @bref 写入多字节
 * @param usartx USART逻辑序号
 * @param pbuf 待写入数据缓冲区起始地址
 * @param len 待写入数据长度
 * @retval 写入成功返回USART_RET_OK，失败返回USART_RET_ERROR
 */
uint8_t pfc_USART_WriteBuffer(uint8_t usartx, uint8_t *pbuf, uint32_t len)
{
	assert_param(usartx >= USART_Seq_NULL);
	assert_param(pbuf == NULL);

	switch (usartx)
	{
	case USART_Seq1:
		break;
	case USART_Seq2:
	{
#if USE_DMA_TX_RX
		if (HAL_UART_Transmit(&huart2, pbuf, len, 0xffff) != HAL_OK)
		{
			return USART_RET_ERROR;
		}
#endif
	}
		break;
	case USART_Seq3:
		break;
	case USART_Seq4:
		break;
	case USART_Seq5:
		break;
	case USART_Seq6:
		break;
	default:
		return USART_RET_ERROR;
	}
	return USART_RET_OK;

}

/* @bref 读入一个字节
 * @param usartx USART逻辑序号
 * @param p_data 数据缓冲区地址
 * @retval 读取成功返回USART_RET_OK，失败返回USART_RET_ERROR
 */
uint8_t pfv_USART_ReadByte(uint8_t usartx, uint8_t *p_data)
{
	assert_param(usartx >= USART_Seq_NULL);
	assert_param(p_data == NULL);

	switch (usartx)
	{
	case USART_Seq1:
		break;
	case USART_Seq2:
	{
#if USE_DMA_TX_RX
		if (HAL_UART_Receive_DMA(&huart2, p_data, 1) != HAL_OK)
		{
			return USART_RET_ERROR;
		}
#endif
	}
		break;
	case USART_Seq3:
		break;
	case USART_Seq4:
		break;
	case USART_Seq5:
		break;
	case USART_Seq6:
		break;
	default:
		return USART_RET_ERROR;
	}
	return USART_RET_OK;

}

/* @bref 读指定长度字节数据
 * @param usartx USART逻辑序号
 * @param p_data 数据缓冲区地址
 * @param len 要读取的数据长度
 * @retval 读取成功返回USART_RET_OK，失败返回USART_RET_ERROR
 */
uint8_t pfv_USART_ReadBuffer(uint8_t usartx, uint8_t *p_data, uint8_t len)
{
	assert_param(usartx >= USART_Seq_NULL);
	assert_param(p_data == NULL);

	switch (usartx)
	{
	case USART_Seq1:
		break;
	case USART_Seq2:
	{
#if USE_DMA_TX_RX
		if (HAL_UART_Receive_DMA(&huart2, p_data, len) != HAL_OK)
		{
			return USART_RET_ERROR;
		}
#endif
	}
		break;
	case USART_Seq3:
		break;
	case USART_Seq4:
		break;
	case USART_Seq5:
		break;
	case USART_Seq6:
		break;
	default:
		return USART_RET_ERROR;
	}
	return USART_RET_OK;

}

// 解析串口协议
extern void Receive_Handle(void);

// 串口中断回调函数
void UART2_Callback_Handler(void)
{
	HAL_UART_DMAStop(&huart2);
	rx_len = RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart2.hdmarx);
	Receive_Handle();
	memset(rx_buffer, 0, RX_BUFFER_SIZE);
	HAL_UART_Receive_DMA(&huart2, rx_buffer, RX_BUFFER_SIZE);
}


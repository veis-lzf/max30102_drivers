#ifndef __PFV_USART_H__
#define __PFV_USART_H__

#include "main.h"

// 使用DMA收发
#define USE_DMA_TX_RX	(1)

// 缓冲区大小
#define TX_BUFFER_SIZE	(512)
#define RX_BUFFER_SIZE	(1*1024L)

extern uint8_t rx_buffer[RX_BUFFER_SIZE];
extern uint32_t rx_len;

/* USART逻辑序号 */
typedef enum
{
	USART_Seq1,
	USART_Seq2,
	USART_Seq3,
	USART_Seq4,
	USART_Seq5,
	USART_Seq6,
	USART_Seq_NULL
} USART_Seq_t;

/* 状态码 */
typedef enum
{
	USART_RET_OK, USART_RET_BUSY, USART_RET_ERROR,
} USART_Status_t;

/* @bref 配置USART
 * @param usartx USART逻辑序号
 * @param baudrate USART速率
 * @retval 初始化成功返回USART_RET_OK，失败返回USART_RET_ERROR
 */
uint8_t pfv_USART_Config(uint8_t usartx, uint32_t baudrate);

/* @bref 写入一个字节
 * @param usartx USART逻辑序号
 * @param data 待写入的数据
 * @retval 写入成功返回USART_RET_OK，失败返回USART_RET_ERROR
 */
uint8_t pfc_USART_WriteByte(uint8_t usartx, uint8_t data);

/* @bref 写入多字节
 * @param usartx USART逻辑序号
 * @param pbuf 待写入数据缓冲区起始地址
 * @param len 待写入数据长度
 * @retval 写入成功返回USART_RET_OK，失败返回USART_RET_ERROR
 */
uint8_t pfc_USART_WriteBuffer(uint8_t usartx, uint8_t *pbuf, uint32_t len);

/* @bref 读入一个字节
 * @param usartx USART逻辑序号
 * @param p_data 数据缓冲区地址
 * @retval 读取成功返回USART_RET_OK，失败返回USART_RET_ERROR
 */
uint8_t pfv_USART_ReadByte(uint8_t usartx, uint8_t *p_data);

/* @bref 读指定长度字节数据
 * @param usartx USART逻辑序号
 * @param p_data 数据缓冲区地址
 * @param len 要读取的数据长度
 * @retval 读取成功返回USART_RET_OK，失败返回USART_RET_ERROR
 */
uint8_t pfv_USART_ReadBuffer(uint8_t usartx, uint8_t *p_data, uint8_t len);

/**
 * @bref 串口接收回调处理函数
 */
void UART2_Callback_Handler(void);

#endif /* __PFV_USART_H__ */

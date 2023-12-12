#include "debug.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

#define NO_OS 1

#ifdef OS_FREE_RTOS
#include "cmsis_os.h"
#endif

#if SHELL_ENABLE
#include "shell_port.h"
uint8_t shell_recv_buf = 0;
#endif

uint8_t DataRxBuffer[RX_BUF_MAX_LEN] =
{ 0 };
uint8_t dbg_rxdata = 0;
static uint32_t count = 0;

STRUCT_USARTx_Fram dbg_Fram_Record =
{ .pRxBuffer = DataRxBuffer, .InfAll = 0 };

// 调试等级
int dbg_level = DEBUG_USER;

static int OnCfgDebug(uint32_t vp_Type, uint32_t vp_P1, uint32_t vp_P2,
		uint32_t vp_P3)
{
	p_info("info:OnCfgDebug:Type=%ld,P1=%ld,P2=%ld,P3=%ld.", vp_Type, vp_P1,
			vp_P2, vp_P3);

	switch (vp_Type)
	{

	default:
		p_info("warn:PARAM INVALID!");
		break;
	}
	memset(DataRxBuffer, 0, RX_BUF_MAX_LEN);
	return 0;
}

// 格式：AT+cmdCfg=vl_CmdId,vl_Type,vl_P1,vl_P2,vl_P3
// 设置定时器命令：666
// 设置关闭和开始时间类型：1
// 开启时间和关闭时间：vl_P1，vl_P2
static int AT_DeviceHandle(const unsigned char *data_buf)
{
	count = 0;

	uint32_t i, vl_CmdId, vl_Type, vl_P1, vl_P2, vl_P3;
	uint32_t nlen = strlen((const char*) data_buf);
	char vl_FormateStr[64];

	vl_CmdId = 0;
	vl_Type = 0;
	vl_P1 = 0;
	vl_P2 = 0;
	vl_P3 = 0;

//	p_dbg("data_buf=%s", data_buf);
	if (!strstr((const char*) data_buf, "="))
		goto RETURN;

	memset(vl_FormateStr, 0, sizeof(vl_FormateStr) / sizeof(vl_FormateStr[0]));
	memcpy(vl_FormateStr, "AT+cmdCfg=%ld", strlen("AT+cmdCfg=%ld"));

//	p_dbg("nlen=%d", nlen);
	for (i = 0; i < nlen; i++)
	{
		if ((',' == data_buf[i]) && (i < nlen - 1))
			memcpy(vl_FormateStr + strlen(vl_FormateStr), ",%ld",
					strlen(",%ld"));
	}
//	p_dbg("vl_FormateStr=%s", vl_FormateStr);
	sscanf((const char*) data_buf, vl_FormateStr, &vl_CmdId, &vl_Type, &vl_P1,
			&vl_P2, vl_P3);

	memset((char*) data_buf, 0, nlen);

	p_dbg("vl_CmdId=%ld, vl_Type=%ld, vl_P1=%ld, vl_P2=%ld, vl_P3=%ld",
			vl_CmdId, vl_Type, vl_P1, vl_P2, vl_P3);

	return OnCfgDebug(vl_Type, vl_P1, vl_P2, vl_P3);

	RETURN: return -1;
}

extern volatile uint32_t tick_ms;

/**
 * @brief      获取系统时间基准
 *
 * @return     返回系统CPU运行时间
 */

uint32_t os_time_get(void)
{
#if NO_OS
	return HAL_GetTick();
#endif
	return tick_ms;
}

extern void UART2_Callback_Handler(void);
/**
 * @brief     串口接收回调函数
 *
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == DEBUG_USART)
	{
		if (dbg_rxdata != 0x0d && dbg_rxdata != 0x0a)
		{
			DataRxBuffer[count++] = dbg_rxdata;
		}
		else if (dbg_rxdata != 0x0a)
		{
			DataRxBuffer[count] = '\0';
			AT_DeviceHandle(DataRxBuffer);
		}
		HAL_UART_Receive_IT(huart, &dbg_rxdata, 1);
	}

//	if(huart->Instance == USART2)
//	{
//		UART2_Callback_Handler();
//	}
}

/* 重定向printf到串口输出 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif 

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart3, (uint8_t*) &ch, 1, 0xFFFF);
	return ch;
}

void DebugLevelInit(iot_log_level_t level)
{
	dbg_level = level;
}

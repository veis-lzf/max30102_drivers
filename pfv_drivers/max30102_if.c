/*
 * max30102_if.c
 *
 *  Created on: Dec 12, 2023
 *      Author: W10
 */
#include "max30102_if.h"
#include "pfv_max30102.h"
#include "algorithm.h"

// 范围限制
#define MAX_HEART_RATE	120
#define MAX_SPO2_VALUE	101

// 数据有效
#define DATA_VALID		0X01
// 数据无效
#define DATA_INVALID	0X00

// 数据缓冲区
#define MAX_BUFFER_SIZE		BUFFER_SIZE
uint32_t aun_ir_red_buffer[2][MAX_BUFFER_SIZE];

uint8_t Heart_Sensor_Init(void)
{
	if (MAX30102_RET_ERROR == max30102_init())
		return Heartsensor_RET_ERROR;

	return Heartsensor_RET_OK;
}

/* @bref 获取心跳数据和血氧数据
 * @param aun_buffer 缓冲区地址
 * @param buffer_length 缓冲区长度
 * @param uch_dummy 舍弃的数据长度
 * @retval 读取成功返回Heartsensor_RET_OK，失败返回Heartsensor_RET_ERROR
 */
uint8_t Get_HeartRate_and_Oxygen(
		/*uint32_t **aun_buffer, uint16_t buffer_length,*/uint16_t uch_dummy)
{
	if (uch_dummy >= MAX_BUFFER_SIZE)
		uch_dummy = 0;

	// variables to calculate the on-board LED brightness that reflects the heartbeats
	//uint32_t un_min, un_max, un_prev_data;
	int8_t ch_spo2_valid; // indicator to show if the SP02 calculation is valid
	int8_t ch_hr_valid; // indicator to show if the heart rate calculation is valid
	uint16_t i = 0; // buffer length of 100 stores 5 seconds of samples running at 100sps
	uint32_t timeout = 0;
	//read the first 500 samples, and determine the signal range
	for (i = uch_dummy; i < MAX_BUFFER_SIZE; i++)
	{
		while (MAX30102_INT == MAX30102_INT_TRIG) //wait until the interrupt pin asserts
		{
			if (timeout++ > MAX_OPT_WAIT_TIMEOUT)
				return Heartsensor_RET_ERROR;
		}

		max30102_FIFO_ReadBytes(REG_FIFO_DATA);
		aun_ir_red_buffer[1][i - uch_dummy] =
				(long) ((long) ((long) g_MAX30102_Dev.m_sensor_data.m_fifo_data[0] & 0x03) << 16)
						| (long) g_MAX30102_Dev.m_sensor_data.m_fifo_data[1] << 8
						| (long) g_MAX30102_Dev.m_sensor_data.m_fifo_data[2]; // Combine values to get the actual number

		aun_ir_red_buffer[0][i - uch_dummy] =
				(long) ((long) ((long) g_MAX30102_Dev.m_sensor_data.m_fifo_data[3] & 0x03) << 16)
						| (long) g_MAX30102_Dev.m_sensor_data.m_fifo_data[4] << 8
						| (long) g_MAX30102_Dev.m_sensor_data.m_fifo_data[5]; // Combine values to get the actual number
	}
	if (uch_dummy > 0)
	{
		for (i = MAX_BUFFER_SIZE - uch_dummy; i < MAX_BUFFER_SIZE; i++)
		{
			while (MAX30102_INT == MAX30102_INT_TRIG) //wait until the interrupt pin asserts
			{
				if (timeout++ > MAX_OPT_WAIT_TIMEOUT)
					return Heartsensor_RET_ERROR;
			}

			max30102_FIFO_ReadBytes(REG_FIFO_DATA);
			aun_ir_red_buffer[1][i] =
					(long) ((long) ((long) g_MAX30102_Dev.m_sensor_data.m_fifo_data[0] & 0x03) << 16)
							| (long) g_MAX30102_Dev.m_sensor_data.m_fifo_data[1] << 8
							| (long) g_MAX30102_Dev.m_sensor_data.m_fifo_data[2]; // Combine values to get the actual number

			aun_ir_red_buffer[0][i] =
					(long) ((long) ((long) g_MAX30102_Dev.m_sensor_data.m_fifo_data[3] & 0x03) << 16)
							| (long) g_MAX30102_Dev.m_sensor_data.m_fifo_data[4] << 8
							| (long) g_MAX30102_Dev.m_sensor_data.m_fifo_data[5]; // Combine values to get the actual number

//			if(g_MAX30102_Dev.m_sensor_data.m_HeartRate > 0 && g_MAX30102_Dev.m_sensor_data.m_HeartRate < MAX_HEART_RATE)
//				printf("%ld\n", g_MAX30102_Dev.m_sensor_data.m_HeartRate);
		}
	}
	//calculate heart rate and SpO2 after first 500 samples (first 5 seconds of samples)
	maxim_heart_rate_and_oxygen_saturation(&aun_ir_red_buffer[0][0],
			MAX_BUFFER_SIZE, &aun_ir_red_buffer[1][0],
			&g_MAX30102_Dev.m_sensor_data.m_SpO2, &ch_spo2_valid,
			&g_MAX30102_Dev.m_sensor_data.m_HeartRate, &ch_hr_valid);

	if (ch_hr_valid && ch_spo2_valid && ch_spo2_valid
	 && (g_MAX30102_Dev.m_sensor_data.m_HeartRate < MAX_HEART_RATE)
	 && (g_MAX30102_Dev.m_sensor_data.m_SpO2 < MAX_SPO2_VALUE))
		return Heartsensor_RET_OK;

	return Heartsensor_RET_ERROR;
}


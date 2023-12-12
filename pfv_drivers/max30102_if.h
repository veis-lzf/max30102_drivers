/*
 * max30102_if.h
 *
 *  Created on: Dec 12, 2023
 *      Author: W10
 */

#ifndef MAX30102_IF_H_
#define MAX30102_IF_H_

#include "main.h"
#include "pfv_max30102.h"

typedef enum
{
	Heartsensor_RET_OK = 0x00, Heartsensor_RET_ERROR,
} Heart_Sensor_Status_t;

/* @bref 初始化心率传感器
 * @retval none
 */
uint8_t Heart_Sensor_Init(void);

/* @bref 获取心跳数据和血氧数据
 * @param aun_buffer 缓冲区地址
 * @param buffer_length 缓冲区长度
 * @param uch_dummy 舍弃的数据长度
 * @retval 读取成功返回Heartsensor_RET_OK，失败返回Heartsensor_RET_ERROR
 */
uint8_t Get_HeartRate_and_Oxygen(
/*uint32_t **aun_buffer, uint16_t buffer_length,*/uint16_t uch_dummy);

#endif /* MAX30102_IF_H_ */

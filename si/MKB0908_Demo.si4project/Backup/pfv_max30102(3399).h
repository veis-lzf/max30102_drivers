/*
 * pfv_max30102.h
 *
 *  Created on: Dec 10, 2023
 *      Author: W10
 */

#ifndef PFV_MAX30102_H_
#define PFV_MAX30102_H_

#include "main.h"

/* 定义使用模拟I2C还是硬件I2C */
#define USE_SOFTWARE_I2C	0x00
#define USE_HARDWARE_I2C 	0x01
#define MAX30102_I2C_MODE	USE_HARDWARE_I2C

/* 设备地址 */
#define max30102_WR_address 0xAE
/* 中断寄存器 */
#define INTERRUPT_REG  		0x00
/* I2C读写控制 */
#define I2C_WR				0x00
#define I2C_RD				0x01

/* 校准状态码 */
typedef enum
{
	MAX30102_RET_OK = 0x00,
	MAX30102_RET_BUSY = 0x01,
	MAX30102_RET_ERROR = 0x02
} MAX30102_RET_Status_t;

typedef struct
{
	uint8_t m_SystolicBloodPressure; /* 收缩压 */
	uint8_t m_Diastolic_BloodPressure; /* 舒张压 */
	uint8_t m_PulseRate; /* 脉搏率 */
} MAX30102_Data_t;

/* 传感器数据类型 */
#ifdef __GNUC__
	#pragma pack(1)
#endif
typedef struct
{
	uint8_t m_address; /* 设备地址 */
	uint8_t m_i2cx; /* I2C逻辑序号 */
	MAX30102_Data_t m_sensor_data;
} MAX30102_Device_t;

extern MAX30102_Device_t g_MAX30102_Dev;

#ifdef __GNUC__
	#pragma pack()
#endif


#endif /* PFV_MAX30102_H_ */

/*
 * pfv_max30102.h
 *
 *  Created on: Dec 10, 2023
 *      Author: W10
 */

#ifndef PFV_MAX30102_H_
#define PFV_MAX30102_H_


/* 校准状态码 */
typedef enum
{
	Calibration_OK = 0x00,
	Calibration_BUSY = 0x01,
	Calibration_ERROR = 0x02
} Calibration_Status_t;

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
	uint8_t m_calibration_status; /* 校准状态 */
	uint8_t m_erase_status; /* 擦除EEPROM状态 */
	uint8_t m_dev_status; /* 工作状态 */
	MAX30102_Data_t m_sensor_data;
} MAX30102_Device_t;

#ifdef __GNUC__
	#pragma pack()
#endif


#endif /* PFV_MAX30102_H_ */

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
#define I2C_WRITE_ADDR 		0xAE
#define I2C_READ_ADDR 		0xAF

/* I2C读写控制 */
#define I2C_WR				0x00
#define I2C_RD				0x01

#ifdef SUPPORT_INT_MODE
#define MAX30102_INT PBin(9)
#endif

// register addresses
#define REG_INTR_STATUS_1 	0x00
#define REG_INTR_STATUS_2 	0x01
#define REG_INTR_ENABLE_1 	0x02
#define REG_INTR_ENABLE_2 	0x03
#define REG_FIFO_WR_PTR 	0x04
#define REG_OVF_COUNTER 	0x05
#define REG_FIFO_RD_PTR 	0x06
#define REG_FIFO_DATA 		0x07
#define REG_FIFO_CONFIG 	0x08
#define REG_MODE_CONFIG 	0x09
#define REG_SPO2_CONFIG 	0x0A
#define REG_LED1_PA 		0x0C
#define REG_LED2_PA 		0x0D
#define REG_PILOT_PA 		0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 		0x1F
#define REG_TEMP_FRAC 		0x20
#define REG_TEMP_CONFIG 	0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 			0xFE
#define REG_PART_ID 		0xFF


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

void max30102_init(void);  
void max30102_reset(void);
uint8_t max30102_Bus_Write(uint8_t Register_Address, uint8_t Word_Data);
uint8_t max30102_Bus_Read(uint8_t Register_Address);
void max30102_FIFO_ReadWords(uint8_t Register_Address,uint16_t  Word_Data[][2],uint8_t count);
void max30102_FIFO_ReadBytes(uint8_t Register_Address,uint8_t* Data);
void maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data);
void maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data);
void maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led);


#endif /* PFV_MAX30102_H_ */

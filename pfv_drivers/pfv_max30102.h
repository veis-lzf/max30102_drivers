/*
 * pfv_max30102.h
 *
 *  Created on: Dec 10, 2023
 *      Author: W10
 */

#ifndef PFV_MAX30102_H_
#define PFV_MAX30102_H_

#include "main.h"

#define SUPPORT_INT_PIN

/* 定义使用模拟I2C还是硬件I2C */
#define USE_SOFTWARE_I2C	0x00
#define USE_HARDWARE_I2C 	0x01
#define MAX30102_I2C_MODE	USE_HARDWARE_I2C

/* 设备地址 */
#define max30102_WR_address 0xAE
#define I2C_WRITE_ADDR 		0xAE
#define I2C_READ_ADDR 		0xAF

/* 设备ID */
#define DEV_PART_ID			0x15

/* I2C读写控制 */
#define I2C_WR				0x00
#define I2C_RD				0x01

/* 中断状态寄存器1 */
/*
 A_FULL：FIFO几乎满
 在SpO2和HR模式下，当FIFO写指针剩余一定数量的空闲空间时，此中断触发。
 触发号可以由FIFO_A_FULL[3:0]寄存器设置。通过读取中断状态1寄存器（0x00）来清除中断。
 */
#define FIFO_FULL_READY		(0x01 << 7)

/* 
 PPG_RDY：新的FIFO数据就绪
 在SpO2和HR模式下，当数据FIFO中有新样本时触发此中断。
 通过读取中断状态1寄存器（0x00）或读取FIFO_DATA寄存器来清除中断。
 */
#define PPG_READY			(0x01 << 6)

/*
 ALC_OVF：环境光消除溢出
 当SpO2/HR光电二极管的环境光消除功能达到最大极限时触发此中断，因此环境光影响ADC的输出。
 通过读取中断状态1寄存器（0x00）清除中断。
 */
#define ALC_OVF_READY		(0x01 << 5)

/*
 PROX_INT：触发接近阈值
 当接近阈值达到时触发接近中断，并且SpO2/HR模式已经开始。
 这让主处理器知道开始运行SpO2/HR算法并收集数据。通过读取中断状态1寄存器（0x00）清除中断。
 */
#define PROX_INT_READY		(0x01 << 4)

/*
 PWR_RDY：电源就绪标志
 在上电或断电条件后，当电源电压VDD从低于欠压锁定（UVLO）电压转换到高于UVLO电压时，
 触发电源就绪中断以发出模块已上电并准备好收集数据的信号。
 */
#define PWR_READY			(0x01 << 0)

/* 中断状态寄存器2 */
/*
 DIE_TEMP_RDY：内部温度就绪标志
 当内部芯片温度转换完成时，会触发此中断，以便处理器可以读取温度数据寄存器。
 通过读取中断状态2寄存器（0x01）或TFRAC寄存器（0x20）来清除中断。
 */
#define DIE_TEMP_READY		(0x01 << 1)

/* 中断使能寄存器1 */
/* 中断使能标记 */
#define FIFO_FULL_ENABLE	(0x01 << 7)
#define PPG_READY_ENABLE	(0x01 << 6)
#define ALC_OVF_ENABLE		(0x01 << 5)
#define PROX_INT_ENABLE		(0x01 << 4)

/* 中断使能寄存器2 */
#define DIE_TEMP_ENABLE		(0x01 << 1)

/* 定义是否支持中断引脚 */
#ifdef SUPPORT_INT_PIN
#define MAX30102_INT_PORT	GPIOB
#define MAX30102_INT_PIN	GPIO_PIN_6
#define MAX30102_INT 		HAL_GPIO_ReadPin(MAX30102_INT_PORT, MAX30102_INT_PIN)
#define MAX30102_INT_TRIG 	GPIO_PIN_SET
#endif

/* 超时时间 */
#define MAX_OPT_WAIT_TIMEOUT	0X7FFFFFFF

/* register addresses */
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
	MAX30102_RET_OK = 0x00, MAX30102_RET_BUSY = 0x01, MAX30102_RET_ERROR = 0x02
} MAX30102_RET_Status_t;

/* 传感器数据类型 */
typedef struct
{
	uint16_t m_partID; /* 设备ID */
	int32_t m_HeartRate; /* 心率 */
	int32_t m_SpO2; /* 血氧 */
	uint8_t m_fifo_data[6];
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

uint16_t max30102_readDeviceID(void);
uint8_t max30102_init(void);
void max30102_reset(void);
uint8_t max30102_Bus_Write(uint8_t Register_Address, uint8_t Word_Data);
uint8_t max30102_Bus_Read(uint8_t Register_Address);
void max30102_FIFO_ReadBytes(uint8_t Register_Address);
void maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data);
void maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data);
void maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led);

#endif /* PFV_MAX30102_H_ */

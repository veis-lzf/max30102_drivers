#ifndef __PFV_I2C_H__
#define __PFV_I2C_H__

#include "main.h"

/* I2C逻辑序号 */
typedef enum
{
	I2C_Seq1,
	I2C_Seq2,
	I2C_Seq3,
	I2C_Seq_NULL
} I2C_Seq_t;

/* I2C速率 */
typedef enum
{
	I2C_Speed_100K, /* 标准模式 */
	I2C_Speed_400K, /* 快速模式 */
	I2C_Speed_1M,   /* 快速模式增强 */
	I2C_Speed_3p4M,	/* 高速模式 */
	I2C_Speed_NULL
} I2C_Speed_t;

/* I2C主模式 */
typedef enum
{
	I2C_Master_Mode, /* 主机模式 */
	I2C_Slave_Mode,  /* 从机模式 */
	I2C_Mode_NULL
} I2C_Mode_t;

/* I2C状态码 */
typedef enum
{
	I2C_RET_OK,
	I2C_RET_BUSY,
	I2C_RET_ERROR,
} I2C_Status_t;


/* @bref 配置I2C
 * @param i2cx I2C逻辑序号
 * @param mode 主从模式
 * @param speed I2C速率
 * @retval 初始化成功返回I2C_RET_OK，失败返回I2C_RET_ERROR
 */
uint8_t pfv_I2C_Config(uint8_t i2cx, uint8_t mode, uint8_t speed);

/* @bref 写入一个字节
 * @param i2cx I2C逻辑序号
 * @param device_addr 设备地址
 * @param reg_addr 待写入的寄存器地址
 * @param reg_data 待写入的数据
 * @retval 写入成功返回I2C_RET_OK，失败返回I2C_RET_ERROR
 */
uint8_t pfc_I2C_WriteByte(uint8_t i2cx, uint8_t device_addr, uint16_t reg_addr, uint8_t reg_data);


/* @bref 读入一个字节
 * @param i2cx I2C逻辑序号
 * @param device_addr 设备地址
 * @param reg_addr 待读取的寄存器地址
 * @param p_data 数据缓冲区地址
 * @retval 读取成功返回I2C_RET_OK，失败返回I2C_RET_ERROR
 */
uint8_t pfv_I2C_ReadByte(uint8_t i2cx, uint8_t device_addr, uint16_t reg_addr, uint8_t *p_data);

#endif /* __PFV_I2C_H__ */

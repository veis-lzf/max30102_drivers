/*
 * pfv_max30102.c
 *
 *  Created on: Dec 10, 2023
 *      Author: veis
 */

#include "pfv_max30102.h"
#include "pfv_i2c.h"
#include "debug.h"
//#include <string.h>

MAX30102_Device_t g_MAX30102_Dev = 
{
	.m_address = max30102_WR_address,
	.m_i2cx = I2C_Seq1,
	.m_sensor_data = {0},
};

uint8_t max30102_Bus_Write(uint8_t Register_Address, uint8_t Word_Data)
{
#if (MAX30102_I2C_MODE == USE_SOFTWARE_I2C)
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(max30102_WR_address | I2C_WR); /* 此处是写指令 */

	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;  /* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址 */
	i2c_SendByte(Register_Address);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;  /* EEPROM器件无应答 */
	}

	/* 第5步：开始写入数据 */
	i2c_SendByte(Word_Data);

	/* 第6步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;  /* EEPROM器件无应答 */
	}

	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 1;	 /* 执行成功 */
 
cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
#elif (MAX30102_I2C_MODE == USE_HARDWARE_I2C)
	uint8_t status = MAX30102_RET_ERROR;

	if(I2C_RET_OK == pfc_I2C_WriteByte(g_MAX30102_Dev.m_i2cx, g_MAX30102_Dev.m_address | I2C_WR, Register_Address, Word_Data))
	{
		status = MAX30102_RET_OK;
	}
	return status;
#else
	#error "MAX30102_I2C_MODE Config Error!"
#endif
}

uint8_t max30102_Bus_Read(uint8_t Register_Address)
{
	uint8_t  data;
#if (MAX30102_I2C_MODE == USE_SOFTWARE_I2C)
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(max30102_WR_address | I2C_WR); /* 此处是写指令 */

	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;  /* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址， */
	i2c_SendByte((uint8_t)Register_Address);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;  /* EEPROM器件无应答 */
	}


	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	i2c_Start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(max30102_WR_address | I2C_RD); /* 此处是读指令 */

	/* 第8步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;  /* EEPROM器件无应答 */
	}

	/* 第9步：读取数据 */
	{
		data = i2c_ReadByte();  /* 读1个字节 */

	 	i2c_NAck(); /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return data;	 /* 执行成功 返回data值 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
#elif (MAX30102_I2C_MODE == USE_HARDWARE_I2C)
	if(I2C_RET_ERROR == pfv_I2C_ReadByte(g_MAX30102_Dev.m_i2cx, g_MAX30102_Dev.m_address | I2C_RD, Register_Address, &data))
		return 0;
	
	return data;
#else
	#error "MAX30102_I2C_MODE Config Error!"
#endif
}
 
 
/*
 * @brief 读取FIFO寄存器
 * @param Register_Address FIFO寄存器
 * */
void max30102_FIFO_ReadBytes(uint8_t Register_Address)
{
	uint32_t timeout = 0;
	
	while(!(PPG_READY & max30102_Bus_Read(REG_INTR_STATUS_1))) // 等待数据填充完成
	{
		if (timeout++ > MAX_OPT_WAIT_TIMEOUT)
			return;
	}
	max30102_Bus_Read(REG_INTR_STATUS_2);
	
#if (MAX30102_I2C_MODE == USE_SOFTWARE_I2C)
	/* 第1步：发起I2C总线启动信号 */
	IIC_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址， */
	IIC_Send_Byte((uint8_t)Register_Address);
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	

	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	IIC_Start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */

	/* 第8步：发送ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第9步：读取数据 */
	g_MAX30102_Dev.m_sensor_data.m_fifo_data[0] = IIC_Read_Byte(1);
	g_MAX30102_Dev.m_sensor_data.m_fifo_data[1] = IIC_Read_Byte(1);
	g_MAX30102_Dev.m_sensor_data.m_fifo_data[2] = IIC_Read_Byte(1);
	g_MAX30102_Dev.m_sensor_data.m_fifo_data[3] = IIC_Read_Byte(1);
	g_MAX30102_Dev.m_sensor_data.m_fifo_data[4] = IIC_Read_Byte(1);
	g_MAX30102_Dev.m_sensor_data.m_fifo_data[5] = IIC_Read_Byte(0);
	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
	/* 发送I2C总线停止信号 */
	IIC_Stop();

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	IIC_Stop();
#elif (MAX30102_I2C_MODE == USE_HARDWARE_I2C)
	if(I2C_RET_ERROR == pfv_I2C_ReadMultByte(g_MAX30102_Dev.m_i2cx, g_MAX30102_Dev.m_address | I2C_RD, Register_Address, &g_MAX30102_Dev.m_sensor_data.m_fifo_data[0], 6))
		return;
#else
	#error "MAX30102_I2C_MODE Config Error!"
#endif
//	memcpy(&g_MAX30102_Dev.m_sensor_data.m_fifo_data[0], Data, 6);
}

/*
 * @brief 复位
 * */
void max30102_reset(void)
{
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
}

/*
 * @brief 写寄存器
 * */
void maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data)
{
#if (MAX30102_I2C_MODE == USE_SOFTWARE_I2C)
	IIC_Write_One_Byte(I2C_WRITE_ADDR, uch_addr, uch_data);
#elif (MAX30102_I2C_MODE == USE_HARDWARE_I2C)
	if(I2C_RET_ERROR == pfc_I2C_WriteByte(g_MAX30102_Dev.m_i2cx, g_MAX30102_Dev.m_address | I2C_WR, uch_addr, uch_data))
		return;
#else
	#error "MAX30102_I2C_MODE Config Error!"
#endif
}

/*
 * @brief 读寄存器
 * */
void maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data)
{
#if (MAX30102_I2C_MODE == USE_SOFTWARE_I2C)
	IIC_Read_One_Byte(I2C_WRITE_ADDR, uch_addr, puch_data);
#elif (MAX30102_I2C_MODE == USE_HARDWARE_I2C)
	if(I2C_RET_ERROR == pfv_I2C_ReadByte(g_MAX30102_Dev.m_i2cx, g_MAX30102_Dev.m_address | I2C_RD, uch_addr, puch_data))
		return;
#else
	#error "MAX30102_I2C_MODE Config Error!"
#endif
}

/*
 * @brief 获取fifo的数据
 * */
void maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led)
{
	uint32_t un_temp;
	uint8_t uch_temp;
	*pun_red_led = 0;
	*pun_ir_led = 0;

	//read and clear status register
	maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
	maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);

	max30102_FIFO_ReadBytes(REG_FIFO_DATA);

	un_temp = (unsigned char) g_MAX30102_Dev.m_sensor_data.m_fifo_data[0];
	un_temp <<= 16;
	*pun_red_led += un_temp;
	un_temp = (unsigned char) g_MAX30102_Dev.m_sensor_data.m_fifo_data[1];
	un_temp <<= 8;
	*pun_red_led += un_temp;
	un_temp = (unsigned char) g_MAX30102_Dev.m_sensor_data.m_fifo_data[2];
	*pun_red_led += un_temp;

	un_temp = (unsigned char) g_MAX30102_Dev.m_sensor_data.m_fifo_data[3];
	un_temp <<= 16;
	*pun_ir_led+=un_temp;
	un_temp = (unsigned char) g_MAX30102_Dev.m_sensor_data.m_fifo_data[4];
	un_temp <<= 8;
	*pun_ir_led += un_temp;
	un_temp = (unsigned char) g_MAX30102_Dev.m_sensor_data.m_fifo_data[5];
	*pun_ir_led += un_temp;
	*pun_red_led &= 0x03FFFF;  // Mask MSB [23:18]
	*pun_ir_led &= 0x03FFFF;   // Mask MSB [23:18]
}

/*
 * @brief 获取设备ID
 * */
uint16_t max30102_readDeviceID(void)
{
	uint8_t data1 = 0, data2 = 0;
	maxim_max30102_read_reg(REG_REV_ID, &data1);		
	maxim_max30102_read_reg(REG_PART_ID, &data2);
	g_MAX30102_Dev.m_sensor_data.m_partID = (data1 << 8) | data2;
	return g_MAX30102_Dev.m_sensor_data.m_partID;
}

/*
 * @brief 初始化传感器
 * */
void max30102_init(void)
{
#if (MAX30102_I2C_MODE == USE_SOFTWARE_I2C)
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	IIC_Init();
#elif (MAX30102_I2C_MODE == USE_HARDWARE_I2C)
	pfv_I2C_Config(g_MAX30102_Dev.m_i2cx, I2C_Master_Mode, I2C_Speed_100K);
#endif

	if((max30102_readDeviceID() & 0xff) != DEV_PART_ID)
	{
		p_err("max30102 sensor no found!");
		return;
	}

	max30102_reset();
	max30102_Bus_Write(REG_INTR_ENABLE_1, 0xc0);	// INTR setting
	max30102_Bus_Write(REG_INTR_ENABLE_2, 0x00);
	max30102_Bus_Write(REG_FIFO_WR_PTR, 0x00);  	// FIFO_WR_PTR[4:0]
	max30102_Bus_Write(REG_OVF_COUNTER, 0x00);  	// OVF_COUNTER[4:0]
	max30102_Bus_Write(REG_FIFO_RD_PTR, 0x00);  	// FIFO_RD_PTR[4:0]
	max30102_Bus_Write(REG_FIFO_CONFIG, 0x0f);  	// sample avg = 1, fifo rollover=false, fifo almost full = 17
	max30102_Bus_Write(REG_MODE_CONFIG, 0x03);  	// 0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
	max30102_Bus_Write(REG_SPO2_CONFIG, 0x27);  	// SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)  
	max30102_Bus_Write(REG_LED1_PA, 0x24);   		// Choose value for ~ 7mA for LED1
	max30102_Bus_Write(REG_LED2_PA, 0x24);   		// Choose value for ~ 7mA for LED2
	max30102_Bus_Write(REG_PILOT_PA, 0x7f);   		// Choose value for ~ 25mA for Pilot LED
}


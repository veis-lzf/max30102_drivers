/*
 * pfv_max30102.c
 *
 *  Created on: Dec 10, 2023
 *      Author: veis
 */

#include "pfv_max30102.h"
#include "pfv_i2c.h"

 MAX30102_Device_t g_MAX30102_Dev = 
 {
 	.m_address = max30102_WR_address,
	.m_i2cx = I2C_Seq1,
 };

uint8_t max30102_Bus_Write(uint8_t Register_Address, uint16_t Word_Data)
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
 
 
void max30102_FIFO_Read(uint8_t Register_Address, uint16_t Word_Data[][2],uint8_t count)
{
	uint8_t i=0;
	uint8_t no = count;
	uint8_t data1, data2;
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
	while (no)
	{
		data1 = i2c_ReadByte();
		i2c_Ack();
		data2 = i2c_ReadByte();
		i2c_Ack();
		Word_Data[i][0] = (((uint16_t)data1 << 8) | data2);

		data1 = i2c_ReadByte();
		i2c_Ack();
		data2 = i2c_ReadByte();
		if(1==no)
			i2c_NAck(); /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		else
			i2c_Ack();
		Word_Data[i][1] = (((uint16_t)data1 << 8) | data2);

		no--;
		i++;
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();

	cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
#else
	while (no)
	{
		data1 = pfv_ ;
		i2c_Ack();
		data2 = i2c_ReadByte();
		i2c_Ack();
		Word_Data[i][0] = (((uint16_t)data1 << 8) | data2);

		data1 = i2c_ReadByte();
		i2c_Ack();
		data2 = i2c_ReadByte();
		if(1==no)
			i2c_NAck(); /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		else
			i2c_Ack();
		Word_Data[i][1] = (((uint16_t)data1 << 8) | data2);

		no--;
		i++;
	}
#endif
}

void max30102_init(void)
{
	max30102_Bus_Write(0x09, 0x0b); // mode configuration : temp_en[3] 	 MODE[2:0]=010 HR only enabled	  011 SP02 enabled
	max30102_Bus_Write(0x01, 0xF0); // open all of interrupt
	max30102_Bus_Write(INTERRUPT_REG, 0x00); // all interrupt clear
	max30102_Bus_Write(0x03, 0x02); // DIE_TEMP_RDY_EN
	max30102_Bus_Write(0x21, 0x01); // SET	 TEMP_EN

	max30102_Bus_Write(0x0a, 0x47); // SPO2_SR[4:2]=001  100 per second    LED_PW[1:0]=11  16BITS
	max30102_Bus_Write(0x0c, 0x47);
	max30102_Bus_Write(0x0d, 0x47);
}
 


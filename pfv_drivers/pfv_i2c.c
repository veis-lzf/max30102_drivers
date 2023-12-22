#include "pfv_i2c.h"
#include "i2c.h"

/**********************************************begin 数据域**********************************************/
typedef struct
{
	GPIO_TypeDef *GPIOx;
	uint32_t GPIO_Pinx;
	uint32_t Alternate;

} I2C_GPIO_t;

typedef struct
{
	I2C_HandleTypeDef *I2C_Handelx;
	I2C_GPIO_t m_sda;
	I2C_GPIO_t m_scl;
} I2C_Device_t;

I2C_Device_t I2C_Dev_Tree[] =
{
		{
				.I2C_Handelx = &hi2c1,
				.m_sda.GPIOx = GPIOB,
				.m_sda.GPIO_Pinx = GPIO_PIN_9,
				.m_sda.Alternate = GPIO_AF4_I2C1,
				.m_scl.GPIOx = GPIOB,
				.m_scl.GPIO_Pinx = GPIO_PIN_8,
				.m_scl.Alternate = GPIO_AF4_I2C1,
		},
/*	
 {
 .I2C_Handelx = &hi2c2,
 .m_sda.GPIOx = GPIOH,
 .m_sda.GPIO_Pinx = GPIO_PIN_5,
 .m_scl.GPIOx = GPIOB,
 .m_scl.GPIO_Pinx = GPIO_PIN_4,
 },

 {
 .I2C_Handelx = &hi2c3,
 .m_sda.GPIOx = GPIOH,
 .m_sda.GPIO_Pinx = GPIO_PIN_8,
 .m_scl.GPIOx = GPIOB,
 .m_scl.GPIO_Pinx = GPIO_PIN_7,
 },
 */
};

/**********************************************end 数据域**********************************************/

/**********************************************begin 接口域**********************************************/

/* @bref 配置I2C
 * @param i2cx I2C逻辑序号
 * @param mode 主从模式
 * @param speed I2C速率
 * @retval 初始化成功返回I2C_RET_OK，失败返回I2C_RET_ERROR
 */
uint8_t pfv_I2C_Config(uint8_t i2cx, uint8_t mode, uint8_t speed)
{
	assert_param(i2cx < I2C_Seq_NULL);
	assert_param(mode < I2C_Mode_NULL);
	assert_param(speed < I2C_Speed_NULL);

	if (i2cx == I2C_Seq1)
	{
		GPIO_InitTypeDef GPIO_Initure =
		{ 0 };
		// 使能相关时钟
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_I2C1_CLK_ENABLE();

		// 初始化GPIO
		GPIO_Initure.Pin = I2C_Dev_Tree[0].m_scl.GPIO_Pinx
				| I2C_Dev_Tree[0].m_sda.GPIO_Pinx;
		GPIO_Initure.Mode = GPIO_MODE_AF_OD;
		GPIO_Initure.Pull = GPIO_PULLUP;
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_Initure.Alternate = I2C_Dev_Tree[0].m_scl.Alternate;
		HAL_GPIO_Init(GPIOB, &GPIO_Initure);

		// 配置I2C参数
		I2C_Dev_Tree[0].I2C_Handelx->Instance = I2C1;
		if (speed == I2C_Speed_100K)
		{
			I2C_Dev_Tree[0].I2C_Handelx->Init.Timing = 0x20404768;
		}

		if (mode == I2C_Master_Mode)
		{
			I2C_Dev_Tree[0].I2C_Handelx->Init.OwnAddress1 = 0;
			I2C_Dev_Tree[0].I2C_Handelx->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
			I2C_Dev_Tree[0].I2C_Handelx->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
			I2C_Dev_Tree[0].I2C_Handelx->Init.OwnAddress2 = 0;
			I2C_Dev_Tree[0].I2C_Handelx->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
			I2C_Dev_Tree[0].I2C_Handelx->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
			I2C_Dev_Tree[0].I2C_Handelx->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
		}
		if (HAL_I2C_Init(&hi2c1) != HAL_OK)
		{
			return I2C_RET_ERROR;
		}
		/** Configure Analogue filter
		 */
		if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
				!= HAL_OK)
		{
			return I2C_RET_ERROR;
		}
		/** Configure Digital filter
		 */
		if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
		{
			return I2C_RET_ERROR;
		}

	}
	else if (i2cx == I2C_Seq2)
	{
	}
	else if (i2cx == I2C_Seq3)
	{
	}
	else
	{
		return I2C_RET_ERROR;
	}

	return I2C_RET_OK;
}

/* @bref 写入一个字节
 * @param i2cx I2C逻辑序号
 * @param device_addr 设备地址
 * @param reg_addr 待写入的寄存器地址
 * @param reg_data 待写入的数据
 * @retval 写入成功返回I2C_RET_OK，失败返回I2C_RET_ERROR
 */
uint8_t pfc_I2C_WriteByte(uint8_t i2cx, uint8_t device_addr, uint16_t reg_addr,
		uint8_t reg_data)
{
	assert_param(i2cx < I2C_Seq_NULL);

	if (i2cx == I2C_Seq1)
	{
		if (HAL_I2C_Mem_Write(I2C_Dev_Tree[0].I2C_Handelx, device_addr,
				reg_addr, I2C_MEMADD_SIZE_8BIT, &reg_data, 1, 1000) != HAL_OK)
		{
			return I2C_RET_ERROR;
		}
	}
	else if (i2cx == I2C_Seq2)
	{
	}
	else if (i2cx == I2C_Seq3)
	{
	}
	else
	{
		return I2C_RET_ERROR;
	}

	return I2C_RET_OK;
}

/* @bref 读取一个字节
 * @param i2cx I2C逻辑序号
 * @param device_addr 设备地址
 * @param reg_addr 待读取的寄存器地址
 * @param p_data 数据缓冲区地址
 * @retval 读取成功返回I2C_RET_OK，失败返回I2C_RET_ERROR
 */
uint8_t pfv_I2C_ReadByte(uint8_t i2cx, uint8_t device_addr, uint16_t reg_addr,
		uint8_t *p_data)
{
	assert_param(i2cx < I2C_Seq_NULL);
	assert_param(p_data != NULL);

	if (i2cx == I2C_Seq1)
	{
		if (HAL_I2C_Mem_Read(I2C_Dev_Tree[0].I2C_Handelx, (device_addr | 0x01),
				reg_addr, I2C_MEMADD_SIZE_8BIT, p_data, 1, 1000) != HAL_OK)
		{
			return I2C_RET_ERROR;
		}
	}
	else if (i2cx == I2C_Seq2)
	{
	}
	else if (i2cx == I2C_Seq3)
	{
	}
	else
	{
		return I2C_RET_ERROR;
	}

	return I2C_RET_OK;
}

/* @bref 读取多个字节
 * @param i2cx I2C逻辑序号
 * @param device_addr 设备地址
 * @param reg_addr 待读取的寄存器地址
 * @param p_data 数据缓冲区地址
 * @retval 读取成功返回I2C_RET_OK，失败返回I2C_RET_ERROR
 */
uint8_t pfv_I2C_ReadMultByte(uint8_t i2cx, uint8_t device_addr,
		uint16_t reg_addr, uint8_t *p_data, uint8_t nsize)
{
	assert_param(i2cx < I2C_Seq_NULL);
	assert_param(p_data != NULL);

	if (i2cx == I2C_Seq1)
	{
		if (HAL_I2C_Mem_Read(I2C_Dev_Tree[0].I2C_Handelx, (device_addr | 0x01),
				reg_addr, I2C_MEMADD_SIZE_8BIT, p_data, nsize, 1000) != HAL_OK)
		{
			return I2C_RET_ERROR;
		}
	}
	else if (i2cx == I2C_Seq2)
	{
	}
	else if (i2cx == I2C_Seq3)
	{
	}
	else
	{
		return I2C_RET_ERROR;
	}

	return I2C_RET_OK;
}

/**********************************************end 接口域**********************************************/


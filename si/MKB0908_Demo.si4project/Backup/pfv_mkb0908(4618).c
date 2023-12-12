#include "pfv_i2c.h"
#include "pfv_usart.h"
#include "pfv_mkb0908.h"

// 操作状态
static volatile uint8_t ret_status = MKB0908_Dev_RET_ERROR;

MKB0908_Device_t g_MKB0908_Device = 
{
#if (INTERFACE_MODE == USE_I2C_MODE)
	.m_address = 0xa0,
	.m_i2cx = I2C_Seq1,
#elif (INTERFACE_MODE == USE_UART_MODE)
	.m_uartx = USART_Seq2,
	.m_baud = 115200,
#endif
	.m_calibration_status = Calibration_OK,
	.m_erase_status = ERASE_EEPROM_OK,
	.m_dev_status = 0xff,
	.m_sensor_data = {0},
#ifdef SUPPORT_CRC8 /* 如果支持CRC8校验 */
	.m_crc8 = 0,
#endif

};

static uint8_t MKB0908_SendCommand(MKB0908_Device_t *obj, uint8_t cmd)
{
	uint8_t ret = 0;
	uint8_t init_data[6] = {cmd, 0, 0, 0, 0, 0};

	// 默认初始化为ERROR状态
	ret_status = MKB0908_Dev_RET_ERROR;
	
#if (INTERFACE_MODE == USE_UART_MODE)
	ret = pfc_USART_WriteBuffer(obj->m_uartx, init_data, sizeof(init_data) / sizeof(uint8_t));
	if(ret == USART_RET_ERROR)
	{
		return MKB0908_Dev_RET_ERROR;
	}
#elif (INTERFACE_MODE == USE_I2C_MODE)
	uint8_t i = 0;
	for(i = 0; i < 5; i++)
	{
		ret = pfc_I2C_WriteByte(obj->m_i2cx, obj->m_address, init_data[0], init_data[i+1]);
		if(ret == I2C_RET_ERROR)
		{
			return MKB0908_Dev_RET_ERROR;
		}
	}
#endif
	return MKB0908_Dev_RET_OK;
}

/* @bref 初始化传感器对象
 * @param obj 传感器对象
 * @retval 成功返回MKB0908_Dev_RET_OK，失败返回MKB0908_Dev_RET_ERROR
 */
uint8_t MKB0908_SensorConfig(MKB0908_Device_t *obj)
{
	assert_param(obj == NULL);
	uint8_t init_data[6] = {CMD_BLOOD_PRESSURE_CALIBRATION, 0, 0, 0, 0, 0};
	uint8_t ret = 0;
	
#if (INTERFACE_MODE == USE_UART_MODE)
	pfv_USART_Config(obj->m_uartx, obj->m_baud);
	ret = pfc_USART_WriteBuffer(obj->m_uartx, init_data, sizeof(init_data) / sizeof(uint8_t));
	if(ret == USART_RET_ERROR)
	{
		return MKB0908_Dev_RET_ERROR;
	}
	
#elif (INTERFACE_MODE == USE_I2C_MODE)
	uint8_t i = 0;

	pfv_I2C_Config(obj->m_i2cx, I2C_Master_Mode, I2C_Speed_100K);
	for(i = 0; i < 5; i++)
	{
		ret = pfc_I2C_WriteByte(obj->m_i2cx, obj->m_address, init_data[0], init_data[i+1]);
		if(ret == I2C_RET_ERROR)
		{
			return MKB0908_Dev_RET_ERROR;
		}
	}

#endif
	
	return MKB0908_Dev_RET_OK;
}

/* @bref 读取温度，收缩压，舒张压数据
 * @param obj 传感器对象
 * @retval 成功返回MKB0908_Dev_RET_OK，失败返回MKB0908_Dev_RET_ERROR
 */
uint8_t MKB0908_Read_NormalData(MKB0908_Device_t *obj)
{
	assert_param(obj == NULL);
	uint8_t ret;
	
	ret = MKB0908_SendCommand(obj, CMD_GET_HEARTRATE_AND_BLOODPRESSURE_AND_TEMPERATURE);

	return ret;
}

/* @bref 读取PPG数据
 * @param obj 传感器对象
 * @retval 成功返回MKB0908_Dev_RET_OK，失败返回MKB0908_Dev_RET_ERROR
 */
uint8_t MKB0908_Read_PPG_Data(MKB0908_Device_t *obj)
{
	assert_param(obj == NULL);
	uint8_t ret;
	
	ret = MKB0908_SendCommand(obj, CMD_READ_PULSE_SIGNAL);
	
	return ret;
}

/* @bref 读取ECG数据
 * @param obj 传感器对象
 * @retval 成功返回MKB0908_Dev_RET_OK，失败返回MKB0908_Dev_RET_ERROR
 */
uint8_t MKB0908_Read_ECG_Data(MKB0908_Device_t *obj)
{
	assert_param(obj == NULL);
	uint8_t ret;
	
	ret = MKB0908_SendCommand(obj, CMD_READ_ECG_SIGNAL);

	return ret;
}

#if (INTERFACE_MODE == USE_UART_MODE && MKB0908_DEV_MAIN_VERSION >= VERSION_V19)

/* @bref 读取版本
 * @param obj 传感器对象
 * @retval 成功返回MKB0908_Dev_RET_OK，失败返回MKB0908_Dev_RET_ERROR
 */
uint8_t MKB0908_ReadVersion(MKB0908_Device_t *obj)
{
	assert_param(obj == NULL);
	uint8_t ret;

	ret = MKB0908_SendCommand(obj, CMD_READ_VERSION);

	return ret;
}

/* @bref 读取HRV数据
 * @param obj 传感器对象
 * @retval 成功返回MKB0908_Dev_RET_OK，失败返回MKB0908_Dev_RET_ERROR
 */
uint8_t MKB0908_Read_HRV_Data(MKB0908_Device_t *obj)
{
	assert_param(obj == NULL);
	uint8_t ret;

	ret = MKB0908_SendCommand(obj, CMD_READ_HRV_DATA);

	return ret;
}
#endif

// 解析串口协议
void Receive_Handle(void)
{
	switch (rx_buffer[0]) // 分类协议头
	{
		// 校准应答0xfb
		case CMD_BLOOD_PRESSURE_CALIBRATION_ACK:
#if (INTERFACE_MODE == USE_UART_MODE && MKB0908_DEV_MAIN_VERSION >= VERSION_V19)
		// HRV数据获取0xf1
		case CMD_READ_HRV_DATA_ACK:
#endif
		// 擦除eeprom应答0xfa
		case CMD_ERASE_EEPROM_ACK:
		// 工作状态应答0xf8
		case CMD_READ_MODULE_WORKING_STATUS_ACK:
			g_MKB0908_Device.m_calibration_status = rx_buffer[3];
			ret_status = MKB0908_Dev_RET_OK;
			break;
		
		// 获取血压温度脉率数据0xfd
		case CMD_GET_HEARTRATE_AND_BLOODPRESSURE_AND_TEMPERATURE_ACK:
			g_MKB0908_Device.m_sensor_data.m_SystolicBloodPressure = rx_buffer[1];
			g_MKB0908_Device.m_sensor_data.m_Diastolic_BloodPressure = rx_buffer[2];
			g_MKB0908_Device.m_sensor_data.m_PulseRate = rx_buffer[3];
			g_MKB0908_Device.m_sensor_data.m_tmperature_byte[0] = rx_buffer[5];
			g_MKB0908_Device.m_sensor_data.m_tmperature_byte[1] = rx_buffer[4];
			ret_status = MKB0908_Dev_RET_OK;
			break;
		
		// 脉搏信号0xfc
		case CMD_READ_PULSE_SIGNAL_ACK:
		// ECG脉搏信号获取0xf9
		case CMD_READ_ECG_SIGNAL_ACK:
#if (INTERFACE_MODE == USE_UART_MODE && MKB0908_DEV_MAIN_VERSION >= VERSION_V19)
		// 版本获取0xf3
		case CMD_READ_VERSION_ACK:
#endif
			g_MKB0908_Device.m_sensor_data.m_version = (rx_buffer[2] << 8) | rx_buffer[3];
			ret_status = MKB0908_Dev_RET_OK;
			break;

		// 缺省
		default:
			ret_status = MKB0908_Dev_RET_ERROR;
			break;
	}
}

/*
 * 返回转换状态，用于判断是否完成读取
 * */
uint8_t MKB0908_GetStatus(void)
{
	return ret_status;
}

#ifdef SUPPORT_CRC8
/*
CRC8校验计算函数
*/
uint8_t CRC8_calc(uint8_t *Data, int length)
{
	uint8_t temp = 0;
	uint8_t i, j;

	for(i = 0; i < length; i++)
	{
		temp ^= Data[i];
		for (j = 0; j < 8; ++j)
		{
			if(temp & 0x01)
			{
				temp = ((temp >> 1)^0xaa);
			}
			else
			{
				temp >>= 1;
			}
		}
	}
	return temp;
}

#endif



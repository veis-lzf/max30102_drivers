#ifndef __PFV_MKB0908_H__
#define __PFV_MKB0908_H__

#define VERSION_V19	((1<<8) | 9)
#define MKB0908_DEV_MAIN_VERSION	VERSION_V19

// 传感器模块接口模式，只能二选一
#define	USE_I2C_MODE	0x01
#define USE_UART_MODE	0x02

#define INTERFACE_MODE	USE_UART_MODE
#if (INTERFACE_MODE != USE_I2C_MODE) && (INTERFACE_MODE != USE_UART_MODE)
	#error "Invalid Configuration for INTERFACE_MODE!"
#endif

// I2C设备地址和串口波特率
#if (INTERFACE_MODE == USE_I2C_MODE)
#define MKB0908_DEV_ADDRESS	0xA0
#elif (INTERFACE_MODE == USE_UART_MODE)
#define DEV_BAUD_RATE	115200U
#endif


/* 
血压校准
主机主动发起对模块进行血压校准，主机传输基准血压以及脉率等相关信息。
从机收到命令后对模块进行校准。在校准未完成前会通过命令回复主机。
命令:0xFE 1Byte
收缩压：0-240 1Byte
舒张压：0-240 1Byte
脉 率：0-240 1Byte
CRC 0：无 1Byte
CRC 1：无 1Byte
*/
#define CMD_BLOOD_PRESSURE_CALIBRATION	0xFE

/* 
从机：
命令：0xFE 1Byte
DATA1:0 1Byte
DATA2:0 1Byte
校准状态:2/1/0 1Byte
注：
1、校准状态1表示校准过程中，0表示校准成功，2表示校准失败。
2、串口校准从机主动回复命令头为 0xFE,i2c 从机不能主动回复，所以命令头改为 0xFB。
*/
#if (INTERFACE_MODE == USE_I2C_MODE)
	#define CMD_BLOOD_PRESSURE_CALIBRATION_ACK	0xFB
#elif (INTERFACE_MODE == USE_UART_MODE)
	#define CMD_BLOOD_PRESSURE_CALIBRATION_ACK	0xFE
#endif


/*
心率血压温度数据读取
主机对从发起实时数 据读取，回复相关参
命令:0xFD 1Byte
DATA1：预留 1Byte
DATA2：预留 1Byte
DATA3：预留 1Byte
CRC 0：无 1Byte
CRC 1：无 1Byte
*/
#define CMD_GET_HEARTRATE_AND_BLOODPRESSURE_AND_TEMPERATURE	0xFD

/*
从机：
命令：0xFD 1Byte
收缩压:0-250 1Byte
舒张压:0-250 1Byte
脉 率:0-250 1Byte
温度MSB：1byte
温度LSM：1byte
注： 
1、运动心率版本，软件自切换。
2、由于运动状态信号对血压影响太大，时会自停止更新。等待适合做计算的时候系统会继续更新血压
3、测量温度读数  =（MSB*256+LSB）/256 （MSB和LSB是 16进制数，要换算成10进制数）。
*/
#define CMD_GET_HEARTRATE_AND_BLOODPRESSURE_AND_TEMPERATURE_ACK	0xFD

/*主机读取模块采集到的脉搏波信号
主机：
命令:0xFC 1Byte
DATA1：预留 1Byte
DATA2：预留 1Byte
DATA3：预留 1Byte
CRC 0：无 1Byte
CRC 1：无 1Byte
*/
#define CMD_READ_PULSE_SIGNAL	0xFC

/*
从机：
命令：0xFC 1Byte
Data1:预留 1Byte
脉搏信号高8位:0-255 1Byte
脉搏信号底8位:0-255 1Byte
注：脉搏信号（PPG）读取频率小于 200Hz，即读取时间间隔大于 5ms,建议间隔时间 20ms
*/
#define CMD_READ_PULSE_SIGNAL_ACK	0xFC

/*
主机命令从机擦除校准信息，主机一次发生该命令从机擦除信息。第二次发送擦除的命令或者之前已经
执行过擦除命令并没有新的校准信息产生的时候，从机回复信息表示擦除成功。
主机：
命令:0xFA 1Byte
DATA1：预留 1Byte
DATA2：预留 1Byte
DATA3：预留 1Byte
CRC 0：无 1Byte
CRC 1：无 1Byte
*/
#define CMD_ERASE_EEPROM	0xFA

/*
从机：
命令：0xFA 1Byte
DATA1:预留 1Byte
DATA2:预留 1Byte
DATA3:1 1Byte
注：可以发送两次，0xFA 0x00 0x00 0x00 0x00 0x00 命令，0xFA 0x00 0x00 0x01 0x00 0x00
表示数据擦除成功，两次时间间隔10ms
*/
#define CMD_ERASE_EEPROM_ACK	0xFA

/*
主机通过命令读取模块采集到的ECG信号，从机回复16位ECG数据
主机：
命令:0xF9 1Byte
DATA1：预留 1Byte
DATA2：预留 1Byte
DATA3：预留 1Byte
CRC 0：无 1Byte
CRC 1：无 1Byte
*/
#define CMD_READ_ECG_SIGNAL	0xF9

/*
从机：
命令：0xF9 1Byte
DATA1:预留 1Byte
ECG信号高8位:0-255 1Byte
ECG信号底8位:0-255 1Byte
注：脉搏信号（PPG）读取频率小于 200Hz，即读取时间间隔大于 5ms,建议间隔时间 20ms
*/
#define CMD_READ_ECG_SIGNAL_ACK	0xF9

/*
主机通过命令读取模块工作状态，包含传感器脱落，ECG导致脱落，功率调节等信息
主机：
命令:0xF8 1Byte
DATA1：预留 1Byte
DATA2：预留 1Byte
DATA3：预留 1Byte
CRC 0：无 1Byte
CRC 1：无 1Byte
*/
#define CMD_READ_MODULE_WORKING_STATUS	0xF8

/*
从机：
命令：0xF8 1Byte
DATA1:预留 1Byte
DATA2:预留 1Byte
DATA3:0-255 1Byte
DATA3: 
BIT0 PPG传感器脱落状态(1/0)
BIT1 PPG传感器功率状态(1/0)
BIT2 信号异常状态(1/0)
BIT3 ECG导联状态1(1/0)
BIT4 ECG功率状态1(1/0)
*/
#define CMD_READ_MODULE_WORKING_STATUS_ACK	0xF8

/*以下通信协议，仅适用讯接口为串口（ URAT）版本且版本号为 V1.9 之后的程序*/
#if ((INTERFACE_MODE == USE_UART_MODE) && (MKB0908_DEV_MAIN_VERSION >= VERSION_V19))
/*
命令:0xF3 1Byte
DATA1：预留 1Byte
DATA2：预留 1Byte
DATA3：预留 1Byte
CRC 0：无 1Byte
CRC 1：无 1Byte
*/
#define CMD_READ_VERSION	0xF3

/*
从机：
命令：0xF3 1Byte
Data1:预留 1Byte
版本号高8位: 0xXX 1Byte
版本号低8位: 0xXX 1Byte
CRC 0：无 1Byte
CRC 1：无 1Byte
注：版本号=版本号高八位*255+版本号低八位,如果版本号=10 则表示当前版本为 1.0 版。
*/
#define CMD_READ_VERSION_ACK	0xF3

/*
主机对从发起实时数据读取，回复相关参主机：
命令:0xF1 1Byte
DATA1：预留 1Byte
DATA2：预留 1Byte
DATA3：预留 1Byte
CRC 0：无 1Byte
CRC 1：无 1Byte
注：建议仅1-2秒发送一次
*/
#define CMD_READ_HRV_DATA	0xF1

/*
从机：
命令：0xF1 1Byte
DATA1: 0x00 1Byte
DATA2: 0x00 1Byte
HRV:0-250 1Byte
注：
1、HRV读取时间必须是正常测量之后大于5min之后才能读取
2、HRV的正常范围是16±13
*/

#define CMD_READ_HRV_DATA_ACK	0xF1


#endif

/* 工作状态码及其掩码 */
#define PPG_SENSOR_STATUS_MASK	0x01
#define PPG_SENSOR_ERROR	0
#define PPG_SENSOR_FINE		1

#define PPG_POWER_STATUS_MASK	0x02
#define PPG_POWER_ERROR		0
#define PPG_POWER_FINE		1

#define SIGNAL_STATUS_MASK		0X04
#define SIGNAL_ERROR		0
#define SIGNAL_FINE			1

#define ECG_SENSOR_STATUS_MASK	0X08
#define ECG_SENSOR_ERROR	0
#define ECG_SENSOR_FINE		1

#define ECG_POWER_STATUS_MASK	0X10
#define ECG_POWER_ERROR		0
#define ECG_POWER_FINE		1

/* 操作状态码 */
typedef enum
{
	MKB0908_Dev_RET_OK,
	MKB0908_Dev_RET_ERROR,
} MKB0908_Dev_OptStatus_t;
	
/* 校准状态码 */
typedef enum
{
	Calibration_OK = 0x00,
	Calibration_BUSY = 0x01,
	Calibration_ERROR = 0x02
} Calibration_Status_t;

/* 擦除状态码 */
typedef enum
{
	ERASE_EEPROM_ERROR = 0x00,
	ERASE_EEPROM_OK = 0x01
} Erase_Status_t;	

typedef struct
{
	uint8_t m_SystolicBloodPressure; /* 收缩压 */
	uint8_t m_Diastolic_BloodPressure; /* 舒张压 */
	uint8_t m_PulseRate; /* 脉搏率 */
	union /* 温度数据 */
	{
		uint16_t m_temperature;
		uint8_t m_tmperature_byte[2]; /* 用于存储MSB和LSB */
	};
	union /* PPG脉搏数据 */
	{
		uint16_t m_ppg_data;
		uint8_t m_ppg_data_byte[2]; /* 用于存储MSB和LSB */
	};
	union /* ECG脉搏数据 */
	{
		uint16_t m_ecg_data;
		uint8_t m_ecg_data_byte[2]; /* 用于存储MSB和LSB */
	};

#if (INTERFACE_MODE == USE_UART_MODE && MKB0908_DEV_MAIN_VERSION >= VERSION_V19)
	uint8_t m_hrv_data; /* HRV数据 */
	uint16_t m_version; /* 版本号 */
#endif
} MKB0908_Data_t;


/* 传感器数据类型 */
#ifdef __GNUC__
	#pragma pack(1)
#endif
typedef struct
{
#if (INTERFACE_MODE == USE_I2C_MODE)
	uint8_t m_address; /* 设备地址 */
	uint8_t m_i2cx; /* I2C逻辑序号 */
#elif (INTERFACE_MODE == USE_UART_MODE)
	uint8_t m_uartx; /* 串口逻辑序号 */
	uint32_t m_baud; /* 波特率 */
#endif
	uint8_t m_calibration_status; /* 校准状态 */
	uint8_t m_erase_status; /* 擦除EEPROM状态 */
	uint8_t m_dev_status; /* 工作状态 */
	MKB0908_Data_t m_sensor_data;
#ifdef SUPPORT_CRC8 /* 如果支持CRC8校验 */
	uint8_t m_crc8;
#endif
} MKB0908_Device_t;

#ifdef __GNUC__
	#pragma pack()
#endif

extern MKB0908_Device_t g_MKB0908_Device;

/* @bref 初始化传感器对象
 * @param obj 传感器对象
 * @retval 成功返回MKB0908_Dev_RET_OK，失败返回MKB0908_Dev_RET_ERROR
 */
uint8_t MKB0908_SensorConfig(MKB0908_Device_t *obj);

/* @bref 读取温度，收缩压，舒张压数据
 * @param obj 传感器对象
 * @retval 成功返回MKB0908_Dev_RET_OK，失败返回MKB0908_Dev_RET_ERROR
 */
uint8_t MKB0908_Read_NormalData(MKB0908_Device_t *obj);

/* @bref 读取PPG数据
 * @param obj 传感器对象
 * @retval 成功返回MKB0908_Dev_RET_OK，失败返回MKB0908_Dev_RET_ERROR
 */
uint8_t MKB0908_Read_PPG_Data(MKB0908_Device_t *obj);

/* @bref 读取ECG数据
 * @param obj 传感器对象
 * @retval 成功返回MKB0908_Dev_RET_OK，失败返回MKB0908_Dev_RET_ERROR
 */
uint8_t MKB0908_Read_ECG_Data(MKB0908_Device_t *obj);

#if (INTERFACE_MODE == USE_UART_MODE && MKB0908_DEV_MAIN_VERSION >= VERSION_V19)

/* @bref 读取版本
 * @param obj 传感器对象
 * @retval 成功返回MKB0908_Dev_RET_OK，失败返回MKB0908_Dev_RET_ERROR
 */
uint8_t MKB0908_ReadVersion(MKB0908_Device_t *obj);

/* @bref 读取HRV数据
 * @param obj 传感器对象
 * @retval 成功返回MKB0908_Dev_RET_OK，失败返回MKB0908_Dev_RET_ERROR
 */
uint8_t MKB0908_Read_HRV_Data(MKB0908_Device_t *obj);
#endif
/*
 * 返回转换状态，用于判断是否完成读取
 * */
uint8_t MKB0908_GetStatus(void);

#ifdef SUPPORT_CRC8
uint8_t CRC8_calc(uint8_t *Data, int length);
#endif


#endif /* __PFV_MKB0908_H__ */


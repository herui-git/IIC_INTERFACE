/**
 ****************************************************************
 * @file  profile_iic.h
 * @brief a common iic interface
 **************************************************************** 
 */
#ifndef __PROFILE_IIC_H__
#define __PROFILE_IIC_H__


#include "stdint.h"
/**
 * @addtogroup USER_Drivers
 * @{
 */
 
/**
 * @addtogroup IIC_INTERFACE
 * @{
 */
 
/**
 * @enum IIC_SDA_DIR
 * IIC SDA pin input or output
 */ 
enum IIC_SDA_DIR
{
	IIC_SDA_INPUT,
	IIC_SDA_OUTPUT,
	ERR,
};

/**
 * @brief IIC interface
 */
typedef struct
{
	void (*iic_clk_pin)(uint8_t);          /**< control clock pin */
	void (*iic_sda_pin)(uint8_t);          /**< control sda pin */
	void (*iic_sda_pin_dir)(uint8_t);      /**< configure sda direction */
	uint8_t (*iic_sda_pin_read)();         /**< read sda pin */
	void (*delay)();                       
}iic_info_t;

/**
 * @addtogroup IIC_FUNCTIONS_GROUP1
 * @{
 */

void iic_write_byte(iic_info_t *iic_info, uint8_t dat);
uint8_t uc_iic_read_byte(iic_info_t *iic_info);

int i_iic_write_data_mem(iic_info_t *iic_info, uint8_t dev, uint8_t addr, uint8_t dat);
int i_iic_read_data_mem(iic_info_t *iic_info, uint8_t dev, uint8_t addr, uint8_t *pdat, uint32_t len);
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#endif



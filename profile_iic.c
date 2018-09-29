/**
 ****************************************************************
 * @file  profile_iic.c
 * @brief a common iic interface
 **************************************************************** 
 */
#include "profile_iic.h"

/**
 * @addtogroup USER_DRIVER
 * @{
 */
 
/**
 * @defgroup IIC_INTERFACE
 * @brief Common IIC Interface
 * @{
 */

/**
 * @defgroup IIC_FUNCTIONS_GROUP1 
 * @brief Common IIC Interface
 * @{
 */

/**
 * @brief iic start
 */
static void iic_start(iic_info_t *iic_info)
{
	iic_info->iic_sda_pin_dir(IIC_SDA_OUTPUT);
	iic_info->iic_sda_pin(1);
	iic_info->iic_clk_pin(1);
	iic_info->delay();
	iic_info->iic_sda_pin(0);
	iic_info->delay();
	iic_info->iic_clk_pin(0);
}

/**
 * @brief iic end
 */
static void iic_end(iic_info_t *iic_info)
{
	iic_info->iic_sda_pin_dir(IIC_SDA_OUTPUT);
	iic_info->iic_clk_pin(0);
	iic_info->iic_sda_pin(0);
	iic_info->delay();
    iic_info->iic_clk_pin(1);
	iic_info->iic_sda_pin(1);
	iic_info->delay();
}


/**
 * @brief iic ack
 */
static int i_iic_ack(iic_info_t *iic_info)
{
	uint8_t tmp = 0xff;
	iic_info->iic_sda_pin_dir(IIC_SDA_INPUT);
	iic_info->iic_clk_pin(1);
	iic_info->delay();
	do
	{
		tmp--;
	}while((iic_info->iic_sda_pin_read() != 0x0) && (tmp > 0));

	if(tmp == 0)
	{
		iic_end(iic_info);
		return -1;
	}

	iic_info->iic_clk_pin(0);
	return 0;	
}

static void i_iic_mack(iic_info_t *iic_info)
{
    //sda=0
	iic_info->iic_sda_pin_dir(IIC_SDA_OUTPUT);
    iic_info->iic_sda_pin(0);
    //clk=1
    iic_info->iic_clk_pin(1);
    //delay
    iic_info->delay();
    //clk=0
    iic_info->iic_clk_pin(0);
    //delay
    iic_info->delay();

}


/**
 * @brief iic writing
 */
void iic_write_byte(iic_info_t *iic_info, uint8_t dat)
{
	uint8_t i = 0;
	iic_info->iic_sda_pin_dir(IIC_SDA_OUTPUT);
	for(i = 0;i < 8;i++)
	{
		iic_info->iic_clk_pin(0);
		iic_info->delay();
		if(dat & 0x80)
		{
			iic_info->iic_sda_pin(1);
		}
		else
		{
			iic_info->iic_sda_pin(0);
		}
		iic_info->iic_clk_pin(1);
		iic_info->delay();
		iic_info->iic_clk_pin(0);
		dat <<= 1;
	}
}



/**
 * @brief iic reading
 */
uint8_t uc_iic_read_byte(iic_info_t *iic_info)
{
	uint8_t i = 0, tmp = 0;
	iic_info->iic_sda_pin_dir(IIC_SDA_INPUT);
	for(i = 0;i < 8;i++)
	{
		tmp <<= 1;
		iic_info->iic_clk_pin(0);
		iic_info->delay();
		iic_info->iic_clk_pin(1);
		if(0 != iic_info->iic_sda_pin_read())
		{
			tmp++;
		}
	}
	iic_info->iic_clk_pin(0);
	iic_info->delay();
	return tmp;	
}

/**
 * @brief iic writing memory
 */
int i_iic_write_data_mem(iic_info_t *iic_info, uint8_t dev, uint8_t addr, uint8_t dat)
{
	iic_start(iic_info);
	iic_write_byte(iic_info, dev);
	if(i_iic_ack(iic_info) < 0)
	{
        iic_end(iic_info);
		return -1;
	}
	iic_write_byte(iic_info, addr);
	if(i_iic_ack(iic_info) < 0)
	{
        iic_end(iic_info);
		return -1;
	}
	iic_write_byte(iic_info, dat);
	if(i_iic_ack(iic_info) < 0)
	{
        iic_end(iic_info);
		return -1;
	}   
	iic_end(iic_info);
	return 0;
}


/**
 * @brief iic reading  memory
 */
int i_iic_read_data_mem(iic_info_t *iic_info, uint8_t dev, uint8_t addr, uint8_t *pdat)
{
	iic_start(iic_info);
	iic_write_byte(iic_info, dev);
	if(i_iic_ack(iic_info) < 0)
	{
        iic_end(iic_info);
		return -1;
	}
	iic_write_byte(iic_info, addr);
	if(i_iic_ack(iic_info) < 0)
	{
        iic_end(iic_info);
		return -1;
	}
	iic_start(iic_info);
	iic_write_byte(iic_info, dev | 0x1);
	if(i_iic_ack(iic_info) < 0)
	{
        iic_end(iic_info);
		return -1;
	}	
	*pdat = uc_iic_read_byte(iic_info);

	iic_end(iic_info);
	return 0;
}

/**
 * @brief iic reading  buf
 */
int i_iic_read_data_buf(iic_info_t *iic_info, uint8_t dev, uint8_t addr, uint8_t *pdat, uint8_t len)
{
	iic_start(iic_info);
	iic_write_byte(iic_info, dev);
	if(i_iic_ack(iic_info) < 0)
	{
        iic_end(iic_info);
		return -1;
	}
	iic_write_byte(iic_info, addr);
	if(i_iic_ack(iic_info) < 0)
	{
        iic_end(iic_info);
		return -1;
	}
	iic_start(iic_info);
	iic_write_byte(iic_info, dev | 0x1);
	if(i_iic_ack(iic_info) < 0)
	{
        iic_end(iic_info);
		return -1;
	}
    while(len-- > 1)
    {
        *pdat++ = uc_iic_read_byte(iic_info);
        i_iic_mack(iic_info);
    }
    *pdat++ = uc_iic_read_byte(iic_info);

	iic_end(iic_info);
	return 0;
}

/**
 * @brief iic reading  buf
 */
int i_iic_write_data_buf(iic_info_t *iic_info, uint8_t dev, uint8_t addr, uint8_t *pdat, uint8_t len)
{
    uint32_t i = 0;
	iic_start(iic_info);
	iic_write_byte(iic_info, dev);
	if(i_iic_ack(iic_info) < 0)
	{
        iic_end(iic_info);
		return -1;
	}
	iic_write_byte(iic_info, addr);
	if(i_iic_ack(iic_info) < 0)
	{
        iic_end(iic_info);
		return -1;
	}
    for(i = 0;i < len;i++)
    {
        iic_write_byte(iic_info, pdat[i]);
        if(i_iic_ack(iic_info) < 0)
        {
            iic_end(iic_info);
            return -1;
        }
    }
	iic_end(iic_info);
	return 0;
}
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */




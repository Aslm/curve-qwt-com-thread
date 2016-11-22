
/*******************************************************************************
* @FunctionName   :
* @Version        : 1.00
* @Description    : 
* @Input          : None
* @Output         : None
* @Return         : None
* @Attention      : None
* @Design         : Aslm @ME:sillyman2008@outlook.com
*******************************************************************************/
uint16_t int16_Turn_uint16(int16_t data)
{
	static uint16_t temp = 0;
	
	temp = 0;
	
	if(data < 0)
		temp = (abs(data) &  0x7FFF) + 0x8000;
	else
	  temp = (data      &  0x7FFF) + 0x0000;
	
	return temp;
}

int16_t uint8x2_Turn_int16(uint8_t data_h,uint8_t data_l)
{
	static int16_t temp = 0;
	
	temp = 0;
	
	if(data_h & 0x80)
	{
		temp = - (uint16_t)(((uint16_t)(data_h & 0x7F) << 8) + data_l);
	}
	else
		temp = + (uint16_t)(((uint16_t)(data_h & 0x7F) << 8) + data_l);
	
	return temp;
}

void My_AppTask_One(void *p_arg)
{
	while(1)
	{
		OS_ERR   err;
		CPU_ERR  cpu_err;
		
		(void)p_arg;

    OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_NON_STRICT,&err);
    if(err != OS_ERR_NONE) DEBUG_Printf_Err_Loction(err);
 
#define M_PI		3.14159265358979323846
		
		uint8_t data[64] = {0};
		static double x;
		uint8_t axis = 0;
		uint8_t clc_flag = 0;
		
		data[axis++] = '{';
		data[axis++] = 'C';
		
		uint16_t temp_1 =  int16_Turn_uint16((int16_t)(sin(x)*1000));
		clc_flag += data[axis++] = (uint8_t)((temp_1 & 0xFF00) >> 8);	
		clc_flag += data[axis++] = (uint8_t)((temp_1 & 0x00FF) >> 0);
		
		uint16_t temp_2 =  int16_Turn_uint16((int16_t)(cos(x)*1000));
		clc_flag += data[axis++] = (uint8_t)((temp_2 & 0xFF00) >> 8);
		clc_flag += data[axis++] = (uint8_t)((temp_2 & 0x00FF) >> 0);
		
	    uint16_t temp_3 =  int16_Turn_uint16((int16_t)(tan(x)*1000));
		clc_flag += data[axis++] = (uint8_t)((temp_3 & 0xFF00) >> 8);
		clc_flag += data[axis++] = (uint8_t)((temp_3 & 0x00FF) >> 0);
		
		uint16_t temp_4 =  int16_Turn_uint16((int16_t)( - sin(x)*1000));
		clc_flag += data[axis++] = (uint8_t)((temp_4 & 0xFF00) >> 8);
		clc_flag += data[axis++] = (uint8_t)((temp_4 & 0x00FF) >> 0);
		
		uint16_t temp_5 =  int16_Turn_uint16((int16_t)( - cos(x)*1000));
		clc_flag += data[axis++] = (uint8_t)((temp_5 & 0xFF00) >> 8);
		clc_flag += data[axis++] = (uint8_t)((temp_5 & 0x00FF) >> 0);
		
		uint16_t temp_6 =  int16_Turn_uint16((int16_t)( - tan(x)*1000));
		clc_flag += data[axis++] = (uint8_t)((temp_6 & 0xFF00) >> 8);
		clc_flag += data[axis++] = (uint8_t)((temp_6 & 0x00FF) >> 0);
		
		data[axis++] = ~clc_flag;
		
		data[axis++] = 'E';
		data[axis++] = '}';
		
		x+=(M_PI / 10.0);
		
		//printf(" %x %x %x \r\n",temp_1,temp_2,temp_3);
		HAL_UART_Transmit(&huart2,data,axis,10);
 
    (void)err;
    (void)cpu_err;
	}
}




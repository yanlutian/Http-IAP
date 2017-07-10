

/***********************************************************************
****************IEEE754标准下浮点数和十六进制转换函数*******************
***********************************************************************/


#include "float_hex.h"




/*********************FLOAT*************************/
float hex_float(unsigned int hex_data)
{
	unsigned int s, e, m, bit_temp;//???????????
	unsigned char i;
	float m_int=0.0;
	
	m = hex_data & 0x007fffff;//?????
	hex_data >>= 23;
	e = hex_data & 0xff;//?????	
	hex_data >>= 8;
	s = hex_data & 0x01;//?????
	
	for (  i = 1,m_int = 0.0; i <= 23; i++)
	{
		bit_temp = 0x400000 & m;
		m_int +=(bit_temp>>22)*powf(2 ,-i);
		m <<= 1;
	}	
	m_int = powf(-1, s) * (1 + m_int)*powf(2, e - 127);

	return m_int;
}

/***************************************************/
unsigned int xiaoshu_hex( float data)
{
	unsigned char i;
	unsigned int m = 0, bit_temp = 0x400000;//???????????
	float  temp = data;

	//?????
	for (i = 1; i <= 23; i++)
	{
		temp = data * 2;
		if ((temp) >= 1.0)
		{
			data = temp - (unsigned int)temp;
			m |= bit_temp;
			if (data == 0.0)
				break;
		}
		else
		{
			data = temp;
		}
		bit_temp >>= 1;
	}
	return m;
}

/***************************************************/
unsigned int float_hex(float data)
{
	unsigned int s=0, e=0, m=0, bit_temp=0x400000;//???????????
	unsigned char i;
	unsigned int hex_temp = 0.0;

	//
	if (data>0.0)
	{
		s = 0;
	}
	else
	{
		s = 0x80000000;
	}

	//
	while (data>=2.0)
	{
		e++;
		data /= 2.0;
	}

	//
	hex_temp= xiaoshu_hex(data - (unsigned int)data);

	hex_temp = s | ((e + 127) << 23) | hex_temp;

	return hex_temp;
}


/*********************DOUBLE*************************/
double hex_double(unsigned long long hex_data)
{
	unsigned long long s, e, m, bit_temp;//???????????
	unsigned char i;
	double m_int = 0.0;

	m = hex_data & 0x000fffffffffffff;//?????
	hex_data >>= 52;
	e = hex_data & 0x7ff;//?????	
	hex_data >>= 8;
	s = hex_data & 0x01;//?????

	for (i = 1, m_int = 0.0; i <= 52; i++)
	{
		bit_temp = 0x8000000000000 & m;
		m_int += (bit_temp >> 51)*powl(2, -i);
		m <<= 1;
	}
	m_int = powl(-1, s) * (1 + m_int)*powl(2, e - 1023);

	return m_int;
}

/***************************************************/
unsigned long long xiaoshu_double_hex(double data)
{
	unsigned char i;
	unsigned long long m = 0, bit_temp = 0x8000000000000;//???????????
	double  temp = data;

	//?????
	for (i = 1; i <= 52; i++)
	{
		temp = data * 2;
		if ((temp) >= 1.0)
		{
			data = temp - (unsigned long)temp;
			m |= bit_temp;
			/*printf("temp=%llx \n", bit_temp);*/
			if (data == 0.0)
				break;
		}
		else
		{
			data = temp;
		}
		bit_temp >>= 1;
	}
	return m;
}

/***************************************************/
unsigned long long double_hex(double data)
{
	unsigned long long s = 0, e = 0, m = 0, bit_temp = 0x8000000000000;//???????????
	unsigned char i;
	unsigned long long hex_temp = 0.0;


	if ((data-1.0)>0.0)
	{
		s = 0;
	}
	else
	{
		s = 0x8000000000000000;
	}

	//?????
	while ((data > 2.0) || ((data-2.0)<0.0000001))
	{
		e++;
		data /= 2.0;
	}

	//?????
	hex_temp = xiaoshu_double_hex(data - (unsigned long long)data);

	hex_temp = s | ((e + 1023) << 52) | hex_temp;

	return hex_temp;
}












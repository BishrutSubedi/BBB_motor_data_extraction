
#ifndef EXAMPLE_I2C_H_
#define EXAMPLE_I2C_H_


#pragma GCC diagnostic ignored "-Wunused-variable"



#include "BlackI2C.h"
#include <string>
#include <iostream>
#include <math.h>
#include <time.h>
const double PI  =3.141592653589793238463;
BlackLib::BlackI2C *R_COM;

class readcompass {
		public:

//			float offsetx=-128;
//			float offsety=250;
				float offsetx=-110;
				float offsety=34;
//				float offsetx=-224;
//				float offsety=383;
			void setting_compass();
			float c_heading(void);

		private:
						uint8_t HMC5883L_ADDRESS              = 0x1e;
						uint8_t HMC5883L_REG_CONFIG_A         = 0x00;
						uint8_t HMC5883L_REG_CONFIG_B         = 0x01;
						uint8_t HMC5883L_REG_MODE             = 0x02;
						uint8_t HMC5883L_REG_OUT_X_M          = 0x03;
						uint8_t HMC5883L_REG_OUT_X_L          = 0x04;
						uint8_t HMC5883L_REG_OUT_Z_M          = 0x05;
						uint8_t HMC5883L_REG_OUT_Z_L          = 0x06;
						uint8_t HMC5883L_REG_OUT_Y_M          = 0x07;
						uint8_t HMC5883L_REG_OUT_Y_L          = 0x08;
						uint8_t HMC5883L_REG_STATUS           = 0x09;
						uint8_t HMC5883L_REG_IDENT_A          = 0x0A;
						uint8_t HMC5883L_REG_IDENT_B          = 0x0B;
						uint8_t HMC5883L_REG_IDENT_C          = 0x0C;
						uint8_t outrate  = 0b00010100;
						uint8_t rangga   = 0b00100001;
						uint8_t outmodel = 0b00000000;
						float mgperdigita=0.92;
						int16_t tempReadx,tempReady,tempReadz;
						int16_t value_x, value_y,value_z;

		};

void readcompass::setting_compass(void)
	{


		R_COM= new BlackLib::BlackI2C (BlackLib::I2C_1, 0x1e);
	    bool isOpened = R_COM->open( BlackLib::ReadWrite | BlackLib::NonBlock );

	    if( !isOpened )
	    {
	        std::cout << "I2C DEVICE CAN\'T OPEN.;" << std::endl;
	        exit(1);

	    }
	    R_COM->writeByte(HMC5883L_REG_CONFIG_A, outrate);//0b00010000 OUT1 15Hz norm
	   	R_COM->writeByte(HMC5883L_REG_CONFIG_B, rangga);//0b00100001 1.3Ga
	   	R_COM->writeByte(HMC5883L_REG_MODE    , outmodel);//0b00000000continuous-measurement mode
	}

float readcompass::c_heading(void)
			{
			 tempReadx = (R_COM->readWord(HMC5883L_REG_OUT_X_M));
	    	 tempReady = (R_COM->readWord(HMC5883L_REG_OUT_Y_M));
	    	 tempReadz =  R_COM->readWord(HMC5883L_REG_OUT_Z_M);

	    	 uint8_t vhax=R_COM->readByte(HMC5883L_REG_OUT_X_M);
	    	 uint8_t vlax=R_COM->readByte(HMC5883L_REG_OUT_X_L);
	    	 value_x = vhax <<8 | vlax;
	    	 uint8_t vhay=R_COM->readByte(HMC5883L_REG_OUT_Y_M);
	    	 uint8_t vlay=R_COM->readByte(HMC5883L_REG_OUT_Y_L);
	    	 value_y = vhay <<8 | vlay;
	    	 uint8_t vhaz=R_COM->readByte(HMC5883L_REG_OUT_Z_M);
	    	 uint8_t vlaz=R_COM->readByte(HMC5883L_REG_OUT_Z_L);
	    	 value_z = vhaz <<8 | vlaz;

	    	 //read normalize;
	    	 float value_X =(value_x-offsetx)*mgperdigita;
	    	 float value_Y =(value_y-offsety)*mgperdigita;
	    	 float value_Z =value_z*mgperdigita;

	    	 //std::cout << "tempvaluex "  << value_x <<", "<< value_y<<", "<<value_z<<std::endl;
	    	 //std::cout << "tempvaluex "  << value_X <<", "<< value_Y<<", "<<value_Z<<std::endl;

	    	 //heading direction
	    	 float heading = atan2(value_Y, value_X);
	    	 float declinationAngle =(3 + (32.0 /60)) /(180/PI);
	    	 heading +=declinationAngle;
	    	 if (heading < 0)
	    	   {
	    	     heading += 2 * PI;
	    	   }

	    	   if (heading > 2 * PI)
	    	   {
	    	     heading -= 2 * PI;
	    	   }
	    	float headingDegrees = heading * 180/M_PI;
	    	return headingDegrees;

			}



#endif /* EXAMPLE_I2C_H_ */

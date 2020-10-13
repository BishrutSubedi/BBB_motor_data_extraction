 /*

 ####################################################################################
 #  BlackLib Library controls Beaglebone Black's inputs and outputs.                #
 #  Copyright (C) 2013-2015 by Yigit YUCE                                           #
 ####################################################################################
 #                                                                                  #
 #  This file is part of BlackLib library.                                          #
 #                                                                                  #
 #  BlackLib library is free software: you can redistribute it and/or modify        #
 #  it under the terms of the GNU Lesser General Public License as published by     #
 #  the Free Software Foundation, either version 3 of the License, or               #
 #  (at your option) any later version.                                             #
 #                                                                                  #
 #  BlackLib library is distributed in the hope that it will be useful,             #
 #  but WITHOUT ANY WARRANTY; without even the implied warranty of                  #
 #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                   #
 #  GNU Lesser General Public License for more details.                             #
 #                                                                                  #
 #  You should have received a copy of the GNU Lesser General Public License        #
 #  along with this program.  If not, see <http://www.gnu.org/licenses/>.           #
 #                                                                                  #
 #  For any comment or suggestion please contact the creator of BlackLib Library    #
 #  at ygtyce@gmail.com                                                             #
 #                                                                                  #
 ####################################################################################

 */





#ifndef EXAMPLE_GPIO_H_
#define EXAMPLE_GPIO_H_




#include "BlackGPIO.h"
#include <string>
#include <iostream>

using namespace std;
BlackLib::BlackGPIO *GPIO45, *GPIO44;


void setting_GPIO()
{

    GPIO45=new BlackLib::BlackGPIO  (BlackLib::GPIO_45,BlackLib::output, BlackLib::FastMode);
    GPIO44=new BlackLib::BlackGPIO  (BlackLib::GPIO_44,BlackLib::output, BlackLib::FastMode);

}
// clockwise
void motor_rotate_direction(int i)
{
	switch(i)
	{
	case 1:
		GPIO45->setValue(BlackLib::high);          // anticlockwise
		GPIO44->setValue(BlackLib::low);
		break;

	case 2:
		GPIO45->setValue(BlackLib::low);          // clockwise
		GPIO44->setValue(BlackLib::high);
		break;
	case 3:
		GPIO45->setValue(BlackLib::high);          //stop
		GPIO44->setValue(BlackLib::high);
		break;

	}

}




#endif /* EXAMPLE_GPIO_H_ */

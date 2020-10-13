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




#ifndef EXAMPLE_PWM_H_
#define EXAMPLE_PWM_H_




#include "BlackPWM.h"
#include <string>
#include <iostream>


BlackLib::BlackPWM *MPWM;
using namespace std;






void setting_PWM()
{

    MPWM=new BlackLib::BlackPWM  (BlackLib::EHRPWM2B);
    // if new period value is less than the current duty value, the new period value setting
    // operation couldn't execute. So firstly duty value is set to zero for safe steps.
    MPWM->setDutyPercent(0.0);
//    MPWM->setPeriodTime(1000000);
    MPWM->setPeriodTime(1000000);
    sleep(1);

//    sleep(10);
//    MPWM->setDutyPercent(5);
}


void Motor_speed(float v)
{
	MPWM->setDutyPercent(v);
}



#endif /* EXAMPLE_PWM_H_ */

#include "R_COM.h"
#include "M_GPIO.h"
#include "M_PWM.h"
#include <string>
#include <iostream>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <sys/time.h>
#include <array>
#include <stdlib.h>

#include <fstream>
#include <cstring>

using namespace std;
using namespace std::chrono;

// THIS CODE HAS NOTHING TO DO WITH PARAMETER ESTIMATION
// THIS IS FOR DATA EXTRACTION ONLY USING PI CONTROLLER

double Setpoint=0, Input, Output;
// Set parameters for the LQG cntroller
double lastW = 0;
double lastE = 0;
//double Kcp = 4.4721;
//double Kcf = 3.1535;

double kp=1.1;  // THIS VALUE IS HERE TO ROTATE THE MOTOR ONLY, NOT FROM ESTIMATION
double ki=1.5;  // FOR ROTATION, KI GAIN, NOTHING TO DO WITH ESTIMATION

const int sampleTime = 60;
double SampleTimeInSec = ((double)sampleTime)/1000;
steady_clock::time_point t2 = steady_clock::now();
steady_clock::time_point t1 = steady_clock::now();
// initial heading*******************************
double offsetTol=1.5;

// THESE ARE TARGET VALUE OF AZIMUTH TO GET THE ANTENNA TO USING DC MOTOR ROTATION
// THEY ARE USED LATER AS HARDCORED FOR ESTIMATION USING MATLAB

//int Azmuth[] ={ 165,80,155,80,160,95,190,120,200,145,225,165,255,185,255,175,235,140,215,135,195,100,175,85,145,60,140};//exp1
//int Azmuth[] ={ 75,150,90,170,220,165,245,290,200,260,170,100,165,80,155,65,135,45,165,100,195,130,215,145,235,140,220};//exp2
//int Azmuth[] ={150,90,150,220,165,245,290,230,280,200,140,175,100,155,65,135,85,145,100,175,130,195,145,235,140,220};//exp3
int Azmuth[] ={ 120,55,130,80,155,85,165,100,180,135,220,135,190,115,165,115,200,155,100,165,90,155,105,185,140,210,150}; //exp4
//int Azmuth[] ={ 145,60,155,230,170,225,150,80,140,65,135,45,105,185,115,175,95,30,125,55,145,200,135,205,125,180,90};//exp5
//int Azmuth[] ={ 125,185,130,215,130,215,135,75,145,95,160,115,185,265,200,275,185,245,180,250,170};//exp6
//int Azmuth[] ={ 135,85,140,90,170,125,195,155,210,165,205,150,200,135,180,125,170,110,165,100,155,85,140,80,135,70,115};//exp7


int Azmuth_index=0;  // IS A GLOBAL VARIABLE

//***********************************************

void setting();     

void PI_Motor() ;
double Get_headingDiff(double Input, double Setpoint);
double PI_Controller(double error);
double integral=0;
readcompass R_compass;  //DECLARING A CLASS I.E. R_compass is object of type readcompass
int Azmuth_num;     // A GLOBAL VARIABLE
/*
 */
//*************************

struct DATA{      // CREATING STRUCTURE WITH 3 FIELDS,  time, pwm and value heading

	double during_time;     //TIME STAMP 
  int value_pwm;
  float value_heading;
};

DATA data[10000];   // CREATE ARRAY OF STRUCTURES, I.E. 10000

int data_index=0;   // A GLOBAL VARIABLE

int main()
{

	setting();        // INITIAL SETTING, SET COMPASS, GPIO, PWM. 

	Azmuth_num=sizeof(Azmuth)/sizeof(Azmuth[0]); 
	cout<<Azmuth_num<<endl;

	ofstream out("data.txt", ios::app);   // OPENING FILE data.txt TO WRITE. THIS FILE MUST PRE-EXIST ON COMPUTER AT RIGHT LOCATION
			   if(!out) {                     // EXIT PROGRAM IF FILE NOT THERE, ADD WARNING ON EXIT
			           cout << "Cannot open file.\n";
			           return 1;
			         }

	out<<endl;
	out<<endl;
	out<<endl;
	out<<endl;
	out<<endl;
	out<<endl;
	out<<"In exp4The Azmuth[] ={ 120,55,130,80,155,85,165,100,180,135,220,135,190,115,165,115,200,155,100,165,90,155,105,185,140,210,150} and kp=1.1, ki=1.5;"<<endl;

	out.close();  // CLOSE file data.txt
	sleep(2);


	while (Azmuth_index < Azmuth_num)
	{
		 PI_Motor();  //Azmuth index is incremented in PI_Motor()

	}
		motor_rotate_direction(3);    //STOP THE MOTOR

	    out.open("data.txt", ios::app);   // OPEN THE FILE data.txt 
      
		   if(!out) {
		           cout << "Cannot open file.\n";
		           return 1;
		         }

      // WRITE TO FILE data.txt

		   for(int i=1;i<data_index;i++)
		   {
			   out<<data[i].during_time<<" "<<data[i].value_pwm<<" "<<data[i].value_heading<<endl;
			   cout<<i<<endl;
			   //this_thread::sleep_for(std::chrono::milliseconds(500));

		   }

		  // out.write((char*) &DATA, sizeof(struct data));

		   out.close();   //CLOSE THE FILE data.txt




		return 0;



}


void setting()
{
		R_compass.setting_compass();//setting compass
		sleep(1);
		//set motor pwm and direction
		setting_GPIO();
		sleep(1);
		setting_PWM();
		sleep(1);
	}



void PI_Motor()   //motor control
  {
	 t2 = steady_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    if ( time_span.count()>SampleTimeInSec)
    {
      float headingRaw =R_compass.c_heading();   //Get current compass
      //cout<<"heading: "<< headingRaw<<endl;
      float targetHeading = Azmuth[Azmuth_index];     //Get destination direction
      Input=headingRaw;
      Setpoint = targetHeading;
      float headingDiff = Get_headingDiff(Input, Setpoint);
      if (abs(headingDiff) < offsetTol){
    	  motor_rotate_direction(3);
    	  if(Azmuth_index<Azmuth_num)
    	   {Azmuth_index++;
    	   }
        }
       else
       {
        Output = PI_Controller(headingDiff);
       // cout<<"output: "<< Output<<endl;
        if (Output > 0)
        {
        	motor_rotate_direction(2);
        	 Motor_speed(Output);
        }
        else
        {
        	motor_rotate_direction(1);
        	 Motor_speed(abs(Output));
          }
        }
        t1 = steady_clock::now();
        data[data_index].during_time=time_span.count();
        data[data_index].value_pwm=Output;
        data[data_index].value_heading=Input;
        data_index++;



    }
  }

double Get_headingDiff(double Input, double Setpoint){
    double error1 = Setpoint - Input;
    double error2 = Setpoint + 360 - Input;
    double error3 = Setpoint - Input - 360;
    double error;
    if (abs(error1)  <= abs(error2) && abs(error1) <= abs(error3))
    {
      error = error1;
      }
    else if (abs(error2) <= abs(error1) && abs(error2) <= abs(error3))
    {
      error = error2;
      }
    else
    {
      error = error3;
    }
    return error;
  }

double PI_Controller(double error)
  {

	integral += error*SampleTimeInSec;
	Output=error*kp+integral*ki;
    if (Output > 254){
      Output = 254;
      }
    else if (Output < -254){
        Output = -254;
      }
    return Output/254*100;
  }






#include <stdio.h>
#include "CE32_sqrs.h"

char sqrs(short data)
{
		const short fs=250;
		static short t0,t1,t2,t3,t4,t5,t6,t7,t8,t9;
	  static short filter; //Filtered data
		static short nslope=0;
		static short slopecrit=5000;
		static short maxslope=0;
		static short scmin=500;
		static short scmax=5000;
		static short maxtime=0;
		static short sign=0;
	
		static short time=0; //Time passed from last QRS
		static short qtime=0;
		const  short s2=500;
		const  short ms160=40;
		const  short ms200=50;
	
		char resp=0;
	  t0 = data;
		filter =  t0 + 4*t1 + 6*t2 + 4*t3 + t4- t5- 4*t6 - 6*t7 - 4*t8 - t9; //Filtering data
		if (time % s2 == 0) {		//If time reach 2 seconds
				if (nslope == 0) {		//
						slopecrit -= slopecrit >> 4;
						if (slopecrit < scmin) slopecrit = scmin;
				}
				else if (nslope >= 5) {
						slopecrit += slopecrit >> 4;
						if (slopecrit > scmax) slopecrit = scmax;
				}
		}
		if (nslope == 0 && abs(filter) > slopecrit) {
				nslope = 1; maxtime = ms160;
				sign = (filter > 0) ? 1 : -1;
				qtime = time;
		}
		if (nslope != 0) {
				if (filter * sign < -slopecrit) {
						sign = -sign;
						maxtime = (++nslope > 4) ? ms200 : ms160;
				}
				else if (filter * sign > slopecrit &&
								 abs(filter) > maxslope)
						maxslope = abs(filter);
				if (maxtime-- < 0) {
						if (2 <= nslope && nslope <= 4) {
								slopecrit += ((maxslope>>2) - slopecrit) >> 3;
								if (slopecrit < scmin) slopecrit = scmin;
								else if (slopecrit > scmax) slopecrit = scmax;
								resp = 1;
								time = 0;	//
						}
						else if (nslope >= 5) {
							resp = -1;
						}
						nslope = 0;
				}
		}
		t9 = t8; t8 = t7; t7 = t6; t6 = t5; t5 = t4;		//Push filter state
		t4 = t3; t3 = t2; t2 = t1; t1 = t0; 
		time++;
		return resp;
}
		
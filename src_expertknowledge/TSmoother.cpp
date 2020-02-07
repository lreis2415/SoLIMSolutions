/*
Copyright (C) 2015, 2016 SoLIM Group, University of Wisconsin-Maison, 
Madison, Wisconsin, USA and Institute of Geographic Sciences and Natural 
Resources Research, Chinese Academy of Sciences, Beijing, China

This program is free software: you can redistribute it and/or 
modify it under the terms of the GNU General Public License 
version 3, 2007 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
GNU General Public License for more details.

A copy of the GNU General Public License version 3 is provided
 in file GPLv3license.txt. This is also available at: 
 https://www.gnu.org/licenses/gpl-3.0.html.

==================additional conditions============================
If the GNU General Public License conditions do not meet your 
needs to use or incorporate this program (or part of it) into 
other software, SoLIM Solutions may also be available under 
alternative licenses (multi licensing). This allows the program 
to be sold or licensed for use in proprietary applications. In 
this case, please contact any of the members of the SoLIM Group 
listed below to discuss a licensing agreement.
SoLIM Group:
(A-Xing Zhu, Cheng-Zhi Qin, Peng Liang, Fang-He Zhao)
Stat Key Laboratory of Resources and Environmental Information System
Institute of Geographic Sciences and Natural Resources Research,
Chinese Academy of Sciences
11A Datun Road, Chaoyang District, Beijing 100101, PR China
Or 
(A-Xing Zhu)
Department of Geography
University of Wisconsin-Maison
550 N. Park St., Madison, WI 53706, USA
https://solim.geography.wisc.edu/index.htm
Email: dsm_solim@yahoo.com

If you wish to use this program (or part of it), please do credit us
either by citing the proper references in your published papers or in 
your program. Related publications on SoLIM can be found in Appendix 
D: SoLIM Publications in the Appendices of the SoLIM Solutions Help 
system at: http://solim.geography.wisc.edu/software/SoLIMSolutions2013/Help_HTML/index.html. 



By downloading this program, you have agreed to conditions of this license.
*/
// TSmoother.cpp: implementation of the CTSmoother class.
//
// Rongxun Wang @ UW - Madison
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TSmoother.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTSmoother::CTSmoother()
{
	a0 = NULL;
	a1 = NULL;
	a2 = NULL;
	a3 = NULL;
}

CTSmoother::~CTSmoother()
{
	if (a0 != NULL){
		delete []a0;
		a0 = NULL;
	}//if

	if (a1 != NULL){
		delete []a1;
		a1 = NULL;
	}//if
	
	if (a2 != NULL){
		delete []a2;
		a2 = NULL;
	}//if
	
	if (a3 != NULL){
		delete []a3;
		a3 = NULL;
	}//if
}

bool CTSmoother::Interpolate(float *x, float *y, int n, float *xi, float *yi, int ni)
{
/*
  purpose: interpolate yi(i) at xi(i), i=1,2,...ni using cardinal spline (exact, local, c1)

   Input:
      x          vector of known x values, must be in ascending order and unique
      y          vector of known y values
      n          length of x, y (greater than 3)
      xi         vector of x values for which an interpolated y is desired
      ni         number of interpolated values yi

   Output:
      yi         vector of ni interpolated values corresponding to xi

   Note:
     1) i do not check that x meets the above requirements on entry
     2) for xi outside range of x, i extrapolate
*/
	
	if (n < 4){
		return false;
	}//if

	Coefficient(x, y, n);

	Evaluate(x, y, n, xi, yi, ni);

	return true;
}

void CTSmoother::Coefficient(float *x, float *y, int n)
{
	int i = 0;

	double d0 = 0, d1 = 0;

	if (a0 != NULL){
		delete []a0;
		a0 = NULL;
	}//if
	a0 = new double[n - 1];

	if (a1 != NULL){
		delete []a1;
		a1 = NULL;
	}//if
	a1 = new double[n - 1];

	if (a2 != NULL){
		delete []a2;
		a2 = NULL;
	}//if
	a2 = new double[n - 1];

	if (a3 != NULL){
		delete []a3;
		a3 = NULL;
	}//if
	a3 = new double[n - 1];

	for (i = 0; i < n - 1; i++){
		if (i == 0){
			d0 = 0;
			if (x[2] == x[0]){
				d1 = (double)99999999;
			}//if
			else{
				d1 = (y[2] - y[0]) / (x[2] - x[0]);
			}//else
		}//if
		else if (i == (n - 2)){
			d1 = 0;
			if (x[n - 1] == x[n - 3]){
				d0 = (double)99999999;
			}//if
			else{
				d0 = (y[n - 1] - y[n - 3]) / (x[n - 1] - x[n - 3]);
			}//else
		}//else if
		else{
			if (x[i + 1] == x[i - 1]){
				d0 = (double)99999999;
			}//if
			else{
				d0 = (y[i + 1] - y[i - 1]) / (x[i + 1] - x[i - 1]);
			}//else

			if (x[i + 2] == x[i]){
				d1 = (double)99999999;
			}//if
			else{
				d1 = (y[i + 2] - y[i]) / (x[i + 2] - x[i]);
			}//else
		}//else

		if (x[i + 1] == x[i]){
			a3[i] = (double)99999999;
			a2[i] = (double)99999999;
			a1[i] = (double)99999999;
			a0[i] = (double)99999999;
		}//if
		else{
			a3[i] = ((x[i + 1] - x[i]) * (d1 + d0) - 2 * (y[i + 1] - y[i])) / ((x[i + 1] - x[i]) * (x[i + 1] - x[i]) * (x[i + 1] - x[i]));
			a2[i] = ((d1 - d0) - 3 * a3[i] * (x[i + 1] * x[i + 1] - x[i] * x[i])) / (2 * (x[i + 1] - x[i]));
			a1[i] = d0 - 3 * a3[i] * x[i] * x[i] - 2 * a2[i] * x[i];
			a0[i] = y[i] - a3[i] * x[i] * x[i] * x[i] - a2[i] * x[i] * x[i] - a1[i] * x[i];
		}//else
	}//for(i)

}

void CTSmoother::Evaluate(float *x, float *y, int n, float *xi, float *yi, int ni)
{
	int i = 0, j = 0;

	for (i = 0; i < ni; i++){
		if (xi[i] <= x[0]){
			yi[i] = y[0];
			continue;
		}//if

		if (xi[i] >= x[n - 1]){
			yi[i] = y[n - 1];
			continue;
		}//if

		for (j = 0; j < n - 1; j++){
			if ((xi[i] >= x[j]) && (xi[i] <= x[j + 1])){
				break;
			}//if
		}//for(j)

		yi[i] = (float)(a0[j] + xi[i] * (a1[j] + xi[i] * (a2[j] + xi[i] * a3[j])));
	}//for(i)

}


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
//UncertainColors.h
//for creating colors in hardened maps.
//Created by Jim Burt.
//format modifed by Xun Shi on Mar. 6, 2002

#pragma once
#include <stdio.h>
#include "utility.h"

//MAXPALETTESIZE must be no larger than 3dMapper value
#ifndef UCLIMITSH
	#define UCLIMITSH
	#define MAXPALETTESIZE 65536
	#define MAXBASE 100
	#define MAXSATS 1001
	#define MAXUNCERTAINTY 0.999
	#define ITERMAX 30
#endif
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif
#ifndef MAX
	#define MAX(x,y)    ( ((x) > (y)) ? (x) : (y))
#endif
#ifndef MIN
	#define MIN(x,y)    ( ((x) < (y)) ? (x) : (y))
#endif
#ifndef D2R
	#define D2R(a) ((a)*M_PI/180.)
#endif
#ifndef R2D
	#define R2D(a) ((a)*180./M_PI)
#endif


class UncertainColors{
public:
	UncertainColors();
	~UncertainColors();
	int BuildHSVPalette(int *baseID, int nbase);

	int BuildLjgPalette(int *baseID, int nbase);
	void SetGamma(bool OnOff);
	void SetConvergeOnWhite(bool OnOff);
	bool SetLevels(int levels);
	float Pixel3d(int base, float uncertainty);
	int WritePalette(FILE *filepointer);
	int WritePalette(char *filename);

private:
	float rasterlookup(PALENTRY *palette, int n, float val);
	void AllocatePalette(int nSats);
	void hsv2rgb(float h, float s, float v, float &r, float &g, float &b);
	void rgb2hsv( float r,float g, float b, float &h, float &s, float &v);

	void InitializeLjg(void);
	bool Invert3by3(double A[3][3],double B[3][3]);
	double Findjg(double L, int ncolors, double *j, double *g);
	double FindLjgTheta(double L, int ncolors, double &r);
	double FindLjgEdge(double L, double theta);

	int XYZ2RGB(double X,double Y, double Z, double &R, double &G, double &B);
	int RGB2XYZ(double R,double G, double B, double &X, double &Y, double &Z);
	void XYZ2Yxy(double X, double Y, double Z, double &x, double &y);
	void XYZ2Ljg(double X,double Y, double Z, double &L, double &j, double &g);
	int Ljg2RGB(double L,double j, double g, double &R, double &G, double &B);
	void RGB2Ljg(double R,double G, double B, double &L, double &j, double &g);
	
	bool BiNewton(double Y, double Xguess, double a, double b,
				double &X, double F(double X, double Parms[]), double Parms[],
				double XTOL, double FTOL);

    void Bisection(double Y, double a, double b, double &Xmid, double &Xlast,
		   double F(double X, double Parms[]), double Parms[],
		   double XTOL, double FTOL, int &MaxIter);

	void DisplayRGB(double r, double g, double b,
		   unsigned char &r255, unsigned char &g255, unsigned char &b255);

	double Phi(double w);

	bool WantGammaCorrection,WantConvergeOnWhite;
	int nColors,nLevels,nSats;
	PALENTRY *palette;

	double RGB2XYZMat[3][3],XYZ2RGBMat[3][3];
};


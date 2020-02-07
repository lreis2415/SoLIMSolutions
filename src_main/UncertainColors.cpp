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
//UncertainColors.cpp
//for coloring hardened maps
//Created by Jim Burt.
//format modified by Xun Shi on Mar. 6, 2002


#include "StdAfx.h"
#include "SoLIM.h"
#include <math.h>
#include "UncertainColors.h"

UncertainColors::UncertainColors(){
	nColors             = 0;
	nLevels             = 101;
	palette             = NULL;
	WantGammaCorrection = true;
	WantConvergeOnWhite = true;
}

UncertainColors::~UncertainColors(){
	if(palette) free(palette);
}

bool UncertainColors::SetLevels(int nlevels){
	if(nlevels > 0 && nlevels < MAXSATS){
		nLevels = nlevels;
		return true;
	}
	else return false;
}

void UncertainColors::AllocatePalette(int nBase){
	nSats = MIN(nLevels, MAXPALETTESIZE/nBase);
	while(nSats*nBase + 2 >= MAXPALETTESIZE) nSats--;

	nColors  = nSats*nBase + 2;

	if(palette) free(palette);

	palette = (PALENTRY*) malloc(nColors*sizeof(PALENTRY));

	for(int k=0; k<nColors; k++){
		palette[k].r = palette[k].g = palette[k].b = 0;
		palette[k].rasterval = 0.;
		palette[k].index     = 0;
	}
	palette[0].r = palette[0].g = palette[0].b = 0;
	palette[0].rasterval = 0.;
	palette[0].index     = 0;

	palette[1].r = palette[1].g = palette[1].b = 255;
	palette[1].rasterval = 0. + MAXUNCERTAINTY;
	palette[1].index     = 0;
}

int UncertainColors::BuildHSVPalette(
      int *baseValues, //input: array[nbase] of base values
      int nBase){      //input: number of classes (> 0)
					   //returns:  number of colors or -1 if fails

/* This builds a palette based on any number of hardened
   soil classes.  Each class has a base value supplied
   by the caller.  Please note, a base value of zero
   is not valid.

   The palette is based on the HSV color model.  Hues are equally
   spaced around the color wheel, one for each base class. Within
   each hue, the colors run from fully saturated to white.  White
   represents maximun uncertainty (minimum membership in a category).

   If the number of categories is 6 or less, all categories
   will have a color value of unity. Thus full membership
   will have colors like bright red, bright yellow, bright green, etc.

   If the number of colors is greater than 6, color values
   alternate from one color to the next.  The 1st, 3rd, 5th...
   colors have values of 0.5, and are darker than the
   the 2nd, 4th, 6th, etc.

   Numerically, palette entries will have index values
   of the form base+uncertainty.  Uncertainty is
   less than one, so it forms the fractional part
   of the index.  Therefore, in decimal form indexes
   have the form base.uncertainty.

   The first entry in the palette is always black, with
   a base value of zero. This can be used to represent
   a missing color. Each class will have the same number
   of palette entries, with uncertainties in equal
   steps within each class.  The number of uncertainties
   within each class is set equal to MAXSATS, unless that
   would create a palette larger than MAXPALETTESIZE,
   the largest 3dMapper can handle.

   The smallest uncertainty in the palette is zero, and
   the largest is MAXUNCERTAINTY.  Thus 6.0 means full
   membership in class 6.  This would be given an rgb
   corresponding to full saturation in the hue and
   value for class 6.  A value of 6.MAXUNCERTAINTY
   means smallest membership in class 6. The latter
   will be assigned as white.*/

	int    i, j, k;
	float  r,g,b,hue,val,sat;
	double uncertainty;

	if(nBase < 1 ){
		printf("number of base values not positive");
		return -1;
	}
	AllocatePalette(nBase);

	k = 2;
	for(i = 0; i < nBase; i++){
		hue = 360.*i/nBase;

		//make saturation go from 0 to 1, with nSats steps for this base value
		for(j = 0; j < nSats; j++){
			sat = (float) j / (nSats-1);

			//interpolate between .5 (sat=1) and unity (sat=0)
			if(nColors > 6 && i%2 == 0) val = .5*sat + 1.*(1.-sat);
			//full value
			else      val = 1.;

			uncertainty = MIN(MAXUNCERTAINTY,1.-sat);
			hsv2rgb(hue, sat, val, r,g,b);
			DisplayRGB(r, g, b, palette[k].r, palette[k].g, palette[k].b);
			palette[k].rasterval = baseValues[i] + uncertainty;
			k++;
		}
	}
	if(k != nColors){
		printf("Error in BuildPalette-- %d colors != %d",nColors,k);
		free(palette);
		return -1;
	}

	//sort palette in rastervalue order (that is, in base.uncertainty order)
	int PaletteValueCompare(const void *a, const void *b);
	qsort((void *) &(palette[0]),nColors, sizeof(PALENTRY), PaletteValueCompare);

	return nColors;
}

int UncertainColors::BuildLjgPalette(
      int *baseValues, //input: array[nbase] of base values
      int nBase){      //input: number of classes (1 to MAXBASE)
					   //returns:  number of colors or -1 if fails

/* This builds a palette based on up to MAXBASE hardened
   soil classes using the Ljg color model.  Each class
   has a base value supplied by the caller.  Please note:
   zero is not a valid base color. This function builds
   as a many rgb values as possible for each base class.
   The rgb entries run from "fully saturated" colors to
   grey (j = g = 0). Grey represents complete uncertainty (
   minimum membership in a category).

   If the number of categories is 6 or less, all categories
   will have the same Lightness value. Lightness will be
   chosen so that "fully saturated" colors are as far from
   grey as possible. Thus full membership will have colors
   like bright red, bright yellow, bright green, etc. To find
   the optimum Lightness, the progam tests Lightness values
   from -3 to 5 in increments of 0.5. Experience shows values
   of 2 to 3 are most common.

   If the number of colors is greater than 6, Lightness values
   alternate from one color to the next.  The 1st, 3rd, 5th...
   colors have Lighness one unit less than that of the 2nd, 4th...
   Thus the 1st, 3rd, 5th... are darker. Again, Lightness
   values are chosen so that "fully saturated" colors are as
   far from grey as possible.

   Numerically, palette entries will have index values
   of the form base+uncertainty.  Uncertainty is
   less than one, so it forms the fractional part
   of the index.  Therefore, in decimal form indexes
   have the form base.uncertainty.

   The first entry in the palette is always black, with
   a base value of zero. This can be used to represent
   a missing color. Each class will have the same number
   of palette entries, with uncertainties in equal
   steps within each class.  The number of uncertainties
   within each class is set equal to MAXSATS, unless that
   would create a palette larger than MAXPALETTESIZE,
   the largest 3dMapper can handle.

   The smallest uncertainty in the palette is zero, and
   the largest is MAXUNCERTAINTY.  Thus 6.0 means full
   membership in class 6.  This would be given an rgb
   corresponding to full saturation in the hue and
   value for class 6.  A value of 6.MAXUNCERTAINTY
   means smallest membership in class 6. The latter
   will be assigned as grey.*/

	int i, j, k;
	double rmax,Ltest,L;
	static double jj[MAXBASE],gg[MAXBASE];
	double uncertainty;

	if(nBase < 1){
		printf("number of base values not positive");
		return -1;
	}
	AllocatePalette(nBase);

	InitializeLjg();

	rmax = 0.;
	for(Ltest = -3; Ltest <= 5; Ltest+=.5){
		double r,jtest[MAXBASE],gtest[MAXBASE];
		r = Findjg(Ltest,nBase,jj,gg);
		if(r > rmax){
			L = Ltest;
			for(int m=0; m<nBase; m++){
				jj[m] = jtest[m];
				gg[m] = gtest[m];
			}
			rmax = r;
		}
	}

	k = 2;
	for(i = 0; i < nBase; i++){
		double Lmax,Lmin,jmax,jmin,gmax,gmin;

		if(nBase > 5 && i%2 == 0) Lmax = L - 1;
		else                      Lmax = L;

		if(WantConvergeOnWhite)   Lmin = 5;
		else Lmin = Lmax;

		jmax   = jj[i];
		gmax   = gg[i];
		jmin   = 0;
		gmin   = 0;

		//make "saturation" go from 0 to maximum,
		//with nSats steps for this base value
		for(j = 0; j < nSats; j++){
			unsigned char r255,g255,b255;
			double f,ltemp,jtemp,gtemp,r,g,b;

			f = (float) j / (nSats-1);

			ltemp = (1.-f)*Lmin + f*Lmax;
			jtemp = (1.-f)*jmin + f*jmax;
			gtemp = (1.-f)*gmin + f*gmax;

			if( Ljg2RGB(ltemp,jtemp,gtemp, r,g,b) == 0){
				DisplayRGB(r,g,b, r255,g255,b255);
				palette[k].r = r255;
				palette[k].g = g255;
				palette[k].b = b255;
			}
			else{
				palette[k].r = 0;
				palette[k].g = 0;
				palette[k].b = 0;
			}
			uncertainty = MIN(MAXUNCERTAINTY,1.-f);
			palette[k].rasterval = baseValues[i] + uncertainty;
			k++;
		}
	}
	if(k != nColors){
		printf("Error in BuildPalette-- %d colors != %d",nColors,k);
		free(palette);
		return -1;
	}

	//sort palette in rastervalue order (that is, in base.uncertainty order)
	int  PaletteValueCompare( const void *a, const void *b);
	qsort((void *) &(palette[0]),nColors, sizeof(PALENTRY), PaletteValueCompare);

	return nColors;
}

int  PaletteValueCompare( const void *a, const void *b){

	#define ARASTERVAL (((PALENTRY *)a)->rasterval)
	#define BRASTERVAL (((PALENTRY *)b)->rasterval)

	if     ( ARASTERVAL < BRASTERVAL ) return (-1);
	else if( ARASTERVAL > BRASTERVAL ) return ( 1);
	else return (0);
}

float UncertainColors::Pixel3d(
	int base,            //input: base index (hardened soil class)
	float uncertainty){  //input: uncertainty (0-1)

/* Return the palette entry whose base matches the
   input base and whose uncertainty is closest to
   the input uncertainty.

   Base out of range:
      If input base is < smallest palette base, return
      the first palette entry. If input base is > largest
      palette base, return the last palette entry. Note
      that uncertainty is ignored in these cases.*/

	float colorindex = base + MIN(MAXUNCERTAINTY,uncertainty);
	return rasterlookup(palette, nColors, colorindex );
}

float UncertainColors::rasterlookup(PALENTRY *palette, int n, float val){
	unsigned int j,jhigh,jmid,jlow;

	if     ( val <= palette[0].rasterval)   return palette[0].rasterval;
	else if( val >= palette[n-1].rasterval) return palette[n-1].rasterval;

	jlow  = 0;
	jhigh = n-1;

	while(jhigh-jlow > 1){
		jmid = (jhigh+jlow) >> 1;
		if(val >= palette[jmid].rasterval) jlow  = jmid;
		else                               jhigh = jmid;
	}

	//is val closer to palette[jlow]?
	if((val - palette[jlow].rasterval)<(palette[jhigh].rasterval - val))
		j = jlow;
	//or closer to jhigh?
	else j = jhigh;

	return palette[j].rasterval;
}

int UncertainColors::WritePalette(char *filename){ //entry: name of file to receive data
	int  result;
	FILE *fp;

	if((fp = fopen(filename,"rb+")) == NULL){
		printf("Couldn't open %s for palette",filename);
		return 1;
	};
	result = WritePalette(fp);
	if(fp) fclose(fp);
	return result;
}

int UncertainColors::WritePalette(FILE *fp){
//write the palette at end of file
//return nonzero if fails

	if(fseek(fp, 0L, SEEK_END)){
		printf("Couldn't move pointer to file end.");
		return 1;
	}
	if( fwrite(palette, sizeof(PALENTRY)*nColors,1, fp) != 1){
		printf("Couldn't write palette.");
		return 1;
	}
	return 0;
}

void UncertainColors::hsv2rgb(
     float h,   //input:  hue (0-360 degrees)
     float s,   //input:  saturation (0-1)
     float v,   //input:  value (0-1)
     float &r,  //output: red   (0-1)
     float &g,  //output: green (0-1)
     float &b){ //output: blue  (0-1)

	int j;
	double f,p,q,t;

	if(h>= 0. && s>= 0. && s<=1. && v>=0. && v<=1.){
		while( h > 360.) h -= 360.;

		if(s < 0.0001) (r = v, g = v, b = v);
		else{
			h  /= 60.;
			j  = (int) floor(h);
			f  = h - (double) j;
			p  = v*(1. - s);
			q  = v*(1.  - s*f);
			t  = v*(1. - (s*(1. - f)));
			switch(j){
			case 0: (r = v, g = t, b = p);  break;
            case 1: (r = q, g = v, b = p);  break;
            case 2: (r = p, g = v, b = t);  break;
            case 3: (r = p, g = q, b = v);  break;
            case 4: (r = t, g = p, b = v);  break;
            case 5: (r = v, g = p, b = q);  break;
            case 6: (r = v, g = t, b = p);  break;
			}
		}
	}
}

void UncertainColors::rgb2hsv(
	float r,     //input:  red (0-1)
	float g,     //input:  green (0-1)
	float b,     //input:  blue (0-1)
	float &hue,  //output: hue (0-360 degrees)
	float &sat,  //output: saturation (0-1)
	float &val){ //output: value (0-1)

	double r1,g1,b1,cmax,cmin,cdif,rdist,gdist,bdist;

	r1 = r;
	g1 = g;
	b1 = b;

	if(r1 > g1){
		if(r1 > b1) cmax = r1;
		else        cmax = b1;
	}
	else{
		if(g1 > b1) cmax = g1;
		else        cmax = b1;
	}

	if(r1 < g1){
		if(r1 < b1) cmin = r1;
		else        cmin = b1;
	}
	else{
		if(g1 < b1) cmin = g1;
		else        cmin = b1;
	}
	cdif = cmax - cmin;
	val = cmax;

	if( cmax <  0.0001){
		sat = 0.;
		hue = 0;
	}
	else sat = cdif/cmax;

	if(sat > 0.0001) {
		rdist = (cmax - r1)/cdif;
		gdist = (cmax - g1)/cdif;
		bdist = (cmax - b1)/cdif;

		if(r1 == cmax)      hue = bdist - gdist;
		else if(g1 == cmax) hue = 2. + rdist - bdist;
		else                hue = 4. + gdist - rdist;
		hue *= 60.;
		if(hue < 0.) hue += 360.;
	}
}

double UncertainColors::Findjg(double L, int ncolors, double *j,double *g){
	double theta,radius;
	theta = FindLjgTheta(L,ncolors, radius);
	for(int i=0; i< ncolors; i++){
		j[i] = radius*cos(D2R(theta + i*360./ncolors));
		g[i] = radius*sin(D2R(theta + i*360./ncolors));
	}
	return radius;
}


double UncertainColors::FindLjgTheta(double L, int ncolors, double &rbest){
	int    i;
	double theta,dtheta,theta0,dtheta0,thetabest;
	double r,rmin,ltop;
	r = ncolors;
	dtheta = 360./r;
	dtheta0 = 2;
	rbest = 0.;
	theta0 = thetabest = 0.;

	while(theta0 < dtheta){
		rmin = 1.e50;
		for(i = 0; i < ncolors; i++){
			theta = theta0 + i*dtheta;
			ltop = L;

			//here to jump up/down in L
			if(ncolors > 6 && i%2 == 0) 
				ltop = L-1.;
			r = FindLjgEdge(ltop,theta);
			rmin = MIN(rmin,r);
		}
		if(rmin > rbest){
			rbest     = rmin;
			thetabest = theta0;
		}
		theta0 += dtheta0;
	}
	return thetabest;
}

double UncertainColors::FindLjgEdge(double L, double theta){
	double r;
	double j,g;
	double R,G,B;

	for(r = 0; r < 30; r+=.1){
		j = r*cos(D2R(theta));
		g = r*sin(D2R(theta));

		if(Ljg2RGB(L,j,g,R,G,B) != 0){
			r -= .1;
			break;
		}
	}
	return r;
}

void UncertainColors::RGB2Ljg(double R,double G, double B,
                       double &L, double &j, double &g){
	double X,Y,Z;
	RGB2XYZ(R,G,B, X,Y,Z);
	XYZ2Ljg(X,Y,Z, L, j, g);
}

int UncertainColors::Ljg2RGB(double L,double j, double g,
                       double &R, double &G, double &B){
	bool   LSearchOK,wSearchOK;
	int    result;
	double Y0,Y30,C,a,b,w;
	double ap,bp,w3,wa,wb;
	double X,Y,Z;
	double parms[2];
	double Y02L(double Y, double parms[]);
	double Phi(double w, double parms[]);

	if(L<-13.51 || j<-18.79 || g<-18.04 || L>7.14 || j>13.23 || g>11.87){
		R = G = B = 0;
		return 1;
	}
	Y0 = 50.;
	LSearchOK = BiNewton(L, 100., 0., 110., Y0, Y02L, NULL, 1.e-6, 1.e-6);
	
	Y30 = fabs(Y0-30.);
	if(Y0 > 30)
		C = 1. + .042*pow(Y30,1./3.) / (pow(Y0, 1./3.) - 2./3.);
	else
		C = 1. - .042*pow(Y30,1./3.) / (pow(Y0, 1./3.) - 2./3.);

	a = g/C;
	b = j/C;

	ap = (9.7*a -  4.0*b)/139.69;
	bp = (8.0*a - 17.7*b)/139.69;
	parms[0] = ap;
	parms[1] = bp;
     
	w = 2.5;
	wSearchOK = BiNewton(Y0, w, 0.,pow(100.,1./3.), w, Phi, parms, 1.e-6, 1.e-6);
	w3 = pow(w,3);
	wa = pow(w+ap,3);
	wb = pow(w+bp,3);

	X =  1.0626*w3 - 0.4121*wa + 0.2975*wb;
	Y =  0.3599*w3 + 0.6401*wa ;//+ 0.0000*wb;
	Z = -0.0001*w3 - 0.3690*wa + 1.4424*wb;

	result = XYZ2RGB(X/100.,Y/100.,Z/100.,R,G,B);
	if(fabs(j-g) < .01){
		R = (R+G+B)/3.;
		G = B = R;
	}
	return result;
}

void UncertainColors::DisplayRGB(double r, double g, double b,
  unsigned char &r255, unsigned char &g255, unsigned char &b255){

	double c1    = 0.055;
	double c2    = 1.055;
	double c3    = 12.92;
	double gamma = 2.4;
	double tol   = 0.00304;

	if(WantGammaCorrection){
		if(r > tol) r = c2*pow(r, 1./gamma) - c1;
		else        r = r*c3;

		if(g > tol) g = c2*pow(g, 1./gamma) - c1;
		else        g = g*c3;

		if(b > tol) b = c2*pow(b, 1./gamma) - c1;
		else        b = b*c3;
	}

	r255 = MIN(255,MAX(0,.5+255*r));
	g255 = MIN(255,MAX(0,.5+255*g));
	b255 = MIN(255,MAX(0,.5+255*b));
}

int UncertainColors::RGB2XYZ(double R,double G, double B,
                       double &X, double &Y, double &Z){

	X = R * RGB2XYZMat[0][0] + G * RGB2XYZMat[0][1] + B * RGB2XYZMat[0][2];
	Y = R * RGB2XYZMat[1][0] + G * RGB2XYZMat[1][1] + B * RGB2XYZMat[1][2];
	Z = R * RGB2XYZMat[2][0] + G * RGB2XYZMat[2][1] + B * RGB2XYZMat[2][2];
	return 0;
}

int UncertainColors::XYZ2RGB(double X,double Y, double Z,
                       double &R, double &G, double &B){

	int flag;
	R =  X * XYZ2RGBMat[0][0] + Y * XYZ2RGBMat[0][1] + Z * XYZ2RGBMat[0][2];
	G =  X * XYZ2RGBMat[1][0] + Y * XYZ2RGBMat[1][1] + Z * XYZ2RGBMat[1][2];
	B =  X * XYZ2RGBMat[2][0] + Y * XYZ2RGBMat[2][1] + Z * XYZ2RGBMat[2][2];

	if(R >= -0.0 && R <= 1.0 &&  G >= -0.0 && G <= 1.0 &&  B >= -0.0 && B <= 1.0)
		flag = 0;
	else
		flag = 1;

	R = MIN(1.,MAX(0.,R));
	G = MIN(1.,MAX(0.,G));
	B = MIN(1.,MAX(0.,B));
	
	return flag;
}

void UncertainColors::XYZ2Ljg(double X,double Y, double Z,
                       double &L, double &j, double &g){

	double R,G,B,C,ScriptL,Y0,x,y;
	double Y03,R3,G3,B3;
	XYZ2Yxy(X,Y,Z, x, y);

	X *= 100;
	Y *= 100;
	Z *= 100;

	Y0 = Y*(4.4934*x*x + 4.3034*y*y - 4.276*x*y - 1.3744*x - 2.5643*y + 1.8103);
	Y03 = pow(fabs(Y0),1./3.);

	if(Y0 > 30)
		ScriptL = 5.9*(Y03 - 2./3. + 0.042*pow(fabs(Y0-30.),1./3.));
	else
		ScriptL = 5.9*(Y03 - 2./3. - 0.042*pow(fabs(Y0-30.),1./3.));

	C = ScriptL/(5.9*(Y03 - 2./3));
	L = (ScriptL - 14.4)/sqrt(2.);

	R =  0.7990*X + 0.4194*Y - 0.1648*Z;
	G = -0.4493*X + 1.3265*Y + 0.0927*Z;
	B = -0.1149*X + 0.3394*Y + 0.7170*Z;

	R3 = pow(fabs(R),1./3.);
	G3 = pow(fabs(G),1./3.);
	B3 = pow(fabs(B),1./3.);

	j = C*(1.7*R3 + 8.*G3 - 9.7*B3);
	g = C*(-13.7*R3 + 17.7*G3 - 4.*B3);
}

void UncertainColors::XYZ2Yxy(double X, double Y, double Z, double &x, double &y){
	double denom = X+Y+Z;
	if(denom != 0.){
		x = X/denom;
		y = Y/denom;
	}
	else{
		x = 0;
		y = 0;
	}
}

bool UncertainColors::BiNewton(double Y, double X0, double a, double b,
       double &X, double F(double X, double Parms[]), double Parms[],
       double XTOL, double FTOL){
	
	int    iter,iter2,itertot;
	double xnew,xlast,xlast0,dFdx,dx;
	double Fnew,Flast;

	iter = 5;
	Bisection(Y, a,b, xnew, xlast, F, Parms, XTOL, FTOL, iter);

	a = MIN(xnew,xlast);
	b = MAX(xnew,xlast);

	itertot = iter;

	Fnew  = F(xnew,Parms);
	Flast = F(xlast,Parms);

	dFdx = (Fnew-Flast)/(xnew-xlast);

	dx   = xnew-xlast;
	iter = 1;
	while(fabs(dx) > XTOL && fabs(Fnew-Y) > FTOL && iter <= ITERMAX){
		dx    = (Y-Fnew)/dFdx;
		xlast0 = xlast;
		xlast  = xnew;
		Flast  = Fnew;
		xnew  = xlast + dx;
		Fnew  = F(xnew,Parms);

		if(Fnew < Y  && xnew > a) a = xnew;
		else if(Fnew >= Y && xnew < b) b = xnew;

		if(fabs(Fnew-Y) > fabs(Flast-Y)){
			iter2 = 20;
			Bisection(Y, a,b, xnew, xlast, F, Parms, XTOL, FTOL, iter2);
			itertot += iter2;
			break;
		}

		dFdx = (Fnew-Flast)/(xnew-xlast);
		iter++;
	}

	itertot += iter;
	X = xnew;
	return (itertot <= ITERMAX);
}

void UncertainColors::Bisection(double Y, double a, double b,
       double &Xmid, double &Xlast,
       double F(double X, double Parms[]),double Parms[],
       double XTOL, double FTOL, int &MaxIter){

	int    iter;
	double Fa,Fxmid;

	Xmid  = (a+b)/2.;

	Fa    = F(a,Parms) - Y;
	Fxmid = F(Xmid,Parms) - Y;
	iter  = 0;
	while(iter < MaxIter && fabs(a-b) > XTOL && fabs(Fxmid) > FTOL){
		if(Fa*Fxmid < 0.){
			b     = Xmid;
			Xlast = a;
		}
		else{
			a     = Xmid;
			Fa    = Fxmid;
			Xlast = b;
		}

		Xmid = (a+b)/2;
		Fxmid = F(Xmid,Parms) - Y;
		iter++;
	}
	MaxIter = iter;
}

double Y02L(double Y0,double Parms[]){
	double Y03,ScriptL;
	Y03 = pow(fabs(Y0),1./3.);
	if(Y0 > 30)
		ScriptL = 5.9*(Y03 - 2./3. + 0.042*pow(fabs(Y0-30.),1./3.));
	else
		ScriptL = 5.9*(Y03 - 2./3. - 0.042*pow(fabs(Y0-30.),1./3.));

	return (ScriptL-14.3993)/sqrt(2.);
}

double Phi(double w, double parms[]){
	double X,Y,Z,x,y,K;
	double ap,bp,w3,wa,wb;
	ap = parms[0];
	bp = parms[1];

	w3 = w*w*w;
	wa = (w + ap)*(w + ap)*(w + ap);
	wb = (w + bp)*(w + bp)*(w + bp);

	X =  1.0626*w3 - 0.4121*wa + 0.2975*wb;
	Y =  0.3599*w3 + 0.6401*wa ;//+ 0.0000*wb;
	Z = -0.0001*w3 - 0.3690*wa + 1.4424*wb;

	if(X > 0. &&  Y > 0. && Z > 0.){
		x = X/(X+Y+Z);
		y = Y/(X+Y+Z);
	}
	else{
		x = y = 0.;
	}
	K = 4.4934*x*x + 4.3034*y*y - 4.276*x*y - 1.3744*x - 2.5643*y + 1.8103;

	return K*Y;
}

void UncertainColors::SetGamma(bool OnOff){
	WantGammaCorrection = OnOff;
}

void UncertainColors::SetConvergeOnWhite(bool OnOff){
	WantConvergeOnWhite = OnOff;
}

void UncertainColors::InitializeLjg(){
	double xr,yr,zr,xg,yg,zg,xb,yb,zb,xw,yw,zw;
	double ar,ag,ab;
	double A[3][3],B[3][3];

	xr = 0.640;
	yr = 0.330;
	xg = 0.300;
	yg = 0.600;
	xb = 0.150;
	yb = 0.060;
	xw = 0.312713;  //www.neuro.sfc.keio.ac.jp/~aly/polygon/info/color-space-faq
	yw = 0.329016;

	zr=1.0-(xr+yr);
	zg=1.0-(xg+yg);
	zb=1.0-(xb+yb);
	zw=1.0-(xw+yw);

	//Color Space Conversions, Ford and Roberts, p. 13
	A[0][0] = xr;    A[0][1] = xg;    A[0][2] = xb;
	A[1][0] = yr;    A[1][1] = yg;    A[1][2] = yb;
	A[2][0] = zr;    A[2][1] = zg;    A[2][2] = zb;

	if(!Invert3by3(A,B)){
      //trap inverse error here
	}

	//assumes relative luminance = 1
	ar = B[0][0]*xw/yw + B[0][1]*1 + B[0][2]*zw/yw;
	ag = B[1][0]*xw/yw + B[1][1]*1 + B[1][2]*zw/yw;
	ab = B[2][0]*xw/yw + B[2][1]*1 + B[2][2]*zw/yw;

	RGB2XYZMat[0][0] = ar*xr;  RGB2XYZMat[0][1] = ag*xg; RGB2XYZMat[0][2] = ab*xb;
	RGB2XYZMat[1][0] = ar*yr;  RGB2XYZMat[1][1] = ag*yg; RGB2XYZMat[1][2] = ab*yb;
	RGB2XYZMat[2][0] = ar*zr;  RGB2XYZMat[2][1] = ag*zg; RGB2XYZMat[2][2] = ab*zb;

	if(!Invert3by3(RGB2XYZMat, XYZ2RGBMat)){
	//trap inverse error here
	}

	//A Guided Tour of Color Space, Poynton, p. 11
	XYZ2RGBMat[0][0] =  3.240479; XYZ2RGBMat[0][1] = -1.537150; XYZ2RGBMat[0][2] = -0.498535;
	XYZ2RGBMat[1][0] = -0.969256; XYZ2RGBMat[1][1] =  1.875992; XYZ2RGBMat[1][2] =  0.041556;
	XYZ2RGBMat[2][0] =  0.055648; XYZ2RGBMat[2][1] = -0.204043; XYZ2RGBMat[2][2] =  1.057311;

	if(!Invert3by3(XYZ2RGBMat, RGB2XYZMat)){
		//inverse error, use Poynton's values
		RGB2XYZMat[0][0] = 0.412453; RGB2XYZMat[0][1] = 0.357580; RGB2XYZMat[0][2] = 0.180423;
		RGB2XYZMat[1][0] = 0.212671; RGB2XYZMat[1][1] = 0.715160; RGB2XYZMat[1][2] = 0.072169;
		RGB2XYZMat[2][0] = 0.019334; RGB2XYZMat[2][1] = 0.119193; RGB2XYZMat[2][2] = 0.950227;
	}
}

bool UncertainColors::Invert3by3(double A[3][3],double B[3][3]){
	double det;
	det =   A[0][0]*A[1][1]*A[2][2]
			+ A[0][1]*A[1][2]*A[2][0]
			+ A[1][0]*A[2][1]*A[0][2]
			- A[2][0]*A[1][1]*A[0][2]
			- A[1][0]*A[0][1]*A[2][2]
			- A[2][1]*A[1][2]*A[0][0];

	if(fabs(det) < 1.e-10) return false;

	B[0][0] =  (A[1][1]*A[2][2] - A[2][1]*A[1][2])/det;
	B[0][1] = -(A[0][1]*A[2][2] - A[2][1]*A[0][2])/det;
	B[0][2] =  (A[0][1]*A[1][2] - A[1][1]*A[0][2])/det;

	B[1][0] = -(A[1][0]*A[2][2] - A[2][0]*A[1][2])/det;
	B[1][1] =  (A[0][0]*A[2][2] - A[2][0]*A[0][2])/det;
	B[1][2] = -(A[0][0]*A[1][2] - A[1][0]*A[0][2])/det;

	B[2][0] =  (A[1][0]*A[2][1] - A[2][0]*A[1][1])/det;
	B[2][1] = -(A[0][0]*A[2][1] - A[2][0]*A[0][1])/det;
	B[2][2] =  (A[0][0]*A[1][1] - A[1][0]*A[0][1])/det;

	return true;
}
//End UncertainColors Code

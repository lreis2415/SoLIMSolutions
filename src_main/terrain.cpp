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
//Written by Xun Shi
//created on Aug. 8, 2001
//last modified: Aug. 13, 2001

#include "StdAfx.h"
#include "SoLIM.h"
#include "terrain.h"
#include "ThreeDR.h"


//flow accumulation buffer, used in MFD


// eight neighborhood ,used in MFD
int ArrDir8X[8] = {1, 1, 1, 0, -1, -1, -1, 0};  
int ArrDir8Y[8] = {1, 0, -1, -1, -1, 0, 1, 1};
double MAX_SINGLE = 999999;



/////////////////////////////////////////////////////////
// Implementations of the functions listed in terrain.h
/////////////////////////////////////////////////////////

/****************************
	Major Functions

	Content:
	
	aspect
	brdNrwRidge
	catchment
	curvature
	dividelines
	fillDEM
	flowAccumulation
	flowDirection
	fuzzyAspects
	gradientCir 
	headwater
	planformCurvature
	profileCurvature
	ridgelines1 (Pueker and Douglas (1975) algorithm)
	ridgelines2 (O'Callaghan and Mark (1984) algorithm)
	ridgelines3 (Skidmore (1991) algorithm)
	ridgelineShi
    shoulder
	streamlineOCallaghan
	streamlinePeuker
	streamlineSkidmore
	terrainPatition
	wetnessIndexMultiDirection
	wetnessIndexSingleDirection

 ****************************/
//Result is in degree (N=0; E=90; S=180; W=270)
int aspectCirEvans(float* DEMBuf, float* outBuf, int nrow, int ncol, float cellsize, float neighborhood, float nodata){
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;

	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;
	double para[6];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] num of sqrt(2)cellsize between the center of the current cell and and bnd of the neighborhood
					//[2] weight1 in the cardinal direction
					//[3] weight2 in the cardinal direction
					//[4] weight1 in the diaganal direction
					//[5] weight2 in the diaganal direction
	bilinearWgtCir(neighborhood, cellsize, nrow, ncol, para);
	double result[9];
	for(i=0; i<numPixels; i++){
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[1], para[2], para[3], para[4], para[5], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p = ((1.41421356*result[3]+2*result[4]+1.41421356*result[5])-(1.41421356*result[1]+2*result[8]+1.41421356*result[7]))/(neighborhood*4);//neighborhood/2*8
			double q = ((1.41421356*result[1]+2*result[2]+1.41421356*result[3])-(1.41421356*result[5]+2*result[6]+1.41421356*result[7]))/(neighborhood*4);
			if(fabs(p-0)<VERYSMALL) outBuf[i] = -1; //flat
			else outBuf[i] = 180-360*atan(q/p)/6.2831853+90*(p/fabs(p)); //in degree
		}
	}
	return 0;
}

//Result is in degree (N=0; E=90; S=180; W=270)
int aspectSqrEvans(float* DEMBuf, float* outBuf, int nrow, int ncol, float cellsize, float neighborhood, float nodata){
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;

	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;

	double para[3];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] weight1 in the cardinal direction
					//[2] weight2 in the cardinal direction
	bilinearWgtSqr(neighborhood, cellsize, nrow, ncol, para);

	double result[9];
	for(i=0; i<numPixels; i++)
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[0], para[1], para[2], para[1], para[2], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p = ((result[3]+result[4]+result[5])-(result[1]+result[8]+result[7]))/(neighborhood*3);//neighborhood/2*6
			double q = ((result[1]+result[2]+result[3])-(result[5]+result[6]+result[7]))/(neighborhood*3);//neighborhood/2*6
			if(fabs(p-0)<VERYSMALL) outBuf[i] = -1; //flat
			else outBuf[i] = 180-360*atan(q/p)/6.2831853+90*(p/fabs(p)); //in degree
		}
	return 0;
}

int aspectHorn(float* DEMBuf,
				 float* outBuf,
				 int nrow, 
				 int ncol, 
				 float cellsize, 
				 float neighborhood, 
				 float nodata){
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;

	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;

	double para[3];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] weight1 in the cardinal direction
					//[2] weight2 in the cardinal direction
	bilinearWgtSqr(neighborhood, cellsize, nrow, ncol, para);

	double result[9];
	for(i=0; i<numPixels; i++)
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[0], para[1], para[2], para[1], para[2], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p = ((result[3]+result[4]*2+result[5])-(result[1]+result[8]*2+result[7]))/(neighborhood*4);//neighborhood/2*8
			double q = ((result[1]+result[2]*2+result[3])-(result[7]+result[6]*2+result[5]))/(neighborhood*4);//neighborhood/2*8
			if(fabs(p-0)<VERYSMALL) outBuf[i] = -1; //flat
			else outBuf[i] = 180-360*atan(q/p)/6.2831853+90*(p/fabs(p)); //in degree
		}
	return 0;
}

int aspectCirShi(float* DEMBuf,
				 float* outBuf,
				 int nrow, 
				 int ncol, 
				 float cellsize, 
				 float neighborhood, 
				 float nodata){
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;
	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;

	double para[6];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] num of sqrt(2)cellsize between the center of the current cell and and bnd of the neighborhood
					//[2] weight1 in the cardinal direction
					//[3] weight2 in the cardinal direction
					//[4] weight1 in the diaganal direction
					//[5] weight2 in the diaganal direction
	bilinearWgtCir(neighborhood, cellsize, nrow, ncol, para);

	double result[9];
	for(i=0; i<numPixels; i++){
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[1], para[2], para[3], para[4], para[5], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p1 = (result[4]-result[8])/neighborhood;//neighborhood/2*2
			double q1 = (result[2]-result[6])/neighborhood;//neighborhood/2*2
			double p2 = (result[3]-result[7])/neighborhood;//neighborhood/2*2
			double q2 = (result[1]-result[5])/neighborhood;//neighborhood/2*2
			double a1 = -1;
			double a2 = -1;
			if(fabs(p1-0)>VERYSMALL) a1 = 180-360*atan(q1/p1)/6.2831853+90*(p1/fabs(p1)); //in degree
			if(fabs(p2-0)>VERYSMALL) a2 = 180-360*atan(q2/p2)/6.2831853+90*(p2/fabs(p2)); //in degree
			if(a1<0 && a2<0)
				outBuf[i] = -1;
			else if(a1<0)
				outBuf[i] = a2;
			else if(a2<0)
				outBuf[i] = a1;
			else
				outBuf[i] = (a1+a2)/2;
		}
	}
	return 0;
}

int aspectSqrShi(float* DEMBuf,
				 float* outBuf,
				 int nrow, 
				 int ncol, 
				 float cellsize, 
				 float neighborhood, 
				 float nodata){
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;

	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;

	double para[3];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] weight1 in the cardinal direction
					//[2] weight2 in the cardinal direction
	bilinearWgtSqr(neighborhood, cellsize, nrow, ncol, para);

	double result[9];
	for(i=0; i<numPixels; i++)
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[0], para[1], para[2], para[1], para[2], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p1 = (result[4]-result[8])/neighborhood;//neighborhood/2*2
			double q1 = (result[2]-result[6])/neighborhood;//neighborhood/2*2
			double p2 = (result[3]-result[7])/(neighborhood*1.41421356);//neighborhood/2*2
			double q2 = (result[1]-result[5])/(neighborhood*1.41421356);//neighborhood/2*2
			double a1 = -1;
			double a2 = -1;
			if(fabs(p1-0)>VERYSMALL) a1 = 180-360*atan(q1/p1)/6.2831853+90*(p1/fabs(p1)); //in degree
			if(fabs(p2-0)>VERYSMALL) a2 = 180-360*atan(q2/p2)/6.2831853+90*(p2/fabs(p2)); //in degree
			if(a1<0 && a2<0)
				outBuf[i] = -1;
			else if(a1<0)
				outBuf[i] = a2;
			else if(a2<0)
				outBuf[i] = a1;
			else
				outBuf[i] = (a1+a2)/2;
		}
	return 0;
}

int aspectZT(float* DEMBuf,
				 float* outBuf,
				 int nrow, 
				 int ncol, 
				 float cellsize, 
				 float neighborhood, 
				 float nodata){
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;

	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;

	double para[3];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] weight1 in the cardinal direction
					//[2] weight2 in the cardinal direction
	bilinearWgtSqr(neighborhood, cellsize, nrow, ncol, para);

	double result[9];
	for(i=0; i<numPixels; i++)
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[0], para[1], para[2], para[1], para[2], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p = (result[4]-result[8])/neighborhood;//neighborhood/2*2
			double q = (result[2]-result[6])/neighborhood;//neighborhood/2*2
			if(fabs(p-0)<VERYSMALL) outBuf[i] = -1; //flat
			else outBuf[i] = 180-360*atan(q/p)/6.2831853+90*(p/fabs(p)); //in degree
		}
	return 0;
}

int back(float* terrainPartitionBuf, 
		 float* slopeBuf, 
		 float* ridgeBuf, 
		 float* outBuf, 
		 float topPercent, 
		 int nrow, int ncol,
		 float nodata){
	if(terrainPartitionBuf == NULL) return 1;
	if(slopeBuf == NULL) return 2;
	if(ridgeBuf == NULL) return 3;
	if(outBuf == NULL) return 4;

	int numPixels = nrow*ncol;
	unsigned char* idBuf = new unsigned char[numPixels];
	for(int i = 0; i<numPixels; i++){
		if(fabs(terrainPartitionBuf[i]-nodata)<VERYSMALL || fabs(slopeBuf[i]-nodata)<VERYSMALL || fabs(ridgeBuf[i]-nodata)<VERYSMALL){
			idBuf[i] = 4;
			outBuf[i] = nodata;
		}
		else{
			if(terrainPartitionBuf[i] == 0)
				idBuf[i] = 4;
			else
				idBuf[i] = 0;
			outBuf[i] = 0;
		}
	}

	for(i = 0; i<numPixels; i++){
		if(idBuf[i] == 0){
			Patch<float>* thePatch = new Patch<float>(i, terrainPartitionBuf, idBuf, nrow, ncol, true);
			thePatch->findBorder(false);
			bool hasRidge = false;
			PatchPixel* temp = thePatch->first;
			while(temp!=NULL){
				if(temp->atBorder){
					if(ridgeBuf[temp->pos]>0){
						hasRidge = true;
						break;
					}
					if(assertN(temp->pos, nrow, ncol) && ridgeBuf[getN(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
					if(assertNE(temp->pos, nrow, ncol) && ridgeBuf[getNE(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
					if(assertE(temp->pos, nrow, ncol) && ridgeBuf[getE(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
					if(assertSE(temp->pos, nrow, ncol) && ridgeBuf[getSE(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
					if(assertS(temp->pos, nrow, ncol) && ridgeBuf[getS(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
					if(assertSW(temp->pos, nrow, ncol) && ridgeBuf[getSW(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
					if(assertW(temp->pos, nrow, ncol) && ridgeBuf[getW(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
					if(assertNW(temp->pos, nrow, ncol) && ridgeBuf[getNW(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
				}
				temp = temp->nextPixel;
			}
			if(!hasRidge){
				delete thePatch;
				continue;
			}

			int arraySize = thePatch->numPixels*topPercent/100;
			int* array = new int[arraySize];
			for(int k=0; k<arraySize; k++)
				array[k] = -1;
			temp = thePatch->first;
			while(temp!=NULL){
				float curV = slopeBuf[temp->pos];
				for(k=0; k<arraySize; k++){
					float candidateV = 0;
					if(array[k] != -1)
						candidateV = slopeBuf[array[k]];
					if(curV<candidateV)
						break;
				}
				if(k==0){
					temp = temp->nextPixel;
					continue;
				}
				k -= 1;
				for(int p=0; p<k; p++)
					array[p] = array[p+1];
				array[k]=temp->pos;
				temp = temp->nextPixel;
			}
			delete thePatch;
			for(k=0; k<arraySize; k++)
				if(array[k]!=-1)
					outBuf[array[k]] = 100;
		}
	}
	
	return 0;
}

int brdNrwRidge(float* ridgelineBuf,
				float label,
				float* gradientBuf, 
				float* outBuf,
				int nrow, 
				int ncol, 
				float cellsize, 
				float upperWidth, 
				float lowerWidth,
				float maxGrad,
				float minArea, 
				float nodata){
	if(ridgelineBuf == NULL) return 1;
	if(gradientBuf == NULL) return 1;
	if(outBuf == NULL) return 1;

	int areaPixels = minArea/(cellsize*cellsize);
	int i = 0;
	int j = 0;
	int numPixels = nrow*ncol;
	unsigned char* idBuf = new unsigned char[numPixels];
	unsigned char* flatArea = new unsigned char[numPixels];

	//Initialize new buffer
	for(i = 0; i < numPixels; i++){
		if(fabs(ridgelineBuf[i]-nodata)<VERYSMALL || fabs(gradientBuf[i]-nodata)<VERYSMALL){
			outBuf[i] = nodata;
			flatArea[i] = 0;
			idBuf[i] = 2;
		}
		else{
			outBuf[i] = 0;
			if(gradientBuf[i]<=maxGrad){
				flatArea[i] = 1;
				idBuf[i] = 0;
			}
			else{
				flatArea[i] = 0;
				idBuf[i] = 4;
			}
		}
	}

	for(i = 0; i < numPixels; i++){
		if(idBuf[i]==0){
			Patch<unsigned char>* thePatch = new Patch<unsigned char>(i, flatArea, idBuf, nrow, ncol, true);

			PatchPixel* tempPixel = thePatch->first;
			while(tempPixel!=NULL){
				if(ridgelineBuf[tempPixel->pos]==label){

					//If the area is smaller than the area threshold, skip the patch.
					if(thePatch->numPixels<=areaPixels){
						outBuf[tempPixel->pos] = label; //narrow ridge
						tempPixel= tempPixel->nextPixel;
						continue;
					}
						
					int uh = 0; //length of the upper half
					int lh = 0; //length of the lower half
					int weight = 0;

					//(1) N-S
					uh = 0;
					lh = 0;
					while(tempPixel->pos-uh*ncol>(ncol-1)) //N
						if(flatArea[tempPixel->pos-(uh+1)*ncol]==1)
							uh++;
						else
							break;
					while((numPixels-(tempPixel->pos+lh*ncol))>ncol) //S
						if(flatArea[tempPixel->pos+(lh+1)*ncol]==1)
							lh++;
						else
							break;
					if((uh+lh+1)*cellsize>=lowerWidth && (uh+lh+1)*cellsize<=upperWidth) 
						weight++;

					//(2) W-E
					uh = 0;
					lh = 0;
					while(tempPixel->pos-uh%ncol != 0) //W
						if(flatArea[tempPixel->pos-(uh+1)]==1)
							uh++;
						else
							break;
					while((tempPixel->pos+lh+1)%ncol!=0) //E
						if(flatArea[tempPixel->pos+(lh+1)]==1)
							lh++;
						else
							break;
					if((uh+lh+1)*cellsize>=lowerWidth && (uh+lh+1)*cellsize<=upperWidth) 
						weight++;

					//(3) NE-SW
					uh = 0;
					lh = 0;
					while(tempPixel->pos-uh*(ncol+1)>(ncol-1)&&(tempPixel->pos-uh*(ncol-1)+1)%ncol!=0) //NE
						if(flatArea[tempPixel->pos-(uh+1)*(ncol-1)]==1)
							uh++;
					else
						break;
					while((numPixels-(tempPixel->pos+lh*(ncol-1)))>ncol&&(tempPixel->pos+lh*(ncol-1))%ncol!=0) //SW
						if(flatArea[tempPixel->pos+(lh+1)*(ncol-1)]==1)
							lh++;
						else
							break;
					if((uh+lh+1)*sqrt(2.0)*cellsize>=lowerWidth && (uh+lh+1)*sqrt(2.0)*cellsize<=upperWidth) 
						weight++;

					//(4) NW-SE
					uh = 0;
					lh = 0;
					while(tempPixel->pos-uh*(ncol+1)>(ncol-1)&&tempPixel->pos-uh*(ncol+1)%ncol!=0) //NW
						if(flatArea[tempPixel->pos-(uh+1)*(ncol+1)]==1)
							uh++;
						else
							break;
					while(numPixels-(tempPixel->pos+lh*(ncol+1))>ncol&&(tempPixel->pos+lh*(ncol+1)+1)%ncol!=0) //SE
						if(flatArea[tempPixel->pos+(lh+1)*(ncol+1)]==1)
							lh++;
						else
							break;
					if((uh+lh+1)*sqrt(2.0)*cellsize>=lowerWidth && (uh+lh+1)*sqrt(2.0)*cellsize<=upperWidth) 
						weight++;

					if(weight>2)
						outBuf[tempPixel->pos] = label*2; //broad ridge
					else
						outBuf[tempPixel->pos] = label; //narrow ridge
				}//end	"if(ridgelineBuf[tempPixel->pos]==ridgeLabel)"
				tempPixel= tempPixel->nextPixel;
			} //end while(tempPixel!=NULL)
			delete thePatch;
		} //end if (idBuf == 0)
	} //end for ()
	
	delete [] idBuf;
	delete [] flatArea;

	return 0;
}

int catchment(float* flowDir,	//flow direction data
			  float* outBuf,	//result
			  int nrow, 
			  int ncol, 
			  float nodata){
	if(flowDir == NULL) return 1;
	if(outBuf == NULL) return 3;

	int i=0;
	int numPixels = nrow*ncol;
	for(i=0; i<numPixels; i++)
		if(fabs(flowDir[i]-nodata)<VERYSMALL)
			outBuf[i] = nodata;
		else
			outBuf[i] = 0;
	
	//find out the lowest point of each catchment (dir code = 0)
	int cachCode = 1;
	int numOfCach = 0;
	for(i=0; i<numPixels; i++){
		if((int)flowDir[i]==0){
			outBuf[i] = cachCode;
			cachCode += 10;
			numOfCach++;
		}
	}

	for(i=0; i<numPixels; i++){
		int curPos = i;
		List<IntNode>* flowPath = new List<IntNode>;
		while((int)outBuf[curPos]==0){
			flowPath->addTail(new IntNode(curPos));
			int intCode = flowDir[curPos];

			//find out the next position
			switch(intCode){
				case 1:
					curPos = curPos-ncol+1;
					break;
				case 2:
					curPos = curPos+1;
					break;
				case 4:
					curPos = curPos+ncol+1;
					break;
				case 8:
					curPos = curPos+ncol;
					break;
				case 16:
					curPos = curPos+ncol-1;
					break;
				case 32:
					curPos = curPos-1;
					break;
				case 64:
					curPos = curPos-ncol-1;
					break;
				case 128:
					curPos = curPos-ncol;
					break;
				default:
					return 4;
			}
		} //end while: end finding a path

		//assign code to the path
		IntNode* temp = flowPath->first;
		while(temp!=NULL){
			outBuf[temp->pos] = outBuf[curPos];
			temp = temp->next;
		}
		delete flowPath;
	}
	return 0;
}

int curvatureCirEvans(float* DEMBuf, float* outBuf, int nrow, int ncol,	float cellsize,	int neighborhood, int type,	float nodata){
	//Derive various types of curvatures
	//The definitions of the curvatures are based on Shary 2002
	//Use Evans' method, circular neighborhood
	//modified: Dec. 28, 2005

	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;

	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;

	double para[6];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] num of sqrt(2)cellsize between the center of the current cell and and bnd of the neighborhood
					//[2] weight1 in the cardinal direction
					//[3] weight2 in the cardinal direction
					//[4] weight1 in the diaganal direction
					//[5] weight2 in the diaganal direction
	bilinearWgtCir(neighborhood, cellsize, nrow, ncol, para);

	double sqrDelta = neighborhood*neighborhood/4; //(neighborhood/2)*(neighborhood/2)
	double result[9];
	// 1 2 3
	// 8 0 4
	// 7 6 5
	for(i=0; i<numPixels; i++)
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[1], para[2], para[3], para[4], para[5], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p = ((1.41421356*result[3]+2*result[4]+1.41421356*result[5])-(1.41421356*result[1]+2*result[8]+1.41421356*result[7]))/(neighborhood*4);//neighborhood/2*8
			double q = ((1.41421356*result[1]+2*result[2]+1.41421356*result[3])-(1.41421356*result[5]+2*result[6]+1.41421356*result[7]))/(neighborhood*4);//neighborhood/2*8
			double r = (result[1]+result[3]+3*result[4]+result[5]+result[7]+3*result[8]-8*result[0]-result[2]-result[6])/(sqrDelta*4);
			double s = (result[3]+result[7]-result[1]-result[5])/(sqrDelta*2);
			double t = (result[1]+3*result[2]+result[3]+result[5]+3*result[6]+result[7]-8*result[0]-result[4]-result[8])/(sqrDelta*4);
			outBuf[i] = curvatures(p, q, r, s, t, type, nodata);
			if(fabs(outBuf[i]-nodata)>VERYSMALL)
				outBuf[i] *= 100;
		}
	return 0;
}

int curvatureSqrEvans(float* DEMBuf, float* outBuf,	int nrow, int ncol,	float cellsize,	int neighborhood, int type,	float nodata){
	//Derive various types of curvatures
	//The definitions of the curvatures are based on Shary 2002
	//Use Evans' method 
	//Xun Shi, Oct. 29, 2003
	//modified: Dec. 28, 2005

	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;

	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;

	double para[3];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] weight1 in the cardinal direction
					//[2] weight2 in the cardinal direction
	bilinearWgtSqr(neighborhood, cellsize, nrow, ncol, para);

	double sqrDelta = neighborhood*neighborhood/4; //(neighborhood/2)*(neighborhood/2)
	double result[9];
	for(i=0; i<numPixels; i++)
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[0], para[1], para[2], para[1], para[2], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p = (result[3]+result[4]+result[5]-result[1]-result[8]-result[7])/(neighborhood*3);
			double q = (result[1]+result[2]+result[3]-result[5]-result[6]-result[7])/(neighborhood*3);
			double r = (result[1]+result[3]+result[4]+result[5]+result[7]+result[8]-(result[0]+result[2]+result[6])*2)/(sqrDelta*3);
			double s = (result[3]+result[7]-result[1]-result[5])/(sqrDelta*4);
			double t = (result[1]+result[2]+result[3]+result[5]+result[6]+result[7]-(result[0]+result[4]+result[8])*2)/(sqrDelta*3);
			outBuf[i] = curvatures(p, q, r, s, t, type, nodata);
			if(fabs(outBuf[i]-nodata)>VERYSMALL)
				outBuf[i] *= 100;
		}
	return 0;
}

int curvatureCirZT(float* DEMBuf, float* outBuf, int nrow, int ncol, float cellsize, int neighborhood, int type, float nodata){
	//Derive various types of curvatures
	//The definitions of the curvatures are based on Shary 2002
	//Use ZT method, circular neighborhood
	//modified: Dec. 28, 2005

	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;

	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;

	double para[6];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] num of sqrt(2)cellsize between the center of the current cell and and bnd of the neighborhood
					//[2] weight1 in the cardinal direction
					//[3] weight2 in the cardinal direction
					//[4] weight1 in the diaganal direction
					//[5] weight2 in the diaganal direction
	bilinearWgtCir(neighborhood, cellsize, nrow, ncol, para);

	double sqrDelta = neighborhood*neighborhood/4; //(neighborhood/2)*(neighborhood/2)
	double result[9];
	// 1 2 3
	// 8 0 4
	// 7 6 5
	for(i=0; i<numPixels; i++)
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[1], para[2], para[3], para[4], para[5], result, nodata)==0, nodata){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p = (result[4]-result[8])/neighborhood;
			double q = (result[2]-result[6])/neighborhood;
			double r = (result[4]+result[8]-result[0]*2)/sqrDelta;
			double s = (result[3]+result[7]-result[1]-result[5])/(sqrDelta*2);
			double t = (result[2]+result[6]-result[0]*2)/sqrDelta;
			outBuf[i] = curvatures(p, q, r, s, t, type, nodata);
			if(fabs(outBuf[i]-nodata)>VERYSMALL)
				outBuf[i] *= 100;
		}
	return 0;
}

int curvatureSqrZT(float* DEMBuf, float* outBuf, int nrow, int ncol, float cellsize, int neighborhood, int type, float nodata){
	//Derive various types of curvatures
	//The definitions of the curvatures are based on Shary 2002
	//Use ZT method 
	//modified: Feb. 8, 2004

	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;

	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;

	double para[3];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] weight1 in the cardinal direction
					//[2] weight2 in the cardinal direction
	bilinearWgtSqr(neighborhood, cellsize, nrow, ncol, para);

	double sqrDelta = neighborhood*neighborhood/4; //(neighborhood/2)*(neighborhood/2)
	double result[9];
	// 1 2 3
	// 8 0 4
	// 7 6 5
	for(i=0; i<numPixels; i++)
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[0], para[1], para[2], para[1], para[2], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p = (result[4]-result[8])/neighborhood;
			double q = (result[2]-result[6])/neighborhood;
			double r = (result[4]+result[8]-result[0]*2)/sqrDelta;
			double s = (result[3]+result[7]-result[1]-result[5])/(sqrDelta*4);
			double t = (result[2]+result[6]-result[0]*2)/sqrDelta;
			outBuf[i] = curvatures(p, q, r, s, t, type, nodata);
			if(fabs(outBuf[i]-nodata)>VERYSMALL)
				outBuf[i] *= 100;
		}
	return 0;
}

//used by the curvauter functions above
float curvatures(double p, double q, double r, double s, double t, int type, float nodata){
	double H = 0;
	double M = 0;
	double E = 0;
	double sqrP = p*p;
	double sqrQ = q*q;
	double pqs = p*q*s;
	switch(type){
		case 5: //Mean curvature H (1/m)
			return -((1+sqrQ)*r-2*pqs+(1+sqrP)*t)/(2*pow(1+sqrP+sqrQ,3.0/2.0));
		case 6: //Unsphericity M (1/m)
			return sqrt(pow((r*sqrt((1+sqrQ)/(1+sqrP))-t*sqrt((1+sqrP)/(1+sqrQ))),2.0)/(1+sqrP+sqrQ) + 
			pow(p*q*r*sqrt((1+sqrQ)/(1+sqrP))-2*s*sqrt((1+sqrQ)*(1+sqrP))+p*q*t*sqrt((1+sqrP)/(1+sqrQ)),2.0))/(2*pow(1+sqrP+sqrQ,1.5));
		case 7: //Difference curvature E (1/m)
			if(sqrP+sqrQ<VERYSMALL*VERYSMALL)
				return nodata;
			else
				return (sqrQ*r-2*pqs+sqrP*t)/((sqrP+sqrQ)*sqrt(1+sqrP+sqrQ)) - 
				(r*(1+sqrQ)-2*pqs+t*(1+sqrP))/(2*pow((1+sqrP+sqrQ),1.5));
		case 8: //Horizontal curvature kh 
			if(sqrP+sqrQ<VERYSMALL*VERYSMALL)
				return nodata;
			else
				return (-1)*(sqrQ*r-2*pqs+sqrP*t)/((sqrP+sqrQ)*sqrt(1+sqrP+sqrQ));
		case 9: //Plan curvature kp
			if(sqrP+sqrQ<VERYSMALL*VERYSMALL)
				return 0;
			else
				//return (-1)*(sqrQ*r-2*pqs+sqrP*t)/(pow((sqrP+sqrQ),1.5)); //Shary 
				return (sqrQ*r-2*pqs+sqrP*t)/(sqrP+sqrQ); //Burrough
		case 10: //Profile curvature kv
			if(sqrP+sqrQ<VERYSMALL*VERYSMALL)
				return 0;
			else
				//return (sqrP*r+2*pqs+sqrQ*t)/((sqrP+sqrQ)*pow(1+sqrP+sqrQ,1.5)); //Shary
				return (sqrP*r+2*pqs+sqrQ*t)/(sqrP+sqrQ); //Burrough
		case 11: //roter
			if(sqrP+sqrQ<VERYSMALL*VERYSMALL)
				return nodata;
			else
				return (s*(sqrP-sqrQ)-p*q*(r-t))/(pow(sqrP+sqrQ,1.5));
		case 12: //Horizontal excess curvature khe
			if(sqrP+sqrQ<VERYSMALL*VERYSMALL)
				return nodata;
			else{
				M = sqrt(pow((r*sqrt((1+sqrQ)/(1+sqrP))-t*sqrt((1+sqrP)/(1+sqrQ))),2.0)/(1+sqrP+sqrQ) + 
					pow(p*q*r*sqrt((1+sqrQ)/(1+sqrP))-2*s*sqrt((1+sqrQ)*(1+sqrP))+p*q*t*sqrt((1+sqrP)/(1+sqrQ)),2.0))/(2*pow(1+sqrP+sqrQ,1.5));
				E = (sqrQ*r-2*pqs+sqrP*t)/((sqrP+sqrQ)*sqrt(1+sqrP+sqrQ)) - (r*(1+sqrQ)-2*pqs+t*(1+sqrP))/(2*pow((1+sqrP+sqrQ),1.5));
				return M-E;
			}
		case 13: //Vertical excess curvature kve
			if(sqrP+sqrQ<VERYSMALL*VERYSMALL)
				return nodata;
			else{
				M = sqrt(pow((r*sqrt((1+sqrQ)/(1+sqrP))-t*sqrt((1+sqrP)/(1+sqrQ))),2.0)/(1+sqrP+sqrQ) + 
					pow(p*q*r*sqrt((1+sqrQ)/(1+sqrP))-2*s*sqrt((1+sqrQ)*(1+sqrP))+p*q*t*sqrt((1+sqrP)/(1+sqrQ)),2.0))/(2*pow(1+sqrP+sqrQ,1.5));
				E = (sqrQ*r-2*pqs+sqrP*t)/((sqrP+sqrQ)*sqrt(1+sqrP+sqrQ)) - (r*(1+sqrQ)-2*pqs+t*(1+sqrP))/(2*pow((1+sqrP+sqrQ),1.5));
				return M+E;
			}
		case 14: //Minimal curvature kmin
			H = -((1+sqrQ)*r-2*pqs+(1+sqrP)*t)/(2*pow(1+sqrP+sqrQ,3.0/2.0));
			M = sqrt(pow((r*sqrt((1+sqrQ)/(1+sqrP))-t*sqrt((1+sqrP)/(1+sqrQ))),2.0)/(1+sqrP+sqrQ) + pow(p*q*r*sqrt((1+sqrQ)/(1+sqrP))-2*s*sqrt((1+sqrQ)*(1+sqrP))+p*q*t*sqrt((1+sqrP)/(1+sqrQ)),2.0))/(2*pow(1+sqrP+sqrQ,1.5));
			return H-M;
		case 15: //Maximal curvature kmax
			H = -((1+sqrQ)*r-2*pqs+(1+sqrP)*t)/(2*pow(1+sqrP+sqrQ,3.0/2.0));
			M = sqrt(pow((r*sqrt((1+sqrQ)/(1+sqrP))-t*sqrt((1+sqrP)/(1+sqrQ))),2.0)/(1+sqrP+sqrQ) + pow(p*q*r*sqrt((1+sqrQ)/(1+sqrP))-2*s*sqrt((1+sqrQ)*(1+sqrP))+p*q*t*sqrt((1+sqrP)/(1+sqrQ)),2.0))/(2*pow(1+sqrP+sqrQ,1.5));
			return H+M;
		case 16: //Total Gaussian curvature K (1/m2)
			return (r*t - s*s)/pow(1+sqrP+sqrQ,2.0);
		case 17: //Total ring curvature KR (1/m2)
			if(sqrP+sqrQ<VERYSMALL*VERYSMALL)
				return nodata;
			else
				return pow(s*(sqrP-sqrQ)-p*q*(r-t),2.0)/(pow(sqrP+sqrQ,2.0)*pow(1+sqrP+sqrQ,2.0));
		case 18: //Total accumulation curvature KA (1/m2)
			if(sqrP+sqrQ<VERYSMALL*VERYSMALL)
				return nodata;
			else{
				H = -((1+sqrQ)*r-2*pqs+(1+sqrP)*t)/(2*pow(1+sqrP+sqrQ,3.0/2.0));
				E = (sqrQ*r-2*pqs+sqrP*t)/((sqrP+sqrQ)*sqrt(1+sqrP+sqrQ)) - (r*(1+sqrQ)-2*pqs+t*(1+sqrP))/(2*pow((1+sqrP+sqrQ),1.5));
				return H*H - E*E;
			}
		case 19: //Curvature (See ArcDoc on Curvature)
			if(sqrP+sqrQ<VERYSMALL*VERYSMALL)
				return nodata;
			else{
				return (-1)*(r+t);
			}
		case 20: //Tangent curvature (plan cur * sin(slope), from Amanda, Dec 28, 2005)
			if(sqrP+sqrQ<VERYSMALL*VERYSMALL)
				return nodata;
			else{
				return (sqrQ*r-2*pqs+sqrP*t)/sqrt(sqrP+sqrQ)/sqrt(1+sqrP+sqrQ);
			}
		default:
			return nodata;
	}
}

int dividelines(float* DEMBuf,
				float* terrainPartitionBuf,
				float* outBuf,
				int nrow,
				int ncol,
				int label,
				float nodata){
	if(DEMBuf == NULL) return 1;
	if(terrainPartitionBuf == NULL) return 2;
	if(outBuf == NULL) return 3;

	int i = 0;
	int numPixels = nrow*ncol;

	//Building the ID buffer
	unsigned char* idBuf = new unsigned char[numPixels];
	if(idBuf == NULL) return 32; //No sufficient memory
	for(i=0; i<numPixels; i++){
		if(fabs(DEMBuf[i]-nodata)<VERYSMALL || fabs(terrainPartitionBuf[i]-nodata)<VERYSMALL){
			outBuf[i] = nodata;
			idBuf[i] = 4;
		}
		else{
			outBuf[i] = 0;	// 0: not on divideline
							// 1: not decided; 
							// 2: on divideline
			if(terrainPartitionBuf[i] == 0)
				idBuf[i] = 4; //should not be considered
			else
				idBuf[i] = 0;
		}
	}

	//label the border pixels
	for(i=0; i<numPixels; i++){
		if(idBuf[i] == 0){
			Patch<float>* thePatch = new Patch<float>(i, terrainPartitionBuf, idBuf, nrow, ncol, false);
			thePatch->findBorder(false);

			PatchPixel* tempPixel = thePatch->first;
			while(tempPixel!=NULL){
				if(tempPixel->atBorder){
					//NorthWest
					if(assertNW(tempPixel->pos, nrow, ncol)) 
						if(terrainPartitionBuf[tempPixel->pos-ncol-1] == 0){
							outBuf[tempPixel->pos] = 0;
							tempPixel = tempPixel->nextPixel;
							continue;
						}

					//North
					if(assertN(tempPixel->pos, nrow, ncol)) 
						if(terrainPartitionBuf[tempPixel->pos-ncol] == 0){
							outBuf[tempPixel->pos] = 0;
							tempPixel = tempPixel->nextPixel;
							continue;
						}

					//NorthEast
					if(assertNE(tempPixel->pos, nrow, ncol)) 
						if(terrainPartitionBuf[tempPixel->pos-ncol+1] == 0){
							outBuf[tempPixel->pos] = 0;
							tempPixel = tempPixel->nextPixel;
							continue;
						}

					//West
					if(assertW(tempPixel->pos, nrow, ncol)) 
						if(terrainPartitionBuf[tempPixel->pos-1] == 0){
							outBuf[tempPixel->pos] = 0;
							tempPixel = tempPixel->nextPixel;
							continue;
						}

					//East
					if(assertE(tempPixel->pos, nrow, ncol)) 
						if(terrainPartitionBuf[tempPixel->pos+1] == 0){
							outBuf[tempPixel->pos] = 0;
							tempPixel = tempPixel->nextPixel;
							continue;
						}

					//SouthWest
					if(assertSW(tempPixel->pos, nrow, ncol)) 
						if(terrainPartitionBuf[tempPixel->pos+ncol-1] == 0){
							outBuf[tempPixel->pos] = 0;
							tempPixel = tempPixel->nextPixel;
							continue;
						}
	
					//South
					if(assertS(tempPixel->pos, nrow, ncol)) 
						if(terrainPartitionBuf[tempPixel->pos+ncol] == 0){
							outBuf[tempPixel->pos] = 0;
							tempPixel = tempPixel->nextPixel;
							continue;
						}

					//SouthEast
					if(assertSE(tempPixel->pos, nrow, ncol)) 
						if(terrainPartitionBuf[tempPixel->pos+ncol+1] == 0){
							outBuf[tempPixel->pos] = 0;
							tempPixel = tempPixel->nextPixel;
							continue;
						}

					//if not adjacent to streams
					outBuf[i] = 1;
				}
				tempPixel = tempPixel->nextPixel;
			}
			delete thePatch;
		}
	}

	//reset the idBuf
	for(i=0; i<numPixels; i++){
		if(outBuf[i] == 0)
			idBuf[i] = 4; //should not be considered
		else
			idBuf[i] = 0;
	}

	//thinning the ridgeline
	for(i=0; i<numPixels; i++){
		if(idBuf[i] == 0){
			Patch<float>* thePatch = new Patch<float>(i, outBuf, idBuf, nrow, ncol, true);
			thePatch->buildMatrix(3);

			List<IntNode>* theList = new List<IntNode>;

			//find the highest pixel as the start pixel
			PatchPixel* tempPixel = thePatch->first;
			PatchPixel* highestPixel = NULL;
			float highest = 0;
			while(tempPixel!=NULL){
				if(DEMBuf[tempPixel->pos]>highest){
					highestPixel = tempPixel;
					highest = DEMBuf[tempPixel->pos];
				}
				tempPixel = tempPixel->nextPixel;
			}

			theList->addTail(new IntNode(highestPixel->pos));
			decreaseMatrixValue(thePatch, highestPixel->pos);

			while(theList->first!=NULL){
				int curPos = theList->first->pos;
				int nextPos = -1;
				outBuf[curPos] = 2;

				while(haveNext(thePatch, curPos, 2)){
					nextPos = setNext(thePatch, DEMBuf, curPos, 2);
					decreaseMatrixValue(thePatch, nextPos);
					theList->addTail(new IntNode(nextPos));
				}
				while(haveNext(thePatch, curPos, 1)){
					nextPos = setNext(thePatch, DEMBuf, curPos, 1);
					decreaseMatrixValue(thePatch, nextPos);
					theList->addTail(new IntNode(nextPos));
				}					
				theList->removeHead();
			}
			delete thePatch;
			delete theList;
		}
	}
	delete [] idBuf;

	for(i = 0; i<numPixels; i++)
		if(outBuf[i]!=2)
			outBuf[i] = 0;
		else
			outBuf[i] = label;

	return 0;
}

int fillDEM(float* DEMBuf,	//Input DEM
			  float* cachBuf,	//empty buffer for holding catchment data
			  float* flowBuf,	//empty buffer for holding flow direction
			  float* outBuf,	//result
			  float threshold,
			  int nrow, 
			  int ncol, 
			  float nodata){
	if(DEMBuf == NULL) return 1;
	if(cachBuf == NULL) return 2;
	if(flowBuf == NULL) return 3;
	if(outBuf == NULL) return 4;
	if(threshold<0) return 5;

	int i = 0;
	int errorCode = 0;
	int numPixels = nrow*ncol;

	for(i=0; i<numPixels; i++){
		outBuf[i] = DEMBuf[i];
		cachBuf[i] = 0;
		flowBuf[i] = 255;
	}

	int numOfSinks = 1;
	while(numOfSinks > 0){
		numOfSinks = 0;
		errorCode = flowDirection(outBuf, flowBuf, nrow, ncol, nodata);
		if(errorCode != 0) return errorCode;
		errorCode = catchment(flowBuf, cachBuf, nrow, ncol, nodata);
		if(errorCode != 0) return errorCode;
		numOfSinks = fillSink(outBuf, cachBuf, outBuf, threshold, nrow, ncol, nodata);
	}
	return 0;
}

int flowAccumulation(float* flowDir,		//flow direction data 
					  float* outBuf,		//result
					  int nrow, 
					  int ncol, 
					  float unitArea, 
					  float nodata){
	if(flowDir == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(unitArea < 0) unitArea = 1;
	int numPixels = nrow*ncol;

	for(int i=0; i<numPixels; i++){
		if(fabs(flowDir[i]-nodata)<VERYSMALL)
			outBuf[i] = nodata;
		else
			outBuf[i]=unitArea;
	}

	for(i=0; i<numPixels; i++){
		int curPos = i;
		while(fabs(flowDir[i]-nodata)>VERYSMALL && flowDir[curPos]!=0){
			curPos = addArea(curPos, flowDir, outBuf, unitArea, nrow, ncol);
			if(curPos<0) return 4;
		}
	}
	return 0;
}


int flowDirection(float* DEMBuf,		//the elevation data
				   float* outBuf,	//result
				   int nrow, 
				   int ncol, 
				   float nodata){
	//0:	Center
	//1:	NE
	//2:	E
	//4:	SE
	//8:	S
	//16:	SW
	//32:	W
	//64:	NW
	//128:	N
	//
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		if(fabs(DEMBuf[i]-nodata)<VERYSMALL)
			outBuf[i] = nodata;
		else
			outBuf[i] = 255;				

	int flat = 0;
	int tempIndex = 0;

	//1. find the lowest dir
	for(i=0; i<numPixels; i++){
		if(fabs(outBuf[i]-nodata)<VERYSMALL) continue;
		float theLargestDiff = 0.0;
		float diff = 0.0;
		unsigned char theLowestCode = 0;
		int theLowestIndex = -1;
		tempIndex = -1;

		if(assertNW(i, nrow, ncol)){
			tempIndex = getNW(i, ncol);
			if(fabs(DEMBuf[tempIndex]-nodata)>VERYSMALL){
				diff = (DEMBuf[i]-DEMBuf[tempIndex])/sqrt(2.0);
				if(theLargestDiff<=diff){
					theLargestDiff=diff;
					theLowestIndex = tempIndex;
					theLowestCode = 64;
				}
			}
		}
		if(assertN(i, nrow, ncol)){ 
			tempIndex = getN(i, ncol);
			if(fabs(DEMBuf[tempIndex]-nodata)>VERYSMALL){
				diff = DEMBuf[i]-DEMBuf[tempIndex];
				if(theLargestDiff<=diff){
					theLargestDiff=diff;
					theLowestIndex = tempIndex;
					theLowestCode = 128;
				}
			}
		}
		if(assertNE(i, nrow, ncol)){ 
			tempIndex = getNE(i, ncol);
			if(fabs(DEMBuf[tempIndex]-nodata)>VERYSMALL){
				diff = (DEMBuf[i]-DEMBuf[tempIndex])/sqrt(2.0);
				if(theLargestDiff<=diff){
					theLargestDiff=diff;
					theLowestIndex = tempIndex;
					theLowestCode = 1;
				}
			}
		}
		if(assertW(i, nrow, ncol)){ 
			tempIndex = getW(i, ncol);
			if(fabs(DEMBuf[tempIndex]-nodata)>VERYSMALL){
				diff = DEMBuf[i]-DEMBuf[tempIndex];
				if(theLargestDiff<=diff){
					theLargestDiff=diff;
					theLowestIndex = tempIndex;
					theLowestCode = 32;
				}
			}
		}
		if(assertE(i, nrow, ncol)){ 
			tempIndex = getE(i, ncol);
			if(fabs(DEMBuf[tempIndex]-nodata)>VERYSMALL){
				diff = DEMBuf[i]-DEMBuf[tempIndex];
				if(theLargestDiff<=diff){
					theLargestDiff=diff;
					theLowestIndex = tempIndex;
					theLowestCode = 2;
				}
			}
		}
		if(assertSW(i, nrow, ncol)){ 
			tempIndex = getSW(i, ncol);
			if(fabs(DEMBuf[tempIndex]-nodata)>VERYSMALL){
				diff = (DEMBuf[i]-DEMBuf[tempIndex])/sqrt(2.0);
				if(theLargestDiff<=diff){
					theLargestDiff=diff;
					theLowestIndex = tempIndex;
					theLowestCode = 16;
				}
			}
		}
		if(assertS(i, nrow, ncol)){
			tempIndex = getS(i, ncol);
			if(fabs(DEMBuf[tempIndex]-nodata)>VERYSMALL){
				diff = DEMBuf[i]-DEMBuf[tempIndex];
				if(theLargestDiff<=diff){
					theLargestDiff=diff;
					theLowestIndex = tempIndex;
					theLowestCode = 8;
				}
			}
		}
		if(assertSE(i, nrow, ncol)){
			tempIndex = getSE(i, ncol);
			if(fabs(DEMBuf[tempIndex]-nodata)>VERYSMALL){
				diff = (DEMBuf[i]-DEMBuf[tempIndex])/sqrt(2.0);
				if(theLargestDiff<=diff){
					theLargestDiff=diff;
					theLowestIndex = tempIndex;
					theLowestCode = 4;
				}
			}
		}
	
		if(theLowestIndex!=-1){
			if(DEMBuf[i]!=DEMBuf[theLowestIndex]) //found a direction and not flat
				outBuf[i] = theLowestCode;
		}
		else
			outBuf[i] = 0; //bottom of basin
	}	

	//handle flat
	unsigned char* IDBuf = new unsigned char[numPixels];
	for(i=0; i<numPixels; i++)
		if(fabs(outBuf[i]-nodata)<VERYSMALL || (int)outBuf[i]!=255)
			IDBuf[i] = 2;
		else
			IDBuf[i] = 0;
	for(i=0; i<numPixels; i++){
		if(IDBuf[i]==0){
			Patch<float>* thePatch = new Patch<float>(i, outBuf, IDBuf, nrow, ncol, true);
			while(thePatch->numPixels>0){
				int flat = thePatch->numPixels; //preserve the original number for testing basin
				PatchPixel* temp = thePatch->first;
				while(temp!=NULL){
					if(assertN(temp->pos, nrow, ncol)){
						tempIndex = getN(temp->pos,ncol);
						if(fabs(outBuf[tempIndex]-nodata)>VERYSMALL && outBuf[tempIndex]<130 && DEMBuf[temp->pos] >= DEMBuf[tempIndex]){
							outBuf[temp->pos] = 138;
							temp=temp->nextPixel;
							continue;
						}
					}
					if(assertW(temp->pos, nrow, ncol)){ 
						tempIndex = getW(temp->pos,ncol);
						if(fabs(outBuf[tempIndex]-nodata)>VERYSMALL && outBuf[tempIndex]<130 && DEMBuf[temp->pos] >= DEMBuf[tempIndex]){
							outBuf[temp->pos] = 136;
							temp=temp->nextPixel;
							continue;
						}
					}
					if(assertE(temp->pos, nrow, ncol)){ 
						tempIndex = getE(temp->pos,ncol);
						if(fabs(outBuf[tempIndex]-nodata)>VERYSMALL && outBuf[tempIndex]<130 && DEMBuf[temp->pos] >= DEMBuf[tempIndex]){
							outBuf[temp->pos] = 132;
							temp=temp->nextPixel;
							continue;
						}
					}
					if(assertS(temp->pos, nrow, ncol)){
						tempIndex = getS(temp->pos,ncol);
						if(fabs(outBuf[tempIndex]-nodata)>VERYSMALL && outBuf[tempIndex]<130 && DEMBuf[temp->pos] >= DEMBuf[tempIndex]){
							outBuf[temp->pos] = 134;
							temp=temp->nextPixel;
							continue;
						}
					}
					if(assertNW(temp->pos, nrow, ncol)){
						tempIndex = getNW(temp->pos,ncol);
						if(fabs(outBuf[tempIndex]-nodata)>VERYSMALL && outBuf[tempIndex]<130 && DEMBuf[temp->pos] >= DEMBuf[tempIndex]){
							outBuf[temp->pos] = 137;
							temp=temp->nextPixel;
							continue;
						}
					}
					if(assertNE(temp->pos, nrow, ncol)){
						tempIndex = getNE(temp->pos,ncol);
						if(fabs(outBuf[tempIndex]-nodata)>VERYSMALL && outBuf[tempIndex]<130 && DEMBuf[temp->pos] >= DEMBuf[tempIndex]){
							outBuf[temp->pos] = 131;
							temp=temp->nextPixel;
							continue;
						}
					}
					if(assertSW(temp->pos, nrow, ncol)){ 
						tempIndex = getSW(temp->pos,ncol);
						if(fabs(outBuf[tempIndex]-nodata)>VERYSMALL && outBuf[tempIndex]<130 && DEMBuf[temp->pos] >= DEMBuf[tempIndex]){
							outBuf[temp->pos] = 135;
							temp=temp->nextPixel;
							continue;
						}
					}
					if(assertSE(temp->pos, nrow, ncol)){ 
						tempIndex = getSE(temp->pos,ncol);
						if(fabs(outBuf[tempIndex]-nodata)>VERYSMALL && outBuf[tempIndex]<130 && DEMBuf[temp->pos] >= DEMBuf[tempIndex]){
							outBuf[temp->pos] = 133;
							temp=temp->nextPixel;
							continue;
						}
					}
					temp=temp->nextPixel;
				}
				temp = thePatch->first;
				while(temp!=NULL){
					bool isExit = true;
					switch((int)outBuf[temp->pos]){
						case 131:
							outBuf[temp->pos] = 1;
							break;
						case 132:
							outBuf[temp->pos] = 2;
							break;
						case 133:
							outBuf[temp->pos] = 4;
							break;
						case 134:
							outBuf[temp->pos] = 8;
							break;
						case 135:
							outBuf[temp->pos] = 16;
							break;
						case 136:
							outBuf[temp->pos] = 32;
							break;
						case 137:
							outBuf[temp->pos] = 64;
							break;
						case 138:
							outBuf[temp->pos] = 128;
							break;
						default:
							isExit = false;
							break;
					}
					if(isExit){
						IDBuf[temp->pos]=2;
						PatchPixel* oldTemp = temp;
						temp=temp->nextPixel;
						thePatch->removePixel(oldTemp);
					}
					else
						temp=temp->nextPixel;
				}
				if(flat==thePatch->numPixels){ //inward flat, arbituraly assign the first pixel as the target 
					outBuf[thePatch->first->pos] = 0;
					IDBuf[thePatch->first->pos] = 2;
					thePatch->removePixel(thePatch->first);
				}
			}
			delete thePatch;
		}
	}
	delete [] IDBuf;
	return 0;
}

int fuzzyAspects(float* aspectBuf,	//an aspect data layer
				  float* outBuf,	//result
				  int nrow,			
				  int ncol, 
				  float lowerLimitFor0, //in degree
				  float upperLimitFor0, //in degree
				  float lowerLimitFor1, //in degree
  				  float upperLimitFor1, //in degree
				  float nodata){

	if(aspectBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(lowerLimitFor0<0 || lowerLimitFor0>360 ||
		lowerLimitFor1<0 || lowerLimitFor1>360 ||
		upperLimitFor1<0 || upperLimitFor1>360 ||
		upperLimitFor0<0 || upperLimitFor0>360)
		return 3;

	//sort the four limits
	//label code: 1: lowerLimitFor0
	//			  2: lowerLimitFor1
	//			  3: upperLimitFor1
	//			  4: upperLimitFor0
	struct Limits{
		int label;
		float degree;
	};
	Limits theLimits[4];
	theLimits[0].label = 1;
	theLimits[0].degree = lowerLimitFor0;
	theLimits[1].label = 2;
	theLimits[1].degree = lowerLimitFor1;
	theLimits[2].label = 3;
	theLimits[2].degree = upperLimitFor1;
	theLimits[3].label = 4;
	theLimits[3].degree = upperLimitFor0;

	Limits tempLimit;

	for(int i=0; i<3; i++){
		for(int j = i+1; j<4; j++){
			if(theLimits[i].degree > theLimits[j].degree){
				tempLimit.label = theLimits[i].label;
				tempLimit.degree = theLimits[i].degree;
				theLimits[i].label = theLimits[j].label;
				theLimits[i].degree = theLimits[j].degree;
				theLimits[j].label = tempLimit.label;
				theLimits[j].degree = tempLimit.degree;
			}
		}
	}

	//check if the limits are reasonable
	for(i=0; i<3; i++)
		if(abs(theLimits[i].label-theLimits[i+1].label) != 1)
			return 4;

	//set value
	int numPixels = nrow*ncol;
	for(i=0; i<numPixels; i++){
		if(aspectBuf[i] == nodata || aspectBuf[i]<0 || aspectBuf[i]>360){
			outBuf[i] = -1;
			continue;
		}
		else{
			for(int j = 0; j<3; j++)
				if(aspectBuf[i]>theLimits[j].degree && aspectBuf[i]<=theLimits[j+1].degree){
					switch(theLimits[j].label){
					case 1:
						if(theLimits[j+1].label == 2)
							outBuf[i] = (aspectBuf[i]-theLimits[j].degree)/(theLimits[j+1].degree-theLimits[j].degree); //linear function
						else if(theLimits[j+1].label == 4)
							outBuf[i] = 0;
						else
							return 5;
						break;
					case 2:
						if(theLimits[j+1].label == 1)
							outBuf[i] = 1-(aspectBuf[i]-theLimits[j].degree)/(theLimits[j+1].degree-theLimits[j].degree); //linear function
						else if(theLimits[j+1].label == 3)
							outBuf[i] = 1;
						else
							return 6;
						break;
					case 3:
						if(theLimits[j+1].label == 2)
							outBuf[i] = 1;
						else if(theLimits[j+1].label == 4)
							outBuf[i] = 1-(aspectBuf[i]-theLimits[j].degree)/(theLimits[j+1].degree-theLimits[j].degree); //linear function
						else
							return 7;
						break;
					case 4:
						if(theLimits[j+1].label == 1)
							outBuf[i] = 0;
						else if(theLimits[j+1].label == 3)
							outBuf[i] = (aspectBuf[i]-theLimits[j].degree)/(theLimits[j+1].degree-theLimits[j].degree); //linear function
						else
							return 8;
						break;
					default:
						return 9;
					}
				}
			if(aspectBuf[i]>theLimits[3].degree && aspectBuf[i]<=theLimits[0].degree){
				float pseudoDegree = nodata;
				if(aspectBuf[i]<=theLimits[0].degree)
					pseudoDegree = aspectBuf[i]+360;
				else
					pseudoDegree = aspectBuf[i];
				float pseudoLimits = theLimits[0].degree + 360;
				switch(theLimits[3].label){
					case 1:
						if(theLimits[0].label == 2)
							outBuf[i] = (pseudoDegree-theLimits[3].degree)/(pseudoLimits-theLimits[3].degree); //linear function
						else if(theLimits[0].label == 4)
							outBuf[i] = 0;
						else
							return 10;
						break;
					case 2:
						if(theLimits[0].label == 1)
							outBuf[i] = 1-(pseudoDegree-theLimits[3].degree)/(pseudoLimits-theLimits[3].degree); //linear function
						else if(theLimits[0].label == 3)
							outBuf[i] = 1;
						else
							return 11;
						break;
					case 3:
						if(theLimits[0].label == 2)
							outBuf[i] = 1;
						else if(theLimits[0].label == 4)
							outBuf[i] = 1-(pseudoDegree-theLimits[3].degree)/(pseudoLimits-theLimits[3].degree); //linear function
						else
							return 12;
						break;
					case 4:
						if(theLimits[0].label == 1)
							outBuf[i] = 0;
						else if(theLimits[0].label == 3)
							outBuf[i] = (pseudoDegree-theLimits[3].degree)/(pseudoLimits-theLimits[3].degree); //linear function
						else
							return 13;
						break;
					default:
						return 14;
				}
			}	
		}
	}
	return 0;
}

//This function uses circular-shape neighborhood and Evans (1972) algo.
//result is in percent (tan(degree))
//Since the weights become equal, this method becomes equivalent to some others, like
//Horn, One Over Distance
int gradientCirEvans(float* DEMBuf, 
			   float* outBuf, 
			   int nrow, 
			   int ncol, 
			   float cellsize,
			   float neighborhood,
			   float nodata){
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;

	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;
	double para[6];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] num of sqrt(2)cellsize between the center of the current cell and and bnd of the neighborhood
					//[2] weight1 in the cardinal direction
					//[3] weight2 in the cardinal direction
					//[4] weight1 in the diaganal direction
					//[5] weight2 in the diaganal direction
	bilinearWgtCir(neighborhood, cellsize, nrow, ncol, para);
	double result[9];
	// 1 2 3
	// 8 0 4
	// 7 6 5
	for(i=0; i<numPixels; i++){
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[1], para[2], para[3], para[4], para[5], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p = ((1.41421356*result[3]+2*result[4]+1.41421356*result[5])-(1.41421356*result[1]+2*result[8]+1.41421356*result[7]))/(neighborhood*4);//neighborhood/2*8
			double q = ((1.41421356*result[1]+2*result[2]+1.41421356*result[3])-(1.41421356*result[5]+2*result[6]+1.41421356*result[7]))/(neighborhood*4);//neighborhood/2*8
			outBuf[i] = sqrt(p*p+q*q)*100;
		}
	}
	return 0;
}

//This function uses square-shape neighborhood and Evans (1972) algo.
//result is in percent (tan(degree)). Used by 3dMapper
int gradientSqrEvans(float* DEMBuf, 
			   float* outBuf, 
			   int nrow, 
			   int ncol, 
			   float cellsize,
			   float neighborhood,
			   float nodata){
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;

	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;

	double para[3];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] weight1 in the cardinal direction
					//[2] weight2 in the cardinal direction
	bilinearWgtSqr(neighborhood, cellsize, nrow, ncol, para);

	double result[9];
	for(i=0; i<numPixels; i++){
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[0], para[1], para[2], para[1], para[2], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p = ((result[3]+result[4]+result[5])-(result[1]+result[8]+result[7]))/(neighborhood*3);//neighborhood/2*6
			double q = ((result[1]+result[2]+result[3])-(result[5]+result[6]+result[7]))/(neighborhood*3);
			outBuf[i] = sqrt(p*p+q*q)*100;
		}
	}
	return 0;
}

//This function uses Horn (1981) algo.
//result is in percent (tan(degree)).  Used by ArcInfo
//Uses square-shape neighborhood.  When using circular neighbor, this method becomes equivalent
//to Evans' (1972)
int gradientHorn(float* DEMBuf, 
			   float* outBuf, 
			   int nrow, 
			   int ncol, 
			   float cellsize,
			   float neighborhood,
			   float nodata){
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;

	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;

	double para[3];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] weight1 in the cardinal direction
					//[2] weight2 in the cardinal direction
	bilinearWgtSqr(neighborhood, cellsize, nrow, ncol, para);

	double result[9];
	for(i=0; i<numPixels; i++)
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[0], para[1], para[2], para[1], para[2], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p = ((result[3]+result[4]*2+result[5])-(result[1]+result[8]*2+result[7]))/(neighborhood*4);//neighborhood/2*8
			double q = ((result[1]+result[2]*2+result[3])-(result[7]+result[6]*2+result[5]))/(neighborhood*4);//neighborhood/2*8
			outBuf[i] = sqrt(p*p+q*q)*100;
		}
	return 0;
}

//This function uses circular-shape neighborhood and Shi algo.
//result is in percent (tan(degree))
int gradientCirShi(float* DEMBuf, 
			   float* outBuf, 
			   int nrow, 
			   int ncol, 
			   float cellsize,
			   float neighborhood,
			   float nodata){
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;
	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;

	double para[6];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] num of sqrt(2)cellsize between the center of the current cell and and bnd of the neighborhood
					//[2] weight1 in the cardinal direction
					//[3] weight2 in the cardinal direction
					//[4] weight1 in the diaganal direction
					//[5] weight2 in the diaganal direction
	bilinearWgtCir(neighborhood, cellsize, nrow, ncol, para);

	double result[9];
	for(i=0; i<numPixels; i++){
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[1], para[2], para[3], para[4], para[5], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p1 = (result[4]-result[8])/neighborhood;
			double q1 = (result[2]-result[6])/neighborhood;
			double p2 = (result[3]-result[7])/neighborhood;
			double q2 = (result[1]-result[5])/neighborhood;
			outBuf[i] = (sqrt(p1*p1+q1*q1)+sqrt(p2*p2+q2*q2))*100/2;
		}
	}
	return 0;
}

//This function uses square-shape neighborhood and Shi algo.
//result is in percent (tan(degree))
int gradientSqrShi(float* DEMBuf, 
			   float* outBuf, 
			   int nrow, 
			   int ncol, 
			   float cellsize,
			   float neighborhood,
			   float nodata){
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;

	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;

	double para[3];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] weight1 in the cardinal direction
					//[2] weight2 in the cardinal direction
	bilinearWgtSqr(neighborhood, cellsize, nrow, ncol, para);

	double result[9];
	for(i=0; i<numPixels; i++)
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[0], para[1], para[2], para[1], para[2], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p1 = (result[4]-result[8])/neighborhood;
			double q1 = (result[2]-result[6])/neighborhood;
			double p2 = (result[3]-result[7])/(neighborhood*1.41421356);
			double q2 = (result[1]-result[5])/(neighborhood*1.41421356);
			outBuf[i] = (sqrt(p1*p1+q1*q1)+sqrt(p2*p2+q2*q2))*100/2;
		}
   return 0;
}

//This function Zevenbergen and Thorne (1987) algo.
//Since only the 4 cells in the cardinal directions are used,
//circular and square neighborhood will give the same result.
//result is in percent (tan(degree))
int gradientZT(float* DEMBuf, 
			   float* outBuf, 
			   int nrow, 
			   int ncol, 
			   float cellsize,
			   float neighborhood,
			   float nodata){
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 2;
	if(cellsize <= 0) return 3;

	int numPixels = nrow*ncol;
	for(int i=0; i<numPixels; i++)
		outBuf[i] = nodata;

	double para[3];	//[0] number of cellsizes between the center of the current cell and the boundary of the neighborhood
					//[1] weight1 in the cardinal direction
					//[2] weight2 in the cardinal direction
	bilinearWgtSqr(neighborhood, cellsize, nrow, ncol, para);

	double result[9];
	for(i=0; i<numPixels; i++)
		if(neighborhoodValues(DEMBuf, nrow, ncol, i, (int)para[0], (int)para[0], para[1], para[2], para[1], para[2], result, nodata)==0){//got the vlaues
			if(fabs(result[0]-nodata)<VERYSMALL || fabs(result[1]-nodata)<VERYSMALL || fabs(result[2]-nodata)<VERYSMALL || 
				fabs(result[3]-nodata)<VERYSMALL || fabs(result[4]-nodata)<VERYSMALL || fabs(result[5]-nodata)<VERYSMALL || 
				fabs(result[6]-nodata)<VERYSMALL || fabs(result[7]-nodata)<VERYSMALL || fabs(result[8]-nodata)<VERYSMALL)
				continue;
			double p = (result[4]-result[8])/neighborhood;
			double q = (result[2]-result[6])/neighborhood;
			outBuf[i] = sqrt(p*p+q*q)*100;
		}
	return 0;
}

int headwater(float* flowDir, 
			  float* stream, 
			  float* planform,
			  float* profile,
			  float* flowAcc,
			  float* outBuf, 
			  int streamOrder,
			  bool nodeType, //false: jump nodes; true: all nodes
			  float planformWgt,
			  float profileWgt,
			  int upSearchRange,
			  int downSearchRange,
			  int nrow, int ncol,
			  float nodata){
	if(flowDir == NULL) return 1;
	if(stream == NULL) return 2;
	if(profile == NULL) return 3;
	if(outBuf == NULL) return 4;
	
	int numPixels = nrow*ncol;
	bool* idBuf = new bool[numPixels];
	bool* idBuf2 = new bool[numPixels];
	for(int i = 0; i<numPixels; i++){
		if(fabs(flowDir[i]-nodata)<VERYSMALL || fabs(stream[i]-nodata)<VERYSMALL || fabs(planform[i]-nodata)<VERYSMALL || fabs(profile[i]-nodata)<VERYSMALL){
			outBuf[i] = nodata;
			idBuf[i] = false;
		}
		else{
			outBuf[i] = 0;
			if(stream[i]>0){
				idBuf[i] = true;
				idBuf2[i] = true;
			}
			else{ 
				idBuf[i] = false;
				idBuf2[i] = false;
			}
		}
	}

	for(i = 0; i<numPixels; i++){
		//1. find the node
		if(stream[i] != streamOrder)
			continue;

		bool isNode = false;
		bool isJumpNode = false;
		int firstUp = previousMaxFlowPixel(i, flowDir, flowAcc, idBuf, nrow, ncol);
		if(firstUp == -1){ //the upper end of the stream
			isNode = true;
			isJumpNode = true;
		}
		else{
			idBuf[firstUp] = false;
			if(stream[firstUp]!=streamOrder)
				isNode = true;
			int secondUp = previousMaxFlowPixel(i, flowDir, flowAcc, idBuf, nrow, ncol);
			if(secondUp > 0){
				if(stream[secondUp]!=streamOrder){
					if(isNode)
						isJumpNode = true;
					else
						isNode = true;
				}
			}
		}
		if(!isNode) continue;
		if(!nodeType && !isJumpNode) continue;

		//2. find the peak location
		float max = profile[i]*profileWgt;
		if(planform!=NULL)
			max += planform[i]*planformWgt;
		int pos = i;
		int curPos = i;

		//2.1 up search
		for(int up = 0; up<upSearchRange; up++){
			curPos = previousMaxFlowPixel(curPos, flowDir, flowAcc, idBuf2, nrow, ncol);
			if(curPos == -1) break;
			idBuf2[curPos] = false;
			float v = profile[curPos]*profileWgt;
			if(planform!=NULL)
				v += planform[curPos]*planformWgt;
			if(v>max){
				max = v;
				pos = curPos;
			}
		}
		curPos = i; //detect the second tributary.  Bug: cannot handle the scenario that more than two tributaries flow into the same pos
		for(up = 0; up<upSearchRange; up++){
			curPos = previousMaxFlowPixel(curPos, flowDir, flowAcc, idBuf2, nrow, ncol);
			if(curPos == -1) break;
			float v = profile[curPos]*profileWgt;
			if(planform!=NULL)
				v += planform[curPos]*planformWgt;
			if(v>max){
				max = v;
				pos = curPos;
			}
		}
		//2.2 down search
		for(int down = 0; down<downSearchRange; down++){
			curPos = nextFlowPixel(curPos, flowDir, ncol);
			if(curPos<0) break;
			float v = profile[curPos]*profileWgt;
			if(planform!=NULL)
				v += planform[curPos]*planformWgt;
			if(v>max){
				max = v;
				pos = curPos;
			}
		}
		outBuf[pos] = 1;
	}
	delete [] idBuf2;
	delete [] idBuf;
	return 0;
}

int ridgelines1(float* inBuf,	//DEM
				float* outBuf,	//result
				int nrow, 
				int ncol, 
				int label,
				float nodata){
	//Puecker and Douglas (1975) algorithm
	if(outBuf == NULL) return 3;
	if(inBuf == NULL) return 1;

	int i= 0, j = 0;
	for(i = 0; i<nrow; i++)
		for(j = 0; j<ncol; j++)
			outBuf[i*ncol + j] = label;

	for(i = 1; i<nrow-1; i++)
		for(j = 1; j<ncol-1; j++){
			float lowest = inBuf[(i-1)*ncol+j-1];
			int id = 1;

			if(lowest > inBuf[(i-1)*ncol+j]){
				lowest = inBuf[(i-1)*ncol+j];
				id = 2;
			}

			if(lowest > inBuf[(i-1)*ncol+j+1]){
				lowest = inBuf[(i-1)*ncol+j+1];
				id = 3;
			}

			if(lowest > inBuf[i*ncol+j-1]){
				lowest = inBuf[i*ncol+j-1];
				id = 4;
			}

			if(lowest > inBuf[i*ncol+j+1]){
				lowest = inBuf[i*ncol+j+1];
				id = 5;
			}

			if(lowest > inBuf[(i+1)*ncol+j-1]){
				lowest = inBuf[(i+1)*ncol+j-1];
				id = 6;
			}

			if(lowest > inBuf[(i+1)*ncol+j]){
				lowest = inBuf[(i+1)*ncol+j];
				id = 7;
			}

			if(lowest > inBuf[(i+1)*ncol+j+1]){
				lowest = inBuf[(i+1)*ncol+j+1];
				id = 8;
			}

			switch(id){
			case 1:
				outBuf[(i-1)*ncol+j-1] = 0;
				break;
			case 2:
				outBuf[(i-1)*ncol+j] = 0;
				break;
			case 3:
				outBuf[(i-1)*ncol+j+1] = 0;
				break;
			case 4:
				outBuf[i*ncol+j-1] = 0;
				break;
			case 5:
				outBuf[i*ncol+j+1] = 0;
				break;
			case 6:
				outBuf[(i+1)*ncol+j-1] = 0;
				break;
			case 7:
				outBuf[(i+1)*ncol+j] = 0;
				break;
			case 8:
				outBuf[(i+1)*ncol+j+1] = 0;
				break;
			default:
				return 4;
			}
		}

	return 0;
}

int ridgelines2(float* DEMBuf,	//DEM
				float* outBuf,	//result
				int nrow, 
				int ncol, 
				int label,
				float nodata){
	//O'Callaghan and Mark (1984) algorithm
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 3;

	int i= 0, j = 0;
	for(i = 0; i<nrow; i++)
		for(j = 0; j<ncol; j++)
			outBuf[i*ncol + j] = 0;

	int opFlag = 0;
	float* flowDir = new float[nrow*ncol];
	float* flowAcc = new float[nrow*ncol];

	opFlag = flowDirection(DEMBuf, flowDir, nrow, ncol, nodata);
	if(opFlag != 0){
		return 6;
	}

	opFlag = flowAccumulation(flowDir, flowAcc, nrow, ncol, 1, nodata);
	if(opFlag != 0){
		return 7;
	}

	for(i = 0; i<nrow*ncol; i++)
		if((int)flowAcc[i]==1)
			outBuf[i] = label;

	delete [] flowDir;
	delete [] flowAcc;
	
	return 0;
}

int ridgelines3(float* DEMBuf,	//DEM
				float* outBuf,	//result
				int nrow, 
				int ncol, 
				int label,
				float nodata){
	//Skidmore (1991) algorithm
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 3;

	int i= 0, j = 0;
	for(i = 0; i<nrow; i++)
		for(j = 0; j<ncol; j++)
			outBuf[i*ncol + j] = 0;
	
	for(i = 1; i<nrow-1; i++)
		for(j = 1; j<ncol-1; j++){
			int p = i;
			int q = j;

			float central = DEMBuf[p*ncol+q];
			
			p = i-1;
			float north = DEMBuf[p*ncol+q];
			while(fabs(north-central)<VERYSMALL && p>=0){
				p = p-1;
				north = DEMBuf[p*ncol+q];
			}

			p = i+1;
			float south = DEMBuf[p*ncol+q];
			while(fabs(south-central)<VERYSMALL && p<nrow){
				p = p+1;
				north = DEMBuf[p*ncol+q];
			}

			p = i;
			q = j-1;
			float west = DEMBuf[p*ncol+q];
			while(fabs(west-central)<VERYSMALL && q>=0){
				q = q-1;
				west = DEMBuf[p*ncol+q];
			}

			q = j+1;
			float east = DEMBuf[p*ncol+q];
			while(fabs(east-central)<VERYSMALL && q<ncol){
				q = q+1;
				east = DEMBuf[p*ncol+q];
			}

			p = i-1;
			q = j-1;
			float northwest = DEMBuf[p*ncol+q];
			while(fabs(northwest-central)<VERYSMALL && p>=0 && q>=0){
				p = p-1;
				q = q-1;
				northwest = DEMBuf[p*ncol+q];
			}

			p = i-1;
			q = j+1;
			float northeast = DEMBuf[p*ncol+q];
			while(fabs(northeast-central)<VERYSMALL && p>=0 && q<ncol){
				p = p-1;
				q = q+1;
				northwest = DEMBuf[p*ncol+q];
			}

			p = i+1;
			q = j-1;
			float southwest = DEMBuf[p*ncol+q];
			while(fabs(southwest-central)<VERYSMALL && p<nrow && q>=0){
				p = p+1;
				q = q-1;
				southwest = DEMBuf[p*ncol+q];
			}

			p = i+1;
			q = j+1;
			float southeast = DEMBuf[p*ncol+q];
			while(fabs(southeast-central)<VERYSMALL && p<nrow && q<ncol){
				p = p+1;
				q = q+1;
				southeast = DEMBuf[p*ncol+q];
			}

			if(((north<central&&south<central)&&(west<central||east<central))
				||((west<central&&east<central)&&(north<central||south<central))
				||((northwest<central&&southeast<central)&&(northeast<central||southwest<central))
				||((northeast<central&&southwest<central)&&(northwest<central||southeast<central)))
				outBuf[i*ncol+j] = label;
		}
	return 0;
}

int ridgelineShi(float* terrainPartitionBuf, float* DEMBuf, float* outBuf, float  TPEleDiff, float proportion, int nrow, int ncol, int label, float nodata){
	if(terrainPartitionBuf == NULL) return 1;
	if(DEMBuf == NULL) return 2;
	if(outBuf == NULL) return 3;
	if(TPEleDiff<0) TPEleDiff = 0;

	int numPixels = nrow*ncol;
	unsigned char* idBuf = new unsigned char[nrow*ncol];
	for(int i = 0; i<numPixels; i++){ 
		if(terrainPartitionBuf[i] == 0) //exclude streamline pixels
			idBuf[i] = 4;
		else
			idBuf[i] = 0;
		outBuf[i] = 0;
	}

	for(i = 0; i<numPixels; i++){
		if(idBuf[i] == 0){
			Patch<float>* thePatch = new Patch<float>(i, terrainPartitionBuf, idBuf, nrow, ncol, true);
			thePatch->findBorder(false);

			PatchPixel* temp = thePatch->first; //get hi and low ele
			float hiEle = 0;
			float lowEle = 99999999;
			while(temp!=NULL){
				if(temp->atBorder){
					float curEle = DEMBuf[temp->pos];
					if(curEle>hiEle)
						hiEle = curEle;
					if(curEle<lowEle)
						lowEle = curEle;
				}
				temp = temp->nextPixel;
			}
			
			if(hiEle-lowEle>TPEleDiff){ //if the partition is "vertical" enough (not a too flat one)
				temp = thePatch->first; //label potential ridgeline
				while(temp!=NULL){
					if(temp->atBorder && (hiEle-DEMBuf[temp->pos])/(hiEle-lowEle)<proportion){ //if the border pixel is at the higher part of the partition
						outBuf[temp->pos] = label;
					}
					temp = temp->nextPixel;
				}
			}
			delete thePatch;
		}
	}

	for(i=0; i<nrow*ncol; i++){ //remove pixels adjacent to stream
		if(outBuf[i] == label){
			int testPos = -1;
			if(assertN(i, nrow, ncol)){
				testPos = getN(i, ncol);
				if(terrainPartitionBuf[testPos] == 0)
					outBuf[i] = 0;
			}
			if(assertNE(i, nrow, ncol)){
				testPos = getNE(i, ncol);
				if(terrainPartitionBuf[testPos] == 0)
					outBuf[i] = 0;
			}
			if(assertE(i, nrow, ncol)){
				testPos = getE(i, ncol);
				if(terrainPartitionBuf[testPos] == 0)
					outBuf[i] = 0;
			}
			if(assertSE(i, nrow, ncol)){
				testPos = getSE(i, ncol);
				if(terrainPartitionBuf[testPos] == 0)
					outBuf[i] = 0;
			}
			if(assertS(i, nrow, ncol)){
				testPos = getS(i, ncol);
				if(terrainPartitionBuf[testPos] == 0)
					outBuf[i] = 0;
			}
			if(assertSW(i, nrow, ncol)){
				testPos = getSW(i, ncol);
				if(terrainPartitionBuf[testPos] == 0)
					outBuf[i] = 0;
			}
			if(assertW(i, nrow, ncol)){
				testPos = getW(i, ncol);
				if(terrainPartitionBuf[testPos] == 0)
					outBuf[i] = 0;
			}
			if(assertNW(i, nrow, ncol)){
				testPos = getNW(i, ncol);
				if(terrainPartitionBuf[testPos] == 0)
					outBuf[i] = 0;
			}
		}
	}
	return 0;
}

int shaveDEM(float* DEMBuf,	//Input DEM
			  float* cachBuf,	//empty buffer for holding catchment data
			  float* flowBuf,	//empty buffer for holding flow direction
			  float* outBuf,	//result
			  float maxEle, //maximum elevation value in the DEM
			  float threshold,
			  int nrow, 
			  int ncol, 
			  float nodata){
	if(DEMBuf == NULL) return 1;
	if(cachBuf == NULL) return 2;
	if(flowBuf == NULL) return 3;
	if(outBuf == NULL) return 4;
	if(threshold<0) return 5;

	int i=0;
	float* reverseDEM = new float[nrow*ncol];
	for(i=0; i<nrow*ncol; i++)
		reverseDEM[i] = maxEle - DEMBuf[i];

	fillDEM(reverseDEM,	cachBuf, flowBuf, outBuf, threshold, nrow, ncol, maxEle-nodata);
	delete [] reverseDEM;
	
	for(i=0; i<nrow*ncol; i++)
		outBuf[i] = maxEle - outBuf[i];

	return 0;
}

int shoulder(float* terrainPartitionBuf, 
			 float* planformBuf, 
			 float* profileBuf, 
			 float* slopeBuf,
			 float* ridgeBuf,
			 float* outBuf, 
			 float planformWgt, 
			 float profileWgt,
			 float slopeThresh,
			 int minArea, //in number of pixels
			 float topPercent,
			 int nrow, int ncol,
			 float nodata){
	if(terrainPartitionBuf == NULL) return 1;
	if(planformBuf == NULL) return 2;
	if(profileBuf == NULL) return 3;
	if(slopeBuf == NULL) return 4;
	if(ridgeBuf == NULL) return 5;
	if(outBuf == NULL) return 6;
	if(topPercent<0) return 7;

	int numPixels = nrow*ncol;
	unsigned char* idBuf = new unsigned char[numPixels];
	for(int i = 0; i<numPixels; i++){
		if(fabs(terrainPartitionBuf[i]-nodata)<VERYSMALL){
			idBuf[i] = 4;
			outBuf[i] = nodata;
		}
		else{
			if(terrainPartitionBuf[i] == 0)
				idBuf[i] = 4;
			else
				idBuf[i] = 0;
			outBuf[i] = 0;
		}
	}

	for(i = 0; i<numPixels; i++){
		if(idBuf[i] == 0){
			Patch<float>* thePatch = new Patch<float>(i, terrainPartitionBuf, idBuf, nrow, ncol, true);
			if(thePatch->numPixels<minArea){ //if the area of the partition is smaller than the threshold, skip
				delete thePatch;
				continue;
			}
			thePatch->findBorder(false);
			bool hasRidge = false;
			PatchPixel* temp = thePatch->first;
			while(temp!=NULL){
				if(temp->atBorder){
					if(ridgeBuf[temp->pos]>0){
						hasRidge = true;
						break;
					}
					if(assertN(temp->pos, nrow, ncol) && ridgeBuf[getN(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
					if(assertNE(temp->pos, nrow, ncol) && ridgeBuf[getNE(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
					if(assertE(temp->pos, nrow, ncol) && ridgeBuf[getE(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
					if(assertSE(temp->pos, nrow, ncol) && ridgeBuf[getSE(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
					if(assertS(temp->pos, nrow, ncol) && ridgeBuf[getS(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
					if(assertSW(temp->pos, nrow, ncol) && ridgeBuf[getSW(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
					if(assertW(temp->pos, nrow, ncol) && ridgeBuf[getW(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
					if(assertNW(temp->pos, nrow, ncol) && ridgeBuf[getNW(temp->pos,ncol)]>0){
						hasRidge = true;
						break;
					}
				}
				temp = temp->nextPixel;
			}
			if(!hasRidge){
				delete thePatch;
				continue;
			}

			int arraySize = thePatch->numPixels*topPercent/100;
			int* array = new int[arraySize];
			for(int k=0; k<arraySize; k++)
				array[k] = -1;
			temp = thePatch->first;
			while(temp!=NULL){
				if(slopeBuf[temp->pos]<slopeThresh){
					temp = temp->nextPixel;
					continue;
				}
				float curV = planformBuf[temp->pos]*planformWgt+profileBuf[temp->pos]*profileWgt;
				for(k=0; k<arraySize; k++){
					float candidateV = 9999;
					if(array[k] != -1)
						candidateV = planformBuf[array[k]]*planformWgt+profileBuf[array[k]]*profileWgt;
					if(curV>candidateV)
						break;
				}
				if(k==0){
					temp = temp->nextPixel;
					continue;
				}
				k -= 1;
				for(int p=0; p<k; p++)
					array[p] = array[p+1];
				array[k]=temp->pos;
				temp = temp->nextPixel;
			}
			delete thePatch;
			for(k=0; k<arraySize; k++)
				if(array[k]!=-1)
					outBuf[array[k]] = 100;
		}
	}
	
	return 0;
}

int streamlineOCallaghan(float* flowDir,	//flow direction layer
				 float* flowAcc,	//flow accumulation layer
				 float* outBuf,		//result
				 int nrow, 
				 int ncol,
				 float threshold,//level of the stream network
				 int label, 
				 CString codingAlg, 
				 float nodata){
	if(flowDir == NULL) return 1;
	if(flowAcc == NULL) return 2;
	if(outBuf == NULL) return 3;
	if(threshold < 0) return 4;
	int numPixels = nrow*ncol;
	for(int i = 0; i<numPixels; i++){
		if(fabs(flowDir[i]-nodata)<VERYSMALL)
			outBuf[i] = nodata;
		else
			outBuf[i] = 0;
	}

	//find out the number of water heads and label the water heads
	int numOfWaterHeads = 0;
	List<IntNode>* headList = new List<IntNode>;
	for(i = 0; i<numPixels; i++)
		if(fabs(flowAcc[i]-nodata)>VERYSMALL && flowAcc[i]>=threshold){
			int previous = previousMaxFlowPixel(i, flowDir, flowAcc, NULL, nrow, ncol);
			if(fabs(flowAcc[previous]-nodata)>VERYSMALL && flowAcc[previous]>=threshold)
				continue;
			if(codingAlg=="Simple mark")
				outBuf[i] = label;
			else if(codingAlg=="Horton's order" || codingAlg=="Strahler's order")
				outBuf[i] = 1;//most upperstream is 1
			else if(codingAlg=="Shi's order")
				outBuf[i] = ++numOfWaterHeads;
			headList->addTail(new IntNode(i));
		}

	int leap = numOfWaterHeads;
	//label the streams
	IntNode* temp = headList->first;
	while(temp!=NULL){
		int curPos = temp->pos;
		while(flowDir[curPos] > 0){
			int oldPos = curPos;
			int curLabel = outBuf[curPos];
			curPos = nextFlowPixel(curPos, flowDir, ncol);
			if((int)outBuf[curPos]==0){
				if(codingAlg=="Simple mark")
					outBuf[curPos] = label;
				else 
					outBuf[curPos] = curLabel;
			}
			else{
				if(codingAlg=="Simple mark")
					break;
				if(codingAlg=="Horton's order" || codingAlg=="Strahler's order"){
					if((int)outBuf[curPos]>curLabel)
						break;
					if((int)outBuf[curPos]==curLabel){ //need to find if it is the real junction of two equal-level tributaries
						if(assertNW(curPos, nrow, ncol))
							if(getNW(curPos, ncol)!=oldPos && (int)flowDir[getNW(curPos, ncol)]==4 && outBuf[getNW(curPos, ncol)]==curLabel){
								outBuf[curPos] += 1;
								continue;
							}
						if(assertN(curPos, nrow, ncol))
							if(getN(curPos, ncol)!=oldPos && (int)flowDir[getN(curPos, ncol)]==8 && outBuf[getN(curPos, ncol)]==curLabel){
								outBuf[curPos] += 1;
								continue;
							}
						if(assertNE(curPos, nrow, ncol))
							if(getNE(curPos, ncol)!=oldPos && (int)flowDir[getNE(curPos, ncol)]==16 && outBuf[getNE(curPos, ncol)]==curLabel){
								outBuf[curPos] += 1;
								continue;
							}
						if(assertW(curPos, nrow, ncol))
							if(getW(curPos, ncol)!=oldPos && (int)flowDir[getW(curPos, ncol)]==2 && outBuf[getW(curPos, ncol)]==curLabel){
								outBuf[curPos] += 1;
								continue;
							}
						if(assertE(curPos, nrow, ncol))
							if(getE(curPos, ncol)!=oldPos && (int)flowDir[getE(curPos, ncol)]==32 && outBuf[getE(curPos, ncol)]==curLabel){
								outBuf[curPos] += 1;
								continue;
							}
						if(assertSW(curPos, nrow, ncol))
							if(getSW(curPos, ncol)!=oldPos && (int)flowDir[getSW(curPos, ncol)]==1 && outBuf[getSW(curPos, ncol)]==curLabel){
								outBuf[curPos] += 1;
								continue;
							}
						if(assertS(curPos, nrow, ncol))
							if(getS(curPos, ncol)!=oldPos && (int)flowDir[getS(curPos, ncol)]==128 && outBuf[getS(curPos, ncol)]==curLabel){
								outBuf[curPos] += 1;
								continue;
							}
						if(assertSE(curPos, nrow, ncol))
							if(getSE(curPos, ncol)!=oldPos && (int)flowDir[getSE(curPos, ncol)]==64 && outBuf[getSE(curPos, ncol)]==curLabel){
								outBuf[curPos] += 1;
								continue;
							}
					}
					else if((int)outBuf[curPos]<curLabel)
						outBuf[curPos] = curLabel;
				}
				else if(codingAlg=="Shi's order"){
					outBuf[curPos] += leap;
				}
			}
		}
		temp = temp->next;
	}
	delete headList;

	//if Horton, reverse the order
	if(codingAlg=="Horton's order"){
		float minmax[2];
		dataMinMax(outBuf, numPixels, 0, minmax);
		for(i = 0; i<numPixels; i++)
			if(fabs(outBuf[i]-0)>VERYSMALL)
				outBuf[i] = minmax[1]-outBuf[i]+1;
	}

	return 0;
}

int streamlinePeuker(float* inBuf, //dem buffer
					   float* outBuf, //output buffer
					   int nrow, 
					   int ncol, 
					   int label, //label of streamline in output
					   float nodata){
	//Puecker and Douglas (1975) algorithm
	if(outBuf == NULL) return 3;
	if(inBuf == NULL) return 1;

	int i= 0, j = 0;
	for(i = 0; i<nrow; i++)
		for(j = 0; j<ncol; j++)
			outBuf[i*ncol + j] = label;

	for(i = 1; i<nrow-1; i++)
		for(j = 1; j<ncol-1; j++){
			float highest = inBuf[(i-1)*ncol+j-1];
			int id = 1;

			if(highest < inBuf[(i-1)*ncol+j]){
				highest = inBuf[(i-1)*ncol+j];
				id = 2;
			}

			if(highest < inBuf[(i-1)*ncol+j+1]){
				highest = inBuf[(i-1)*ncol+j+1];
				id = 3;
			}

			if(highest < inBuf[i*ncol+j-1]){
				highest = inBuf[i*ncol+j-1];
				id = 4;
			}

			if(highest < inBuf[i*ncol+j+1]){
				highest = inBuf[i*ncol+j+1];
				id = 5;
			}

			if(highest < inBuf[(i+1)*ncol+j-1]){
				highest = inBuf[(i+1)*ncol+j-1];
				id = 6;
			}

			if(highest < inBuf[(i+1)*ncol+j]){
				highest = inBuf[(i+1)*ncol+j];
				id = 7;
			}

			if(highest < inBuf[(i+1)*ncol+j+1]){
				highest = inBuf[(i+1)*ncol+j+1];
				id = 8;
			}

			switch(id){
			case 1:
				outBuf[(i-1)*ncol+j-1] = 0;
				break;
			case 2:
				outBuf[(i-1)*ncol+j] = 0;
				break;
			case 3:
				outBuf[(i-1)*ncol+j+1] = 0;
				break;
			case 4:
				outBuf[i*ncol+j-1] = 0;
				break;
			case 5:
				outBuf[i*ncol+j+1] = 0;
				break;
			case 6:
				outBuf[(i+1)*ncol+j-1] = 0;
				break;
			case 7:
				outBuf[(i+1)*ncol+j] = 0;
				break;
			case 8:
				outBuf[(i+1)*ncol+j+1] = 0;
				break;
			default:
				return 4;
			}
		}

	return 0;	
}

int streamlineSkidmore(float* DEMBuf, //dem buffer
					   float* outBuf, //output buffer
					   float upper,
					   float lower,
					   int nrow, 
					   int ncol, 
					   int label, //label of streamline in output
					   float nodata){
	//Skidmore (1991) algorithm
	if(DEMBuf == NULL) return 1;
	if(outBuf == NULL) return 3;

	int i= 0, j = 0;
	for(i = 0; i<nrow; i++)
		for(j = 0; j<ncol; j++)
			outBuf[i*ncol + j] = 0;
	
	for(i = 1; i<nrow-1; i++)
		for(j = 1; j<ncol-1; j++){
			float central = DEMBuf[i*ncol+j];
			bool flag = true;

			int p = i;
			int q = j;
			while(true){
				p--;
				if(p<0){
					flag = false;
					break;
				}
				if(central - DEMBuf[p*ncol+q] > lower){
					flag = false;
					break;
				}
				if(DEMBuf[p*ncol+q] - central > upper){
					flag = true;
					break;
				}
			}

			if(flag){
				p = i;
				while(true){
					p++;
					if(p>nrow-1){
						flag = false;
						break;
					}
					if(central - DEMBuf[p*ncol+q] > lower){
						flag = false;
						break;
					}
					if(DEMBuf[p*ncol+q] - central > upper){
						flag = true;
						break;
					}
				}
			}

			if(flag){
				outBuf[i*ncol+j] = label;
				continue;
			}

			p = i;
			while(true){
				q--;
				if(q<0){
					flag = false;
					break;
				}
				if(central - DEMBuf[p*ncol+q] > lower){
					flag = false;
					break;
				}
				if(DEMBuf[p*ncol+q] - central > upper){
					flag = true;
					break;
				}
			}

			if(flag){
				q = j;
				while(true){
					q++;
					if(q>ncol-1){
						flag = false;
						break;
					}
					if(central - DEMBuf[p*ncol+q] > lower){
						flag = false;
						break;
					}
					if(DEMBuf[p*ncol+q] - central > upper){
						flag = true;
						break;
					}
				}
			}

			if(flag){
				outBuf[i*ncol+j] = label;
				continue;
			}

			q = j;
			while(true){ //northwest
				p--;
				q--;
				if(p<0 || q<0){
					flag = false;
					break;
				}
				if(central - DEMBuf[p*ncol+q] > lower){
					flag = false;
					break;
				}
				if(DEMBuf[p*ncol+q] - central > upper){
					flag = true;
					break;
				}
			}

			if(flag){ //southeast
				p = i;
				q = j;
				while(true){
					p++;
					q++;
					if(p>nrow-1 || q>ncol-1){
						flag = false;
						break;
					}
					if(central - DEMBuf[p*ncol+q] > lower){
						flag = false;
						break;
					}
					if(DEMBuf[p*ncol+q] - central > upper){
						flag = true;
						break;
					}
				}
			}

			if(flag){
				outBuf[i*ncol+j] = label;
				continue;
			}

			p = i;
			q = j;
			while(true){ //northeast
				p--;
				q++;
				if(p<0 || q>ncol-1){
					flag = false;
					break;
				}
				if(central - DEMBuf[p*ncol+q] > lower){
					flag = false;
					break;
				}
				if(DEMBuf[p*ncol+q] - central > upper){
					flag = true;
					break;
				}
			}

			if(flag){ //southwest
				p = i;
				q = j;
				while(true){
					p++;
					q--;
					if(p>nrow-1 || q<0){
						flag = false;
						break;
					}
					if(central - DEMBuf[p*ncol+q] > lower){
						flag = false;
						break;
					}
					if(DEMBuf[p*ncol+q] - central > upper){
						flag = true;
						break;
					}
				}
			}

			if(flag){
				outBuf[i*ncol+j] = label;
				continue;
			}
		}
	return 0;
}

int terrainPartition(float* streamlineBuf, 
					 float* flowDirBuf,
					 float* outBuf, 
					 int nrow, int ncol, 
					 float theAngle, //threshold for combining partitions w/ similar aspects
					 float nodata){ 
	if(streamlineBuf == NULL) return 1;
	if(flowDirBuf == NULL) return 2;
	if(outBuf == NULL) return 3;

	int numPixels = nrow*ncol;
	for(int i = 0; i<numPixels; i++)
		outBuf[i] = streamlineBuf[i];

	for(i=0; i<numPixels; i++){
		int curPos = i;
		List<IntNode>* flowPath = new List<IntNode>;
		while(outBuf[curPos]==0){
			flowPath->addTail(new IntNode(curPos));
			curPos = nextFlowPixel(curPos, flowDirBuf, ncol);
		} //end while: end finding a path

		//avoid flowing into the head of the main stream
		int realCurPos = curPos;
		if(flowPath->last!=NULL && streamlineBuf[curPos]>0){
			int endPos = flowPath->last->pos;
			int posN = -1;
			if(assertN(endPos, nrow, ncol)) posN = getN(endPos, ncol);
			int posNW = -1;
			if(assertNW(endPos, nrow, ncol)) posNW = getNW(endPos, ncol);
			int posNE = -1;
			if(assertNE(endPos, nrow, ncol)) posNE = getNE(endPos, ncol);
			int posE = -1;
			if(assertE(endPos, nrow, ncol)) posE = getE(endPos, ncol);
			int posW = -1;
			if(assertW(endPos, nrow, ncol)) posW = getW(endPos, ncol);
			int posSW = -1;
			if(assertSW(endPos, nrow, ncol)) posSW = getSW(endPos, ncol);
			int posS = -1;
			if(assertS(endPos, nrow, ncol)) posS = getS(endPos, ncol);
			int posSE = -1;
			if(assertSE(endPos, nrow, ncol)) posSE = getSE(endPos, ncol);
			int num = 0;
			switch((int)flowDirBuf[endPos]){
			case 1: //NE, should check N and E
				if(outBuf[posN]>0 && outBuf[posE]>0 && outBuf[posN]<outBuf[curPos] && outBuf[posE]<outBuf[curPos])
					if(outBuf[posN]>outBuf[posE])
						realCurPos = posN;
					else
						realCurPos = posE;
				break;
			case 2: //E, should check (N, S), (N, SE), (S, NE), (NE, SE)
				if(assertN(endPos, nrow, ncol)){
					if(outBuf[posN]>0 && outBuf[posN]<outBuf[curPos]){
						if(assertS(endPos, nrow, ncol)){
							if(outBuf[posS]>0 && outBuf[posS]<outBuf[curPos]){ //(N, S)
								if(outBuf[posN]<outBuf[posS])
									realCurPos = posS;
								else
									realCurPos = posN;
							}
							else if(outBuf[posSE]>0 && outBuf[posSE]<outBuf[curPos]){ //(N, SE)
								if(outBuf[posN]<outBuf[posSE])
									realCurPos = posSE;
								else
									realCurPos = posN;
							}
						}
					}
					else if(outBuf[posNE]>0 && outBuf[posNE]<outBuf[curPos]){
						if(assertS(endPos, nrow, ncol)){
							if(outBuf[posS]>0 && outBuf[posS]<outBuf[curPos]){ //(S, NE)
								if(outBuf[posNE]>outBuf[posS])
									realCurPos = posNE;
								else
									realCurPos = posS;
							}
							else if(outBuf[posSE]>0 && outBuf[posSE]<outBuf[curPos]){ //(NE, SE)
								if(outBuf[posNE]<outBuf[posSE])
									realCurPos = posSE;
								else
									realCurPos = posNE;
							}
						}
					}
				}
				break;
			case 4: //SE, should check E and S
				if(outBuf[posS]>0 && outBuf[posE]>0 && outBuf[posS]<outBuf[curPos] && outBuf[posE]<outBuf[curPos])
					if(outBuf[posS]>outBuf[posE])
						realCurPos = posS;
					else
						realCurPos = posE;
				break;
			case 8: //S, should check (E, W), (E, SW), (W, SE), (SW, SE)
				if(assertE(endPos, nrow, ncol)){
					if(outBuf[posE]>0 && outBuf[posE]<outBuf[curPos]){
						if(assertW(endPos, nrow, ncol)){
							if(outBuf[posW]>0 && outBuf[posW]<outBuf[curPos]){ //(E, W)
								if(outBuf[posE]<outBuf[posW])
									realCurPos = posW;
								else
									realCurPos = posE;
							}
							else if(outBuf[posSW]>0 && outBuf[posSW]<outBuf[curPos]){ //(E, SW)
								if(outBuf[posE]<outBuf[posSW])
									realCurPos = posSW;
								else
									realCurPos = posE;
							}
						}
					}
					else if(outBuf[posSE]>0 && outBuf[posSE]<outBuf[curPos]){
						if(assertW(endPos, nrow, ncol)){
							if(outBuf[posW]>0 && outBuf[posW]<outBuf[curPos]){ //(W, SE)
								if(outBuf[posSE]>outBuf[posW])
									realCurPos = posSE;
								else
									realCurPos = posW;
							}
							else if(outBuf[posSW]>0 && outBuf[posSW]<outBuf[curPos]){ //(SE, SW)
								if(outBuf[posSE]<outBuf[posSW])
									realCurPos = posSW;
								else
									realCurPos = posSE;
							}
						}
					}
				}
				break;
			case 16: //SW, should check W and S
				if(outBuf[posW]>0 && outBuf[posS]>0 && outBuf[posW]<outBuf[curPos] && outBuf[posS]<outBuf[curPos])
					if(outBuf[posW]>outBuf[posS])
						realCurPos = posW;
					else
						realCurPos = posS;
				break;
			case 32: //W, should check (S, N), (S, NW), (N, SW), and (NW, SW)
				if(assertN(endPos, nrow, ncol)){
					if(outBuf[posN]>0 && outBuf[posN]<outBuf[curPos]){
						if(assertS(endPos, nrow, ncol)){
							if(outBuf[posS]>0 && outBuf[posS]<outBuf[curPos]){ //(N, S)
								if(outBuf[posN]<outBuf[posS])
									realCurPos = posS;
								else
									realCurPos = posN;
							}
							else if(outBuf[posSW]>0 && outBuf[posSW]<outBuf[curPos]){ //(N, SW)
								if(outBuf[posN]<outBuf[posSW])
									realCurPos = posSW;
								else
									realCurPos = posN;
							}
						}
					}
					else if(outBuf[posNW]>0 && outBuf[posNW]<outBuf[curPos]){
						if(assertS(endPos, nrow, ncol)){
							if(outBuf[posS]>0 && outBuf[posS]<outBuf[curPos]){ //(S, NW)
								if(outBuf[posNW]>outBuf[posS])
									realCurPos = posNW;
								else
									realCurPos = posS;
							}
							else if(outBuf[posSW]>0 && outBuf[posSW]<outBuf[curPos]){ //(NW, SW)
								if(outBuf[posNW]<outBuf[posSW])
									realCurPos = posSW;
								else
									realCurPos = posNW;
							}
						}
					}
				}
				break;
			case 64: //NW, should check N and W
				if(outBuf[posN]>0 && outBuf[posW]>0 && outBuf[posN]<outBuf[curPos] && outBuf[posW]<outBuf[curPos])
					if(outBuf[posN]>outBuf[posW])
						realCurPos = posN;
					else
						realCurPos = posW;
				break;
			case 128: //N, should check (E, W), (E, NW), (W, NE), and (NW, NE)
				if(assertE(endPos, nrow, ncol)){
					if(outBuf[posE]>0 && outBuf[posE]<outBuf[curPos]){
						if(assertW(endPos, nrow, ncol)){
							if(outBuf[posW]>0 && outBuf[posW]<outBuf[curPos]){ //(E, W)
								if(outBuf[posE]<outBuf[posW])
									realCurPos = posW;
								else
									realCurPos = posE;
							}
							else if(outBuf[posNW]>0 && outBuf[posNW]<outBuf[curPos]){ //(E, NW)
								if(outBuf[posE]<outBuf[posNW])
									realCurPos = posNW;
								else
									realCurPos = posE;
							}
						}
					}
					else if(outBuf[posNE]>0 && outBuf[posNE]<outBuf[curPos]){
						if(assertW(endPos, nrow, ncol)){
							if(outBuf[posW]>0 && outBuf[posW]<outBuf[curPos]){ //(W, NE)
								if(outBuf[posNE]>outBuf[posW])
									realCurPos = posNE;
								else
									realCurPos = posW;
							}
							else if(outBuf[posNW]>0 && outBuf[posNW]<outBuf[curPos]){ //(NE, NW)
								if(outBuf[posNE]<outBuf[posNW])
									realCurPos = posNW;
								else
									realCurPos = posNE;
							}
						}
					}
				}
				break;
			}
		}

		//assign code to the path
		IntNode* temp = flowPath->first;
		while(temp!=NULL){
			outBuf[temp->pos] = outBuf[realCurPos];
			temp = temp->next;
		}
		delete flowPath;
	}

	//combine partitions w/ similar aspects
	if(theAngle>0)
		combinePartitions(outBuf, streamlineBuf, flowDirBuf, nrow, ncol, theAngle);

	//assign the streamlines to be 0 and bisecting partitions that are at two sides of the same link
	unsigned char* idBuf = new unsigned char[nrow*ncol];
	for(i = 0; i<nrow*ncol; i++){
		if(streamlineBuf[i] != 0){
			outBuf[i] = 0;
			idBuf[i] = 4;
		}
		else
			idBuf[i] = 0;
	}
	int code = 0;
	for(i = 0; i<nrow*ncol; i++){
		if(idBuf[i] == 0){
			code++;
			Patch<float>* thePatch = new Patch<float>(i, outBuf, idBuf, nrow, ncol, (float)0);
			thePatch->assignValue(outBuf, code);
			delete thePatch;
		}
	}

	return 0;
}


void wetnessIndexSingleDirection(float* inBuf, 
								 float* outBuf, 
								 int nrow, 
								 int ncol, 
								 int neighborhood, 
								 float nodata){

}

void wetnessIndexMultiDirection(float* inBuf, 
								float* outBuf, 
								int nrow, 
								int ncol, 
								int neighborhood, 
								float nodata){

}

/*************************************
	UTILITY FUNCTIONS
 
	Content:

	addArea    (used by flowAccumulation)
	combinePartitions (used by terrainPartition)
	decreaseMatrixValue (used by dividelines)
	fillSink   (used by fillSinks)
	filling (filling a sink, used by fillSink)
	findStreamSectionEnd (used by testStreamSections)
	findStreamSectionHead (used by testStreamSections)
	flowInLink(used by combinePartitions)
	haveNext (used by dividelines)
	neighborhoodValues (return the 9 values of a neighborhood)
	nextFlowPixel (used by headwater)
	setNext (used by dividelines)

*************************************/

int addArea(int curPos, float* flowDir, float* flowAcc, float unitArea, int nrow, int ncol){
	int posCode = flowDir[curPos];
	switch(posCode){
	case(1):
		flowAcc[curPos-ncol+1] += unitArea;
		return curPos-ncol+1;
	case(2):
		flowAcc[curPos+1] += unitArea;
		return curPos+1;
	case(4):
		flowAcc[curPos+ncol+1] += unitArea;
		return curPos+ncol+1;
	case(8):
		flowAcc[curPos+ncol] += unitArea;
		return curPos+ncol;
	case(16):
		flowAcc[curPos+ncol-1] += unitArea;
		return curPos+ncol-1;
	case(32):
		flowAcc[curPos-1] += unitArea;
		return curPos-1;
	case(64):
		flowAcc[curPos-ncol-1] += unitArea;
		return curPos-ncol-1;
	case(128):
		flowAcc[curPos-ncol] += unitArea;
		return curPos-ncol;
	default:
		return -1;
	}
	return -1;
}

void bilinearWgtCir(float neighborhood, float cellsize, int nrow, int ncol, double para[]){
	if(neighborhood/cellsize>nrow)
		neighborhood = cellsize*nrow;
	if(neighborhood/cellsize>ncol)
		neighborhood = cellsize*ncol;
	para[0] = neighborhood/cellsize/2; //number of cellsizes between the center of the current cell and the boundary of the neighborhood
	para[1] = neighborhood/cellsize/2.828427; //num of sqrt(2)cellsize between the center of the current cell and and bnd of the neighborhood
	para[2] = para[0]-floor(para[0]); //weight1 in cardinal dir
	para[3] = 1-para[2];
	para[4] = para[1]-floor(para[1]); //weight1 in diaganal dir
	para[5] = 1-para[4];
}

void bilinearWgtSqr(float neighborhood, float cellsize, int nrow, int ncol, double para[]){
	if(neighborhood/cellsize>nrow)
		neighborhood = cellsize*nrow;
	if(neighborhood/cellsize>ncol)
		neighborhood = cellsize*ncol;
	para[0] = neighborhood/cellsize/2; //number of cellsizes between the center of the current cell and the boundary of the neighborhood
	para[1] = para[0]-floor(para[0]); //weight1 in cardinal dir
	para[2] = 1-para[1];
}

//used by terrainPartition
void combinePartitions(float* terrainPartitionBuf, float* streamlineBuf, float* flowDirBuf, int nrow, int ncol, float theAngle){
	float* copyBuf = new float[nrow*ncol];
	bool* idBuf = new bool[nrow*ncol];
	for(int i = 0; i<nrow*ncol; i++){
		if(streamlineBuf[i]>0)
			idBuf[i] = true;
		else
			idBuf[i] = false;
		copyBuf[i] = terrainPartitionBuf[i];
	}
	for(i = 0; i<nrow*ncol; i++){
		if(flowDirBuf[i] == 0){ //end of streamline(s)
			Tree<FFINode>* streamNet;
			while(true){ //Maybe more than one stream flow into this end
				streamNet = new Tree<FFINode>; //build a tree of the stream network.
				int flowInPos = flowInLink(flowDirBuf, idBuf, nrow, ncol, i);
				if(flowInPos<0) break; //no more stream flowing into this end
				idBuf[flowInPos] = false;
				streamNet->addHead(new FFINode(streamlineBuf[flowInPos], 0, flowInPos));
				streamNet->thisList->addTail(streamNet->head);
				while(streamNet->thisList->first != NULL){
					int head = findStreamSectionHead(flowDirBuf, streamlineBuf, nrow, ncol, (int)streamlineBuf[streamNet->thisList->first->i1]);
					int headRow = head/ncol; 
					int headCol = head - ncol*headRow; 
					int endRow = streamNet->thisList->first->i1/ncol;
					int endCol = streamNet->thisList->first->i1 - ncol*endRow;
					double tan = (float)(headRow-endRow)/((float)(headCol-endCol)+VERYSMALL);
					if(tan>0)
						streamNet->thisList->first->f2 = atan(tan);
					else
						streamNet->thisList->first->f2 = -atan(-tan);

					while(true){ //there are more than one stream flow into this stream
						flowInPos = flowInLink(flowDirBuf, idBuf, nrow, ncol, head);
						if(flowInPos<0) break; //no more stream flowing into this end
						idBuf[flowInPos] = false;
						FFINode* newLink = new FFINode(streamlineBuf[flowInPos],0,flowInPos);
						streamNet->thisList->first->children->addTail(newLink);
						streamNet->thisList->addTail(newLink);
					}
					streamNet->thisList->first = streamNet->thisList->first->next;
				}
				streamNet->thisList->release();
				streamNet->thisList->addTail(streamNet->head);
				while(streamNet->thisList->first != NULL){
					streamNet->thisList->addTail(streamNet->thisList->first->children);
					FFINode* temp = streamNet->thisList->first->children->first;
					while(temp!=NULL && // 1. make sure the current section does have children
						temp!=streamNet->thisList->first->children->last->next){ //2. make sure the operation is performed on the //children of the current node (since thisList links all the sections, children->last->next points to the children of next node).
						if(temp->children->first != NULL){	//3. make sure the children is not an external link
							float anglediff = fabs(streamNet->thisList->first->f2-temp->f2)*180/3.141592654;
							if(anglediff<theAngle){ //combine the two partitions
								float parentValue = 0;
								float childValue = 0;
								for(int j = 0; j<nrow*ncol; j++){
									if((int)copyBuf[j]==(int)(streamNet->thisList->first->f1)){
										parentValue = terrainPartitionBuf[j];
										break;
									}
								}
								for(j=0; j<nrow*ncol; j++){
									if((int)copyBuf[j]==(int)(temp->f1)){
										childValue = terrainPartitionBuf[j];
										break;
									}
								}
								for(j = 0; j<nrow*ncol; j++){
									if((int)terrainPartitionBuf[j]==(int)childValue)
										terrainPartitionBuf[j] = parentValue;
								}
							}
						}
						temp = temp->next;
					}
					streamNet->thisList->first = streamNet->thisList->first->next;
				}
				delete streamNet;
			}
		}
	}
	delete [] idBuf;
	delete [] copyBuf;
}

//used by dividelines
void decreaseMatrixValue(Patch<float>* thePatch, int pos){
	int nrow = thePatch->nrow;
	int ncol = thePatch->ncol;

	thePatch->setMatrixValue(pos, 0);
	
	int matrixValue = thePatch->getMatrixValue(pos-ncol-1);
	if(matrixValue>0)
		thePatch->setMatrixValue(pos-ncol-1, matrixValue-1);

	matrixValue = thePatch->getMatrixValue(pos-ncol);
	if(matrixValue>0)
		thePatch->setMatrixValue(pos-ncol, matrixValue-1);
	
	matrixValue = thePatch->getMatrixValue(pos-1);
	if(matrixValue>0)
		thePatch->setMatrixValue(pos-1, matrixValue-1);
	
	matrixValue = thePatch->getMatrixValue(pos+1);
	if(matrixValue>0)
		thePatch->setMatrixValue(pos+1, matrixValue-1);
	
	matrixValue = thePatch->getMatrixValue(pos+ncol-1);
	if(matrixValue>0)
		thePatch->setMatrixValue(pos+ncol-1, matrixValue-1);
	
	matrixValue = thePatch->getMatrixValue(pos+ncol+1);
	if(matrixValue>0)
		thePatch->setMatrixValue(pos+ncol+1, matrixValue-1);
}

//a function used by fillSink
template<class T>
int filling(Patch<T>* thePatch, float* DEMBuf, float* outBuf){
	if(thePatch->first == NULL) return 1;
	if(DEMBuf == NULL) return 2;
	if(outBuf == NULL) return 3;
	PatchPixel* temp = thePatch->first;
	while(temp!=NULL){
		if(DEMBuf[temp->pos]<=thePatch->lowestEdge)
			outBuf[temp->pos] = thePatch->lowestEdge;
		temp=temp->nextPixel;
	}
	return 0;
}

//a function used by fillDEM
int fillSink(float* DEMBuf,	//Input DEM
			  float* cachBuf,	//catchment data
			  float* outBuf,	//result
			  float threshold,
			  int nrow, 
			  int ncol, 
			  float nodata){
	int numPixels = nrow*ncol;
	int i = 0;
	unsigned char* IDBuf = new unsigned char[numPixels];
	for(i=0; i<numPixels; i++){
		outBuf[i] = DEMBuf[i];
		if(fabs(outBuf[i]-nodata)<VERYSMALL)
			IDBuf[i] = 2;
		else
			IDBuf[i] = 0;
	}

	int numOfSinks = 0;
	for(i=0; i<numPixels; i++){
		if(IDBuf[i]==0){
			Patch<float>* thePatch;
			Patch<float>* thePatch2;
			thePatch = new Patch<float>(i, cachBuf, IDBuf, nrow, ncol, true);
			while(thePatch != NULL){
				thePatch->findBorder(false);
				float minDiff = thePatch->findMinDiff(DEMBuf, nodata);
				if(minDiff>0 && minDiff<threshold){ //if minDiff==0, bottom and pour point are at the same location and this is the only situation that lowestPourPosNext will be -1.
					if(IDBuf[thePatch->lowestPourPosNext]==0){
						thePatch2 = new Patch<float>(thePatch->lowestPourPosNext, cachBuf, IDBuf, nrow, ncol, true);
						thePatch2->findBorder(false);
						float minDiff2 = thePatch2->findMinDiff(DEMBuf, nodata);
						if(minDiff2>0 && minDiff2<threshold){
							if(thePatch->value == cachBuf[thePatch2->lowestPourPosNext]){ //if share the lowest pour point
								if(thePatch->bottom>thePatch2->bottom){ //fill the higher one
									if(filling(thePatch, DEMBuf, outBuf) == 0)
										numOfSinks++;
								}
								else if(thePatch->bottom<thePatch2->bottom){
									if(filling(thePatch2, DEMBuf, outBuf) == 0)
										numOfSinks++;
								}
								else{ //if equal fill the smaller one
									if(thePatch->numPixels<=thePatch2->numPixels){
										if(filling(thePatch, DEMBuf, outBuf) == 0)
											numOfSinks++;
									}
									else{
										if(filling(thePatch2, DEMBuf, outBuf) == 0)
											numOfSinks++;
									}
								}
								PatchPixel* temp = thePatch2->first; //label the two cash as one
								while(temp!=NULL){
									cachBuf[temp->pos] = thePatch->value;
									IDBuf[temp->pos] = 0;
									temp=temp->nextPixel;
								}
								temp = thePatch->first;
								while(temp!=NULL){
									IDBuf[temp->pos] = 0;
									temp=temp->nextPixel;
								}
								delete thePatch;
								delete thePatch2;
								thePatch = new Patch<float>(i, cachBuf, IDBuf, nrow, ncol, true);
							} //end if share lowest pour point
							else{ //if not share lowest pour point, fill thePatch, set thePatch2 to be thePatch
								if(filling(thePatch, DEMBuf, outBuf) == 0)
									numOfSinks++;
								delete thePatch;
								thePatch = thePatch2;
							}
						} //end if thePatch2 is a shallow one
						else{ // if thePatch2 if not a shallow one, fill thePatch, move on to next patch
							if(filling(thePatch, DEMBuf, outBuf) == 0)
								numOfSinks++;
							delete thePatch;
							delete thePatch2;
							thePatch = NULL;
						}
					} //end of testing if there is an adjacent untested cash
					else{ //if thePatch is an isolated one, fill it, move on to next patch
						if(filling(thePatch, DEMBuf, outBuf) == 0)
							numOfSinks++;
						delete thePatch;
						thePatch = NULL;
					}
				} //end of testing if thePatch is a shallow one.
				else{ //if thePatch is not a shallow one, move on to the next patch.
					delete thePatch;
					thePatch = NULL;
				}
			} //end of while thePatch!=NULL
		}
	}
	delete [] IDBuf;
	return numOfSinks;
}

//used by testStreamSections
int findStreamSectionEnd(float* flowDir, float* streamlineBuf, int nrow, int ncol, int streamCode){
	int curPos = -1;
	for(int i=0; i<nrow*ncol; i++){
		if((int)streamlineBuf[i]==streamCode){
			curPos = i;
			while(flowDir[curPos] != 0 && streamlineBuf[curPos]==streamCode){
				curPos = nextFlowPixel(curPos, flowDir, ncol);
			}
			break;
		}
	}
	return curPos;
}

//used by testStreamSections and prunBranches
int findStreamSectionHead(float* flowDir, float* streamlineBuf, int nrow, int ncol, int streamCode){
//64 128 1
//32  0  2
//16  8  4
	int curPos = -1;
	for(int i=0; i<nrow*ncol; i++){
		if((int)streamlineBuf[i]==streamCode){
			curPos = i;
			int tempIndex = -1;
			while(true){
				if(assertNW(curPos, nrow, ncol)){
					tempIndex = getNW(curPos, ncol);
					if((int)flowDir[tempIndex] == 4 && (int)streamlineBuf[tempIndex]==streamCode){
						curPos = tempIndex;
						continue;
					}
				}
				if(assertN(curPos, nrow, ncol)){ 
					tempIndex = getN(curPos, ncol);
					if((int)flowDir[tempIndex] == 8 && (int)streamlineBuf[tempIndex]==streamCode){
						curPos = tempIndex;
						continue;
					}
				}
				if(assertNE(curPos, nrow, ncol)){
					tempIndex = getNE(curPos, ncol);
					if((int)flowDir[tempIndex] == 16 && (int)streamlineBuf[tempIndex]==streamCode){
						curPos = tempIndex;
						continue;
					}
				}
				if(assertW(curPos, nrow, ncol)){
					tempIndex = getW(curPos, ncol);
					if((int)flowDir[tempIndex] == 2 && (int)streamlineBuf[tempIndex]==streamCode){
						curPos = tempIndex;
						continue;
					}
				}
				if(assertE(curPos, nrow, ncol)){
					tempIndex = getE(curPos, ncol);
					if((int)flowDir[tempIndex] == 32 && (int)streamlineBuf[tempIndex]==streamCode){
						curPos = tempIndex;
						continue;
					}
				}
				if(assertSW(curPos, nrow, ncol)){
					tempIndex = getSW(curPos, ncol);
					if((int)flowDir[tempIndex] == 1 && (int)streamlineBuf[tempIndex]==streamCode){
						curPos = tempIndex;
						continue;
					}
				}
				if(assertS(curPos, nrow, ncol)){
					tempIndex = getS(curPos, ncol);
					if((int)flowDir[tempIndex] == 128 && (int)streamlineBuf[tempIndex]==streamCode){
						curPos = tempIndex;
						continue;
					}
				}
				if(assertSE(curPos, nrow, ncol)){ 
					tempIndex = getSE(curPos, ncol);
					if((int)flowDir[tempIndex] == 64 && (int)streamlineBuf[tempIndex]==streamCode){
						curPos = tempIndex;
						continue;
					}
				}
				break;
			}
			break;
		}
	}
	return curPos;
}

//used by combinePartitions and prunBranches
int flowInLink(float* flowDirBuf, bool* idBuf, int nrow, int ncol, int pos){
	//return the end position of a stream that flows into pos.  Do clockwise and return the first 
	//found position whose id is true in idBuf.
	if(assertNW(pos, nrow, ncol)){
		int candidate = getNW(pos, ncol);
		if(idBuf[candidate]&&flowDirBuf[candidate]==4)
			return candidate;
	}
	if(assertN(pos, nrow, ncol)){
		int candidate = getN(pos, ncol);
		if(idBuf[candidate]&&flowDirBuf[candidate]==8)
			return candidate;
	}
	if(assertNE(pos, nrow, ncol)){
		int candidate = getNE(pos, ncol);
		if(idBuf[candidate]&&flowDirBuf[candidate]==16)
			return candidate;
	}
	if(assertW(pos, nrow, ncol)){
		int candidate = getW(pos, ncol);
		if(idBuf[candidate]&&flowDirBuf[candidate]==2)
			return candidate;
	}
	if(assertE(pos, nrow, ncol)){
		int candidate = getE(pos, ncol);
		if(idBuf[candidate]&&flowDirBuf[candidate]==32)
			return candidate;
	}
	if(assertSW(pos, nrow, ncol)){
		int candidate = getSW(pos, ncol);
		if(idBuf[candidate]&&flowDirBuf[candidate]==1)
			return candidate;
	}
	if(assertS(pos, nrow, ncol)){
		int candidate = getS(pos, ncol);
		if(idBuf[candidate]&&flowDirBuf[candidate]==128)
			return candidate;
	}
	if(assertSE(pos, nrow, ncol)){
		int candidate = getSE(pos, ncol);
		if(idBuf[candidate]&&flowDirBuf[candidate]==64)
			return candidate;
	}
	return -1; //no candidate found
}

//used by dividelines
bool haveNext(Patch<float>* thePatch, int curPos, int level){
	int nrow = thePatch->nrow;
	int ncol = thePatch->ncol;

	if(thePatch->getMatrixValue(curPos-ncol-1)==level) return true;
	if(thePatch->getMatrixValue(curPos-ncol)==level) return true;
	if(thePatch->getMatrixValue(curPos-ncol+1)==level) return true;
	if(thePatch->getMatrixValue(curPos-1)==level) return true;
	if(thePatch->getMatrixValue(curPos+1)==level) return true;
	if(thePatch->getMatrixValue(curPos+ncol-1)==level) return true;
	if(thePatch->getMatrixValue(curPos+ncol)==level) return true;
	if(thePatch->getMatrixValue(curPos+ncol+1)==level) return true;

	return false;
}

//used by dividelines
int setNext(Patch<float>* thePatch, float* DEMBuf, int curPos, int level){
	int nrow = thePatch->nrow;
	int ncol = thePatch->ncol;
	float highest = 0;
	int nextPos = -1;
	int tempPos = -1;
	tempPos = curPos-ncol-1;
	if(thePatch->getMatrixValue(tempPos)==level)
		if(DEMBuf[tempPos]>highest){
			highest = DEMBuf[tempPos];
			nextPos = tempPos;
		}
	tempPos = curPos-ncol;
	if(thePatch->getMatrixValue(tempPos)==level)
		if(DEMBuf[tempPos]>highest){
			highest = DEMBuf[tempPos];
			nextPos = tempPos;
		}
	tempPos = curPos-ncol+1;
	if(thePatch->getMatrixValue(tempPos)==level)
		if(DEMBuf[tempPos]>highest){
			highest = DEMBuf[tempPos];
			nextPos = tempPos;
		}
	tempPos = curPos-1;
	if(thePatch->getMatrixValue(tempPos)==level)
		if(DEMBuf[tempPos]>highest){
			highest = DEMBuf[tempPos];
			nextPos = tempPos;
		}
	tempPos = curPos+1;
	if(thePatch->getMatrixValue(tempPos)==level)
		if(DEMBuf[tempPos]>highest){
			highest = DEMBuf[tempPos];
			nextPos = tempPos;
		}
	tempPos = curPos+ncol-1;
	if(thePatch->getMatrixValue(tempPos)==level)
		if(DEMBuf[tempPos]>highest){
			highest = DEMBuf[tempPos];
			nextPos = tempPos;
		}
	tempPos = curPos+ncol;
	if(thePatch->getMatrixValue(tempPos)==level)
		if(DEMBuf[tempPos]>highest){
			highest = DEMBuf[tempPos];
			nextPos = tempPos;
		}
	tempPos = curPos+ncol+1;
	if(thePatch->getMatrixValue(tempPos)==level)
		if(DEMBuf[tempPos]>highest){
			highest = DEMBuf[tempPos];
			nextPos = tempPos;
		}
	return nextPos;
}

int neighborhoodValues(float* dataBuf, int nrow, int ncol, int pos, int NumCellsize, int dNumCellsize, 
				double wc1, double wc2, double wd1, double wd2, double result[], float nodata){
	//find out the 8 values at the edges of a user-specified neighborhood in a circular shape.
	//used by slope gradient and other functions that need information in a window.
	//use bilinear function to derive values.
	//result is a 9-element array
	//Xun, Jan. 14, 2004

	if(dataBuf == NULL) return 1;
	if(result == NULL) return 2;
	result[0] = dataBuf[pos];

	//For get the exact neighborhood, use bilinear algo to do interpolation first.
	int row = pos/ncol; 
	int col = pos%ncol;

	int ni = pos-NumCellsize*ncol; //north inner pixel number
	if(ni<0) ni = col;
	int no = ni-ncol; //north outter pixel
	if(no<0) no = ni;

	int si = pos+NumCellsize*ncol; //south inner pixel number
	if(si>nrow*ncol-1) si = (nrow-1)*ncol+col;
	int so = si+ncol; //south outter pixel
	if(so>nrow*ncol-1) so = si;

	int wi = pos-NumCellsize; //west inner pixel number
	int wo = wi-1; //west outter pixel
	if(col<=NumCellsize){
		wi = pos-col;
		wo = wi;
	}

	int ei = pos+NumCellsize; //east inner pixel number
	int eo = ei+1; //east outter pixel
	if(ncol-col-1<=NumCellsize){
		ei = pos+ncol-col-1;
		eo = ei;
	}

	int i = row-dNumCellsize; //northwest value
	if(i<0) i = 0;		//check north
	int p = i -1;
	if(p<0) p = 0;
	int j = col-dNumCellsize; //check west
	if(j<0) j = 0;
	int q = j - 1;
	if(q<0) q = 0;
	int nwse = i*ncol+j;
	int nwne = p*ncol+j;
	int nwsw = i*ncol+q;
	int nwnw = p*ncol+q;

	j = col+dNumCellsize; //northeast value
	if(j>ncol-1) j = ncol-1; //check east
	q = j + 1;
	if(q>ncol-1) q = j;
	int nesw = i*ncol+j;
	int nenw = p*ncol+j;
	int nese = i*ncol+q;
	int nene = p*ncol+q;

	i = row+dNumCellsize; //southeast value
	if(i>nrow-1) i = nrow-1; //check south
	p = i + 1;
	if(p>nrow-1) p = i;
	int senw = i*ncol+j;
	int sesw = p*ncol+j;
	int sene = i*ncol+q;
	int sese = p*ncol+q;

	j = col-dNumCellsize; //southwest value
	if(j<0) j = 0;  //check west
	q = j - 1;
	if(q<0) q = 0;
	int swne = i*ncol+j;
	int swse = p*ncol+j;
	int swnw = i*ncol+q;
	int swsw = p*ncol+q;

	// 1 2 3
	// 8 0 4
	// 7 6 5
	//values in cardinal directions
	float vNi = dataBuf[ni];
	float vNo = dataBuf[no];
	float vEi = dataBuf[ei];
	float vEo = dataBuf[eo];
	float vSi = dataBuf[si];
	float vSo = dataBuf[so];
	float vWi = dataBuf[wi];
	float vWo = dataBuf[wo];
	if(fabs(vNi-nodata)<VERYSMALL && fabs(vNo-nodata)<VERYSMALL){
		for(int t = 0; t<NumCellsize; t++)
			if(fabs(dataBuf[ni+(t+1)*ncol]-nodata)>VERYSMALL){
				vNi = dataBuf[ni+(t+1)*ncol];
				vNo = vNi;
			}
	}
	else if(fabs(vNi-nodata)<VERYSMALL)
		vNi = vNo;
	else if(fabs(vNo-nodata)<VERYSMALL)
		vNo = vNi;

	if(fabs(vEi-nodata)<VERYSMALL && fabs(vEo-nodata)<VERYSMALL){
		for(int t = 0; t<NumCellsize; t++)
			if(fabs(dataBuf[ei-(t+1)]-nodata)>VERYSMALL){
				vEi = dataBuf[ei-(t+1)];
				vEo = vEi;
			}
	}
	else if(fabs(vEi-nodata)<VERYSMALL)
		vEi = vEo;
	else if(fabs(vEo-nodata)<VERYSMALL)
		vEo = vEi;

	if(fabs(vSi-nodata)<VERYSMALL && fabs(vSo-nodata)<VERYSMALL){
		for(int t = 0; t<NumCellsize; t++)
			if(fabs(dataBuf[si-(t+1)*ncol]-nodata)>VERYSMALL){
				vSi = dataBuf[si-(t+1)*ncol];
				vSo = vSi;
			}
	}
	else if(fabs(vSi-nodata)<VERYSMALL)
		vSi = vSo;
	else if(fabs(vSo-nodata)<VERYSMALL)
		vSo = vSi;

	if(fabs(vWi-nodata)<VERYSMALL && fabs(vWo-nodata)<VERYSMALL){
		for(int t = 0; t<NumCellsize; t++)
			if(fabs(dataBuf[wi+(t+1)]-nodata)>VERYSMALL){
				vWi = dataBuf[wi+(t+1)];
				vWo = vWi;
			}
	}
	else if(fabs(vWi-nodata)<VERYSMALL)
		vWi = vWo;
	else if(fabs(vWo-nodata)<VERYSMALL)
		vWo = vWi;
	result[2] = wc2*vNi+wc1*vNo;
	result[4] = wc2*vEi+wc1*vEo;
	result[6] = wc2*vSi+wc1*vSo;
	result[8] = wc2*vWi+wc1*vWo;

	//values in diagnal directions
	float vNWSE = dataBuf[nwse];
	float vNWNE = dataBuf[nwne];
	float vNWSW = dataBuf[nwsw];
	float vNWNW = dataBuf[nwnw];
	float vNESE = dataBuf[nese];
	float vNENE = dataBuf[nene];
	float vNESW = dataBuf[nesw];
	float vNENW = dataBuf[nenw];
	float vSESE = dataBuf[sese];
	float vSENE = dataBuf[sene];
	float vSESW = dataBuf[sesw];
	float vSENW = dataBuf[senw];
	float vSWSE = dataBuf[swse];
	float vSWNE = dataBuf[swne];
	float vSWSW = dataBuf[swsw];
	float vSWNW = dataBuf[swnw];
	if(fabs(vNWSE-nodata)<VERYSMALL && fabs(vNWNE-nodata)<VERYSMALL && fabs(vNWSW-nodata)<VERYSMALL && fabs(vNWNW-nodata)<VERYSMALL){
		int t = 0;
		while(t<nwse/ncol-row && t<nwse%ncol-col && t<dNumCellsize){
			if(fabs(dataBuf[nwse+(t+1)*ncol+(t+1)]-nodata)>VERYSMALL){
				vNWSE = dataBuf[nwse+(t+1)*ncol+(t+1)];
				vNWNE = vNWSE;
				vNWSW = vNWSE;
				vNWNW = vNWSE;
				break;
			}
			t++;
		}
		if(fabs(vNWSE-nodata)<VERYSMALL){
			vNWSE = dataBuf[pos];
			vNWNE = vNWSE;
			vNWSW = vNWSE;
			vNWNW = vNWSE;
		}
	}
	else{
		if(fabs(vNWSE-nodata)<VERYSMALL) vNWSE = handleDiagonalNodata(vNWNE, vNWSW, vNWNW, nodata);
		if(fabs(vNWNE-nodata)<VERYSMALL) vNWNE = handleDiagonalNodata(vNWSE, vNWNW, vNWSW, nodata);
		if(fabs(vNWSW-nodata)<VERYSMALL) vNWSW = handleDiagonalNodata(vNWNW, vNWSE, vNWNE, nodata);
		if(fabs(vNWNW-nodata)<VERYSMALL) vNWNW = handleDiagonalNodata(vNWNE, vNWSW, vNWSE, nodata);
	}

	if(fabs(vNESE-nodata)<VERYSMALL && fabs(vNENE-nodata)<VERYSMALL && fabs(vNESW-nodata)<VERYSMALL && fabs(vNENW-nodata)<VERYSMALL){
		int t = 0;
		while(t<nesw/ncol-row && t<nesw%ncol-col && t<dNumCellsize){
			if(fabs(dataBuf[nesw+(t+1)*ncol-(t+1)]-nodata)>VERYSMALL){
				vNESE = dataBuf[nesw+(t+1)*ncol-(t+1)];
				vNENE = vNESE;
				vNESW = vNESE;
				vNENW = vNESE;
				break;
			}
			t++;
		}
		if(fabs(vNESE-nodata)<VERYSMALL){
			vNESE = dataBuf[pos];
			vNENE = vNESE;
			vNESW = vNESE;
			vNENW = vNESE;
		}
	}
	else{
		if(fabs(vNESE-nodata)<VERYSMALL) vNESE = handleDiagonalNodata(vNENE, vNESW, vNENW, nodata);
		if(fabs(vNENE-nodata)<VERYSMALL) vNENE = handleDiagonalNodata(vNESE, vNENW, vNESW, nodata);
        if(fabs(vNESW-nodata)<VERYSMALL) vNESW = handleDiagonalNodata(vNENW, vNESE, vNENE, nodata);
        if(fabs(vNENW-nodata)<VERYSMALL) vNENW = handleDiagonalNodata(vNENE, vNESW, vNESE, nodata);
	}

	if(fabs(vSESE-nodata)<VERYSMALL && fabs(vSENE-nodata)<VERYSMALL && fabs(vSESW-nodata)<VERYSMALL && fabs(vSENW-nodata)<VERYSMALL){
		int t = 0;
		while(t<senw/ncol-row && t<senw%ncol-col && t<dNumCellsize){
			if(fabs(dataBuf[senw-(t+1)*ncol-(t+1)]-nodata)>VERYSMALL){
				vSESE = dataBuf[senw-(t+1)*ncol-(t+1)];
				vSENE = vSESE;
				vSESW = vSESE;
				vSENW = vSESE;
				break;
			}
			t++;
		}
		if(fabs(vSESE-nodata)<VERYSMALL){
			vSESE = dataBuf[pos];
			vSENE = vSESE;
			vSESW = vSESE;
			vSENW = vSESE;
		}
	}
	else{
		if(fabs(vSESE-nodata)<VERYSMALL) vSESE = handleDiagonalNodata(vSENE, vSESW, vSENW, nodata);
        if(fabs(vSENE-nodata)<VERYSMALL) vSENE = handleDiagonalNodata(vSESE, vSENW, vSESW, nodata);
        if(fabs(vSESW-nodata)<VERYSMALL) vSESW = handleDiagonalNodata(vSENW, vSESE, vSENE, nodata);
        if(fabs(vSENW-nodata)<VERYSMALL) vSENW = handleDiagonalNodata(vSENE, vSESW, vSESE, nodata);
	}

	if(fabs(vSWSE-nodata)<VERYSMALL && fabs(vSWNE-nodata)<VERYSMALL && fabs(vSWSW-nodata)<VERYSMALL && fabs(vSWNW-nodata)<VERYSMALL){
		int t = 0;
		while(t<swne/ncol-row && t<swne%ncol-col && t<dNumCellsize){
			if(fabs(dataBuf[swne-(t+1)*ncol+(t+1)]-nodata)>VERYSMALL){
				vSWSE = dataBuf[swne-(t+1)*ncol+(t+1)];
				vSWNE = vSWSE;
				vSWSW = vSWSE;
				vSWNW = vSWSE;
				break;
			}
			t++;
		}
		if(fabs(vSWSE-nodata)<VERYSMALL){
			vSWSE = dataBuf[pos];
			vSWNE = vSWSE;
			vSWSW = vSWSE;
			vSWNW = vSWSE;
		}
	}
	else{
		if(fabs(vSWSE-nodata)<VERYSMALL) vSWSE = handleDiagonalNodata(vSWNE, vSWSW, vSWNW, nodata);
        if(fabs(vSWNE-nodata)<VERYSMALL) vSWNE = handleDiagonalNodata(vSWSE, vSWNW, vSWSW, nodata);
        if(fabs(vSWSW-nodata)<VERYSMALL) vSWSW = handleDiagonalNodata(vSWNW, vSWSE, vSWNE, nodata);
        if(fabs(vSWNW-nodata)<VERYSMALL) vSWNW = handleDiagonalNodata(vSWNE, vSWSW, vSWSE, nodata);
	}
	result[1] = wd2*(wd2*vNWSE+wd1*vNWNE)+wd1*(wd2*vNWSW+wd1*vNWNW);
	result[3] = wd2*(wd2*vNESW+wd1*vNENW)+wd1*(wd2*vNESE+wd1*vNENE);
	result[5] = wd2*(wd2*vSENW+wd1*vSESW)+wd1*(wd2*vSENE+wd1*vSESE);
	result[7] = wd2*(wd2*vSWNE+wd1*vSWSE)+wd1*(wd2*vSWNW+wd1*vSWSW);
	return 0;
}

//used by headwater, terrainPartition,
int nextFlowPixel(int curPos, float* flowDir, int ncol){
	switch((int)flowDir[curPos]){
	case 1:
		return getNE(curPos,ncol);
	case 2:
		return getE(curPos, ncol);
	case 4:
		return getSE(curPos,ncol);
	case 8:
		return getS(curPos, ncol);
	case 16:
		return getSW(curPos,ncol);
	case 32:
		return getW(curPos, ncol);
	case 64:
		return getNW(curPos,ncol);
	case 128:
		return getN(curPos, ncol);
	case 0:
		return -1;
	default:
		return -2;
	}
}

//return the previous pixel that has the maximum flow acc.  used by headwater and stream
int previousMaxFlowPixel(int curPos, float* flowDir, float* flowAcc, bool* idBuf, int nrow, int ncol){
	int previous = -1;
	int tempIndex = -1;
	double max = 0;
	if(assertNW(curPos, nrow, ncol))
		if((int)flowDir[getNW(curPos, ncol)]==4)
			if(idBuf == NULL || idBuf[getNW(curPos, ncol)]){
				previous = getNW(curPos, ncol);
				max = flowAcc[previous];
			}
	if(assertN(curPos, nrow, ncol))
		if((int)flowDir[getN(curPos, ncol)]==8)
			if(idBuf == NULL || idBuf[getN(curPos, ncol)]){
				tempIndex = getN(curPos, ncol); 
				if(flowAcc[tempIndex]>max){
					max = flowAcc[tempIndex];
					previous = tempIndex;
				}
			}
	if(assertNE(curPos, nrow, ncol))
		if((int)flowDir[getNE(curPos, ncol)]==16)
			if(idBuf == NULL || idBuf[getNE(curPos, ncol)]){
				tempIndex = getNE(curPos, ncol); 
				if(flowAcc[tempIndex]>max){
					max = flowAcc[tempIndex];
					previous = tempIndex;
				}
			}
	if(assertW(curPos, nrow, ncol))
		if((int)flowDir[getW(curPos, ncol)]==2)
			if(idBuf == NULL || idBuf[getW(curPos, ncol)]){
				tempIndex = getW(curPos, ncol); 
				if(flowAcc[tempIndex]>max){
					max = flowAcc[tempIndex];
					previous = tempIndex;
				}
			}
	if(assertE(curPos, nrow, ncol))
		if((int)flowDir[getE(curPos, ncol)]==32)
			if(idBuf == NULL || idBuf[getE(curPos, ncol)]){
				tempIndex = getE(curPos, ncol); 
				if(flowAcc[tempIndex]>max){
					max = flowAcc[tempIndex];
					previous = tempIndex;
				}
			}
	if(assertSW(curPos, nrow, ncol))
		if((int)flowDir[getSW(curPos, ncol)]==1)
			if(idBuf == NULL || idBuf[getSW(curPos, ncol)]){
				tempIndex = getSW(curPos, ncol); 
				if(flowAcc[tempIndex]>max){
					max = flowAcc[tempIndex];
					previous = tempIndex;
				}
			}
	if(assertS(curPos, nrow, ncol))
		if((int)flowDir[getS(curPos, ncol)]==128)
			if(idBuf == NULL || idBuf[getS(curPos, ncol)]){
				tempIndex = getS(curPos, ncol); 
				if(flowAcc[tempIndex]>max){
					max = flowAcc[tempIndex];
					previous = tempIndex;
				}
			}
	if(assertSE(curPos, nrow, ncol))
		if((int)flowDir[getSE(curPos, ncol)]==64)
			if(idBuf == NULL || idBuf[getSE(curPos, ncol)]){
				tempIndex = getSE(curPos, ncol); 
				if(flowAcc[tempIndex]>max){
					max = flowAcc[tempIndex];
					previous = tempIndex;
				}
			}
	return previous;
}

float handleDiagonalNodata(float v1, float v2, float v3, float nodata){
	if(fabs(v1-nodata)>VERYSMALL) return v1;
	if(fabs(v2-nodata)>VERYSMALL) return v2;
	if(fabs(v3-nodata)>VERYSMALL) return v3;
	return nodata;
}



int MFD(float* DEMBuf, float* AccRes, int nrow, int ncol,float cellsize,float nodata,int FlowDistribType,double p){
    if(DEMBuf == NULL) return 1;
	if(AccRes == NULL) return 2;
	if(cellsize <= 0) return 3;   

	double *arrMFFractSum= new double[nrow*ncol];
    double *arrPara=new double[nrow*ncol];  

	MFDPreProcess(DEMBuf,nrow,ncol,cellsize,nodata,FlowDistribType,arrMFFractSum,arrPara,p); 
    MFDAccumCompute(DEMBuf,AccRes,nrow,ncol,cellsize,nodata,FlowDistribType,arrMFFractSum,arrPara);
	if(arrMFFractSum!=NULL)
	{
		delete []arrMFFractSum;
        arrMFFractSum=NULL;
	}
	if(arrPara!=NULL)
	{
		delete []arrPara;
		arrPara=NULL;
	}	
	return 0;


}

	

void MFDPreProcess(float* DEMBuf, int nrow, int ncol,float cellsize,float nodata,int FlowDistribType,double *arrMFFractSum,double *arrPara,double p)
{

	double slope ,dSum;
	int i,j;	

	if(FlowDistribType==0)  //MFD_Quinn
	{
		for(int y=0;y<nrow;y++)
			for(int x=0;x<ncol;x++)
			{
				if(DEMBuf[y*ncol+x] == nodata)
					arrMFFractSum[y*ncol+x]=0;
				else
			 {
				 dSum=0;
				 for(int k=0;k<8;k++)
				 {

					 i=x+ArrDir8X[k];
					 j=y+ArrDir8Y[k];  

					 if(IsValidXYSrcValue(i, j,nrow, ncol))
					 {
						 if(DEMBuf[y*ncol+x]>DEMBuf[j*ncol+i] )  //current elevation is higher than one neighborhood pixel
						 { /*
						   slope=0.5*(DEMBuf[y*ncol+x]- DEMBuf[j*ncol+i])/cellsize;  
						   if(k%2==0)   
						   slope=slope/sqrt(2.0);	
						   dSum = dSum + pow(slope, p);*/

							 slope =(DEMBuf[y*ncol+x]- DEMBuf[j*ncol+i])/cellsize; 
							 if(k%2==0)
							 {
								 slope = slope / sqrt(2.0);
								 dSum = dSum + pow(slope,p) * sqrt(2.0) / 4;
							 }
							 else
								 dSum = dSum + pow(slope,p) / 2;

						 }
					 }
				 }
				 if(dSum==0)   //no downslope
					 arrMFFractSum[y*ncol+x]=MAX_SINGLE;          
				 else 
					 arrMFFractSum[y*ncol+x] =dSum;
			 }

			 arrPara[y*ncol+x] = p;
			}			

			return;
	}



	/////////////////// calculate arrMFFractSum and arrPara for MFD-md /////////////////////////
	// get the regional scope of max among 8 neighbor cells	
	double dMin,dMax;     
	for(int y=0;y<nrow;y++)
		for(int x=0;x<ncol;x++)
		{		
			if(DEMBuf[y*ncol+x]!=nodata)
		 {
			 dMin =MAX_SINGLE;
			 dMax=0;
			 for(int k=0;k<8;k++)
			 {
				 i=x+ArrDir8X[k];
				 j=y+ArrDir8Y[k];
				 if(IsValidXYSrcValue(i, j,nrow, ncol))
				 {
					 if(DEMBuf[y*ncol+x]>DEMBuf[j*ncol+i] )   //current elevation is higher than one neighborhood pixel
					 {

						 slope = (DEMBuf[y*ncol+x]- DEMBuf[j*ncol+i]) /cellsize; 
						 if(k%2 == 0) 
							 slope = slope/sqrt(2.0);	
						 if(slope>dMax)
							 dMax = slope;
					 }
				 }
			 }

			 arrPara[y*ncol+x]= dMax;   //by max downslope
		 }  
			else
				arrPara[y*ncol+x] = 0;
		}


		double a,b;         
		a = 10; 
		b = 1.1;    
		for(int y=0;y<nrow;y++)
			for(int x=0;x<ncol;x++)
			{
				if(DEMBuf[y*ncol+x] == nodata)
					arrMFFractSum[y*ncol+i]=0;
				else
				{
					if(arrPara[y*ncol+x]<=0)
						arrPara[y*ncol+x]=1.1;
					else if(arrPara[y*ncol+x] >= 1)
						arrPara[y*ncol+x]= 10;
					else 
						arrPara[y*ncol+x]= (a-b) * arrPara[y*ncol+x] + b;	

					dSum = 0;
					for(int k=0;k<8;k++)
					{
						i=x+ArrDir8X[k];
						j=y+ArrDir8Y[k];
						if(IsValidXYSrcValue(i, j,nrow, ncol))
						{
							if(DEMBuf[y*ncol+x]>DEMBuf[j*ncol+i] )  
							{ 
								slope = (DEMBuf[y*ncol+x]- DEMBuf[j*ncol+i]) /cellsize;

								if(k%2 == 0)
								{
									slope = slope /sqrt(2.0);
									dSum = dSum +pow(slope,arrPara[y*ncol+x]) * sqrt(2.0) / 4;
								}
								else
									dSum = dSum + pow(slope,arrPara[y*ncol+x]) / 2;

							}
						}
					}
					if(dSum ==0) //no downslope
						arrMFFractSum[y*ncol+x] = MAX_SINGLE;
					else
						arrMFFractSum[y*ncol+x]  = dSum;
				}
			}


			float *par=new float[nrow*ncol];
			float *fra=new float[nrow*ncol];

			for(int y=0;y<nrow;y++)
				for(int x=0;x<ncol;x++)
				{ 
					par[y*ncol+x]= arrPara[y*ncol+x];
					fra[y*ncol+x] = arrMFFractSum[y*ncol+x];

				}

}




void MFDAccumCompute(float *DEMBuf,float *AccRes,int nrow,int ncol,float cellsize,float nodata,int FlowDistribType,double *arrMFFractSum,double *arrPara) 
{
	if(AccRes==NULL)
	{
	   AccRes=new float[nrow*ncol];

	   for(int y=0;y<nrow;y++)
	    for(int x=0;x<ncol;x++)
	    {
		 if(DEMBuf[y*ncol+x] == nodata)
			 AccRes[y*ncol+x]=nodata;
		 else
			 AccRes[y*ncol+x] =0;
	    }
	}



	for(int y=0;y<nrow;y++)
	   for(int x=0;x<ncol;x++)
	   {
		 
		  if(fabs(DEMBuf[y*ncol+x] -nodata) > VERYSMALL)
		  {
			 CheckNeighbor(x, y,DEMBuf,AccRes,nrow, ncol, cellsize,nodata, FlowDistribType,arrMFFractSum, arrPara);
             
		  }

		}

	 for(int y=0;y<nrow;y++)
	   for(int x=0;x<ncol;x++)
		     AccRes[y*ncol+x]=AccRes[y*ncol+x]*cellsize*cellsize;
			

		

}



//recursion for accumulation calculation
double CheckNeighbor(int i,int  j,float *DEMBuf,float *AccRes,int nrow,int ncol,float cellsize,float nodata,int FlowDistribType,double *arrMFFractSum,double *arrPara)
{
	
	double temp,dSlope;
	int x,y;
    if(AccRes[j*ncol+i] == nodata)
	  return 0;

	if(AccRes[j*ncol+i] <= 0)
	{
		 AccRes[j*ncol+i] = 1;
		 bool tag = true;
		 for(int k=0;k<8;k++)
		 {
			  x=i+ArrDir8X[k];
		      y=j+ArrDir8Y[k];
			  if(IsValidXYSrcValue(x, y,nrow, ncol))
			  {
			      if(DEMBuf[y*ncol+x] > DEMBuf[j*ncol+i])  //find higher pixel
			      {
					  tag = false;
				      if(arrMFFractSum[y*ncol+x] == MAX_SINGLE)
			             temp = 0;
				      else
				      {
					
						 dSlope = (DEMBuf[y*ncol+x] - DEMBuf[j*ncol+i])/cellsize;  
                          if(k%2 ==0) 
						 {
                            dSlope = dSlope / sqrt(2.0);
                            temp = pow(dSlope, arrPara[y*ncol+x]) * sqrt(2.0)/ (4 * arrMFFractSum[y*ncol+x]);
						 }
                         else
                            temp = pow(dSlope, arrPara[y*ncol+x]) / (2 * arrMFFractSum[y*ncol+x]);
                    
					
				       }
				 
				     if(temp > 0)
                        AccRes[j*ncol+i] = AccRes[j*ncol+i] + temp * CheckNeighbor(x,y,DEMBuf,AccRes,nrow, ncol, cellsize,FlowDistribType,nodata, arrMFFractSum, arrPara);
			      }
			  }
		 }

		 if(tag == true)
		 {
			 int m = 3;
		 }
	 }
	 return AccRes[j*ncol+i];


}


bool IsValidXYSrcValue(int i, int j,int nrow, int ncol)
{
	if(i<0||i>ncol-1||j<0||j>nrow-1)
		return false;
	else 
		return true;
}



bool FillDEM_PD(float *DEMBuf,float *vWf,int nrow,int ncol,float cellsize,float nodata,double dElevEps)
{
   //dElevEps = 0.001
   double dElevEps0 = dElevEps;
   double dElevEps1 = dElevEps * sqrt(2.0);
   int i,j, k;
   bool tag=true;

   int RowFrom[8] = {0, nrow- 1, 0, nrow - 1, 0, nrow - 1, 0, nrow - 1};
   int ColFrom[8] = {0, ncol - 1, ncol  - 1, 0,ncol - 1, 0, 0, ncol  - 1};
             

   bool boolSomeDone;

   for(int y=0;y<nrow;y++)
   {
	    vWf[y*ncol+0] = DEMBuf[y*ncol+0];
	    vWf[y*ncol+ncol-1] = DEMBuf[y*ncol+ncol-1];
   }
   for(int x=1;x<ncol-1;x++)
   {
	   vWf[0*ncol+x] = DEMBuf[0*ncol+x];
	   vWf[(nrow-1)*ncol+x] = DEMBuf[(nrow-1)*ncol+x];
	   for(y =1;y<nrow-1;y++) //the inner pixels get their values
	   {
		   if(DEMBuf[y*ncol+x] == nodata)
			   vWf[y*ncol+x] = nodata;
		   else
			   vWf[y*ncol+x] = MAX_SINGLE;
	   }
   }

  



   for(y=0; y<nrow;y++)
   {
	   if(DEMBuf[y*ncol+0] != nodata)
		   DryUpwardCell(0,y,DEMBuf,vWf,nrow,ncol,dElevEps0,dElevEps1);
   }
   for(y=0;y<nrow;y++)
   {
	   if(DEMBuf[y*ncol+ncol-1] != nodata)
		   DryUpwardCell(ncol-1,y,DEMBuf,vWf,nrow,ncol,dElevEps0,dElevEps1);
   }
   for(x=1;x<ncol-1;x++)
   {
	   if(DEMBuf[0*ncol+x] != nodata)
		   DryUpwardCell(x,0,DEMBuf,vWf,nrow,ncol,dElevEps0,dElevEps1);
   }
   for(x=1;x<ncol-1;x++)
   {
	   if(DEMBuf[(nrow-1)*ncol+x] != nodata)
		   DryUpwardCell(x,nrow-1,DEMBuf,vWf,nrow,ncol,dElevEps0,dElevEps1);
   }


   

   bool out;

   int IterationNum=0;

  do
  {
      for(int scan=0;scan<1;scan++)
      {
	     x=ColFrom[scan];
	     y=RowFrom[scan];
	     boolSomeDone = false;
	     if(vWf[y*ncol+x]!=nodata)
	     {
		    do
		    {
			   if(IsValidXYSrcValue(x,y,nrow,ncol)&&vWf[y*ncol+x] !=nodata&&vWf[y*ncol+x] > DEMBuf[y*ncol+x])
			   {
				
				   for(k=0;k<8;k++)
				   {
					   i = x+ArrDir8X[k];
					   j = y+ArrDir8Y[k];
					   dElevEps = (k%2 ==0?dElevEps1:dElevEps0);
					   if(IsValidXYSrcValue(i,j,nrow,ncol)&&vWf[j*ncol+i]!=nodata)
					   {					   
							  if(DEMBuf[y*ncol+x]>=vWf[j*ncol+i] + dElevEps&&vWf[y*ncol+x]>=vWf[j*ncol+i]+dElevEps)
							   {
								   vWf[y*ncol+x] = DEMBuf[y*ncol+x];
								   boolSomeDone = true;
								   DryUpwardCell(x,y,DEMBuf,vWf,nrow,ncol,dElevEps0,dElevEps1);
								   break;
								   //goto tag;
							   }
							 if(vWf[y*ncol+x]>vWf[j*ncol+i]+dElevEps)
							   {
								   vWf[y*ncol+x] = vWf[j*ncol+i] +dElevEps;
								   boolSomeDone = true;
							   }
						   
						}
				   }
			
			    }	

                tag: out=true;;		 

		     }while(NextCell(&x, &y, scan,nrow,ncol)==true);

			 if(boolSomeDone==false)	      
			   break;
	      }	     
	    
	   } 
	  IterationNum++;
  }while(boolSomeDone==true&&IterationNum<10000);

  if(IterationNum>=10000&&boolSomeDone==true)
	  return false;

  return true;

}

bool NextCell(int* x,int* y,int scan,int nrow,int ncol)
{	

   int DRow[8] = {0, 0, 1, -1, 0, 0, 1, -1};
   int DCol[8] = {1, -1, 0, 0, -1, 1, 0, 0};
   int RowTo[8] = {1, -1, - nrow + 1,  nrow - 1, 1, -1, - nrow + 1,  nrow- 1};
   int ColTo[8] = {-ncol + 1, ncol - 1, -1, 1, ncol - 1, -ncol + 1, 1, -1};

    *y = *y+DRow[scan];
	*x = *x+DCol[scan];
	if(*y<0||*x<0||*y>=nrow||*x>=ncol)
	{
		*y = *y+RowTo[scan];
		*x = *x+ColTo[scan];
		if(*y<0||*x<0||*y>=nrow||*x>=ncol)
			return false;
	}
	return true;
}


void DryUpwardCell(int x,int y,float *DEMBuf,float *vWf,int nrow,int ncol,double dElevEps0,double dElevEps1)
{
	int MAX_DEPTH = 2000;
	int i,j;
	double dElevEps;
	static int depth=0;  

    depth = depth +1; 
	if(depth>MAX_DEPTH)
	{
		depth = depth - 1;
		return;
	}


//	if(depth<MAX_DEPTH)
	//{
		 for(int k=0;k<8;k++)
		 {
			 i = x + ArrDir8X[k];
			 j = y + ArrDir8Y[k];
			 if(IsValidXYSrcValue(i, j, nrow,  ncol))
			 {
				 if(vWf[j*ncol+i] == MAX_SINGLE )
				 {
					 dElevEps = (k%2==0? dElevEps1:dElevEps0);
					 if(DEMBuf[j*ncol+i]>=vWf[y*ncol+x] + dElevEps)
					 {
						 vWf[j*ncol+i] = DEMBuf[j*ncol+i];
						 DryUpwardCell(i,j,DEMBuf,vWf,nrow,ncol,dElevEps0,dElevEps1);
					 }
				 }
			 }
		 }
	//}
	depth = depth - 1;
}

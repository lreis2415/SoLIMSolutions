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
//header file of terrain
//This is a library containing functions for terrain analysis.
//Written by Xun Shi
//created on Aug. 8, 2001
//last modified: Aug. 12, 2001
#pragma once

#include "utility.h"
#include "patch.h"


#define DEFAULTNEIGHBOR 100
#define Pi 3.1415926


/****************************
 	MAJOR FUNCTIONS

	Content:

	aspect
	back
	brdNrwRidge
	cachment
	curvature
	fillDEM
	shaveDEM
	flowAccumulation
	flowDirection
	fuzzyAspects
	gradient
	gradientShi
	headwater
	planformCurvature
	profileCurvature
	ridgelines
	ridgelineShi
	shoulder
	streamlineOCallaghan
	streamlinePeuker
	streamlineSkidmore
	terrainPatition
	wetnessIndexMultiDirection
	wetnessIndexSingleDirection

 ****************************/

int aspectCirEvans(float* DEMBuf, float* outBuf, int nrow, int ncol, float cellsize, float neighborhood, float nodata);

int aspectSqrEvans(float* DEMBuf, float* outBuf, int nrow, int ncol, float cellsize, float neighborhood, float nodata);

int aspectHorn(float* DEMBuf,
				 float* outBuf,
				 int nrow, 
				 int ncol, 
				 float cellsize, 
				 float neighborhood, 
				 float nodata);

int aspectCirShi(float* DEMBuf,
				 float* outBuf,
				 int nrow, 
				 int ncol, 
				 float cellsize, 
				 float neighborhood, 
				 float nodata);

int aspectSqrShi(float* DEMBuf,
				 float* outBuf,
				 int nrow, 
				 int ncol, 
				 float cellsize, 
				 float neighborhood, 
				 float nodata);

int aspectZT(float* DEMBuf,
				 float* outBuf,
				 int nrow, 
				 int ncol, 
				 float cellsize, 
				 float neighborhood, 
				 float nodata);

int back(float* terrainPartitionBuf, 
		 float* slopeBuf, 
		 float* ridgeBuf, 
		 float* outBuf, 
		 float topPercent, 
		 int nrow, int ncol,
		 float nodata);

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
				float nodata);

int catchment(float* flowDir,	//flow direction data
			  float* outBuf,	//result
			  int nrow, 
			  int ncol, 
			  float nodata);

int curvatureCirEvans(float* DEMBuf, 
				float* outBuf, 
				int nrow, 
				int ncol,
				float cellsize,
				int neighborhood,
				int type,
				float nodata);

int curvatureSqrEvans(float* DEMBuf, 
				float* outBuf, 
				int nrow, 
				int ncol,
				float cellsize,
				int neighborhood,
				int type,
				float nodata);

int curvatureCirZT(float* DEMBuf, 
				float* outBuf, 
				int nrow, 
				int ncol,
				float cellsize,
				int neighborhood,
				int type,
				float nodata);

int curvatureSqrZT(float* DEMBuf, 
				float* outBuf, 
				int nrow, 
				int ncol,
				float cellsize,
				int neighborhood,
				int type,
				float nodata);

//used by the curvauter functions above
float curvatures(double r, double s, double t, double p, double q, int type, float nodata);

int dividelines(float* DEMBuf,
				float* terrainPartitionBuf,
				float* outBuf,
				int nrow,
				int ncol,
				int label,
				float nodata);

int fillDEM(float* DEMBuf,	//Input DEM
			  float* cachBuf,	//empty buffer for holding cachment data
			  float* flowBuf,	//empty buffer for holding flow direction
			  float* outBuf,	//result
			  float threshold,
			  int nrow, 
			  int ncol, 
			  float nodata);

int flowAccumulation(float* flowDir,		//flow direction data 
					  float* outBuf,		//result
					  int nrow, 
					  int ncol, 
					  float unitArea,
					  float nodata);

int flowDirection(float* DEMBuf,		//the elevation data
				   float* outBuf,	//result
				   int nrow, 
				   int ncol, 
				   float nodata);
	//0:	Center
	//1:	NE
	//2:	E
	//4:	SE
	//8:	S
	//16:	SW
	//32:	W
	//64:	NW
	//128:	N

int fuzzyAspects(float* aspectBuf,	//an aspect data layer
				  float* outBuf,	//result
				  int nrow,			
				  int ncol, 
				  float lowerLimitFor0, //in degree
				  float upperLimitFor0, //in degree
				  float lowerLimitFor1, //in degree
  				  float upperLimitFor1, //in degree
				  float nodata);

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
			   float nodata);

//This function uses square-shape neighborhood and Evans (1972) algo.
//result is in percent (tan(degree))
int gradientSqrEvans(float* DEMBuf, 
			   float* outBuf, 
			   int nrow, 
			   int ncol, 
			   float cellsize,
			   float neighborhood,
			   float nodata);

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
			   float nodata);

//This function uses circular-shape neighborhood and Shi algo.
//result is in percent (tan(degree))
int gradientCirShi(float* DEMBuf, 
			   float* outBuf, 
			   int nrow, 
			   int ncol, 
			   float cellsize,
			   float neighborhood,
			   float nodata);

//This function uses square-shape neighborhood and Shi algo.
//result is in percent (tan(degree))
int gradientSqrShi(float* DEMBuf, 
			   float* outBuf, 
			   int nrow, 
			   int ncol, 
			   float cellsize,
			   float neighborhood,
			   float nodata);

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
			   float nodata);

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
			  float nodata);

void planformCurvature(float* inBuf, 
					   float* outBuf, 
					   int nrow, 
					   int ncol, 
					   int neighborhood, 
					   float nodata);

void profileCurvature(float* inBuf, 
					  float* outBuf, 
					  int nrow, 
					  int ncol, 
					  int neighborhood, 
					  float nodata);

//Peucker and Douglas (1975) algorithm
int ridgelines1(float* DEMBuf, float* outBuf, int nrow, int ncol, int label, float nodata);

//O'Callaghan and Mark (1984) algorithm
int ridgelines2(float* DEMBuf, float* outBuf, int nrow, int ncol, int label,	float nodata);

//Skidmore (1991) algorithm
int ridgelines3(float* DEMBuf, float* outBuf, int nrow,	int ncol, int label, float nodata);

int ridgelineShi(float* terrainPartitionBuf, float* DEMBuf, float* outBuf, float  TPEleDiff, float  proportion, int nrow, int ncol, int label, float nodata);

int shaveDEM(float* DEMBuf,	//Input DEM
			  float* cachBuf,	//empty buffer for holding cachment data
			  float* flowBuf,	//empty buffer for holding flow direction
			  float* outBuf,	//result
			  float maxEle, //maximum elevation value in the DEM
			  float threshold,
			  int nrow, 
			  int ncol, 
			  float nodata);

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
			 float nodata);

int streamlineOCallaghan(float* flowDir,	//flow direction layer
				 float* flowAcc,	//flow accumulation layer
				 float* outBuf,		//result
				 int nrow, 
				 int ncol,
				 float threshold,	//level of the stream network
				 int label,
				 CString codingAlg, 
				 float nodata); 

int streamlinePeuker(float* inBuf, //dem buffer
					   float* outBuf, //output buffer
					   int nrow, 
					   int ncol, 
					   int label, //label of streamline in output
					   float nodata);

int streamlineSkidmore(float* inBuf, //dem buffer
					   float* outBuf, //output buffer
					   float upper,
					   float lower,
					   int nrow, 
					   int ncol, 
					   int label, //label of streamline in output
					   float nodata);

int terrainPartition(float* streamlineBuf, 
					 float* flowDirBuf,
					 float* outBuf, 
					 int nrow, int ncol,
					 float theAngle, //threshold for combining partitions w/ similar aspects
					 float nodata); 

void wetnessIndexMultiDirection(float* inBuf, 
								float* outBuf, 
								int nrow, 
								int ncol, 
								int neighborhood, 
								float nodata);

void wetnessIndexSingleDirection(float* inBuf, 
								 float* outBuf, 
								 int nrow, 
								 int ncol, 
								 int neighborhood, 
								 float nodata);


/*************************************
	UTILITY FUNCTIONS
 
	Content:

	addArea    (used by flowAccumulation)
	bilinearWgtCir
	bilinearWgtSqr
	cirNeighbor (find out the eight values in a user-specified neighborhood in a circular shape)
	combinePartitions (used by terrainPartition)
	decreaseMatrixValue (used by dividelines)
	fillSink   (used by fillDEM and shaveDEM)
	filling (filling a sink, used by fillSink)
	findStreamSectionEnd (used by testStreamSections)
	findStreamSectionHead (used by testStreamSections)
	flowInLink(used by combinePartitions)
	haveNext (used by dividelines)
	nextFlowPixel (used by headwater, terrainPartition)
	previouseFlowPixel (used by headwater, streamO)
	sqrNeighbor (find out the eight values in a user-specified neighborhood in a square shape)
	setNext (used by dividelines)
	smallPatches
	testAdjacency (used by combinePartitions)
	testStreamSections (used by combinePartitions, replaced by testAdjacency)
	unionOverlay ("union" is a keyword of C++)

*************************************/

int addArea(int curPos, float* flowDir, float* flowAcc, float unitArea, int nrow, int ncol);

void bilinearWgtCir(float neighborhood, float cellsize, int nrow, int ncol, double para[]);

void bilinearWgtSqr(float neighborhood, float cellsize, int nrow, int ncol, double para[]);

void combinePartitions(float* terrainPartitionBuf, float* streamlineBuf, float* flowDirBuf, int nrow, int ncol, float theAngle);

void decreaseMatrixValue(Patch<float>* thePatch, int pos);

int fillSink(float* DEMBuf,	float* cachBuf, float* outBuf, float threshold, int nrow, int ncol, float nodata);

template<class T>
int filling(Patch<T>* thePatch, float* DEMBuf);

int findStreamSectionEnd(float* flowDir, float* streamlineBuf, int nrow, int ncol, int streamCode);

int findStreamSectionHead(float* flowDir, float* streamlineBuf, int nrow, int ncol, int streamCode);

//return the end position of a stream that flows into pos.  Working clockwise and return the first
//found position whose id in idBuf is true (avoid returning the same position everytime).
int flowInLink(float* flowDirBuf, bool* idBuf, int nrow, int ncol, int pos);

float handleDiagonalNodata(float v1, float v2, float v3, float nodata);

bool haveNext(Patch<float>* thePatch, int curPos, int level);

int neighborhoodValues(float* dataBuf, int nrow, int ncol, int pos, int NumCellsize, int dNumCellsize, 
			double wc1, double wc2, double wd1, double wd2, double result[], float nodata);

int nextFlowPixel(int curPos, float* flowDir, int ncol);

int previousMaxFlowPixel(int curPos, float* flowDir, float* flowAcc, bool* idBuf, int nrow, int ncol);

int setNext(Patch<float>* thePatch, float* DEMBuf, int curPos, int level);

template<class T>
int smallPatches(T* inBuf, float* outBuf, int nrow, int ncol, int threshold, bool eightDir, int label, T nodata){
	int i = 0;
	//Building the ID buffer
	unsigned char* idBuf = new unsigned char[nrow*ncol];
	if(idBuf == NULL) return 32; //No sufficient memory
	for(i=0; i<nrow*ncol; i++){
		if(inBuf[i]==nodata)  //if nodata
			idBuf[i] = 4;	//0: not visited
		else				//1: visited
			idBuf[i] = 0;	//4: nodata
		outBuf[i] = 0;
	}

	for(i=0; i<nrow*ncol; i++){
		if(idBuf[i] == 0){
			Patch<T>* thePatch = new Patch<T>(i, inBuf, idBuf, nrow, ncol, eightDir);

			PatchPixel* tempPixel = thePatch->first;
			if(thePatch->numPixels<=threshold){//a small patch
				while(tempPixel!=NULL){
					outBuf[tempPixel->pos] = label;
					tempPixel = tempPixel->nextPixel;
				}
			}
			else{								//not a small patch
				while(tempPixel!=NULL){
					outBuf[tempPixel->pos] = 0;
					tempPixel = tempPixel->nextPixel;
				}
			}
		}
	}
	return 0;
}

bool testAdjacency(float* terrainPartitionBuf, int nrow, int ncol, int firstCode, int secondCode);

bool testStreamSections(float* flowDir, float* streamlineBuf, int nrow, int ncol, int firstCode, int secondCode);


//MFDCaculation
int MFD(float* DEMBuf, float* AccRes, int nrow, int ncol,float cellsize,float nodata,int FlowDistribType,double p);
void MFDPreProcess(float* DEMBuf, int nrow, int ncol,float cellsize,float nodata,int FlowDistribType,double *arrMFFractSum,double *arrPara,double p);
void MFDAccumCompute(float *DEMBuf,float *AccRes,int nrow,int ncol,float cellsize,float nodata,int FlowDistribType,double *arrMFFractSum,double *arrPara);
double CheckNeighbor(int i,int  j,float *DEMBuf,float *AccRes,int nrow,int ncol,float cellsize,float nodata,int FlowDistribType,double *arrMFFractSum,double *arrPara);
bool IsValidXYSrcValue(int i, int j,int nrow, int ncol);


//FillDEM
bool FillDEM_PD(float *DEMBuf,float *vWf,int nrow,int ncol,float cellsize,float nodata,double dElevEps=0.01);
bool NextCell(int *x,int *y,int scan,int nrow,int ncol);
void DryUpwardCell(int x,int y,float *DEMBuf,float *vWf,int nrow,int ncol,double dElevEps0,double dElevEps1);



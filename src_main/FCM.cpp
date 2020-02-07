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
// FCM.cpp: implementation of the FCM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FCM.h"
#include "Utility.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FCM::FCM(CEnvDatabase *pEnvDatabase,bool* idBuf,  int numOfValidPixel, int ksize, double weight, double EPS, int iteration)
{
	// TODO: add one-time construction code here
	MaxIteration = iteration;
	DistNorm = 1;
	m_pEnvDatabase = pEnvDatabase;
	this->ksize = ksize;

	this->EPS = EPS;
	this->WeightExp = weight;

	this->numOfValidPixel =  numOfValidPixel;

	maskBuf = idBuf;

	NumberOfVariables  = m_pEnvDatabase->getLayerNum();


}

FCM::~FCM()
{

	for(int i=0; i<samplingPixelNum; i++)	 
		delete [] pixelFeatures[i].vars;

	delete []pixelFeatures;

	pixelFeatures = NULL;

}


void FCM::InitiatePixels()
{
	if(m_pEnvDatabase == NULL)
		return;

	int hksize = (int) floor((float)ksize/2);


	int ncol = m_pEnvDatabase->m_pTheSetting->getColNum();
	int nrow = m_pEnvDatabase->m_pTheSetting->getRowNum();

	samplingPixelNum = 0;

	for (int i=hksize; i<(nrow-hksize); i=(i+ksize)) 
		for (int j=hksize; j<(ncol-hksize); j=(j+ksize)) 
		{

			bool tag = true;
			for(int m=0;m<NumberOfVariables;m++)
				if(fabs(m_pEnvDatabase->getLayer(m)->getData(i,j)-m_pEnvDatabase->getLayer(m)->m_pTheData->getNullDataValue())<VERYSMALL)
				{
					break;
					tag = false;
				}

				if (tag == true && maskBuf[i*ncol+j]== true)
			    {
				    samplingPixelNum ++;
			    }		

		}


	pixelFeatures = new PixelFeatures[samplingPixelNum];
	for(int i=0; i<samplingPixelNum; i++)
		pixelFeatures[i].vars = new float[this->NumberOfVariables];
}




void FCM::SamplePixels()
{
	if(m_pEnvDatabase == NULL)
		return;

	int hksize = (int) floor((float)ksize/2);


	int ncol = m_pEnvDatabase->m_pTheSetting->getColNum();
	int nrow = m_pEnvDatabase->m_pTheSetting->getRowNum();

	int samplingPixelNum = 0;

	for (int i=hksize; i<(nrow-hksize); i=(i+ksize)) 
		for (int j=hksize; j<(ncol-hksize); j=(j+ksize)) 
		{

			bool tag = true;
			for(int m=0;m<NumberOfVariables;m++)
				if(fabs(m_pEnvDatabase->getLayer(m)->getData(i,j)-m_pEnvDatabase->getLayer(m)->m_pTheData->getNullDataValue())<VERYSMALL)
				{
					break;
					tag = false;
				}

				if (tag == true && maskBuf[i*ncol+j]== true)
			    {
				   //pixelFeatures[samplingPixelNum].x = j;
				   //pixelFeatures[samplingPixelNum].y = i;

				   for(int m=0;m<NumberOfVariables;m++)
					 pixelFeatures[samplingPixelNum].vars[m] =m_pEnvDatabase->getLayer(m)->getData(i,j);
				   samplingPixelNum ++;
			    }		

		}

}






void FCM::AssignScalingMatrix()
{

	int i,j;
	for(i=0;i<NumberOfVariables;i++)
	{
		for(j=0;j<NumberOfVariables;j++)
		{
			if(j==i) scalingMatrix[i][j] = 1.0;
			else
				scalingMatrix[i][j] = 0.0;
		}
	}
}



void FCM::CalClassesCentroid()
{

	//int i,j;

	IniMembershipMatrix();

	int iterationNum=0;
	double errMax, err;

	errMax = 100.0;

	while((iterationNum<MaxIteration) && (errMax>EPS))
	{
		UpdateClassCenters();
		UpdateMemberShip();

		errMax = 0.0;

		for(int i=0;i<NumberOfClasses;i++)
		{
			for(int j=0;j<samplingPixelNum;j++)
			{
				err = fabs(membershipMatrix[i][j]-updatedMembershipMatrix[i][j]);
				if(err>errMax) errMax = err;
			}
		}


		//copy the new membership matrix to the old one
		for(int i=0;i<NumberOfClasses;i++)
		{
			for(int j=0;j<samplingPixelNum;j++)
			{
				membershipMatrix[i][j] = updatedMembershipMatrix[i][j];
			}
		}

		iterationNum++;


	}

	convergedIterNum = iterationNum;
	convergedErrMax = errMax;
}

void FCM::IniMembershipMatrix()
{

	membershipMatrix = new float*[NumberOfClasses];

	updatedMembershipMatrix = new float*[NumberOfClasses];



	for(int  i=0;i<this->NumberOfClasses;i++)  
	{    

		membershipMatrix[i] = new float[samplingPixelNum];
		updatedMembershipMatrix[i] = new float[samplingPixelNum];		
		//updatedMembershipMatrix[i].resize(samplingPixelNum);
	}



	float randomNum; //random number between 0~1.0
	float remainOfMembership;

	float powerValue; //dummy variable

	for(int j=0;j<samplingPixelNum;j++)
	{
		remainOfMembership = 1.0;

		for(int i=0;i<NumberOfClasses-1;i++)
		{
			randomNum = (float)rand()/RAND_MAX; //RAND_MAX is defined in VC library
			powerValue = 1.0/(NumberOfClasses-1-i);
			membershipMatrix[i][j] = remainOfMembership*(1.0-pow(randomNum,powerValue));
			remainOfMembership -= membershipMatrix[i][j];
		}

		membershipMatrix[i][j] = remainOfMembership; //so the sum of all membership for this sample pixel is 1.0
	}


}

void FCM::UpdateClassCenters()
{
	int i,j,k;

	double dummySum;

	for(i=0;i<NumberOfClasses;i++)
	{
		for(j=0;j<NumberOfVariables;j++)
		{

			//initialize the class centers
			classCentroid[i][j] = 0.0;

			dummySum=0.0;
			for(k=0;k<samplingPixelNum;k++)
			{
				classCentroid[i][j] += pow((double)membershipMatrix[i][k],WeightExp)*pixelFeatures[k].vars[j];

				dummySum += pow((double)membershipMatrix[i][k],WeightExp);
			}

			classCentroid[i][j] = classCentroid[i][j]/dummySum;
		}
	}


}

void FCM::UpdateMemberShip()
{
	double dummySum1; //A
	double dummySum2; //C
	double sum; //SUM

	int i,j;
	int k,l;
	int n;

	for(i=0;i<NumberOfClasses;i++)
	{
		for(j=0;j<samplingPixelNum;j++)
		{
			updatedMembershipMatrix[i][j] = 0.0;

			dummySum1 = 0.0;
			for(k=0;k<NumberOfVariables;k++)
			{
				for(l=0;l<NumberOfVariables;l++)
				{
					dummySum1 += (pixelFeatures[j].vars[k]-classCentroid[i][k])
						*scalingMatrix[k][l]
						*(pixelFeatures[j].vars[l]-classCentroid[i][l]);

				}
			}


			dummySum1 = 1.0/pow(dummySum1, 1.0/(WeightExp-1.0));

			sum = 0.0;

			for(n=0;n<NumberOfClasses;n++)
			{
				dummySum2 = 0.0;

				for(k=0;k<NumberOfVariables;k++)
				{
					for(l=0;l<NumberOfVariables;l++)
					{
						dummySum2 += (pixelFeatures[j].vars[k]-classCentroid[n][k])
							*scalingMatrix[k][l]
							*(pixelFeatures[j].vars[l]-classCentroid[n][l]);
					}
				}


				dummySum2 = 1.0/pow(dummySum2, 1.0/(WeightExp-1.0));

				sum += dummySum2;
			}

			updatedMembershipMatrix[i][j] = dummySum1/sum;

		}
	}


}


void FCM::CalEntropy()
{

	float tempMembership;
	int i,j;

	//initialize the partion coerricients and entropy
	partitionCoef = 0.0;
	entropy = 0.0;

	//calcuate entropy and partition coefficient
	for(i=0;i<NumberOfClasses;i++)
	{
		for(j=0;j<samplingPixelNum;j++)
		{
			tempMembership = membershipMatrix[i][j];
			partitionCoef += tempMembership*tempMembership/samplingPixelNum;
			if(tempMembership>0) 
			{
				entropy += -1.0*tempMembership*log(tempMembership)/samplingPixelNum;
			}
		}
	}

	//calculate entropy bound
	entropyBound = 1.0 - partitionCoef; 

	//calculate payoff
	float dist;
	int l,m;
	payoff = 0.0;
	for(i=0;i<NumberOfClasses;i++)
	{
		for(j=0;j<samplingPixelNum;j++)
		{
			dist = 0.0;
			for(l=0;l<NumberOfVariables;l++)
			{
				for(m=0;m<NumberOfVariables;m++)
				{
					dist += (pixelFeatures[j].vars[l]-classCentroid[i][l])
						*scalingMatrix[l][m]
						*(pixelFeatures[j].vars[m]-classCentroid[i][m]);
				}
			}

			payoff += pow((double)membershipMatrix[i][j],WeightExp) * dist;


		}
	}


}

void FCM::WriteClassCentroidFile()
{

	CString strCentroidFile;
	strCentroidFile.Format("%s%s", strResultDir, "\\cluster_centroids.txt");

	FILE *fp;

	fp = fopen(strCentroidFile.GetBuffer(), "w");
	if(fp==NULL)
	{
		AfxMessageBox("The output file can NOT be created!!!\n");
		return;
	}

	fprintf(fp,"NumberOfClasses: %d \n", NumberOfClasses);

	fprintf(fp,"NumberOfVariables: %d \n", NumberOfVariables);

	fprintf(fp,"NumberOfIteration: %d \n", convergedIterNum);

	fprintf(fp,"MaxError: %lf \n", convergedErrMax);

	//The class centroids
	fprintf(fp,"ClassCentroids:\n");
	for(int i=0;i<NumberOfClasses;i++)
	{
		fprintf(fp,"Class%d: (", i+1);
		for(int j=0;j<NumberOfVariables-1;j++)
		{
			fprintf(fp,"%lf, ", classCentroid[i][j]);
		}
		fprintf(fp,"%lf", classCentroid[i][NumberOfVariables-1]);
		fprintf(fp,")\n");
	}

	fprintf(fp,"PartitionCoefficient: %lf \n", partitionCoef);
	fprintf(fp,"Entropy: %lf \n", entropy);
	fprintf(fp,"PayOff: %lf \n", payoff);

	fprintf(fp,"\n");

	fclose(fp);

}


void FCM::WriteMembershipFiles()
{

	if(m_pEnvDatabase == NULL)
		return;


	int NumOfVariables  = m_pEnvDatabase->getLayerNum();
	int ncol = m_pEnvDatabase->m_pTheSetting->getColNum();
	int nrow = m_pEnvDatabase->m_pTheSetting->getRowNum();


		double dummySum1; //A
		double dummySum2; //C
		double sum; //SUM



		for(int i=0; i<NumberOfClasses; i++)
		{

			float *resultBuf = new float[nrow*ncol];

			for(int irow=0;irow<nrow;irow++)
				for(int icol=0;icol<ncol;icol++)		 	 
				{

					bool tag = true;
					for(int m=0;m<NumberOfVariables;m++)
						if(fabs(m_pEnvDatabase->getLayer(m)->getData(irow,icol)-m_pEnvDatabase->getLayer(m)->m_pTheData->getNullDataValue())<VERYSMALL)
						{
							break;
							tag = false;
						}


						if (tag == true && maskBuf[irow*ncol+icol] == true)
						{

							//membership = 0.0;

							dummySum1 = 0.0;
							for(int k=0;k<NumberOfVariables;k++)
							{
								for(int l=0;l<NumberOfVariables;l++)
								{
									dummySum1 += (m_pEnvDatabase->getLayer(k)->getData(irow,icol)-classCentroid[i][k])
										*scalingMatrix[k][l]*(m_pEnvDatabase->getLayer(l)->getData(irow,icol)-classCentroid[i][l]);

								}
							}


							dummySum1 = 1.0/pow(dummySum1, 1.0/(WeightExp-1.0));

							sum = 0.0;

							for(int n=0;n<NumberOfClasses;n++)
							{
								dummySum2 = 0.0;

								for(int k=0;k<NumberOfVariables;k++)
								{
									for(int l=0;l<NumberOfVariables;l++)
									{
										dummySum2 += (m_pEnvDatabase->getLayer(k)->getData(irow,icol)-classCentroid[n][k])
											*scalingMatrix[k][l]
											*(m_pEnvDatabase->getLayer(l)->getData(irow,icol)-classCentroid[n][l]);
									}
								}


								dummySum2 = 1.0/pow(dummySum2, 1.0/(WeightExp-1.0));

								sum += dummySum2;
							}

							resultBuf[irow*ncol+icol] = dummySum1/sum;

						}	

						else
						{
							resultBuf[irow*ncol+icol] = 0;
						}


				}


				CString strOutputFileName;
				strOutputFileName.Format("%s%s%d%s", strResultDir, "\\class", i+1,".3dr");
				CThreeDR* output = new CThreeDR;
				output->setNumberOfRows(nrow);
				output->setNumberOfColumns(ncol);
				output->setXmin(m_pEnvDatabase->m_pTheSetting->getWest());
				output->setYmin(m_pEnvDatabase->m_pTheSetting->getSouth());
				output->setCellSize(m_pEnvDatabase->m_pTheSetting->getCellSize());
				output->setNullDataValue(NOFLOATVALUE);
				output->setDataName(extractFileName(strOutputFileName).GetBuffer(1));
				output->setDataUnits(m_pEnvDatabase->m_pTheSetting->getDataUnit().GetBuffer());
				output->setGridUnits(m_pEnvDatabase->m_pTheSetting->getGridUnit().GetBuffer());
				output->setDataBuf(resultBuf);
				output->calcStat();
				output->writeout(strOutputFileName.GetBuffer(),0);
				delete output;			//write 3dr file
		}

}

void FCM::setResultDir(CString strResultDir)
{
	this->strResultDir = strResultDir;
}


void FCM::setNumOfClasses(int numOfClasses)
{
	this->NumberOfClasses = numOfClasses;

}

void FCM::DestroyMatrix()
{
	for(int i = 0; i < this->NumberOfClasses; i++)
	{

		delete[] membershipMatrix[i];
		delete[] updatedMembershipMatrix[i];
	}
	delete[] membershipMatrix;
	delete[] updatedMembershipMatrix;
	membershipMatrix = NULL;
	updatedMembershipMatrix = NULL;
}


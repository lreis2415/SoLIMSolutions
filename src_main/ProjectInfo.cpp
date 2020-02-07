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

#include "StdAfx.h"
#include <iostream>

#include ".\projectinfo.h"

using namespace std;

ProjectInfo::ProjectInfo()
{

}



ProjectInfo::~ProjectInfo(void)
{
}


bool ProjectInfo::fillProjectInfo(CProjectCase *pProjectCase)
{

    double maxResolution = 0;
    


	for(int i=0; i<pProjectCase->g_vecstrClimateLayers.size(); i++)
	{

		EnViriable enVar;
		enVar.name = pProjectCase->vecClimateLayerNames[i];
		enVar.file = pProjectCase->g_vecstrClimateLayers[i];
		enVar.resolution = pProjectCase->vecClimateLayerResolutions[i];

		if(enVar.resolution > maxResolution)
		{
			maxResolution = enVar.resolution;
			
	       resolution = maxResolution;
	      // refFile = pProjectCase->g_vecstrClimateLayers[i];
		}


		if ( pProjectCase->vecClimateLayerSimiTypes[i] == "Prob. Density")
		{
			enVar.simiCalInfo.simiType = SIMI_TYPE_PDF;
			enVar.simiCalInfo.simiMethod = SIMI_METHOD_CM;
		}
		else
		{
			enVar.simiCalInfo.simiType = SIMI_TYPE_SINGLEVALUE;
			
			if ( pProjectCase->vecClimateLayerSimiMethods[i] == "Boolean")
			{
				enVar.simiCalInfo.simiMethod = SIMI_METHOD_BOOLEAN;
			}
			else
			{
				enVar.simiCalInfo.simiMethod = SIMI_METHOD_GOWER;
			}
		}	

		this->climateVec.push_back(enVar);
	}

	for(int i=0; i<pProjectCase->g_vecstrGeologyLayers.size(); i++)
	{

		EnViriable enVar;
		enVar.name = pProjectCase->vecGeologyLayerNames[i];
		enVar.file = pProjectCase->g_vecstrGeologyLayers[i];
		enVar.resolution = pProjectCase->vecGeologyLayerResolutions[i];

			if(enVar.resolution > maxResolution)
		{
			maxResolution = enVar.resolution;
			
	       resolution = maxResolution;
	      // refFile = pProjectCase->g_vecstrGeologyLayers[i];
		}


		if ( pProjectCase->vecGeologyLayerSimiTypes[i] == "Prob. Density")
		{
			enVar.simiCalInfo.simiType = SIMI_TYPE_PDF;
			enVar.simiCalInfo.simiMethod = SIMI_METHOD_CM;
		}
		else
		{
			enVar.simiCalInfo.simiType = SIMI_TYPE_SINGLEVALUE;
			
			if ( pProjectCase->vecGeologyLayerSimiMethods[i] == "Boolean")
			{
				enVar.simiCalInfo.simiMethod = SIMI_METHOD_BOOLEAN;
			}
			else
			{
				enVar.simiCalInfo.simiMethod = SIMI_METHOD_GOWER;
			}
		}	

		this->materialVec.push_back(enVar);

	}

	 for(int i=0; i<pProjectCase->g_vecstrTerrainLayers.size(); i++)
	{

		EnViriable enVar;
		enVar.name = pProjectCase->vecTerrainLayerNames[i];
		enVar.file = pProjectCase->g_vecstrTerrainLayers[i];
		enVar.resolution = pProjectCase->vecTerrainLayerResolutions[i];

			if(enVar.resolution > maxResolution)
		{
			maxResolution = enVar.resolution;
			
	       resolution = maxResolution;
	      /// refFile = pProjectCase->g_vecstrTerrainLayers[i];
		}


		if ( pProjectCase->vecTerrainLayerSimiTypes[i] == "Prob. Density")
		{
			enVar.simiCalInfo.simiType = SIMI_TYPE_PDF;
			enVar.simiCalInfo.simiMethod = SIMI_METHOD_CM;
		}
		else
		{
			enVar.simiCalInfo.simiType = SIMI_TYPE_SINGLEVALUE;
		
			if ( pProjectCase->vecTerrainLayerSimiMethods[i] == "Boolean")
			{
				enVar.simiCalInfo.simiMethod = SIMI_METHOD_BOOLEAN;
			}
			else
			{
				enVar.simiCalInfo.simiMethod = SIMI_METHOD_GOWER;
			}
		}	

		this->topoVec.push_back(enVar);

	}


	for(int i=0; i<pProjectCase->g_vecstrVegetationLayers.size(); i++)
	{

		EnViriable enVar;
		enVar.name = pProjectCase->vecVegetationLayerNames[i];
		enVar.file = pProjectCase->g_vecstrVegetationLayers[i];
		enVar.resolution = pProjectCase->vecVegetationLayerResolutions[i];

			if(enVar.resolution > maxResolution)
		{
			maxResolution = enVar.resolution;
			
	       resolution = maxResolution;
	      // refFile = pProjectCase->g_vecstrVegetationLayers[i];
		}


		if ( pProjectCase->vecVegetationLayerSimiTypes[i] == "Prob. Density")
		{
			enVar.simiCalInfo.simiType = SIMI_TYPE_PDF;
			enVar.simiCalInfo.simiMethod = SIMI_METHOD_CM;
		}
		else
		{
			enVar.simiCalInfo.simiType = SIMI_TYPE_SINGLEVALUE;
			
			if ( pProjectCase->vecVegetationLayerSimiMethods[i] == "Boolean")
			{
				enVar.simiCalInfo.simiMethod = SIMI_METHOD_BOOLEAN;
			}
			else
			{
				enVar.simiCalInfo.simiMethod = SIMI_METHOD_GOWER;
			}
		}	

		this->vegeVec.push_back(enVar);

	}


		for(int i=0; i<pProjectCase->g_vecstrOtherLayers.size(); i++)
	{

		EnViriable enVar;
		enVar.name = pProjectCase->vecOtherLayerNames[i];
		enVar.file = pProjectCase->g_vecstrOtherLayers[i];
		enVar.resolution = pProjectCase->vecOtherLayerResolutions[i];

			if(enVar.resolution > maxResolution)
		{
			maxResolution = enVar.resolution;
			
	       resolution = maxResolution;
	      // refFile = pProjectCase->g_vecstrOtherLayers[i];
		}


		if ( pProjectCase->vecOtherLayerSimiTypes[i] == "Prob. Density")
		{
			enVar.simiCalInfo.simiType = SIMI_TYPE_PDF;
			enVar.simiCalInfo.simiMethod = SIMI_METHOD_CM;
		}
		else
		{
			enVar.simiCalInfo.simiType = SIMI_TYPE_SINGLEVALUE;
		
			if ( pProjectCase->vecOtherLayerSimiMethods[i] == "Boolean")
			{
				enVar.simiCalInfo.simiMethod = SIMI_METHOD_BOOLEAN;
			}
			else
			{
				enVar.simiCalInfo.simiMethod = SIMI_METHOD_GOWER;
			}
		}	

		this->otherVec.push_back(enVar);

	}




	//read the inference related information


	
	//read Sample Point	
	samplePoint.file = pProjectCase->getProjectDir() + "\\" + pProjectCase->m_strCasebaseFile;
	samplePoint.xField = pProjectCase->m_strXIndex;
	samplePoint.yField = pProjectCase->m_strYIndex;

	samplePoint.IDField = pProjectCase->m_strIDIndex;

	for(int i=0; i< pProjectCase->vecPropertyToInfer.size(); i++)
	{
		

			string tmpField = pProjectCase->vecPropertyToInfer[i];
	    samplePoint.attributeVec.push_back(tmpField);
	}


		for(int i=0; i< pProjectCase->vecTypeToInfer.size(); i++)
	{
		

		string tmpType= pProjectCase->vecTypeToInfer[i];
	    //samplePoint.attributeVec.push_back(tmpType);
	}




	
	//read sample level similarity integration method
	sampleLevelMethod.type =  pProjectCase->m_iCaseInteMethod;

	if (sampleLevelMethod.type == 2)
	{

		sampleLevelMethod.climateWeight = pProjectCase->m_dClimateWeight;

		sampleLevelMethod.climateCut = pProjectCase->m_dClimateCut;

		sampleLevelMethod.topoWeight = pProjectCase->m_dTopoWeight;

		sampleLevelMethod.vegeWeight = pProjectCase->m_dVegWeight;

		sampleLevelMethod.otherWeight = pProjectCase->m_dOtherWeight;

		

	}

	//read output
	
	output.uncertaintyFile = pProjectCase->m_strUncertaintyFile;

	output.threshold = pProjectCase->m_thresholdProperty;
	output.attributeFold = pProjectCase->m_strAttributeFolder;

	output.maxSimiFolder = pProjectCase->m_strMaxSimiFolder;

	if(pProjectCase->iCBROption == 1)

	   output.tempFolder = pProjectCase->m_strAttributeFolder + "\\calculation";

	else 
		output.tempFolder = pProjectCase->m_strMaxSimiFolder + "\\calculation";


	::CreateDirectory(output.tempFolder.c_str(), NULL);


	//if failed check Why with GetlastError()

    if(ERROR_ALREADY_EXISTS == GetLastError())   //folder exists
		return true;
	else
		return false;

}



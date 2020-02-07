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
#pragma once

#include <vector>
using namespace std;

#include "ThreeDR.h"
#include "ThreeDM.h"
#include "terrain.h"

#include "CBRSamplePoint.h"

#include "dbfopen.h"

#include "DataFile.h"



class CProjectCase
{
public:
	CProjectCase(void);
	~CProjectCase(void);

		//layer names
	vector<CString> vecClimateLayerNames;	//climate factors
	vector<CString> vecGeologyLayerNames;	//parent material factors
	vector<CString> vecTerrainLayerNames;	//terrain factors
	vector<CString> vecVegetationLayerNames;   //Vegetation variable
	vector<CString> vecOtherLayerNames;   //Vegetation variable


	// layer file names
	vector<CString> vecClimateLayerFiles;	//climate factors
	vector<CString> vecGeologyLayerFiles;	//parent material factors
	vector<CString> vecTerrainLayerFiles;	//terrain factors
	vector<CString> vecVegetationLayerFiles;   //Vegetation variable
	vector<CString> vecOtherLayerFiles;   //Vegetation variable


    // layer full path
	vector<CString> g_vecstrClimateLayers;	//climate factors
	vector<CString> g_vecstrGeologyLayers;	//parent material factors
	vector<CString> g_vecstrTerrainLayers;	//terrain factors
	vector<CString> g_vecstrVegetationLayers;   //Vegetation variable
	vector<CString> g_vecstrOtherLayers;   //Vegetation variable



	//layer resolutions
	vector<double> vecClimateLayerResolutions;	//climate factors
	vector<double> vecGeologyLayerResolutions;	//parent material factors
	vector<double> vecTerrainLayerResolutions;	//terrain factors
	vector<double> vecVegetationLayerResolutions;   //Vegetation variable
	vector<double> vecOtherLayerResolutions;   //Vegetation variable

	//layer similarity type  (sigle value/ probability density function)	
	vector<CString> vecClimateLayerSimiTypes;	//climate factors
	vector<CString> vecGeologyLayerSimiTypes;	//parent material factors
	vector<CString> vecTerrainLayerSimiTypes;	//terrain factors
	vector<CString> vecVegetationLayerSimiTypes;   //Vegetation variable
	vector<CString> vecOtherLayerSimiTypes;   //Vegetation variable


	//layer similarity method   (gower/boolean/CM)
	vector<CString> vecClimateLayerSimiMethods;	//climate factors
	vector<CString> vecGeologyLayerSimiMethods;	//parent material factors
	vector<CString> vecTerrainLayerSimiMethods;	//terrain factors
	vector<CString> vecVegetationLayerSimiMethods;   //Vegetation variable
	vector<CString> vecOtherLayerSimiMethods;   //Vegetation variable



	




	/*




	// data resampled to the unified resolution
	vector<CString> g_vecClimate;
	vector<CString> g_vecParentMaterial;
	vector<CString> g_vecTerrain;
	vector<CString> g_vecVegetation;

	//sample data files for small scale inference
	CString g_climateSample;
	CString g_parentMaterialSample;
	CString g_topoSample;
	CString g_vegSample;
	CString g_attrSample;
	vector<CString> g_vecTerrainSample;
	vector<CString> g_vecVegetationSample;

	//final similarity vector
	vector<CString> g_vecSimiFile;
	vector<CBRSamplePoint*> g_vecSamplePoint;*/






	//destination resolution
	double g_dstResolution;
	//whether or not use kernel density estimation
	bool g_useKde, g_useKdeVegetation;



    bool m_bIsEditVersion;
	
private:
	CString m_strProjectDir;
	CString m_strProjectName;
	CString m_strProjectDataType;	

public:
	void setProjectDir(CString strProjectDir);
	CString getProjectDir();
	void setProjectName(CString strProjectName);
	CString getProjectName();
	void setProjectDataType(CString strProjectDataType);
	CString getProjectDataType();

    void setIsEditVersion(bool bIsEditVersion);
	bool getIsEditVersion();


	//DBFHandle hDBF;

	CDataFile *hCSV;


	int iCBROption;  //added by Fei

	char* m_projection;	// added by zhanglei
	

	//int xIndex, yIndex;

	//std::vector<std::vector<CString> > vecSampleData;

	CString m_strCasebasePath;




	double m_thresholdProperty;
	double m_thresholdType;


	int m_iCaseInteMethod;

	CString m_strCasebaseFile;

	CString m_strXIndex, m_strYIndex;

		CString m_strIDIndex;

	double m_dClimateCut;


     double m_dClimateWeight;
	 double m_dTopoWeight;
	 double m_dVegWeight;

	 double m_dOtherWeight;

	 	
	CString m_strUncertaintyFile;

	CString m_strAttributeFolder;


	CString m_strMaxSimiFolder;

      std::vector<CString>  vecPropertyToInfer;

	       std::vector<CString>  vecTypeToInfer;

		 CString m_strCurTypeField;


};

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
#include "afxwin.h"
#include "Inference.h"
#include "GetDirDlg.h"
#include "afxcmn.h"



#include "ListCtrlEditable.h"





#include "ProjectInfo.h"

typedef struct ThreeDRHeaderInfo{
	int width;
	int height;
	float noDataValue;
	float cellSize;
	float xMin;
	float yMin;
	float dataMin;
	float dataMax;
}ThreeDRHeaderInfo;




// CCBInferenceView form view


class CCBInferenceView : public CFormView
{
	DECLARE_DYNCREATE(CCBInferenceView)

public:
	CCBInferenceView();           // protected constructor used by dynamic creation
	virtual ~CCBInferenceView();

public:
	enum { IDD = IDD_CBINFERENCE_FORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	virtual void OnInitialUpdate();


	void FillInferenceView();

	void SaveData();







private:


	CString GetFilePath();

	std::vector< CThreeDR* > climateDataVec, materialDataVec, topoDataVec, vegDataVec, otherDataVec;
	float biggestRes;
	float dstRes;
	int biggestResIndex;

public:

	afx_msg void OnBnClickedOk();



public:
	CString resValue;

	int inputCount;



	CString strOutputPath;




	int m_curSelTab;


	//property infer


	CString m_OutputUncertaintyFilename;
	CString m_OutputPredictvaluesFoldername;		

	double m_thresholdProperty;

	CEdit uncertaintyCtrl;
	CEdit propertyFolderCtrl;
	CEdit thresholdPropertyCtrl;


	CCheckListBox listAllField;
	afx_msg void OnBnClickedButtonSaveuncertainty();
	afx_msg void OnBnClickedButtonSavevalues();

	CStatic m_frameResult;
	CStatic m_staticSaveMap;
	CButton m_btnSaveMap;
	CStatic m_staticThreshold;
	CStatic m_staticSaveUncertainty;
	CButton m_btnSaveUncertainty;


	//type infer
	CString m_strMaxSimiFolder;

	CComboBox comboTypeField;

	CString typeFieldName;


	int typeFieldIndex;

	CCheckListBox listAllType;

	CStatic m_staticTypeField;
	CStatic m_staticSelectType;
	CStatic m_frameResultType;
	CStatic m_staticSaveTypeResult;
	CEdit m_editTypeResultFolder;
	CButton m_btnSaveTypeResult;

	CStatic m_staticSaveType;
	CComboBox m_comboSaveType;


	afx_msg void OnBnClickedButtonSaveTyperesult();
	afx_msg void OnCbnSelchangeComboTypeField();








private:
	//combox中的文本
	CString climSimiRule;
	CString parentSimiRule;
	CString topoSimiRule;
	CString intergratedSimiRule;
	//要输入的气候要素的文件名，以2个为例

	//存储环境要素
	vector<CThreeDR*> climateVarVec; 
	vector<CThreeDR*> parentVarVec; 
	vector<CThreeDR*> topoVarVec;
	vector<CThreeDR*> vegVarVec;
	vector<CThreeDR*>  otherVarVec;

	//存储相似度结果
	vector<CString> climateSimiVec;
	vector<CString> parentSimiVec; 
	vector<CString> topoSimiVec; 
	vector<CString> vegSimiVec;



private:




	double climateCut;
	double climateWeight;
	double topoWeight;
	double vegWeight;

	CString outputFolderSimi;
	//double nProcess;
	int varCount;

	double curProcess;

	CThreeDR m_uncertainty;



	//afx_msg void OnBnClickedButton1();
	CComboBox m_comboSampleMethod;
	afx_msg void OnCbnSelchangeComboCase();

public:



	//计算不确定性和土壤属性值的函数
	//int getSimilarityVec(vector<CThreeDR*>& similarityVec);
	//CThreeDR* calcUncertainty(vector<CThreeDR*>& in_similarities); 
	//int calcValues(vector<CThreeDR*>& in_similarities, vector<CBRSamplePoint*>& in_samples, CThreeDR* in_uncertainty, float in_threshold);
	//int findMaxSum(vector<float>& inputarray, int &flag, float &max, float &sum);

	//void calcMaxSimAndTime(vector<CThreeDR*>& in_similarities);

	//不确定性阈值
	//CString m_OutputUncertaintyFilename;
	//CString m_OutputPredictvaluesFoldername;		

	//double threshold;

	//CString m_OutputMaxSimFilename;
	//CString m_OutputTimesFilename;

	//CEdit maxSimCtrl;
	//CEdit propertyFolderCtrl;
	//CEdit thresholdCtrl;

	//double m_thresholdType;


	//CTypeInferPage m_typeInferPage;
	//CPropertyInferPage m_propertyInferPage;

	CProgressCtrl m_ProgressBar;


	CEditableListCtrl m_ListLayerSetting;
	CComboBox m_comboSimiMeasure;
	afx_msg void OnBnClickedButtonCasemethodSetting();







	void getThreeDRHeaderInfo(const char* filename, ThreeDRHeaderInfo* info);
	void getThreeDRData(const char* filename, float* data);

	double threeDR2Ascii(const char* filename, const char* outputfile);


	bool resampleAndClipThreeDR(const char* filename, float resolution, const char* outputfile, double west, int numOfCol, double south, int numOfRow);



	bool calKDEAndClip(const char* srcfile, float resolution, const char* outputfile, double west, int numOfCol, double north, int numOfRow);


	void generateKdeFile(const char* originalFile, const char* dataname, 
		double dstRes, int sampleCount, 
		double *sampleX, double *sampleY, const char* outputfile,
		vector< CBRSamplePoint* >& vecSamplePoint, int iType, int iVar);


	void generateSingleValueFile(const char* originalFile, const char* dataname, 
		int sampleCount, double *sampleX, double *sampleY, 
		const char* outputfile, 
		vector< CBRSamplePoint* >& vecSamplePoint, int iType, int iVar);


	int calcBooleanSimilarity(vector< CBRSamplePoint* >& vecSamplePoint, int iType, int iVar, 
		string& dataName, string& inputFile, string& outputFolder, 
		vector<string>* outputSimiVec);

	int calcGowerSimilarity(vector< CBRSamplePoint* >& vecSamplePoint, int iType, int iVar, 
		string& dataName, string& inputFile, string& outputFolder,
		vector<string>* outputSimiVec);

	int calcCMSimilarity(vector< CBRSamplePoint* >& vecSamplePoint, int iType, int iVar, 
		string& dataName, string& kdeFile, /*string& refFile,*/ string& outputFolder,
		vector<string>* outputSimiVec);


	void envChar(ProjectInfo& info, int totalCount, vector< vector<EnViriable>* >& vecvec,
		vector<string>& vecDescription, vector< vector<string>* >& vecvecChar);

	void generateSampleData(ProjectInfo& info, int totalCount, vector< vector<EnViriable>* >& vecvec,
		vector<string>& vecDescription, 
		vector< vector<string>* >& vecvecSamplePointFile,
		vector< CBRSamplePoint* >& vecSamplePoint);


	void generateSimiFiles(ProjectInfo& info, int totalCount, 
		vector< vector<EnViriable>* >& vecvec, vector<string>& vecDescription, vector< vector<string>* > vecvecChar,
		vector< CBRSamplePoint* >& vecSamplePoint, 
		vector< vector< vector<string>* >* >& vvvSimi);


	void integrateSimiLimiting(vector< vector<string>* >& vec3drFileName, int sampleCount,
		string& outputFolder, vector<string>* pVecSimi, vector< CBRSamplePoint* >& vecSamplePoint);


	void integrateSimiAverage(vector< vector<string>* >& vec3drFileName, int sampleCount,
		string& outputFolder, vector<string>* pVecSimi, int indexGeology, vector< CBRSamplePoint* >& vecSamplePoint);

	void integrateSimiUserDefined(vector< vector<string>* >& vec3drFileName, 
		double climateCut, vector<double>& vecWeight,
		int sampleCount,
		string& outputFolder, vector<string>* pVecSimi, int indexClimate,int indexGeology, vector< CBRSamplePoint* >& vecSamplePoint);


	void integrateSimiLimitingForOneType(vector< vector<string>* >& vec3drFileName, int sampleCount,
		string& outputFolder, vector<string>* pVecSimi, vector< CBRSamplePoint* >& vecSamplePoint);





	void integrateSimiByType(ProjectInfo& info, int sampleCount, 
		vector< vector<EnViriable>* >& vecvec, vector<string>& vecDescription,
		vector< vector< vector<string>* >* >& vvvSimi, 
		vector< vector<string>* >& vecTypeSimi, vector< CBRSamplePoint* >& vecSamplePoint);	


	void integrateSimiBySample(ProjectInfo& info, int sampleCount, 
		vector< vector<EnViriable>* >& vecvec, vector<string>& vecDescription,
		vector< vector<string>* >& vvSimi, 
		vector<string>& vecSimi, vector< CBRSamplePoint* >& vecSamplePoint);


	int findMaxSum(vector<float>& inputarray, int &flag, float &max, float &sum);


	int calcUncertainty(vector<string>& vecSimi, vector< CBRSamplePoint* >& vecSamplePoint,
		string& uncertaintyFile, string& outputFolder);



	int calcValues(vector<string>& vecSimi, vector< CBRSamplePoint* >& vecSamplePoint,
		double threshold, string& uncertaintyFile, string& outputFolder);


	int recursiveDeleteDir(const char* sDirName);


	int calcMaxSimilarityFile(vector<string>& vecSimi, vector< CBRSamplePoint* >& vecSamplePoint,
		string& maxSimilarityFile);


	void generateSampleDataForType(ProjectInfo& info, int totalCount, vector< vector<EnViriable>* >& vecvec,
		vector<string>& vecDescription, 
		vector< vector<string>* >& vecvecSamplePointFile,
		vector< CBRSamplePoint* >& vecSamplePoint,string typeField, string typeName);

	void Convert_3drToGdal(CThreeDR *input, CString output, char *type);




	//afx_msg void OnNMClickListLayerParaSetting(NMHDR *pNMHDR, LRESULT *pResult);



	CTabCtrl m_tabPropertyType;
	afx_msg void OnTcnSelchangeTabPropertyType(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_editProgress;
	CButton m_btnRunInference;
	CStatic m_frameLayerSetting;
	CStatic m_frameSampleMethod;
	CStatic m_staticSampleMethod;
	CButton m_btnSampleMethod;
	CStatic m_frameItemInfer;
	CStatic m_frame;

	CStatic m_staticSelectProperty;
	double m_dFinalRes;
	afx_msg void OnEnKillfocusEditFinalResolution();
	CStatic m_staticResolution;
	CEdit m_editResolution;

	CString m_strResultExt;

	CString GetOutputExt();
};



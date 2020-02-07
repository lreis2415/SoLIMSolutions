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
// CBRSampleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "CBRSampleDlg.h"
#include ".\CBRSampleDlg.h"
#include "ThreeDR.h"
#include "dbfopen.h"
#include "Distribution.h"
#include "CBRSimilarityDlg.h"

#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

// CBRSampleDlg 对话框

IMPLEMENT_DYNAMIC(CBRSampleDlg, CDialog)
CBRSampleDlg::CBRSampleDlg(CWnd* pParent /*=NULL*/)
: CDialog(CBRSampleDlg::IDD, pParent)
, xFieldName(_T(""))
, yFieldName(_T(""))
, hDBF(NULL)

{
	strOutputPath = getCurrentPath() + "\\sample";
	//testof.open("E:\\cbrsample.txt");
}

CBRSampleDlg::~CBRSampleDlg()
{
	if (hDBF)
		DBFClose(hDBF);
	//testof.close();
}

void CBRSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Sample, m_listctrlSampleData);
	DDX_Control(pDX, IDC_COMBO_FIELD_X, comboFieldX);
	DDX_Control(pDX, IDC_COMBO_FIELD_Y, comboFieldY);
	DDX_CBString(pDX, IDC_COMBO_FIELD_X, xFieldName);
	DDX_CBString(pDX, IDC_COMBO_FIELD_Y, yFieldName);
	DDX_Control(pDX, IDC_LIST_ALL_FIELD, listAllField);
	DDX_Control(pDX, IDC_LIST_SEL_FIELD, listSelField);

	DDX_Control(pDX, IDC_LIST_ALL_TYPE, listAllType);
	DDX_Control(pDX, IDC_LIST_SEL_TYPE, listSelType);
	DDX_Control(pDX, IDC_COMBO_TYPE_FIELD, comboTypeField);
	DDX_CBString(pDX, IDC_COMBO_TYPE_FIELD, typeFieldName);


	DDX_Control(pDX, IDC_RADIO_INFER_PROPERTY, m_radioProperty);
	DDX_Control(pDX, IDC_RADIO_INFER_TYPE, m_radioType);
}


BEGIN_MESSAGE_MAP(CBRSampleDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_GET_SAMPLE, OnBnClickedButtonGetSample)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_SAPMLE_LIST, OnBnClickedButtonClearSapmleList)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FIELD, OnBnClickedButtonAddField)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FIELD2, OnBnClickedButtonAddField2)
	ON_BN_CLICKED(ID_BACK, OnBnClickedBack)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TYPE, OnBnClickedButtonAddType)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TYPE2, OnBnClickedButtonAddType2)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_FIELD, OnCbnSelchangeComboTypeField)
	ON_BN_CLICKED(IDC_RADIO_INFER_TYPE, OnBnClickedRadioType)
	ON_BN_CLICKED(IDC_RADIO_INFER_PROPERTY, OnBnClickedRadioProperty)
END_MESSAGE_MAP()


// CBRSampleDlg 消息处理程序

BOOL CBRSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->m_listctrlSampleData.InsertColumn(1,CString("Sequence ID"));
	this->m_listctrlSampleData.SetColumnWidth(0,80);

	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	if(pApp->iCBROption == 1)
	{  m_radioProperty.SetCheck(TRUE);
	m_radioType.SetCheck(FALSE);
	GetDlgItem(IDC_LIST_ALL_FIELD)->EnableWindow(true);
	GetDlgItem(IDC_LIST_ALL_FIELD)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_ADD_FIELD)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_ADD_FIELD2)->EnableWindow(true);
	GetDlgItem(IDC_COMBO_TYPE_FIELD)->EnableWindow(false);
	GetDlgItem(IDC_LIST_ALL_TYPE)->EnableWindow(false);
	GetDlgItem(IDC_LIST_SEL_TYPE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_ADD_TYPE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_ADD_TYPE2)->EnableWindow(false);

	}
	else
	{
		m_radioProperty.SetCheck(FALSE);
		m_radioType.SetCheck(TRUE);
		CSoLIMApp* pApp = (CSoLIMApp*)AfxGetApp();
		GetDlgItem(IDC_COMBO_TYPE_FIELD)->EnableWindow(true);
		GetDlgItem(IDC_LIST_ALL_TYPE)->EnableWindow(true);
		GetDlgItem(IDC_LIST_SEL_TYPE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_ADD_TYPE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_ADD_TYPE2)->EnableWindow(true);

		GetDlgItem(IDC_LIST_ALL_FIELD)->EnableWindow(false);
		GetDlgItem(IDC_LIST_ALL_FIELD)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_ADD_FIELD)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_ADD_FIELD2)->EnableWindow(false);

	}



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE

}

void CBRSampleDlg::OnBnClickedButtonGetSample()
{
	// TODO: 在此添加控件通知处理程序代码
	// import sample list(ID X Y) 导入样点列表（ID X Y）
	//static char BASED_CODE szFilter[] = "Sample List Files (*.txt)|*.txt|All Files (*.*)|*.*||";
	//CFileDialog filedlgBrowseSampleDlg(TRUE, NULL, "*.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	static char BASED_CODE szFilter[] = "Sample List Files (*.dbf)|*.dbf|All Files (*.*)|*.*||";
	CFileDialog filedlgBrowseSampleDlg(TRUE, NULL, "*.dbf", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	if (filedlgBrowseSampleDlg.DoModal()==IDOK)
	{
		CString strSampleDataListName=filedlgBrowseSampleDlg.GetPathName();

		if(hDBF)
			DBFClose(hDBF);

		hDBF = DBFOpen( (LPCTSTR) strSampleDataListName, "rb" );
		if (hDBF->fp == NULL) return;


		int nFields = DBFGetFieldCount( hDBF );
		int nRec = DBFGetRecordCount( hDBF );

		int iID = DBFGetFieldIndex(hDBF, "ID");


		// add field names to combobox and listctrl
		char fieldName[100];
		int width, deci;
		m_listctrlSampleData.DeleteAllItems();
		while(comboFieldY.GetCount())
			comboFieldY.DeleteString(0);
		while(comboFieldX.GetCount())
			comboFieldX.DeleteString(0);
		while (listAllField.GetCount() > 0)
			listAllField.DeleteString(0);
		while(listSelField.GetCount() > 0)
			listSelField.DeleteString(0);

		//added by Fei
		while (listAllType.GetCount() > 0)
			listAllType.DeleteString(0);
		while(listSelType.GetCount() > 0)
			listSelType.DeleteString(0);

		int index = 1;
		for (int j = 0; j < nFields; ++j)
		{
			DBFGetFieldInfo(hDBF, j, fieldName, &width, &deci);
			m_listctrlSampleData.InsertColumn(index, CString(fieldName));
			m_listctrlSampleData.SetColumnWidth(index++, 80);
			comboFieldX.AddString(fieldName);
			comboFieldY.AddString(fieldName);
			listAllField.AddString(fieldName);
			comboTypeField.AddString(fieldName);
			UpdateData(False);
		}
		comboFieldX.SetCurSel(0);
		comboFieldY.SetCurSel(0);

		comboTypeField.SetCurSel(0);

		for(int i = 0; i < nRec; ++i)
		{
			CString sID;
			sID.Format("%d", i+1);
			m_listctrlSampleData.InsertItem(m_listctrlSampleData.GetItemCount(), sID.GetBuffer());

			for (int j = 0; j < nFields; ++j)
			{
				const char* content = DBFReadStringAttribute( hDBF, i, j);
				m_listctrlSampleData.SetItemText(m_listctrlSampleData.GetItemCount()-1, j+1, content);
			}
		}
	}

}

void CBRSampleDlg::OnBnClickedButtonClearSapmleList()
{
	this->m_listctrlSampleData.DeleteAllItems();
}


/////////////////////////////////////////////////////////////////////
/*****************************************************************
** 函数名:GetFilePath
** 输 入: 无
** 输 出: CString strPath
**        strPath非空, 表示用户选择的文件夹路径
**        strPath为空, 表示用户点击了“取消”键，取消选择
** 功能描述:显示“选择文件夹”对话框，让用户选择文件夹
****************************************************************/
CString CBRSampleDlg::GetFilePath()
{
	CString strPath = "";
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner = NULL; //m_hWnd;
	bInfo.lpszTitle = _T("请选择路径: ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;    

	LPITEMIDLIST lpDlist; //用来保存返回信息的IDList
	lpDlist = SHBrowseForFolder(&bInfo) ; //显示选择对话框
	if(lpDlist != NULL)  //用户按了确定按钮
	{
		TCHAR chPath[255]; //用来存储路径的字符串
		SHGetPathFromIDList(lpDlist, chPath); //把项目标识列表转化成字符串
		strPath = chPath; //将TCHAR类型的字符串转换为CString类型的字符串
	}
	return strPath;
}

void CBRSampleDlg::generateAttributeFiles()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CString filename = strOutputPath + "\\attribute.txt";
	pApp->g_attrSample = filename;
	ofstream output(filename.GetBuffer());

	int nRec = DBFGetRecordCount( hDBF );
	output << "Count:  " << nRec << endl;
	output << "AttributeCount:  " << listSelField.GetCount() << endl;

	for (int i = 0; i < listSelField.GetCount(); ++i)
	{
		CString fileName;
		listSelField.GetText(i, fileName);
		output << fileName << "\t";
	}
	output << endl;

	for (int k = 0; k < nRec; ++k)
	{	
		output << m_listctrlSampleData.GetItemText(k, 0) << "\t" ;
		for (int i = 0; i < listSelField.GetCount(); ++i)
		{
			CString fileName;
			listSelField.GetText(i, fileName);
			int iField = DBFGetFieldIndex(hDBF, fileName.GetBuffer());
			output << m_listctrlSampleData.GetItemText(k, iField + 1) << "\t";
		}
		output << endl;
	}

	output.close();
}

void CBRSampleDlg::generateAttributeFilesForType()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CString filename = strOutputPath + "\\attribute.txt";
	pApp->g_attrSample = filename;
	ofstream output(filename.GetBuffer());

	

	//int nRec = DBFGetRecordCount( hDBF );
	output << "Count:  " << 0 << endl;
	output << "AttributeCount:  " << 0/*listSelField.GetCount()*/ << endl;

	/*for (int i = 0; i < listSelField.GetCount(); ++i)
	{
	CString fileName;
	listSelField.GetText(i, fileName);
	output << fileName << "\t";
	}
	output << endl;

	for (int k = 0; k < nRec; ++k)
	{	
	output << m_listctrlSampleData.GetItemText(k, 0) << "\t" ;
	for (int i = 0; i < listSelField.GetCount(); ++i)
	{
	CString fileName;
	listSelField.GetText(i, fileName);
	int iField = DBFGetFieldIndex(hDBF, fileName.GetBuffer());
	output << m_listctrlSampleData.GetItemText(k, iField + 1) << "\t";
	}
	output << endl;
	}*/

	output.close();
}


CString CBRSampleDlg::generateFiles(CString type, vector<CString>& vecFiles)
{
	double X=0;
	double Y=0;	//坐标
	int iRow=0;
	int iCol=0;	//行列号
	int iSampleCount = this->m_listctrlSampleData.GetItemCount();

	ofstream fileOutput;
	int layerCount = vecFiles.size();
	vector<vector<float> > vecData;
	vecData.resize(iSampleCount);
	for (int k=0; k<iSampleCount; k++)
	{
		vecData[k].resize(layerCount);
	}
	CString oFileName;
	oFileName.Format("%s\\%s.txt", strOutputPath, type);
	fileOutput.open(oFileName);

	CString strLayerName;
	fileOutput << "AttributeCount: " << vecFiles.size() << endl;
	fileOutput<<"ID"<<'\t'<<"X"<<'\t'<<"Y"<<'\t';
	for (int i=0; i<vecFiles.size(); i++)
	{
		CThreeDR threedr;
		strLayerName = vecFiles[i];
		threedr.readin(strLayerName.GetBuffer(),0);
		fileOutput<<strLayerName.Mid(strLayerName.ReverseFind('\\')+1,strLayerName.ReverseFind('.')-strLayerName.ReverseFind('\\')-1)<<'\t';
		CString strTmp;
		for (int j=0;j<this->m_listctrlSampleData.GetItemCount();j++)
		{
			Y=atof(this->m_listctrlSampleData.GetItemText(j,yIndex).GetBuffer());
			iRow=(Y-threedr.getYmin())/threedr.getCellSize();
			strTmp = this->m_listctrlSampleData.GetItemText(j,xIndex);
			X=atof(strTmp.GetBuffer());
			iCol=(X-threedr.getXmin())/threedr.getCellSize();
			if (iRow>=0 && iRow<threedr.getNumberOfRows() && iCol>=0 && iCol<threedr.getNumberOfColumns())
			{
				vecData[j][i] = threedr.getData(X, Y);
			}
			else
			{
				vecData[j][i] = -9999;
			}
		}
	}
	fileOutput<<endl;
	fileOutput << "SampleCount: " << iSampleCount << endl; 

	for (int i=0;i<iSampleCount;i++)
	{
		fileOutput<<this->m_listctrlSampleData.GetItemText(i,0)<<'\t'
			//<<this->m_listctrlSampleData.GetItemText(i,1)<<'\t'
			<<this->m_listctrlSampleData.GetItemText(i,xIndex)<<'\t'
			<<this->m_listctrlSampleData.GetItemText(i,yIndex)<<'\t';
		for (int j=0;j<layerCount;j++)
		{
			fileOutput << setprecision(6) <<vecData[i][j]<<'\t';
		}
		fileOutput<<endl;
	}
	fileOutput.close();

	return oFileName;
}


CString CBRSampleDlg::generateFilesForType(CString type, CString typeName,vector<CString>& vecFiles)
{
	double X=0;
	double Y=0;	//坐标
	int iRow=0;
	int iCol=0;	//行列号
	int iTotalSampleCount = this->m_listctrlSampleData.GetItemCount();
	int iSampleCount = 0;
	for(int i = 0; i < iTotalSampleCount; i ++)
	{
		if(m_listctrlSampleData.GetItemText(i,typeFieldIndex+1).GetBuffer() == typeName)
			iSampleCount ++;

	}

	ofstream fileOutput;
	int layerCount = vecFiles.size();
	vector<vector<float> > vecData;
	vecData.resize(iSampleCount);
	for (int k=0; k<iSampleCount; k++)
	{
		vecData[k].resize(layerCount);
	}
	CString oFileName;
	oFileName.Format("%s\\%s.txt", strOutputPath,type);
	fileOutput.open(oFileName);

	CString strLayerName;
	fileOutput << "AttributeCount: " << vecFiles.size() << endl;
	fileOutput<<"ID"<<'\t'<<"X"<<'\t'<<"Y"<<'\t';
	for (int i=0; i<vecFiles.size(); i++)
	{
		CThreeDR threedr;
		strLayerName = vecFiles[i];
		threedr.readin(strLayerName.GetBuffer(),0);
		fileOutput<<strLayerName.Mid(strLayerName.ReverseFind('\\')+1,strLayerName.ReverseFind('.')-strLayerName.ReverseFind('\\')-1)<<'\t';
		CString strTmp;
		int curNum = 0;
		for (int j=0;j<this->m_listctrlSampleData.GetItemCount();j++)
		{
			if(m_listctrlSampleData.GetItemText(j,typeFieldIndex+1).GetBuffer() == typeName)
			{
				Y=atof(this->m_listctrlSampleData.GetItemText(j,yIndex).GetBuffer());
				iRow=(Y-threedr.getYmin())/threedr.getCellSize();
				strTmp = this->m_listctrlSampleData.GetItemText(j,xIndex);
				X=atof(strTmp.GetBuffer());
				iCol=(X-threedr.getXmin())/threedr.getCellSize();
				if (iRow>=0 && iRow<threedr.getNumberOfRows() && iCol>=0 && iCol<threedr.getNumberOfColumns())
				{
					vecData[curNum][i] = threedr.getData(X, Y);
				}
				else
				{
					vecData[curNum][i] = -9999;
				}
				curNum ++;
			}
		}
	}
	fileOutput<<endl;
	fileOutput << "SampleCount: " << iSampleCount << endl; 

	int curNum = 0;

	for (int i=0;i<iTotalSampleCount;i++)
	{
		if(m_listctrlSampleData.GetItemText(i,typeFieldIndex+1).GetBuffer() == typeName)
		{
			fileOutput<<this->m_listctrlSampleData.GetItemText(i,0)<<'\t'
				//<<this->m_listctrlSampleData.GetItemText(i,1)<<'\t'
				<<this->m_listctrlSampleData.GetItemText(i,xIndex)<<'\t'
				<<this->m_listctrlSampleData.GetItemText(i,yIndex)<<'\t';
			for (int j=0;j<layerCount;j++)
			{
				fileOutput << setprecision(6) <<vecData[curNum][j]<<'\t';
			}
			fileOutput<<endl;
			curNum ++;
		}
	}
	fileOutput.close();

	return oFileName;
}


void CBRSampleDlg::generateKdeFiles(CString type, vector<CString>& vecOriginalFile, vector<CString>& vecSample)
{
	CString dic;
	dic.Format("%s\\%s", strOutputPath, type);
	CreateDirectory(dic.GetString(), NULL);

	vecSample.clear();

	CString outputfile;
	for (unsigned int i = 0; i < vecOriginalFile.size(); ++i)
	{
		CThreeDR data;
		data.readin(vecOriginalFile[i].GetBuffer(), 0);

		CString dataname = getCoreFilename(vecOriginalFile[i]);
		outputfile.Format("%s\\%s.txt", dic, dataname);

		//testof << dataname << endl;

		CDistribution kde;
		ofstream output(outputfile);
		output << dataname << endl;
		output << "NumberOfPointsPerLine:\t" << kde.getNumOfPDE() << endl;
		output << "SampleCount:\t" << m_listctrlSampleData.GetItemCount() << endl;
		output << "ID	X	Y	value" << endl;
		//calculate the stdDev of the whole raster
		float* srcArray = data.getDataBuf();
		int num = data.getNumberOfColumns() * data.getNumberOfRows();
		float stdDev = calcStdDev(srcArray, num, data.getNullDataValue());
		int count = num;
		for(int i=0; i<num; i++)
		{
			if(srcArray[i] == data.getNullDataValue())
				--count;
		}
		float bandWidth = (float)(1.06 * stdDev * pow((double)count, -1.0/5.0));
		kde.setBandWidth(bandWidth);

		//testof << "bandwidth: " << bandWidth << endl;

		CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
		int winSize = (int)(pApp->g_dstResolution / data.getCellSize()) + 1;
		for (int j=0; j < this->m_listctrlSampleData.GetItemCount(); j++)
		{
			double x = atof(this->m_listctrlSampleData.GetItemText(j,xIndex).GetBuffer());
			double y = atof(this->m_listctrlSampleData.GetItemText(j,yIndex).GetBuffer());
			//find the neigbourhood in the source data
			vector<float> neighbors;
			findNeighborhood(&data, x, y, winSize, neighbors);

			//testof << "size:\t" << neighbors.size() << endl;
			//for (int m = 0; m < neighbors.size(); ++m)
			//{
			//	testof << neighbors[m] << "\t";
			//}

			if (neighbors.size() > 0)
			{
				int num = neighbors.size();
				float* pData = new float[num];
				for(int m = 0; m < num; ++m)
					pData[m] = neighbors[m];
				kde.SetValue(neighbors.size(), pData);
				kde.EstimatePD();
				output << setprecision(6) <<this->m_listctrlSampleData.GetItemText(j,0)<<'\t'
					<< x << "\t" << y << endl;
				for( int i = 0; i < kde.getNumOfPDE(); i++ )
					output << kde.getRangeAt(i) << "\t";
				output << endl;
				for( int i = 0; i < kde.getNumOfPDE(); i++ )
					output << kde.getPDVal(i) << "\t";
				output << endl;
				delete pData;
			}
			else
				output << endl << endl;

			//testof << endl;
		}
		output.close();
		vecSample.push_back(outputfile);
	}
}


void CBRSampleDlg::generateKdeFilesForType(CString type, CString typeName,vector<CString>& vecOriginalFile, vector<CString>& vecSample)
{
	CString dic;
	dic.Format("%s\\%s", strOutputPath, type);
	CreateDirectory(dic.GetString(), NULL);

	int iTotalSampleCount = this->m_listctrlSampleData.GetItemCount();
	int iSampleCount = 0;
	for(int i = 0; i < iTotalSampleCount; i ++)
	{
		if(m_listctrlSampleData.GetItemText(i,typeFieldIndex+1).GetBuffer() == typeName)
			iSampleCount ++;

	}

	CString outputfile;
	for (unsigned int i = 0; i < vecOriginalFile.size(); ++i)
	{
		CThreeDR data;
		data.readin(vecOriginalFile[i].GetBuffer(), 0);

		CString dataname = getCoreFilename(vecOriginalFile[i]);
		outputfile.Format("%s\\%s.txt", dic, dataname);

		//testof << dataname << endl;

		CDistribution kde;
		ofstream output(outputfile);
		output << dataname << endl;
		output << "NumberOfPointsPerLine:\t" << kde.getNumOfPDE() << endl;
		output << "SampleCount:\t" << iSampleCount << endl;
		output << "ID	X	Y	value" << endl;
		//calculate the stdDev of the whole raster
		float* srcArray = data.getDataBuf();
		int num = data.getNumberOfColumns() * data.getNumberOfRows();
		float stdDev = calcStdDev(srcArray, num, data.getNullDataValue());
		int count = num;
		for(int i=0; i<num; i++)
		{
			if(srcArray[i] == data.getNullDataValue())
				--count;
		}
		float bandWidth = (float)(1.06 * stdDev * pow((double)count, -1.0/5.0));
		kde.setBandWidth(bandWidth);

		//testof << "bandwidth: " << bandWidth << endl;

		CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
		int winSize = (int)(pApp->g_dstResolution / data.getCellSize()) + 1;
		for (int j=0; j < this->m_listctrlSampleData.GetItemCount(); j++)
		{

			if(m_listctrlSampleData.GetItemText(j,typeFieldIndex + 1).GetBuffer() == typeName)
			{
				double x = atof(this->m_listctrlSampleData.GetItemText(j,xIndex).GetBuffer());
				double y = atof(this->m_listctrlSampleData.GetItemText(j,yIndex).GetBuffer());
				//find the neigbourhood in the source data
				vector<float> neighbors;
				findNeighborhood(&data, x, y, winSize, neighbors);

				//testof << "size:\t" << neighbors.size() << endl;
				//for (int m = 0; m < neighbors.size(); ++m)
				//{
				//	testof << neighbors[m] << "\t";
				//}

				if (neighbors.size() > 0)
				{
					int num = neighbors.size();
					float* pData = new float[num];
					for(int m = 0; m < num; ++m)
						pData[m] = neighbors[m];
					kde.SetValue(neighbors.size(), pData);
					kde.EstimatePD();
					output << setprecision(6) <<this->m_listctrlSampleData.GetItemText(j,0)<<'\t'
						<< x << "\t" << y << endl;
					for( int i = 0; i < kde.getNumOfPDE(); i++ )
						output << kde.getRangeAt(i) << "\t";
					output << endl;
					for( int i = 0; i < kde.getNumOfPDE(); i++ )
						output << kde.getPDVal(i) << "\t";
					output << endl;
					delete pData;
				}
				else
					output << endl << endl;
			}

			//testof << endl;
		}
		output.close();
		vecSample.push_back(outputfile);
	}
}


void CBRSampleDlg::OnBnClickedOk()
{
	// get the x and y coordiantes index
	UpdateData();
	cbrDeleteDir(strOutputPath.GetBuffer());
	CreateDirectory(strOutputPath, NULL);

	if(this->m_listctrlSampleData.GetItemCount() == 0)
	{
		MessageBox("Sample data should be imported first.");
		return;
	}




	const char* strEx = DBFReadStringAttribute(hDBF, 0, DBFGetFieldIndex(hDBF, xFieldName.GetBuffer()));
	CString ex(strEx);
	if (!isNumeric(ex))
	{
		MessageBox("The X coordinate field should be numeric.");
		return;
	}

	const char* strEy = DBFReadStringAttribute(hDBF, 0, DBFGetFieldIndex(hDBF, yFieldName.GetBuffer()));
	CString ey(strEx);
	if (!isNumeric(ey))
	{
		MessageBox("The Y coordinate field should be numeric.");
		return;
	}

	xIndex = DBFGetFieldIndex(hDBF, xFieldName.GetBuffer()) + 1;
	yIndex = DBFGetFieldIndex(hDBF, yFieldName.GetBuffer()) + 1;

	int iSampleCount=this->m_listctrlSampleData.GetItemCount();


	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();

	if(pApp->iCBROption == 1) //infer soil properties
	{

		if(listSelField.GetCount() == 0)
		{
			MessageBox("At least one property should be selected.");
			return;
		}

		generateAttributeFiles();
		//	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
		if(!pApp->g_vecstrClimateLayers.empty())
			pApp->g_climateSample = generateFiles("climate", pApp->g_vecClimate);
		if(!pApp->g_vecstrParentMaterialLayers.empty())
			pApp->g_parentMaterialSample = generateFiles("geology", pApp->g_vecParentMaterial);

		if(!pApp->g_vecstrTopographicLayers.empty())
		{
			if (pApp->g_useKde)
				generateKdeFiles("topology", pApp->g_vecstrTopographicLayers, pApp->g_vecTopologySample);
			else
				pApp->g_topoSample = generateFiles("topology", pApp->g_vecTopology);
		}

		if(!pApp->g_vecstrVegetationLayers.empty())
		{
			if (pApp->g_useKdeVegetation)
				generateKdeFiles("vegetation", pApp->g_vecstrVegetationLayers, pApp->g_vecVegetationSample);
			else
				pApp->g_vegSample = generateFiles("vegetation", pApp->g_vecVegetation);
		}


		//OnOK();
		this->ShowWindow(SW_HIDE);
		if (!pApp->pSimiDlg)
		{
			pApp->pSimiDlg = new CBRSimilarityDlg();
			pApp->pSimiDlg->Create(IDD_CBR_SIMILAR, NULL);
		}
		pApp->pSimiDlg->ShowWindow(SW_SHOW);
		pApp->pSimiDlg->fillKdeCombo();
	}
	else if(pApp->iCBROption == 2)
	{

		if(listSelType.GetCount() == 0)
		{
			MessageBox("At least one type should be selected.");
			return;
		}

		generateAttributeFilesForType();

		CString str;
		listSelType.GetText(0, str);
		if(!pApp->g_vecstrClimateLayers.empty())
			pApp->g_climateSample = generateFilesForType("climate", str, pApp->g_vecClimate);
		if(!pApp->g_vecstrParentMaterialLayers.empty())
			pApp->g_parentMaterialSample = generateFilesForType("geology",str, pApp->g_vecParentMaterial);

		if(!pApp->g_vecstrTopographicLayers.empty())
		{
			if (pApp->g_useKde)
				generateKdeFilesForType("topology", str, pApp->g_vecstrTopographicLayers, pApp->g_vecTopologySample);
			else
				pApp->g_topoSample = generateFilesForType("topology", str, pApp->g_vecTopology);
		}
		if(!pApp->g_vecstrVegetationLayers.empty())
		{

			if (pApp->g_useKdeVegetation)
				generateKdeFilesForType("vegetation", str, pApp->g_vecstrVegetationLayers, pApp->g_vecVegetationSample);
			else
				pApp->g_vegSample = generateFilesForType("vegetation",str, pApp->g_vecVegetation);
		}


		this->ShowWindow(SW_HIDE);
		if (!pApp->pSimiDlg)
		{
			pApp->pSimiDlg = new CBRSimilarityDlg();
			pApp->pSimiDlg->Create(IDD_CBR_SIMILAR, NULL);
		}
		pApp->pSimiDlg->ShowWindow(SW_SHOW);
		pApp->pSimiDlg->fillKdeCombo();

	}
}

void CBRSampleDlg::OnBnClickedButtonAddfield()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CBRSampleDlg::OnBnClickedButtonDeltefield()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CBRSampleDlg::OnBnClickedButtonAddField()
{
	int i = listAllField.GetCurSel();
	CString str;
	listAllField.GetText(i, str);
	listAllField.DeleteString(i);
	listSelField.AddString(str);
}

void CBRSampleDlg::OnBnClickedButtonAddField2()
{
	int i = listSelField.GetCurSel();
	CString str;
	listSelField.GetText(i, str);
	listSelField.DeleteString(i);
	listAllField.AddString(str);
}

void CBRSampleDlg::OnBnClickedBack()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	this->ShowWindow(SW_HIDE);
	pApp->pGenerateFileDlg->ShowWindow(SW_SHOW);
	pApp->pGenerateFileDlg->resetProcess();
}

void CBRSampleDlg::OnBnClickedCancel()
{
	OnCancel();
	CSoLIMApp* pApp = (CSoLIMApp*)AfxGetApp();
	if(pApp->pSmallScaleDlg)
	{
		delete pApp->pSmallScaleDlg;
		pApp->pSmallScaleDlg = NULL;
	}
	if(pApp->pGenerateFileDlg)
	{
		delete pApp->pGenerateFileDlg;
		pApp->pGenerateFileDlg = NULL;
	}
	if(pApp->pSampleDlg)
	{
		delete pApp->pSampleDlg;
		pApp->pSampleDlg = NULL;
	}
	if(pApp->pSimiDlg)
	{
		delete pApp->pSimiDlg;
		pApp->pSimiDlg = NULL;
	}
	if(pApp->pUncertaintyDlg)
	{
		delete pApp->pUncertaintyDlg;
		pApp->pUncertaintyDlg = NULL;
	}

	if(pApp->pTypeinferDlg)
	{
		delete pApp->pTypeinferDlg;
		pApp->pTypeinferDlg = NULL;
	}
}


void CBRSampleDlg::OnBnClickedButtonAddType()
{

	if(listSelType.GetCount()==0)
	{
		int i = listAllType.GetCurSel();
		CString str;
		listAllType.GetText(i, str);
		listAllType.DeleteString(i);
		listSelType.AddString(str);
	}
	else
	{
		MessageBox("Only one type can be inferred at one time.");
	}
}

void CBRSampleDlg::OnBnClickedButtonAddType2()
{
	int i = listSelType.GetCurSel();
	if(i >= 0)
	{
		CString str;
		listSelType.GetText(i, str);
		listSelType.DeleteString(i);
		listAllType.AddString(str);
	}

}

void CBRSampleDlg::OnCbnSelchangeComboTypeField()
{

	UpdateData();
	// TODO: Add your control notification handler code here
	//added by Fei
	while (listAllType.GetCount() > 0)
		listAllType.DeleteString(0);
	while(listSelType.GetCount() > 0)
		listSelType.DeleteString(0);


	const char* strType = DBFReadStringAttribute(hDBF, 0, DBFGetFieldIndex(hDBF, typeFieldName.GetBuffer()));
	CString ex(strType);


	CString strSel;
	comboTypeField.GetLBText(comboTypeField.GetCurSel(), strSel);



	typeFieldIndex = DBFGetFieldIndex(hDBF, strSel.GetBuffer());

	int nRec = DBFGetRecordCount( hDBF );




	//only add new species to the list
	for (int k = 0; k < nRec; ++k)
	{
		CString newName =  m_listctrlSampleData.GetItemText(k,  typeFieldIndex + 1);
		bool bExists = false;
		for(int m = 0; m < listAllType.GetCount(); m++)
		{
			CString tempName;
			listAllType.GetText(m,tempName);

			if(newName == tempName)
			{
				bExists = true;
				break;
			}
		}
		if(bExists == false)  //this is totally a new species
			listAllType.AddString(newName);
	}



}

void CBRSampleDlg::OnBnClickedRadioProperty()
{
	m_radioProperty.SetCheck(TRUE);
	m_radioType.SetCheck(FALSE);
	UpdateData();

	// TODO: Add your control notification handler code here
	CSoLIMApp* pApp = (CSoLIMApp*)AfxGetApp();
	GetDlgItem(IDC_LIST_ALL_FIELD)->EnableWindow(true);
	GetDlgItem(IDC_LIST_ALL_FIELD)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_ADD_FIELD)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_ADD_FIELD2)->EnableWindow(true);
	GetDlgItem(IDC_COMBO_TYPE_FIELD)->EnableWindow(false);
	GetDlgItem(IDC_LIST_ALL_TYPE)->EnableWindow(false);
	GetDlgItem(IDC_LIST_SEL_TYPE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_ADD_TYPE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_ADD_TYPE2)->EnableWindow(false);


	pApp->iCBROption = 1;
}


void CBRSampleDlg::OnBnClickedRadioType()
{
	m_radioProperty.SetCheck(FALSE);
	m_radioType.SetCheck(TRUE);
	UpdateData();


	CSoLIMApp* pApp = (CSoLIMApp*)AfxGetApp();
	GetDlgItem(IDC_COMBO_TYPE_FIELD)->EnableWindow(true);
	GetDlgItem(IDC_LIST_ALL_TYPE)->EnableWindow(true);
	GetDlgItem(IDC_LIST_SEL_TYPE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_ADD_TYPE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_ADD_TYPE2)->EnableWindow(true);

	GetDlgItem(IDC_LIST_ALL_FIELD)->EnableWindow(false);
	GetDlgItem(IDC_LIST_ALL_FIELD)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_ADD_FIELD)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_ADD_FIELD2)->EnableWindow(false);


	pApp->iCBROption = 2;
}


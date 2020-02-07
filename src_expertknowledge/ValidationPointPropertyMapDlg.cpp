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
// ValidationPointPropertyMapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "ValidationPointPropertyMapDlg.h"
#include ".\ValidationPointPropertyMapdlg.h"
#include "Utility.h"




// CValidationPointPropertyMapDlg 对话框

IMPLEMENT_DYNAMIC(CValidationPointPropertyMapDlg, CDialog)
CValidationPointPropertyMapDlg::CValidationPointPropertyMapDlg(CWnd* pParent /*=NULL*/)
: CDialog(CValidationPointPropertyMapDlg::IDD, pParent)
{

	m_AccuracyReportFileName = _T("");	
	m_PropertyMapFileName = _T("");
	m_Neighborhood = 1;
	//m_Nodata = -9999;
	m_PointListFileName = _T("");
	m_Mask = FALSE;
	m_MaskFileName = _T("");
	m_MaskValue = 0;

}

CValidationPointPropertyMapDlg::~CValidationPointPropertyMapDlg()
{
}

void CValidationPointPropertyMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ValidationProBar, m_ProgressBar);
	DDX_Text(pDX, IDC_PropertyMapFld, m_PropertyMapFileName);
	DDX_Text(pDX, IDC_AccuracyReportFld, m_AccuracyReportFileName);

	DDX_Text(pDX, IDC_ValidationNeighborFld, m_Neighborhood);
	DDV_MinMaxInt(pDX, m_Neighborhood, 1, 100000);
	//DDX_Text(pDX, IDC_ValidationNodataFld, m_Nodata);
	DDX_Text(pDX, IDC_ObservedFld, m_PointListFileName);
	DDX_Check(pDX, IDC_MaskChk, m_Mask);
	DDX_Text(pDX, IDC_MaskFileFld, m_MaskFileName);
	DDX_Text(pDX, IDC_MaskValueFld, m_MaskValue);

}


BEGIN_MESSAGE_MAP(CValidationPointPropertyMapDlg, CDialog)

	ON_BN_CLICKED(IDC_ObservedBtn, OnBnClickedObservedbtn)
	ON_BN_CLICKED(IDC_PropertyMapBtn, OnBnClickedPropertymapbtn)
	ON_BN_CLICKED(IDC_AccuracyReportBtn, OnBnClickedAccuracyreportbtn)

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_MaskFileBtn, OnMaskFileBtn)
	ON_BN_CLICKED(IDC_MaskChk, OnMaskChk)
END_MESSAGE_MAP()



void CValidationPointPropertyMapDlg::OnBnClickedObservedbtn()
{
		UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "Point List Files (*.txt)|*.txt|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_PointListFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CValidationPointPropertyMapDlg::OnBnClickedPropertymapbtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_PropertyMapFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CValidationPointPropertyMapDlg::OnBnClickedAccuracyreportbtn()
{
		// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "Accuracy Report (*.txt)|*.txt|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(FALSE, "txt", "*.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_AccuracyReportFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}


}



void CValidationPointPropertyMapDlg::OnBnClickedOk()
{
		UpdateData(TRUE);
	//GetDlgItem(IDValidationOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	
	if(m_PointListFileName.IsEmpty()){
		MessageBox("No Point list file.");
	
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_PropertyMapFileName.IsEmpty()){
		MessageBox("No property map file.");
	
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_AccuracyReportFileName.IsEmpty()){
		MessageBox("No output accuracy report file.");
	
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_Mask == true)
	{
	   if(this->m_MaskFileName.IsEmpty()){
	     	MessageBox("No mask file.");	
		    m_ProgressBar.SetPos(0);
		    return;
	   }
	}

    

	FILE *fp;
	char record[FN_LEN];

	// Variables for the harden map

	int numRows;			// The number of rows in the hardened image
	int numCols;			// The number of columns in the hardened image
	int ksize;				// The size of the neighbourhood in which to find a mode label
	float xMin;			// The x-minimum in the hardened image
	float yMin;			// The y-minimum in the hardened image
	float cellSize;		// The resolution of the hardened image
	float skipval;			// The category value to be skipped in searching for mode category


	// Variables for the point file

	int  numPoints;		// The number of total points in the point file
	int  numCases;			// The number of points which are valid in the point file
	int  numClasses;		// Number of unique classes in the point file
	char pointID[FN_LEN];	// The ID in the point file
	float x;				// The X coordinate in the point file
	float y;				// The Y coordinate in the point file
    float ObservedProperty;

	// Structures for class data
	CLASSES *classes;		// Structure to store the class IDs and Labels
	CASES *cases;			// Structure to store the inferred and observed classes 
	// at the field points

	// Temporary variables;

	int i, j, k, l, found, row, col, inferred, observed;
	float cat;

	// The error matrix

	int *err_mat;
	float users, producers, overall, khat;
	int total_diagonal, total_in_rc;


	//open the hardened map file
	CThreeDR* theData = new CThreeDR;
	int readflag = theData->readin(m_PropertyMapFileName.GetBuffer(1), 0);
	if(readflag != 0){
	
		m_ProgressBar.SetPos(0);
		return;
	}
	numRows = theData->getNumberOfRows();
	numCols = theData->getNumberOfColumns();
	xMin = theData->getXmin();
	yMin = theData->getYmin();
	cellSize = theData->getCellSize();

	ksize = m_Neighborhood;
	skipval = theData->getNullDataValue();

	m_ProgressBar.SetPos(15);



	
	bool* idBuf = new bool[numRows*numCols];
	for(i=0; i<numRows*numCols; i++)
	{
		idBuf[i]= true;
	}
	if(m_Mask == true)
	{	
		CheckMask(idBuf, numRows,numCols,theData);
	}




	//read point list file
	numPoints = (count_records(m_PointListFileName.GetBuffer())-1 );
	if (numPoints < 0)
	{
		MessageBox("No record in point list file.");
		return;
	}

	cases = new CASES[numPoints];
	classes = new CLASSES[numPoints];

	if (cases == (CASES *) NULL) {
		MessageBox("Out of memory when allocating memory for cases");
		return;
	}

	if ((fp=fopen(m_PointListFileName.GetBuffer(), "r"))==NULL) {
		MessageBox("Can't open point list file.");
		return;
	}

	int headerLength;
	read_record(fp, &headerLength, record); // read off the heading row

	j=0;
	k=0;
	int numMaskOutCases = 0;
	int numNoDataCases = 0;
	int numValidCases = 0;


	for (i=0; i<numPoints; i++) {
		fscanf(fp, "%s %f %f %f", pointID, &x, &y, &ObservedProperty);
		

		row = (int) (floor(((numRows*cellSize+yMin)-y)/cellSize));
		col = (int) (floor((x-xMin)/cellSize));
		// find the most frequent class in the neighbourhood with size of ksize
		get_mean_float(&cat, theData->getDataBuf(), numRows, numCols, row, col, ksize, 1, skipval);

		if(this->m_Mask == true)
		{
			if(idBuf[row*numCols+col] == false)
			{
				cases[j].IsMaskedOut = true;
				cases[j].IsNoData = true;;
				strcpy(cases[j].pointID, pointID);
			  cases[j].x = x;
			  cases[j].y = y;
			  cases[j].ObservedProperty = ObservedProperty;
			   cases[j].InferredProperty = skipval ;
			   j++;
			   numMaskOutCases++;
			   continue;
			
			}

		}

		if(fabs(cat-skipval)<VERYSMALL)
		{	
			cases[j].IsMaskedOut = false;
			cases[j].IsNoData = true;
			strcpy(cases[j].pointID, pointID);
			cases[j].x = x;
			cases[j].y = y;
			cases[j].ObservedProperty = ObservedProperty;
			cases[j].InferredProperty = skipval ;
			j++;
			 numNoDataCases ++;
			
		}	
		else
		{
			cases[j].IsMaskedOut = false;
			cases[j].IsNoData = false;
			strcpy(cases[i].pointID, pointID);
			cases[j].x = x;
			cases[j].y = y;
			cases[j].ObservedProperty = ObservedProperty;
			cases[j].InferredProperty = cat ;
			j++;
			numValidCases ++;
		
		}

		
			   

	}

	fclose(fp);
	numCases = j;

	

	/* outputing the accuracy report file */

	if ((fp=fopen(m_AccuracyReportFileName.GetBuffer(), "w"))==NULL) {
		MessageBox("Can't open accuracy report file.");
		return;

	}

	fprintf(fp, "The Accuracy Report (%s vs %s)\n", 
		m_PropertyMapFileName.GetBuffer(), m_PointListFileName.GetBuffer());

	fprintf(fp, "\nTotal Number of Points:             %d",numCases);
	fprintf(fp, "\nNumber of Points Masked Out:        %d",numMaskOutCases);
	fprintf(fp, "\nNumber of Points with NO DATA:      %d",numNoDataCases);
	fprintf(fp, "\nNumber of Points with Valid Values: %d",numValidCases);

	fprintf(fp, "\n\nResults based on the %d points which have both inferred and observed values:",numValidCases);

	float sum = 0;

	sum = 0;
	for (i=0; i<numCases; i++) {
		if(cases[i].IsNoData == false && cases[i].IsMaskedOut == false)
		  sum += cases[i].ObservedProperty;
	}
	float meanObs = sum/numValidCases;  

    sum = 0;
	for (i=0; i<numCases; i++) {
		if(cases[i].IsNoData == false && cases[i].IsMaskedOut == false)
		   sum += pow(cases[i].InferredProperty - cases[i].ObservedProperty, 2);
	}
	float RMSE = sqrt(sum/numValidCases);

	fprintf(fp, "\n%s %10.6f ", "RMSE: ", RMSE);

	 sum = 0;
	for (i=0; i<numCases; i++) {
		if(cases[i].IsNoData == false && cases[i].IsMaskedOut == false)
		   sum += pow((fabs(cases[i].InferredProperty - meanObs) +fabs(cases[i].ObservedProperty - meanObs)),2);
	}

	float AC = 1-numValidCases*RMSE*RMSE/sum;

	fprintf(fp, "\n%s %10.6f ", "Agreement Coefficient: ", AC);

	
	sum = 0;

	
	for (i=0; i<numCases; i++) {
		if(cases[i].IsNoData == false && cases[i].IsMaskedOut == false)
		  sum += fabs(cases[i].InferredProperty - cases[i].ObservedProperty);
	}
	float MAE = sum/numValidCases;

	fprintf(fp, "\n%s %10.6f ", "Mean Absolute Error: ", MAE);
	

	sum = 0;

	for (i=0; i<numCases; i++) {
		if(cases[i].IsNoData == false && cases[i].IsMaskedOut == false)
		  sum += pow(cases[i].ObservedProperty- meanObs, 2);
	}
	float std = sqrt(sum/numValidCases);

	fprintf(fp, "\n%s %10.6f ", "Standard Deviation of Observed Values: ", std);


	sum = 0;
	for (i=0; i<numCases; i++) {
		if(cases[i].IsNoData == false && cases[i].IsMaskedOut == false)
		  sum += (cases[i].InferredProperty - cases[i].ObservedProperty);
	}
	float ME = sum/numValidCases; 
	fprintf(fp, "\n%s %10.6f ", "Mean Error: ", ME);




    bool bHasMaskedOut = false;
	bool bHasNodata = false;
	bool bHasValid = false;

	fprintf(fp, "\n\nPoints Masked Out \(%d points\)",numMaskOutCases );

		for (i=0; i<numCases; i++) {

			if(cases[i].IsMaskedOut == true) { //mask out
				bHasMaskedOut = true;
				break;
			}
		}
		if(!bHasMaskedOut)
			fprintf(fp, "\n    %(none%)");
		else
		{

			fprintf(fp, "\n    PointID               X               Y               Inferred               Observed");
			for (i=0; i<numCases; i++) {

				if(cases[i].IsMaskedOut == true) { //mask out
					int idDigits = strlen(cases[i].pointID);
					int xDigits = IntegerDigits(cases[i].x);
					int yDigits = IntegerDigits(cases[i].y); 
					int inferDigits = IntegerDigits(cases[i].InferredProperty);

					fprintf(fp, "\n    %s", cases[i].pointID);
					for(j = 0; j< 22 - idDigits; j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].x);

					for(j = 0; j< 16 - (xDigits + 7); j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].y);
					for(j = 0; j< 16 - (yDigits + 7); j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].InferredProperty);
					for(j = 0; j< 23 - (inferDigits+7) ; j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].ObservedProperty);


				}
			}
		}




		fprintf(fp, "\n\nPoints with NO DATA \(%d points\)",numNoDataCases);

		for (i=0; i<numCases; i++) {

			if(cases[i].IsMaskedOut == false && cases[i].IsNoData == true) { //no data
				bHasNodata = true;
				break;
			}
		}
		if(!bHasNodata)
			fprintf(fp, "\n    %(none%)");
		else
		{

			fprintf(fp, "\n    PointID               X               Y               Inferred               Observed");
			for (i=0; i<numCases; i++) {

				if(cases[i].IsMaskedOut == false && cases[i].IsNoData == true)  { //no data
					int idDigits = strlen(cases[i].pointID);
					int xDigits = IntegerDigits(cases[i].x);
					int yDigits = IntegerDigits(cases[i].y); 
					int inferDigits = IntegerDigits(cases[i].InferredProperty);

					fprintf(fp, "\n    %s", cases[i].pointID);
					for(j = 0; j< 22 - idDigits; j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].x);

					for(j = 0; j< 16 - (xDigits + 7); j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].y);
					for(j = 0; j< 16 - (yDigits + 7); j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].InferredProperty);
					for(j = 0; j< 23 - (inferDigits + 7) ; j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].ObservedProperty);


				}
			}
		}

		
		
		fprintf(fp, "\n\nPoints with Inferred and Observed Values \(%d\)", numValidCases);

		for (i=0; i<numCases; i++) {

			if(cases[i].IsMaskedOut == false && cases[i].IsNoData == false) { //valid
				bHasValid = true;
				break;
			}
		}
		if(!bHasValid)
			fprintf(fp, "\n    %(none%)");
		else
		{

			fprintf(fp, "\n    PointID               X               Y               Inferred               Observed");
			for (i=0; i<numCases; i++) {

				if(cases[i].IsMaskedOut == false && cases[i].IsNoData == false)  { //valid
					int idDigits = strlen(cases[i].pointID);
					int xDigits = IntegerDigits(cases[i].x);
					int yDigits = IntegerDigits(cases[i].y); 

					int inferDigits = IntegerDigits(cases[i].InferredProperty);

					fprintf(fp, "\n    %s", cases[i].pointID);
					for(j = 0; j< 22 - idDigits; j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].x);

					for(j = 0; j< 16 - (xDigits + 7); j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].y);
					for(j = 0; j< 16 - (yDigits + 7); j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].InferredProperty);
					for(j = 0; j< 23 - (inferDigits + 7) ; j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].ObservedProperty);


				}
			}
		}




	
	fclose(fp);

	if(idBuf != NULL)
	{
		delete []idBuf;
		idBuf = NULL;
	}
	
	if(cases != NULL)
	{
		delete []cases;
		cases = NULL;
	}
		

	m_ProgressBar.SetPos(100);
	CDialog::OnOK();

}

void CValidationPointPropertyMapDlg::OnBnClickedCancel()
{

	OnCancel();
}

void CValidationPointPropertyMapDlg::CheckMask(bool* idBuf, int nrow, int ncol,CThreeDR *theData){ //Check if mask should be used
	//int nrow = theEnvDatabase->theSetting->nrow;
	//int ncol = theEnvDatabase->theSetting->ncol;
	int numPixels = nrow*ncol;
	if(m_MaskFileName.IsEmpty()){
		//MessageBox("No mask file specified.");
		m_Mask = false;
	}
	else{
		CThreeDR* maskFile = new CThreeDR;
		int readflag = maskFile->readin(m_MaskFileName.GetBuffer(100), 0);
		if(readflag > 0){
			MessageBox("Error in reading mask file. No mask used.");
			m_Mask = false;
		}
		else{
			if(!maskFile->spatialRangeMatch(theData)){
				MessageBox("The mask file and the first layer . No mask used.");
				m_Mask = false;
			}
			else{
				for(int i=0; i<numPixels; i++)
					if(fabs(maskFile->getDataBuf()[i]-m_MaskValue)<VERYSMALL)
						idBuf[i] = false;
			}
		}
		delete maskFile;
	}
}


void CValidationPointPropertyMapDlg::OnMaskChk(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_Mask){
		GetDlgItem(IDC_MaskFileFld)->EnableWindow(TRUE);
		GetDlgItem(IDC_MaskFileBtn)->EnableWindow(TRUE);
		GetDlgItem(IDC_MaskValueLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_MaskValueFld)->EnableWindow(TRUE);
	}
	else{
		GetDlgItem(IDC_MaskFileFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_MaskFileBtn)->EnableWindow(FALSE);
		GetDlgItem(IDC_MaskValueLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_MaskValueFld)->EnableWindow(FALSE);
	}
}

void CValidationPointPropertyMapDlg::OnMaskFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_MaskFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}


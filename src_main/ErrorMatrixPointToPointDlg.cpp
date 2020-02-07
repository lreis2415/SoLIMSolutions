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
// ErrorMatrixPointToPointDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "ErrorMatrixPointToPointDlg.h"
#include ".\ErrorMatrixPointToPointdlg.h"
#include "Utility.h"


// CErrorMatrixPointToPointDlg 对话框

IMPLEMENT_DYNAMIC(CErrorMatrixPointToPointDlg, CDialog)
CErrorMatrixPointToPointDlg::CErrorMatrixPointToPointDlg(CWnd* pParent /*=NULL*/)
: CDialog(CErrorMatrixPointToPointDlg::IDD, pParent)
{
	m_MatrixFileName = _T("");
	m_AccuracyReportFileName = _T("");
	m_MismatchFileName = _T("");
	

	//m_Nodata = -9999;
	m_ObservedFileName = _T("");
	m_InferredFileName = _T("");
}

CErrorMatrixPointToPointDlg::~CErrorMatrixPointToPointDlg()
{
}

void CErrorMatrixPointToPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ErrorMatrixProBar, m_ProgressBar);
	DDX_Text(pDX, IDC_ErrorMatrixMatrixFld, m_MatrixFileName);

	
	DDX_Text(pDX, IDC_MismatchListFld, m_MismatchFileName);
	DDX_Text(pDX, IDC_AccuracyReportFld, m_AccuracyReportFileName);



	//DDX_Text(pDX, IDC_NodataFld, m_Nodata);
	DDX_Text(pDX, IDC_ObservedFld, m_ObservedFileName);
	DDX_Text(pDX, IDC_InferredFld, m_InferredFileName);
}


BEGIN_MESSAGE_MAP(CErrorMatrixPointToPointDlg, CDialog)
	//ON_BN_CLICKED(IDErrorMatrixOK, OnBnClickedErrormatrixok)
	ON_BN_CLICKED(IDC_ObservedBtn, OnBnClickedObservedbtn)
	ON_BN_CLICKED(IDC_InferredBtn, OnBnClickedInferredbtn)

	ON_BN_CLICKED(IDC_ErrorMatrixMatrixBtn, OnBnClickedErrormatrixmatrixbtn)
	//ON_BN_CLICKED(IDErrorMatrixCancel, OnBnClickedErrormatrixcancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CErrorMatrixPointToPointDlg 消息处理程序



void CErrorMatrixPointToPointDlg::OnBnClickedObservedbtn()
{
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "Point List Files (*.txt)|*.txt|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_ObservedFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CErrorMatrixPointToPointDlg::OnBnClickedInferredbtn()
{
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "Point List Files (*.txt)|*.txt|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_InferredFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}



void CErrorMatrixPointToPointDlg::OnBnClickedErrormatrixmatrixbtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "All Files (*.*)|*.*||";
	CFileDialog m_ldFile(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		CString str = m_ldFile.GetPathName();

		m_MatrixFileName = str + "_Matrix.csv";

		m_AccuracyReportFileName = str + "_Report.txt";

		m_MismatchFileName = str + "_Mismatch.csv";

		UpdateData(FALSE);
	}
}


void CErrorMatrixPointToPointDlg::OnBnClickedOk()
{
		UpdateData(TRUE);
	//GetDlgItem(IDErrorMatrixOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	//check input fields on the dialogbox

	if(m_ObservedFileName.IsEmpty()){
		MessageBox("No observed point list file.");
		GetDlgItem(IDErrorMatrixOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	if(m_InferredFileName.IsEmpty()){
		MessageBox("No inferred point list file.");
		GetDlgItem(IDErrorMatrixOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	
	if(m_MatrixFileName.IsEmpty()){
		MessageBox("No output error matrix file.");
		GetDlgItem(IDErrorMatrixOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}


	FILE *fp;
	char record[FN_LEN];

	
	float skipval;			// The category value to be skipped in searching for mode category


	// Variables for the point file

	int  numPoints;		// The number of total points in the point file
	int  numCases;			// The number of points which are valid in the point file
	int  numClasses;		// Number of unique classes in the point file
	char pointID[FN_LEN];	// The ID in the point file
	float x;				// The X coordinate in the point file
	float y;				// The Y coordinate in the point file
	int  soilID;			// The hardened label for the input file
	char soilName[FN_LEN];	// The observation in the point file

	// Structures for class data

	CASES *cases;			// Structure to store the observed classes 
	CLASSES *classes;		// Structure to store the class IDs and Labels
	// at the field points

	// Temporary variables;

	int i, j, k, l, found, inferred, observed;
	float cat;

	// The error matrix

	int *err_mat;
	float users, producers, overall, khat;
	int total_diagonal, total_in_rc;


	
	//skipval = m_Nodata;

	m_ProgressBar.SetPos(15);

	
	//read point list file
	int numObservedPoints = (count_records(m_ObservedFileName.GetBuffer())-1 );
	int numInferredPoints = (count_records(m_InferredFileName.GetBuffer())-1 );
	if(numObservedPoints != numInferredPoints)
	{
		MessageBox("The numbers of records should be identical in observed point list file and inferred point list file.");
		return;
	}



	if (numObservedPoints < 0)
	{
		MessageBox("No record in observed point list file.");
		return;
	}
	if (numInferredPoints < 0)
	{
		MessageBox("No record in inferred point list file.");
		return;
	}

	numPoints = numObservedPoints;

	cases = new CASES[numPoints];
	classes = new CLASSES[2*numPoints];


	if (cases == (CASES *) NULL) {
		MessageBox("Out of memory when allocating memory for cases");
		return;
	}


	//read observed file
	if ((fp=fopen(m_ObservedFileName.GetBuffer(), "r"))==NULL) {
		MessageBox("Can't open observed point list file.");
		return;
	}

	int headerLength;
	 read_record(fp, &headerLength, record); // read off the heading row
	j=0;
	k=0;
	for (i=0; i<numPoints; i++) {
		fscanf(fp, "%s %f %f %s", pointID, &x, &y, soilName);		
		strcpy(cases[i].pointID, pointID);
			cases[i].x = x;
			cases[i].y = y;
			strcpy(cases[i].observedSoilName, soilName);

			// building a list of unique soil classes

			// first element;
			if (k==0) {
				classes[k].ClassID = k;
				strcpy(classes[k].ClassLabel, soilName);
				cases[i].Observed = k;
				k++;
			}
			else {
				// subsequent element;
				found=0;
				for (l=0; l<k; l++) {
					if (strcmp(classes[l].ClassLabel, soilName) == 0) {
						found=1;
						cases[i].Observed = l;
						break;
					}
				}
				if (found==0) {
					classes[k].ClassID = k;
					strcpy(classes[k].ClassLabel, soilName);
					cases[i].Observed = k;
					k++;
				}
			}
	}

	fclose(fp);

	//read inferred file
	if ((fp=fopen(m_InferredFileName.GetBuffer(), "r"))==NULL) {
		MessageBox("Can't open inferred point list file.");
		return;
	}

	read_record(fp, &headerLength, record); // read off the heading row


	for (i=0; i<numPoints; i++) {
		fscanf(fp, "%s %f %f %s", pointID, &x, &y, soilName);	
		if(fabs(cases[i].x - x) > VERYSMALL)
		{
			CString str;
			str.Format("The coordinates of point %d do not match.",i); 
			MessageBox(str);
			return;
		}

		if(fabs(cases[i].y - y) > VERYSMALL)
		{
			CString str;
			str.Format("The coordinates of point %d do not match.",i); 
			MessageBox(str);
			return;
		}
	
		
		strcpy(cases[i].inferredSoilName, soilName);

		found=0;
		for (l=0; l<k; l++) {
			if (strcmp(classes[l].ClassLabel, soilName) == 0) {
				found=1;
				cases[i].Inferred = l;
				break;
			}
		}
		if (found==0) {
			classes[k].ClassID = k;		
			strcpy(classes[k].ClassLabel, soilName);
			cases[i].Inferred = k;
			k++;
		}

	}

	fclose(fp);

	numCases = numPoints;
	numClasses = k;




	if ((err_mat=(int *)malloc(((numClasses+1)*(numClasses+1)*sizeof(int))))
		==NULL)
	{
		printf("\n\n\tOut of memory while allocating mem for err_mat buffer");
		exit(-1);
	}

	for (i=0; i<=numClasses; i++) {
		for (j=0; j<=numClasses; j++) {
			err_mat[i*(numClasses+1)+j]=0;
		}
	}

	// fill the matrix

	for (i=0; i<numCases; i++) {
		for (k=0; k<numClasses; k++) {
			if (cases[i].Inferred==classes[k].ClassID) {
				inferred=k;
			}
			if (cases[i].Observed==classes[k].ClassID) {
				observed=k;
			}
		}
		err_mat[inferred*(numClasses+1)+observed]++;
	}

	// compute the total for each row and store it
	// in the last column. This is used for computing
	// user's accuracy for each class
	for (i=0; i<numClasses; i++) {
		for (j=0; j<numClasses; j++) {
			err_mat[i*(numClasses+1)+numClasses]
			=err_mat[i*(numClasses+1)+numClasses]
			+err_mat[i*(numClasses+1)+j];
		}
	}

	// compute the total for each column and store it
	// in the last row. This is used for computing
	// the producer's accuracy for each class

	for (j=0; j<numClasses; j++) {
		for (i=0; i<numClasses; i++) {
			err_mat[numClasses*(numClasses+1)+j]
			=err_mat[numClasses*(numClasses+1)+j]
			+err_mat[i*(numClasses+1)+j];
		}
	}


	// compute the total along the diagonal

	total_diagonal=0;
	total_in_rc=0;
	for (i=0; i<numClasses; i++)
	{
		total_diagonal=total_diagonal+err_mat[i*(numClasses+1)+i];
		total_in_rc=total_in_rc+err_mat[i*(numClasses+1)+numClasses]
		*err_mat[numClasses*(numClasses+1)+i];
	}
	err_mat[numClasses*(numClasses+1)+numClasses] = total_diagonal;




	/* output error matrix file */

	if ((fp=fopen(m_MatrixFileName.GetBuffer(), "w"))==NULL) {
		MessageBox("Can't open error matrix file.");
		return;
	}

	fprintf(fp, "The Error Matrix (%s vs %s)", 
		m_InferredFileName.GetBuffer(), m_ObservedFileName.GetBuffer());
	for(i=0;i<numClasses+2;i++)
		fprintf(fp, "%s", ",");

	fprintf(fp, "\nInferred\\Observed,");

	for (i=0; i<numClasses; i++)
	{
		fprintf(fp, "%10s,", classes[i].ClassLabel);
	}

	fprintf(fp, "%10s,%12s", "Row Total", "User's");

	float *usersAccuracy = new float[numClasses];
	float *producersAccuracy = new float[numClasses];

	// compute users' accuracy
	users=0.0;
	for (i=0; i<numClasses; i++)
	{

		fprintf(fp, "\n%12s,", classes[i].ClassLabel);
		for (j=0; j<=numClasses; j++)
		{

			fprintf(fp, "%10d,", err_mat[i*(numClasses+1)+j]);
		}
		if (err_mat[i*(numClasses+1)+numClasses]==0) {
			users=0.0;
		}
		else {
			users=((float)err_mat[i*(numClasses+1)+i])
				/((float)err_mat[i*(numClasses+1)+numClasses]);
		}

		fprintf(fp, "%10.6f", users);
		usersAccuracy[i] = users;
	}


	fprintf(fp, "\n%12s,", "Col Total");
	for (j=0; j<=numClasses; j++)
	{

		fprintf(fp, "%10d,", err_mat[numClasses*(numClasses+1)+j]);
	}

	// compute producers' accuracy

	producers=0.0;
	fprintf(fp, "\n%12s,", "Producers");
	for (j=0; j<numClasses; j++) {
		if (err_mat[numClasses*(numClasses+1)+j]==0) {
			producers=0.0;
		}
		else {
			producers=((float)err_mat[j*(numClasses+1)+j])
				/((float)err_mat[numClasses*(numClasses+1)+j]);
		}
		if(j == numClasses-1)
			fprintf(fp, "%10.6f", producers);
		else
			fprintf(fp, "%10.6f,", producers);
		producersAccuracy[j] = producers;
	}

	// compute overall accuracy
	overall=0.0;
	overall=(float)total_diagonal
		/(float)numCases; 


	fprintf(fp, "\n%12s %10.6f", "Overall Accuracy: ", overall);
	for(i=0;i<numClasses+2;i++)
		fprintf(fp, "%s", ",");

	// compute kappa

	khat = ((float)numCases*total_diagonal-(float)total_in_rc)
		/((float)(numCases*numCases)-(float)(total_in_rc)); 
	fprintf(fp, "\n%12s %10.6f", "KAPPA: ", khat);
	for(i=0;i<numClasses+2;i++)
		fprintf(fp, "%s", ",");

	fclose(fp);



	/* outputing the accuracy report file */

	if ((fp=fopen(m_AccuracyReportFileName.GetBuffer(), "w"))==NULL) {
		MessageBox("Can't open accuracy report file.");
		return;

	}

	fprintf(fp, "The Accuracy Report (%s vs %s)\n", 
		m_InferredFileName.GetBuffer(), m_ObservedFileName.GetBuffer());

	fprintf(fp, "\nTotal Number of Points: %d",numCases);

	fprintf(fp, "\n%s %10.6f ", "Overall Accuracy: ", overall);

	fprintf(fp, "\n%s %10.6f ", "KAPPA: ", khat);
	for(k=0; k<numClasses; k++)
	{
		fprintf(fp, "\n\n%s", classes[k].ClassLabel);	
		fprintf(fp, "\n  User's Accuracy:%1.6f", usersAccuracy[k]);
		fprintf(fp, "\n  Producer's Accuracy:%1.6f",producersAccuracy[k]);
		

		bool bHasCorrect = false;
		bool bHasMismatch = false;

		fprintf(fp, "\n  Correct");
		for (i=0; i<numCases; i++) {

			if (cases[i].Inferred==classes[k].ClassID && cases[i].Observed==classes[k].ClassID) { //correct
				bHasCorrect = true;
				break;
			}
		}
		if(!bHasCorrect)
			fprintf(fp, "\n    %(none%)");
		else
		{

			fprintf(fp, "\n    PointID               X               Y               Inferred               Observed");

			for (i=0; i<numCases; i++) {

				if (cases[i].Inferred==classes[k].ClassID && cases[i].Observed==classes[k].ClassID) { //correct
					int idDigits = strlen(cases[i].pointID);
					int xDigits = IntegerDigits(cases[i].x);
					int yDigits = IntegerDigits(cases[i].y); 
					int inferDigits = strlen(cases[i].inferredSoilName);

					fprintf(fp, "\n    %s", cases[i].pointID);
					for(j = 0; j< 22 - idDigits; j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].x);

					for(j = 0; j< 16 - (xDigits + 7); j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].y);
					for(j = 0; j< 16 - (yDigits + 7); j++)
						fprintf(fp, " ");
					fprintf(fp, "%s", cases[i].inferredSoilName);
					for(j = 0; j< 23 - inferDigits ; j++)
						fprintf(fp, " ");
					fprintf(fp, "%s", cases[i].observedSoilName);


				}
			}
		}

		fprintf(fp, "\n  Mismatch");

		for (i=0; i<numCases; i++) {


			if(cases[i].Inferred!=classes[k].ClassID&&cases[i].Observed==classes[k].ClassID)  { //correct
				bHasMismatch = true;
				break;
			}
		}
		if(!bHasMismatch)
			fprintf(fp, "\n    %(none%)");
		else
		{

			fprintf(fp, "\n    PointID               X               Y               Inferred               Observed");
			for (i=0; i<numCases; i++) {

				if(cases[i].Inferred!=classes[k].ClassID&&cases[i].Observed==classes[k].ClassID) { //mismatch
					int idDigits = strlen(cases[i].pointID);
					int xDigits = IntegerDigits(cases[i].x);
					int yDigits = IntegerDigits(cases[i].y); 
					int inferDigits = strlen(cases[i].inferredSoilName);

					fprintf(fp, "\n    %s", cases[i].pointID);
					for(j = 0; j< 22 - idDigits; j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].x);

					for(j = 0; j< 16 - (xDigits + 7); j++)
						fprintf(fp, " ");
					fprintf(fp, "%f", cases[i].y);
					for(j = 0; j< 16 - (yDigits + 7); j++)
						fprintf(fp, " ");
					fprintf(fp, "%s", cases[i].inferredSoilName);
					for(j = 0; j< 23 - inferDigits ; j++)
						fprintf(fp, " ");
					fprintf(fp, "%s", cases[i].observedSoilName);
				}
			}
		}
	}


	fclose(fp);



	if ((fp=fopen(m_MismatchFileName.GetBuffer(), "w"))==NULL) {
		MessageBox("Can't open mismatch file.");
		return;

	}

	fprintf(fp, "The Mismatch Point List (%s vs %s)", 
		m_InferredFileName.GetBuffer(), m_ObservedFileName.GetBuffer());
	fprintf(fp, "\n    PointID,X,Y,Inferred,Observed");



	for (i=0; i<numCases; i++) {

		if (cases[i].Inferred!=cases[i].Observed) {
			fprintf(fp, "\n%s,%f,%f", 	cases[i].pointID, cases[i].x, cases[i].y);
			fprintf(fp, ",%s,%s", 	cases[i].inferredSoilName,cases[i].observedSoilName);
		}

	}

	fclose(fp);

	


	if(cases != NULL)
	{
		delete []cases;
		cases = NULL;
	}
	if(classes !=NULL)
	{
		delete []classes;
		classes = NULL;
	}


	m_ProgressBar.SetPos(100);
	CDialog::OnOK();

}

void CErrorMatrixPointToPointDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

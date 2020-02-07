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
// ErrorMatrixPntLabMapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "ErrorMatrixPntLabMapDlg.h"
#include ".\ErrorMatrixPntLabMapdlg.h"
#include "Utility.h"






// CErrorMatrixPntLabMapDlg 对话框

IMPLEMENT_DYNAMIC(CErrorMatrixPntLabMapDlg, CDialog)
CErrorMatrixPntLabMapDlg::CErrorMatrixPntLabMapDlg(CWnd* pParent /*=NULL*/)
: CDialog(CErrorMatrixPntLabMapDlg::IDD, pParent)

{
	m_MatrixFileName = _T("");
	m_AccuracyReportFileName = _T("");
	m_MismatchFileName = _T("");
	m_HardenedMapFileName = _T("");
	m_Neighborhood = 1;
	//m_Nodata = -9999;
	m_PntFileName = _T("");
	m_LookupFileName = _T("");
	m_LabFileName = _T("");

		m_Mask = FALSE;
	m_MaskFileName = _T("");
	m_MaskValue = 0;
}

CErrorMatrixPntLabMapDlg::~CErrorMatrixPntLabMapDlg()
{
}

void CErrorMatrixPntLabMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ErrorMatrixProBar, m_ProgressBar);
	DDX_Text(pDX, IDC_ErrorMatrixMatrixFld, m_MatrixFileName);

	DDX_Text(pDX, IDC_ErrorMatrixHardenedMapFld, m_HardenedMapFileName);
	DDX_Text(pDX, IDC_MismatchListFld, m_MismatchFileName);
	DDX_Text(pDX, IDC_AccuracyReportFld, m_AccuracyReportFileName);

	DDX_Text(pDX, IDC_ErrorMatrixNeighborFld, m_Neighborhood);
	DDV_MinMaxInt(pDX, m_Neighborhood, 1, 100000);
	//DDX_Text(pDX, IDC_ErrorMatrixNodataFld, m_Nodata);
	DDX_Text(pDX, IDC_ErrorMatrixPntFileFld, m_PntFileName);
	DDX_Text(pDX, IDC_LookupTableFld, m_LookupFileName);
	DDX_Text(pDX, IDC_ErrorMatrixLabFileFld, m_LabFileName);

	DDX_Check(pDX, IDC_MaskChk, m_Mask);
	DDX_Text(pDX, IDC_MaskFileFld, m_MaskFileName);
	DDX_Text(pDX, IDC_MaskValueFld, m_MaskValue);
}


BEGIN_MESSAGE_MAP(CErrorMatrixPntLabMapDlg, CDialog)
	ON_BN_CLICKED(IDErrorMatrixOK, OnBnClickedErrormatrixok)
	//ON_BN_CLICKED(IDC_ObservedBtn, OnBnClickedObservedbtn)
	ON_BN_CLICKED(IDC_LookupTableBtn, OnBnClickedLookuptablebtn)
	ON_BN_CLICKED(IDC_ErrorMatrixHardenedMapBtn, OnBnClickedErrormatrixhardenedmapbtn)
	ON_BN_CLICKED(IDC_ErrorMatrixMatrixBtn, OnBnClickedErrormatrixmatrixbtn)
	ON_BN_CLICKED(IDErrorMatrixCancel, OnBnClickedErrormatrixcancel)
	ON_BN_CLICKED(IDC_ErrorMatrixPntFileBtn, OnBnClickedErrormatrixpntfilebtn)
	ON_BN_CLICKED(IDC_ErrorMatrixLblFileBtn, OnBnClickedErrormatrixlblfilebtn)
		ON_BN_CLICKED(IDC_MaskFileBtn, OnMaskFileBtn)
	ON_BN_CLICKED(IDC_MaskChk, OnMaskChk)
END_MESSAGE_MAP()


// CErrorMatrixPntLabMapDlg 消息处理程序

void CErrorMatrixPntLabMapDlg::OnBnClickedErrormatrixok()
{
	UpdateData(TRUE);
	//GetDlgItem(IDErrorMatrixOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	//check input fields on the dialogbox
	if(m_LookupFileName.IsEmpty()){
		MessageBox("No lookup table file.");
		GetDlgItem(IDErrorMatrixOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_PntFileName.IsEmpty()){
		MessageBox("No pnt file.");
		GetDlgItem(IDErrorMatrixOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	if(m_LabFileName.IsEmpty()){
		MessageBox("No lable file.");
		GetDlgItem(IDErrorMatrixOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_HardenedMapFileName.IsEmpty()){
		MessageBox("No hardened map file.");
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
	int  soilID;			// The hardened label for the input file
	char soilName[FN_LEN];	// The observation in the point file

	int classID;

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
	int readflag = theData->readin(m_HardenedMapFileName.GetBuffer(1), 0);
	if(readflag != 0){
		GetDlgItem(IDErrorMatrixOK)->EnableWindow(TRUE);
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




	//read the lookup file and build the list 
	int numLookupItems = (count_records(m_LookupFileName.GetBuffer())-1);
	if(numLookupItems < 0)
	{
		MessageBox("No record in look-up table file.");
		return;
	}

	if ((fp=fopen(m_LookupFileName.GetBuffer(), "r"))==NULL) {
		MessageBox("Can't open look-up talbe file");
		return;
	}

	int headerLength;
	read_record(fp, &headerLength, record); // read off the heading row

	int m = 0;
	CLASSES *  LookupClasses;		// Structure to store the class IDs and Labels
	LookupClasses = new CLASSES[numLookupItems];
	for (i=1; i<=numLookupItems; i++) {
		fscanf(fp, "%d %s", &soilID, soilName);

		if (m==0) {
			//the first element
			LookupClasses[m].ClassID = soilID;
			strcpy( LookupClasses[m].ClassLabel, soilName);
			m++;
		}
		else {
			// subsequent element;
			found=0;
			for (l=0; l<m; l++) {
				if (strcmp(LookupClasses[l].ClassLabel,soilName)==0 && LookupClasses[l].ClassID != soilID) {

					MessageBox("There are duplicated soil names with different IDs. Please check your look-up table file.");
					return;
				}
			}


			if (found==0) {  
				LookupClasses[m].ClassID = soilID;
				strcpy(LookupClasses[m].ClassLabel, soilName);
				m++;
			}
		}
	}


	//read lab  file

	int numLabItems = (count_records(m_LabFileName.GetBuffer())-1);
	if(numLabItems < 0)
	{
		MessageBox("No record in label file.");
		return;
	}

	if ((fp=fopen(m_LabFileName.GetBuffer(), "r"))==NULL) {
		MessageBox("Can't open label talbe");
		return;
	}
	read_record(fp, &headerLength, record); // read off the heading row

	int t = 0;
	CLASSES *  LabClasses;		// Structure to store the class IDs and Labels
	LabClasses = new CLASSES[numLabItems];
	for (i=1; i<=numLabItems; i++) {
		fscanf(fp, "%d %s", &classID, soilName);

		if (t==0) {
			//the first element
			LabClasses[t].ClassID = classID;
			strcpy( LabClasses[t].ClassLabel, soilName);
			t++;
		}
		else {
			// subsequent element;
			found=0;
			for (l=0; l<t; l++) {
				if (strcmp(LabClasses[l].ClassLabel,soilName)!=0 && LabClasses[l].ClassID == classID) {

					MessageBox("There are duplicated IDs with different soil names. Please check your label file.");
					return;
				}
			}

			if (found==0) {  
				LabClasses[t].ClassID = classID;
				strcpy(LabClasses[t].ClassLabel, soilName);
				t++;
			}
		}
	}




	//read pnt file
	numPoints = (count_records(m_PntFileName.GetBuffer())-1 );
	if (numPoints < 0)
	{
		MessageBox("No record in point list file.");
		return;
	}

	cases = new CASES[numPoints];
	classes = new CLASSES[2*numPoints];

	if (cases == (CASES *) NULL) {
		MessageBox("Out of memory when allocating memory for cases");
		return;
	}

	if ((fp=fopen(m_PntFileName.GetBuffer(), "r"))==NULL) {
		MessageBox("Can't open point list file.");
		return;
	}
	read_record(fp, &headerLength, record); // read off the heading row

	// read off the heading row
	//read_record(fp, &soilID, record);

	j=0;
	k=0;

	int numMaskOutCases = 0;
	int numNoDataCases = 0;
	int numValidCases = 0;

	for (i=1; i<=numPoints; i++) {
		fscanf(fp, "%d %f %f", &classID, &x, &y);

		//attach soil name by using lab file
		found = 0;
		for(int n=0; n<t;n++)
		{
			if(classID == LabClasses[n].ClassID)
			{
				found = 1;
				break;
			}
		}
		if(found == 0)
			strcpy(soilName, "UnknownType");
		else
			strcpy(soilName, LabClasses[n].ClassLabel);

       //find the soil type in look up table
		found = 0;
		for(int n=0;n<m;n++)
		{
			if(strcmp(soilName,LookupClasses[n].ClassLabel)==0) 
			{
				found = 1;
				break;
			}
		}
		if(found == 0)  
			soilID = skipval;
		else	
			soilID = LookupClasses[n].ClassID;


		row = (int) (floor(((numRows*cellSize+yMin)-y)/cellSize));
		col = (int) (floor((x-xMin)/cellSize));
		// find the most frequent class in the neighbourhood with size of ksize
		get_mode_float(&cat, theData->getDataBuf(), numRows, numCols, row, col, ksize, 1, skipval);

		if(this->m_Mask == true)
		{
			if(idBuf[row*numCols+col] == false)  //MASK
			{
				cases[j].IsMaskedOut = true;
				 sprintf(cases[j].pointID, "%d", classID);
		
				cases[j].x = x;
				cases[j].y = y;
				cases[j].Observed = soilID;
				cases[j].Inferred = skipval;

				strcpy(cases[j].inferredSoilName, "MASKOUT");       

				strcpy(cases[j].observedSoilName, soilName);
				j++;
				numMaskOutCases++;
				continue;

			}

		}

		if(fabs(cat-skipval)<VERYSMALL)
		{	
			cases[j].IsMaskedOut = false;
			cases[j].IsNoData = true;
			 sprintf(cases[j].pointID, "%d", classID);
			cases[j].x = x;
			cases[j].y = y;
			cases[j].Observed = soilID;
			cases[j].Inferred = skipval;
			//sprintf(record, "UnknownType%d", cases[j].Inferred);
			strcpy(cases[j].inferredSoilName, "NODATA");       

			strcpy(cases[j].observedSoilName, soilName);
			j++;
			numNoDataCases ++;

		}	
		else
		{

			// building a list of unique soil classes

			// first element;
			if (k==0) {
				classes[k].ClassID = soilID;
				strcpy(classes[k].ClassLabel, soilName);
				k++;
			}
			else {
				// subsequent element;
				found=0;
				for (l=0; l<k; l++) {
					if (classes[l].ClassID == soilID) {
						found=1;
						break;
					}
				}
				if (found==0) {
					classes[k].ClassID = soilID;
					strcpy(classes[k].ClassLabel, soilName);
					k++;
				}
			}

			cases[j].IsMaskedOut = false;
			cases[j].IsNoData = false;

			 sprintf(cases[j].pointID, "%d", classID);
			cases[j].x = x;
			cases[j].y = y;
			cases[j].Observed = soilID;
			cases[j].Inferred = cat;

			found = 0;
			for(int n=0;n<m;n++)
			{
				if(cat == LookupClasses[n].ClassID) //find the soil type in look up table
				{

					if(found == 0)
						strcpy(cases[j].inferredSoilName,LookupClasses[n].ClassLabel);
					else
					{
						strcat(cases[j].inferredSoilName,"|");
						strcat(cases[j].inferredSoilName,LookupClasses[n].ClassLabel);
					}

					found = 1;
					//break;
				}
			}
			if(found == 0)  
			{ 
				sprintf(record, "UnknownType%d", cases[j].Inferred);
				strcpy(cases[j].inferredSoilName, record);
			}


			cases[j].Observed = soilID;
			strcpy(cases[j].observedSoilName, soilName);
			j++;
			numValidCases ++;

		}


	}

	fclose(fp);
	numCases = j;




   for (i=0; i<numCases; i++) {
		if(cases[i].IsNoData == false && cases[i].IsMaskedOut == false)
		{

			found=0;
			for (l=0; l<k; l++) {
				if (classes[l].ClassID == cases[i].Inferred) {
					found=1;
					break;
				}
			}
			if (found==0) {
				classes[k].ClassID = cases[i].Inferred;
				sprintf(record, "UnknownType%d", cases[i].Inferred);
				strcpy(classes[k].ClassLabel, record);
				k++;
			}
		}
	}

	numClasses=k;




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
		if(cases[i].IsMaskedOut == false && cases[i].IsNoData == false)
		{
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
		m_HardenedMapFileName.GetBuffer(), m_PntFileName.GetBuffer());
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
		/(float)numValidCases; 


	fprintf(fp, "\n%12s %10.6f", "Overall Accuracy: ", overall);
	for(i=0;i<numClasses+2;i++)
		fprintf(fp, "%s", ",");

	// compute kappa

	khat = ((float)numValidCases*total_diagonal-(float)total_in_rc)
		/((float)(numValidCases*numValidCases)-(float)(total_in_rc)); 
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
		m_HardenedMapFileName.GetBuffer(), m_PntFileName.GetBuffer());

	fprintf(fp, "\nTotal Number of Points:             %d",numCases);
	fprintf(fp, "\nNumber of Points Masked Out:        %d",numMaskOutCases);
	fprintf(fp, "\nNumber of Points with NO DATA:      %d",numNoDataCases);
	fprintf(fp, "\nNumber of Points with Valid Values: %d",numValidCases);

	fprintf(fp, "\n\nResults based on the %d points which have both inferred and observed values:",numValidCases);

	fprintf(fp, "\n\n%s %10.6f ", "Overall Accuracy: ", overall);

	fprintf(fp, "\n%s %10.6f ", "KAPPA: ", khat);

	bool bHasMaskedOut = false;
	bool bHasNodata = false;
	bool bHasValid = false;

	fprintf(fp, "\n\nPoints Masked Out \(%d points\) ",numMaskOutCases );

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


	fprintf(fp, "\n\nFor the %d Valid Points:",numValidCases);


	for(k=0; k<numClasses; k++)
	{
		fprintf(fp, "\n\n%s %(%d%)", classes[k].ClassLabel,classes[k].ClassID);
		fprintf(fp, "\n  User's Accuracy:%1.6f", usersAccuracy[k]);
		fprintf(fp, "\n  Producer's Accuracy:%1.6f",producersAccuracy[k]);

		int numCorrect = 0;
	    int numMismatch = 0;

		fprintf(fp, "\n  Correct");
		for (i=0; i<numCases; i++) {
			if(cases[i].IsMaskedOut == false && cases[i].IsNoData == false)
			{

				if (cases[i].Inferred==classes[k].ClassID && cases[i].Observed==classes[k].ClassID)  //correct
			      numCorrect ++;
			}
		}
		fprintf(fp, "\n  Correct \(%d points\)",numCorrect);
		if(numCorrect == 0)
			fprintf(fp, "\n    %(none%)");
		else
		{

			fprintf(fp, "\n    PointID               X               Y               Inferred               Observed");

			for (i=0; i<numCases; i++) {
				if(cases[i].IsMaskedOut == false && cases[i].IsNoData == false)
				{

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
		}

		fprintf(fp, "\n  Mismatch");

		for (i=0; i<numCases; i++) {
			if(cases[i].IsMaskedOut == false && cases[i].IsNoData == false)
			{
				if(cases[i].Inferred!=classes[k].ClassID&&cases[i].Observed==classes[k].ClassID)   //mismatch
					numMismatch ++;
				
			}
		}
		fprintf(fp, "\n  Mismatch \(%d points\)", numMismatch);
		if(numMismatch == 0)
			fprintf(fp, "\n    %(none%)");
		else
		{

			fprintf(fp, "\n    PointID               X               Y               Inferred               Observed");
			for (i=0; i<numCases; i++) {
				if(cases[i].IsMaskedOut == false && cases[i].IsNoData == false)
				{

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
	}


	fclose(fp);



	if ((fp=fopen(m_MismatchFileName.GetBuffer(), "w"))==NULL) {
		MessageBox("Can't open mismatch file.");
		return;

	}

	fprintf(fp, "The Mismatch Point List(%s vs %s)", 
		m_HardenedMapFileName.GetBuffer(), m_PntFileName.GetBuffer());
	fprintf(fp, "\n    PointID,X,Y,Inferred,Observed");



	for (i=0; i<numCases; i++) {
	    if(cases[i].IsMaskedOut == false && cases[i].IsNoData == false)
		{

		   if (cases[i].Inferred!=cases[i].Observed) {
			fprintf(fp, "\n%s,%f,%f", 	cases[i].pointID, cases[i].x, cases[i].y);
			fprintf(fp, ",%s,%s", 	cases[i].inferredSoilName,cases[i].observedSoilName);
		  }
		}

	}

	fclose(fp);

	if(idBuf != NULL)
	{
		delete []idBuf;
		idBuf = NULL;
	}

/*	if(usersAccuracy != NULL)
	{
		delete usersAccuracy;
		usersAccuracy = NULL;
	}
	if(producersAccuracy != NULL)
	{
		delete producersAccuracy;
        producersAccuracy = NULL;
	}*/

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
	if(	LookupClasses != NULL)
	{
		delete LookupClasses;	
		LookupClasses = NULL;
	}
	


	m_ProgressBar.SetPos(100);
	CDialog::OnOK();



}



void CErrorMatrixPntLabMapDlg::OnBnClickedLookuptablebtn()
{
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "Look-up Table Files (*.lkt)|*.lkt|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.lkt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_LookupFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CErrorMatrixPntLabMapDlg::OnBnClickedErrormatrixhardenedmapbtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_HardenedMapFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CErrorMatrixPntLabMapDlg::OnBnClickedErrormatrixmatrixbtn()
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

void CErrorMatrixPntLabMapDlg::OnBnClickedErrormatrixcancel()
{
	CDialog::OnCancel();
}

void CErrorMatrixPntLabMapDlg::OnBnClickedErrormatrixpntfilebtn()
{
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "pnt File (*.pnt)|*.pnt|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.pnt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_PntFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CErrorMatrixPntLabMapDlg::OnBnClickedErrormatrixlblfilebtn()
{
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "Lable Files (*.lab)|*.lab|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.lab", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_LabFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CErrorMatrixPntLabMapDlg::CheckMask(bool* idBuf, int nrow, int ncol,CThreeDR *theData){ //Check if mask should be used
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


void CErrorMatrixPntLabMapDlg::OnMaskChk(){
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

void CErrorMatrixPntLabMapDlg::OnMaskFileBtn(){
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

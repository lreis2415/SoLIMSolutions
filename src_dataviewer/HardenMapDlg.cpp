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
// HardenMapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "HardenMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHardenMapDlg dialog


CHardenMapDlg::CHardenMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHardenMapDlg::IDD, pParent){
	//{{AFX_DATA_INIT(CHardenMapDlg)
	m_OutputFileName = _T("");
	m_GammaCorrection = TRUE;
	m_ColorModel = 0;
	m_Mask = FALSE;
	m_MaskFileName = _T("");
	m_MaskValue = 0;
	//}}AFX_DATA_INIT
	HINSTANCE hinst = AfxGetInstanceHandle();
	/*hiconNew = (HICON)LoadImage(hinst, MAKEINTRESOURCE(IDI_AddIco), IMAGE_ICON, 14, 14, LR_SHARED);
	hiconLoad = (HICON)LoadImage(hinst, MAKEINTRESOURCE(IDI_OpenIco), IMAGE_ICON, 14, 14, LR_SHARED);
	hiconSave = (HICON)LoadImage(hinst, MAKEINTRESOURCE(IDI_SaveIco), IMAGE_ICON, 14, 14, LR_SHARED);
	hiconRemove = (HICON)LoadImage(hinst, MAKEINTRESOURCE(IDI_SubtractIco), IMAGE_ICON, 14, 14, LR_SHARED);*/
}

void CHardenMapDlg::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHardenMapDlg)
	DDX_Text(pDX, IDC_OutputPrefixFld, m_OutputFileName);
	DDX_Check(pDX, IDC_GammaCorrectionChk, m_GammaCorrection);
	DDX_Radio(pDX, IDC_HSVRdo, m_ColorModel);
	DDX_Check(pDX, IDC_MaskChk, m_Mask);
	DDX_Text(pDX, IDC_MaskFileFld, m_MaskFileName);
	DDX_Text(pDX, IDC_MaskValueFld, m_MaskValue);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LayerList, m_theList);
}

BEGIN_MESSAGE_MAP(CHardenMapDlg, CDialog)
	ON_BN_CLICKED(IDC_OutputBtn, OnBnClickedOutputbtn)
	ON_BN_CLICKED(IDC_DATALOAD, OnBnClickedDataload)
	ON_BN_CLICKED(IDC_DATASAVE, OnBnClickedDatasave)
	ON_BN_CLICKED(IDC_LAYERADD, OnBnClickedLayeradd)
	ON_BN_CLICKED(IDC_LAYERDEL, OnBnClickedLayerdel)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LayerList, OnLvnEndlabeleditLayerlist)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LayerList, OnNMKillfocusLayerlist)
	ON_BN_CLICKED(IDC_MaskFileBtn, OnMaskFileBtn)
	ON_BN_CLICKED(IDC_MaskChk, OnMaskChk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHardenMapDlg message handlers

BOOL CHardenMapDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	/*
	CButton* b = (CButton*)GetDlgItem(IDC_LAYERADD);
	b->SetIcon(hiconNew);
	b = (CButton*)GetDlgItem(IDC_DATALOAD);
	b->SetIcon(hiconLoad);
	b = (CButton*)GetDlgItem(IDC_DATASAVE);
	b->SetIcon(hiconSave);
	b = (CButton*)GetDlgItem(IDC_LAYERDEL);
	b->SetIcon(hiconRemove);*/

	CDC* pDC = GetDC();
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	m_theList.InsertColumn(0, "Label", LVCFMT_LEFT, 8*tm.tmAveCharWidth, 0);
	m_theList.InsertColumn(1, "File", LVCFMT_LEFT, 60*tm.tmAveCharWidth, 1);
	m_theList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHardenMapDlg::OnBnClickedDataload(){
	// TODO: Add your control notification handler code here
 	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "Name List Files (*.lst)|*.lst|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.lst", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK && m_ldFile.GetPathName()!=""){
		m_listFileName = m_ldFile.GetPathName();
	}
	else
		return;

	ifstream fin;
	fin.open(m_listFileName);
	if(fin.fail()){
		CString msg = "Cannot open file ";
		msg += m_listFileName;
		msg += ".";
		MessageBox(msg);
		return;
	}

	m_theList.DeleteAllItems();

	while(!fin.eof()){
		
		///Modified by DuFei
		CString aline;
		fin.getline(aline.GetBuffer(1024),1024);
		aline.ReleaseBuffer(-1);
		int pos;
		pos=aline.Find(":",0);
		pos=pos-1;		
		CString label;
		CString fileName;
		label=aline.Left(pos);
		label.Trim();
		fileName=aline.Right(aline.GetLength()-pos);
		fileName.Trim();
        /////////////////////////////


		//fin>>label.GetBuffer(100)>>fileName.GetBuffer(100);
		label.ReleaseBuffer(-1);
		fileName.ReleaseBuffer(-1);

		if(!label.IsEmpty()&& !fileName.IsEmpty()){
			int count = m_theList.GetItemCount();
			m_theList.InsertItem(LVIF_TEXT|LVIF_STATE, count, label, count==0 ? LVIS_SELECTED : 0, LVIS_SELECTED, 0, 0);
			m_theList.SetItemText(count, 1, fileName);
		}
	}
	fin.close();
	UpdateData(FALSE);
}

void CHardenMapDlg::OnBnClickedDatasave(){
	// TODO: Add your control notification handler code here
 	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "Name List File (*.lst)|*.lst|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(FALSE, "lst", ".lst", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK && m_ldFile.GetPathName()!="")
		m_listFileName = m_ldFile.GetPathName();
	else 
		return;

	ofstream fout;
	fout.open(m_listFileName);
	int count = m_theList.GetItemCount();
	for(int i=0; i<count; i++){
		CString label = m_theList.GetItemText(i, 0);
		CString filename = m_theList.GetItemText(i, 1);
		fout<<label.GetBuffer(1)<<"	"<<filename.GetBuffer(1)<<endl;
	}
	fout.close();		
}

void CHardenMapDlg::OnBnClickedLayeradd(){
	// TODO: Add your control notification handler code here
	CString layerFileName;
 	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_ALLOWMULTISELECT |OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Locate Soil Fuzzy Membership Files";
	CString   strFileNames;  
	m_ldFile.m_ofn.lpstrFile=strFileNames.GetBuffer(4000); 
	m_ldFile.m_ofn.nMaxFile= 4000 ;  
	if(m_ldFile.DoModal() == IDOK){
		POSITION mPos= m_ldFile.GetStartPosition(); 
		while(mPos!=NULL) {         
            layerFileName = m_ldFile.GetNextPathName(mPos);         
		    if(layerFileName == ""){
			   MessageBox("No input file.");
			   return;
		    }
		    int count = m_theList.GetItemCount();
		    int max = 0;
		    CString label = "";

			CString filename;
			filename = extractFileName(layerFileName);


	        int pos = filename.Find('.');
	        if(pos == -1){
				  MessageBox("Error in adding one fuzzy membership file.That soil will be ignored.");	
				  continue;
	        }
	        filename = filename.Left(pos);

			int length = filename.GetLength();
			char c;
			bool IsIDBegin = true;
			for(int i = 0;i<length ;i++)
			{
				c = filename.GetAt(i);
				if(i == 0&&( c<'0'||c>'9'))
				{
					IsIDBegin = false;
					break;
					
				}

				if(c>='0' && c<='9')
					label+=c;
				else
					break;
			}
			if(IsIDBegin == false)
			{
				for(int i = length -1;i>=0 ;i--)
			    {
				   c = filename.GetAt(i);				  

				   if(i == 0&&(c<'0' || c>'9'))
				   { 
					   label = "-1";
					   break;
				   }
				   if(c>='0' && c<='9')
					   	label+=c;
				   else
					 break;			 

			     }
				label.MakeReverse();
			}

		    m_theList.InsertItem(LVIF_TEXT|LVIF_STATE, count, label, LVIS_SELECTED, LVIS_SELECTED, 0, 0);
		    m_theList.SetItemText(count, 1, layerFileName);
		    UpdateData(FALSE);	
	   }
   }

}

void CHardenMapDlg::OnBnClickedLayerdel(){
	// TODO: Add your control notification handler code here
 	UpdateData(TRUE);
	POSITION pos = m_theList.GetFirstSelectedItemPosition();
	while (pos){
      int nItem = m_theList.GetNextSelectedItem(pos);
	  m_theList.DeleteItem(nItem);
	}
}

void CHardenMapDlg::OnBnClickedOutputbtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(FALSE, "3dr", "HardenUnMap.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save Harderned Map As";	
	if(m_ldFile.DoModal() == IDOK){

		  m_OutputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CHardenMapDlg::OnLvnEndlabeleditLayerlist(NMHDR *pNMHDR, LRESULT *pResult){
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	CString str = pDispInfo->item.pszText;
	if(str == "") return;
	POSITION pos = m_theList.GetFirstSelectedItemPosition();
	if (pos != NULL){
		int nItem = m_theList.GetNextSelectedItem(pos);
		m_theList.SetItemText(nItem, 0, str);
	}
	*pResult = 0;
}

void CHardenMapDlg::OnNMKillfocusLayerlist(NMHDR *pNMHDR, LRESULT *pResult){
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CHardenMapDlg::OnOK(){
	// TODO: Add extra ErrorMatrix here
	UpdateData(TRUE);

	int theChunk = 0;
	int numOfFiles = 0; // Number of membership files
	CString msg;	

	//1. read in filenames from the label file, and create the file list.
	List<ICThreeDRNode>* fileList = new List<ICThreeDRNode>;
	int count = m_theList.GetItemCount();
	int readflag = 0;
	for(int i=0; i<count; i++){
		CString label = m_theList.GetItemText(i, 0);
		int tempID = atoi(label.GetString());
		CString tempFileName = m_theList.GetItemText(i, 1);
		CThreeDR* aThreeDR = new CThreeDR;
		readflag = aThreeDR->readin(tempFileName.GetBuffer(1), CHUNK);
		if(readflag != 0 && readflag != 1){
			CString fileName = extractFileName(tempFileName);
			msg = "Error in reading ";
			msg += fileName;
			MessageBox(msg);
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			return;
		}

		if(fileList->first != NULL)
			if(!fileList->first->theThreeDR->spatialRangeMatch(aThreeDR)){
				msg = "The spatial ranges of ";
				msg += fileList->first->c;
				msg += " and ";
				msg += tempFileName;
				msg += " do not match each other.";
				MessageBox(msg);
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				return;
			}

		fileList->addTail(new ICThreeDRNode(tempID, tempFileName, aThreeDR));
		numOfFiles++;
	}
	int nrow = fileList->first->theThreeDR->getNumberOfRows();
	int ncol = fileList->first->theThreeDR->getNumberOfColumns();

	bool* idBuf = new bool[nrow*ncol];
	for(i=0; i<nrow*ncol; i++)
	{
		idBuf[i]= true;
	}
	if(m_Mask == true)
	{	
		CheckMask(idBuf, nrow, ncol,fileList->first->theThreeDR);
	}
    


	float nodata = fileList->first->theThreeDR->getNullDataValue();

	if(CHUNK>=nrow)
		theChunk = nrow;
	else
		theChunk = CHUNK;

	//2 setting up the palette
	UncertainColors* uncertaincolors = new UncertainColors;
	int numBaseValues = numOfFiles;
	int* baseID = new int[numOfFiles];
	ICThreeDRNode* tempNode = fileList->first;
	i=0;
	while(tempNode != NULL){
		baseID[i] = tempNode->i;
		tempNode = tempNode->next;
		i++;
	}

	if(!m_GammaCorrection)
		uncertaincolors->SetGamma(false);

	int ncolors = 0;
	if(m_ColorModel==0){ //if HSV
		if((ncolors=uncertaincolors->BuildHSVPalette(baseID,numBaseValues))<0){
			MessageBox("Error in allocating the color palette");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			delete [] baseID;
			return;
		}
	}
	else{ //if Ljg
		if((ncolors=uncertaincolors->BuildLjgPalette(baseID,numBaseValues))<0){
			MessageBox("Error in allocating the color palette");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			delete [] baseID;
			return;
		}
	}
	delete [] baseID;

	//3. setup the output files
	CString hardenColorExg = m_OutputFileName;
	hardenColorExg.Replace(".3dr","Exg.3dr");
	CString hardenColorEnt = m_OutputFileName;
	hardenColorEnt.Replace(".3dr","Ent.3dr");
	
	CThreeDR* theHarden3dr = new CThreeDR;
	float* hardenBuf = new float[theChunk*ncol]; 
	theHarden3dr->setDataBuf(hardenBuf);
	theHarden3dr->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));
	theHarden3dr->copyBasicFields(fileList->first->theThreeDR);
	theHarden3dr->setNumberOfColors(ncolors);

	CThreeDR* theHardenExg3dr = new CThreeDR;
	float* hardenExgBuf = new float[theChunk*ncol]; 
	theHardenExg3dr->setDataBuf(hardenExgBuf);
	theHardenExg3dr->setDataName(extractFileName(hardenColorExg).GetBuffer(1));
	theHardenExg3dr->copyBasicFields(fileList->first->theThreeDR);
	theHardenExg3dr->setNumberOfColors(ncolors);

	CThreeDR* theHardenEnt3dr = new CThreeDR;
	float* hardenEntBuf = new float[theChunk*ncol]; 
	theHardenEnt3dr->setDataBuf(hardenEntBuf);
	theHardenEnt3dr->setDataName(extractFileName(hardenColorEnt).GetBuffer(1));
	theHardenEnt3dr->copyBasicFields(fileList->first->theThreeDR);
	theHardenEnt3dr->setNumberOfColors(ncolors);

	CString tempHardenUn = m_OutputFileName;
	tempHardenUn.Replace(".3dr", "tempHardenUn");
	CString tempHardenExg = m_OutputFileName;
	tempHardenExg.Replace(".3dr", "temphardenExg");
	CString tempHardenEnt = m_OutputFileName;
	tempHardenEnt.Replace(".3dr", "temphardenEnt");
	int PixelNum= ncol*nrow;

	//4. hardening and computing the uncertainty values
	for(int bigI = 0; ;bigI++){
		for(i = 0; i<theChunk*ncol; i++){
			if(m_Mask==true&&i+bigI*theChunk*ncol<PixelNum&&idBuf[i+bigI*theChunk*ncol]==false)
			{
				hardenBuf[i] = nodata;
				hardenExgBuf[i] = nodata;
				hardenEntBuf[i] = nodata;
			}
			else
			{
				float sumMem = 0.0;
				float scaled_mem = 0.0;
				float PlogP = 0.0;
				float entropy = 0.0;
				float exaggeration = 0.0;
				int maxID;	// the SoilID of the membership file which has 
				//the highest membership value at the current location
				hardenBuf[i] = 0.0;
				hardenExgBuf[i] = 0.0;
				hardenEntBuf[i] = 0.0;

				tempNode = fileList->first;
				while(tempNode != NULL){	
					float v = tempNode->theThreeDR->getDataBuf()[i];
					if(hardenBuf[i] < v){
						hardenBuf[i] = v;
						maxID = tempNode->i; // the soilID
					}
					if(v < VERYSMALL)
						sumMem += VERYSMALL;
					else
						sumMem += v;
					tempNode = tempNode->next;
				}

				// computing entropy value for the current 
				if(sumMem>(numOfFiles*VERYSMALL)){
					tempNode = fileList->first;
					while(tempNode != NULL){
					
						scaled_mem = tempNode->theThreeDR->getDataBuf()[i]/sumMem;
						if(scaled_mem<VERYSMALL) scaled_mem = VERYSMALL;
						PlogP += scaled_mem*(log(scaled_mem));
						tempNode = tempNode->next;
					}
					entropy = (-1.0*PlogP)/log((float)numOfFiles);
				}
				else{
					entropy = MAXUNCERTAINTY;
				}
				exaggeration = 1.0 - hardenBuf[i]/100.0;

				if (hardenBuf[i] < VERYSMALL)
					hardenBuf[i] = uncertaincolors->Pixel3d(0, 0.0);
				else
					hardenBuf[i] = uncertaincolors->Pixel3d(maxID, 0.0);

				hardenExgBuf[i] = uncertaincolors->Pixel3d((int)hardenBuf[i], exaggeration);
				hardenEntBuf[i] = uncertaincolors->Pixel3d((int)hardenBuf[i], entropy);
			}
		}

		theHarden3dr->writeout(tempHardenUn.GetBuffer(1), theChunk);
		theHardenExg3dr->writeout(tempHardenExg.GetBuffer(1), theChunk);
		theHardenEnt3dr->writeout(tempHardenEnt.GetBuffer(1), theChunk);
		
		if(readflag == 0) break; //reading function reaches the end of the file

		tempNode = fileList->first;
		while(tempNode != NULL){
			readflag = tempNode->theThreeDR->readin(tempNode->c.GetBuffer(1), theChunk);
			if(readflag != 0 && readflag != 1){
				msg = "Error in reading ";
				msg += tempNode->c;
				msg += ".";
				MessageBox(msg);
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				return;
			}
			tempNode = tempNode->next;
		}
	}

	//5. wrap up the output file (calc the statistics for them)
	theHarden3dr->flush();
	theHardenExg3dr->flush();
	theHardenEnt3dr->flush();

	theHarden3dr->readin(tempHardenUn.GetBuffer(1), 0);
	theHarden3dr->setNullDataValue(nodata);
	theHarden3dr->calcStat();
	theHarden3dr->writeout(m_OutputFileName.GetBuffer(1), 0);

	theHardenExg3dr->readin(tempHardenExg.GetBuffer(1), 0);
	theHardenExg3dr->setNullDataValue(nodata);
	theHardenExg3dr->calcStat();
	theHardenExg3dr->writeout(hardenColorExg.GetBuffer(1), 0);

	theHardenEnt3dr->readin(tempHardenEnt.GetBuffer(1), 0);
	theHardenEnt3dr->setNullDataValue(nodata);
	theHardenEnt3dr->calcStat();
	theHardenEnt3dr->writeout(hardenColorEnt.GetBuffer(1), 0);

	//delete the temp files
	CFile  tmpFile; 
	tmpFile.Remove(tempHardenUn);
	tmpFile.Remove(tempHardenExg);
	tmpFile.Remove(tempHardenEnt);


	//6. write the palettes
	uncertaincolors->WritePalette(m_OutputFileName.GetBuffer(1));
	uncertaincolors->WritePalette(hardenColorEnt.GetBuffer(1));
    uncertaincolors->WritePalette(hardenColorExg.GetBuffer(1));

	//7. clean up
	delete theHarden3dr;
	delete theHardenExg3dr;
	delete theHardenEnt3dr;

	tempNode = fileList->first;
	while(tempNode != NULL){
		delete tempNode->theThreeDR;
		tempNode = tempNode->next;
	}
	delete fileList;
	delete uncertaincolors;

	delete []idBuf;

	GetDlgItem(IDOK)->EnableWindow(TRUE);
	CDialog::OnOK();
}

void CHardenMapDlg::CheckMask(bool* idBuf, int nrow, int ncol,CThreeDR *theData){ //Check if mask should be used
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


void CHardenMapDlg::OnMaskChk(){
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

void CHardenMapDlg::OnMaskFileBtn(){
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


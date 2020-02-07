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
#include ".\wordrule.h"

#include   <msxml2.h>      
#import   <msxml4.dll>

CWordRule::CWordRule(void)
{
	m_strLibFileName="";
	m_strPhrase="";	
	m_pWordLib=NULL;	
}

CWordRule::CWordRule(CWordRule &aWordRule)
{
	m_strRuleName = aWordRule.getRuleName();
	m_bIsUsed = aWordRule.getIsUsed();
	m_dDispMin = aWordRule.getDispMin();
	m_dDispMax = aWordRule.getDispMax();
	m_iRuleType = aWordRule.getRuleType();
	m_strLayerName = aWordRule.getLayerName();
	m_strLibFileName = aWordRule.getLibFileName();
	m_strPhrase = aWordRule.getPhrase();
	m_strEnvAttri = aWordRule.getEnvAttri();
	if(aWordRule.getWordLib() == NULL)
		m_pWordLib = NULL;
	else
		m_pWordLib = new CWordLib(*aWordRule.getWordLib());	
	m_dCurGraphMin = aWordRule.getCurGraphMin();
	m_dCurGraphMax = aWordRule.getCurGraphMax();

}

CWordRule::~CWordRule(void)
{
	if(m_pWordLib!=NULL)
	{
		delete m_pWordLib;
		m_pWordLib=NULL;
	}
}

CString CWordRule::getPhrase()
{
	return m_strPhrase;
}

void CWordRule::setPhrase(CString strPhrase)
{
	m_strPhrase = strPhrase;
}

CString CWordRule::getLibFileName()
{
	return m_strLibFileName;
}

void CWordRule::setLibFileName(CString strLibFileName)
{
	m_strLibFileName = strLibFileName;
}

CWordLib* CWordRule::getWordLib()
{
	return m_pWordLib;
}

double CWordRule::Evaluate(float fEnvValue)
{
	if(fabs(fEnvValue-NOFLOATVALUE)<VERYSMALL)
	{
		return 1;
	}

	//use spline curve, it is quite like freehand rule
	CWordCurve *pWordCurve;
	pWordCurve = m_pWordLib->getWordCurve(m_strPhrase,m_strEnvAttri);
	double result;
	int i,j;
	double h0,h1,alpha,beta;
	vector <double> vecDY(pWordCurve->getKnotNum());
	vector <double> vecDDY(pWordCurve->getKnotNum());
	vector <double> vecS(pWordCurve->getKnotNum());	

	vecDDY[0] = 0;
	vecDDY[pWordCurve->getKnotNum()-1] = 0;
	vecDY[0] = -0.5;
	h0 = pWordCurve->getKnotX(1) - pWordCurve->getKnotX(0);
	vecS[0] = 3.0 * (pWordCurve->getKnotY(1) -pWordCurve->getKnotY(0)) / (2.0 * h0) - vecDDY[0] * h0 / 4.0;
	for(j=1;j<=pWordCurve->getKnotNum()-2; j++)
	{
		h1 = pWordCurve->getKnotX(j+1)-pWordCurve->getKnotX(j);
		alpha = h0/(h0+h1);
		beta = (1.0-alpha)*(pWordCurve->getKnotY(j)-pWordCurve->getKnotY(j-1))/h0;
		beta = 3.0*(beta+alpha*(pWordCurve->getKnotY(j+1)-pWordCurve->getKnotY(j))/h1);
		vecDY[j] = -alpha/(2.0+(1.0-alpha)*vecDY[j-1]);
		vecS[j] = beta-(1.0-alpha)*vecS[j-1];
		vecS[j] = vecS[j]/(2.0+(1.0-alpha)*vecDY[j-1]);
		h0=h1;
	}
	vecDY[pWordCurve->getKnotNum()-1] = (3.0*(pWordCurve->getKnotY(pWordCurve->getKnotNum()-1)-pWordCurve->getKnotY(pWordCurve->getKnotNum()-2))/h1+vecDDY[pWordCurve->getKnotNum()-1]*h1/2-vecS[pWordCurve->getKnotNum()-2])/(2.0+vecDY[pWordCurve->getKnotNum()-2]);
	for(j=pWordCurve->getKnotNum()-2; j>=0; j--)
		vecDY[j] = vecDY[j]*vecDY[j+1]+vecS[j];

	for(j=0; j<=pWordCurve->getKnotNum()-2; j++)
		vecS[j] = pWordCurve->getKnotX(j+1)-pWordCurve->getKnotX(j);

	for(j=0;j<=pWordCurve->getKnotNum()-2;j++)
	{
		h1 = vecS[j]*vecS[j];
		vecDDY[j] = 6.0*(pWordCurve->getKnotY(j+1)-pWordCurve->getKnotY(j))/h1-2.0*(2.0*vecDY[j]+vecDY[j+1])/vecS[j];
	}
	h1 = vecS[pWordCurve->getKnotNum()-2]*vecS[pWordCurve->getKnotNum()-2];
	vecDDY[pWordCurve->getKnotNum()-1] = 6*(pWordCurve->getKnotY(pWordCurve->getKnotNum()-2)-pWordCurve->getKnotY(pWordCurve->getKnotNum()-1))/h1+2*(2*vecDY[pWordCurve->getKnotNum()-1]+vecDY[pWordCurve->getKnotNum()-2])/vecS[pWordCurve->getKnotNum()-2];

	//compute the final result	
	if(fEnvValue>pWordCurve->getKnotX(pWordCurve->getKnotNum()-1)) //if the input number is greater than any X 
		result = vecDY[pWordCurve->getKnotNum()-1]*(fEnvValue-pWordCurve->getKnotX(pWordCurve->getKnotNum()-1))+pWordCurve->getKnotY(pWordCurve->getKnotNum()-1);	
	else if(fEnvValue<pWordCurve->getKnotX(0))  //if the input number is smaller than any X 
		result = vecDY[0]*(fEnvValue-pWordCurve->getKnotX(0))+pWordCurve->getKnotY(0);
	else
	{
		i = 0;
		while(fEnvValue>pWordCurve->getKnotX(i+1))
			i = i+1;	   
		h1 = (pWordCurve->getKnotX(i+1)-fEnvValue)/vecS[i];
		h0 = h1*h1;
		result = (3.0*h0-2.0*h0*h1)*pWordCurve->getKnotY(i);
		result = result+vecS[i]*(h0-h0*h1)*vecDY[i];
		h1 = (fEnvValue-pWordCurve->getKnotX(i))/vecS[i];
		h0 = h1*h1;
		result = result+(3.0*h0-2.0*h0*h1)*pWordCurve->getKnotY(i+1);
		result = result-vecS[i]*(h0-h0*h1)*vecDY[i+1];
	} 	  
	if(result>1)
		result = 1;
	else if(result<0) 
		result = 0;
	return result;  
}

void  CWordRule::DrawGraph(int cx, int cy, CPaintDC *dc)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,cx,0,cy);	
	glViewport(0,0,cx,cy);
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f,1.0f,0.0f);
	glEnable(GL_LINE_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

	//draw axis
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glVertex2f(20.0f, 20.0f);
	glVertex2f(cx-10,20.0f);
	glVertex2f(20.0f, 20.0f);
	glVertex2f(20.0f,cy-10);
	glVertex2f(cx-10,20.0f);
	glVertex2f(cx-15,25);
	glVertex2f(cx-10,20.0f);
	glVertex2f(cx-15,15);
	glVertex2f(20.0f,cy-10);
	glVertex2f(15,cy-15);
	glVertex2f(20.0f,cy-10);
	glVertex2f(25,cy-15);

	CWordCurve *pWordCurve = NULL;
	if(IsLegal())
	{     
		pWordCurve = m_pWordLib->getWordCurve(m_strPhrase,m_strEnvAttri);
		if(pWordCurve->getKnotNum()>2)
		{
			double step = (getDispMax()-getDispMin())/100;
			double result[101];	
			for(int i=0;i<101;i++)
			{
				result[i]=Evaluate(m_dDispMin+step*i);
			}

			for(int i=0;i<100;i++)
			{
				glVertex2f((cx-40)*(step*i)/(m_dDispMax-m_dDispMin)+20, (cy-40)*result[i]+20);		
				glVertex2f((cx-40)*(step*i+step)/(m_dDispMax-m_dDispMin)+20, (cy-40)*result[i+1]+20);		
			}
		}
	}

	double step1=(m_dDispMax-m_dDispMin)/10;
	for(int i=0;i<=10;i++)
	{
		glVertex2f((cx-40)*(step1*i)/(m_dDispMax-m_dDispMin)+20,20);
		glVertex2f((cx-40)*(step1*i)/(m_dDispMax-m_dDispMin)+20,23);
	}
	for(int i=1;i<=10;i++)
	{ 	
		glVertex2f(20,20+i*0.1*(cy-40));
		glVertex2f(23,20+i*0.1*(cy-40));		

	}
	glEnd();	
	if(pWordCurve != NULL)
	{
		for(int i=0;i<pWordCurve->getKnotNum();i++)
		{
			glBegin(GL_QUADS);		
			glVertex2f((pWordCurve->getKnotX(i)-m_dDispMin)*(cx-40)/(m_dDispMax-m_dDispMin)+20-3, pWordCurve->getKnotY(i)*(cy-40)+20-3);		
			glVertex2f((pWordCurve->getKnotX(i)-m_dDispMin)*(cx-40)/(m_dDispMax-m_dDispMin)+20-3, pWordCurve->getKnotY(i)*(cy-40)+20+3);	
			glVertex2f((pWordCurve->getKnotX(i)-m_dDispMin)*(cx-40)/(m_dDispMax-m_dDispMin)+20+3, pWordCurve->getKnotY(i)*(cy-40)+20+3);	
			glVertex2f((pWordCurve->getKnotX(i)-m_dDispMin)*(cx-40)/(m_dDispMax-m_dDispMin)+20+3, pWordCurve->getKnotY(i)*(cy-40)+20-3);	
			glEnd();	
		}
	}
	glFlush();

	SwapBuffers(wglGetCurrentDC());
	HDC  hdc=wglGetCurrentDC();
	//::SetBkMode(hdc, TRANSPARENT );
	::SetTextColor( hdc, RGB(255,255,255) );
	::SetBkColor(hdc, RGB(0, 0, 0));
	HFONT hFont;
	hFont=::CreateFont(10,5,0,0,100,0,0,0,DEFAULT_CHARSET,0,0,2,DEFAULT_PITCH,"1");
	::SelectObject(hdc,hFont);

	CPen NewPen(PS_SOLID,4,RGB(255,255,0));
	CPen *pOldPen=dc->SelectObject(&NewPen);
	CFont font;		
	font.CreateFont(7,5,0,0,100,0,0,0,DEFAULT_CHARSET,0,0,2,DEFAULT_PITCH,"1");	
	CFont *pOldFont=dc->SelectObject(&font);

	CString str;	
	for(int i=1;i<=10;i++)
	{   
		str.Format("%.1f",i*0.1);	
		::TextOut(hdc,4,cy-20-i*0.1*(cy-40)-3,str,str.GetLength());
	}
	for(int i=0;i<=10;i++)
	{
		str.Format("%f",m_dDispMin+i*step1);
		str = str.Left(6);
		::TextOut(hdc,(cx-40)*(step1*i)/(m_dDispMax-m_dDispMin)+12,cy-19,str,str.GetLength());
	}

	str="Optimality";
	::TextOut(hdc,3,0,str,str.GetLength());
	str="Env. Condition";
	::TextOut(hdc,cx/2-5,cy-10,str,str.GetLength());

	::SelectObject(hdc,pOldFont);
	::SelectObject(hdc,pOldPen);	

}

double CWordRule::ComputeRuleGraphMin()
{	
	if(m_strPhrase == ""||m_pWordLib==NULL||m_strLibFileName == "")
	{
		return -1000;
	}
	CWordCurve *pWordCurve;
	pWordCurve = m_pWordLib->getWordCurve(m_strPhrase, m_strEnvAttri);
	if(pWordCurve->getKnotNum()>2)
	{
		return pWordCurve->getKnotX(0)-0.1*(pWordCurve->getKnotX(pWordCurve->getKnotNum()-1) - pWordCurve->getKnotX(0));
	}
	else 
		return -1000;	
}

double CWordRule::ComputeRuleGraphMax()
{
	if(m_strPhrase == ""||m_pWordLib==NULL||m_strLibFileName == "")
	{
		return 1000;
	}
	CWordCurve *pWordCurve;
	pWordCurve = m_pWordLib->getWordCurve(m_strPhrase,m_strEnvAttri);
	if(pWordCurve->getKnotNum()>2)
	{
		return pWordCurve->getKnotX(pWordCurve->getKnotNum()-1)+0.1*(pWordCurve->getKnotX(pWordCurve->getKnotNum()-1) - pWordCurve->getKnotX(0));
	}
	else 
		return 1000;	
}

int CWordRule::ComputeScreenCoordX(double x, int cx)
{
	return (cx-40)*(x-m_dDispMin)/(m_dDispMax-m_dDispMin)+20;
}
int CWordRule::ComputeScreenCoordY(double y, int cy)
{
	return (1-y)*cy+40*y-20;
}
double CWordRule::ComputeGraphCoordX(int x, int cx)
{
	return (x-20)*(m_dDispMax-m_dDispMin)/(cx-40)+m_dDispMin;
}
double CWordRule::ComputeGraphCoordY(int y, int cy)
{
	double t=(double)(cy-y-20)/(cy-40);
	return t;
}

bool CWordRule::IsLegal(void)
{
	if(m_strPhrase == ""||m_pWordLib==NULL||m_strLibFileName == "")
	{
		return false;
	}	
	return true;
}

void CWordRule::LoadWordLib()
{
	HRESULT hr;	 
	MSXML2::IXMLDOMDocumentPtr pDoc;//xml document
	MSXML2::IXMLDOMElementPtr pRootElem;
	_variant_t VariantValue;
	CString str;
	hr = CoInitialize(NULL);
	if(FAILED(hr))
	{
	   AfxMessageBox("Error in reading word library file");
		return;
	}
	hr=pDoc.CreateInstance(_uuidof(MSXML2::DOMDocument));
	if(FAILED(hr))
	{
		AfxMessageBox("Error in reading word library file");
		return;
	}
	//load xml doc
	_variant_t varxml(m_strLibFileName);
	VARIANT_BOOL varOut;
	varOut = pDoc->load(varxml);
	if(!varOut)
	{
		AfxMessageBox("Error in reading word library file");
		return;
	}
	//parse root node 
	pDoc->get_documentElement(&pRootElem);
	if(pRootElem->hasChildNodes() != -1)//-1 is True
	{
		return;
	}
	if(m_pWordLib!=NULL)
	{
		delete m_pWordLib;
		m_pWordLib =NULL;
	}
	m_pWordLib = new CWordLib();
	MSXML2::IXMLDOMNodeListPtr pEnvAttriList;
	MSXML2::IXMLDOMNodePtr pEnvAttriChildNodes;
	MSXML2::IXMLDOMNodePtr pEnvAttriNameNode;
	MSXML2::IXMLDOMNamedNodeMapPtr pEnvAttriAttr;
	pRootElem->get_childNodes(&pEnvAttriList);
	pRootElem.Release();
	long iEnvAttriNum;		
	pEnvAttriList->get_length(&iEnvAttriNum);		
	for (int i=0;i<iEnvAttriNum;i++)
	{
		pEnvAttriList->get_item(i,&pEnvAttriChildNodes);
		pEnvAttriChildNodes->get_attributes(&pEnvAttriAttr);   
		pEnvAttriNameNode = pEnvAttriAttr->getNamedItem("Name");
		pEnvAttriNameNode->get_nodeTypedValue(&VariantValue);		
		CString strEnvAttriName =(char *)(_bstr_t)VariantValue;	
		m_pWordLib->AddEnvAttri(strEnvAttriName);
		MSXML2::IXMLDOMNodeListPtr pCurveList;
		MSXML2::IXMLDOMNodeListPtr pCurveParaList;
		MSXML2::IXMLDOMNodePtr pCurveNameNode;
		MSXML2::IXMLDOMNodePtr pCurveNodeNum;
		MSXML2::IXMLDOMNodePtr pCurveCoordinatesNode;
		MSXML2::IXMLDOMNodePtr pCurveChildNodes;
		pEnvAttriChildNodes->get_childNodes(&pCurveList);
		long iCurveNum;		
		pCurveList->get_length(&iCurveNum);	
		for (int i=0;i<iCurveNum;i++)
		{
			CWordCurve *pWordCurve = new CWordCurve;
			pCurveList->get_item(i,&pCurveChildNodes);
			pCurveChildNodes->get_childNodes(&pCurveParaList);
			pCurveParaList->get_item(0,&pCurveNameNode);
			pCurveParaList->get_item(1,&pCurveNodeNum);
			pCurveParaList->get_item(2,&pCurveCoordinatesNode);
			VARIANT varVal;
			pCurveNameNode->get_nodeTypedValue(&varVal);
			CString strCurveName = (char*)(_bstr_t)varVal;
			pCurveNodeNum->get_nodeTypedValue(&VariantValue);	
			CString strNodeNum =(char *)(_bstr_t)VariantValue;	
			int NodeNum = atoi(strNodeNum);
			pCurveCoordinatesNode->get_nodeTypedValue(&varVal);
			CString strCurveCoordinates = (char*)(_bstr_t)varVal;
			pWordCurve->setPhrase(strCurveName);
			pWordCurve->setEnvAttri(strEnvAttriName);
			double NodeX[100];
			double NodeY[100];
			split1(strCurveCoordinates,NodeX, NodeY);
			if(NodeNum>100)
			{
				AfxMessageBox("The number of nodes must be smaller than 100");
				delete pWordCurve;
				pWordCurve = NULL;
				continue;
			}
			for(int i=0;i<NodeNum; i++)				
				pWordCurve->AddKnot(NodeX[i],NodeY[i]);		
			m_pWordLib->AddWordCurve(pWordCurve);
		}
		pCurveList.Release();
		pCurveParaList.Release();
		pCurveNameNode.Release();
		pCurveNodeNum.Release();
		pCurveCoordinatesNode.Release();
		pCurveChildNodes.Release();
	}
	pEnvAttriList.Release();
	pEnvAttriChildNodes.Release();
	pEnvAttriNameNode.Release();
	pEnvAttriAttr.Release();
	pDoc.Release();	
	CoUninitialize();
}

CString CWordRule::getEnvAttri()
{
	return m_strEnvAttri;
}
void CWordRule::setEnvAttri(CString strEnvAttri)
{
	m_strEnvAttri=strEnvAttri;
}

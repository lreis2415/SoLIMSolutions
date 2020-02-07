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
#ifndef _SHPCASEBASE_H
#define _SHPCASEBASE_H

/******************************************************************************
 * SHPCaseBase
 *
 * Purpose:  Header file for class SHPCaseBase
 * Author:   Erika Dade
 * Date:	 17 April 2005
 *
 ******************************************************************************
 */
#include "stdafx.h"
using namespace std;

#ifndef KNOWLEDGE_H
#include "knowledge.h"
#endif

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef _DBFOPEN_H
#include "dbfopen.h"
#endif

#ifndef _SHPOPEN_H
#include "shpopen.h"
#endif

#ifndef _DBFIO_H
#include "dbfio.h"
#endif

// DBF field order and names
class SHPCaseBase : public DBFIO {
private:
	// CaseBase DBF field variables
	int		cbUID;		// CaseBase UID
	CString cbName;		// CaseBase Name

	int		clID;		// CaseList ID
	int		clTypeID;	// CsaeList TypeID
	int		clUID;		// CaseList UID
	CString clName;		// CaseList Name
	bool	clMsk;		// CaseList use mask or not
	CString clMskF;		// CaseList mask file name
	bool	clInv;		// CaseList inverse mask value or not
	
	CString csCnt;		// Case central value method (for line/poly case)
	float   csTlr;		// Case tolerance in calculating mode

	int		atID;		// Attr ID
	CString atName;		// Attr Name
	CString atAlg;		// Attr algorithm name
	float	atMemberValue; //  Attr case value
	int		atPosNeg;	//Positive/Negative
	bool	atMsk;		// Attr use mask or not
	CString atMskF;		// Attr mask file name
	bool	atInv;		// Attr inverse mask value or not

	double	spX;
	double	spY;
	float	spSearchDist;
	bool	spUseDistSim;
	float	spDistDecay;
	float	spVtoH;
	int		spIntAlg;	// Spatial integrating algorithm code
	float	spWeightDecay;

	int		fnID[maxNodes];
	CString fnName[maxNodes];
	float	fnValue1[maxNodes];
	float	fnValue2[maxNodes];
	bool	fnUseOrNot[maxNodes];
	float	fnWeight[maxNodes];
	CString fnFunctionType[maxNodes];
	float	fnWidth1[maxNodes];
	float	fnR1[maxNodes];
	float	fnK1[maxNodes];
	float	fnWidth2[maxNodes];
	float	fnR2[maxNodes];
	float	fnK2[maxNodes];

	int readDBF( int n ); //return 0 if reading record n successful; return -1 if not a casebase file

	template <typename T1>
	int writeDBF( int n, const CaseBase<T1> *cb, const CaseList<T1> *cl, const T1 *c);

public:

	SHPHandle hSHP;		// Shapefile handle

	int nShapeType;		// SHPT_POINT or SHPT_ARC or SHPT_POLYGON

	SHPCaseBase( int nShapeTypeID );  // Constructor requires SHPT_ shapetype
	~SHPCaseBase();

	int openFiles( CString SHPName );	// Existing file
	int openFiles( CString SHPName, int shapeTYPE );	// New file
	void closeFiles();

	int writeSHP( const CaseBase<PointCase>  *cb );
	int writeSHP( const CaseBase<LineCase>   *cb );
	int writeSHP( const CaseBase<PolyCase>   *cb );

	int readSHP( CaseBase<PointCase>  *cb );
	int readSHP( CaseBase<LineCase>   *cb );
	int readSHP( CaseBase<PolyCase>   *cb );
	
	int verifySHPFile( int expectedShapeType );

};

/************************************************************************/
/****************************************************************************/
/* Write DBF template														*/
/*																			*/
/* Returns:																	*/
/*	 n+1 if record n written successfully, or								*/
/*   -fieldNumber (0:-numFields-1) if write error occurred on fieldNumber.	*/
/****************************************************************************/
template<typename T1>
int SHPCaseBase::writeDBF( const int n,
						   const CaseBase<T1> *cb,
						   const CaseList<T1> *cl,
						   const T1 *theCase ){
	int f;
	char use;
	Attr * attr = theCase->theAttr;
	Spatial *sp = theCase->theSpatial;
	List<FeatureNode> *fList = attr->theFeatureList;


	if (!DBFWriteIntegerAttribute( hDBF, n, f=getDBFFieldNum("UID"), cb->uid )) return -f;
	if (!DBFWriteStringAttribute(  hDBF, n, f=getDBFFieldNum("CB_NAME"), (LPCTSTR)cb->name )) return -f;

	if (!DBFWriteIntegerAttribute( hDBF, n, f=getDBFFieldNum("CL_ID"), cl->id )) return -f;
	if (!DBFWriteIntegerAttribute( hDBF, n, f=getDBFFieldNum("CL_TID"), cl->typeID )) return -f;
	if (!DBFWriteIntegerAttribute( hDBF, n, f=getDBFFieldNum("CL_UID"), cl->uid )) return -f;
	if (!DBFWriteStringAttribute(  hDBF, n, f=getDBFFieldNum("CL_NAME"), (LPCTSTR)cl->name )) return -f;
	if (!DBFWriteIntegerAttribute(  hDBF, n, f=getDBFFieldNum("CL_MSK"), (int)cl->useMask )) return -f;
	if (!DBFWriteStringAttribute(  hDBF, n, f=getDBFFieldNum("CL_MSKF"), (LPCTSTR)cl->maskFileName )) return -f;
	if (!DBFWriteIntegerAttribute(  hDBF, n, f=getDBFFieldNum("CL_INV"), (int)cl->inverseMaskValue )) return -f;

	if(nShapeType!= SHPT_POINT){ //line/poly case
		if (!DBFWriteStringAttribute(  hDBF, n, f=getDBFFieldNum("CS_CNT"), (LPCTSTR)theCase->theCentralValueMethod )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, n, f=getDBFFieldNum("CS_TLR"), (double)theCase->modeTolerance )) return -f;
	}

	if (!DBFWriteIntegerAttribute( hDBF, n, f=getDBFFieldNum("AT_ID"), attr->id )) return -f;
	if (!DBFWriteStringAttribute(  hDBF, n, f=getDBFFieldNum("AT_NAME"), (LPCTSTR)attr->name )) return -f;
	if (!DBFWriteStringAttribute(  hDBF, n, f=getDBFFieldNum("AT_ALG"), (LPCTSTR)attr->algo )) return -f;
	if (!DBFWriteDoubleAttribute(  hDBF, n, f=getDBFFieldNum("AT_MV"), (double)attr->memberValue )) return -f;
	if (!DBFWriteIntegerAttribute( hDBF, n, f=getDBFFieldNum("AT_PN"), attr->positive )) return -f;
	if (!DBFWriteIntegerAttribute(  hDBF, n, f=getDBFFieldNum("AT_MSK"), (int)attr->useMask )) return -f;
	if (!DBFWriteStringAttribute(  hDBF, n, f=getDBFFieldNum("AT_MSKF"), (LPCTSTR)attr->maskFileName )) return -f;
	if (!DBFWriteIntegerAttribute(  hDBF, n, f=getDBFFieldNum("AT_INV"), (int)attr->inverseMaskValue )) return -f;

	if (!DBFWriteDoubleAttribute(  hDBF, n, f=getDBFFieldNum("SP_X"), sp->x )) return -f;
	if (!DBFWriteDoubleAttribute(  hDBF, n, f=getDBFFieldNum("SP_Y"), sp->y )) return -f;
	if (!DBFWriteDoubleAttribute(  hDBF, n, f=getDBFFieldNum("SP_SD"), (double)sp->searchDist )) return -f;
	if (!DBFWriteIntegerAttribute( hDBF, n, f=getDBFFieldNum("SP_UDS"), (int)sp->useDistSim )) return -f;
	if (!DBFWriteDoubleAttribute(  hDBF, n, f=getDBFFieldNum("SP_DDF"), (double)sp->distDecayFactor )) return -f;
	if (!DBFWriteDoubleAttribute(  hDBF, n, f=getDBFFieldNum("SP_VHF"), (double)sp->VtoHConvertingFactor )) return -f;
	if (!DBFWriteIntegerAttribute(  hDBF, n, f=getDBFFieldNum("SP_IA"), sp->integratingAlgo )) return -f;
	if (!DBFWriteDoubleAttribute(  hDBF, n, f=getDBFFieldNum("SP_WDF"), (double)sp->wgtDecayFactor )) return -f;

	// Write out FeatureNodes
	FeatureNode *fNode = fList->first;
	int offset;
	for (int k=0; k < fList->numOfNodes; k++) {  // number of FeatureNodes per List
		offset = k*numDBFNodeFields + numDBFMainFields;
		if (!DBFWriteIntegerAttribute( hDBF, n, f=(getDBFFieldNum("FT_ID")+offset), fNode->id )) return -f;
		if (!DBFWriteStringAttribute(  hDBF, n, f=(getDBFFieldNum("FT_NAME")+offset), (LPCTSTR)fNode->name)) return -f;
		if (!DBFWriteIntegerAttribute( hDBF, n, f=(getDBFFieldNum("FT_UON")+offset), (int)fNode->useOrNot )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, n, f=(getDBFFieldNum("FT_WT")+offset), (double) fNode->weight )) return -f;
		if (!DBFWriteStringAttribute(  hDBF, n, f=(getDBFFieldNum("FT_FT")+offset), (LPCTSTR)fNode->functionType)) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, n, f=(getDBFFieldNum("FT_V1")+offset), (double)fNode->value1 )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, n, f=(getDBFFieldNum("FT_W1")+offset), (double)fNode->width1 )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, n, f=(getDBFFieldNum("FT_R1")+offset), (double)fNode->r1 )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, n, f=(getDBFFieldNum("FT_K1")+offset), (double)fNode->k1 )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, n, f=(getDBFFieldNum("FT_V2")+offset), (double)fNode->value2 )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, n, f=(getDBFFieldNum("FT_W2")+offset), (double)fNode->width2 )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, n, f=(getDBFFieldNum("FT_R2")+offset), (double)fNode->r2 )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, n, f=(getDBFFieldNum("FT_K2")+offset), (double)fNode->k2 )) return -f;

		fNode = fNode->next;
	}

	return n+1;
}

//Functions for reading Shapefiles that are not Casebases yet.  Used by the "New" button.
int readShapePoints(CString shapefileName, List<ICDDNode>* pointList, CString DBFFieldName);
int readShapeLines(CString shapefileName,  List<xPolyline>* lineList, List<ICNode>* typeNames, CString DBFFieldName); 
int readShapePolygons(CString shapefileName, List<xPolygon>* polygonList,  List<ICNode>* typeNames, CString DBFFieldName);


#endif

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
/******************************************************************************
 * DBFRasterCase
 *
 * Purpose:  Class for I/O of RasterCases data structures to DBF
 * Author:   Erika Dade
 * Date:	 12 May 2005
 * Modified by Xun Shi on June 6, 2005
 * 1. Change field widths for "R_NAME" and FL_V and other double numbers
 * 2. Add casebase name to the dbf table
 ******************************************************************************/
#include "stdafx.h"
#ifndef _DBFRASTERCASE_H
#include "dbfrastercase.h"
#endif

#ifndef _DBFIO_H
#include "dbfio.h"
#endif

/************************************************************************/
/************************************************************************/
// Constructor/Destructor
/************************************************************************/
DBFRasterCase::DBFRasterCase() :  DBFIO(){
	// Number of primary DBF fields in main block
	numDBFMainFields = 2 + numDBFAttrFields+numDBFSpatialFields;  

	// Define DBF fields unique to this class first
									// Name		  TypeEnum	Size    Number	Position
	struct DBFFieldDefinition temp0 = {"CB_NAME", FTString,	30,		0};		// 0
	struct DBFFieldDefinition temp1 = {"R_NAME",  FTString,	200,	0};		// 1

	field[0] = temp0;
	field[1] = temp1;

	// Add Attr and Spatial fields to main block from base class
	for (int i=0; i<numDBFAttrFields; i++ ) field[2+i] = attrField[i];
	for (int i=0; i<numDBFSpatialFields; i++) field[2+i+numDBFAttrFields] = spatialField[i];
}

DBFRasterCase::~DBFRasterCase(){
}


/************************************************************************/
/************************************************************************/
/* openDBF - Open DBF file for RasterCase read or write					*/
/************************************************************************/
int DBFRasterCase::openDBF( const CString DBFName, 
						    const RasterCases * rc, 
						    bool  bCreateNew ){
	// Create a new DBF file (or overwrite existing one)
	if (bCreateNew) {
		hDBF = DBFCreate( (LPCTSTR) DBFName );
		if (hDBF->fp == NULL) return -1;
	
		int numFeatureNodes = rc->theAttr->theFeatureList->numOfNodes;

		nFields = prepareDBFFields( numFeatureNodes );
		nRec = 0;
		readOnly = false;
	}

	// Open existing DBF file and get number of records and fields
	else {
		hDBF = DBFOpen( (LPCTSTR) DBFName, "rb" );
		if (hDBF->fp == NULL) return -1;

		nFields = DBFGetFieldCount( hDBF );
		nRec = DBFGetRecordCount( hDBF );
		if (nRec != 1) return -1;

		readOnly = true;

		int istat = verifyDBFFile();
		if ( istat < 0) return istat;
	}

	return 0;
}


/************************************************************************/
/* Output RasterCase structure to DBF									*/
/* Returns:																*/
/*	 1 if record 0 written successfully, or								*/
/*   0 if file handle null or readonly, or								*/
/*   -fieldNumber (0:-numFields) if write error occurred on fieldNumber.*/
/************************************************************************/
int DBFRasterCase::writeDBF( const RasterCases *rc ){
	int f;
	char use;

	if ( hDBF == NULL || readOnly ) return 0;

	Attr *attr = rc->theAttr;
	Spatial * sp = rc->theSpatial;
	List<FeatureNode> * fList = attr->theFeatureList;

	if (!DBFWriteStringAttribute(  hDBF, 0, f=getDBFFieldNum("CB_NAME"), (LPCTSTR)rc->name)) return -f;
	if (!DBFWriteStringAttribute(  hDBF, 0, f=getDBFFieldNum("R_NAME"), (LPCTSTR)rc->rasterFileName )) return -f;

	if (!DBFWriteIntegerAttribute( hDBF, 0, f=getDBFFieldNum("AT_ID"), attr->id )) return -f;
	if (!DBFWriteStringAttribute(  hDBF, 0, f=getDBFFieldNum("AT_NAME"), (LPCTSTR)attr->name )) return -f;
	if (!DBFWriteStringAttribute(  hDBF, 0, f=getDBFFieldNum("AT_ALG"), (LPCTSTR)attr->algo )) return -f;
	if (!DBFWriteDoubleAttribute(  hDBF, 0, f=getDBFFieldNum("AT_MV"), (double)attr->memberValue )) return -f;
	if (!DBFWriteDoubleAttribute(  hDBF, 0, f=getDBFFieldNum("AT_PN"), (int)attr->positive )) return -f;
	if (!DBFWriteIntegerAttribute( hDBF, 0, f=getDBFFieldNum("AT_MSK"), (int)attr->useMask )) return -f;
	if (!DBFWriteStringAttribute(  hDBF, 0, f=getDBFFieldNum("AT_MSKF"), (LPCTSTR)attr->maskFileName )) return -f;
	if (!DBFWriteDoubleAttribute(  hDBF, 0, f=getDBFFieldNum("AT_INV"), (int)attr->inverseMaskValue )) return -f;

	if (!DBFWriteDoubleAttribute(  hDBF, 0, f=getDBFFieldNum("SP_X"), sp->x )) return -f;
	if (!DBFWriteDoubleAttribute(  hDBF, 0, f=getDBFFieldNum("SP_Y"), sp->y )) return -f;
	if (!DBFWriteDoubleAttribute(  hDBF, 0, f=getDBFFieldNum("SP_SD"), (double)sp->searchDist )) return -f;
	if (!DBFWriteLogicalAttribute( hDBF, 0, f=getDBFFieldNum("SP_UDS"), (int)sp->useDistSim )) return -f;
	if (!DBFWriteDoubleAttribute(  hDBF, 0, f=getDBFFieldNum("SP_DDF"), (double)sp->distDecayFactor )) return -f;
	if (!DBFWriteDoubleAttribute(  hDBF, 0, f=getDBFFieldNum("SP_VHF"), (double)sp->VtoHConvertingFactor )) return -f;
	if (!DBFWriteIntegerAttribute(  hDBF, 0, f=getDBFFieldNum("SP_IA"), sp->integratingAlgo )) return -f;
	if (!DBFWriteDoubleAttribute(  hDBF, 0, f=getDBFFieldNum("SP_WDF"), (double)sp->wgtDecayFactor )) return -f;

	FeatureNode *fNode = fList->first;
	int off;
	for (int k=0; k < fList->numOfNodes; k++) {  // number of FeatureNodes per List
		off = k*numDBFNodeFields + numDBFMainFields;
		if (!DBFWriteIntegerAttribute( hDBF, 0, f=(getDBFFieldNum("FT_ID")+off), fNode->id )) return -f;
		if (!DBFWriteStringAttribute(  hDBF, 0, f=(getDBFFieldNum("FT_NAME")+off), (LPCTSTR)fNode->name)) return -f;
		if (!DBFWriteLogicalAttribute( hDBF, 0, f=(getDBFFieldNum("FT_UON")+off), (int)fNode->useOrNot )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, 0, f=(getDBFFieldNum("FT_WT")+off), (double) fNode->weight )) return -f;
		if (!DBFWriteStringAttribute(  hDBF, 0, f=(getDBFFieldNum("FT_FT")+off), (LPCTSTR)fNode->functionType)) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, 0, f=(getDBFFieldNum("FT_V1")+off), (double)fNode->value1 )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, 0, f=(getDBFFieldNum("FT_W1")+off), (double)fNode->width1 )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, 0, f=(getDBFFieldNum("FT_R1")+off), (double)fNode->r1 )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, 0, f=(getDBFFieldNum("FT_K1")+off), (double)fNode->k1 )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, 0, f=(getDBFFieldNum("FT_V2")+off), (double)fNode->value2 )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, 0, f=(getDBFFieldNum("FT_W2")+off), (double)fNode->width2 )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, 0, f=(getDBFFieldNum("FT_R2")+off), (double)fNode->r2 )) return -f;
		if (!DBFWriteDoubleAttribute(  hDBF, 0, f=(getDBFFieldNum("FT_K2")+off), (double)fNode->k2 )) return -f;
		fNode = fNode->next;
	}

	nRec = 1;

	return nRec;
}

/************************************************************************/
/************************************************************************/
/* Read DBF and fill RasterCase structure								*/
/*	 1 if record 0 written successfully, or								*/
/*   0 if file handle null or file empty								*/
/*																		*/
/*   (Note: internal read functions (dbfopen.cpp) return NULL on error,	*/
/*    which cannot be distinguished from valid return value of 0)		*/
/************************************************************************/
int DBFRasterCase::readDBF( RasterCases * rc ){
	char * use;

	if (hDBF == NULL || nRec == 0) return 0;

	Attr * attr = rc->theAttr;
	Spatial * sp = rc->theSpatial;
	List<FeatureNode> * fl = attr->theFeatureList;

	// Read and store directly into RasterCases members
	rc->name = DBFReadStringAttribute( hDBF, 0, getDBFFieldNum("CB_NAME"));
	rc->rasterFileName = DBFReadStringAttribute( hDBF, 0, getDBFFieldNum("R_NAME"));

	attr->id = DBFReadIntegerAttribute( hDBF, 0, getDBFFieldNum("AT_ID"));
	attr->name = DBFReadStringAttribute( hDBF, 0, getDBFFieldNum("AT_NAME"));
	attr->algo = DBFReadStringAttribute( hDBF, 0, getDBFFieldNum("AT_ALG"));
	attr->memberValue = (float) DBFReadDoubleAttribute( hDBF, 0, getDBFFieldNum("AT_MV"));
	attr->positive = DBFReadDoubleAttribute( hDBF, 0, getDBFFieldNum("AT_PN"));
	attr->useMask = DBFReadIntegerAttribute( hDBF, 0, getDBFFieldNum("AT_MSK"));
	attr->maskFileName = DBFReadStringAttribute( hDBF, 0, getDBFFieldNum("AT_MSKF"));
	attr->inverseMaskValue = DBFReadIntegerAttribute( hDBF, 0, getDBFFieldNum("AT_INV"));

	sp->x = DBFReadDoubleAttribute( hDBF, 0, getDBFFieldNum("SP_X"));
	sp->y = DBFReadDoubleAttribute( hDBF, 0, getDBFFieldNum("SP_Y"));
	sp->searchDist = (float) DBFReadDoubleAttribute( hDBF, 0, getDBFFieldNum("SP_SD"));
	sp->useDistSim = DBFReadIntegerAttribute( hDBF, 0, getDBFFieldNum("SP_UDS"));
	sp->distDecayFactor = (float) DBFReadDoubleAttribute( hDBF, 0, getDBFFieldNum("SP_DDF"));
	sp->VtoHConvertingFactor = (float) DBFReadDoubleAttribute( hDBF, 0, getDBFFieldNum("SP_VHF"));
	sp->integratingAlgo = DBFReadIntegerAttribute( hDBF, 0, getDBFFieldNum("SP_IA"));
	sp->wgtDecayFactor = (float) DBFReadDoubleAttribute( hDBF, 0, getDBFFieldNum("SP_WDF"));

	int off;
	int numFeatureNodes = (nFields-numDBFMainFields)/numDBFNodeFields;

	for (int i=0; i<numFeatureNodes; i++){
		off = i*numDBFNodeFields + numDBFMainFields;
		FeatureNode * fn = new FeatureNode;
		fn->id = DBFReadIntegerAttribute( hDBF, 0, (getDBFFieldNum("FT_ID")+off ));
		fn->name = DBFReadStringAttribute( hDBF, 0, (getDBFFieldNum("FT_NAME")+off ));
		fn->useOrNot = DBFReadIntegerAttribute( hDBF, 0, (getDBFFieldNum("FT_UON")+off));
		fn->weight =  (float) DBFReadDoubleAttribute(  hDBF, 0, (getDBFFieldNum("FT_WT")+off));
		fn->functionType = DBFReadStringAttribute(  hDBF, 0, (getDBFFieldNum("FT_FT")+off ));
		fn->value1 = (float) DBFReadDoubleAttribute( hDBF, 0, (getDBFFieldNum("FT_V1")+off));
		fn->width1 = (float) DBFReadDoubleAttribute(  hDBF, 0, (getDBFFieldNum("FT_W1")+off));
		fn->r1 = (float) DBFReadDoubleAttribute(  hDBF, 0, (getDBFFieldNum("FT_R1")+off));
		fn->k1 = (float) DBFReadDoubleAttribute(  hDBF, 0, (getDBFFieldNum("FT_K1")+off));
		fn->value2 = (float) DBFReadDoubleAttribute( hDBF, 0, (getDBFFieldNum("FT_V2")+off));
		fn->width2 = (float) DBFReadDoubleAttribute(  hDBF, 0, (getDBFFieldNum("FT_W2")+off));
		fn->r2 = (float) DBFReadDoubleAttribute(  hDBF, 0, (getDBFFieldNum("FT_R2")+off));
		fn->k2 = (float) DBFReadDoubleAttribute(  hDBF, 0, (getDBFFieldNum("FT_K2")+off));

		fl->addTail( fn );
	}

	return 1;
}

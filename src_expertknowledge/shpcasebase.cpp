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
 * SHPCaseBase
 *
 * Purpose:  Class for I/O of CaseBase data structures to SHP/DBF
 * Author:   Erika Dade
 * Date:	 17 April 2005
 *
 ******************************************************************************
 */
#include "stdafx.h"
#ifndef _SHPCASEBASE_H
#include "shpcasebase.h"
#endif


/************************************************************************/
/************************************************************************/
// Constructor/Destructor
/************************************************************************/

SHPCaseBase::SHPCaseBase( int nShapeTypeID )  : DBFIO(){

	// Define DBF fields unique to this class first
	int numDBFCbFields = 2;
	struct DBFFieldDefinition tempCbFields[2] =
	{
	//   Name		TypeEnum	Size    Number	Position
		{"UID",		FTInteger,	 8,		0},		// 0
		{"CB_NAME", FTString,	30,		0},		// 1
	};

	int numDBFClFields = 7;
	struct DBFFieldDefinition tempClFields[7] =
	{
	//   Name		TypeEnum	Size    Number	Position
		{"CL_ID",	FTInteger,	 8,		0},		// 2
		{"CL_TID",	FTInteger,	 8,		0},		// 3
		{"CL_UID",	FTInteger,	 8,		0},		// 4
		{"CL_NAME", FTString,	30,		0},		// 5
		{"CL_MSK",	FTInteger,	 8,		0},		// 6
		{"CL_MSKF", FTString,	200,	0},		// 7
		{"CL_INV",	FTInteger,	 8,		0},		// 8
	};

	int numDBFCsFields = 2;
	struct DBFFieldDefinition tempCsFields[2] =
	{
	//   Name		TypeEnum	Size    Number	Position
		{"CS_CNT",	FTString,	30,		0},		// 9
		{"CS_TLR",	FTDouble,	10,		2},		// 10
	};

	for (int i=0; i<numDBFCbFields; i++ ) field[i] = tempCbFields[i];
	numDBFMainFields = numDBFCbFields;
	for (i=0; i<numDBFClFields; i++ ) field[numDBFMainFields+i] = tempClFields[i];
	numDBFMainFields += numDBFClFields;
	if(nShapeTypeID != SHPT_POINT){ //line/poly case, 2 extra fields at the case level
		for (i=0; i<numDBFCsFields; i++ ) field[numDBFMainFields+i] = tempCsFields[i];
		numDBFMainFields += numDBFCsFields;
	}
	// Add Attr and Spatial fields to main block from base class
	for (i=0; i<numDBFAttrFields; i++ ) field[numDBFMainFields+i] = attrField[i];
	numDBFMainFields += numDBFAttrFields;
	for (i=0; i<numDBFSpatialFields; i++) field[numDBFMainFields+i] = spatialField[i];
	numDBFMainFields += numDBFSpatialFields;	// Number of primary DBF fields in main block

	hSHP = NULL;
	nShapeType = nShapeTypeID;
}

SHPCaseBase::~SHPCaseBase()
{
	if (hSHP != NULL) closeFiles();
}

/************************************************************************/
/************************************************************************/
/* verifySHPFile()														*/
/*																		*/
/*  Returns number of records if OK, or -1 on error.					*/
/************************************************************************/
int SHPCaseBase::verifySHPFile( int expectedShapeType )
{
	// Make sure we're all talking about the same geometry
	if ( SHPCaseBase::nShapeType != expectedShapeType ) return -1;

	// Test SHAPEfile handle
	if (hSHP == NULL) return -1;

	//  Verify geometry in file
	int nEntities;
	int nShapeTypeID;
	double * adfMinBound = new double[4];
	double * adfMaxBound = new double[4];
    SHPGetInfo( hSHP, &nEntities, &nShapeTypeID, adfMinBound, adfMaxBound );

	delete [] adfMinBound;
	delete [] adfMaxBound;

	if ( nShapeTypeID != expectedShapeType ) return (-1);

	return nEntities;
}


/************************************************************************/
/************************************************************************/
/* openFiles( CString, int )	- FOR WRITE								*/
/*                                                                      */
/* Overloaded function													*/
/* Create new SHP/DBF files for CaseBase (OR overwrite existing)		*/
/* Returns -1 if error opening Shapefile								*/
/*		   -2 if error opening DBF file									*/
/*		   -3 if Shapefile is wrong geometry type for this CaseBase		*/
/************************************************************************/
int SHPCaseBase::openFiles( CString fileName, int shapeType )
{
	if ( (shapeType != SHPCaseBase::nShapeType) ||
			(shapeType != SHPT_POINT && 
			 shapeType != SHPT_ARC && 
	   		 shapeType != SHPT_POLYGON) )
	{	
		hSHP = NULL;
		return -3;
	}
	// fileName path and base are used for SHP and DBF filenames

	hSHP = SHPCreate( (LPCTSTR) fileName, shapeType );
	if (hSHP->fpSHP == NULL) return -1;

	hDBF = DBFCreate( (LPCTSTR) fileName );
	if (hDBF->fp == NULL) {
		SHPClose( hSHP );
		return -2;
	}

	readOnly = false;

	return 0;
}

/************************************************************************/
/* openFiles( CString )		- FOR READ									*/
/*																		*/
/* Overloaded function													*/
/* Open existing SHP/DBF files for CaseBase.							*/
/* Returns number of records if OK										*/
/* Returns -1 if error opening Shapefile								*/
/*		   -2 if error opening or verifying DBF file					*/
/*		   -3 if Shapefile is wrong geometry type for this CaseBase		*/
/************************************************************************/
int SHPCaseBase::openFiles( CString fileName )
{
	int nEntities;

	// Open existing Shapefile
	hSHP = SHPOpen( (LPCTSTR) fileName, "rb" );
	if (hSHP->fpSHP == NULL) return -1;
 
	// Check that Shapefile geometry is what we expect it to be
	if ( (nEntities = verifySHPFile( nShapeType )) < 0 ) return -3;

	// Open existing DBF file 
	hDBF = DBFOpen( (LPCTSTR) fileName, "rb" );
	if (hDBF->fp == NULL) {
		//SHPClose( hSHP );
		return -2;
	}

	readOnly = true;

	// Verify DBF info
	nFields = DBFGetFieldCount( hDBF );
	nRec = DBFGetRecordCount( hDBF );

    if ( ( nRec == 0 ) ||				// DBF file is empty
		 ( nEntities != nRec ) ||		// Number of records in Shapefile != number in DBF file
		 ( verifyDBFFile() < 0 ) ) {    // DBF fields not defined correctly

		//SHPClose( hSHP );  //comment out, because if close the file here, the caseio cannot be deleted (the files will be closed again in the destroyer
		//DBFClose( hDBF );
		return -2;
	}

	return nRec;
}

/************************************************************************/
/************************************************************************/
/* closeFiles()															*/
/************************************************************************/
void SHPCaseBase::closeFiles()
{
	SHPClose( hSHP );

	closeDBF();
}

/************************************************************************/
/************************************************************************/
/* Read DBF																*/
/*																		*/
/*	Read record 'n' in DBF file, assuming a CaseBase structure.			*/
/*																		*/
/*   (Note: internal read functions (dbfopen.cpp) return NULL on error,	*/
/*    which cannot be distinguished from valid return value of 0)		*/
/* Modified by Xun Shi on June 4, 2005 to test if a Shapefile that is   */
/*   a casebase                                                         */
/************************************************************************/
int SHPCaseBase::readDBF( const int n ){
	char *use;

	// Only need to read CaseBase fields once
	if (n == 0) {
		cbUID = DBFReadIntegerAttribute( hDBF, 0, getDBFFieldNum("UID"));
		cbName = DBFReadStringAttribute( hDBF, 0, getDBFFieldNum("CB_NAME"));
		//if(cbName==NULL) return -1; //if not a casebase file
	}

	clID = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("CL_ID"));
	clTypeID = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("CL_TID"));
	clUID = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("CL_UID"));
	clName = DBFReadStringAttribute( hDBF, n, getDBFFieldNum("CL_NAME"));
	clMsk = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("CL_MSK") );
	clMskF = DBFReadStringAttribute( hDBF, n, getDBFFieldNum("CL_MSKF") );
	clInv = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("CL_INV") );

	if(nShapeType != SHPT_POINT){ //line/poly case, 2 extra fields at the case level
		csCnt = DBFReadStringAttribute( hDBF, n, getDBFFieldNum("CS_CNT") );
		csTlr = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("CS_TLR"));
	}

	atID = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("AT_ID"));
	atName = DBFReadStringAttribute( hDBF, n, getDBFFieldNum("AT_NAME"));
	atAlg = DBFReadStringAttribute( hDBF, n, getDBFFieldNum("AT_ALG"));
	atMemberValue = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("AT_MV"));
	atPosNeg = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("AT_PN"));
	atMsk = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("AT_MSK") );
	atMskF = DBFReadStringAttribute( hDBF, n, getDBFFieldNum("AT_MSKF") );
	atInv = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("AT_INV") );

	spX = DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("SP_X"));
	spY = DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("SP_Y"));
	spSearchDist = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("SP_SD"));
	spUseDistSim = DBFReadIntegerAttribute( hDBF, 0, getDBFFieldNum("SP_UDS"));
	spDistDecay = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("SP_DDF"));
	spVtoH = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("SP_VHF"));
	spIntAlg = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("SP_IA"));
	spWeightDecay = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("SP_WDF"));

	int offset;
	int numFeatureNodes = (nFields-numDBFMainFields)/numDBFNodeFields;

	for (int i=0; i<numFeatureNodes; i++) {
		offset = i*numDBFNodeFields + numDBFMainFields;

		fnID[i] = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("FT_ID")+offset );
		fnName[i] = DBFReadStringAttribute( hDBF, n, (getDBFFieldNum("FT_NAME")+offset));
		fnUseOrNot[i] = DBFReadIntegerAttribute( hDBF, n, (getDBFFieldNum("FT_UON")+offset));
		fnWeight[i] =  (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("FT_WT")+offset );
		fnFunctionType[i] = DBFReadStringAttribute( hDBF, n, (getDBFFieldNum("FT_FT")+offset));
		if(fnFunctionType[i] == "Default")
				fnFunctionType[i] = "Continuous:bell-shape";
		fnValue1[i] = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("FT_V1")+offset );
		fnWidth1[i] = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("FT_W1")+offset );
		fnR1[i] = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("FT_R1")+offset );
		fnK1[i] = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("FT_K1")+offset );
		fnValue2[i] = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("FT_V2")+offset );
		fnWidth2[i] = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("FT_W2")+offset );
		fnR2[i] = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("FT_R2")+offset );
		fnK2[i] = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("FT_K2")+offset );
	}
	return 0;
}


/************************************************************************/
/************************************************************************/
/* writeSHP()							---POINTCASE---					*/
/*											---POINTCASE---				*/
/* Overloaded function							---POINTCASE---			*/
/* Output CaseBase structures to SHP/DBF			---POINTCASE---		*/
/************************************************************************/
int SHPCaseBase::writeSHP( const CaseBase<PointCase> *cb ){
	int i, j;
	int istat;
	int n = 0;
	double zNULL = 0.0;

	if ( hSHP == NULL || readOnly ) return -1;

	// Output one CasBase containing
	//   one or more CaseList containing
	//     one or more PointCase containing 
	//       one Spatial and 
	//       one Attr containing one List of one or more FeatureNode

	CaseList<PointCase> *cl = cb->first;
    PointCase *ptCase;

	// Figure out how many FeatureNodes there are in FeatureLists for this CaseBase
	// and set up the new DBF fields accordingly
	int numFeatureNodes = cl->first->theAttr->theFeatureList->numOfNodes;

	if ( (nFields = prepareDBFFields( numFeatureNodes )) <= 0) return -nFields;
	nRec = 0;
	
	// Write out SHP/DBF records, one per PointCase
	for (i=0; i<cb->numOfTypes; i++) {  // number of CaseLists

		ptCase = cl->first;
	
		for (j=0; j<cl->numCases; j++) {   // number of PointCases

			// Create a point shape object using Spatial for geometry
			SHPObject *pSHPObj = SHPCreateSimpleObject(	SHPT_POINT,
														1,
														&(ptCase->theSpatial->x),
														&(ptCase->theSpatial->y),
														&zNULL );

			// Append Shapefile record (Shape FID assigned automatically and is a sequential counter)
			//   Returns record number if OK. On error, returns -1 or uses assert
			if ((istat = SHPWriteObject(hSHP, -1, pSHPObj)) < 0) return istat;
			
			// Append DBF record
			//   Returns n+1 if OK
			if ((istat = writeDBF<PointCase>( n, cb, cl, ptCase )) <= 0) return istat;

			++n;

			SHPDestroyObject( pSHPObj );

			ptCase = ptCase->next;
		}
		cl = cl->next;
	}

	nRec = n;
	return n;
}

/************************************************************************/
/************************************************************************/
/* readSHP()							---POINTCASE---					*/
/*											---POINTCASE---				*/
/* Overloaded function							---POINTCASE---			*/
/* Read SHP/DBF and fill CaseBase structures		---POINTCASE---		*/
/* Modified by Xun Shi on June 4, 2005 to test if a Shapefile is        */
/*   a casebase                                                         */
/************************************************************************/
int SHPCaseBase::readSHP( CaseBase<PointCase> *cb ){
	int i, n;
	SHPObject *psShape;
	
	if ( hSHP == NULL || nRec == 0 ) return -1;
	int numFeatureNodes = (nFields - numDBFMainFields)/numDBFNodeFields;

	CaseList<PointCase> * cl;
	PointCase *ptCase;
	FeatureNode *fNode;

	// Read from SHP and DBF simultaneously and populate CaseBase
	for (n=0; n<nRec; n++) {
		psShape = SHPReadObject( hSHP, n );
		if(SHPCaseBase::readDBF(n)<0) return -2; //not a casebase

		// Create new CaseLists when necessary
		if (n == 0 || cl->id != clID) {
			// Set CaseBase info once
			if (n == 0) {
				cb->uid = cbUID;
				cb->name = cbName;
			}
			cl = new CaseList<PointCase>;
			cl->id = clID;
			cl->typeID = clTypeID;
			cl->uid = clUID;
			cl->name = clName;
			cl->useMask = clMsk;
			cl->maskFileName = clMskF;
			cl->inverseMaskValue = clInv;
			cb->addList( cl );
		}
		ptCase = new PointCase;

		ptCase->theAttr->id = atID;
		ptCase->theAttr->name = atName;
		ptCase->theAttr->algo = atAlg;
		ptCase->theAttr->memberValue = atMemberValue;
		ptCase->theAttr->positive = atPosNeg;
		ptCase->theAttr->useMask = atMsk;
		ptCase->theAttr->maskFileName = atMskF;
		ptCase->theAttr->inverseMaskValue = atInv;

		ptCase->theSpatial->x = psShape->padfX[0]; // From Shape geometry
		ptCase->theSpatial->y = psShape->padfY[0]; //  
		ptCase->theSpatial->searchDist = spSearchDist;
		ptCase->theSpatial->useDistSim = spUseDistSim;
		ptCase->theSpatial->distDecayFactor = spDistDecay;
		ptCase->theSpatial->VtoHConvertingFactor = spVtoH;
		ptCase->theSpatial->integratingAlgo = spIntAlg;
		ptCase->theSpatial->wgtDecayFactor = spWeightDecay;

		cl->addCase( ptCase );  
		
		for (i=0; i<numFeatureNodes; i++) {
			fNode = new FeatureNode(fnID[i], 
								fnUseOrNot[i], fnName[i], fnWeight[i], 
								fnFunctionType[i], 
								fnValue1[i], fnWidth1[i], fnK1[i], fnR1[i],
								fnValue2[i], fnWidth2[i], fnK2[i], fnR2[i]);
			ptCase->theAttr->theFeatureList->addTail( fNode );
		}
		SHPDestroyObject( psShape );
	}
	return 0;
}

/************************************************************************/
/************************************************************************/
/* writeSHP()							---LINECASE---					*/
/*											---LINECASE---				*/
/* Overloaded function							---LINECASE---			*/
/* Output CaseBase structures to SHP/DBF			---LINECASE---		*/
/************************************************************************/
int SHPCaseBase::writeSHP( const CaseBase<LineCase> *cb ){
	int i, j, k;
	int istat;
	int nP;
	int n = 0;
	double zNULL = 0.0;

	if ( hSHP == NULL || readOnly ) return -1;

	// Output one CasBase containing
	//   one or more CaseList containing
	//     one or more LineCase containing 
	//       one Spatial and 
	//       one Attr containing one List of one or more FeatureNode


	CaseList<LineCase> *cl = cb->first;
    LineCase *lineCase;


	// Figure out how many FeatureNodes there are in FeatureLists for this CaseBase
	// and set up the new DBF fields accordingly
	int numFeatureNodes = cl->first->theAttr->theFeatureList->numOfNodes;

	if ( (nFields = prepareDBFFields( numFeatureNodes )) <= 0) return -nFields;
	nRec = 0;
	
	// Write out SHP/DBF records, one per LineCase
	for (i=0; i<cb->numOfTypes; i++) {  // number of CaseLists

		lineCase = cl->first;
	
		for (j=0; j<cl->numCases; j++) {   // number of LineCases

			nP = lineCase->theLine->numOfNodes;
			IDDNode *node = lineCase->theLine->first;

			// Create a 1-part line shape object using List<IDDNODE> for geometry
			double *fX = new double [nP];
			double *fY = new double [nP];
			for (k=0; k<nP; k++) {
				fX[k] = node->d1;
				fY[k] = node->d2;
				node = node->next;
			}

			SHPObject *pSHPObj = SHPCreateSimpleObject(	SHPT_ARC,
														nP,
														fX,
														fY,
														&zNULL );

			// Append Shapefile record (Shape FID assigned automatically and is a sequential counter)
			if ((istat = SHPWriteObject(hSHP, -1, pSHPObj)) < 0) return istat;

			// Append DBF record
			if ((istat = writeDBF<LineCase>( n, cb, cl, lineCase )) <= 0) return istat;

			++n;

			SHPDestroyObject( pSHPObj );
			delete [] fX;
			delete [] fY;

			lineCase = lineCase->next;
		}
		cl = cl->next;
	}

	nRec = n;
	return n;
}

/************************************************************************/
/************************************************************************/
/* readSHP()							---LINECASE---					*/
/*											---LINECASE---				*/
/* Overloaded function							---LINECASE---			*/
/* Read SHP/DBF and fill CaseBase structures		---LINECASE---		*/
/* Modified by Xun Shi on June 4, 2005 to test if a Shapefile is        */
/*   a casebase                                                         */
/************************************************************************/
int SHPCaseBase::readSHP( CaseBase<LineCase> *cb ){
	int i, n;
	SHPObject *psShape;

	if ( hSHP == NULL || nRec == 0 ) return -1;
	int numFeatureNodes = (nFields - numDBFMainFields)/numDBFNodeFields;

	CaseList<LineCase> * cl;
	LineCase *lineCase;
	FeatureNode *fNode;

	// Read from SHP and DBF simultaneously and populate CaseBase
	for (n=0; n<nRec; n++) {
		psShape = SHPReadObject( hSHP, n );
		if(SHPCaseBase::readDBF(n)<0) return -2; //not a casebase

		// Create new CaseLists when necessary
		if (n == 0 || cl->id != clID) {
			// Set CaseBase info once
			if (n == 0) {
				cb->uid = cbUID;
				cb->name = cbName;
			}
			cl = new CaseList<LineCase>;
			cl->id = clID;
			cl->typeID = clTypeID;
			cl->uid = clUID;
			cl->name = clName;
			cl->useMask = clMsk;
			cl->maskFileName = clMskF;
			cl->inverseMaskValue = clInv;
			cb->addList( cl );
		}
		lineCase = new LineCase;

		// Pick up shapefile geometry
		for (i=0; i<psShape->nVertices; i++) {
			IDDNode *node = new IDDNode (i, psShape->padfX[i], psShape->padfY[i]);
			lineCase->theLine->addTail( node );
		}
		// Not changing the constructor-assigned defaults for other 'theLine' members...
		lineCase->theCentralValueMethod = csCnt;
		lineCase->modeTolerance = csTlr;

		lineCase->theAttr->id = atID;
		lineCase->theAttr->name = atName;
		lineCase->theAttr->algo = atAlg;
		lineCase->theAttr->memberValue = atMemberValue;
		lineCase->theAttr->positive = atPosNeg;
		lineCase->theAttr->useMask = atMsk;
		lineCase->theAttr->maskFileName = atMskF;
		lineCase->theAttr->inverseMaskValue = atInv;

		lineCase->theSpatial->x = spX;
		lineCase->theSpatial->y = spY;  
		lineCase->theSpatial->searchDist = spSearchDist;
		lineCase->theSpatial->useDistSim = spUseDistSim;
		lineCase->theSpatial->distDecayFactor = spDistDecay;
		lineCase->theSpatial->VtoHConvertingFactor = spVtoH;
		lineCase->theSpatial->integratingAlgo = spIntAlg;
		lineCase->theSpatial->wgtDecayFactor = spWeightDecay;

		cl->addCase( lineCase );  
		
		for (i=0; i<numFeatureNodes; i++) {
			fNode = new FeatureNode(fnID[i], 
								fnUseOrNot[i], fnName[i], fnWeight[i], 
								fnFunctionType[i], 
								fnValue1[i], fnWidth1[i], fnK1[i], fnR1[i],
								fnValue2[i], fnWidth2[i], fnK2[i], fnR2[i]);

			lineCase->theAttr->theFeatureList->addTail( fNode );
		}
		SHPDestroyObject( psShape );
	}
	return 0;
}

/************************************************************************/
/************************************************************************/
/* writeSHP()							---POLYCASE---					*/
/*											---POLYCASE---				*/
/* Overloaded function							---POLYCASE---			*/
/* Output CaseBase structures to SHP/DBF			---POLYCASE---		*/
/************************************************************************/
int SHPCaseBase::writeSHP( const CaseBase<PolyCase> *cb )
{
	int i, j, k, m, nc;
	int istat;
	int nP, nKids;
	int n = 0;
	double zNULL = 0.0;
	SHPObject * pSHPObj;

	if ( hSHP == NULL || readOnly ) return -1;

	// Output one CasBase containing
	//   one or more CaseList containing
	//     one or more PolyCase containing 
	//       one xPolygon containing 0 or more children
	//       one Spatial and 
	//       one Attr containing one List of one or more FeatureNode


	CaseList<PolyCase> *cl = cb->first;
    PolyCase *polyCase;


	// Figure out how many FeatureNodes there are in FeatureLists for this CaseBase
	// and set up the new DBF fields accordingly
	int numFeatureNodes = cl->first->theAttr->theFeatureList->numOfNodes;

	if ( (nFields = prepareDBFFields( numFeatureNodes )) <= 0) return -nFields;
	nRec = 0;
	
	// Write out SHP/DBF records, one per PolyCase
	for (i=0; i<cb->numOfTypes; i++) {  // number of CaseLists

		polyCase = cl->first;
	
		for (j=0; j<cl->numCases; j++) {   // number of PolyCases

			nP = polyCase->thePoly->vertexList->numOfNodes;
			IDDNode *node = polyCase->thePoly->vertexList->first;

			// First part is the parent polygon
			double *fX = new double [nP];
			double *fY = new double [nP];
			for (k=0; k<nP; k++) {
				fX[k] = node->d1;
				fY[k] = node->d2;
				node = node->next;
			}
			
			// **********************************************************
			// If there is only a parent then create a single part object
			// **********************************************************
			// Might have to test if children has even allocated yet...
			if ((nKids = polyCase->thePoly->children->numOfNodes) == 0)

				pSHPObj = SHPCreateSimpleObject(	SHPT_POLYGON,
															nP,
															fX,
															fY,
															&zNULL );

			// **********************************************************
			// If there are children then create a multipart shape object
			// **********************************************************
			else {
				int nShapeID = n;	// Far as I can tell, this is ignored unless replacing a record
				int nParts = nKids + 1;
				int nVerts = nP;
				int *panPartStart = new int[nParts];
				int *panPartType = new int[nParts];

				// Need to count up total number of vertices for parent and children
                xPolygon* kidPoly = polyCase->thePoly->children->first;
				for (m=0; m<nKids; m++) {
					nVerts += kidPoly->vertexList->numOfNodes;
					kidPoly = kidPoly->next;
				}

				// Allocate arrays and copy parent
				double *fXX = new double [nVerts];
				double *fYY = new double [nVerts];
				for (k=0; k<nP; k++) {
					fXX[k] = fX[k];
					fYY[k] = fY[k];
				}
				*panPartStart = 0;
				*panPartType = SHPP_RING;

				// Go through children again, adding to vertice arrays
                kidPoly = polyCase->thePoly->children->first;

				for (m=1; m<=nKids; m++) {
					panPartStart[m] = nP;
					panPartType[m] = SHPP_RING;

					IDDNode* kidNode = kidPoly->vertexList->first;
					for (nc=0; nc<kidPoly->vertexList->numOfNodes; nc++) {
						fXX[nP] = kidNode->d1;
						fYY[nP] = kidNode->d2;
						kidNode = kidNode->next;
						++nP;
					}

					kidPoly = kidPoly->next;
				}
		
				// Make the multipart shape object
				pSHPObj = SHPCreateObject( SHPT_POLYGON, nShapeID, 
									  	   nParts, panPartStart, panPartType, 
										   nVerts, fXX, fYY, &zNULL, &zNULL );
		
				delete [] fXX;
				delete [] fYY;
				delete [] panPartStart;
				delete [] panPartType;
			}
			// **********************************************************

			// Append Shapefile record (Shape FID assigned automatically and is a sequential counter)
			if ((istat = SHPWriteObject(hSHP, -1, pSHPObj)) < 0) return istat;

			// Append DBF record
			if ((istat = writeDBF<PolyCase>( n, cb, cl, polyCase )) <= 0) return istat;

			++n;

			SHPDestroyObject( pSHPObj );
			delete [] fX;
			delete [] fY;

			polyCase = polyCase->next;
		}
		cl = cl->next;
	}

	nRec = n;
	return n;
}

/************************************************************************/
/************************************************************************/
/* readSHP()							---POLYCASE---					*/
/*											---POLYCASE---				*/
/* Overloaded function							---POLYCASE---			*/
/* Read SHP/DBF and fill CaseBase structures		---POLYCASE---		*/
/* Modified by Xun Shi on June 4, 2005 to test if a Shapefile is        */
/*   a casebase                                                         */
/************************************************************************/
int SHPCaseBase::readSHP( CaseBase<PolyCase> *cb ){
	int i, n;
	SHPObject *psShape;

	if ( hSHP == NULL || nRec == 0 ) return -1;
	int numFeatureNodes = (nFields - numDBFMainFields)/numDBFNodeFields;

	CaseList<PolyCase> * cl;
	PolyCase *polyCase;
	FeatureNode *fNode;

	// Read from SHP and DBF simultaneously. Populate CaseBase
	for (n=0; n<nRec; n++) {
		psShape = SHPReadObject( hSHP, n );
		if(SHPCaseBase::readDBF(n)<0) return -2;

		// Create new CaseLists when necessary
		if (n == 0 || cl->id != clID) {
			// Set CaseBase info once
			if (n == 0) {
				cb->uid = cbUID;
				cb->name = cbName;
			}
			cl = new CaseList<PolyCase>;
			cl->id = clID;
			cl->typeID = clTypeID;
			cl->uid = clUID;
			cl->name = clName;
			cl->useMask = clMsk;
			cl->maskFileName = clMskF;
			cl->inverseMaskValue = clInv;
			cb->addList( cl );
		}

		polyCase = new PolyCase;
		polyCase->thePoly->theID = n;

		// Pick up shapefile geometry.  
		int nodeID;
		int * partStart = psShape->panPartStart;
		bool child = false;   // First part is the parent
		int nP = 0;
		xPolygon * kidPoly;

		// Walk through all the vertices in Shape record
		for (i=0; i<psShape->nVertices; i++) {

			// Find start of each part
			if (i == *partStart) {
				nodeID = 0;		// Number the nodes in each polygon from zero
				nP += 1;		// Note the part number
				if (nP < psShape->nParts) 
					partStart += 1; // Increment start of part index pointer

				if (i > 0)  {	// Keep children separate
					if (child)     // If already have one, add it to parent's list
						polyCase->thePoly->children->addTail( kidPoly );
					else
						child = true;
					kidPoly = new xPolygon( nP-2 );  // Number kids from zero
				}
			}
			// Make node from this vertex
			IDDNode *node = new IDDNode (nodeID++, psShape->padfX[i], psShape->padfY[i]);

			// Add to parent, or
			if (!child)
				polyCase->thePoly->vertexList->addTail( node );
			// add node to a child
			else
				kidPoly->vertexList->addTail( node );
		}

		// Add last child to parent's list
		if (child) 
			polyCase->thePoly->children->addTail( kidPoly );

		// Not changing the constructor-assigned defaults for other 'thePoly' members...
		polyCase->theCentralValueMethod = csCnt;
		polyCase->modeTolerance = csTlr;

		polyCase->theAttr->id = atID;
		polyCase->theAttr->name = atName;
		polyCase->theAttr->algo = atAlg;
		polyCase->theAttr->memberValue = atMemberValue;
		polyCase->theAttr->positive = atPosNeg;
		polyCase->theAttr->useMask = atMsk;
		polyCase->theAttr->maskFileName = atMskF;
		polyCase->theAttr->inverseMaskValue = atInv;

		polyCase->theSpatial->x = spX;
		polyCase->theSpatial->y = spY;  
		polyCase->theSpatial->searchDist = spSearchDist;
		polyCase->theSpatial->useDistSim = spUseDistSim;
		polyCase->theSpatial->distDecayFactor = spDistDecay;
		polyCase->theSpatial->VtoHConvertingFactor = spVtoH;
		polyCase->theSpatial->integratingAlgo = spIntAlg;
		polyCase->theSpatial->wgtDecayFactor = spWeightDecay;

		cl->addCase( polyCase );  
		
		for (i=0; i<numFeatureNodes; i++) {
			fNode = new FeatureNode(fnID[i], 
								fnUseOrNot[i], fnName[i], fnWeight[i], 
								fnFunctionType[i], 
								fnValue1[i], fnWidth1[i], fnK1[i], fnR1[i],
								fnValue2[i], fnWidth2[i], fnK2[i], fnR2[i]);

			polyCase->theAttr->theFeatureList->addTail( fNode );
		}
			
		SHPDestroyObject( psShape );
	}
	return 0;
}

/**************************************************************************************
Functions for reading Shapefiles that are not Casebases yet.  Used by the "New" button.
***************************************************************************************/
//Reads Point type Shape files (not MultiPoint) and fills List of	IDDNodes.
//DBFFieldName must be a string field (soil type name)
int readShapePoints (CString shapeFileName, List<ICDDNode>* pointList, CString DBFFieldName){
	SHPHandle	hSHP;
	DBFHandle	hDBF;
	int			nShapeType, nEntities;
	int			IDfield;
	double 		adfMinBound[4], adfMaxBound[4];
	SHPObject	*psShape;
 
	//Open the passed shapefile.
	hSHP = SHPOpen( shapeFileName, "rb" );
	if( hSHP == NULL )	//Unable to open Shapefile
		return 1;

	//Get Shapefile info and check this is a Point file
	SHPGetInfo( hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound );
	if ( nShapeType != SHPT_POINT )	//printf( "Error: Shapefile must be Point type\n");
		return 2;

	//Open the associated DBF file.
    hDBF = DBFOpen( shapeFileName, "rb" );
    if( hDBF == NULL ) //if == NULL, printf( "Unable to open DBF file for: %s\n", shapeFileName );	printf( "Will assign a simple integer counter to the object ID\n" );
		return 3;
	else
		IDfield = DBFGetFieldIndex(hDBF, DBFFieldName);
    
	//Pick up shape objects and populate a List of IDDNodes
	for(int i = 0; i < nEntities; i++ ){
		psShape = SHPReadObject( hSHP, i );

		// Check object record for Point feature characteristics */
		if ( psShape->nSHPType != SHPT_POINT )	//printf( "Error: Shapefile must be Point type, not %s\n", SHPTypeName(psShape->nSHPType) );
			return 3;
		else if ( (psShape->nVertices != 1) || (psShape->nParts != 0) ) //printf( "Error: Point shapefile record expected to have 1 vertex and 0 parts\n");
			return 4;

		CString	ptName = DBFReadStringAttribute( hDBF, i, IDfield );

		//Assign each point to a Node in the list. Ignore part groupings.
		pointList->addTail( new ICDDNode(i, ptName, psShape->padfX[0], psShape->padfY[0]) );

        SHPDestroyObject( psShape );
    }

	//adjust id so that the same name (type) has the same id
	int id = 0;
	ICDDNode* tempPoint = pointList->first;
	if(tempPoint!=NULL) tempPoint->i=id;
	while(tempPoint!=NULL){
		bool flag = true;
		ICDDNode* tempPoint2 = pointList->first;
		while(tempPoint2!=tempPoint){
			if(tempPoint->c==tempPoint2->c){
				flag = false;
				tempPoint->i = tempPoint2->i;
				break;
			}
			tempPoint2=tempPoint2->next;
		}
		if(flag)
			tempPoint->i=++id;
		tempPoint=tempPoint->next;
	}

	//clean up
    SHPClose( hSHP );
	DBFClose( hDBF );

    return 0;
}

//Reads basic Line type Shapefiles and fills list of lines.
int readShapeLines(CString shapefileName, List<xPolyline>* lineList, List<ICNode>* typeNames, CString DBFFieldName){
    SHPHandle	hSHP;
	DBFHandle	hDBF;
    int			nShapeType, nEntities, IDfield;
    double 		adfMinBound[4], adfMaxBound[4];
	SHPObject	*psShape;

	//Open the passed shapefile.
    hSHP = SHPOpen( shapefileName, "rb" );
    if( hSHP == NULL )	//printf( "Unable to open Shapefile: %s\n", shapeFileName );
		return 1;

	//Get Shapefile info and check this is a line file
    SHPGetInfo( hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound );
	if ( nShapeType != SHPT_ARC ) //printf( "Error: Shapefile must be line type\n");
		return 2;

	//Open the associated DBF file.
	hDBF = DBFOpen( shapefileName, "rb" );
	if( hDBF == NULL ) //pintf( "Unable to open DBF file for: %s\n", shapeFileName );printf( "Will assign a simple integer counter to the object ID\n" );
		return 3;
	else
		IDfield = DBFGetFieldIndex(hDBF, DBFFieldName);

	// Read from SHP and DBF simultaneously
    for(int i = 0; i < nEntities; i++){
		psShape = SHPReadObject(hSHP, i);

		// Check object record for Polygon type
		if ( psShape->nSHPType != SHPT_ARC) //printf( "Error: Shape entity no. %d must be line type, not %s\n",	i, SHPTypeName(psShape->nSHPType) );
			return 4;

		typeNames->addTail(new ICNode(i, DBFReadStringAttribute(hDBF, i, IDfield)));
		lineList->addTail(new xPolyline(i));

		// Pick up shapefile geometry
		for (int n=0; n<psShape->nVertices; n++)
			lineList->last->vertexList->addTail(new IDDNode(n, psShape->padfX[n], psShape->padfY[n]));

		SHPDestroyObject( psShape );
	}

	//adjust id so that the same name (type) has the same id
	int id = 0;
	xPolyline* curLine = lineList->first;
	ICNode* tempNode = typeNames->first;
	if(curLine!=NULL && tempNode!=NULL){
		curLine->theID = id;
		tempNode->i=id;
	}
	while(curLine!=NULL && tempNode!=NULL){
		bool flag = true;
		xPolyline* curLine2 = lineList->first;
		ICNode* tempNode2 = typeNames->first;
		while(curLine2!=curLine && tempNode2 != tempNode){
			if(tempNode->c==tempNode2->c){
				flag = false;
				tempNode->i = tempNode2->i;
				curLine->theID = curLine2->theID;
				break;
			}
			curLine2 = curLine2->next;
			tempNode2=tempNode2->next;
		}
		if(flag){
			curLine->theID = ++id;
			tempNode->i=id;
		}
		curLine=curLine->next;
		tempNode=tempNode->next;
	}

    SHPClose( hSHP );
	DBFClose( hDBF );

	return 0;
}

//Reads basic Polygon type Shape files and fills List of xPolygons.
int readShapePolygons (CString shapeFileName, List<xPolygon>* polygonList, List<ICNode>* typeNames, CString DBFFieldName){
    SHPHandle	hSHP;
	DBFHandle	hDBF;
    int			nShapeType, nEntities, IDfield;
	int			nPartsIn = 0, nParents = 0, nChildren = 0;
	int			isContained;
    double 		adfMinBound[4], adfMaxBound[4];
	SHPObject	*psShape;

	const int	CHILDID = -9999;

	//Open the passed shapefile.
    hSHP = SHPOpen( shapeFileName, "rb" );
    if( hSHP == NULL )	//printf( "Unable to open Shapefile: %s\n", shapeFileName );
		return 1;

	//Get Shapefile info and check this is a Polygon file
    SHPGetInfo( hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound );
	if ( nShapeType != SHPT_POLYGON ) //printf( "Error: Shapefile must be Polygon type\n");
		return 2;

	//Open the associated DBF file.
	hDBF = DBFOpen( shapeFileName, "rb" );
	if( hDBF == NULL ) //pintf( "Unable to open DBF file for: %s\n", shapeFileName );printf( "Will assign a simple integer counter to the object ID\n" );
		return 3;
	else
		IDfield = DBFGetFieldIndex(hDBF, DBFFieldName);

	//Pick up shape objects and populate a List of xPolygons
	int id = 0; 
	//This id was added by Xun on 10/14/05: polygon ID.  
	//If the field has a String type (e.g., soil type for SoLIM), id = i
	//If the field has int type (e.g., zip code for CCTester), id = the field value.
    for(int i = 0; i < nEntities; i++ ){
		psShape = SHPReadObject( hSHP, i );

		// Check object record for Polygon type
		if ( psShape->nSHPType != SHPT_POLYGON ) //printf( "Error: Shape entity no. %d must be Polygon type, not %s\n",	i, SHPTypeName(psShape->nSHPType) );
			return 4;

		if(typeNames != NULL){ //The user has specified a String field and provide a typeNames list (e.g., for SoLIM)
			typeNames->addTail(new ICNode(i, DBFReadStringAttribute(hDBF, i, IDfield)));
			id = i;
		}
		else                  //The user has specified an int field and specify the typeNames list to be NULL (e.g., for CCTester to use zip code polygons).
			id = DBFReadIntegerAttribute(hDBF, i, IDfield);
		/* For any polygon containing islands, create a parent with island children. */
		/* Parent gets ZipCode ID and children assigned ID of -9999.				 */
		/* For any parts that lie separately (not contained within parent), create   */
		/* a new xPolygon, but with same ID.								 */

		/*	Three assumptions:														 */
        /*	  1) Assume 1st part is a parent of any contained polygons (outer ring). */
		/*	  2) Assume any other parts are either contained by previous parent or	 */
		/*          are separate (simple) polygons.									 */
		/*	  3) Assume any contained parts (inner rings) follow immediately after	 */
		/*			parent part.													 */

		/* NB, xPolygon assignments (=) and addTail are NOT deep copies */

		xPolygon * aPoly = new xPolygon(id);
		xPolygon * parentPoly = new xPolygon(id);
		isContained = FALSE;
		int jNode = 0;

		nPartsIn += psShape->nParts;

		for(int j = 0, iPart = 1; j < psShape->nVertices; j++ ){       
			// Find start of each additional part.
			if( iPart < psShape->nParts && psShape->panPartStart[iPart] == j ){
				if (isContained){  // Add child
					aPoly->theID = CHILDID;
					aPoly->getBound();
					parentPoly->children->addTail( aPoly );	
					nChildren++;
				}
				else{			// Add completed parent to master list
					parentPoly = aPoly; 
					parentPoly->theID = id;
					parentPoly->getBound();
					polygonList->addTail( parentPoly );
					isContained = TRUE;
					nParents++;
				}

				aPoly = new xPolygon(id);
				iPart++;
				jNode=0;
 			}
			
			// Create a node for this vertex
			IDDNode * pNode = new IDDNode(jNode++, psShape->padfX[j], psShape->padfY[j]);

			// Test if node is in parent (don't bother if we know some lie outside)
			if ((iPart > 0) && isContained && pointInSimplePoly(parentPoly, pNode) < 0) 
				isContained = FALSE;

			// Add node to this part's polygon
			aPoly->vertexList->addTail( pNode );
		}
			
		// Assign last child or parent polygon part
		if (isContained){
			aPoly->theID = CHILDID;
			aPoly->getBound();
			parentPoly->children->addTail( aPoly );
			nChildren++;
		}
		else{
			aPoly->theID = id;
			xPolygon *parentPoly = aPoly;
			parentPoly->getBound();
			polygonList->addTail( parentPoly );
			nParents++;
		}
        SHPDestroyObject( psShape );
    }

	//adjust id so that the same name (type) has the same id
	if(typeNames!=NULL){
		id = 0;
		xPolygon* curPoly = polygonList->first;
		ICNode* tempNode = typeNames->first;
		if(curPoly!=NULL && tempNode!=NULL){
			curPoly->theID = id;
			tempNode->i=id;
		}
		while(curPoly!=NULL && tempNode!=NULL){
			bool flag = true;
			xPolygon* curPoly2 = polygonList->first;
			ICNode* tempNode2 = typeNames->first;
			while(curPoly2!=curPoly && tempNode2 != tempNode){
				if(tempNode->c==tempNode2->c){
					flag = false;
					tempNode->i = tempNode2->i;
					curPoly->theID = curPoly2->theID;
					break;
				}
				curPoly2 = curPoly2->next;
				tempNode2=tempNode2->next;
			}
			if(flag){
				curPoly->theID = ++id;
				tempNode->i=id;
			}
			curPoly=curPoly->next;
			tempNode=tempNode->next;
		}
	}

	SHPClose( hSHP );
	DBFClose( hDBF );

    return 0;
}

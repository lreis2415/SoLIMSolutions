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
#include "stdafx.h"
#ifndef _DBFIO_H
#include "dbfio.h"
#endif


DBFIO::DBFIO()
{
	// Attr class
	struct DBFFieldDefinition tempA[numDBFAttrFields]=
	{
		{"AT_ID",	FTInteger,	 8,		0},
		{"AT_NAME", FTString,	30,		0},
		{"AT_ALG",	FTString,	30,		0},	
		{"AT_MV",	FTDouble,	30,		7}, //membership value
		{"AT_PN",	FTInteger,	 8,		0}, //positive/negative
		{"AT_MSK",	FTInteger,	 8,		0}, //use mask or not
		{"AT_MSKF",	FTString,	200,	0}, //mask file name
		{"AT_INV",	FTInteger,	 8,		0}	//inverse mask value or not
	};
	for (int i=0; i<numDBFAttrFields; i++ ) attrField[i] = tempA[i];

	// Spatial class
	struct DBFFieldDefinition tempS[numDBFSpatialFields] =
	{
		{"SP_X",	FTDouble,	30,		7},
		{"SP_Y",	FTDouble,	30,		7},
		{"SP_SD",	FTDouble,	30,		7}, //search distance
		{"SP_UDS",  FTInteger,	 8,		0}, //use distance similarity
		{"SP_DDF",	FTDouble,	30,		7}, //distance decay factor
		{"SP_VHF",	FTDouble,	30,		7}, //V to H factor
		{"SP_IA",	FTInteger,	 8,		0}, //integration algorighm
		{"SP_WDF",  FTDouble,	30,		7}  //weight decay factor
	};
	for (int i=0; i<numDBFSpatialFields; i++) spatialField[i] = tempS[i];

	// FeatureNode class - 16 fields per node 
	struct DBFFieldDefinition tempF[numDBFNodeFields] = 
	{
		{"FT_ID",	FTInteger,	 8,		0},		// 	1
		{"FT_NAME",	FTString,	30,		0},		//  2
		{"FT_UON",	FTInteger,	 8,		0},		// 	3 //use or not
		{"FT_WT",	FTDouble,	30,		7},		// 	4 //weight
		{"FT_FT",	FTString,	30,		0},		// 	5 //function
		{"FT_V1",	FTDouble,	30,		7},		// 	6
		{"FT_W1",	FTDouble,	30,		7},		// 	7
		{"FT_R1",	FTDouble,	30,		7},		// 	8
		{"FT_K1",	FTDouble,	30,		7},		// 	9
		{"FT_V2",	FTDouble,	30,		7},		// 	10
		{"FT_W2",	FTDouble,	30,		7},		// 	11
		{"FT_R2",	FTDouble,	30,		7},		// 	12
		{"FT_K2",	FTDouble,	30,		7}		// 	13	
	};

	for (int i=0; i<numDBFNodeFields; i++ ) nodeField[i] = tempF[i];

	hDBF = NULL;
	nRec = 0;
	nFields = 0;

}

DBFIO::~DBFIO()
{
	if ( hDBF != NULL ) closeDBF();
}

/************************************************************************/
/************************************************************************/
/* closeDBF																*/
/************************************************************************/
void DBFIO::closeDBF()
{
	if ( hDBF != NULL ) DBFClose( hDBF );

	nRec = 0;
	nFields = 0;
	hDBF = NULL;
}

/************************************************************************/
/************************************************************************/
/* prepareDBFFields()													*/
/*																		*/
/* Sets up DBF fields for output of a Case or RuleBase					*/
/*	Returns number of fields added if OK, or -field number if error		*/
/*  Numbering here is 1:n, while DBF uses 0:n-1							*/
/************************************************************************/
int DBFIO::prepareDBFFields( int numFeatureNodes )
{
	int fieldNum;
	int recSize = 0;
	int nF = 1;
	char nodeName[11];

	DBFFieldDefinition *fd;

	// Set main DBF fields 
	fd = field;

	for (int i=0; i<numDBFMainFields; i++) {
		recSize += fd->size;
		//if (recSize > DBF_MAX_RECORD_LENGTH) return -recSize;

		fieldNum = DBFAddField(hDBF, fd->name, fd->type, fd->size, fd->decimals);
		if (fieldNum == -1 ) return -nF;
		else nF++;

		++fd;
	}

	// Repeat node fields based on number of FeatureNodes per FeatureList
	// Field names have node number appended to name (eg FT_ID_1)
	for (int n=0; n<numFeatureNodes; n++) {

		fd = nodeField;

		for (int i=0; i<numDBFNodeFields; i++) {
			recSize += fd->size;
			//if (recSize > DBF_MAX_RECORD_LENGTH) return -recSize;

			sprintf(nodeName, "%s_%d", fd->name, n+1);
			fieldNum = DBFAddField(hDBF, nodeName, fd->type, fd->size, fd->decimals);
			if (fieldNum == -1 ) return -nF;
			else nF++;

			++fd;
		}
	}

	DBFRecordSize = recSize;

	return nF-1;
}

/************************************************************************/
/************************************************************************/
/* verifyDBFFile()														*/
/*																		*/
/*  Returns 0 if OK, or -fieldIndex if error (indices 1:n)				*/
/************************************************************************/
int DBFIO::verifyDBFFile()
{
	// Test file handle
	if (hDBF == NULL) return -1;
	// Make sure number of fields is plausible (including one or more FeatureNode groups)
	if ( ( (nFields-numDBFMainFields) % numDBFNodeFields ) != 0 ) return -1;

	// Check main fields
	int i, in;
	int nWidth;
	int nDecimals;
	char fieldName[30], nodeName[30];
	DBFFieldType fieldType;

	for (i=0; i<numDBFMainFields; i++) {
		fieldType = DBFGetFieldInfo( hDBF, i, fieldName, &nWidth, &nDecimals );
		if ( ( strcmp(field[i].name, fieldName) != 0 ) ||
			 ( field[i].type != fieldType ) ) return -(i+1);
	}

	// Check first group of node fields
	for (i=0; i<numDBFNodeFields; i++) {
		in = i+numDBFMainFields;
		fieldType = DBFGetFieldInfo( hDBF, in, fieldName, &nWidth, &nDecimals );
		sprintf(nodeName, "%s_%d", nodeField[i].name, 1);
		if ( ( strcmp(nodeName, fieldName) != 0 ) ||
			( nodeField[i].type != fieldType ) ) return -(in+1);
	}

	return 0;
}

/************************************************************************/
/************************************************************************/
/* getDBFFieldNum														*/
/*																		*/
/* Returns field number (0 to n-1) from name storage structure			*/
/* (NOT the DBF file).													*/
/* Note that nodeFields are numbered from 0 as well. Calling routine	*/
/* must supply the offset from the main fields to get actual DBF		*/
/* position for node fields.											*/
/************************************************************************/
int DBFIO::getDBFFieldNum( const CString name )
{
	// Look for match in main fields (if field name does NOT start with "FT")
	if (name.Find("FT_") < 0) {
		for (int i=0; i<numDBFMainFields; i++) {
			if ( name.Compare( _T(field[i].name) ) == 0) return i;
		}
	}

	// or look in FT fields for a match
	else {
		for (int i=0; i<numDBFNodeFields; i++) {
			if ( name.Compare( _T(nodeField[i].name) ) == 0) return i;
		}
	}

	// Error, field name not found
	return -1;
}


/************************************************************************/
/************************************************************************/
/* getDBFRecordSize														*/
/*																		*/
/************************************************************************/
int DBFIO::getDBFRecordSize()
{
	return DBFRecordSize;
}
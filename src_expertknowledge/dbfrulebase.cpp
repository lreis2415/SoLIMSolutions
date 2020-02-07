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
 * DBFRuleBase
 *
 * Purpose:  Class for I/O of RuleBase data structures to DBF
 * Author:   Erika Dade
 * Date:	 29 March 2005
 *
 ******************************************************************************
 */
#include "stdafx.h"
#ifndef _DBFRULEBASE_H
#include "dbfrulebase.h"
#endif

/************************************************************************/
/************************************************************************/
// Constructor/Copy Constructor/Destructor
/************************************************************************/

DBFRuleBase::DBFRuleBase() : DBFIO()
{
	// Number of primary DBF fields in main block
	numDBFMainFields = 9 + numDBFAttrFields;

	// Define DBF fields unique to this class first
	struct DBFFieldDefinition temp[9] =
	{
	//   Name		TypeEnum	Size    Number	Position
		{"UID",		FTInteger,	 8,		0},	  // 0
		{"RB_NAME", FTString,	30,		0},	  // 1
		{"RL_ID",	FTInteger,	 8,		0},	  // 2
		{"RL_TID",	FTInteger,	 8,		0},	  // 3
		{"RL_UID",	FTInteger,	 8,		0},	  // 4
		{"RL_NAME", FTString,	30,		0},	  // 5
		{"RL_MSK",	FTInteger,	 8,		0},	  // 6
		{"RL_MSKF", FTString,	200,	0},	  // 7
		{"RL_INV",	FTInteger,	 8,		0},	  // 8
	};

	for (int i=0; i<9; i++ ) field[i] = temp[i];

	// Add Attr fields to main block from base class (no Spatial)
	for (int i=0; i<numDBFAttrFields; i++ ) field[9+i] = attrField[i];

}

DBFRuleBase::~DBFRuleBase()
{
}

/************************************************************************/
/************************************************************************/
/* openDBF - Open DBF file for RuleBase read or write					*/
/************************************************************************/
int DBFRuleBase::openDBF( CString DBFName, bool bCreateNew )
{

	// Create a new DBF file (or overwrite existing one)
	if (bCreateNew) {
		hDBF = DBFCreate( (LPCTSTR) DBFName );
		if(hDBF==NULL) return -1;
		if (hDBF->fp == NULL) return -1;
		
		nRec = 0;
		readOnly = false;
	}

	// Open existing DBF file and get number of records and fields
	else {
		hDBF = DBFOpen( (LPCTSTR) DBFName, "rb" );
		if (hDBF->fp == NULL) return -1;

		nFields = DBFGetFieldCount( hDBF );
		nRec = DBFGetRecordCount( hDBF );
		readOnly = true;

		int istat;
		if ( (istat = verifyDBFFile() ) < 0) return istat;
	}

	return 0;
}


/************************************************************************/
/************************************************************************/
/* Output RuleBase structures to DBF									*/
/* Returns:																*/
/*	 n if written successfully, or										*/
/*   0 if file handle null or readonly, or								*/
/*   -1 if write error occurred on any field							*/
/************************************************************************/
int DBFRuleBase::writeDBF( const RuleBase *rb )
{
	int i, j, k;
	int n = 0;
	char use;
	
	if ( hDBF == NULL || readOnly ) return 0;
		
	// Output a single RuleBase containing
	// multiple RuleLists, Rules and Lists of FeatureNodes
	if((nFields = prepareDBFFields(rb->first->first->theFeatureList->numOfNodes))<=0) return -nFields;

	List<FeatureNode> *fList;
	FeatureNode *fNode;
	RuleList *rl = rb->first;
	for (i=0; i<rb->numRuleLists; i++) {  // RuleList

		Attr *attr = rl->first;
		for (j=0; j<rl->numRules; j++) {   // Attr = Rules

			fList = attr->theFeatureList;   // FeatureNode list
			if (!DBFWriteIntegerAttribute( hDBF, n, getDBFFieldNum("UID"), rb->uid )) return -1;
			if (!DBFWriteStringAttribute(  hDBF, n, getDBFFieldNum("RB_NAME"), (LPCTSTR)rb->name )) return -1;
			if (!DBFWriteIntegerAttribute( hDBF, n, getDBFFieldNum("RL_ID"), rl->id )) return -1;
			if (!DBFWriteIntegerAttribute( hDBF, n, getDBFFieldNum("RL_TID"), rl->typeID )) return -1;
			if (!DBFWriteIntegerAttribute( hDBF, n, getDBFFieldNum("RL_UID"), rl->uid )) return -1;
			if (!DBFWriteStringAttribute(  hDBF, n, getDBFFieldNum("RL_NAME"), (LPCTSTR)rl->name )) return -1;
			if (!DBFWriteIntegerAttribute(  hDBF, n, getDBFFieldNum("RL_MSK"), (int)rl->useMask )) return -1;
			if (!DBFWriteStringAttribute(  hDBF, n, getDBFFieldNum("RL_MSKF"), (LPCTSTR)rl->maskFileName )) return -1;
			if (!DBFWriteIntegerAttribute(  hDBF, n, getDBFFieldNum("RL_INV"), (int)rl->inverseMaskValue )) return -1;
			if (!DBFWriteIntegerAttribute( hDBF, n, getDBFFieldNum("AT_ID"), attr->id )) return -1;
			if (!DBFWriteStringAttribute(  hDBF, n, getDBFFieldNum("AT_NAME"), (LPCTSTR)attr->name )) return -1;
			if (!DBFWriteStringAttribute(  hDBF, n, getDBFFieldNum("AT_ALG"), (LPCTSTR)attr->algo )) return -1;
			if (!DBFWriteDoubleAttribute(  hDBF, n, getDBFFieldNum("AT_MV"), (double)attr->memberValue )) return -1;
			if (!DBFWriteIntegerAttribute( hDBF, n, getDBFFieldNum("AT_PN"), (int)attr->positive )) return -1;
			if (!DBFWriteIntegerAttribute(  hDBF, n, getDBFFieldNum("AT_MSK"), (int)attr->useMask )) return -1;
			if (!DBFWriteStringAttribute(  hDBF, n, getDBFFieldNum("AT_MSKF"), (LPCTSTR)attr->maskFileName )) return -1;
			if (!DBFWriteIntegerAttribute(  hDBF, n, getDBFFieldNum("AT_INV"), (int)attr->inverseMaskValue )) return -1;

			fNode = fList->first;
			int offset = 0;
			for (k=0; k<fList->numOfNodes; k++) {
				offset = k*numDBFNodeFields + numDBFMainFields;

				if (!DBFWriteIntegerAttribute( hDBF, n, getDBFFieldNum("FT_ID")+offset, fNode->id )) return -1;
				if (!DBFWriteStringAttribute(  hDBF, n, getDBFFieldNum("FT_NAME")+offset, (LPCTSTR)fNode->name)) return -1;
				if (!DBFWriteIntegerAttribute( hDBF, n, getDBFFieldNum("FT_UON")+offset, (int)fNode->useOrNot )) return -1;
				if (!DBFWriteDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_WT")+offset, (double) fNode->weight )) return -1;
				if (!DBFWriteStringAttribute(  hDBF, n, getDBFFieldNum("FT_FT")+offset, (LPCTSTR)fNode->functionType)) return -1;
				if (!DBFWriteDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_V1")+offset, (double)fNode->value1 )) return -1;
				if (!DBFWriteDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_W1")+offset, (double)fNode->width1 )) return -1;
				if (!DBFWriteDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_R1")+offset, (double)fNode->r1 )) return -1;
				if (!DBFWriteDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_K1")+offset, (double)fNode->k1 )) return -1;
				if (!DBFWriteDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_V2")+offset, (double)fNode->value2 )) return -1;
				if (!DBFWriteDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_W2")+offset, (double)fNode->width2 )) return -1;
				if (!DBFWriteDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_R2")+offset, (double)fNode->r2 )) return -1;
				if (!DBFWriteDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_K2")+offset, (double)fNode->k2 )) return -1;

				fNode = fNode->next;
			}
			++n;
			attr = attr->next;
		}
		rl = rl->next;
	}

	nRec = n;
	return n;
}

/************************************************************************/
/************************************************************************/
// Read DBF and fill RuleBase structures
/************************************************************************/
int DBFRuleBase::readDBF( RuleBase *rb )
{
	int n;
	char *use;

	if (hDBF == NULL) return 0;

	if (nRec == 0) return 0;

	// Only need to read RuleBase fields once
	uid = DBFReadIntegerAttribute( hDBF, 0, getDBFFieldNum("UID") );
	rbName = DBFReadStringAttribute( hDBF, 0, getDBFFieldNum("RB_NAME") );
	rb->uid = uid;
    rb->name = rbName;

	RuleList * rl; 
	Attr *attr;
	FeatureNode *fNode;

	int numFeatureNodes = (nFields-numDBFMainFields)/numDBFNodeFields;

	for (n=0; n<nRec; n++) {
		rlID = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("RL_ID") );
		rlTypeID = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("RL_TID") );
		rlUID = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("RL_UID") );
		rlName = DBFReadStringAttribute( hDBF, n, getDBFFieldNum("RL_NAME") );
		rlMsk = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("RL_MSK") );
		rlMskF = DBFReadStringAttribute( hDBF, n, getDBFFieldNum("RL_MSKF") );
		rlInv = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("RL_INV") );
		atID = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("AT_ID") );
		atName = DBFReadStringAttribute( hDBF, n, getDBFFieldNum("AT_NAME") );
		atAlg = DBFReadStringAttribute( hDBF, n, getDBFFieldNum("AT_ALG") );
		atMemberValue = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("AT_MV") );
		atPosNeg = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("AT_PN") );
		atMsk = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("AT_MSK") );
		atMskF = DBFReadStringAttribute( hDBF, n, getDBFFieldNum("AT_MSKF") );
		atInv = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("AT_INV") );

		if (n == 0 || rl->id != rlID) { //New RuleList needed
			rl = new RuleList;
			rl->id = rlID;
			rl->typeID = rlTypeID;
			rl->uid = rlUID;
			rl->name = rlName;
			rl->useMask = rlMsk;
			rl->maskFileName = rlMskF;
			rl->inverseMaskValue = rlInv;
			rb->addList( rl );
			attr = new Attr;
			attr->id = atID;
			attr->name = atName;
			attr->algo = atAlg;
			attr->memberValue = atMemberValue;
			attr->positive = atPosNeg;
			attr->useMask = atMsk;
			attr->maskFileName = atMskF;
			attr->inverseMaskValue = atInv;
			rl->addRule( attr );
		}
		else if (attr == NULL || attr->id != atID) { //New Rule (Attr) needed
			attr = new Attr;
			attr->id = atID;
			attr->name = atName;
			attr->algo = atAlg;
			attr->memberValue = atMemberValue;
			attr->positive = atPosNeg;
			attr->useMask = atMsk;
			attr->maskFileName = atMskF;
			attr->inverseMaskValue = atInv;
			rl->addRule( attr );
		}

		int offset=0;
		for (int i=0; i<numFeatureNodes; i++){
			offset = i*numDBFNodeFields + numDBFMainFields;

			fnID = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("FT_ID")+offset );
			fnName = DBFReadStringAttribute( hDBF, n, getDBFFieldNum("FT_NAME")+offset );
			fnUseOrNot = DBFReadIntegerAttribute( hDBF, n, getDBFFieldNum("FT_UON")+offset );
			fnWeight =  (float) DBFReadDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_WT")+offset );
			fnFunctionType = DBFReadStringAttribute(  hDBF, n, getDBFFieldNum("FT_FT")+offset );
			if(fnFunctionType == "Default")
				fnFunctionType = "Continuous:bell-shape";
			fnValue1 = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("FT_V1")+offset );
			fnWidth1 = (float) DBFReadDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_W1")+offset );
			fnR1 = (float) DBFReadDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_R1")+offset );
			fnK1 = (float) DBFReadDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_K1")+offset );
			fnValue2 = (float) DBFReadDoubleAttribute( hDBF, n, getDBFFieldNum("FT_V2")+offset );
			fnWidth2 = (float) DBFReadDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_W2")+offset );
			fnR2 = (float) DBFReadDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_R2")+offset );
			fnK2 = (float) DBFReadDoubleAttribute(  hDBF, n, getDBFFieldNum("FT_K2")+offset );

			fNode = new FeatureNode(fnID, 
								fnUseOrNot, fnName, fnWeight, 
								fnFunctionType, 
								fnValue1, fnWidth1, fnK1, fnR1,
								fnValue2, fnWidth2, fnK2, fnR2);
			attr->theFeatureList->addTail( fNode );
		}
	}

	return nRec;
}

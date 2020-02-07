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
/****************************************************************************/
/*	DBFIO																	*/
/*																			*/
/*	Base class for DBF I/O of Rulebase and CaseBase data structures.		*/
/****************************************************************************/

#ifndef _DBFIO_H
#define _DBFIO_H

#include "stdafx.h"
#include <string>
#include <afx.h>
using namespace std;

#ifndef _DBFOPEN_H
#include "dbfopen.h"
#endif

class DBFIO {
public:
	int nFields;		// Number of DBF fields
	int nRec;			// Number of records in SHP/DBF files

	void closeDBF();
	int getDBFRecordSize();						 // Returns record length in bytes

protected:

	DBFHandle hDBF;		// DBF file handle
	
	bool readOnly;		   // Read-only flag

	int numDBFMainFields;  // Actual number of primary DBF fields (varies depending on Case or Rule)
	const static int maxMainFields = 32;       // Max number of primary DBF fields (allocated storage)

	const static int numDBFAttrFields = 8;	   // Number of fields in Attr group
	const static int numDBFSpatialFields = 8; // Number of fields in Spatial group

	const static int numDBFNodeFields = 13;    // Number of fields in a FeatureNode group
	const static int maxNodes = 10;	           // Maximum number of FeatureNode groups per DBF record

	int DBFRecordSize;	   // Length of DBF record in bytes

	// DBF field initializing definitions
	// Struct values must be assigned outside of class declaration; see constructor
	// Field position numbers are sequential from 0 and implicit in assignment order
	struct DBFFieldDefinition {	
		char		*name;		// field name
		DBFFieldType type;		// datatype
		int			 size;		// width in characters
		int			 decimals;  // number of decimals if double datatype
	};
	
	// Derived class sets the main fields of a DBF record
	struct DBFFieldDefinition field[maxMainFields];

	// Contained class members can be tacked on to main fields group as needed
	struct DBFFieldDefinition attrField[numDBFAttrFields];
	struct DBFFieldDefinition spatialField[numDBFSpatialFields];

	// Feature node classes added at end of DBF record, repeating as needed
	struct DBFFieldDefinition nodeField[numDBFNodeFields];

	DBFIO();
	~DBFIO();

    int prepareDBFFields( int numFeatureNodes ); // Initializes fields in new DBF file
	int verifyDBFFile();						 // Checks that fields are correct name and type
	int getDBFFieldNum( const CString name );    // Returns column number of field in DBF field definition structure
};

#endif
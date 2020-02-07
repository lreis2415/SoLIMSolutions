#ifndef _DBFOPEN_H
#define _DBFOPEN_H

/******************************************************************************
 * $Id: shapefil.h,v 1.26 2002/09/29 00:00:08 warmerda Exp $
 *
 * Project:  Shapelib
 * Purpose:  Primary include file for Shapelib.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 1999, Frank Warmerdam
 *
 * This software is available under the following "MIT Style" license,
 * or at the option of the licensee under the LGPL (see LICENSE.LGPL).  This
 * option is discussed in more detail in shapelib.html.
 *
 * --
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************
  */

#include <stdio.h>

#ifdef USE_DBMALLOC
#include <dbmalloc.h>
#endif

/************************************************************************/
/*                        Configuration options.                        */
/************************************************************************/

/* -------------------------------------------------------------------- */
/*      Should the DBFReadStringAttribute() strip leading and           */
/*      trailing white space?                                           */
/* -------------------------------------------------------------------- */
#define TRIM_DBF_WHITESPACE
    
/************************************************************************/
/*	Implements dBASE III plus (no memo)									*/
/*  4/21/2005	Size limits		EFD										*/
/************************************************************************/
#define DBF_MAX_RECORD_LENGTH  40000
#define DBF_MAX_FIELDS  1024

/************************************************************************/
/*                             DBF Support.                             */
/************************************************************************/
typedef	struct
{
    FILE	*fp;

    int     nRecords;

    int		nRecordLength;
    int		nHeaderLength;
    int		nFields;
    int		*panFieldOffset;
    int		*panFieldSize;
    int		*panFieldDecimals;
    char	*pachFieldType;

    char	*pszHeader;

    int		nCurrentRecord;
    int		bCurrentRecordModified;
    char	*pszCurrentRecord;
    
    int		bNoHeader;
    int		bUpdated;
} DBFInfo;

typedef DBFInfo * DBFHandle;

typedef enum {
  FTString,
  FTInteger,
  FTDouble,
  FTLogical,
  FTInvalid
} DBFFieldType;

#define XBASE_FLDHDR_SZ       32



DBFHandle DBFOpen( const char * pszDBFFile, const char * pszAccess );
DBFHandle DBFCreate( const char * pszDBFFile );

int	DBFGetFieldCount( DBFHandle psDBF );
int	DBFGetRecordCount( DBFHandle psDBF );
int	DBFAddField( DBFHandle hDBF, const char * pszFieldName,
                   DBFFieldType eType, int nWidth, int nDecimals );

DBFFieldType
      DBFGetFieldInfo( DBFHandle psDBF, int iField, 
                       char * pszFieldName, int * pnWidth, int * pnDecimals );

int DBFGetFieldIndex(DBFHandle psDBF, const char *pszFieldName);

int DBFReadIntegerAttribute( DBFHandle hDBF, int iShape, int iField );
double DBFReadDoubleAttribute( DBFHandle hDBF, int iShape, int iField );
const char * DBFReadStringAttribute( DBFHandle hDBF, int iShape, int iField );
const char * DBFReadLogicalAttribute( DBFHandle hDBF, int iShape, int iField );
int DBFIsAttributeNULL( DBFHandle hDBF, int iShape, int iField );

int DBFWriteIntegerAttribute( DBFHandle hDBF, int iShape, int iField, 
                                int nFieldValue );
int DBFWriteDoubleAttribute( DBFHandle hDBF, int iShape, int iField,
                               double dFieldValue );
int DBFWriteStringAttribute( DBFHandle hDBF, int iShape, int iField,
                               const char * pszFieldValue );
int DBFWriteNULLAttribute( DBFHandle hDBF, int iShape, int iField );

int DBFWriteLogicalAttribute( DBFHandle hDBF, int iShape, int iField,
			       const char lFieldValue);
int DBFWriteAttributeDirectly(DBFHandle psDBF, int hEntity, int iField,
                               void * pValue );
const char * DBFReadTuple(DBFHandle psDBF, int hEntity );
int DBFWriteTuple(DBFHandle psDBF, int hEntity, void * pRawTuple );

DBFHandle DBFCloneEmpty(DBFHandle psDBF, const char * pszFilename );
 
void DBFClose( DBFHandle hDBF );
char DBFGetNativeFieldType( DBFHandle hDBF, int iField );


#endif 

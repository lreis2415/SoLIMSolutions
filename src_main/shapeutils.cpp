// shapeutils.cpp 

/******************************************************************************
 *
 * Module:   Shapeutils
 * Purpose:  Utility for read-only access to ESRI Shapefiles (SHP/SHX) and
 *			 associated attribute table (DBF).  Uses Shapelib-1.2.10 functions. 
 * Modified: Erika Dade, 2005/1/11
 *
 * Revision: 
 */

/******************************************************************************
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
#include "stdafx.h"
#include "shapeutils.h"
#include "dbfopen.h"

//Reads Point type Shape files (not MultiPoint) and fills List of	IDDNodes.
//DBFFieldName must be a string field (soil type name)
int readShapePoints (CString shapeFileName, List<ICDDNode>* pointList, CString DBFFieldName){
	SHPHandle	hSHP;
	DBFHandle	hDBF;
	int			nShapeType, nEntities;
	int			ptID, IDfield;
	CString		ptName;
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
    if( hDBF != NULL ) //if == NULL, printf( "Unable to open DBF file for: %s\n", shapeFileName );	printf( "Will assign a simple integer counter to the object ID\n" );
		IDfield = DBFGetFieldIndex(hDBF, DBFFieldName);
    
	//Pick up shape objects and populate a List of IDDNodes
	for(int i = 0; i < nEntities; i++ ){
		psShape = SHPReadObject( hSHP, i );

		// Check object record for Point feature characteristics */
		if ( psShape->nSHPType != SHPT_POINT )	//printf( "Error: Shapefile must be Point type, not %s\n", SHPTypeName(psShape->nSHPType) );
			return 3;
		else if ( (psShape->nVertices != 1) || (psShape->nParts != 0) ) //printf( "Error: Point shapefile record expected to have 1 vertex and 0 parts\n");
			return 4;

		ptID = i;
		if (hDBF == NULL) //Get ID from DBF file. Uses entity count if DBF doesn't exist; Sets to 0 if attribute is NULL in DBF field.*/
			ptName = "";
		else
			ptName = DBFReadStringAttribute( hDBF, i, IDfield );

		//Assign each point to a Node in the list. Ignore part groupings.
		pointList->addTail( new ICDDNode(ptID, ptName, psShape->padfX[0], psShape->padfY[0]) );

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
	if (hDBF != NULL) 
		DBFClose( hDBF );

    return 0;
}

//Reads basic Polygon type Shape files and fills List of xPolygons.
int readShapePolygons (CString shapeFileName, List<xPolygon>* polygonList, CString DBFFieldName){
    SHPHandle	hSHP;
	DBFHandle	hDBF;
    int			nShapeType, nEntities, IDfield;
	int			nPartsIn = 0, nParents = 0, nChildren = 0;
	int			iZip, iPart, jNode;
	int			isContained;
    double 		adfMinBound[4], adfMaxBound[4];
	SHPObject	*psShape;

	const int	CHILDZIP = -9999;

	//Open the passed shapefile.
    hSHP = SHPOpen( shapeFileName, "rb" );
    if( hSHP == NULL )	//printf( "Unable to open Shapefile: %s\n", shapeFileName );
		return 1;

	//Get Shapefile info and check this is a Point file
    SHPGetInfo( hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound );
	if ( nShapeType != SHPT_POLYGON ) //printf( "Error: Shapefile must be Polygon type\n");
		return 2;

	//Open the associated DBF file.
	hDBF = DBFOpen( shapeFileName, "rb" );
	if( hDBF != NULL ) //if ==NULL,	printf( "Unable to open DBF file for: %s\n", shapeFileName );printf( "Will assign a simple integer counter to the object ID\n" );
		IDfield = DBFGetFieldIndex(hDBF, DBFFieldName);

	//Pick up shape objects and populate a List of xPolygons
    for(int i = 0; i < nEntities; i++ ){
		psShape = SHPReadObject( hSHP, i );

		// Check object record for Polygon type
		if ( psShape->nSHPType != SHPT_POLYGON ) //printf( "Error: Shape entity no. %d must be Polygon type, not %s\n",	i, SHPTypeName(psShape->nSHPType) );
			return 1;

		// Get ZipCode from DBF file. Uses entity count if DBF doesn't exist; Sets to 0 if attribute is NULL in DBF field.*/
		if (hDBF == NULL)
			iZip = i;
		else
			iZip = DBFReadIntegerAttribute( hDBF, i, IDfield );

		/* For any polygon containing islands, create a parent with island children. */
		/* Parent gets ZipCode ID and children assigned ID of -9999.				 */
		/* For any parts that lie separately (not contained within parent), create   */
		/* a new xPolygon, but with same ZipCode ID.								 */

		/*	Three assumptions:														 */
        /*	  1) Assume 1st part is a parent of any contained polygons (outer ring). */
		/*	  2) Assume any other parts are either contained by previous parent or	 */
		/*          are separate (simple) polygons.									 */
		/*	  3) Assume any contained parts (inner rings) follow immediately after	 */
		/*			parent part.													 */

		/* NB, xPolygon assignments (=) and addTail are NOT deep copies */

		xPolygon * aPoly = new xPolygon( iZip );
		xPolygon * parentPoly = new xPolygon( iZip );
		isContained = FALSE;
		jNode = 0;

		nPartsIn += psShape->nParts;

		for(int j = 0, iPart = 1; j < psShape->nVertices; j++ ){       
			// Find start of each additional part.
			if( iPart < psShape->nParts && psShape->panPartStart[iPart] == j ){
				if (isContained){  // Add child
					aPoly->theID = CHILDZIP;
					aPoly->getBound();
					parentPoly->children->addTail( aPoly );	
					nChildren++;
				}
				else{			// Add completed parent to master list
					parentPoly = aPoly; 
					parentPoly->theID = iZip;
					parentPoly->getBound();
					polygonList->addTail( parentPoly );
					isContained = TRUE;
					nParents++;
				}

				aPoly = new xPolygon( iZip );
				iPart++;
				jNode = 0;
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
			aPoly->theID = CHILDZIP;
			aPoly->getBound();
			parentPoly->children->addTail( aPoly );
			nChildren++;
		}
		else{
			aPoly->theID = iZip;
			xPolygon *parentPoly = aPoly;
			parentPoly->getBound();
			polygonList->addTail( parentPoly );
			nParents++;
		}

        SHPDestroyObject( psShape );

    }

    SHPClose( hSHP );
	if (hDBF != NULL) 
		DBFClose( hDBF );

    return 0;
}

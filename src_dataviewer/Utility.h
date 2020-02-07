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
//utility.h
//header file for utilities used by solim
//including:
//1. utilities for handling strings
//2. utilities for calc statistics
//3. utilities for handling file I/O
//4. others


#ifndef UTILITY_H
#define UTILITY_H


#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <afx.h>
#include <time.h>
#include <vector>

using namespace std;

#define STRLEN 200
#define CHUNK 200
#define VERYSMALL 0.00005
#define	VERYBIG 999999999999999


/*
#define NOINTVALUE -999999999
#define NOFLOATVALUE -9999999
#define NODOUBLEVALUE -999999999999999*/

#define NOINTVALUE -9999
#define NOFLOATVALUE -9999
#define NODOUBLEVALUE -9999


#ifndef FN_LEN
#define FN_LEN 400
#endif

#ifndef FNLEN
#define FNLEN FN_LEN
#endif

#ifndef MSG_LEN
#define MSG_LEN 4*FN_LEN
#endif

#ifndef FN_SHORT
#define FN_SHORT FN_LEN/2
#endif



#ifndef PALENTRYH
#define PALENTRYH
typedef struct{
	int index;
	float rasterval;
	unsigned char r,g,b;
} PALENTRY;
#endif


typedef struct PARTS 
{
	char piece[2*FN_LEN];
} PARTS;


typedef struct CASES {
	char pointID[FN_LEN];
	float x;
	float y;
	//int soilID;
	char observedSoilName[FN_LEN];
	int Inferred;
	int Observed;
	char inferredSoilName[FN_LEN];
	float ObservedProperty;
	float InferredProperty;
	bool IsMaskedOut;
	bool IsNoData;

} CASES;


typedef struct CLASSES {
	int ClassID;
	char ClassLabel[FN_LEN];
} CLASSES;



//common routines from solim_lib

void Beep(void);

void calcBasicStats(float array[], int theSize, int noDataValue, float *min, float *max, float *mean, float *stdDev);

char *cat_filename(char *dir, char *slash, char *name);

int  count_records(char *filename);

char *extract_filename(char *filespec);

char get_enter(void);

int get_mode_float(float *cat, float *img, int nrow, int ncol, int i, int j, int ksize, int skip, float skipval);

int get_mean_float(float *cat, float *img, int nrow, int ncol, int i, int j, int ksize, int skip, float skipval); //added by fei

void init_buffer_AXZ_float(float *buffer, int nrow, int ncol, float init_val);

void init_buffer_int(int *buffer, int nrow, int ncol, int init_val);

int output_AXZ_float(char *filename, float *buffer, int nrow, int ncol, char *err_msg);

int  read_record(FILE *fp, int *len, char *record);

int shellsort_2arr_int(int *arr1, int *arr2, int num);

int shellsort_2arr_float(float *matrix, float *second, int num, int order);

//char *strncpynm(char *s1, char *s2, int n, int m);

//char *trim(char *s1, char *s2);

//float min(float a, float b);

//float max(float a, float b);

//void dataMinMax(float *array, int theSize, float nodata, float minmax[]);

float dataMean(float array[], int theSize, int nodata);

float calcStdDev(float array[], int theSize, int nodata);





/******************************
 *	Utilities for Strings
 ******************************/
char *strncpynm(char *s1, char *s2, int n, int m);
char *trim(char *s1, char *s2);
CString extractFileName(CString s);
CString extractPathName(CString s);
CString dropFileSuffix(CString s);

int split_rec(char *piece, char *record, int start);
int countPieces(char *record);
PARTS *retrievePieces(char *line, PARTS *pieces, int numPieces);


/******************************
 *	Utilities for Statistics
 ******************************/
double calMin(double a, double b);
double calMax(double a, double b);
void dataMinMax(float* array, int theSize, float nodata, float minmax[]);
//the mean
float dataMean(float array[], int theSize, float nodata);
//the variance
float calcVariance(float array[], int theSize, float nodata);
//the standard deviation
float calcStdDev(float array[], int theSize, float nodata);
float mae(float* dataBuf1, float nodata1, float* dataBuf2, float nodata2, int numPixels);
float rmse(float* dataBuf1, float nodata1, float* dataBuf2, float nodata2, int numPixels);
float correlationCoefficent(float* dataBuf1, float nodata1, float* dataBuf2, float nodata2, int numPixels);
float rSquare(float* dataBuf1, float nodata1, float* dataBuf2, float nodata2, int numPixels);
//Agreement Coefficient (AC)
float AC(float* dataBuf1, float nodata1, float* dataBuf2, float nodata2, int numPixels);
//compare paired data and return the p Value
//The difference is calculated as array2 - array1. The eMean should be compatible with this setting.
float pPaired(float* firstArray, float* secondArray, int size, float nodata, float eMean);
//return p-Value for a given value in a normal distribution
float pNormalDistribution(float value, float mean, float std, bool singleSide);


/************************************************
 *	Utilities for Testing and Getting Neighbors
 ************************************************/
bool assertNW(int pos, int nrow, int ncol);
bool assertN(int pos, int nrow, int ncol);
bool assertNE(int pos, int nrow, int ncol);
bool assertW(int pos, int nrow, int ncol);
bool assertE(int pos, int nrow, int ncol);
bool assertSW(int pos, int nrow, int ncol);
bool assertS(int pos, int nrow, int ncol);
bool assertSE(int pos, int nrow, int ncol);
int getNW(int pos, int ncol);
int getN(int pos, int ncol);
int getNE(int pos, int ncol);
int getW(int pos, int ncol);
int getE(int pos, int ncol);
int getSW(int pos, int ncol);
int getS(int pos, int ncol);
int getSE(int pos, int ncol);


/************************************************
Utilities used by SoLIM 5.0
************************************************/
void split1(CString str,int x[]);
void split1(CString str,double x[]);
void split1(CString str,double *x,double *y);
bool IsFloat(CString str);
bool IsInteger(CString str);

int FormatDouble(double dNumIn,int L);  
int FormatFloat(float dNumIn,int L);  

int IntegerDigits(float dNumIn);

float effectiveDigits(float value);


/*********************************
 * Linked List Class
 *********************************/
template<class N>
class List{
public:
	//Attributes
	int id;
	double value;
	N* first;
	N* last;
	int numOfNodes;
	List* before;
	List* next;

	//Constructor and destructor
	List(){
		id = 0;
		value = NODOUBLEVALUE;
		first = NULL;
		last = NULL;
		numOfNodes = 0;
		before = NULL;
		next = NULL;
	};
	List(const List& aList){
		id = aList.id;
		value = aList.value;
		numOfNodes = aList.numOfNodes;
		if(aList.first == NULL){
			first = NULL;
			last = NULL;
		}
		else{
			N* temp = aList.first;
			first = new N(*temp);
			last = first;
			temp = temp->next;
			while(temp!=aList.last->next){
				last->next = new N(*temp);
				last->next->before = last;
				last = last->next;
				temp = temp->next;
			}
			first->before = NULL;
			last->next = NULL;
		}
		before = NULL;
		next = NULL;
	};
	~List(){
		while(first != NULL){
			N* temp = first;
			first = first->next;
			delete temp;
		}
		numOfNodes = 0;
		before = NULL;
		next = NULL;
	};

	//operations
	int addHead(N* aNode){
		if(aNode == NULL) return 1; //error: no sufficient memory
		if(first == NULL){
			first = aNode;
			last = aNode;
		}
		else{
			aNode->next = first;
			first->before = aNode;
			first = aNode;
		}
		first->before = NULL;
		last->next = NULL;
		numOfNodes++;
		return 0;
	};
	int addTail(N* aNode){
		if(aNode == NULL) return 1; //error: no sufficient memory
		if(first == NULL){
			first = aNode;
			last = aNode;
		}
		else{
			aNode->before = last;
			last->next = aNode;
			last = aNode;
		}
		first->before = NULL;
		last->next = NULL;
		numOfNodes++;
		return 0;
	};
	int addTail(List<N>* aList){
		if(aList == NULL) return 1; //error: no sufficient memory
		if(first == NULL){
			id = aList->id;
			value = aList->value;
			numOfNodes = aList->numOfNodes;
			if(aList->first == NULL){
				first = NULL;
				last = NULL;
			}
			else{
				first = aList->first;
				last = aList->last;
/*				N* temp = aList->first;
				first = new N(*temp);
				last = first;
				temp = temp->next;
				while(temp!=aList->last->next){
					last->next = new N(*temp);
					last->next->before = last;
					last = last->next;
					temp = temp->next;
				}*/
				first->before = NULL;
				last->next = NULL;
			}
		}
		else{
			if(aList->first!=NULL){
				last->next = aList->first;
				last->next->before = last;
				last = aList->last;
				first->before = NULL;
				last->next = NULL;
			}
		}
		numOfNodes += aList->numOfNodes;
		return 0;
	};
	int insertBefore(N* existingNode, N* newNode){
		if(newNode == NULL) return 1; //error: no sufficient memory
		if(first == NULL){
			first = newNode;
			last = newNode;
		}
		else if(first == existingNode){
			newNode->next = first;
			first->before = newNode;
			first = newNode;
		}
		else{
			N* temp = first;
			while(temp != NULL && temp != existingNode)
				temp = temp->next;
			if(temp == existingNode){
				newNode->next = temp;
				newNode->before = temp->before;
				temp->before->next = newNode;
				temp->before = newNode;
			}
		}
		first->before = NULL;
		last->next = NULL;
		numOfNodes++;
		return 0;
	};
	int insertAfter(N* existingNode, N* newNode){
		if(newNode == NULL) return 1; //error: no sufficient memory
		if(first == NULL){
			first = newNode;
			last = newNode;
		}
		else{
			N* temp = first;
			while(temp != NULL && temp != existingNode)
				temp = temp->next;
			if(temp == existingNode){
				newNode->next = temp->next;
				newNode->before = temp;
				temp->next = newNode;
				if(newNode->next == NULL)
					last = newNode;
				else 
					newNode->next->before = newNode;
			}
		}
		first->before = NULL;
		last->next = NULL;
		numOfNodes++;
		return 0;
	};
	void removeHead(){
		if(first == NULL) return;
		if(first==last){
			delete first;
			first = NULL;
			last = NULL;
			numOfNodes = 0;
			return;
		}
		first = first->next;
		delete first->before;
		first->before = NULL;
		numOfNodes--;
	};
	void removeTail(){
		if(last == NULL) return;
		if(first==last){
			delete first;
			first = NULL;
			last = NULL;
			numOfNodes = 0;
			return;
		}
		last = last->before;
		delete last->next;
		last->next = NULL;
		numOfNodes--;
	};
	void avoidHeadTail(N* aNode){ //if aNode is the head or tail, move the head or tail from it, but not delete it
		if(first == NULL) return;
		if(first == aNode && last == aNode){ //aNode is the only node in the list
			first = NULL;
			last = NULL;
			numOfNodes--;
			return;
		}
		if(first == aNode){
			first = first->next;
			numOfNodes--;
		}
		else if(last == aNode){
			last = last->before;
			numOfNodes--;
		}
		first->before = NULL;
		last->next = NULL;
	};
	void removeNode(N* aNode){ //remove (delete) aNode
		if(first == NULL) return;
		if(aNode == NULL) return;
		//To be safe for certain situations, do not explicitly set first->before and last->next to be NULL
		//but inherit whatever they are from the current nodes.
		N* temp = first;
		while(temp!=NULL){
			if(temp==aNode){
				if(temp->before != NULL)
					temp->before->next = temp->next;
				if(temp->next != NULL)
					temp->next->before = temp->before;
				break;
			}
			temp=temp->next;
		}
		if(first == aNode && last == aNode){ //aNode is the only node in the list
			first = NULL;
			last = NULL;
		}
		else if(first == aNode)	first = first->next;
		else if(last == aNode)	last = last->before;
		delete aNode;
		numOfNodes--;
	};
	void switchNodes(N* node1, N* node2){
		if(first == NULL) return;
		if(node1==node2) return;
		if(node1->before==NULL && first!=node1) return;
		if(node2->before==NULL && first!=node2) return;
		if(node1->next==node2){
			node1->next = node2->next;
			node2->before = node1->before;
			node1->before = node2;
			node2->next = node1;
			if(node1!=first)
				node2->before->next = node2;
			else
				first = node2;
			if(node2!=last)
				node1->next->before = node1;
			else
				last = node1;
		}
		else if(node2->next==node1){
			node2->next = node1->next;
			node1->before = node2->before;
			node2->before = node1;
			node1->next = node2;
			if(node2!=first)
				node1->before->next = node1;
			else
				first = node1;
			if(node1!=last)
				node2->next->before = node2;
			else
				last = node2;
		}
		else{
			N* next1 = node1->next;
			node1->next = node2->next;
			node2->next = next1;

			N* before1 = node1->before;
			node1->before = node2->before;
			node2->before = before1;

			if(node1 != first)
				node2->before->next = node2;
			if(node2 != first)
				node1->before->next = node1;
			if(node1==first)
				first = node2;
			else if(node2==first)
				first = node1;

			if(node1 != last)
				node2->next->before = node2;
			if(node2 != last)
				node1->next->before = node1;
			if(node1==last)
				last=node2;
			else if(node2==last)
				last = node1;
		}
		first->before = NULL;
		last->next = NULL;
	};
	void reverse(){ //reverse the order of the nodes
		if(first==NULL) return;
		N* before = first;
		N* cur = before->next;
		while(cur!=NULL){
			N* theNext = cur->next;
			cur->next = before;
			cur->before = theNext;
			before = cur;
			cur = theNext;
		}
		N* temp = first;
		first = last;
		last = temp;
		first->before = NULL;
		last->next = NULL;
	};
	void clean(){ //remove all the nodes
		N* temp = first;
		while(temp != NULL){
			first = first->next;
			delete temp;
			temp=first;
		}
		numOfNodes = 0;
		release();
	};
	void release(){
		first = NULL;
		last = NULL;
	};
};


/******************************
 *  Tree Class
 ******************************/
template<class N>
class Tree{
public:
	//Attributes
	int id;
	int numOfNodes;
	N* head;
	List<N>* thisList;

	//Constructor and destructor
	Tree(){
		id = 0;
		numOfNodes = 0;
		head = NULL;
		thisList = new List<N>;
	};
	Tree(const Tree& aTree){
		id = aTree.id;
		numOfNodes = aTree.numOfNodes;
		thisList =  = new List<N>;
		if(aTree.head == NULL)
			head = NULL;
		else{
			head = new N(*(aTree.head));
			thisList->clean();
			thisList->addTail(head);
			aTree.thisList->first=NULL;
			aTree.thisList->addTail(aTree.head);
			while(thisList->first != NULL){
				thisList->first-children = new List<N>(*(aTree.thisList->first->children->first));
				thisList->addTail(thisList->first->children);
				aTree.thisList->addTail(aTree.thisList->first->children);
				thisList->first = thisList->first->next;
				aTree.thisList->first = aTree.thisList->first->next;
			}
		}
	};
	~Tree(){
		thisList->release();
		thisList->addTail(head);
		while(thisList->first != NULL){
			thisList->addTail(thisList->first->children);
			N* temp = thisList->first;
			thisList->first = thisList->first->next;
			delete temp;
		}
		numOfNodes = 0;
		thisList->release();
		delete thisList;
		thisList = NULL;
	};

	//operations
	int addHead(N* aNode){
		if(aNode == NULL) return 1; //error: no sufficient memory
		if(head == NULL)
			head = aNode;
		else{
			aNode->children->addTail(head);
			head = aNode;
		}
		numOfNodes++;
		return 0;
	};
	int addChild(N* parent, N* child){
		if(child == NULL) return 1; //error: no sufficient memory
		if(head == NULL)
			head = child;
		else{
			thisList->first=NULL;
			thisList->addTail(head);
			while(thisList->first != NULL){
				if(thisList->first == parent){
					if(parent->children->numOfNodes == 0)
						numOfNodes++; //the tree's number of nodes increases
					parent->children->addTail(child);
					return 0;
				}
				thisList->addTail(thisList->first->children);
				thisList->first = thisList->first->next;
			}
		}
		return 2; //the specified parent is not in the tree
	};
	int addChild(int parentID, N* child){
		if(child == NULL) return 1; //error: no sufficient memory
		if(head == NULL)
			head = child;
		else{
			thisList->first=NULL;
			thisList->addTail(head);
			while(thisList->first != NULL){
				if(thisList->first->id == parentID){
					if(thisList->first->children->numOfNodes == 0)
						numOfNodes++; //the tree's number of nodes increases
					parent->children->addTail(child);
					return 0;
				}
				thisList->addTail(thisList->first->children);
				thisList->first = thisList->first->next;
			}
		}
		return 2; //the specified parent is not in the tree
	};
};


/******************************
 *  Node Classes
 ******************************/
class DNode{
public:
	double d;
	DNode* before;
	DNode* next;
	List<DNode>* children;

	DNode(double aD){
		d = aD;
		before = NULL;
		next = NULL;
		children = new List<DNode>;
	};
	DNode(const DNode& aDNode){
		d = aDNode.d;
		before = NULL;
		next = NULL;
		children = new List<DNode>;
	};
	~DNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class DDNode{
public:
	double d1;
	double d2;
	DDNode* before;
	DDNode* next;
	List<DDNode>* children;

	DDNode(double aD1, double aD2){
		d1 = aD1;
		d2 = aD2;
		before = NULL;
		next = NULL;
		children = new List<DDNode>;
	};
	DDNode(const DDNode& aDDNode){
		d1 = aDDNode.d1;
		d2 = aDDNode.d2;
		before = NULL;
		next = NULL;
		children = new List<DDNode>;
	};
	~DDNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class ICNode{
public:
	int i;
	CString c;
	ICNode* before;
	ICNode* next;
	List<ICNode>* children;

	ICNode(int anI, CString aC){
		i = anI;
		c = aC;
		before = NULL;
		next = NULL;
		children = new List<ICNode>;
	};
	ICNode(const ICNode& anICNode){
		i = anICNode.i;
		c = anICNode.c;
		before = NULL;
		next = NULL;
		children = new List<ICNode>;
	};
	~ICNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class IntNode{
public:
	int pos;
	IntNode* before;
	IntNode* next;
	List<IntNode>* children;

	IntNode(int p){
		pos = p;
		before = NULL;
		next = NULL;
		children = new List<IntNode>;
	};
	IntNode(const IntNode& anIntNode){
		pos = anIntNode.pos;
		before = NULL;
		next = NULL;
		children = new List<IntNode>;
	};
	~IntNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class IINode{
public:
	int i1;
	int i2;
	IINode* before;
	IINode* next;
	List<IINode>* children;

	IINode(int anI1, int anI2){
		i1 = anI1;
		i2 = anI2;
		before = NULL;
		next = NULL;
		children = new List<IINode>;
	};
	IINode(const IINode& anIINode){
		i1 = anIINode.i1;
		i2 = anIINode.i2;
		before = NULL;
		next = NULL;
		children = new List<IINode>;
	};
	~IINode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class IIINode{
public:
	int i1;
	int i2;
	int i3;
	IIINode* before;
	IIINode* next;
	List<IIINode>* children;

	IIINode(int anI1, int anI2, int anI3){
		i1 = anI1;
		i2 = anI2;
		i3 = anI3;
		before = NULL;
		next = NULL;
		children = new List<IIINode>;
	};
	IIINode(const IIINode& anIIINode){
		i1 = anIIINode.i1;
		i2 = anIIINode.i2;
		i3 = anIIINode.i3;
		before = NULL;
		next = NULL;
		children = new List<IIINode>;
	};
	~IIINode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class IIIINode{
public:
	int i1;
	int i2;
	int i3;
	int i4;
	IIIINode* before;
	IIIINode* next;
	List<IIIINode>* children;

	IIIINode(int anI1, int anI2, int anI3, int anI4){
		i1 = anI1;
		i2 = anI2;
		i3 = anI3;
		i4 = anI4;
		before = NULL;
		next = NULL;
		children = new List<IIIINode>;
	};
	IIIINode(const IIIINode& anIIIINode){
		i1 = anIIIINode.i1;
		i2 = anIIIINode.i2;
		i3 = anIIIINode.i3;
		i4 = anIIIINode.i4;
		before = NULL;
		next = NULL;
		children = new List<IIIINode>;
	};
	~IIIINode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class IDDNode{
public:
	int i;
	double d1;
	double d2;
	IDDNode* before;
	IDDNode* next;
	List<IDDNode>* children;

	IDDNode(int anI, double aD1, double aD2){
		i = anI;
		d1 = aD1;
		d2 = aD2;
		before = NULL;
		next = NULL;
		children = new List<IDDNode>;
	};
	IDDNode(const IDDNode& anIDDNode){
		i = anIDDNode.i;
		d1 = anIDDNode.d1;
		d2 = anIDDNode.d2;
		before = NULL;
		next = NULL;
		children = new List<IDDNode>;
	};
	~IDDNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class IDDDNode{
public:
	int i;
	double d1;
	double d2;
	double d3;
	IDDDNode* before;
	IDDDNode* next;
	List<IDDDNode>* children;
	IDDDNode(){
		i = NOINTVALUE;
		d1 = NODOUBLEVALUE;
		d2 = NODOUBLEVALUE;
		d3 = NODOUBLEVALUE;
		before = NULL;
		next = NULL;
		children = new List<IDDDNode>;
	};
	IDDDNode(int anI, double aD1, double aD2, double aD3){
		i = anI;
		d1 = aD1;
		d2 = aD2;
		d3 = aD3;
		before = NULL;
		next = NULL;
		children = new List<IDDDNode>;
	};
	IDDDNode(const IDDDNode& anIDDDNode){
		i = anIDDDNode.i;
		d1 = anIDDDNode.d1;
		d2 = anIDDDNode.d2;
		d3 = anIDDDNode.d3;
		before = NULL;
		next = NULL;
		children = new List<IDDDNode>;
	};
	~IDDDNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class ICDDDNode{
public:
	int i;
	CString c;
	double d1;
	double d2;
	double d3;
	ICDDDNode* before;
	ICDDDNode* next;
	List<ICDDDNode>* children;
	ICDDDNode(){
		i = NOINTVALUE;
		d1 = NODOUBLEVALUE;
		d2 = NODOUBLEVALUE;
		d3 = NODOUBLEVALUE;
		c = "";
		before = NULL;
		next = NULL;
		children = new List<ICDDDNode>;
	};
	ICDDDNode(int anI, CString aC, double aD1, double aD2, double aD3){
		i = anI;
		c = aC;
		d1 = aD1;
		d2 = aD2;
		d3 = aD3;
		before = NULL;
		next = NULL;
		children = new List<ICDDDNode>;
	};
	ICDDDNode(const ICDDDNode& anICDDDNode){
		i = anICDDDNode.i;
		c = anICDDDNode.c;
		d1 = anICDDDNode.d1;
		d2 = anICDDDNode.d2;
		d3 = anICDDDNode.d3;
		before = NULL;
		next = NULL;
		children = new List<ICDDDNode>;
	};
	~ICDDDNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class IDDDDNode{
public:
	int i;
	double d1;
	double d2;
	double d3;
	double d4;
	IDDDDNode* before;
	IDDDDNode* next;
	List<IDDDDNode>* children;
	IDDDDNode(){
		i = NOINTVALUE;
		d1 = NODOUBLEVALUE;
		d2 = NODOUBLEVALUE;
		d3 = NODOUBLEVALUE;
		d4 = NODOUBLEVALUE;
		before = NULL;
		next = NULL;
		children = new List<IDDDDNode>;
	};
	IDDDDNode(int anI, double aD1, double aD2, double aD3, double aD4){
		i = anI;
		d1 = aD1;
		d2 = aD2;
		d3 = aD3;
		d4 = aD4;
		before = NULL;
		next = NULL;
		children = new List<IDDDDNode>;
	};
	IDDDDNode(const IDDDDNode& anIDDDDNode){
		i = anIDDDDNode.i;
		d1 = anIDDDDNode.d1;
		d2 = anIDDDDNode.d2;
		d3 = anIDDDDNode.d3;
		d4 = anIDDDDNode.d4;
		before = NULL;
		next = NULL;
		children = new List<IDDDDNode>;
	};
	~IDDDDNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class IIDNode{
public:
	int i1;
	int i2;
	double d;
	IIDNode* before;
	IIDNode* next;
	List<IIDNode>* children;

	IIDNode(int anI1, int anI2, double aD){
		i1 = anI1;
		i2 = anI2;
		d = aD;
		before = NULL;
		next = NULL;
		children = new List<IIDNode>;
	};
	IIDNode(const IIDNode& anIIDNode){
		i1 = anIIDNode.i1;
		i2 = anIIDNode.i2;
		d = anIIDNode.d;
		before = NULL;
		next = NULL;
		children = new List<IIDNode>;
	};
	~IIDNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class IIDDNode{
public:
	int i1;
	int i2;
	double d1;
	double d2;
	IIDDNode* before;
	IIDDNode* next;
	List<IIDDNode>* children;

	IIDDNode(int anI1, int anI2, double aD1, double aD2){
		i1 = anI1;
		i2 = anI2;
		d1 = aD1;
		d2 = aD2;
		before = NULL;
		next = NULL;
		children = new List<IIDDNode>;
	};
	IIDDNode(const IIDDNode& anIIDDNode){
		i1 = anIIDDNode.i1;
		i2 = anIIDDNode.i2;
		d1 = anIIDDNode.d1;
		d2 = anIIDDNode.d2;
		before = NULL;
		next = NULL;
		children = new List<IIDDNode>;
	};
	~IIDDNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class IIIDDDNode{
public:
	int i1;
	int i2;
	int i3;
	double d1;
	double d2;
	double d3;
	IIIDDDNode* before;
	IIIDDDNode* next;
	List<IIIDDDNode>* children;

	IIIDDDNode(int anI1, int anI2, int anI3, double aD1, double aD2, double aD3){
		i1 = anI1;
		i2 = anI2;
		i3 = anI3;
		d1 = aD1;
		d2 = aD2;
		d3 = aD3;
		before = NULL;
		next = NULL;
		children = new List<IIIDDDNode>;
	};
	IIIDDDNode(const IIIDDDNode& anIIIDDDNode){
		i1 = anIIIDDDNode.i1;
		i2 = anIIIDDDNode.i2;
		i3 = anIIIDDDNode.i3;
		d1 = anIIIDDDNode.d1;
		d2 = anIIIDDDNode.d2;
		d3 = anIIIDDDNode.d3;
		before = NULL;
		next = NULL;
		children = new List<IIIDDDNode>;
	};
	~IIIDDDNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class ICDDNode{
public:
	int i;
	CString c;
	double d1;
	double d2;
	ICDDNode* before;
	ICDDNode* next;
	List<ICDDNode>* children;

	ICDDNode(int anI, CString aC, double aD1, double aD2){
		i = anI;
		c = aC;
		d1 = aD1;
		d2 = aD2;
		before = NULL;
		next = NULL;
		children = new List<ICDDNode>;
	};
	ICDDNode(const ICDDNode& anICDDNode){
		i = anICDDNode.i;
		c = anICDDNode.c;
		d1 = anICDDNode.d1;
		d2 = anICDDNode.d2;
		before = NULL;
		next = NULL;
		children = new List<ICDDNode>;
	};
	~ICDDNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class ValueNode{
public:
	int num;
	double value;
	ValueNode* before;
	ValueNode* next;
	List<ValueNode>* children;

	ValueNode(double v, int n){
		value = v;
		num = n;
		before = NULL;
		next = NULL;
		children = new List<ValueNode>;
	};
	ValueNode(const ValueNode& aValueNode){
		value = aValueNode.value;
		num = aValueNode.num;
		before = NULL;
		next = NULL;
		children = new List<ValueNode>;
	};
	~ValueNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class FCNode{
public:
	float f;
	CString c;
	FCNode* before;
	FCNode* next;
	List<FCNode>* children;

	FCNode(float anF, CString aC){
		f = anF;
		c = aC;
		before = NULL;
		next = NULL;
		children = new List<FCNode>;
	};
	FCNode(const FCNode& anFCNode){
		f = anFCNode.f;
		c = anFCNode.c;
		before = NULL;
		next = NULL;
		children = new List<FCNode>;
	};
	~FCNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class FFINode{
public:
	float f1;
	float f2;
	int i1;
	FFINode* before;
	FFINode* next;
	List<FFINode>* children;

	FFINode(float anF1, float anF2, int anI1){
		f1 = anF1;
		f2 = anF2;
		i1 = anI1;
		before = NULL;
		next = NULL;
		children = new List<FFINode>;
	};
	FFINode(const FFINode& anFFINode){
		f1 = anFFINode.f1;
		f2 = anFFINode.f2;
		i1 = anFFINode.i1;
		before = NULL;
		next = NULL;
		children = new List<FFINode>;
	};
	~FFINode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class FeatureNode{
public:
	int id;
	CString name;
	float mean;
	float min;
	float max;
	float std;
	float value1; //lower value
	float value2; //upper value

	bool useOrNot;
	float weight;
	CString functionType;
	float width1;
	float r1;
	float k1;
	float width2;
	float r2;
	float k2;

	FeatureNode* before;
	FeatureNode* next;
	List<FeatureNode>* children;
	
	FeatureNode(){
		id = -1;
		name = "";
		before = NULL;
		next = NULL;
		children = new List<FeatureNode>;
	};
	FeatureNode(int anID, CString aName, float aWeight, float aValue1, float aValue2){
		id = anID;
		name = aName;
		weight = aWeight;
		value1 = aValue1;
		value2 = aValue2;
		before = NULL;
		next = NULL;
		children = new List<FeatureNode>;
	};
	FeatureNode(int anID, bool aUseOrNot, CString aName, float aWeight, CString aFunctionType,
				float aValue1, float aWidth1, float aK1, float anR1, 
				float aValue2, float aWidth2, float aK2, float anR2){
		id = anID;
		useOrNot = aUseOrNot;
		name = aName;
		weight = aWeight;
		functionType = aFunctionType;
		value1 = aValue1;
		width1 = aWidth1;
		k1 = aK1;
		r1 = anR1;
		value2 = aValue2;
		width2 = aWidth2;
		k2 = aK2;
		r2 = anR2;
		before = NULL;
		next = NULL;
		children = new List<FeatureNode>;
	};

	FeatureNode(const FeatureNode& aFeatureNode){
		id = aFeatureNode.id;
		name = aFeatureNode.name;
		weight = aFeatureNode.weight;
		value1 = aFeatureNode.value1;
		value2 = aFeatureNode.value2;

		useOrNot = aFeatureNode.useOrNot;
		functionType = aFeatureNode.functionType;
		width1 = aFeatureNode.width1;
		r1 = aFeatureNode.r1;
		k1 = aFeatureNode.k1;
		width2 = aFeatureNode.width2;
		r2 = aFeatureNode.r2;
		k2 = aFeatureNode.k2;

		before = NULL;
		next = NULL;
		children = new List<FeatureNode>;
	};
	~FeatureNode(){
		children->release();
		delete children;
		before = NULL;
		next = NULL;
	};
};

class xPolyline{ 
public:
	int theID;
	double Xmin;
	double Ymin;
	double Xmax;
	double Ymax;
	double labelX;
	double labelY;
	List<IDDNode>* vertexList;
	xPolyline* before;
	xPolyline* next;
	List<xPolyline>* children;

	xPolyline(int anID){
		theID = anID;
		Xmin = 99999999;
		Ymin = 99999999;
		Xmax = -99999999;
		Ymax = -99999999;
		labelX = NODOUBLEVALUE;
		labelY = NODOUBLEVALUE;
		vertexList = new List<IDDNode>;
		children = new List<xPolyline>;
		before = NULL;
		next = NULL;
	};
	xPolyline(const xPolyline& aPolyline){
		theID = aPolyline.theID;
		Xmin = aPolyline.Xmin;
		Ymin = aPolyline.Ymin;
		Xmax = aPolyline.Xmax;
		Ymax = aPolyline.Ymax;
		labelX = aPolyline.labelX;
		labelY = aPolyline.labelY;
		vertexList = new List<IDDNode>(*(aPolyline.vertexList));
		children = new List<xPolyline>;
		before = NULL;
		next = NULL;
	};
	~xPolyline(){
		theID = -1;
		Xmin = 99999999;
		Ymin = 99999999;
		Xmax = -99999999;
		Ymax = -99999999;
		labelX = NODOUBLEVALUE;
		labelY = NODOUBLEVALUE;
		children->release();
		delete children;
		delete vertexList;
		before = NULL;
		next = NULL;
	};
	void getBound(){
		IDDNode* tempP = vertexList->first;
		while(tempP != NULL){
			if(tempP->d1 < Xmin) Xmin = tempP->d1;
			if(tempP->d2 < Ymin) Ymin = tempP->d2;
			if(tempP->d1 > Xmax) Xmax = tempP->d1;
			if(tempP->d2 > Ymax) Ymax = tempP->d2;
			tempP = tempP->next;
		}
	};
};

class xPolygon{ //can be used to construct a polygon list, used in diversity program for island polygons
				//the last vertex must be the same as the first one
public:
	int theID;
	double Xmin;
	double Ymin;
	double Xmax;
	double Ymax;
	double labelX;
	double labelY;
	List<IDDNode>* vertexList;
	xPolygon* before;
	xPolygon* next;
	List<xPolygon>* children;

	xPolygon(int anID){
		theID = anID;
		Xmin = 99999999;
		Ymin = 99999999;
		Xmax = -99999999;
		Ymax = -99999999;
		labelX = NODOUBLEVALUE;
		labelY = NODOUBLEVALUE;
		vertexList = new List<IDDNode>;
		children = new List<xPolygon>;
		before = NULL;
		next = NULL;
	};
	xPolygon(const xPolygon& aPolygon){
		theID = aPolygon.theID;
		Xmin = aPolygon.Xmin;
		Ymin = aPolygon.Ymin;
		Xmax = aPolygon.Xmax;
		Ymax = aPolygon.Ymax;
		labelX = NODOUBLEVALUE;
		labelY = NODOUBLEVALUE;
		vertexList = new List<IDDNode>(*(aPolygon.vertexList));
		children = new List<xPolygon>;
		before = NULL;
		next = NULL;
	};
	~xPolygon(){
		theID = -1;
		Xmin = 99999999;
		Ymin = 99999999;
		Xmax = -99999999;
		Ymax = -99999999;
		labelX = NODOUBLEVALUE;
		labelY = NODOUBLEVALUE;
		children->release();
		delete children;
		delete vertexList;
		before = NULL;
		next = NULL;
	};
	void getBound(){
		IDDNode* tempP = vertexList->first;
		while(tempP != NULL){
			if(tempP->d1 < Xmin) Xmin = tempP->d1;
			if(tempP->d2 < Ymin) Ymin = tempP->d2;
			if(tempP->d1 > Xmax) Xmax = tempP->d1;
			if(tempP->d2 > Ymax) Ymax = tempP->d2;
			tempP = tempP->next;
		}
	};
};


/******************************
 *	Other Utilities
 ******************************/

//find out the mode in a neighborhood
float neighborhoodMajority(float* dataBuf, 
					   int nrows,
					   int ncols,
					   int row, 
					   int col, 
					   int neighborhood,
					   float nodata);

int intersectPnt(double x1, double y1, double x2, double y2, 
				  double x3, double y3, double x4, double y4);

int pointInSimplePoly(xPolygon* aPolygon, IDDNode* aPoint); //not consider islands, used by pointInPolygon
															//-1: outside polygon
															//0: right on boundary
															//1: inside polygon
int pointInPolygon(xPolygon* aPolygon, List<xPolygon>* islands, IDDNode* aPoint); //true: inside; false: outside


/******************************
 *	Utilities for File I/O
 ******************************/
//find out number of lines in a file
int numOfRecords(char* FileName); 

//read files in ArcInfo Generate format.  The selList is for reading in selected features
int readLabFile(CString filename, List<IINode>* list, List<IntNode>* selList);
int readPntFile(CString filename, List<IDDNode>* list, List<IntNode>* selList);
//value point: id x y z
int readValuePointFile(List<IDDDNode>*	pntList, CString VPntFileName, List<IntNode>* selList);
int readLineFile(CString filename, List<xPolyline>* list, List<IntNode>* selList);
int readPolyFile(CString filename, List<xPolygon>* list, List<IntNode>* selList);


double areaSimplePoly(xPolygon *aPoly, bool absolute);
IDDNode* centroidSimplePoly( xPolygon *aPoly );


/******************************
 *	Utilities for Raster Handling
 ******************************/

//---------------------------------------------------------------------------
void HistogramEqualize(int *sourceBuf, int *targetBuf, int width, int height);


std::vector<int> GetHistogram(int *dataBuf, int width, int height);


std::vector<int> GetCumulativeHistogram(std::vector<int>& histogram);


#endif
    



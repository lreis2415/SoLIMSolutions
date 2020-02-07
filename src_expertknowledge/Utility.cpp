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
#include "Utility.h"

// Some utility functions copied from the old SoLIM


/* rings the bell */

void Beep()
{
 putchar ('\7');
 fflush (stdout);
}

 /* put three pieces into a filename */

char *cat_filename(char *dir, char *slash, char *name)
{
 char *tmpstr;

 if ((tmpstr=(char *)malloc((strlen(dir)+strlen(slash)+strlen(name))
          *sizeof(char)))==(char *)NULL)
    {
    fprintf(stderr, "\nOut of memory in cat_filename, aborting ...\n");
    exit(-1);
    } 
 
 strcpy(tmpstr, dir);
 strcat(tmpstr, slash);
 strcat(tmpstr, name);

 return(tmpstr);
}

// calculate the basic stats (min, max, mean, stdDev) of a floating point array

void calcBasicStats(float array[], int theSize, int noDataValue, float *min, float *max, float *mean, float *stdDev){
    int k = 0;
	float V = 0.0;
	
	*mean = 0.0;

	for (int i=0; i<theSize; i++) {
		if ((int)array[i]!= noDataValue){
			(*mean) +=array[i];
			k++;
			if (k==1) {
				*min=array[i];
				*max=array[i];
			}
			else {
				if (*min>array[i])
					*min=array[i];
				if (*max<array[i])
					*max=array[i];
			}// if (k)
		} // if (int)
	} // for i

  (*mean) = (*mean)/(float) k;
  
  for (int j=0; j<theSize; j++) {
	  if ((int)array[i]!= noDataValue){
		  V+=(array[j]-(*mean))*(array[j]-(*mean));
	  } // if
  } // for
  *stdDev = (float) sqrt (V/ (float) k);

  return;
}


/* extract filename from a file specification */

char *extract_filename(char *filespec)
{
 int len=0, j=0;
 char *tmpstr;

 len=strlen(filespec);
 if ((tmpstr=(char *)malloc(sizeof(char)*len))==NULL)
    {
    printf("\n\tOut of memory in extract_filename, program aborted");
    exit(-1);
    }
 filespec[len]='\0';
 strcpy(tmpstr, filespec);
 for (j=len-1; j>=0; j--)
    {
    if (filespec[j]=='/' || filespec[j]=='\\' || filespec[j]==':')
       {
       j++;
       break;
       }
    }
 if (j<0)
    j=0;
 strncpynm(tmpstr, filespec, j, len-1);
 return(tmpstr);

}




//---------------------------------------------------------------------------------------------------
/* a function to split a record into pieces */
int split_rec(char *piece, char *record, int start)
{
	int i=0, j=0;
	piece[j]=record[start];
	for (i=start+1; record[i]!='\0'; i++){
        if ((piece[j]!=' '&&piece[j]!='\t') && (record[i]==' '||record[i]=='\t'))
		   break;  //the first part is not to break when multiple white spaces at front
		else
			piece[++j]=record[i];
    }
	piece[++j]='\0';

	trim(piece,piece);

	if (record[i]=='\0')
		return(-1);
	return(i);
}

//---------------------------------------------------------------------------------------------------
//return the number of pieces in a record, it uses the split_rec function
int countPieces(char *record) 
{
	int numPieces = 0, start=0;
	char part[2*FN_LEN];
	while (start!=(-1)){
	    start=split_rec(part, record, start);
		if (strlen(trim(part, part))>0)
			numPieces++;
    }
	return numPieces;
}
//---------------------------------------------------------------------------------------------------
//retrieve the pieces from a record, return as a PARTS array
PARTS *retrievePieces(char *line, PARTS *pieces, int numPieces)
{
	int start=0,i=0;
	char part[2*FN_LEN];
	while (start!=(-1)){
	    start=split_rec(part, line, start);
		if (strlen(trim(part, part))>0){
			strcpy(pieces[i++].piece, part);
		}
    }
	return pieces;
}


/* wait for user to press <enter> and returns the
   first character in the buffer */

char get_enter()
{
 char ch[2*FN_LEN];

 printf("\n\n\tHit <enter> to continue, or enter <q> to quit ");
 gets(ch);
 if (tolower(ch[0])=='q')
    exit(1);
 return(ch[0]);
}

/*  find the most frequent number (the mode number) within a neighbourhood.
  if there is a tie, the first one encountered used 
  this is for the floating point array 

   the most frequent category (number) is returned via *cat */

int get_mode_float(float *cat, float *img, int nrow, int ncol, int i, int j, int ksize, int skip, float skipval) {
 int hsize, i1, i2, j1, j2;
 int *freq;
 int k, l, maxfreq, flag=0;
 float maxmode, *mode;

 if (i<0 || i>(nrow-1) || j<0 || j>(ncol-1)) {
	 *cat = skipval;
	 return(-1);
 }

 if (ksize==1) {	 
	 *cat = img[i*ncol+j];
	 return (1);
 }

 mode=(float *) malloc(ksize*ksize*sizeof(float));
 freq=(int *) malloc(ksize*ksize*sizeof(int));

 hsize=(int)((ksize-1)/2);

 i1=i-hsize;
 if (i1<0)
    i1=0;
 i2=i+hsize;
 if (i2>(nrow-1))
    i2=nrow-1;
 j1=j-hsize;
 if (j1<0)
    j1=0;
 j2=j+hsize;
 if (j2>(ncol-1))
    j2=ncol-1;

 k=0;
 for (i=i1; i<=i2; i++) {
	 for (j=j1; j<=j2; j++) {
		 if (skip==1) {
			 if (fabs(skipval-img[i*ncol+j])<VERYSMALL) {
				 continue;
			 }
		 }
         flag=0;
		 // first ever element 
         if (k==0) {
			 mode[k]= img[i*ncol+j];
			 freq[k++]=1;
		 }
		 // subsequent elements
         else {
			 for (l=0; l<k; l++) {
				 // if element already in, increase the frequency
				 if (mode[l]==img[i*ncol+j]) {
					 flag=1;
					 freq[l]++;
					 break;
				 }
			 }
			 // if element does not exist, add the element
             if (flag==0) {
				 mode[k]= img[i*ncol+j];
				 freq[k++]=1;
			 }
		 }
	 }
 }

 // locating the most frequent category
 if (k==0) {
	 fprintf(stdout, "\n\tNo other categories were found");
	 fprintf(stdout, "\n\texcept %f, which is the skipped category", skipval);
	 return(-1);
 }

 maxfreq=freq[0];
 maxmode=mode[0];
 for (i=1; i<k; i++) {
	 if (maxfreq<freq[i]) {
		 maxfreq=freq[i];
		 maxmode=mode[i];
	 }
 }

 *cat = maxmode;
 return(1);
}


/*  find the mean  within a neighbourhood.
  if there is a tie, the first one encountered used 
  this is for the floating point array 

   the mean is returned via *cat */

int get_mean_float(float *cat, float *img, int nrow, int ncol, int i, int j, int ksize, int skip, float skipval) {
 int hsize, i1, i2, j1, j2;

 int k;


 if (i<0 || i>(nrow-1) || j<0 || j>(ncol-1)) {
	 *cat = skipval;
	 return(-1);
 }

 if (ksize==1) {	 
	 *cat = img[i*ncol+j];
	 return (1);
 }



 hsize=(int)((ksize-1)/2);

 i1=i-hsize;
 if (i1<0)
    i1=0;
 i2=i+hsize;
 if (i2>(nrow-1))
    i2=nrow-1;
 j1=j-hsize;
 if (j1<0)
    j1=0;
 j2=j+hsize;
 if (j2>(ncol-1))
    j2=ncol-1;

 k=0;

 float sum = 0;

 int num = 0;

 for (i=i1; i<=i2; i++) {
	 for (j=j1; j<=j2; j++) {
		 
			 if (fabs(skipval - img[i*ncol+j])<VERYSMALL) {
				 continue;
			 }
		
		 sum += img[i*ncol+j];
		 num ++;


	}
 }
 
 if(num==0)
	  *cat = skipval;
 else 
     *cat = sum/num;
 return(1);
}




/* initialize an AXZ_float buffer to init_val */

void init_buffer_AXZ_float(float *buffer, int nrow, int ncol, float init_val)
{
 int i, j;

 for (i=0; i<nrow; i++)
    for (j=0; j<ncol; j++)
       buffer[i*ncol+j]=init_val;

}

/* initialize an integer buffer to init_val */

void init_buffer_int(int *buffer, int nrow, int ncol, int init_val)
{
 int i, j;

 for (i=0; i<nrow; i++)
    for (j=0; j<ncol; j++)
       buffer[i*ncol+j]=init_val;

}

/* output an AXZ_float image to a file */

int output_AXZ_float(char *filename, float *buffer, int nrow, int ncol, char *err_msg)
{
 FILE *fp;

 if ((fp=fopen(filename, "wb"))==NULL)
    {
    Beep();
    printf("\n\tCan't open \"%s\" for output", filename);
    fflush(stdout);
    sprintf(err_msg,"@center(Can't open)\n@center(@bold(\"%s\"))\n", filename);
    strcat(err_msg, "@center(for output)\n");
    strcat(err_msg, "@center(in function @bold(output_AXZ_float))\n");
    return(-1);
    } 
 fwrite(buffer, sizeof(float), nrow*ncol, fp);
 fclose(fp);

return(1);
}

/* read a record upto a new line or eof
   return -1 if eof
   otherwise 0 */

int read_record(FILE *fp, int *len, char *record)
{
 int  j;
 char ch;
 
 if (fp==NULL)
    {
    len=0;
    return(-1);
    }

 for (j=0;((ch=getc(fp))!='\n' && !feof(fp)); j++)
     {
     record[j]=ch;
     }
 record[j]='\0';
 *len=strlen(record);
 if (feof(fp))
    return(-1);
 return(0);
}

/* performs shell sorting on two correlated integer arrays in acending order, 
   returns the number of swaps */

int shellsort_2arr_int(int *arr1, int *arr2, int num)
{
 int g=0, m=0, f=2, j=0, s=0, tmp=0, p=0;

 g=num;

/* 
 printf("\n\nBefore sorting");
 for (j=0; j<num; j++)
     printf("\n\t%d %d %d", j, arr1[j], arr2[j]);
 */

 while(g>=1)
    {
    g=g/2;
    m=num-g;
    f=2;
    while (f>0)
       {
       f=0;
       for (j=0;j<m;j++)
          {
          p=j+g;
          if (arr1[j]>arr1[p])
             {
             tmp=arr1[j];
             arr1[j]=arr1[p];
             arr1[p]=tmp;
             tmp=arr2[j];
             arr2[j]=arr2[p];
             arr2[p]=tmp;
             s=s+1;
             f++;
             }
          }
       }
    }

 /* 
 printf("\n\nAfter sorting");
 for (j=0; j<num; j++)
     printf("\n\t%d %d %d", j, arr1[j], arr2[j]);

 printf("\n\tTotal swaps: %d", s);
 getchar();
 */

 return(s);
}

/* performs shell sorting on two float matrix in acending or decending order,
   returns the number of swaps
   order =  1 for acending
   order = -1 for decending */

int shellsort_2arr_float(float *matrix, float *second, int num, int order)
{
 int g=0, m=0, f=2, j=0, s=0, p=0;
 float tmp=0.0;

 g=num;
/*
 printf("\n\nBefore sorting");
 for (j=0; j<num; j++)
     printf("\n\t%d first: %f second: %f", j, matrix[j], second[j]);
*/
 while(g>=1)
    {
    g=g/2;
    m=num-g;
    f=2;
    while (f>0)
       {
       f=0;
       for (j=0;j<m;j++)
       for (j=0;j<m;j++)
          {
          p=j+g;
          if (order==1 && matrix[j]>matrix[p])
             {
             tmp=matrix[j];
             matrix[j]=matrix[p];
             matrix[p]=tmp;
             tmp=second[j];
             second[j]=second[p];
             second[p]=tmp;
             s=s+1;
             f++;
             }
          else if (order==(-1) && matrix[j]<matrix[p])
             {
             tmp=matrix[j];
             matrix[j]=matrix[p];
             matrix[p]=tmp;
             tmp=second[j];
             second[j]=second[p];
             second[p]=tmp;
             s=s+1;
             f++;
             }
          }
       }
    }
/*
 printf("\n\nAfter sorting");
 for (j=0; j<num; j++)
     printf("\n\t%d first: %f second: %f", j, matrix[j], second[j]);
 printf("\n\tTotal swaps: %d", s);
 getchar();
*/
 return(s);
}


/* determines number of records in a file
   returns -1 if file error
   otherwise returns the number of records (rows)
   in the file
   */

int count_records(char *filename) {
 FILE *fp; 
 char record[FN_LEN];
 int numRecords=0, done=0, len=0;

 if ((fp=fopen(filename, "r"))==NULL) {
	 printf("\n\nCan't open \"%s\" for reading", filename);
	 return(-1);
 }

 while (!done) {
    if (read_record(fp, &len, record)==(-1)) {
		if (len==0 || strlen(trim(record, record))==0) {
			break;
		}
        else {
		   numRecords++;
		   done=1;
		}
	}
	else {
		numRecords++;
	}
//	printf("\n%s", record);
 }
// printf("\n");
 fclose(fp);
 return (numRecords);
}


/* copy charaters from n to m in s2 to s1 inclusive */
/*
char *strncpynm(char *s1, char *s2, int n, int m){
	int i, j=0;
	for (i=n;i<=m && s2[i]!='\0';i++){
		s1[j]=s2[i];
		j++;
	}
	s1[j]='\0';
	return(s1);
}*/

/* trim the leading and trailing spaces and tabs of a string (s2) */

/*
char *trim(char *s1, char *s2){
	int i=0, j=0, len=0;
	char *tmp;

	len=strlen(s2);
	if (len==0){
		strcpy(s1, s2);
		return(s1);
	}

	if ((tmp=(char *) malloc((len+1)*sizeof(char)))==NULL){
		printf("\n\tOut of memory, program aborted");
		exit(0);
    }

	for (i=0; (s2[i]==' ' || s2[i]=='\t') && s2[i]!='\0'; i++);
	for (j=strlen(s2)-1; (s2[j]==' ' || s2[j]=='\t') && j>=0; j--);
	strncpynm(tmp, s2, i, j);
	strcpy(s1, tmp);
	free(tmp);
	return(s1);
}
*/
//Some utility functions for computing statistics

/*float min(float a, float b){
	if(a<b) return a;
	else return b;
}

float max(float a, float b){
  if(a>b) return a;
  else return b;
}*/

/*
void dataMinMax(float* array, int theSize, float nodata, float minmax[]){
	minmax[0] = array[0]; //holds the dataMin
	minmax[1] = array[0]; //holds the dataMax

	for(int i=0; i<theSize; i++)

		if(fabs(array[i] -nodata) > 0.0001){
			if(minmax[0]>array[i])
				minmax[0] = array[i];
			if(minmax[1]<array[i])
				minmax[1] = array[i];
		}
}*/

//the mean

float dataMean(float array[], int theSize, int nodata){

	float mean = 0.0;
	int num = 0;
	for(int i=0; i<theSize; i++){
        if(array[i]!= nodata){
			mean+=array[i];
			num++;
		}
	}
	mean = mean/num;
	return mean;
}

//the standard diviation

float calcStdDev(float array[], int theSize, int nodata){

	float mean = 0.0;
	float V = 0.0;
	int num = 0;
	for(int i=0; i<theSize; i++){
		if(array[i]!=nodata){
			mean+=array[i];
			num++;
		}
	}

	mean = mean/num;
	
	for(int j=0; j<theSize; j++){
		if(array[j]!=nodata){
			V+=(array[j]-mean)*(array[j]-mean);
		}
	}

	return ((float) sqrt(V/num));
}


/******************************
 *	Utilities for Strings
 ******************************/
char* strncpynm(char *s1, char *s2, int n, int m){
	int i, j=0;
	for (i=n;i<=m && s2[i]!='\0';i++){
		s1[j]=s2[i];
		j++;
	}
	s1[j]='\0';
	return(s1);
}

// trim the leading and trailing spaces and tabs of a string (s2)
char* trim(char *s1, char *s2){
	int i=0, j=0;
	char *tmp;
	if (strlen(s2)==0){
		strcpy(s1, s2);
		return(s1);
	}
	if ((tmp=(char *) malloc((strlen(s2)+1)*sizeof(char)))==NULL){
		printf("\n\tOut of memory, program aborted");
		exit(0);
    }
	for (i=0; (s2[i]==' ' || s2[i]=='\t') && s2[i]!='\0'; i++);
	for (j=strlen(s2)-1; (s2[j]==' ' || s2[j]=='\t') && j>=0; j--);
	strncpynm(tmp, s2, i, j);
	strcpy(s1, tmp);
	free(tmp);
	return(s1);
}

CString extractFileName(CString s){
	s.MakeReverse();
	int c = s.Find('\\');
	if(c == -1){
		s.MakeReverse();
		return s;
	}
	CString t = s.Left(c);
	t.MakeReverse();
	return t;
}

CString extractPathName(CString s){
	s.MakeReverse();
	int c = s.Find('\\');
	if(c == -1) return "";
	s.MakeReverse();
	CString t = s.Left(s.GetLength()-c);
	return t;
}

CString dropFileSuffix(CString s){
	s.MakeReverse();
	int c = s.Find('\.');
	if(c == -1){
		s.MakeReverse();
		return s;
	}
	CString t = s.Right(s.GetLength()-c-1);
	t.MakeReverse();
	return t;
}

/******************************
 *	Utilities for Statistics
 ******************************/
double calMin(double a, double b){
	if(a<b) return a;
	else return b;
}

double calMax(double a, double b){
  if(a>b) return a;
  return b;
}

void dataMinMax(float* array, int theSize, float nodata, float minmax[]){
	if(array == NULL){
		minmax[0] = NOFLOATVALUE; //holds the dataMin
		minmax[1] = NOFLOATVALUE; //holds the dataMax
		return;
	}
	minmax[0] = -NOFLOATVALUE; //holds the dataMin
	minmax[1] = NOFLOATVALUE; //holds the dataMax
	for(int i=0; i<theSize; i++)
		if(fabs(array[i] - nodata) > VERYSMALL){
			if(minmax[0]>array[i])
				minmax[0] = array[i];
			if(minmax[1]<array[i])
				minmax[1] = array[i];
		}
}

//the mean
float dataMean(float array[], int theSize, float nodata){
	if(array == NULL)
		return nodata;

	float mean = 0.0;
	int num = 0;
	for(int i=0; i<theSize; i++){
        if(fabs(array[i] - nodata) > VERYSMALL){
			mean+=array[i];
			num++;
		}
	}
	return mean/num;
}

//the variance
float calcVariance(float array[], int theSize, float nodata){
	if(array == NULL)
		return nodata;

	float mean = dataMean(array, theSize, nodata);
	float V = 0.0;
	int num = 0;
	for(int j=0; j<theSize; j++){
		if(array[j]!=nodata){
			V+=(array[j]-mean)*(array[j]-mean);
			num++;
		}
	}
	return V/num;
}

//the standard diviation
float calcStdDev(float array[], int theSize, float nodata){
	if(array == NULL)
		return nodata;
	float V = calcVariance(array, theSize, nodata);
	return sqrt(V);
}

float mae(float* dataBuf1, float nodata1, float* dataBuf2, float nodata2, int numOfPixels){
	if(dataBuf1 == NULL)
		return -1;
	if(dataBuf2 == NULL)
		return -2;

	float m = 0.0;
	int num = 0;
	for(int i=0; i<numOfPixels; i++){
		if(dataBuf1[i]!=nodata1 && dataBuf2[i]!=nodata2){
			m += fabs(dataBuf1[i]-dataBuf2[i]);
			num++;
		}
	}
	return m/num;
}

float rmse(float* dataBuf1, float nodata1, float* dataBuf2, float nodata2, int numOfPixels){
	if(dataBuf1 == NULL)
		return -1;
	if(dataBuf2 == NULL)
		return -2;

	float rms = 0.0;
	int num = 0;
	for(int i=0; i<numOfPixels; i++){
		if(dataBuf1[i]!=nodata1 && dataBuf2[i]!=nodata2){
			rms += pow(fabs(dataBuf1[i]-dataBuf2[i]), 2);
			num++;
		}
	}
	return sqrt(rms/num);
}

float correlationCoefficent(float* dataBuf1, float nodata1, float* dataBuf2, float nodata2, int numOfPixels){
	double sumX = 0;
	double sumSX = 0; //squared x
	double sumY = 0;
	double sumSY = 0; //squared y
	double sumXY = 0;
	int num = 0;
	for(int i = 0; i<numOfPixels; i++){
		if(dataBuf1[i]!=nodata1 && dataBuf2[i]!=nodata2){
			sumX += dataBuf1[i];
			sumSX += dataBuf1[i]*dataBuf1[i];
			sumY += dataBuf2[i];
			sumSY += dataBuf2[i]*dataBuf2[i];
			sumXY += dataBuf1[i]*dataBuf2[i];
			num++;
		}
	}
	return (num*sumXY - sumX*sumY)/sqrt(num*sumSX-sumX*sumX)/sqrt(num*sumSY-sumY*sumY);
}

float rSquare(float* dataBuf1, float nodata1, float* dataBuf2, float nodata2, int numOfPixels){
	double sumX = 0;
	double sumSX = 0; //squared x
	double sumY = 0;
	double sumSY = 0; //squared y
	double sumXY = 0;
	int num = 0;
	for(int i = 0; i<numOfPixels; i++){
		if(dataBuf1[i]!=nodata1 && dataBuf2[i]!=nodata2){
			sumX += dataBuf1[i];
			sumSX += dataBuf1[i]*dataBuf1[i];
			sumY += dataBuf2[i];
			sumSY += dataBuf2[i]*dataBuf2[i];
			sumXY += dataBuf1[i]*dataBuf2[i];
			num++;
		}
	}
	double ssxy = sumXY - sumX*sumY/num;
	return ssxy*ssxy/(sumSX-sumX*sumX/num)/(sumSY-sumY*sumY/num);
}

//Agreement Coefficient (AC)
float AC(float* dataBuf1, float nodata1, float* dataBuf2, float nodata2, int numOfPixels){
	float RMSE = rmse(dataBuf1, nodata1, dataBuf2, nodata2, numOfPixels);
	float Obar = dataMean(dataBuf1, numOfPixels, nodata1);
	float PE = 0;
	for(int i=0; i<numOfPixels; i++)
		if(dataBuf1[i]!=nodata1 && dataBuf2[i]!=nodata2)
			PE += pow(fabs(dataBuf1[i]-Obar)+fabs(dataBuf2[i]-Obar), 2);
	return 1-RMSE*RMSE*numOfPixels/PE; //note: numOfPixels include novalue points, which may cause error in the result
}

//compare paired data and return the p Value
//The difference is calculated as array2 - array1. The eMean should be compatible with this setting.
float pPaired(float* firstArray, float* secondArray, int size, float nodata, float eMean){
	float* diffArray = new float[size];
	float num = 0;
	for(int i=0; i<size; i++){
		if(firstArray[i]!=nodata && secondArray[i]!=nodata){
			diffArray[i] = secondArray[i] - firstArray[i];
			num++;
		}
		else
			diffArray[i] = nodata;
	}
	float mean = dataMean(diffArray, size, nodata);
	float std = calcStdDev(diffArray, size, nodata);
	//float t = (mean-eMean)/(std/sqrt(num));
	delete [] diffArray;
	return pNormalDistribution(mean, eMean, std, true);
}

//return p-Value for a given value in a normal distribution
float pNormalDistribution(float value, float mean, float std, bool singleSide){
	//e = 2.718281828459;
	//the equation: 0.398942*pow(2.718281828459, -0.5*z*z)
	//the table:
	//0.5 std: 0.382925
	//1 std: 0.682689
	//1.5 std: 0.866386
	//2 std: 0.9545
	//2.5 std: 0.987581
	//3 std: 0.9973
	//3.5 std: 0.999535
	//4 std: 0.999937

	//1. convert the value to the value in the standard normal distribution
	//   and to the positive side
	float absX = fabs((value-mean)/std);

	//2. find the interval
	float base = 0; //area under the curve
	float startX = 0; //value
	bool up = true; //use the start as the lower bound
	if(absX < 0.5){
		if(0.5-absX > absX){
			base = 0;
			startX = 0;
			up = true;
		}
		else{
			base = 0.382925;
			startX = 0.5;
			up = false;
		}
	}
	else if(absX < 1){
		if(1-absX > absX-0.5){
			base = 0.382925;
			startX = 0.5;
			up = true;
		}
		else{
			base = 0.682689;
			startX = 1;
			up = false;
		}
	}
	else if(absX < 1.5){
		if(1.5-absX > absX-1){
			base = 0.682689;
			startX = 1;
			up = true;
		}
		else{
			base = 0.866386;
			startX = 1.5;
			up = false;
		}
	}
	else if(absX < 2){
		if(2-absX > absX-1.5){
			base = 0.866386;
			startX = 1.5;
			up = true;
		}
		else{
			base = 0.9545;
			startX = 2;
			up = false;
		}
	}
	else if(absX < 2.5){
		if(2.5-absX > absX-2){
			base = 0.9545;
			startX = 2;
			up = true;
		}
		else{
			base = 0.987581;
			startX = 2.5;
			up = false;
		}
	}
	else if(absX < 3){
		if(3-absX > absX-2.5){
			base = 0.987581;
			startX = 2.5;
			up = true;
		}
		else{
			base = 0.9973;
			startX = 3;
			up = false;
		}
	}
	else if(absX < 3.5){
		if(3.5-absX > absX-3){
			base = 0.9973;
			startX = 3;
			up = true;
		}
		else{
			base = 0.999535;
			startX = 3.5;
			up = false;
		}
	}
	else if(absX < 4){
		if(4-absX > absX-3.5){
			base = 0.999535;
			startX = 3.5;
			up = true;
		}
		else{
			base = 0.999937;
			startX = 4;
			up = false;
		}
	}
	else
		return 0.00001;

	float curX = 0;
	float diff = 0;
	if(up){
		curX = startX + 0.000005;
		diff = absX - curX;
	}
	else{
		curX = startX - 0.000005;
		diff = curX - absX;
	}
	float tot = 0;
	while(diff>VERYSMALL){
		tot += 	0.00000398942*pow(2.718281828459, -0.5*curX*curX);
		if(up){
			curX += 0.00001;
			diff = absX - curX;
		}
		else{
			curX -= 0.00001;
			diff = curX - absX;
		}
	}

	if(up){
		if(singleSide)
			return 1-(base+tot);
		else
			return (1-(base+tot))*2;
	}
	else{
		if(singleSide)
			return 1-(base-tot);
		else
			return (1-(base-tot))*2;
	}
}


/****************************
Utilities used by SoLIM 5.0
**************************/
void split1(CString str,int x[])
{
	int len=str.GetLength();
	char *p=str.GetBuffer(len);
	int PrevPos=0,CurPos=0;
	int count=0;
	int countx=0;
	while(*p||count==len)
	{
		if(*p==','||*p == '\0')
		{
			CurPos=count;
			char q[100];
			for(int i=0;i<CurPos-PrevPos;i++)
			{
				q[i]=str.GetBuffer(len)[PrevPos+i];
			}
			q[i] = '\0';
			x[countx]=atof(q);
			countx++;			
			//x++;
			PrevPos=CurPos+1;
		}
		p++;
		count++;	
	}
	str.ReleaseBuffer();
}

void split1(CString str,double x[])
{
	int len=str.GetLength();
	char *p=str.GetBuffer(len);
	int PrevPos=0,CurPos=0;
	int count=0;
	int countx=0;
	while(*p||count==len)
	{
		if(*p==','||*p == '\0')
		{
			CurPos=count;
			char q[100];
			for(int i=0;i<CurPos-PrevPos;i++)
			{
				q[i]=str.GetBuffer(len)[PrevPos+i];
			}
			q[i] = '\0';
			x[countx]=atof(q);
			countx++;			
			//x++;
			PrevPos=CurPos+1;
		}
		p++;
		count++;	
	}
	str.ReleaseBuffer();
}

void split1(CString str,double x[],double y[])
{
	int len=str.GetLength();
	//char *p=str.GetBuffer(len);
	int PrevPos=0,CurPos=0;
	//int count=0;
	int count=0;

	for(int   i = 0;  i <=len; i++ )   
	{
		if(i<len)
		{
		char  c = str.GetAt(i);
		if(c==',')
		{
			CurPos=i;
			char q1[100];
			char q2[100];
			int MidPos=0;
			int n=0;
			while(str.GetAt(PrevPos+n)!=' ')
			{
				n++;
			}
			MidPos=PrevPos+n;
			for(int i=0;i<MidPos-PrevPos;i++)
			{
				q1[i]=str.GetAt(PrevPos+i);
			}
			q1[i] = '\0';
			for(int j=0;j<CurPos-MidPos;j++)
			{
				q2[j]=str.GetAt(MidPos+j);
			}
			q2[j] = '\0';
			x[count]=atof(q1);
			y[count]=atof(q2);             
			count++;	
			//county++;
			PrevPos=CurPos+1;
		}
		}
		else
		{
			CurPos=i;
			char q1[100];
			char q2[100];
			int MidPos=0;
			int n=0;
			while(str.GetAt(PrevPos+n)!=' ')
			{
				n++;
			}
			MidPos=PrevPos+n;
			for(int i=0;i<MidPos-PrevPos;i++)
			{
				q1[i]=str.GetAt(PrevPos+i);
			}
			q1[i] = '\0';
			for(int j=0;j<CurPos-MidPos;j++)
			{
				q2[j]=str.GetAt(MidPos+j);
			}
			q2[j] = '\0';
			x[count]=atof(q1);
			y[count]=atof(q2);             
			count++;	
			//county++;
			PrevPos=CurPos+1;
		}
	}
}



bool IsFloat(CString str)
{
	int   iLen   =   strlen(str);   
   bool  bResult   =  true;
   bool bHasRadixPoint = false;
    for(int   i = 0;  i <iLen; i++ )   
    {   
      char   c = str.GetAt(i);   
	  if(i==0 && c=='-')
			  continue;
	  else if(i==0 && c=='+')
			  continue;
      if ( !isdigit(c) )   
      {  
		  
		  if(c!='.')
		  {
             bResult = false;   
             break;   
		  }
	
		  else
		  {
			  if(bHasRadixPoint != false)
			  {
				  bResult = false;   
                  break;
			  }
			  else 
			  {
				  bHasRadixPoint = true;
			  }
		  }
      }   
    }
	return bResult;
}


bool IsInteger(CString str)
{
	int   iLen   =   strlen(str);   
   bool  bResult   =  true;
   bool bHasRadixPoint = false;
    for(int   i = 0;  i <iLen; i++ )   
    {   
      char   c = str.GetAt(i);   
	  if(i==0 && c=='-')
			  continue;
	  else if(i==0 && c=='+')
			  continue;
      if ( !isdigit(c) )   
      {  
		  
		 
             bResult = false;   
             break;   
		 
      }   
    }
	return bResult;
}




int FormatDouble(double dNumIn,int L) 
{ 
	
	int iNumIn;
	int L_int=1,L_double; 
	double dNumFmt;
	

	if(dNumIn >= 0)
	  iNumIn=floor(dNumIn);//the input number
	else
		iNumIn=-floor(fabs(dNumIn));

	int temp=iNumIn;
	while (temp/10!=0)
	{
		L_int++;
		temp=temp/10;
	}
	if (iNumIn<0)
	{
		L_int=L_int+1;
	
	}

	if (L_int<=L)
	{
		L_double=L-L_int;//小数位L_double
		dNumFmt=dNumIn+(double)(0.5*pow(10.0,-L_double)); //L_double－1位上四舍五入
	}
	else
	{
		L_double=0;
		dNumFmt=iNumIn;
	}



	return L_double;
}


int FormatFloat(float dNumIn,int L) 
{ 
	
	int iNumIn;
	int L_int=1,L_float; 
	float dNumFmt;
	

	if(dNumIn >= 0)
	  iNumIn=floor(dNumIn);//the input number
	else
		iNumIn=-floor(fabs(dNumIn));

	int temp=iNumIn;
	while (temp/10!=0)
	{
		L_int++;
		temp=temp/10;
	}
	if (iNumIn<0)
	{
		L_int=L_int+1;
		
	}

	if (L_int<=L)
	{
		L_float=L-L_int;//小数位L_float
		dNumFmt=dNumIn+(float)(0.5*pow(10.0,-L_float)); //L_double－1位上四舍五入
	}
	else
	{
		L_float=0;
		dNumFmt=iNumIn;
	}



	return L_float;
}



int IntegerDigits(float dNumIn) 
{ 
	
	int iNumIn;
	int L_int=1;

	

	if(dNumIn >= 0)
	  iNumIn=floor(dNumIn);//the input number
	else
		iNumIn=-floor(fabs(dNumIn));

	int temp=iNumIn;
	while (temp/10!=0)
	{
		L_int++;
		temp=temp/10;
	}
	if (iNumIn<0)
	{
		L_int=L_int+1;		
	}	

	return L_int;
}


//sets effective digits for a float number for output purpose
float effectiveDigits(float value){
	/*int k = 6;
	float temp = 0.0;
	for(int i = (-1)*k; i<k; i++)
	{
		if(value>=pow(10, i) && value<pow(10, i+1))
		{
			temp = (int)(value*pow(10, (i+1)*(-1)+numDigits)+0.5)/pow(10.0, (i+1)*(-1)+numDigits);
			return temp;
		}

			//return (int)(value*pow(10, (i+1)*(-1)+numDigits)+0.5)/pow(10.0, (i+1)*(-1)+numDigits);
	}
	return value;*/
   // int a = (int)(value*pow(10, numDigits)+0.5);
	//float temp = a /pow(10, numDigits);
//	return temp;

	CString str;
	str.Format("%.6f", value);
	float temp;
	temp = atof(str);
	return temp;




}





/************************************************
 *	Utilities for Testing and Getting Neightbors
 ************************************************/
bool assertNW(int pos, int nrow, int ncol){
	return(pos>(ncol-1) && pos%ncol > 0);
}

bool assertN(int pos, int nrow, int ncol){
	return(pos>(ncol-1));
}

bool assertNE(int pos, int nrow, int ncol){
	return(pos>(ncol-1) && (pos+1)%ncol > 0);
}

bool assertW(int pos, int nrow, int ncol){
	return(pos%ncol > 0);
}

bool assertE(int pos, int nrow, int ncol){
	return((pos+1)%ncol > 0);
}

bool assertSW(int pos, int nrow, int ncol){
	return((nrow*ncol-pos)>ncol && pos%ncol > 0);
}

bool assertS(int pos, int nrow, int ncol){
	return((nrow*ncol-pos)>ncol);
}

bool assertSE(int pos, int nrow, int ncol){
	return((nrow*ncol-pos)>ncol && (pos+1)%ncol > 0);
}

int getNW(int pos, int ncol){
	return(pos-ncol-1);
}

int getN(int pos, int ncol){
	return(pos-ncol);
}

int getNE(int pos, int ncol){
	return(pos-ncol+1);
}

int getW(int pos, int ncol){
	return(pos-1);
}

int getE(int pos, int ncol){
	return(pos+1);
}

int getSW(int pos, int ncol){
	return(pos+ncol-1);
}

int getS(int pos, int ncol){
	return(pos+ncol);
}

int getSE(int pos, int ncol){
	return(pos+ncol+1);
}


/******************************
 *Other	Utilities
 ******************************/
float neighborhoodMajority(float* dataBuf, 
					   int nrows,
					   int ncols,
					   int row, 
					   int col, 
					   int neighborhood,
					   float nodata){
	if(neighborhood<1)	return NOFLOATVALUE;
	else if(neighborhood==1) return dataBuf[row*ncols+col];
	else{
		int startRow = row - neighborhood/2;
		int startCol = col - neighborhood/2;

		List<ValueNode>* theValueList = new List<ValueNode>;
		for(int i=startRow; i<startRow+neighborhood; i++)
			for(int j=startCol; j<startCol+neighborhood; j++){
				if(theValueList->first == NULL)
					theValueList->addTail(new ValueNode(dataBuf[startRow*ncols+startCol], 1));
				else{
					bool flag = true;
					ValueNode* tempValue = theValueList->first;
					while(tempValue!=NULL){
						if(dataBuf[i*ncols+j]==tempValue->value){
							tempValue->num++;
							flag=false;
							break;
						}
						tempValue = tempValue->next;
					}
					if(flag)
						theValueList->addTail(new ValueNode(dataBuf[i*ncols+j], 1));
				}
			}

		float theMajority = nodata;
		int theMaxNum = 0;
		ValueNode* tempValue = theValueList->first;
		while(tempValue!=NULL){
			if(tempValue->num>theMaxNum && tempValue->value!=nodata){
				theMajority = tempValue->value;
				theMaxNum = tempValue->num;
			}
			tempValue = tempValue->next;
		}

		//clean up
		delete theValueList;
		
		return theMajority;
	}
}

int intersectPnt(double x1, double y1, double x2, double y2, 
				  double x3, double y3, double x4, double y4){
	if(fabs((y1-y2)*(x3-x4)-(x1-x2)*(y3-y4))<VERYSMALL) return -1; //parallel
	double x = ((x1-x2)*(x3*y4-x4*y3)-(x3-x4)*(x1*y2-x2*y1)) / ((y1-y2)*(x3-x4)-(x1-x2)*(y3-y4));
	double y = ((y1-y2)*(y3*x4-y4*x3)-(y3-y4)*(y1*x2-y2*x1)) / ((x1-x2)*(y3-y4)-(x3-x4)*(y1-y2));
	int conX1 = -2; //indicates the position of x on x1-x2
	int conX2 = -2; //indicates the position of x on x3-x4
	int conY1 = -2; //indicates the position of y on y1-y2
	int conY2 = -2; //indicates the position of y on y3-y4: 1: between two ends
					//										0: right on one end
					//										-1:outside the range
	//conX1
	if(fabs(x1-x2)<VERYSMALL) //a level line
		conX1 = 1;
	else if(fabs(x1-x)<VERYSMALL*VERYSMALL || fabs(x-x2)<VERYSMALL*VERYSMALL) //right on one end
		conX1 = 0;
	else if((x1-x)*(x-x2)>0) //between two ends
		conX1 = 1;
	else
		conX1 = -1;

	//conX2
	if(fabs(x3-x4)<VERYSMALL) //a level line
		conX2 = 1;
	else if(fabs(x3-x)<VERYSMALL*VERYSMALL || fabs(x-x4)<VERYSMALL*VERYSMALL) //right on one end
		conX2 = 0;
	else if((x3-x)*(x-x4)>0) //between two ends
		conX2 = 1;
	else
		conX2 = -1;

	//conY1
	if(fabs(y1-y2)<VERYSMALL) //a vertical line
		conY1 = 1;
	else if(fabs(y1-y)<VERYSMALL*VERYSMALL || fabs(y-y2)<VERYSMALL*VERYSMALL) //right on one end
		conY1 = 0;
	else if((y1-y)*(y-y2)>0) //between two ends
		conY1 = 1;
	else
		conY1 = -1;

	//conY2
	if(fabs(y3-y4)<VERYSMALL) //a vertical line
		conY2 = 1;
	else if(fabs(y3-y)<VERYSMALL*VERYSMALL || fabs(y-y4)<VERYSMALL*VERYSMALL) //right on one end
		conY2 = 0;
	else if((y3-y)*(y-y4)>0) //between two ends
		conY2 = 1;
	else
		conY2 = -1;

	if(conX1==-1 || conY1==-1 || conX2==-1 || conY2==-1)
		return 0; //not touch
	if((conX1==0 || conY1==0) && (conX2==0 || conY2==0))
		return 1; //end touches end
	if((conX1==0 || conY1==0) && conX2==1 && conY2==1)
		return 2; //end of the first touches the edge of the second
	if(conX1==1 && conY1==1 && (conX2==0 || conY2==0))
		return 3; //end of the second touches the edge of the first
	else
		return 4; //cross
}

int pointInPolygon(xPolygon* aPolygon, List<xPolygon>* islands, IDDNode* aPoint){
	if(islands == NULL)
		return pointInSimplePoly(aPolygon, aPoint);
	int v = pointInSimplePoly(aPolygon, aPoint);
	if(v<1)
		return v;
	else{
		xPolygon* curIsland = islands->first;
		while(curIsland != NULL){
			if(pointInSimplePoly(curIsland, aPoint)>0)
				return -1; //inside the island, so outside the polygon
			else
				curIsland = curIsland->next;
		}
	}
	return 1;
}

int pointInSimplePoly(xPolygon* aSimplePoly, IDDNode* aPoint){
	int ni = -1;
	double x2=aPoint->d1+888888888;
	IDDNode* tempP = aSimplePoly->vertexList->first;
	if(tempP == NULL){
		return -1;
	}
	while(tempP->next != NULL){
		int v = intersectPnt(aPoint->d1, aPoint->d2, x2, aPoint->d2+999999999, 
							tempP->d1, tempP->d2, tempP->next->d1, tempP->next->d2);
		if(v==3){ //avoid tangently touch
			x2 += 999999;
			ni = -1;
			tempP=aSimplePoly->vertexList->first;
			continue;
		}
		if(v==1 || v==2) return 0; //the point is right on the boundary
		if(v==4) ni=ni*(-1); //intersect
		tempP = tempP->next;
	}
	return ni;
}




/******************************
 *	Utilities for Reading Files
 ******************************/
int numOfRecords(char* FileName){
	ifstream fin;
	fin.open(FileName);
	if(fin.fail())	return 0;

	int numOfRecords = 0;
	while(!fin.eof()){
		char utilityString[STRLEN];
		fin.getline(utilityString, 100);
		if(utilityString[strlen(utilityString)-1]=='\n')
			utilityString[strlen(utilityString)-1]='\0';
		trim(utilityString, utilityString);
		if(strlen(utilityString)>0)
			numOfRecords++;
	}
	fin.close();
	return numOfRecords;
}

//read files in ArcInfo Generate format
int readLabFile(CString filename, List<IINode>* list, List<IntNode>* selList){
	ifstream fin;
	fin.open(filename);
	if(fin.fail()) return 1;
	int pntID = 0;
	int zoneID = 0;
	while(!fin.eof()){
		fin>>pntID>>zoneID;
		if(fin.eof() && pntID==list->last->i1 && zoneID==list->last->i2)
			break;
		bool selected = false;
		if(selList != NULL){
			IntNode* tempNode = selList->first;
			while(tempNode != NULL){
				if(tempNode->pos==pntID){
					selected = true;
					break;
				}
				tempNode = tempNode->next;
			}
		}
		else
			selected = true;
		if(selected)
			list->addTail(new IINode(pntID, zoneID));
	}
	fin.close();
	return 0;
}

int readPntFile(CString filename, List<IDDNode>* list, List<IntNode>* selList){
	ifstream fin;
	fin.open(filename);
	if(fin.fail()) return 1;
	char aLine[200];
	while(!fin.eof()){
		fin.getline(aLine,190);
		int id;
		double x;
		double y;
		sscanf(aLine, "%i %lf %lf", &id, &x, &y);

		bool selected = false;
		if(selList != NULL){
			IntNode* tempNode = selList->first;
			while(tempNode != NULL){
				if(tempNode->pos==id){
					selected = true;
					break;
				}
				tempNode = tempNode->next;
			}
		}
		else
			selected = true;

		if(selected && (list->last==NULL || list->last->i != id)) //avoid duplicate points
			list->addTail(new IDDNode(id, x, y));
	}
	fin.close();
	return 0;
}

//value point: id x y z
int readValuePointFile(List<IDDDNode>*	pntList, CString VPntFileName, List<IntNode>* selList){
	ifstream fin;
	fin.open(VPntFileName.GetBuffer(1));
	if(fin.fail()) return 1;

	char aLine[200];
	while(!fin.eof()){
		fin.getline(aLine,190);
		int id;
		double x;
		double y;
		double z;
		sscanf(aLine, "%i %lf %lf %lf", &id, &x, &y, &z);

		bool selected = false;
		if(selList != NULL){
			IntNode* tempNode = selList->first;
			while(tempNode != NULL){
				if(tempNode->pos==id){
					selected = true;
					break;
				}
				tempNode = tempNode->next;
			}
		}
		else
			selected = true;

		if(selected && (pntList->last==NULL || pntList->last->i != id)) //avoid duplicate points
			pntList->addTail(new IDDDNode(id, x, y, z));
	}
	fin.close();
	return 0;
}

int readLineFile(CString filename, List<xPolyline>* list, List<IntNode>* selList){
	return 0;
}

int readPolyFile(CString filename, List<xPolygon>* list, List<IntNode>* selList){
	ifstream zonefile;
	zonefile.open(filename);
	if(zonefile.fail()) return 1;

	CString tempString;
	bool fend = false; //file end

	//get the first zone id
	zonefile.getline(tempString.GetBuffer(220), 200);
	tempString.ReleaseBuffer(-1);
	tempString.TrimLeft();
	tempString.TrimRight();
	if(tempString == "END" || tempString == "End") return 2;
	int polyID = atoi(tempString.Left(tempString.FindOneOf(" ")));
	while(!zonefile.eof()){
		//Check if this zone has cases
		bool selected = false;
		IntNode* tempNode = NULL;
		if(selList!=NULL){
			tempNode = selList->first;
			while(tempNode!=NULL){
				if(tempNode->pos==polyID){
					selected = true;
					break;
				}
				tempNode = tempNode->next;
			}
		}
		else
			selected = true;

		//If has cases, add to the zoneList; otherwise, move to the next zone
		if(selected){ //this zone is selected
			//read in the polygon and islands (if any)
			xPolygon* curPoly = new xPolygon(polyID);
			List<xPolygon>* islands = new List<xPolygon>;
			tempString.Empty();
			zonefile.getline(tempString.GetBuffer(220), 200);
			tempString.ReleaseBuffer(-1);
			tempString.TrimLeft();
			tempString.TrimRight();
			int vid = 0;
			double x = 0;
			double y = 0;
			while(tempString != "END" && tempString != "End"){
				x = atof(tempString.Left(tempString.FindOneOf(" ")));
				y = atof(tempString.Right(tempString.GetLength()-tempString.FindOneOf(" ")));
				if(curPoly->vertexList->first==NULL||
					fabs(x-curPoly->vertexList->last->d1)>VERYSMALL||
					fabs(y-curPoly->vertexList->last->d2)>VERYSMALL)
					curPoly->vertexList->addTail(new IDDNode(vid, x, y));
				tempString.Empty();
				zonefile.getline(tempString.GetBuffer(220), 200);
				tempString.ReleaseBuffer(-1);
				tempString.TrimLeft();
				tempString.TrimRight();
				vid++;
			}
			if(!zonefile.eof()){  //read the ID for the next poly, but leave the vertices for the next loop
				zonefile.getline(tempString.GetBuffer(220), 200);
				tempString.ReleaseBuffer(-1);
				tempString.TrimLeft();
				tempString.TrimRight();
				if(tempString == "END" || tempString == "End")
					fend = true;
				if(!fend){
					if(tempString.FindOneOf(" ")>0)
						polyID = atoi(tempString.Left(tempString.FindOneOf(" ")));
					else
						polyID = atoi(tempString);
				}
			}

			while(!fend && polyID == -99999){ //island
				xPolygon* newIsland = new xPolygon(polyID);
				tempString.Empty();
				zonefile.getline(tempString.GetBuffer(220), 200);
				tempString.ReleaseBuffer(-1);
				tempString.TrimLeft();
				tempString.TrimRight();
				int vIsland = 0;
				double xIsland = 0;
				double yIsland = 0;
				while(tempString != "END" && tempString != "End"){
					xIsland = atof(tempString.Left(tempString.FindOneOf(" ")));
					yIsland = atof(tempString.Right(tempString.GetLength()-tempString.FindOneOf(" ")));
					if(newIsland->vertexList->first==NULL||
						fabs(x-newIsland->vertexList->last->d1)>VERYSMALL||
						fabs(y-newIsland->vertexList->last->d2)>VERYSMALL)
						newIsland->vertexList->addTail(new IDDNode(vIsland, xIsland, yIsland));
					tempString.Empty();
					zonefile.getline(tempString.GetBuffer(120), 100);
					tempString.ReleaseBuffer(-1);
					tempString.TrimLeft();
					tempString.TrimRight();
					vIsland++;
				}
				if(fabs(newIsland->vertexList->first->d1-newIsland->vertexList->last->d1)>VERYSMALL || 
					fabs(newIsland->vertexList->first->d2-newIsland->vertexList->last->d2)>VERYSMALL) //not closed
					delete newIsland;
				else
					islands->addTail(newIsland);
				if(!zonefile.eof()){
					zonefile.getline(tempString.GetBuffer(220), 200);
					tempString.ReleaseBuffer(-1);
					tempString.TrimLeft();
					tempString.TrimRight();
					if(tempString == "END" || tempString == "End")
						fend = true;
					if(!fend){
						if(tempString.FindOneOf(" ")>0)
							polyID = atoi(tempString.Left(tempString.FindOneOf(" ")));
						else
							polyID = atoi(tempString);
					}
				}
			}

			if(fabs(curPoly->vertexList->first->d1-curPoly->vertexList->last->d1)>VERYSMALL || 
				fabs(curPoly->vertexList->first->d2-curPoly->vertexList->last->d2)>VERYSMALL){ //check closure
				delete curPoly;
				delete islands;	
				if(fend)
					break;
				else
					continue;
			}

			//add the poly to zoneList
			list->addTail(curPoly);
			if(islands->first!=NULL){
				delete curPoly->children;
				curPoly->children = islands;
			}
			else
				delete islands;
			if(fend)
				break;
		}
		else{ //no cases in this zone. move to the next zone
			while(!zonefile.eof()){  //read the ID for the next poly, but leave the vertices for the next loop
				zonefile.getline(tempString.GetBuffer(220), 200);
				tempString.ReleaseBuffer(-1);
				tempString.TrimLeft();
				tempString.TrimRight();
				if(tempString == "END" || tempString == "End")
					break;
			}
			zonefile.getline(tempString.GetBuffer(220), 200);
			tempString.ReleaseBuffer(-1);
			tempString.TrimLeft();
			tempString.TrimRight();
			if(tempString == "END" || tempString == "End") //the last end
				break;
			if(tempString.FindOneOf(" ")>0)
				polyID = atoi(tempString.Left(tempString.FindOneOf(" ")));
			else
				polyID = atoi(tempString);

			while(!fend && polyID == -99999){ //island
				while(!zonefile.eof()){  //read the ID for the next poly, but leave the vertices for the next loop
					zonefile.getline(tempString.GetBuffer(220), 200);
					tempString.ReleaseBuffer(-1);
					tempString.TrimLeft();
					tempString.TrimRight();
					if(tempString == "END" || tempString == "End")
						break;
				}
				zonefile.getline(tempString.GetBuffer(220), 200);
				tempString.ReleaseBuffer(-1);
				tempString.TrimLeft();
				tempString.TrimRight();
				if(tempString == "END" || tempString == "End") //the last end
					fend = true;
				if(!fend){
					if(tempString.FindOneOf(" ")>0)
						polyID = atoi(tempString.Left(tempString.FindOneOf(" ")));
					else
						polyID = atoi(tempString);
				}
			}
			if(fend)
				break;
		}
	}
	zonefile.close();

	return 0;
}


/****************************************************************************/
/* areaSimplePoly							EFD added by Erika				*/
/*																			*/
/* Returns area of xPolygon object's vertexList								*/
/*	(ignores children and linked lists)										*/
/*  Absolute value returned for CW-ordered vertices (negative area)			*/
/****************************************************************************/
double areaSimplePoly(xPolygon *aPoly, bool absolute){ //The "absolute" determins if the absolute (positive) value should be returned. 
   int i,j;
   double area = 0;

   int n = aPoly->vertexList->numOfNodes;

   IDDNode *inode = aPoly->vertexList->first;
   IDDNode *jnode;

   for (i=0; i<n; i++) {
      j = (i + 1) % n;
	  if (j > 0) 
		  jnode = inode->next;
	  else if (j == 0)
		  jnode = aPoly->vertexList->first;

	  area += (inode->d1 * jnode->d2) - (inode->d2 * jnode->d1);
	  inode = inode->next;
   }

   area /= 2.0;
	if(absolute) return(area < 0 ? -area : area);
	else return area;
}

/****************************************************************************/
/* centroidSimplePoly						EFD	Added by Erika				*/
/*																			*/
/* Return geometric center of mass of xPolygon object's vertexList			*/
/*	(ignores children and linked lists)										*/
/****************************************************************************/
IDDNode* centroidSimplePoly( xPolygon *aPoly ){
	int i, j;
	double cx = 0.0;
	double cy = 0.0;
	double area = areaSimplePoly( aPoly, false );
	double factor;

	int n = aPoly->vertexList->numOfNodes;
	if (n == 0) return (IDDNode *) NULL;

    IDDNode *inode = aPoly->vertexList->first;
    IDDNode *jnode;

	for (i=0; i<n; i++) {
		j = (i + 1) % n;
	    if (j > 0) 
		    jnode = inode->next;
		else if (j == 0)
			jnode = aPoly->vertexList->first;

		factor = (inode->d1*jnode->d2 - jnode->d1*inode->d2);
		cx += (inode->d1 + jnode->d1)*factor;
		cy += (inode->d2 + jnode->d2)*factor;
		inode = inode->next;
	}

	return ( new IDDNode(0, cx/(6.0*area), cy/(6.0*area)) );
}


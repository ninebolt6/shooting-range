#include "stl.h"
#include <iostream>
#include <cstring>

void Stl::readStl(const char *fileName) {
	int i,j,k;
	int maxi = 100000;
	char buf[10000];
	char key[64],key2[64];;
	unsigned int width1, height1;
	float xx,yy,zz;
	float nxx,nyy,nzz;

	nxx=nyy=nzz=0.;
	tnum=0;

  FILE *fp1 = fopen(fileName, "r");
	fgets(buf,10000,fp1); // title //
	// printf(" buf = %s\n", buf);
	sscanf(buf,"%s",key);
	if (strcmp(key,"solid") == 0 ) {
		// printf("OK 1\n");
	} else {
		printf("NG 1: %s\n", key);
	}
	for(i=0;i<maxi;i++){
		fgets(buf,10000,fp1); // title //
		sscanf(buf,"%s",key);
		if (strcmp(key,"facet") == 0 ) {
//			printf("OK: %d\n",i);
		} else 	if (strcmp(key,"endsolid") == 0 ) {
			// printf("Normal termination: %s\n", key);
			break;
		} else {
			printf("NG: %d, %s\n",i, key);
			exit(-1);
		}
		fgets(buf,10000,fp1); // outer loop
		fgets(buf,10000,fp1); // point1
		fgets(buf,10000,fp1); // point2
		fgets(buf,10000,fp1); // point3
		fgets(buf,10000,fp1); // end loop

		fgets(buf,10000,fp1); // title //
		sscanf(buf,"%s",key);
		if (strcmp(key,"endfacet") == 0 ) {
//			printf("OK end \n");
		} else {
			printf("NG end: %s\n", key);
			exit(-1);
		}

	}
	tnum = i;
	// printf(" tnum = %d\n", tnum);
	x = (float *) malloc(3*tnum*sizeof(float));
	y = (float *) malloc(3*tnum*sizeof(float));
	z = (float *) malloc(3*tnum*sizeof(float));
	nx = (float *) malloc(tnum*sizeof(float));
	ny = (float *) malloc(tnum*sizeof(float));
	nz = (float *) malloc(tnum*sizeof(float));
	rewind(fp1);

	fgets(buf,10000,fp1); // title //
	// printf(" buf = %s\n", buf);
	sscanf(buf,"%s",key);
	if (strcmp(key,"solid") == 0 ) {
		// printf("OK 1\n");
	} else {
		printf("NG 1: %s\n", key);
	}
	for(i=0;i<tnum;i++){
		fgets(buf,10000,fp1); // title //
		sscanf(buf,"%s %s %f %f %f",key,key2,&nxx,&nyy,&nzz);
		if (strcmp(key,"facet") == 0 ) {
//			printf("OK: %d\n",i);
			nx[i] = nxx;
			ny[i] = nyy;
			nz[i] = nzz;
		} else 	if (strcmp(key,"endsolid") == 0 ) {
			// printf("Normal termination: %s\n", key);
			break;
		} else {
			printf("NG: %d, %s\n",i, key);
			exit(-1);
		}
		fgets(buf,10000,fp1); // outer loop
		fgets(buf,10000,fp1); // point1
		sscanf(buf,"%s %f %f %f",key,&xx,&yy,&zz);
		x[3*i+0] = xx;
		y[3*i+0] = yy;
		z[3*i+0] = zz;
		fgets(buf,10000,fp1); // point2
		sscanf(buf,"%s %f %f %f",key,&xx,&yy,&zz);
		x[3*i+1] = xx;
		y[3*i+1] = yy;
		z[3*i+1] = zz;
		fgets(buf,10000,fp1); // point3
		sscanf(buf,"%s %f %f %f",key,&xx,&yy,&zz);
		x[3*i+2] = xx;
		y[3*i+2] = yy;
		z[3*i+2] = zz;
		fgets(buf,10000,fp1); // end loop

		fgets(buf,10000,fp1); // title //
		sscanf(buf,"%s",key);
		if (strcmp(key,"endfacet") == 0 ) {
//			printf("OK STL data was stored \n");
		} else {
			printf("NG end: %s\n", key);
			exit(-1);
		}
	}
}

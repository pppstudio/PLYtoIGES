#include <stdio.h>

#include "wuPLY.h"


int main(){

	char *fname = "E:\\lab.ply";
	char *sname = "E:\\lab.iges";

	CwuPLY ply;
	ply.setDebug(true);

	ply.openPLY(fname);
	ply.saveIGES(sname);

	printf("Process Complete Please Any Key");
	getchar();

	return 0;
}
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "wuPLY.h"
#include "wuCommon.h"

// =============================================================================================================
//  定数宣言
// =============================================================================================================
const char *NameElementVertex = "element vertex ";
const char *Name_endheader = "end_header";
const int MaxLineLength = 100;
const int MaxIgesLineCount = 80;
const int MaxColumCount = 72;
const int PointCount	= 1;
const char Delim = ' ';


// =============================================================================================================
//  コンストラクタ
// =============================================================================================================
CwuPLY::CwuPLY(){

	// 色々変数の初期化
	vertex_count = 0;
	header_count = 0;
	global_count = 0;
	direct_count = 0;
	param_count = 0;

	vertex_count = 0;
	debug = false;

	view_ratio = 20;

	// IGES情報の初期化
	iges.DelimChar				= "1H,";
	iges.EndDelimChar			= "1H;";
	iges.Softname				= "Westunitis IGES Maker";
	iges.Filename				= "WU IGES";
	iges.SystemID				= "Westunitis - www.westunitis.co.jp";
	iges.AnsiStdVer				= "4HV5.0";
	iges.IntNumBits				= 32;
	iges.IndexNumBitsSingle		= 75;
	iges.MantissaNumBitsSingle	= 6;
	iges.IndexNumBitsDouble		= 75;
	iges.MantissaNumBitsDouble	= 15;
	iges.ReceiveSystemID		= "4HWust";
	iges.ScaleModelSpace		= 1;
	iges.UnitFlag				= 2;
	iges.Unit					= "2HMM";
	iges.MaxStep				= 1000;
	iges.MaxLineSize			= 1.0;
	iges.FileCreateDate			= "15H20140424.194105";
	iges.MinResolution			= 0.001;
	iges.MaxCoordinate			= 100;
	iges.AuthorName				= "2HShinMakinaga";
	iges.OrganizationName		= "8HWestunitis";
	iges.IgesVer				= 11;
	iges.ReferenceCode			= 7;
	iges.ModelCreateDate		= iges.FileCreateDate;

	// IGES Point 構造体の初期化
	igespoint.Element			= 116;		// 点とする
	igespoint.CardSeq			= 0;
	igespoint.IgesVer			= 0;
	igespoint.LineType			= 1;		// 実線とする
	igespoint.Level				= 1;
	igespoint.Tousyozu			= 0;
	igespoint.TransMatrix		= 0;
	igespoint.CouplingElement	= 0;
	igespoint.Status			= "00000000";
	igespoint.LineWidth			= 1;
	igespoint.PenType			= 1;
	igespoint.CardLength		= 0;
	igespoint.NumberFormat		= 1;
	igespoint.Reserve1			= "        ";
	igespoint.Reserve2			= "        ";
	igespoint.Label				= "Point   ";
	igespoint.SubScript			= 0;
	igespoint.Seq				= 0;
}


// =============================================================================================================
//  get set
// =============================================================================================================
int CwuPLY::getVertexCount(){
	return vertex_count;
}

void CwuPLY::setDebug(bool b){
	debug = b;
}


// =============================================================================================================
//  PLY Open
// =============================================================================================================
int CwuPLY::openPLY(char *readfilename){
	if(debug == true){ printf( "=============================================================================\n" ); }
	if(debug == true){ printf( "= PLY Open Process Start...                                                 =\n" ); }
	if(debug == true){ printf( "=============================================================================\n" ); }

	std::ifstream ifs(readfilename);

	// 要素数の取得
	int endheader = 0;
	while( ifs.peek() > -1 && endheader == 0 ){
		char buf[MaxLineLength];
		char *p;

		ifs.getline(buf, MaxLineLength);
		
		if((p = strstr(buf, NameElementVertex)) != NULL){
			vertex_count = atoi(buf + strlen(NameElementVertex));
		}
		if ((p = strstr(buf, Name_endheader)) != NULL ){
			endheader = 1;
        }
	}

	//if(debug == true){ vertex_count = 10020; }

	// 配列数決定
	ver = new vertex[vertex_count];

	// データ取得
	for(int i=0; i<vertex_count; i++){
		char buf[MaxLineLength];
		ifs.getline(buf, MaxLineLength);

		std::vector<std::string> spbuf = wuSplit(buf, Delim);
		ver[i].x = atof(spbuf[0].c_str())*100;
        ver[i].y = atof(spbuf[1].c_str())*100;
		ver[i].z = atof(spbuf[2].c_str())*100;
		ver[i].r = atoi(spbuf[3].c_str());
		ver[i].g = atoi(spbuf[4].c_str());
		ver[i].b = atoi(spbuf[5].c_str());

		ver[i].igesp = igespoint;
		ver[i].igesp.PenType = 1;		// 点の色決定

		if(debug == true){
			if( i % (vertex_count / view_ratio) == 0 ){
				printf("\r");
				printf("  PLY File Load: ");
				for(int loop=0; loop < view_ratio; loop++){
					if(loop <= (i / (vertex_count / view_ratio))){ printf("■"); }
					else{ printf("□"); }
				}
				if( view_ratio-1 < (i / (vertex_count / view_ratio)) ){ printf(" Complete!!\n"); }
			}
		}
	}

	if(debug == true){ printf( "  PLY Open Process All Complete!!\n" ); }
	if(debug == true){ printf( "=============================================================================\n" ); }

	ifs.close();
	return 0;
}


// =============================================================================================================
//  SAVE IGES
// =============================================================================================================
int CwuPLY::saveIGES(char *savefilename){
	if(debug == true){ printf( "=============================================================================\n" ); }
	if(debug == true){ printf( "= IGES Save Process Start...                                                =\n" ); }
	if(debug == true){ printf( "=============================================================================\n" ); }

	
	char bufGlobal[512];
	char bufDirEnt[512];
	char bufParam[512];
	char bufTarm[512];
	char writebuf[16];

	std::ofstream ofs(savefilename);

	// ヘッダーセクション
	// ---------------------------------------------------------------------------------------------------------
	ofs.write("Westunitis IGES Maker                                                   S0000001\n", 81);
	ofs.write("Copyright (C) WESTUNITIS CO.,LTD.                                       S0000002\n", 81);
	
	header_count = 2;
	

	// グローバルセクション
	// ---------------------------------------------------------------------------------------------------------
	sprintf(bufGlobal, "%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%s,%d,%d,%s,%d,%f,%s,%f,%f,%s,%s,%d,%d,%s;",
		iges.DelimChar, iges.EndDelimChar, iges.Softname, iges.Filename, iges.SystemID, iges.AnsiStdVer,
		iges.IntNumBits, iges.IndexNumBitsSingle, iges.MantissaNumBitsSingle, iges.IndexNumBitsDouble, iges.MantissaNumBitsDouble,
		iges.ReceiveSystemID, iges.ScaleModelSpace, iges.UnitFlag, iges.Unit, iges.MaxStep, iges.MaxLineSize,
		iges.FileCreateDate, iges.MinResolution, iges.MaxCoordinate, iges.AuthorName, iges.OrganizationName, iges.IgesVer,iges.ReferenceCode, iges.ModelCreateDate);

	for(int i=0; bufGlobal[i-1] != NULL; i++){
		bool countwrite = false;

		if(bufGlobal[i] != NULL){
			// データ書き込み
			ofs.write(bufGlobal+i, 1);
		} else {
			// 空白埋め
			if((i) % MaxColumCount != 0){
				for(int j=0; j < MaxColumCount - ((i) % MaxColumCount); j++){ ofs.write(" ", 1); }
				countwrite = true;
			}
		}

		// データ内容と行数の書き込み
		if(((i+1) % MaxColumCount == 0) || countwrite){ 
			global_count += 1;
			sprintf(writebuf, "G%7d\n", global_count);
			ofs.write(writebuf, 9);
		}
	}
	
	
	// ディレクトリエントリセクション
	// ---------------------------------------------------------------------------------------------------------

	int parametaSectionSeq = 0;

	// 点データの書き込み
	for(int i=0; i < vertex_count; i++){
		ver[i].igesp.CardSeq = parametaSectionSeq + 1;
		ver[i].igesp.CardLength = PointCount;
		ver[i].igesp.Seq = direct_count + 1;

		parametaSectionSeq += PointCount;

		// １行目書き込み
		sprintf(bufDirEnt, "%8d%8d%8d%8d%8d%8d%8d%8d%s",
			ver[i].igesp.Element, ver[i].igesp.CardSeq, ver[i].igesp.IgesVer, ver[i].igesp.LineType, ver[i].igesp.Level,
			ver[i].igesp.Tousyozu, ver[i].igesp.TransMatrix, ver[i].igesp.CouplingElement, ver[i].igesp.Status);
		ofs.write(bufDirEnt, MaxColumCount);

		// データ内容と行数の書き込み
		direct_count += 1;
		sprintf(writebuf, "D%7d\n", direct_count);
		ofs.write(writebuf, 9);

		// ２行目書き込み
		sprintf(bufDirEnt, "%8d%8d%8d%8d%8d%s%s%s%8d",
			ver[i].igesp.Element, ver[i].igesp.LineWidth, ver[i].igesp.PenType, ver[i].igesp.CardLength, ver[i].igesp.NumberFormat,
			ver[i].igesp.Reserve1, ver[i].igesp.Reserve2, ver[i].igesp.Label, ver[i].igesp.SubScript);
		ofs.write(bufDirEnt, MaxColumCount);

		// データ内容と行数の書き込み
		direct_count += 1;
		sprintf(writebuf, "D%7d\n", direct_count);
		ofs.write(writebuf, 9);

		if(debug == true){
			if( i % (vertex_count / view_ratio) == 0 ){
				printf("\r");
				printf("  IGES Write (Directory): ");
				for(int loop=0; loop < view_ratio; loop++){
					if(loop <= (i / (vertex_count / view_ratio))){ printf("■"); }
					else{ printf("□"); }
				}
				if( view_ratio-1 < (i / (vertex_count / view_ratio)) ){ printf(" Complete!!\n"); }
			}
		}
	}


	// パラメータデータセクション
	// ---------------------------------------------------------------------------------------------------------

	// 点データの書き込み
	for(int i=0; i < vertex_count; i++){
		sprintf(bufParam, "%d,%f,%f,%f;", ver[i].igesp.Element, ver[i].x, ver[i].y, ver[i].z);
		ofs.write(bufParam, strlen(bufParam));
		// 空白埋め
		for(int j=0; j < (MaxColumCount-7) - strlen(bufParam); j++){ ofs.write(" ", 1); }

		//ディレクトリセクションの場所
		sprintf(writebuf, "%7d\n", ver[i].igesp.Seq);
		ofs.write(writebuf, 7);

		// データ内容と行数の書き込み
		param_count += 1;
		sprintf(writebuf, "P%7d\n", param_count);
		ofs.write(writebuf, 9);

		if(debug == true){
			if( i % (vertex_count / view_ratio) == 0 ){
				printf("\r");
				printf("  IGES Write (Parameter): ");
				for(int loop=0; loop < view_ratio; loop++){
					if(loop <= (i / (vertex_count / view_ratio))){ printf("■"); }
					else{ printf("□"); }
				}
				if( view_ratio-1 < (i / (vertex_count / view_ratio)) ){ printf(" Complete!!\n"); }
			}
		}
	}


	// ターミネートセクション
	// ---------------------------------------------------------------------------------------------------------

	// 書き込み
	sprintf(bufTarm, "S%7dG%7dD%7dP%7d", header_count, global_count, direct_count, param_count);
	ofs.write(bufTarm, strlen(bufTarm));
	// 空白埋め
	for(int j=0; j < MaxColumCount - strlen(bufTarm); j++){ ofs.write(" ", 1); }
	// データ内容と行数の書き込み
	sprintf(writebuf, "T%7d\n", 1);
	ofs.write(writebuf, 9);

	if(debug == true){ printf( "  IGES Save Process All Complete!!\n" ); }
	if(debug == true){ printf( "=============================================================================\n" ); }

	ofs.close();

	return 0;
}
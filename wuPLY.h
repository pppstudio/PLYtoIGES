#ifndef WUPLY_H
#define WUPLY_H

// =============================================================================================================
//  クラス宣言
// =============================================================================================================
class CwuPLY{

public:
	
	// コンストラクタ
	CwuPLY();

	// 関数のプロトタイプ宣言
	int getVertexCount();
	void setDebug(bool b);
	int openPLY(char *readfilename);
	int saveIGES(char *savefilename);

private:

	// IGESファイルの構造体宣言
	struct IGES{
		char *DelimChar;			// デリミタ文字
		char *EndDelimChar;			// 終了デリミタ文字
		char *Softname;				// 送信システムの送信ＩＤ
		char *Filename;				// ファイル名
		char *SystemID;				// システムＩＤ
		char *AnsiStdVer;			// ANSI標準翻訳バージョン
		int IntNumBits;				// 整数ビット数
		int IndexNumBitsSingle;		// 単精度の指数ビット数
		int MantissaNumBitsSingle;	// 単精度の仮数ビット数
		int IndexNumBitsDouble;		// 倍精度の指数ビット数
		int MantissaNumBitsDouble;	// 倍精度の指数ビット数
		char *ReceiveSystemID;		// 受信システム製品ＩＤ
		int ScaleModelSpace;		// モデル空間のスケール
		int UnitFlag;				// 単位フラグ
		char *Unit;					// 単位
		int MaxStep;				// 線幅の刻み値の最大値
		double MaxLineSize;			// 最大線幅サイズ
		char *FileCreateDate;		// ファイル作成日時
		double MinResolution;		// ユーザの希望最小解像度
		double MaxCoordinate;		// 最大座標値
		char *AuthorName;			// 作成者名
		char *OrganizationName;		// 組織名
		int IgesVer;				// 参照IGES仕様バージョン
		int ReferenceCode;			// 参照製図規格コード
		char *ModelCreateDate;		// モデル最終作成変更日時
	};

	// IGESポイントデータの構造体宣言
	struct IGESPoint{
		int Element;				// 要素番号 （点は116）
		int CardSeq;				// 索引番号
		int IgesVer;				// IGESのバージョン
		int LineType;				// 線種（1:実線）
		int Level;					// レベル（レイヤ番号）
		int Tousyozu;				// 投象図（0固定）
		int TransMatrix;			// 変換行列（0固定）
		int CouplingElement;		// 結合要素
		char *Status;				// ステータス
		int LineWidth;				// 線幅
		int PenType;				// ペン番号（1:黒　2:赤　3:緑　4:青　5:黄　6:マゼンダ　7:シアン　8:白）
		int CardLength;				// カード数
		int NumberFormat;			// 形式番号
		char *Reserve1;				// 予備１
		char *Reserve2;				// 予備２
		char *Label;				// 要素ラベル
		int SubScript;				// サブスクリプト
		int Seq;					// ディレクトリエントリセクションのシーケンス
	};

	// 点群の構造体宣言
	struct vertex{
		double x;
		double y;
		double z;

		int r;
		int g;
		int b;

		IGESPoint igesp;
	};

	bool debug;				// デバッグモード
	int view_ratio;		// 進捗率の表示個数

	int vertex_count;		// 点群の数
	int header_count;		// ヘッダーセクションのカウント
	int global_count;		// グローバルセクションのカウント
	int direct_count;		// ディレクトリエントリセクションのカウント
	int param_count;		// パラメータデータセクションのカウント

	vertex *ver;			// 点群たちの構造体配列
	IGES iges;				// IGESに保存する時に使用する構造体
	IGESPoint igespoint;	// IGESのPointデータ初期値保持用
	
};



#endif
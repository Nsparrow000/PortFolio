//*****************************************************************************
// ボタン(Butten.h)
// Author : 佐藤秀亮
//*****************************************************************************
#ifndef _BUTTEN_H_
#define _BUTTEN_H_

#include "Scene2D.h"
class CKeyboard;
class CMouse;

//*****************************************************************************
//マクロ
//*****************************************************************************
#define MAX_BUTTENPATTERN 16
#define MAX_BUTTEN 64

class CButten :public CScene2D
{
public:


	//表示するボタン
	typedef enum
	{
		ALL,
		CHANGE,
		DISPLAY_MAX,
	} DISPLAY;

	//表示するボタン
	typedef enum
	{
		NONE,
		PATTERN0,
		PATTERN1,
		PATTERN2,
		PATTERN3,
		PATTERN4,
		PATTEN_MAX,
	} DISPLAY_PATTERN;

	//変更する物の指定
	typedef enum
	{
		PATTERN,
		COLSELECT,
		COL,
		ADDCOL,
		SIZE,
		ADDSIZE,
		MOVEX,
		MOVEY,
		ADDMOVEX,
		ADDMOVEY,
		LIFE,
		DENSITY,
		DIFFUSION,
		TEXTURE,
		ROTATE,
		SAVE,
		LOAD,
		DESTROY,
		TRAJECTTOP,
		TRAJECTTOPCOL,
		TRAJECTTOPADDCOL,
		TRAJECTCUR,
		MOVE3DX,
		MOVE3DY,
		MOVE3DZ,
		RANDMOVE3D,
		SYNTHETIC,
		PARTICLESIZE,
		PARTICLETIME,
		DISTANCE,
		ACTIVETIME,
		MAXSIZE,
		PARTICLEADDSIZE,
		PARTICLECOLOR,
		PARTICLEADDCOLOR,
		PARTICLESYNSETIC,
		ACTIVEADDSIZE,
		PRESETLOAD,
		SCONDTIME,
		VTX,
		TYPE,	//球か半球か等
		TEXU,	//テクスチャU
		TEXV,	//テクスチャV
		TEXNUM,	//テクスチャ枚数
		SECONDTYPE,
		TEXSPLITU,
		TEXSPLITV,
		ANIMCONT,
		TEXNUMV,
		HIGTH,
		CONTROLBEZIERX,	//ベジェ制御点
		CONTROLBEZIERY,	//ベジェ制御点
		CONTROLBEZIERZ,	//ベジェ制御点
		SECONDTEX,
		MAX,
	} OPERATION;

	//ボタンの情報
	typedef struct
	{
		D3DXVECTOR3 m_pos;	//座標
		float m_SizeX;	//サイズx
		float m_SizeY;	//サイズＹ
		int m_nType;	//テクスチャ
		OPERATION m_Operation;	//変更タイプ
		float m_nIndeNum;	//加算数
		DISPLAY_PATTERN Pattrn;	//表示パターン
	} BUTTEN_STATE;

	CButten(int nPriority);
	~CButten();

	HRESULT Init(D3DXVECTOR3 pos,
		float SizeX, float SizeY,
		int nType,
		OPERATION Operation,
		float nIndeNum,
		DISPLAY Display,
		DISPLAY_PATTERN Pattern);

	void Uninit();
	void Update();
	void Draw();

	void Set(OPERATION Operation, float nIndeNum);

	static CButten *Create(D3DXVECTOR3 pos,
		float SizeX, float SizeY,
		int nType,
		OPERATION Operation,
		float nIndeNum,
		DISPLAY Display,
		DISPLAY_PATTERN Pattern);

	//ボタンステータスの格納
	static void SetButtenState(D3DXVECTOR3 pos,
		float SizeX, float SizeY,
		int nType,
		OPERATION Operation,
		float nIndeNum,
		DISPLAY_PATTERN Pattern);

	static void SetButten(int nPatten);	//ボタン配置


	static void AddTotal(int nTotal) { m_nButtenTotal = nTotal;}	//パターンの変更
	static void SetTotal(int nPatten) { m_nTotal[nPatten] = m_nButtenTotal; }	//総数の格納
	static void ResetTotal() {m_nButtenTotal = 0;}
	static void SetPatten() { m_nPatten++; }
	static void ResetPatten() { m_nPatten = 0; }

private:
	static BUTTEN_STATE ButtenState[MAX_BUTTENPATTERN][MAX_BUTTEN];	//ボタンのステータス
	static int m_nPatten;	//個々のステータス
	static int m_nButtenTotal;	//そのパターンのボタン総数
	static int m_nTotal[MAX_BUTTENPATTERN];	//ループ用

	static bool m_MouseButtenPush;	//ボタン押してるか
	static int m_MousePushTime;	//押してる時間

	static bool m_PushDeley;	//ボタン押してるか
	static int m_DeleyTime;	//押してる時間


	CMouse *m_pMouse;
	CKeyboard *m_pKeyboard;

	static POINT m_po;					//マウス座標

	D3DXVECTOR2 m_Size;		//サイズ
	static D3DXVECTOR2 m_Mousepos;		//マウス座標

	bool m_bUninit;		//消す

	float m_fAlpha;	//アルファ値

	OPERATION m_aOperation;		//変更する項目
	DISPLAY aDisplay;	//表示するボタン
	DISPLAY_PATTERN aDisplayPattern;	//細かく
	int m_nNum;		//色
	float m_nIndeNum;		//増減
};
#endif
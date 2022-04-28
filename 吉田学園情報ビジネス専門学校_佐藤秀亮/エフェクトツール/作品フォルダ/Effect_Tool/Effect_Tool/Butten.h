//*****************************************************************************
// ボタン(Butten.h)
// Author : 佐藤秀亮
//*****************************************************************************
#ifndef _BUTTEN_H_
#define _BUTTEN_H_

#include "Scene2D.h"
class CKeyboard;
class CMouse;

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
		MAX,
	} OPERATION;

	CButten(int nPriority);
	~CButten();

	HRESULT Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, OPERATION Operation, float nIndeNum, DISPLAY Display, DISPLAY_PATTERN Pattern);
	void Uninit();
	void Update();
	void Draw();

	void Set(OPERATION Operation, int nCol, float nIndeNum);

	static CButten *Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, OPERATION Operation,float nIndeNum, DISPLAY Display, DISPLAY_PATTERN Pattern);
private:
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
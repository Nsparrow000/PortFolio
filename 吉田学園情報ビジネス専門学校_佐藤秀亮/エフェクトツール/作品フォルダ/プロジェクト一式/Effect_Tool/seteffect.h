//*****************************************************************************
// エフェクト設置処理(SetEffect.h)
// Author : 佐藤秀亮
//*****************************************************************************

#ifndef _SETEFFECT_H_
#define _SETEFFECT_H_

#include "Scene2D.h"
class CKeyboard;
class CMouse;

class CSetEffect :public CScene2D
{
public:

	CSetEffect(int nPriority);
	~CSetEffect();

	HRESULT Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType);
	void Uninit();
	void Update();
	void Draw();

	static void SetEffect(int nPatern);

	static D3DXVECTOR3 GetSetPos() { return m_pos; }

	static CSetEffect *Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType);
private:
	CMouse *m_pMouse;
	CKeyboard *m_pKeyboard;

	static POINT m_po;					//マウス座標

	static D3DXVECTOR3 m_pos;			//現在位置

	D3DCOLORVALUE m_Color;		//カラー
	bool m_bUninit;				//消すか

	//大きさ情報
	float m_fSizeX;
	float m_fSizeY;

	static float m_fSizeX2;
	static float m_fSizeY2;

	static bool m_bOnce;

	static int m_nSerectEffect;	//設置エフェクトプレビュー番号
};
#endif
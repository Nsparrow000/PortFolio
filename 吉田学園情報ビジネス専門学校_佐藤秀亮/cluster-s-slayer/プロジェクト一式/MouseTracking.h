//*****************************************************************************
// (目標に向かって飛んでく)エフェクト処理 [MouseTracking.h]
// Author : 佐藤秀亮
//*****************************************************************************
#ifndef _MOUSETRACKING_H_
#define _MOUSETRACKING_H_

#include "effect2d.h"

class CMouseTracking : public CEffect2d
{
public:
	CMouseTracking(OBJTYPE nPlyorty = CScene::OBJTYPE_EFFECT);
	~CMouseTracking();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size,
		D3DXVECTOR2 MinSize, int nLife, int nType,
		D3DXVECTOR3 Endpos, int Diffusion, int UninitVectl, int nSynthetic);

	void Uninit();
	void Update();
	void Draw();

	static CMouseTracking *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size,
		D3DXVECTOR2 MinSize, int nLife, int nType,
		D3DXVECTOR3 Endpos,int Diffusion, int UninitVectl,int nSynthetic);
private:
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_Vec;
	D3DXVECTOR3 m_Endpos;

	float m_fAngle;
	float m_Vectl;
	int m_UninitVectl;
};

#endif // !_MOUSETRACKING_H_

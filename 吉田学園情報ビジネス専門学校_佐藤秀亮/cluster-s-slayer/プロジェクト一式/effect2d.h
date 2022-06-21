//*****************************************************************************
// エフェクト処理 [Effect.h]
// Author : 佐藤秀亮
//*****************************************************************************
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "Scene2D.h"

class CEffect2d : public CScene2D
{
public:
	CEffect2d(OBJTYPE nPlyorty = CScene::OBJTYPE_EFFECT);
	virtual ~CEffect2d();

	virtual HRESULT Init(D3DXVECTOR3 pos, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size, D3DXVECTOR2 MinSize, int nLife, int nType, int Synthetic);
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();


protected:
	bool m_bUninit;	//削除
	int m_nLife;	//寿命
	D3DXVECTOR2 m_Size;			//大きさ
	D3DXVECTOR2 m_MinSize;		//大きさ変動

private:

	//カラー値
	D3DCOLORVALUE m_Color;

	//減らすカラー値
	D3DCOLORVALUE m_MinColor;

	D3DXVECTOR2 m_move;			//移動
	int m_nSynthenic;	//合成

};
#endif

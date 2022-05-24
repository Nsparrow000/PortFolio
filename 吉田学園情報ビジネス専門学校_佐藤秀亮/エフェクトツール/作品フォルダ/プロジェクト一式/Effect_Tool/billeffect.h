//=============================================================================
// 3Dエフェクト処理 [BillEffect.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _BILLEFFECT_H_
#define _BILLEFFECT_H_

#include "main.h"
#include "plane.h"

class CBillEffect : public CPlane
{
public:
	CBillEffect(int nPriority);
	virtual ~CBillEffect();

	virtual HRESULT Init(D3DXVECTOR3 Size, D3DXVECTOR3 MinSize, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, int nTex, int nLife, float TexNum);
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void SetTexture(int nTex);		//指定された番号のテクスチャを貼れるように

protected:
	bool m_bUninit;	//抹消フラグ
	int m_nSynthenic;	//合成
	D3DXVECTOR3 m_MinSize;		//大きさ変動値
	D3DXVECTOR3 m_Size;			//サイズ

	D3DXVECTOR2 m_TexMove;
	float m_fTexNum;

private:

	//カラー
	D3DCOLORVALUE m_Color;

	//カラー減衰
	D3DCOLORVALUE m_MinColor;

	int m_nLife;	//寿命
};

#endif
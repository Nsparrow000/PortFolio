//*****************************************************************************
// 回転エフェクトの親処理(Rotate3d.h)
// Author : 佐藤秀亮
//*****************************************************************************
#ifndef _ROTATE3D_H_
#define _ROTATE3D_H_

#include "plane.h"

class CRotate3D : public CPlane
{
public:
	typedef enum
	{
		START,
		ACTIVE,
		END,
		MAX,
	} EFFECT_TIME;


	CRotate3D(int nPriority);
	~CRotate3D();

	HRESULT Init(D3DXVECTOR3 SetSize,
		D3DXVECTOR3 pos,
		D3DXVECTOR2 Tex,
		D3DXVECTOR3 Size,
		D3DXVECTOR3 AddSize,
		D3DCOLORVALUE Color,
		D3DCOLORVALUE AddColor,
		float Distance,
		float AddDistance,
		float fAngle,
		float fAddAngle,
		int nTex,
		int Synthetic,
		int nLife,
		int nParticleLife,
		int nBuckTime,
		float fActive);

	void Uninit();
	void Update();

	static CRotate3D *Create(D3DXVECTOR3 SetSize,
		D3DXVECTOR3 pos,
		D3DXVECTOR2 Tex,
		D3DXVECTOR3 Size,
		D3DXVECTOR3 AddSize,
		D3DCOLORVALUE Color,
		D3DCOLORVALUE AddColor,
		float Distance,
		float AddDistance,
		float fAngle,
		float fAddAngle,
		int nTex,
		int Synthetic,
		int nLife,
		int nParticleLife,
		int nBuckTime,
		float m_fActive);

private:
	EFFECT_TIME m_EffectTime;

	bool m_bUninit;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_Size;
	D3DXVECTOR3 m_AddSize;
	D3DCOLORVALUE m_Color;
	D3DCOLORVALUE m_MinColor;

	float m_nDistanse;
	float m_nBuckDistanse;

	float m_nAddDistance;
	float m_fAngle;
	float m_fAddAngle;
	float m_fRandAngle;
	float m_fRandAngle2;
	int m_nBuckTime;	//距離が縮まるまでの時間
	float m_fActive;	//アクティブになるまでの距離

	int m_nTex;
	int m_nSynthetic;
	int m_nLife;
	int m_ParticleLife;

};

#endif
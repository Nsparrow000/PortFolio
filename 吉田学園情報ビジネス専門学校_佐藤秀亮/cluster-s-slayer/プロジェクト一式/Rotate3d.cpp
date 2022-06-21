//*****************************************************************************
// 回転エフェクトの親処理(Rotate3d.cpp)
// Author : 佐藤秀亮
//*****************************************************************************
#include "Rotate3d.h"
#include "manager.h"

#include "control.h"
#include "straight3d.h"

#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CRotate3D::CRotate3D(int nPriority) : CPlane(nPriority)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CRotate3D::~CRotate3D()
{

}

//*****************************************************************************
//初期化
//*****************************************************************************
HRESULT CRotate3D::Init(D3DXVECTOR3 SetSize,
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
	float fActive,
	POS_PATTERN PosPattern)
{
	CPlane::Init(0, nTex);
	CScene::SetObjType(OBJTYPE::OBJTYPE_EFFECT);

	m_EffectTime = START;
	m_pos = pos;
	m_Size = Size;
	m_AddSize = AddSize;
	m_Color = Color;
	m_MinColor = AddColor;
	m_nDistanse = Distance;
	m_nBuckDistanse = -Distance;
	m_nAddDistance = AddDistance;
	m_fAngle = fAngle;
	m_fAddAngle = fAddAngle;
	m_nTex = nTex;
	m_nSynthetic = Synthetic;
	m_nLife = nLife;
	m_ParticleLife = nParticleLife;
	m_nBuckTime = nBuckTime;
	m_fActive = fActive;
	m_PosPattern = PosPattern;

	m_fRandAngle = CIRCLE2;
	m_fRandAngle2 = CIRCLE2;

	SetPos(m_pos, m_Size);

	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CRotate3D::Uninit()
{
	CPlane::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CRotate3D::Update()
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 PlayerPos = CManager::GetGame()->GetPlayer()->GetPos();

	switch (m_EffectTime)
	{
	case(START):
		m_nDistanse += m_nAddDistance;

		if (m_nDistanse > m_fActive)
		{
			m_EffectTime = ACTIVE;
		}
		break;
	case(ACTIVE):
		m_nBuckTime--;
		if (m_nBuckTime < 0)
		{
			m_EffectTime = END;
		}
		break;
	case(END):
		m_nDistanse -= m_nAddDistance;
		if (m_nDistanse < 0)
		{
			m_bUninit = true;
		}

		break;
	}

	m_fAngle += m_fAddAngle;
	switch (m_PosPattern)
	{
	case(DEFULT):
		m_pos = D3DXVECTOR3(
			m_nDistanse * sinf(m_fRandAngle + m_fAngle) * cosf(m_fRandAngle2 + m_fAngle) + pos.x,
			m_nDistanse * cosf(m_fRandAngle + m_fAngle) + pos.y,
			m_nDistanse * sinf(m_fRandAngle + m_fAngle) * sinf(m_fRandAngle2 + m_fAngle) + pos.z);
		break;
	case(PLAYERTARGET):
		m_pos = D3DXVECTOR3(
			PlayerPos.x + m_nDistanse * sinf(m_fRandAngle + m_fAngle) * cosf(m_fRandAngle2 + m_fAngle),
			pos.y + m_nDistanse * cosf(m_fRandAngle + m_fAngle),
			PlayerPos.z + m_nDistanse * sinf(m_fRandAngle + m_fAngle) * sinf(m_fRandAngle2 + m_fAngle));
		break;
	case(ATHERTARGET):
		break;
	default:
		break;
	}


	if (m_nDistanse < 0)
	{
		m_bUninit = true;
	}

	CStraight3D::Create(m_pos,
		m_Size,
		m_AddSize,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		m_Color,
		m_MinColor,
		m_nTex, m_ParticleLife,
		CStraight3D::STRAIGHT, {}, m_nSynthetic,
		0.0f,
		(CStraight3D::RAND_PATTEN)0,
		(CStraight3D::POS_PATTERN)3,
		D3DXVECTOR2(0.0f, 0.0f),
		1.0f);

	m_nLife--;

	if (m_nLife < 0)
	{
		m_bUninit = true;
	}


	if (m_bUninit == true)
	{
		Uninit();
	}
}


//*****************************************************************************
//作成
//*****************************************************************************
CRotate3D *CRotate3D::Create(D3DXVECTOR3 SetSize,
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
	float fActive,
	POS_PATTERN PosPattern)
{
	CRotate3D * pRotate3D = NULL;
	pRotate3D = new CRotate3D(OBJTYPE::OBJTYPE_EFFECT);
	if (pRotate3D != NULL)
	{
		pRotate3D->Init(SetSize,
			pos, Tex,
			Size,
			AddSize,
			Color,
			AddColor,
			Distance,
			AddDistance,
			fAngle,
			fAddAngle,
			nTex, Synthetic, nLife,
			nParticleLife,
			nBuckTime,
			fActive,
			PosPattern);
	}
	return pRotate3D;
}

//----------------------------------------------
//ゲージの処理
//Author::林海斗
//--------------------------------------------
#include "main.h" 
#include "manager.h"
#include "renderer.h"
#include "gauge.h"
#include "fade.h"
#include "Polygon.h"
#include "gaugeber.h"

CGauge::CGauge(OBJTYPE nPriority) : CScene(nPriority)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_pGaugeBer[nCnt] = nullptr;
	}
	m_pFrame = nullptr;
}

CGauge::~CGauge()
{

}

CGauge *CGauge::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, const D3DXCOLOR& col, const float& MaxGauge, const int& nValue, const int& type)
{
	CGauge *pGauge = NULL;
	pGauge = new CGauge(OBJTYPE_GAUGE);
	pGauge->m_pGaugeBer[0] = CGaugeBer::Create(pos, scale, col, MaxGauge, nValue, type);
	pGauge->m_pGaugeBer[1] = CGaugeBer::Create(pos, scale, {1.0,1.0,0.0,1.0}, MaxGauge, nValue, type);
	pGauge->m_pGaugeBer[1]->ResetGauge();
	//ゲージの枠の生成
	if (!pGauge->m_pFrame)
	{
		pGauge->m_pFrame = CPolygon::Create({ pos.x + (MaxGauge / 2.0f),pos.y,0.0f }, { scale.x / 1.97f ,scale.y*1.1f,0.0f }, CTexture::GAUGEFRAME, {1.0,1.0,1.0,1.0}, OBJTYPE_GAUGEFRAME);
	}
	pGauge->Init();

	return pGauge;
}

HRESULT CGauge::Init(void)
{

	return S_OK;
}

void CGauge::Uninit()
{
	if (m_pFrame != nullptr)
	{
		m_pFrame->Uninit();
		m_pFrame = nullptr;
	}
}

void CGauge::Update()
{


}

void CGauge::Draw()
{
	
}
//--------------------------------------------------------------------
//ゲージのリセット
//--------------------------------------------------------------------
void CGauge::ResetGauge(int nGaugeType)
{
	m_pGaugeBer[nGaugeType]->ResetGauge();

}

//--------------------------------------------------------------------
//ゲージの減少用
//--------------------------------------------------------------------
void CGauge::SetGauge(float Set, int nGaugeType)
{
	m_pGaugeBer[nGaugeType]->SetGauge(Set);

}

//--------------------------------------------------------------------
//スケールのpos＋側だけ増やすため
//--------------------------------------------------------------------
void CGauge::SetScalePos(D3DXVECTOR3 pos, D3DXVECTOR3 scale, int AddType, int nGaugeType)
{
	m_pGaugeBer[nGaugeType]->SetScalePos(pos, scale, AddType);
}
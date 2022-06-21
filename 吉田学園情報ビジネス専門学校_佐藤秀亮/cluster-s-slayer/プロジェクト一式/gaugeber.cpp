//----------------------------------------------
//ゲージバーの処理
//Author::林海斗
//--------------------------------------------
#include "main.h" 
#include "manager.h"
#include "renderer.h"
#include "gaugeber.h"
#include "fade.h"
#include "Polygon.h"
CGaugeBer::CGaugeBer(OBJTYPE nPriority) : CScene2D(nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.0f,0.0f,0.0f };
	m_col = { 1.0,1.0,1.0,1.0 };
	m_fMaxGauge = 395.0f;
	m_AddType = L_ADD;
}

CGaugeBer::~CGaugeBer()
{

}

CGaugeBer *CGaugeBer::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, const D3DXCOLOR& col, const float& MaxGauge, const int& nValue, const int& type)
{
	CGaugeBer *pGauge = NULL;
	pGauge = new CGaugeBer(OBJTYPE_GAUGE);
	pGauge->m_pos = pos;
	pGauge->m_Scale = scale;
	pGauge->m_fMaxGauge = MaxGauge;
	pGauge->m_col = col;
	pGauge->Init();
	pGauge->BindTexture(CTexture::GAUGEBER);
	pGauge->m_fValueMax = nValue;
	pGauge->m_fValue = nValue;
	pGauge->m_AddType = (ADDTYPE)type;
	return pGauge;
}

HRESULT CGaugeBer::Init(void)
{
	CScene2D::Init();

	return S_OK;
}

void CGaugeBer::Uninit()
{
	CScene2D::Uninit();
	Release();
}

void CGaugeBer::Update()
{

	SetScalePos(m_pos, m_Scale, m_AddType);

	//ゲージが最大になったら最大値にする
	if (m_Scale.x >= m_fMaxGauge)
	{
		m_Scale.x = m_fMaxGauge;
	}
	//ゲージをマイナスまでいかせない
	else if (m_Scale.x <= 0.0f)
	{
		m_Scale.x = 0.0f;
	}
}

void CGaugeBer::Draw()
{
	CScene2D::Draw();
}
//--------------------------------------------------------------------
//ゲージのリセット
//--------------------------------------------------------------------
void CGaugeBer::ResetGauge()
{
	m_Scale.x = 0.0f;
	m_fValue = 0;

}

//--------------------------------------------------------------------
//ゲージの減少用
//--------------------------------------------------------------------
void CGaugeBer::SetGauge(float Set)
{
	m_fValue -= Set;
	float fAdd = (m_fMaxGauge * Set) / m_fValueMax;
	m_Scale.x -= fAdd;

	if (m_fValue >= m_fValueMax)
	{
		m_fValue = m_fValueMax;
		m_Scale.x = m_fMaxGauge;
	}
}

//--------------------------------------------------------------------
//スケールのpos＋側だけ増やすため
//--------------------------------------------------------------------
void CGaugeBer::SetScalePos(D3DXVECTOR3 pos, D3DXVECTOR3 scale, int AddType)
{

	CScene::SetPos(pos);
	CScene::SetScale(scale);

	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	switch (AddType)
	{
	case R_ADD:
		//バッファの生成
		pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - scale.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + scale.x, pos.y - scale.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + scale.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + scale.x, pos.y + scale.y, 0.0f);

		break;
	case L_ADD:
		//バッファの生成
		pVtx[0].pos = D3DXVECTOR3(pos.x - scale.x, pos.y - scale.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x, pos.y - scale.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - scale.x, pos.y + scale.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x, pos.y + scale.y, 0.0f);


		break;
	}
	pVtx[0].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[1].tex = D3DXVECTOR2((1.0f / m_fMaxGauge)*scale.x, 0.0);
	pVtx[2].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2((1.0f / m_fMaxGauge)*scale.x, 1.0);

	m_pVtxBuff->Unlock();

}
//*****************************************************************************
// (目標に向かって飛んでく)エフェクト処理 [MouseTracking.cpp]
// Author : 佐藤秀亮
//*****************************************************************************
#include "MouseTracking.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CMouseTracking::CMouseTracking(OBJTYPE nPriority) : CEffect2d(nPriority)
{
	m_move = {};
	m_Vec = {};
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CMouseTracking::~CMouseTracking()
{

}

//*****************************************************************************
//	初期化
//*****************************************************************************
HRESULT CMouseTracking::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size,
	D3DXVECTOR2 MinSize, int nLife, int nType,
	D3DXVECTOR3 Endpos, int Diffusion, int UninitVectl, int nSynthetic)
{
	CEffect2d::Init(pos, color, Mincolor, Size, MinSize, nLife, nType, nSynthetic);
	m_Endpos = pos;
	m_Vec = pos - Endpos;
	m_move = move;

	m_UninitVectl = UninitVectl;

	float randAngle = float(rand() % Diffusion) - float(rand() % Diffusion);
	randAngle /= 100.0f;

	m_fAngle = (float)atan2(m_Vec.x, m_Vec.y);		//角度

	m_fAngle += randAngle;


	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CMouseTracking::Uninit()
{
	CEffect2d::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CMouseTracking::Update()
{
	D3DXVECTOR3 pos = GetPos();

	m_Vec = pos - m_Endpos;

	m_Vectl = sqrtf(m_Vec.x * m_Vec.x + m_Vec.y *  m_Vec.y);

	if (m_Vectl >= m_UninitVectl)
	{
		m_bUninit = true;
	}

	pos += D3DXVECTOR3(sinf(m_fAngle) * -m_move.x, cosf(m_fAngle) * -m_move.x, 0.0f);

	m_Size += m_MinSize;

	//適応
	CScene2D::SetPos(pos, D3DXVECTOR3(m_Size.x, m_Size.y, {}));

	CEffect2d::Update();
}

//*****************************************************************************
//描画
//*****************************************************************************
void CMouseTracking::Draw()
{
	CEffect2d::Draw();
}

//*****************************************************************************
//作成
//*****************************************************************************
CMouseTracking *CMouseTracking::Create(
	D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size,
	D3DXVECTOR2 MinSize, int nLife, int nType,
	D3DXVECTOR3 Endpos, int Diffusion, int UninitVectl, int nSynthetic)
{
	CMouseTracking *pMouseTracking = NULL;
	pMouseTracking = new CMouseTracking(CScene::OBJTYPE_EFFECT);		//メモリ確保
										//NULLチェック
	if (pMouseTracking != NULL)
	{
		pMouseTracking->Init(pos, move, color, Mincolor, Size, MinSize, nLife, nType, Endpos,Diffusion, UninitVectl, nSynthetic);
	}

	return pMouseTracking;
}
